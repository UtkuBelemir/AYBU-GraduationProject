#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;

int main(int argc, char **argv) {
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "Error while connecting camera quiting...." << endl;
        return -1;
    }
    while (true) {
        cv::Mat imgOriginal;
        bool bSuccess = cap.read(imgOriginal);
        if (!bSuccess)
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }

        cv::Mat imgHSV;
        //imgOriginal = cv::imread("/Users/utkubelemir/Desktop/opencv-color-1/traffic2.jpg", CV_LOAD_IMAGE_COLOR);   // Read the file
        cv::cvtColor(imgOriginal, imgHSV, cv::COLOR_BGR2HSV);
        cv::Mat colorLowerBound;
        cv::Mat colorUpperBound;
        cv::Mat contouredImage;
        cv::Mat thresholdedImg;
        cv::Mat tempIMG = imgOriginal;
        /*cv::inRange(imgHSV, cv::Scalar(20, 100, 100), cv::Scalar(30, 255, 255), colorLowerBound);
        cv::inRange(imgHSV, cv::Scalar(20, 100, 100), cv::Scalar(30, 255, 255), colorUpperBound);*/
        cv::inRange(imgHSV, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), colorLowerBound);
        cv::inRange(imgHSV, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), colorUpperBound);
        /*cv::inRange(imgHSV, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), colorUpperBound);*/
        cv::addWeighted(colorLowerBound,1.0,colorUpperBound,1.0,0.0,thresholdedImg);
        cv::Size ignoreSize = cv::Size(1, 1);
        cv::morphologyEx(thresholdedImg, thresholdedImg, cv::MORPH_OPEN,
                         cv::getStructuringElement(cv::MORPH_ELLIPSE, ignoreSize));
        cv::morphologyEx(thresholdedImg, thresholdedImg, cv::MORPH_CLOSE,
                         cv::getStructuringElement(cv::MORPH_ELLIPSE, ignoreSize));
        std::vector<std::vector<cv::Point> > contours;
        std::vector<std::vector<cv::Point> > exampleContours;
        cv::Mat contourOutput = thresholdedImg.clone();
        //cv::Mat exCont = cv::imread("/Users/utkubelemir/Desktop/opencv-color-1/cakmak.png",CV_LOAD_IMAGE_GRAYSCALE);
        cv::findContours( contourOutput, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
        //cv::findContours( exCont, exampleContours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
        cv::Mat exampleContourImage(cv::Size(1280,720), CV_8UC3, cv::Scalar(0,0,0));
        cv::drawContours(exampleContourImage,exampleContours,-1,cv::Scalar(255,255,255),1);
        cv::Mat forX,forY;
        //cv::matchTemplate(contourOutput,exCont,forX,CV_TM_CCORR_NORMED);
        //cv::matchTemplate(contourOutput,exCont,forY,CV_TM_CCOEFF);

        cv::Point xMin, xMax,yMin,yMax;
        double tempMin, tempMax;
        cv::minMaxLoc(forX, &tempMin, &tempMax, &xMin,&xMax);
        cv::minMaxLoc(forY, &tempMin, &tempMax, &yMin,&yMax);
        cv::Rect xx(yMax,cv::Size(abs(xMin.x-xMax.x)/2,abs(yMin.y - yMax.y)*2));
        cv::rectangle(imgOriginal,xx,cv::Scalar(185, 128, 41),5);
        for (size_t idx = 0; idx < contours.size(); idx++) {
            /*double matchR = cv::matchShapes(contours[idx], exampleContours[1],CV_CONTOURS_MATCH_I1,0);
            cout << "xx" << matchR << endl;*/
            cv::Rect rectangle = cv::boundingRect(contours[idx]);
            if(rectangle.area() < 3500){
                continue;
            }
            cv::rectangle(imgOriginal,rectangle,cv::Scalar(185, 128, 41),5);
        }

        cv::Mat contourImage(cv::Size(1280,720), CV_8UC3, cv::Scalar(0,0,0));
        /*cout << "min loc: x:"<< min_loc.x << " - min loc y:"<<min_loc.y <<endl;
        cout << "max loc: x:"<< max_loc.x << " - max loc y:"<<max_loc.y <<endl;
        cout << "MAX:"<< max << " - MIN:"<<min <<endl;*/
        cv::drawContours(contourImage,contours,-1,cv::Scalar(255,255,255),1);
        cv::imshow("Contour Image", contourImage);
        /*cv::imshow("forX", forX);
        cv::imshow("forY", forY);*/
        cv::imshow("Contour Output", contourOutput);
        //cv::imshow("EXCONT", exCont);
        cv::imshow("Realtime", imgOriginal);
        if (cv::waitKey(30) == 27)
        {
            cout << "ESC Key" << endl;
            break;
        }
    }

    return 0;
}



/*   cv::namedWindow("HSV Control", CV_WINDOW_AUTOSIZE);

   HSVValue hue(0, 179);
   HSVValue saturation(0, 255);
   HSVValue value(0, 255);

   cv::createTrackbar("Hue Low", "HSV Control", &hue.low, 179); //Hue (0 - 179)
   cv::createTrackbar("Hue High", "HSV Control", &hue.high, 179);

   cv::createTrackbar("Saturation Low", "HSV Control", &saturation.low, 255); //Saturation (0 - 255)
   cv::createTrackbar("Saturation High", "HSV Control", &saturation.high, 255);

   cv::createTrackbar("Value Low", "HSV Control", &value.low, 255);
   cv::createTrackbar("Value High", "HSV Control", &value.high, 255);*/


/*cv::erode(thresholdedImg, thresholdedImg, cv::getStructuringElement(cv::MORPH_ELLIPSE, ignoreSize));
    cv::dilate(thresholdedImg, thresholdedImg, cv::getStructuringElement(cv::MORPH_ELLIPSE, ignoreSize));

    //morphological closing (fill small holes in the foreground)
    cv::dilate(thresholdedImg, thresholdedImg, cv::getStructuringElement(cv::MORPH_ELLIPSE, ignoreSize));
    cv::erode(thresholdedImg, thresholdedImg, cv::getStructuringElement(cv::MORPH_ELLIPSE, ignoreSize));*/


/*

class HSVValue {
public:
    int low;
    int high;

    HSVValue(int lowVal, int highVal);
};


HSVValue::HSVValue(int lowVal, int highVal) {
    low = lowVal;
    high = highVal;
}

 */



/*
  cv::Mat image;
    cv::Mat image2;
    image = cv::imread("/Users/utkubelemir/Desktop/opencv-color-1/real.png", CV_LOAD_IMAGE_COLOR);   // Read the file
    cv::cvtColor(image, image2, cv::COLOR_BGR2HSV);
    namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image2 );                   // Show our image inside it.

    cv::waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
 */