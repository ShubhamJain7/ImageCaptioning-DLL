#include "pch.h"
#include "internal.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <onnxruntime_cxx_api.h>

using namespace std;
using namespace cv;

Mat preprocess_image(Mat image) {
    // convert image values from int to float
    image.convertTo(image, CV_32FC3);
    // Change image format from BGR to RGB
    cvtColor(image, image, COLOR_BGR2RGB);

    Mat image_resized;
    // resize image to (224x224) to fit model input dimensions
    resize(image, image_resized, Size(224, 224), INTER_LANCZOS4);

    // normalize image (values between 0-1)
    Mat image_float;
    image_resized.convertTo(image_float, CV_32FC3, 1.0f / 255.0f, 0);

    // split image channels
    vector<cv::Mat> channels(3);
    split(image_float, channels);

    // define mean and std-dev for each channel
    vector<double> mean = { 0.485, 0.456, 0.406 };
    vector<double> stddev = { 0.229, 0.224, 0.225 };
    size_t i = 0;
    // normalize each channel with corresponding mean and std-dev values
    for (auto& c : channels) {
        c = (c - mean[i]) / stddev[i];
        ++i;
    }

    // concatenate channels to change format from HWC to CHW
    Mat image_normalized;
    vconcat(channels, image_normalized);

    return image_normalized;
}

std::string _getCaption(wchar_t* encoder_path, wchar_t* decoder_path, char* vocab_file_path, char* image_path)
{
    // declare vector to store all words
    vector<string> vocab;
    // open vocab file
    ifstream file(vocab_file_path);
    string line;
    // read vocab file line by line and append to vocab vector
    while (std::getline(file, line)) {
        vocab.push_back(line);
    }

    // load image to process
    Mat im;
    im = imread(image_path, IMREAD_COLOR);

    // get processed image
    Mat image = preprocess_image(im);


    // create ONNX env and sessionOptions objects
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "test");
    Ort::SessionOptions session_options;

    /* --------------------------------------------------- Encoder ---------------------------------------------------------------- */

    // create ONNX session
    Ort::Session encoder_session(env, encoder_path, session_options);

    // define model input and output node names
    static const char* encoder_input_names[] = { "image" };
    static const char* encoder_output_names[] = { "feature" };

    // get input node info
    Ort::TypeInfo encoder_type_info = encoder_session.GetInputTypeInfo(0);
    auto encoder_tensor_info = encoder_type_info.GetTensorTypeAndShapeInfo();
    vector<int64_t> encoder_input_node_dims;
    encoder_input_node_dims = encoder_tensor_info.GetShape();

    // create input tensor
    auto encoder_memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
    size_t encoder_input_tensor_size = 1 * 3 * 224 * 224;
    Ort::Value encoder_input_tensor = Ort::Value::CreateTensor<float>(encoder_memory_info, reinterpret_cast<float*>(image.data), encoder_input_tensor_size,
        encoder_input_node_dims.data(), 4);

    // pass inputs through model and get output
    auto encoder_outs = encoder_session.Run(Ort::RunOptions{ nullptr }, encoder_input_names, &encoder_input_tensor, 1, encoder_output_names, 1);
    auto features = encoder_outs[0].GetTensorMutableData<float>();

    float features_data[256];
    size_t features_len = encoder_outs[0].GetTensorTypeAndShapeInfo().GetElementCount();
    for (size_t i = 0; i < features_len; i++) {
        features_data[i] = features[i];
    }

    /* --------------------------------------------------- Decoder ---------------------------------------------------------------- */

    // create ONNX session
    Ort::Session decoder_session(env, decoder_path, session_options);

    // define model input and output node names
    static const char* decoder_input_names[] = { "feature" };
    static const char* decoder_output_names[] = { "sample_ids" };

    // get input node info
    Ort::TypeInfo decoder_type_info = decoder_session.GetInputTypeInfo(0);
    auto decoder_tensor_info = decoder_type_info.GetTensorTypeAndShapeInfo();
    vector<int64_t> decoder_input_node_dims;
    decoder_input_node_dims = decoder_tensor_info.GetShape();

    // create input tensor
    auto decoder_memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
    size_t decoder_input_tensor_size = 1 * 256;
    Ort::Value decoder_input_tensor = Ort::Value::CreateTensor<float>(decoder_memory_info, reinterpret_cast<float*>(features_data), decoder_input_tensor_size,
        decoder_input_node_dims.data(), 2);

    // pass inputs through model and get output
    auto decoder_outs = decoder_session.Run(Ort::RunOptions{ nullptr }, decoder_input_names, &decoder_input_tensor, 1, decoder_output_names, 1);
    auto sample_ids = decoder_outs[0].GetTensorMutableData<long long int>();

    size_t sample_ids_len = decoder_outs[0].GetTensorTypeAndShapeInfo().GetElementCount();
    string sentence = "";
    for (size_t i = 0; i < sample_ids_len; i++) {
        int word_index = (int)sample_ids[i];
        string word = vocab[word_index];
        sentence = sentence + word + " ";
        if (word == "<end>") {
            break;
        }
    }
    return sentence;
}
