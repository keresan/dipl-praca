#include "depthmap.h"

DepthMap::DepthMap() {

	depthMap = cv::Mat(200,200,CV_32F, cv::Scalar(-999));

    topLeft = cv::Point2d(-50,-50);
    bottomRight = cv::Point2d(50,50);
}


double DepthMap::averageValueInTraignle(cv::Point2f queryPoint, Mesh &face) {
    for(int i = 0; i < face.triangles.count(); i++) {

        /*
        //create points from indecies
        int ind1 = face.triangles.at(i)[0];
        int ind2 = face.triangles.at(i)[1];
        int ind3 = face.triangles.at(i)[2];

        std::vector<cv::Point2f> contour(3);
        contour[0] = cv::Point2f(face.pointsMat(ind1,0),face.pointsMat(ind1,1));
        contour[1] = cv::Point2f(face.pointsMat(ind2,0),face.pointsMat(ind2,1));
        contour[2] = cv::Point2f(face.pointsMat(ind3,0),face.pointsMat(ind3,1));
        */

        std::vector<cv::Point2f> contour(3);

        contour[0] = cv::Point2f(-5.2,5.34);
        contour[1] = cv::Point2f(-3.42,-4.34);
        contour[2] = cv::Point2f(2.05,3.5);

        cv::Rect rectangle;

        rectangle = cv::boundingRect(contour);

        std::cout << "contours: "<< contour << std::endl;
        std::cout <<"rectangle: "<< rectangle << std::endl;
        std::cout << rectangle.x << " "<< rectangle.y << std::endl;
        std::cout << "w: "<<rectangle.width << ",h: "<< rectangle.height << std::endl;
        std::cout << "tl: "<<rectangle.tl() << ",br: "<< rectangle.br() << std::endl;

        break;

        int result = cv::pointPolygonTest(contour, queryPoint, false);

        if(result >= 0) {

            //qDebug() << contour[0].x << contour[0].y;
            //qDebug() << contour[1].x << contour[1].y;
           //qDebug() << contour[2].x << contour[2].y;

        }


    }

    return -1.0;
}

int DepthMap::selectFromDepthMap(cv::Point2d tl, cv::Point2d br, QVector<cv::Point2f> &vector) {


    int tlR, tlC, brR,brC;

	if(! mapPointToIndecies(tl,tlR, tlC)) {
		 return -1;
    }

	if(! mapPointToIndecies(br,brR, brC)) {
		 return -1;
    }

	//qDebug("tl: [%8.3f %8.3f] -> [%5d %5d]", tl.x, tl.y, tlR, tlC);
	//qDebug("br: [%8.3f %8.3f] -> [%5d %5d]", br.x, br.y, brR, brC);

	int counter = -1;
    for(int r = tlR; r <= brR; r++) {
		for(int c = tlC; c <= brC; c++) {
            cv::Point2f resultPoint;
            if(mapIndeciesToPoint(r,c,resultPoint)) {
				counter++;
				if(counter >= vector.size()) {
					vector.append(resultPoint);
				} else {
					vector[counter] = resultPoint;
				}


				//vector.append(resultPoint);
            }

        }
    }

	return counter
	;

}

void DepthMap::createDepthMap(Mesh &face) {

     for(int i = 0; i < face.triangles.count(); i++) {

         //get triangle points
         /*
         //create points from indecies
         int ind1 = face.triangles.at(i)[0];
         int ind2 = face.triangles.at(i)[1];
         int ind3 = face.triangles.at(i)[2];

         std::vector<cv::Point2f> contour(3);
         contour[0] = cv::Point2f(face.pointsMat(ind1,0),face.pointsMat(ind1,1));
         contour[1] = cv::Point2f(face.pointsMat(ind2,0),face.pointsMat(ind2,1));
         contour[2] = cv::Point2f(face.pointsMat(ind3,0),face.pointsMat(ind3,1));
         */

         std::vector<cv::Point2f> contour(3);

         contour[0] = cv::Point2f(-5,5);
         contour[1] = cv::Point2f(-3,-4);
         contour[2] = cv::Point2f(2,3);

         //get bounging rect
         cv::Rect rectangle;
         rectangle = cv::boundingRect(contour);

         //get points from depth map in bounding rect



     }


    /*
    for(int r = 0; r < depthMap.rows ; r++) {
         for(int c = 0; c < depthMap.cols; c++) {

             cv::Point2f queryPoint = mapIndeciesToPoint(r,c);

             int result = averageValueInTraignle(queryPoint, face);


             //printf("[%5.1f %5.1f]", point.x, point.y);
         }

    }
    */
}


