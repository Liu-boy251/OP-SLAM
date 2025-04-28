/*
从特征提取开始复盘和创造OP-SLAM项目
*/

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

#include "ORBextractor.h"


using namespace std;
using namespace cv;
using namespace OP_SLAM2;

int main(int argc, char const *argv[])
{
    /* code */

    Mat ImageL = imread("1.png");
    cvtColor(ImageL,ImageL,COLOR_BGR2GRAY);

    int nFeatures = 200;
    float fScaleFactor = 1.2;
    int nLevels = 8;
    int fIniThFAST = 20;
    int fMinThFAST = 10;
    ORBextractor* mpORBextractorLeft;

    //描述子
    Mat mDescriptors;
    //特征点
    vector<KeyPoint> mvkeys;

    mpORBextractorLeft = new ORBextractor(nFeatures, fScaleFactor, nLevels, fIniThFAST, fMinThFAST);
    (*mpORBextractorLeft)(ImageL, Mat(), mvkeys, mDescriptors);

    Mat keyPointImageL;
    drawKeypoints(ImageL, mvkeys, keyPointImageL,Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    namedWindow("KeyPoints of imageL");
    imshow("Keypoints of image",keyPointImageL);
    waitKey();
    cout<<"OK!"<<endl;

    return 0;
}
