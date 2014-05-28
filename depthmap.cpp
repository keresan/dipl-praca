#include "depthmap.h"

/**
 * @brief Constructor. Initialize area for computing depthmap.
 * @param face Model of face
 * @param topLeft Top Left point of interested area
 * @param bottomRight Bottom right point of interested area
 * @param pixelsX Pixels resolution in axis X
 * @param piselxY Pixels resolution in axis Y
 */
DepthMap::DepthMap(Mesh &face, cv::Point2d topLeft, cv::Point2d bottomRight, int pixelsX, int piselxY) {

	depthMap = cv::Mat(piselxY,pixelsX,CV_32F, cv::Scalar(Common::depthmapInitValue));
	this->topLeft = topLeft;
	this->bottomRight = bottomRight;

	createDepthMap(face);
}

/**
 * @brief Choose point from depthmap, who are between topLeft and Bottom right
 * @param tl Top left point
 * @param br Bottom Right point
 * @param vector Vector of points who are in interested area
 * @return Number of picked up points
 */
int DepthMap::selectFromDepthMap(cv::Point2d tl, cv::Point2d br, QVector<cv::Point2f> &vector) {

	int tlR, tlC, brR,brC;
	bool resultTL, resultBR;

	resultTL = mapPointToIndecies(tl,tlR, tlC);
	resultBR = mapPointToIndecies(br,brR, brC);

	if(resultTL && !resultBR) {
		//TL is in, BR is out => shift BR to border
		if(br.x > bottomRight.x) {
			br.x = bottomRight.x;
		}
		if(br.y < bottomRight.y) {
			br.y = bottomRight.y;
		}
		//compute BR again
		if(! mapPointToIndecies(br,brR, brC)) {
			return -1;
		}
	} else if(!resultTL && resultBR) {
		//TL is out, BR is in => shift TL to border
		if(tl.x < topLeft.x) {
			tl.x = topLeft.x;
		}
		if(tl.y > topLeft.y) {
			tl.y = topLeft.y;
		}
		//compute TL again
		if(! mapPointToIndecies(tl,tlR, tlC)) {
			return -1;
		}

	} else if(!resultTL && !resultBR) {
		//TL is out, BR is out => skip it
		return -1;
	}

	//qDebug("tl: [%8.3f;%8.3f] -> [%5dx%5d]", tl.x, tl.y, tlR, tlC);
	//qDebug("br: [%8.3f;%8.3f] -> [%5dx%5d]", br.x, br.y, brR, brC);


	//iterate row by row and pick up points
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
			}
		}
	}
	return counter;
}

/**
 * @brief Map row and col indecies to point in depthmap.
 * @param row Index of row
 * @param col Index of col
 * @param p Point to map
 * @return True if point is mapped correctly
 */
bool DepthMap::mapIndeciesToPoint(int row, int col, cv::Point2f &p) {

	//check
	if(row < 0 || row > depthMap.rows) {
		return FALSE;
	}
	if(col < 0 || col > depthMap.cols) {
		return FALSE;
	}


	float xx = topLeft.x - (topLeft.x - bottomRight.x)/(float)(depthMap.cols-1) * col;
	float yy = topLeft.y - (topLeft.y - bottomRight.y)/(float)(depthMap.rows-1) * row;


	p = cv::Point2f(xx,yy);

	return TRUE;
}


/**
 * @brief Map point position to row and col indecies.
 * @param p Point to map
 * @param row Index of row
 * @param col Index of col
 * @return True if indecies are mapped correctly
 */
bool DepthMap::mapPointToIndecies(cv::Point2d p, int &row, int &col) {

	//check
	if(p.x < this->topLeft.x || p.y > this->topLeft.y) {
		return FALSE;
	}
	if(p.x > this->bottomRight.x || p.y < this->bottomRight.y) {
		return FALSE;
	}

	//map
	col = abs(qRound((p.x - topLeft.x) /( qAbs(topLeft.x - bottomRight.x)/(float)(depthMap.cols-1))));
	row = abs(qRound((p.y - topLeft.y) /( qAbs(topLeft.y - bottomRight.y)/(float)(depthMap.rows-1))));

	return TRUE;
}

/**
 * @brief Print points of depthmap
 */
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
 * @brief Compute depth of point using weighted aritmetic mean
 * @param x X coord of point with unknown depth
 * @param y Y coord of point with unknown depth
 * @param x0 Point 0
 * @param y0 Point 0
 * @param z0 Point 0
 * @param x1 Point 1
 * @param y1 Point 1
 * @param z1 Point 1
 * @param x2 Point 2
 * @param y2 Point 2
 * @param z2 Point 2
 * @return Computed depth value
 */
float DepthMap::weightedArtMean(float x, float y, float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2) {

	float dist0 = sqrt((x0-x)*(x0-x) + (y0-y)*(y0-y));
	float dist1 = sqrt((x1-x)*(x1-x) + (y1-y)*(y1-y));
	float dist2 = sqrt((x2-x)*(x2-x) + (y2-y)*(y2-y));

	float z = (1/dist0*z0 + 1/dist1*z1 + 1/dist2*z2) / (1/dist0 + 1/dist1 + 1/dist2);

	return z;
}

