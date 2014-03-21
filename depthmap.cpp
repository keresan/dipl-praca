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

void DepthMap::selectFromDepthMap(cv::Rect rectangle, QVector<cv::Point2f> &vector) {

    //qDebug() << "hu" << rectangle.tl().x << rectangle.tl().y;
    //qDebug() << "hu" << rectangle.br().x << rectangle.br().y;
    int tlR, tlC, brR,brC;

    if(! mapPointToIndecies(rectangle.tl(),tlR, tlC)) {
         return;
    }

    if(! mapPointToIndecies(rectangle.br(),brR, brC)) {
         return;
    }

    for(int r = tlR; r <= brR; r++) {
        for(int c = tlR; c <= brC; c++) {
            cv::Point2f resultPoint;
            if(mapIndeciesToPoint(r,c,resultPoint)) {
                vector.append(resultPoint);
            }

        }
    }

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

void DepthMap::test1(Mesh &face) {

    qDebug() << "triangles:" << face.triangles.count();

    for(int i = 0; i < face.triangles.count(); i++) {

        //create points from indecies
        int ind0 = face.triangles.at(i)[0];
        int ind1 = face.triangles.at(i)[1];
        int ind2 = face.triangles.at(i)[2];

        std::vector<cv::Point2f> contour(3);
        contour[0] = cv::Point2f(face.pointsMat(ind0,0),face.pointsMat(ind0,1));
        contour[1] = cv::Point2f(face.pointsMat(ind1,0),face.pointsMat(ind1,1));
        contour[2] = cv::Point2f(face.pointsMat(ind2,0),face.pointsMat(ind2,1));


        //get bounging rect
        cv::Rect rectangle;
        rectangle = cv::boundingRect(contour);

        //select points from depth map
        QVector<cv::Point2f> vector;
        selectFromDepthMap(rectangle, vector);

        /*
        if(vector.count() > 0) {
            qDebug() << vector.count();
        }
        */

         for(int j = 0; j < vector.count(); j++) {
             int result = cv::pointPolygonTest(contour, vector.at(j), false);

             if(result >= 0) {
                 //qDebug("====triangel: [%8.3f %8.3f][%8.3f %8.3f][%8.3f %8.3f]",contour[0].x,contour[0].y,contour[1].x,contour[1].y,contour[2].x,contour[2].y);

                 //qDebug("[%8.3f %8.3f]",vector.at(j).x, vector.at(j).y);
                 //compute distances to triangle points
                 double dist0 = sqrt((contour[0].x-vector.at(j).x)*(contour[0].x-vector.at(j).x) + (contour[0].y-vector.at(j).y)*(contour[0].y-vector.at(j).y));
                 double dist1 = sqrt((contour[1].x-vector.at(j).x)*(contour[1].x-vector.at(j).x) + (contour[1].y-vector.at(j).y)*(contour[1].y-vector.at(j).y));
                 double dist2 = sqrt((contour[2].x-vector.at(j).x)*(contour[2].x-vector.at(j).x) + (contour[2].y-vector.at(j).y)*(contour[2].y-vector.at(j).y));

                 /*
                 if (dist0 > 0) {
                    qDebug() << "dist0:" << dist0 << 1/dist0 << face.pointsMat(ind0,2);
                    qDebug() << "dist1:" << dist1 << 1/dist1 << face.pointsMat(ind1,2);
                    qDebug() << "dist2:" << dist2 << 1/dist2 << face.pointsMat(ind2,2);
                 }
                 */

                 //vazeny priemer, ale cim mensia vzdialenost, tym vacsia vaha !!

                 double z = (1/dist0*face.pointsMat(ind0,2) + 1/dist1*face.pointsMat(ind1,2) + 1/dist2*face.pointsMat(ind2,2)) / (1/dist0 + 1/dist1 + 1/dist2);

                 //qDebug() << "z: " << z;


                 //qDebug() << z;

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
     }

    //Common::printMatrix(depthMap);

}
