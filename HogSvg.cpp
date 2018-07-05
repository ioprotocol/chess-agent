//
// Created by xushy on 18-7-6.
//

#include <iostream>
#include <fstream>
#include <opencv2/core/bufferpool.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

int main()
{
    //creat SVM classfier
    Ptr<SVM> svm = SVM::create();
    //load train file
    svm = SVM::load<SVM>("SVM_HOG.xml");

    if (!svm)
    {
        cout << "Load file failed..." << endl;
    }
    Mat test;
    test = imread("4.png");
    //if (!test)
    //{
    //	cout << "not exist" << endl;
    //	return -1;
    //}
    cout << "Load test image done..." << endl;

    //winsize(64,128),blocksize(16,16),blockstep(8,8),cellsize(8,8),bins9
    //检测窗口(64,128),块尺寸(16,16),块步长(8,8),cell尺寸(8,8),直方图bin个数9
    HOGDescriptor hog(Size(128, 128), Size(16, 16), Size(8, 8), Size(8, 8), 9);

    vector<float> descriptors;//HOG描述子向量
    hog.compute(test, descriptors, Size(8, 8));//计算HOG描述子，检测窗口移动步长(8,8)

    int r = svm->predict(descriptors);   //对所有行进行预测

    cout << "The number is " << r << endl;
    //waitKey();//note：The function only works if there is at least one HighGUI window created and the window is active.If there are several HighGUI windows, any of them can be active.
    //getchar();
    system("pause");
    return 1;
}
