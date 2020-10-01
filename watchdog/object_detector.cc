#include "object_detector.h"
ObjectDetector::ObjectDetector(const std::string label_fpath,
 const std::string model_fpath,
 const int threads)
    :label_file_path(label_fpath),
    model_file_path(model_fpath),
    kThreadNum(threads)
{
    kCPUPowerMode = paddle::lite_api::PowerMode::LITE_POWER_HIGH;
}

bool ObjectDetector::Init(){
    word_labels = load_labels(label_file_path);

    paddle::lite_api::MobileConfig config;
    config.set_model_from_file(model_file_path);
    config.set_threads(kThreadNum);
    config.set_power_mode(kCPUPowerMode);
    predictor = paddle::lite_api::CreatePaddlePredictor<paddle::lite_api::MobileConfig>(config);

    return true;
}

void ObjectDetector::Detect(cv::Mat &input_image){
    bool need_storage = true;
    cv::Mat output_image = process(input_image, word_labels, predictor, need_storage);
    if(need_storage){
        std::string cur_time = get_time();
        std::string output_image_path = "/home/pi/USB_DISK/image/" + cur_time + ".jpg";
        bool ret = cv::imwrite(output_image_path, output_image);
        if( !ret){
            std::string output_image_path = "/home/pi/Pictures/" + cur_time + ".jpg";
            cv::imwrite(output_image_path, output_image);
        }

    }
}

std::vector<std::string> ObjectDetector::load_labels(const std::string &path){
    std::ifstream file;
    std::vector<std::string> labels;
    file.open(path);
    while (file) {
        std::string line;
        std::getline(file, line);
        labels.push_back(line);
    }
    file.clear();
    file.close();
    return labels;
}

void ObjectDetector::preprocess(cv::Mat &input_image, const std::vector<float> &input_mean,
            const std::vector<float> &input_std, int input_width,
            int input_height, float *input_data){
  cv::Mat resize_image;
  cv::resize(input_image, resize_image, cv::Size(input_width, input_height), 0, 0);
  if (resize_image.channels() == 4) {
    cv::cvtColor(resize_image, resize_image, CV_BGRA2RGB);
  }
  cv::Mat norm_image;
  resize_image.convertTo(norm_image, CV_32FC3, 1 / 255.f);
  // NHWC->NCHW
  int image_size = input_height * input_width;
  const float *image_data = reinterpret_cast<const float *>(norm_image.data);
  float32x4_t vmean0 = vdupq_n_f32(input_mean[0]);
  float32x4_t vmean1 = vdupq_n_f32(input_mean[1]);
  float32x4_t vmean2 = vdupq_n_f32(input_mean[2]);
  float32x4_t vscale0 = vdupq_n_f32(1.0f / input_std[0]);
  float32x4_t vscale1 = vdupq_n_f32(1.0f / input_std[1]);
  float32x4_t vscale2 = vdupq_n_f32(1.0f / input_std[2]);
  float *input_data_c0 = input_data;
  float *input_data_c1 = input_data + image_size;
  float *input_data_c2 = input_data + image_size * 2;
  int i = 0;
  for (; i < image_size - 3; i += 4) {
    float32x4x3_t vin3 = vld3q_f32(image_data);
    float32x4_t vsub0 = vsubq_f32(vin3.val[0], vmean0);
    float32x4_t vsub1 = vsubq_f32(vin3.val[1], vmean1);
    float32x4_t vsub2 = vsubq_f32(vin3.val[2], vmean2);
    float32x4_t vs0 = vmulq_f32(vsub0, vscale0);
    float32x4_t vs1 = vmulq_f32(vsub1, vscale1);
    float32x4_t vs2 = vmulq_f32(vsub2, vscale2);
    vst1q_f32(input_data_c0, vs0);
    vst1q_f32(input_data_c1, vs1);
    vst1q_f32(input_data_c2, vs2);
    image_data += 12;
    input_data_c0 += 4;
    input_data_c1 += 4;
    input_data_c2 += 4;
  }
  for (; i < image_size; i++) {
    *(input_data_c0++) = (*(image_data++) - input_mean[0]) / input_std[0];
    *(input_data_c1++) = (*(image_data++) - input_mean[1]) / input_std[1];
    *(input_data_c2++) = (*(image_data++) - input_mean[2]) / input_std[2];
  }
}

