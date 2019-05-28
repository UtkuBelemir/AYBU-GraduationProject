#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "colors.h"
using namespace std;

double calculateHUMoments(cv::Moments realMoments, cv::Moments targetMoments);

int main(int argc, char **argv) {
    // Start video capturing
    cv::VideoCapture cap(0);
    Colors targetColor;
    // Stop program if camera couldn't be opened
    if (!cap.isOpened()) {
        cout << "Error while connecting camera quiting...." << endl;
        return -1;
    }
    float detectedObjectVisible = 0.0;  /*  This is a custom threshold for detected contours.
                                        For example, when we are searching for Object X,
                                        if there are at least 5 frames including our object, spot it! */

    float HUMomentThreshold = 0.8;
    int visibilityThreshold = 4;
    // Define an ignore size for reducing noise in image
    cv::Size ignoreSize = cv::Size(10, 10);

    // Load template image
    cv::Mat targetImage = cv::imread("/Users/utkubelemir/Desktop/AYBU-GraduationProject/exampleImages/jhjhkjh.png");

    cv::imshow("Reference Image",targetImage);
    cv::Mat templateColorLowerBound, templateColorUpperBound;

    // Convert template image's RGB colors to HSV
    cv::cvtColor(targetImage, targetImage, cv::COLOR_BGR2HSV);

    // Find objects in lower color bound;
    cv::inRange(targetImage, targetColor.lowStart, targetColor.lowEnd, templateColorLowerBound);

    // Find objects in upper color bound;
    cv::inRange(targetImage, targetColor.upStart, targetColor.upEnd, templateColorUpperBound);

    // Merge upper and lower bound results
    cv::addWeighted(templateColorLowerBound, 1.0, templateColorUpperBound, 1.0, 0.0, targetImage);

    // Apply morphological transformations for eliminating irrelevant and small pixels from image
    cv::morphologyEx(targetImage, targetImage, cv::MORPH_OPEN,
                     cv::getStructuringElement(cv::MORPH_ELLIPSE, ignoreSize));
    cv::morphologyEx(targetImage, targetImage, cv::MORPH_CLOSE,
                     cv::getStructuringElement(cv::MORPH_ELLIPSE, ignoreSize));

    std::vector<std::vector<cv::Point> > targetContours;
    cv::findContours(targetImage, targetContours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    cv::Mat targetContourImage(cv::Size(1280, 720), CV_8UC3, cv::Scalar(0, 0, 0));
    cv::drawContours(targetContourImage, targetContours, -1, cv::Scalar(255, 255, 255), 1);

    cv::Moments targetMoments = cv::moments(targetImage, false);

    // Create an empty variable for using active frame from camera
    cv::Mat realTimeFrame;

    // Create a variable for converting original real time BGR image to HSV
    cv::Mat realTimeHSV;

    // Create an infinite loop for real-time camera
    while (true) {

        // Get frame from camera. If there is a problem, exit program.
        bool bSuccess = cap.read(realTimeFrame);
        if (!bSuccess) {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }
        // Convert real-time frame to HSV
        cv::cvtColor(realTimeFrame, realTimeHSV, cv::COLOR_BGR2HSV);

        cv::Mat colorLowerBound, colorUpperBound;

        // Find objects in lower color bound;
        cv::inRange(realTimeHSV, targetColor.lowStart, targetColor.lowEnd, colorLowerBound);

        // Find objects in upper color bound;
        cv::inRange(realTimeHSV, targetColor.upStart, targetColor.upEnd, colorUpperBound);

        // Merge upper and lower bound results
        cv::addWeighted(colorLowerBound, 1.0, colorUpperBound, 1.0, 0.0, realTimeHSV);

        // Apply morphological transformations for eliminating irrelevant and small pixels from image
        cv::morphologyEx(realTimeHSV, realTimeHSV, cv::MORPH_OPEN,
                         cv::getStructuringElement(cv::MORPH_ELLIPSE, ignoreSize));
        cv::morphologyEx(realTimeHSV, realTimeHSV, cv::MORPH_CLOSE,
                         cv::getStructuringElement(cv::MORPH_ELLIPSE, ignoreSize));

        std::vector<std::vector<cv::Point> > realTimeContours;
        // Find contours of segmented objects
        cv::findContours(realTimeHSV, realTimeContours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);


        cv::Moments realTimeMoments = cv::moments(realTimeHSV, false);
        double HUMomentsResult = calculateHUMoments(realTimeMoments,targetMoments);

        if (detectedObjectVisible < 0.0) {
            detectedObjectVisible = 0.0;
        }

        bool hMom = (HUMomentsResult < HUMomentThreshold && HUMomentsResult > (HUMomentThreshold * -1) && !isinf(HUMomentsResult));
        if (detectedObjectVisible > visibilityThreshold || hMom) {
            if (!hMom) {
                detectedObjectVisible -= 0.5;
            } else {
                detectedObjectVisible += 1;
            }
            for (size_t contourIterator = 0; contourIterator < realTimeContours.size(); contourIterator++) {
                cv::Rect spottedArea = cv::boundingRect(realTimeContours[contourIterator]);
                if (spottedArea.area() < 3500) {
                    continue;
                }
                cv::rectangle(realTimeFrame, spottedArea, cv::Scalar(226, 0, 247), 5);
            }
        }
        if (!hMom) {
            detectedObjectVisible -= 0.5;
        }

        cv::Mat contourImage(cv::Size(1280, 720), CV_8UC3, cv::Scalar(0, 0, 0));
        cv::drawContours(contourImage, realTimeContours, -1, cv::Scalar(255, 255, 255), 1);
        cv::imshow("Realtime", realTimeFrame);
        if (cv::waitKey(30) == 27) {
            cout << "ESC Key" << endl;
            break; // Realtime Camera
        }
    }

    return 0;
}

double calculateHUMoments(cv::Moments realMoments, cv::Moments targetMoments){
    double realHUMoments[7], targetHUMoments[7];
    cv::HuMoments(realMoments, realHUMoments);
    cv::HuMoments(targetMoments, targetHUMoments);
    double result = 0;
    for (int i = 0; i < 7; i++) {
        realHUMoments[i] = -1 * copysign(1.0, realHUMoments[i]) * log10(abs(realHUMoments[i]));
        targetHUMoments[i] = -1 * copysign(1.0, targetHUMoments[i]) * log10(abs(targetHUMoments[i]));
        result += (realHUMoments[i] - targetHUMoments[i]);
    }
    return result / 7;
}