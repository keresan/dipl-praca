#include "depthmap.h"

DepthMap::DepthMap(Mesh &face, cv::Point2d topLeft, cv::Point2d bottomRight, int pixelsX, int piselxY) {

	depthMap = cv::Mat(piselxY,pixelsX,CV_64F, cv::Scalar(-999.0));
	_topLeft = topLeft;
	_bottomRight = bottomRight;

	createDepthMap(face);

	computeMinMax();
	qDebug() <<"min max:" << _min << _max;

	policing(_min,_max,false);

}

int DepthMap::selectFromDepthMap(cv::Point2d tl, cv::Point2d br, QVector<cv::Point2f> &vector) {


    int tlR, tlC, brR,brC;

	if(! mapPointToIndecies(tl,tlR, tlC)) {
		 return -1;
    }

	if(! mapPointToIndecies(br,brR, brC)) {
		 return -1;
    }


	//qDebug("tl: [%8.3f;%8.3f] -> [%5dx%5d]", tl.x, tl.y, tlR, tlC);
	//qDebug("br: [%8.3f;%8.3f] -> [%5dx%5d]", br.x, br.y, brR, brC);


	int counter = 0;
    for(int r = tlR; r <= brR; r++) {
		for(int c = tlC; c <= brC; c++) {
            cv::Point2f resultPoint;
            if(mapIndeciesToPoint(r,c,resultPoint)) {

				if(counter >= vector.size()) {
					vector.append(resultPoint);
				} else {
					vector[counter] = resultPoint;
				}
				counter++;

				//vector.append(resultPoint);
            }

        }
    }

	return counter
	;

}




bool DepthMap::mapIndeciesToPoint(int row, int col, cv::Point2f &p) {

    //check
    if(row < 0 || row > depthMap.rows) {
        return FALSE;
    }
    if(col < 0 || col > depthMap.cols) {
        return FALSE;
    }


	double xx = _topLeft.x - (_topLeft.x - _bottomRight.x)/(depthMap.cols-1) * col;
	double yy = _topLeft.y - (_topLeft.y - _bottomRight.y)/(depthMap.rows-1) * row;

	p = cv::Point2f(xx,yy);

    return TRUE;
}

bool DepthMap::mapPointToIndecies(cv::Point2d p, int &row, int &col) {

    //check
	if(p.x < this->_topLeft.x || p.y > this->_topLeft.y) {
        return FALSE;
    }
	if(p.x > this->_bottomRight.x || p.y < this->_bottomRight.y) {
        return FALSE;
    }


	/*
	qDebug("====[%8.2f;%8.2f]",p.x,p.y);
	qDebug() << "size: " << abs(topLeft.x - bottomRight.x) <<  " x " <<abs(topLeft.y - bottomRight.y);
	qDebug() << "dephMap RxC: " << depthMap.rows << depthMap.cols;
	double stepX = abs(topLeft.x - bottomRight.x) / (double)(depthMap.cols-1);
	double stepY = abs(topLeft.y - bottomRight.y) / (double)(depthMap.rows-1);
	qDebug("step: %5.2f;%5.2f",stepX,stepY);

	qDebug() << "rozdiel od tl.x:" << p.x - topLeft.x;
	qDebug() << "rozdiel od tl.y:" << p.y - topLeft.y;
	*/

	col = abs(qRound((p.x - _topLeft.x) /( abs(_topLeft.x - _bottomRight.x)/(double)(depthMap.cols-1))));
	row = abs(qRound((p.y - _topLeft.y) /( abs(_topLeft.y - _bottomRight.y)/(double)(depthMap.rows-1))));

	//qDebug() << "RxS:" << row << col;

    return TRUE;

}

void DepthMap::printPoints() {
    for(int r = 0; r < depthMap.rows ; r++) {
         for(int c = 0; c < depthMap.cols; c++) {

             cv::Point2f queryPoint;
             if( mapIndeciesToPoint(r,c,queryPoint)) {
				 printf("[%6.2f;%6.2f]", queryPoint.x, queryPoint.y);
             } else {
                 printf("= = = = = =");
             }

         }
         printf("\n");
    }
}

/**
 * @brief DepthMap::weightedArtMean - weighted aritmetic mean
 * @param x
 * @param y
 * @param x0
 * @param y0
 * @param z0
 * @param x1
 * @param y1
 * @param z1
 * @param x2
 * @param y2
 * @param z2
 * @return
 */
double DepthMap::weightedArtMean(double x, double y, double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2) {

	double dist0 = sqrt((x0-x)*(x0-x) + (y0-y)*(y0-y));
	double dist1 = sqrt((x1-x)*(x1-x) + (y1-y)*(y1-y));
	double dist2 = sqrt((x2-x)*(x2-x) + (y2-y)*(y2-y));

	double z = (1/dist0*z0 + 1/dist1*z1 + 1/dist2*z2) / (1/dist0 + 1/dist1 + 1/dist2);

	return z;
}

