#include<string>
#include<vector>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <fstream>
#include "paddle_api.h"
#include <arm_neon.h>
#include <sys/time.h>
#include <time.h>
#include <thread>

class ObjectDetector{
public:
    struct RESULT {
        std::string class_name;
        float score;
        float left;
        float top;
        float right;
        float bottom;
    };
public:
    ObjectDetector(const std::string label_fpath, const std::string model_fpath, const int threads);
    bool Init();
    void Detect(cv::Mat &input_image);
private:
    std::vector<std::string> load_labels(const std::string &path);
    void preprocess(cv::Mat &input_image, const std::vector<float> &input_mean,
                const std::vector<float> &input_std, int input_width,
                int input_height, float *input_data);
    std::vector<RESULT> postprocess(const float *output_data, int64_t output_size,
                                const std::vector<std::string> &word_labels,
                                const float score_threshold,
                                cv::Mat &output_image, double time);
    cv::Mat process(cv::Mat &input_image,
                std::vector<std::string> &word_labels,
                std::shared_ptr<paddle::lite_api::PaddlePredictor> &predictor, bool& need_storage);  
    inline int64_t get_current_us() {
        struct timeval time;
        gettimeofday(&time, NULL);
        return 1000000LL * (int64_t)time.tv_sec + (int64_t)time.tv_usec;
    }   
    inline std::string get_time(){
    	time_t t = time(NULL);
    	char ch[64] = {0};
	    strftime(ch, sizeof(ch) - 1, "%Y-%m-%d_%H-%M-%S", localtime(&t));     //年-月-日_时-分-秒
        return std::string(ch);
    }
private:
    std::string label_file_path = "";
    std::string model_file_path = "";
    int kThreadNum = 0;
    paddle::lite_api::PowerMode kCPUPowerMode;
    std::shared_ptr<paddle::lite_api::PaddlePredictor> predictor;
    std::vector<std::string> word_labels;
    std::thread trd;
    const std::vector<int64_t> INPUT_SHAPE = {1, 3, 300, 300};
    const std::vector<float> INPUT_MEAN = {0.5f, 0.5f, 0.5f};
    const std::vector<float> INPUT_STD = {0.5f, 0.5f, 0.5f};
    const float SCORE_THRESHOLD = 0.5f;

    const int kWarmupCount = 0;
    const int kRepeatCount = 1;
};