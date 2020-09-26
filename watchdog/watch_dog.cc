#include "watchdog.h"
#include <iostream>
WatchDog::WatchDog(){
    cv::VideoCapture cap(-1);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    if (!cap.isOpened()) {
        return ;
    }
    while (true) {
        cv::Mat input_image;
        cap >> input_image;
        std::cout <<" frame" << std::endl;
        //cv::imshow("Camera", input_image);
        if (cv::waitKey(1) == char('q')) {
            break;
        }
    }
    cap.release();
    cv::destroyAllWindows();
}