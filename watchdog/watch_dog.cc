#include "watch_dog.h"
#include <iostream>
WatchDog::WatchDog(){
    ObjectDetector obj_detector(
        "/home/pi/workspace/watchdog/build/third_party/paddile_lite_demo/src/Paddle-Lite-Demo/PaddleLite-armlinux-demo/object_detection_demo/labels/pascalvoc_label_list",
        "/home/pi/workspace/watchdog/build/third_party/paddile_lite_demo/src/Paddle-Lite-Demo/PaddleLite-armlinux-demo/object_detection_demo/models/ssd_mobilenet_v1_pascalvoc_for_cpu/model.nb",
        2
    );
    obj_detector.Init();
    cv::VideoCapture cap(-1);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 360);
    if (!cap.isOpened()) {
        return ;
    }
    while (true) {
        cv::Mat input_image;
        cap >> input_image;
        obj_detector.Detect(input_image);
        //cv::imshow("Camera", input_image);
        if (cv::waitKey(1) == char('q')) {
            break;
        }
    }
    cap.release();
    cv::destroyAllWindows();
}