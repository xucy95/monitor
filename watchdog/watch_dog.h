#include "object_detector.h"
#include "motor.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>

class WatchDog{
public:
    WatchDog();
private:
    ObjectDetector obj_detector;
    Motor motor;
};