std::vector<ObjectDetector::RESULT> ObjectDetector::postprocess(const float *output_data, int64_t output_size,
                            const std::vector<std::string> &word_labels,
                            const float score_threshold,
                            cv::Mat &output_image, double time){
  std::vector<ObjectDetector::RESULT> results;
  std::vector<cv::Scalar> colors = {
      cv::Scalar(237, 189, 101), cv::Scalar(0, 0, 255), cv::Scalar(102, 153, 153),
      cv::Scalar(255, 0, 0), cv::Scalar(9, 255, 0), cv::Scalar(0, 0, 0),
      cv::Scalar(51, 153, 51)};
    for (int64_t i = 0; i < output_size; i += 6) {
        if (output_data[i + 1] < score_threshold) {
        continue;
        }
        int class_id = static_cast<int>(output_data[i]);
        float score = output_data[i + 1];
        ObjectDetector::RESULT result;
        std::string class_name = "Unknown";
        if (word_labels.size() > 0 && class_id >= 0 && 
            class_id < word_labels.size()) {
        class_name = word_labels[class_id];
        }
        result.class_name = class_name;
        result.score = score;
        result.left = output_data[i + 2];
        result.top = output_data[i + 3];
        result.right = output_data[i + 4];
        result.bottom = output_data[i + 5];
        int lx = static_cast<int>(result.left * output_image.cols);
        int ly = static_cast<int>(result.top * output_image.rows);
        int w = static_cast<int>(result.right * output_image.cols) - lx;
        int h = static_cast<int>(result.bottom * output_image.rows) - ly;
        cv::Rect bounding_box = cv::Rect(lx, ly, w, h) &
            cv::Rect(0, 0, output_image.cols, output_image.rows);
        if (w > 0 && h > 0 && score <= 1) {
        cv::Scalar color = colors[results.size() % colors.size()];
        cv::rectangle(output_image, bounding_box, color);
        cv::rectangle(output_image, cv::Point2d(lx, ly), cv::Point2d(lx + w, ly - 10),
                        color, -1);
        cv::putText(output_image,
                    std::to_string(results.size()) + "." + class_name + ":" +
                        std::to_string(score),
                    cv::Point2d(lx, ly), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 255, 255));
        results.push_back(result);
        }
    }
    return results;
}

cv::Mat ObjectDetector::process(cv::Mat &input_image,
            std::vector<std::string> &word_labels,
            std::shared_ptr<paddle::lite_api::PaddlePredictor> &predictor, bool& need_storage){
  // Preprocess image and fill the data of input tensor
  std::unique_ptr<paddle::lite_api::Tensor> input_tensor(
      std::move(predictor->GetInput(0)));
  input_tensor->Resize(INPUT_SHAPE);
  int input_width = INPUT_SHAPE[3];
  int input_height = INPUT_SHAPE[2];
  auto *input_data = input_tensor->mutable_data<float>();
  double preprocess_start_time = get_current_us();
  preprocess(input_image, INPUT_MEAN, INPUT_STD, input_width, input_height,
             input_data);
  double preprocess_end_time = get_current_us();
  double preprocess_time = (preprocess_end_time - preprocess_start_time) / 1000.0f;

  double prediction_time = 0;
  // Run predictor
  // warm up to skip the first inference and get more stable time, remove it in
  // actual products
  for (int i = 0; i < kWarmupCount; i++) {
    predictor->Run();
  }
  // repeat to obtain the average time, set kRepeatCount=1 in actual products
  double max_time_cost = 0.0f;
  double min_time_cost = std::numeric_limits<float>::max();
  double total_time_cost = 0.0f;
  for (int i = 0; i < kRepeatCount; i++) {
    auto start = get_current_us();
    predictor->Run();
    auto end = get_current_us();
    double cur_time_cost = (end - start) / 1000.0f;
    if (cur_time_cost > max_time_cost) {
      max_time_cost = cur_time_cost;
    }
    if (cur_time_cost < min_time_cost) {
      min_time_cost = cur_time_cost;
    }
    total_time_cost += cur_time_cost;
    prediction_time = total_time_cost / kRepeatCount;
    //printf("iter %d cost: %f ms\n", i, cur_time_cost);
  }
  printf("warmup: %d repeat: %d, average: %f ms, max: %f ms, min: %f ms\n",
         kWarmupCount, kRepeatCount, prediction_time,
         max_time_cost, min_time_cost);

  // Get the data of output tensor and postprocess to output detected objects
  std::unique_ptr<const paddle::lite_api::Tensor> output_tensor(
      std::move(predictor->GetOutput(0)));
  const float *output_data = output_tensor->mutable_data<float>();
  int64_t output_size = 1;
  for (auto dim : output_tensor->shape()) {
    output_size *= dim;
  }
  cv::Mat output_image = input_image.clone();
  double postprocess_start_time = get_current_us();
  std::vector<ObjectDetector::RESULT> results = postprocess(
      output_data, output_size, word_labels, SCORE_THRESHOLD, output_image, prediction_time);
  double postprocess_end_time = get_current_us();
  double postprocess_time = (postprocess_end_time - postprocess_start_time) / 1000.0f;

  printf("results: %d\n", results.size());
  need_storage = false;
  for (int i = 0; i < results.size(); i++) {
    if(results[i].class_name == "person"){
      need_storage = true;
    }
    printf("[%d] %s - %f %f,%f,%f,%f\n", i, results[i].class_name.c_str(),
           results[i].score, results[i].left, results[i].top, results[i].right,
           results[i].bottom);
  }
  printf("Preprocess time: %f ms\n", preprocess_time);
  printf("Prediction time: %f ms\n", prediction_time);
  printf("Postprocess time: %f ms\n\n", postprocess_time);
  return output_image;
}