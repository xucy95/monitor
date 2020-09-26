#include<string>
#include<vector>
#include <opencv2/core/core.hpp>

class ObjectDetector{
public:
    ObjectDetector(){}
    ObjectDetector(const std::string label_fpath, const std::string model_fpath, const int threads);
    bool Init();
    void Detect(cv::Mat &input_image);
private:
    std::vector<std::string> load_labels(const std::string &path);
private:
    const int kThreadNum = 0;
    const std::string label_file_path = "";
    const std::string model_file_path = "";

    const std::vector<int64_t> INPUT_SHAPE = {1, 3, 300, 300};
    const std::vector<float> INPUT_MEAN = {0.5f, 0.5f, 0.5f};
    const std::vector<float> INPUT_STD = {0.5f, 0.5f, 0.5f};
    const float SCORE_THRESHOLD = 0.5f;
};