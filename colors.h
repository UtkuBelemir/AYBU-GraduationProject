#import "opencv2/core/types.hpp"
#ifndef GRADUATION_PROJECT_COLORS_H
#define GRADUATION_PROJECT_COLORS_H


class Colors {
    public:
        cv::Scalar lowStart;
        cv::Scalar lowEnd;
        cv::Scalar upStart;
        cv::Scalar upEnd;
        Colors(){
            initializeColors();
        };
        void initializeColors(){
            // RED
            this->lowStart = cv::Scalar(175, 100, 100);
            this->lowEnd = cv::Scalar(180, 255, 255);
            this->upStart = cv::Scalar(0, 100, 100);
            this->upEnd = cv::Scalar(5, 255, 255);
            /*// BLACK
            this->lowStart = cv::Scalar(0,0,0);
            this->lowEnd = cv::Scalar(0,0,20);
            this->upStart = cv::Scalar(0, 0, 20);
            this->upEnd = cv::Scalar(0,0,40);*/
            /*// GREEN
            this->lowStart = cv::Scalar(30, 100, 100);
            this->lowEnd = cv::Scalar(40, 255, 255);
            this->upStart = cv::Scalar(40, 100, 100);
            this->upEnd = cv::Scalar(50, 255, 255);*/
            /*// BLUE
            this->lowStart = cv::Scalar(80, 100, 100);
            this->lowEnd = cv::Scalar(100, 255, 255);
            this->upStart = cv::Scalar(100, 100, 100);
            this->upEnd = cv::Scalar(125, 255, 255);*/
            /*// YELLOW
            this->lowStart = cv::Scalar(25, 100, 100);
            this->lowEnd = cv::Scalar(30, 255, 255);
            this->upStart = cv::Scalar(30, 100, 100);
            this->upEnd = cv::Scalar(55, 255, 255);*/
        }
};


#endif //GRADUATION_PROJECT_COLORS_H