/**
 * @brief Compute depth of point using linear interpolation
 * @param x X coord of point with unknown depth
 * @param y Y coord of point with unknown depth
 * @param x0 Point 0
 * @param y0 Point 0
 * @param z0 Point 0
 * @param x1 Point 1
 * @param y1 Point 1
 * @param z1 Point 1
 * @param x2 Point 2
 * @param y2 Point 2
 * @param z2 Point 2
 * @return Computed depth value
 */
float DepthMap::linearInterpolation(float x, float y, float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2) {
	if (x0 == x1 && y0 == y1 && x0 == x2 && y0 == y2) {
		return (z0+z1+z2)/3;
	}

	// Ax + By + Cz + D = 0
	float A = y0*(z1 - z2) + y1*(z2 - z0) + y2*(z0 - z1);
	float B = z0*(x1 - x2) + z1*(x2 - x0) + z2*(x0 - x1);
	float C = x0*(y1 - y2) + x1*(y2 - y0) + x2*(y0 - y1);
	float D = -A*x0 - B*y0 - C*z0;

	float result = -(A/C)*x - (B/C)*y - D/C;
	if (result != result) {
		return (z0+z1+z2)/3.0;
	}
	return result;
}


/**
 * @brief Create depthmap from model of face.
 * @param face Model of face
 */
void DepthMap::createDepthMap(Mesh &face) {

	//allocated space in advace, speed up computing about 10 ms
	QVector<cv::Point2f> vector(100);

	for(int i = 0; i < face.triangles.count(); i++) {

		//create points from indecies
		int ind0 = face.triangles.at(i)[0];
		int ind1 = face.triangles.at(i)[1];
		int ind2 = face.triangles.at(i)[2];

		float p0x = face.pointsMat(ind0,0);
		float p0y = face.pointsMat(ind0,1);
		float p1x = face.pointsMat(ind1,0);
		float p1y = face.pointsMat(ind1,1);
		float p2x = face.pointsMat(ind2,0);
		float p2y = face.pointsMat(ind2,1);

		std::vector<cv::Point2f> contour(3);
		contour[0] = cv::Point2f(p0x,p0y);
		contour[1] = cv::Point2f(p1x,p1y);
		contour[2] = cv::Point2f(p2x,p2y);

		//compute bounging rect
		float tlx = min(p0x, p1x, p2x);
		float tly = max(p0y, p1y, p2y);

		float brx = max(p0x, p1x, p2x);
		float bry = min(p0y, p1y, p2y);

		//qDebug("rectangle: [%8d %8d][%8d %8d]", rectangle.tl().x,rectangle.tl().y,rectangle.br().x,rectangle.br().y);

		//select points from depth map
		int vectorSize = selectFromDepthMap(cv::Point2d(tlx,tly),cv::Point2d(brx,bry), vector);

		for(int j = 0; j < vectorSize; j++) {
			int result = cv::pointPolygonTest(contour, vector[j], false);

			if(result >= 0) {

				//depth by linear inerpolation
				float z = linearInterpolation(vector.at(j).x, vector.at(j).y,
										   p0x, p0y, face.pointsMat(ind0,2),
										   p1x, p1y, face.pointsMat(ind1,2),
										   p2x, p2y, face.pointsMat(ind2,2));

				//depth by weighted mean
				/*
				float z = weightedArtMean(vector.at(j).x, vector.at(j).y,
											p0x, p0y, face.pointsMat(ind0,2),
											p1x, p1y, face.pointsMat(ind1,2),
											p2x, p2y, face.pointsMat(ind2,2));
				*/
				 int col, row;
				 if(mapPointToIndecies(vector.at(j),row,col)) {
					 if (z > depthMap.at<float>(row, col)) {
						 depthMap.at<float>(row, col) = z;

					 }
				 }
			 }
		 }
	 }
}

/**
 * @brief Show depthmap with all landmarks.
 * @param depthMap Depthmap to show
 * @param landmarks
 * @param label
 * @param addTime
 */
void DepthMap::showAllLandmarks(const cv::Mat &depthMap, Landmarks &landmarks, QString label, bool addTime) {
	VectorOfLandmarks landmarksVector = landmarks.getLandmarks();

	cv::Mat normalizeMap = Common::norm_0_255(depthMap);
	cv::Mat colorMap;
	cvtColor(normalizeMap, colorMap, CV_GRAY2RGB);

	int thickness = -1;
	int lineType = 8;
	float diameter = 1.0;

	for(int i =0; i < landmarksVector.size(); i++) {
		if(landmarksVector.at(i).second) {
			cv::circle(colorMap,landmarksVector.at(i).first, diameter, cv::Scalar( 0, 0, 255 ), thickness, lineType );
		}
	}
	if(addTime) {
		cv::imshow(label.toStdString() +" "+ QTime::currentTime().toString("hh:mm:ss.zzz").toStdString(), colorMap);
	} else {
		cv::imshow(label.toStdString(), colorMap);
	}

}
