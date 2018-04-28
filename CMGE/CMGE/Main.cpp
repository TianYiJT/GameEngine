
#include"CMGE.h"
#include"Sample_Exp4.h"
#include<iostream>

int main(int argc, char ** argv)
{
	Sample_Exp4 *exp = new Sample_Exp4(argc,argv);
	exp->Start();
	glutMainLoop();
	return 0;
}

/*
#include<opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>  
#include<iostream>  

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	VideoCapture capture(0);
	namedWindow("opencv_image", CV_WINDOW_AUTOSIZE);
	while (1)
	{
		Mat frame;
		capture >> frame;     //讲摄像头的视频流转换成Mat 格式的图像
		if (!frame.data)   // 判断图像是否有数据                         
		{

			cout << "没有数据" << std::endl;

			return -1;

		}

		imshow("opencv_image", frame);  //显示摄像头的数据
		waitKey(30);
	}
	return 0;
}
*/