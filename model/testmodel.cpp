//
// Created by px on 7/9/2019.
//
#include "./Model.h"

int main(){

    Model testModel;
    std::shared_ptr<EditableVideo> testVideo;
    testVideo = testModel.openFile("C:\\Users\\px\\Desktop\\test.mp4");
    std::shared_ptr<cv::Mat> testMat;
    testVideo->seekImage(8);
    for(int i=0;i<1500;i++){
        testMat = testVideo->getNextImage();
        cv::namedWindow("Video", CV_WINDOW_AUTOSIZE);
        cv::imshow("Video", *testMat);
        cv::waitKey(1);
    }
}
