#include <iostream>
#include <ImageCaptioning-DLL/ImageCaptioningLib.h>

int main()
{
    wchar_t encoder_path[] = L"C:/Users/dell/source/repos/ImageCaptioning/data/encoder.onnx";
    wchar_t decoder_path[] = L"C:/Users/dell/source/repos/ImageCaptioning/data/decoder.onnx";
    char vocab_file_path[] = "C:/Users/dell/source/repos/ImageCaptioning/data/vocab.txt";
    char image_path[] = "C:/Users/dell/source/repos/ImageCaptioning/test.jpg";

    
    int result = doDetection(encoder_path, decoder_path, vocab_file_path, image_path);
    std::cout << result << std::endl;
    
    char caption[300];
    int size = getCaption(caption, 300);
    std::cout << caption << std::endl;
}