double DepthMap::linearInterpolation(double x, double y, double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2) {
	if (x0 == x1 && y0 == y1 && x0 == x2 && y0 == y2) {
		return (z0+z1+z2)/3;
	}

	// Ax + By + Cz + D = 0
	double A = y0*(z1 - z2) + y1*(z2 - z0) + y2*(z0 - z1);
	double B = z0*(x1 - x2) + z1*(x2 - x0) + z2*(x0 - x1);
	double C = x0*(y1 - y2) + x1*(y2 - y0) + x2*(y0 - y1);
	double D = -A*x0 - B*y0 - C*z0;

	double result = -(A/C)*x - (B/C)*y - D/C;
	if (result != result) {
		return (z0+z1+z2)/3;
	}
	return result;
}


/**
 * @brief DepthMap::createDepthMap
 * @param face
 * pozn: okraj hlbkovej mapy sa nemusi spravne vytvorit, pretoze trojuholniky, ktore ju presahuju sa preskocia
 */
void DepthMap::createDepthMap(Mesh &face) {

	//qDebug() << "depthmap triangles:" << face.triangles.count();

	//dopredu alokovany priestor, zrychluje vypocet o cca 10 ms
	QVector<cv::Point2f> vector(100);

	for(int i = 0; i < face.triangles.count(); i++) {

        //create points from indecies
        int ind0 = face.triangles.at(i)[0];
        int ind1 = face.triangles.at(i)[1];
        int ind2 = face.triangles.at(i)[2];

		double p0x = face.pointsMat(ind0,0);
		double p0y = face.pointsMat(ind0,1);
		double p1x = face.pointsMat(ind1,0);
		double p1y = face.pointsMat(ind1,1);
		double p2x = face.pointsMat(ind2,0);
		double p2y = face.pointsMat(ind2,1);

        std::vector<cv::Point2f> contour(3);
		contour[0] = cv::Point2f(p0x,p0y);
		contour[1] = cv::Point2f(p1x,p1y);
		contour[2] = cv::Point2f(p2x,p2y);


        //get bounging rect
	   // cv::Rect rectangle;
	   // rectangle = cv::boundingRect(contour);

		double tlx = min(p0x, p1x, p2x);
		double tly = max(p0y, p1y, p2y);

		double brx = max(p0x, p1x, p2x);
		double bry = min(p0y, p1y, p2y);



		//qDebug("rectangle: [%8d %8d][%8d %8d]", rectangle.tl().x,rectangle.tl().y,rectangle.br().x,rectangle.br().y);

        //select points from depth map
		int vectorSize = selectFromDepthMap(cv::Point2d(tlx,tly),cv::Point2d(brx,bry), vector);

		for(int j = 0; j < vectorSize; j++) {
			int result = cv::pointPolygonTest(contour, vector[j], false);

			if(result >= 0) {


				//vazeny priemer vs linearna interpolacia: vysledky sa lisia v stotinach
				double z = linearInterpolation(vector.at(j).x, vector.at(j).y,
										   p0x, p0y, face.pointsMat(ind0,2),
										   p1x, p1y, face.pointsMat(ind1,2),
										   p2x, p2y, face.pointsMat(ind2,2));
                 //vazeny priemer, ale cim mensia vzdialenost, tym vacsia vaha !!
				/*
				double z = weightedArtMean(vector.at(j).x, vector.at(j).y,
											p0x, p0y, face.pointsMat(ind0,2),
											p1x, p1y, face.pointsMat(ind1,2),
											p2x, p2y, face.pointsMat(ind2,2));
				*/
                 int col, row;
				 if(mapPointToIndecies(vector.at(j),row,col)) {
                     //qDebug() << row << col;
					 //qDebug() << depthMap.at<double>(row, col);
					 if (z > depthMap.at<double>(row, col)) {
						 depthMap.at<double>(row, col) = z;

                     }
                 }
             }
         }
     }

	//Common::printMatrix(depthMap);
}


void DepthMap::policing(double min, double max, bool shiftToZero) {

	for (int r = 0; r < depthMap.rows; r++) {
		for (int c = 0; c < depthMap.cols; c++) {
			double z = depthMap.at<double>(r, c);

			if(z <= min) {
				//z = min;
				depthMap.at<double>(r, c) = min;
			} else if(z >= max) {
				//z = max;
				depthMap.at<double>(r, c) = max;
				//qDebug() << z;
			}

			if(shiftToZero) {
				z += abs(min) + 1;
			}

			//depthMap.at<double>(r, c) = z;

			/*
			else {
				depthMap.at<double>(r, c) =  (z+abs(min))/255.0;//(abs(min)+abs(max));

				qDebug() <<r <<c << ":"<< z << "->" << depthMap.at<double>(r, c);
			}
			*/

		}

	}
}

void DepthMap::computeMinMax() {

	//nemoze byt priamo na urcity prvok, pretoze ten moze obsahovat prazdne pole
	_min = 100;
	_max = -100;


	for (int r = 0; r < depthMap.rows; r++) {
		for (int c = 0; c < depthMap.cols; c++) {
				double z = depthMap.at<double>(r, c);

				if( z > _max) {
					_max = z;
				}
				if(z < _min && z != -999.0) {
					_min = z;
				}

				//if(z > -0.1 && z < 0.1) {
					//qDebug() << r << c << " -> "<< z;
				//}

		}
	}

}
