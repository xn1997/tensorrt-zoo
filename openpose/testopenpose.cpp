/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-08-21 09:45:10
 * @LastEditTime: 2019-12-04 19:27:27
 * @LastEditors: zerollzeng
 */
#include "OpenPose.hpp"
#include "opencv2/opencv.hpp"

#include <vector>
#include <string>
#include "time.h"
//Get image data
std::vector<float> GetInputData(const cv::Mat& img, int N, int C, int H, int W)
{
    // int N = 1;
    // int C = 3;
    // int H = 480;
    // int W = 640;
    std::vector<float> inputData;
    inputData.resize(N * C * H * W);

    unsigned char* data = img.data;
    for (int n = 0; n < N; n++)
    {
        for (int c = 0; c < C; c++)
        {
            for (int i = 0; i < W * H; i++)
            {
                inputData[i + c * W * H + n * C * H * W] = (float)data[i * C + c]/255.f - 0.5f;
            }
        }
    }

    return inputData;

}
class InputParser{
    public:
        InputParser (int &argc, char **argv){
            for (int i=1; i < argc; ++i)
                this->tokens.push_back(std::string(argv[i]));
        }
        /// @author iain
        const std::string& getCmdOption(const std::string &option) const{
            std::vector<std::string>::const_iterator itr;
            itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
            if (itr != this->tokens.end() && ++itr != this->tokens.end()){
                return *itr;
            }
            static const std::string empty_string("");
            return empty_string;
        }
        /// @author iain
        bool cmdOptionExists(const std::string &option) const{
            return std::find(this->tokens.begin(), this->tokens.end(), option)
                   != this->tokens.end();
        }
    private:
        std::vector <std::string> tokens;
};

int main(int argc, char** argv) {
//    std::cout << "usage: path/to/testopenpose --prototxt path/to/prototxt --caffemodel path/to/caffemodel/ --save_engine path/to/save_engin --input path/to/input/img --run_mode 0/1/2" << std::endl;
//    InputParser cmdparams(argc, argv);
//    const std::string& prototxt = cmdparams.getCmdOption("--prototxt");
//    const std::string& caffemodel = cmdparams.getCmdOption("--caffemodel");
//    const std::string& save_engine = cmdparams.getCmdOption("--save_engine");
//    const std::string& img_name = cmdparams.getCmdOption("--input");
//    int run_mode = std::stoi(cmdparams.getCmdOption("--run_mode"));
//    int H = std::stoi(cmdparams.getCmdOption("--h"));
//    int W = std::stoi(cmdparams.getCmdOption("--w"));

    const std::string prototxt = "../model/pose_deploy.prototxt";
    const std::string caffemodel = "../model/pose_iter_584000.caffemodel";
    const std::string save_engine = "../model/sample.engine";
    const std::string& img_name = "../test.jpg";
    // const std::string img_name = "./test.jpg";
    int run_mode = 2;//0 for float32, 1 for float16, 2 for int8
    int H = 224; //480;
    int W = 224; //640;

    cv::Mat img = cv::imread(img_name);
    if(img.empty()) {
        std::cout << "error: can not read image" << std::endl;
        return -1;
    }

    cv::cvtColor(img,img,cv::COLOR_BGR2RGB);
    cv::resize(img,img,cv::Size(W,H));

    int N = 1;
    int C = 3;
    std::vector<float> inputData;
    inputData.resize(N * C * H * W);

    unsigned char* data = img.data;
    for(int n=0; n<N;n++) {
        for(int c=0;c<3;c++) {
            for(int i=0;i<W*H;i++) {
                inputData[i+c*W*H+n*3*H*W] = (float)data[i*3+c];
            }
        }
    }
    std::vector<float> result;
    
    std::vector<std::string> outputBlobname{"net_output"};
    std::vector<std::vector<float>> calibratorData;
//    calibratorData.resize(3);
//    for(size_t i = 0;i<calibratorData.size();i++) {
//        calibratorData[i].resize(3*H*W);
//        for(size_t j=0;j<calibratorData[i].size();j++) {
//            calibratorData[i][j] = 0.05;
//        }
//    }
    //--------------------------------------------
    cv::Mat calibrator_image;
    calibratorData.resize(2692);  // 224 输入至少为100张才勉强可以用，2000相对较好
    for(size_t i = 0;i<calibratorData.size();i++) {
        calibratorData[i].resize(3*H*W);
        calibrator_image = cv::imread("../model/resize_image/" + std::to_string(i).append(".jpg"));
        std::cout<< i << " image\n" << std::endl;
        cv::cvtColor(calibrator_image,calibrator_image,cv::COLOR_BGR2RGB);
//            cv::resize(calibrator_image,calibrator_image, cv::Size(W,H));
//            cv::imshow("d",calibrator_image);
//            cv::waitKey(1);
        std::vector<float> inputData = GetInputData(calibrator_image,N,C,H,W);
        calibratorData[i] = inputData;
        calibrator_image.release();
    }
    //--------------------------------------------
    int maxBatchSize = N;

    OpenPose* openpose = new OpenPose(prototxt,
                        caffemodel,
                        save_engine,
                        outputBlobname,
                        calibratorData,
                        maxBatchSize,
                        run_mode);

    double avg_time = 0;
    int i=0;
    while(1) {
        clock_t start = clock();
        openpose->DoInference(inputData,result);
        clock_t end = clock();
        double all_time = ((double)(end - start) / CLOCKS_PER_SEC)*1000;
        std::cout << "inference Time : " << all_time << " ms" << std::endl;
        i++;
        if(i>1){
            avg_time += all_time;
        }
        if(i>=11){
            avg_time /= 10;
            printf("avg_time : %f",avg_time);
            break;
        }
    }
    
    cv::cvtColor(img,img,cv::COLOR_RGB2BGR);
    for(size_t i=0;i<result.size()/3;i++) {
        cv::circle(img,cv::Point(result[i*3],result[i*3+1]),2,cv::Scalar(0,255,0),-1);
    }
    cv::imwrite("result.jpg",img);
    
    return 0;

}
