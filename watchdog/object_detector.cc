#include "object_detector.h"

ObjectDetector::ObjectDetector(const std::string label_fpath, const std::string model_fpath, const int threads){}
bool ObjectDetector::Init(){}
void ObjectDetector::Detect(cv::Mat &input_image){}
std::vector<std::string> ObjectDetector::load_labels(const std::string &path){}