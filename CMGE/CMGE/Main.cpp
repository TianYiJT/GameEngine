
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
		capture >> frame;     //������ͷ����Ƶ��ת����Mat ��ʽ��ͼ��
		if (!frame.data)   // �ж�ͼ���Ƿ�������                         
		{

			cout << "û������" << std::endl;

			return -1;

		}

		imshow("opencv_image", frame);  //��ʾ����ͷ������
		waitKey(30);
	}
	return 0;
}
*/