bool DepthMap::mapIndeciesToPoint(int row, int col, cv::Point2f &p) {

    //check
    if(row < 0 || row > depthMap.rows) {
        return FALSE;
    }
    if(col < 0 || col > depthMap.cols) {
        return FALSE;
    }

    float cc = topLeft.x - (topLeft.x - bottomRight.x)/(depthMap.cols-1) * col;
    float rr = topLeft.y - (topLeft.y - bottomRight.y)/(depthMap.rows-1) * row;

    p = cv::Point2f(rr,cc);

    return TRUE;
}

bool DepthMap::mapPointToIndecies(cv::Point2d p, int &row, int &col) {

    //check
    if(p.x < this->topLeft.x || p.y < this->topLeft.y) {
        return FALSE;
    }
    if(p.x > this->bottomRight.x || p.y > this->bottomRight.y) {
        return FALSE;
    }

    row = abs(qRound((p.x - topLeft.x) /( (topLeft.x - bottomRight.x)/(depthMap.cols-1))));
    col = abs(qRound((p.y - topLeft.y) /( (topLeft.y - bottomRight.y)/(depthMap.rows-1))));

    return TRUE;

}

void DepthMap::printPoints() {
    for(int r = 0; r < depthMap.rows ; r++) {
         for(int c = 0; c < depthMap.cols; c++) {

             cv::Point2f queryPoint;
             if( mapIndeciesToPoint(r,c,queryPoint)) {
                 printf("[%5.1f %5.1f]", queryPoint.x, queryPoint.y);
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

	if(x == x0 && y == y0) {
		return z0;
	}

	if(x == x1 && y == y1) {
		return z1;
	}

	if(x == x2 && y == y2) {
		return z2;
	}

	double dist0 = sqrt((x0-x)*(x0-x) + (y0-y)*(y0-y));
	double dist1 = sqrt((x1-x)*(x1-x) + (y1-y)*(y1-y));
	double dist2 = sqrt((x2-x)*(x2-x) + (y2-y)*(y2-y));

	double z = (1/dist0*z0 + 1/dist1*z1 + 1/dist2*z2) / (1/dist0 + 1/dist1 + 1/dist2);

	return z;

}

void DepthMap::test1(Mesh &face) {

    qDebug() << "triangles:" << face.triangles.count();

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
		double tly = min(p0y, p1y, p2y);

		double brx = max(p0x, p1x, p2x);
		double bry = max(p0y, p1y, p2y);

		//qDebug("====triangle: [%8.3f %8.3f][%8.3f %8.3f][%8.3f %8.3f]",contour[0].x ,contour[0].y,contour[1].x,contour[1].y,contour[2].x,contour[2].y);

		//qDebug("rectangle: [%8d %8d][%8d %8d]", rectangle.tl().x,rectangle.tl().y,rectangle.br().x,rectangle.br().y);
		//qDebug("own      : [%8.3f %8.3f][%8.3f %8.3f]", tlx,tly,brx,bry);





        //select points from depth map
		int vectorSize = selectFromDepthMap(cv::Point2d(tlx,tly),cv::Point2d(brx,bry), vector);
		for(int j = 0; j < vectorSize; j++) {
			int result = cv::pointPolygonTest(contour, vector[j], false);
			if(result >= 0) {
                 //vazeny priemer, ale cim mensia vzdialenost, tym vacsia vaha !!
				 double z = weightedArtMean(vector.at(j).x, vector.at(j).y,
											p0x, p0y, face.pointsMat(ind0,2),
											p1x, p1y, face.pointsMat(ind1,2),
											p2x, p2y, face.pointsMat(ind2,2));

                 int col, row;
				 if(mapPointToIndecies(vector.at(j),row,col)) {
                     //qDebug() << row << col;
                     //qDebug() << depthMap.at<double>(row, col);
                     if (depthMap.at<double>(row, col) < z) {
                         depthMap.at<double>(row, col) = z;
                     }
                 }
             }

         }
		 /*
		 if(vectorSize > 700) {

			 qDebug("====triangle: [%8.3f %8.3f][%8.3f %8.3f][%8.3f %8.3f]",contour[0].x ,contour[0].y,contour[1].x,contour[1].y,contour[2].x,contour[2].y);
			 qDebug("own      : [%8.3f %8.3f][%8.3f %8.3f]", tlx,tly,brx,bry);
			 qDebug() << vectorSize;
			 qDebug() << counter;
		 }
		 */
     }

	Common::printMatrix(depthMap);

}
