#pragma once
#include"opencv2\opencv.hpp"
#include<vector>
#include"Resource.h"
#include"ResourceManager.h"
#include"ResourceLoader.h"

using namespace cv;
using namespace std;
namespace CMGERenderEngine
{
	class Video:public Resource
	{
	public:
		void Load(string path)
		{
			Resource::Load(path);
			VideoCapture tCapture(path);
			
			if (!tCapture.isOpened())
				return;
			
			for (; ;)
			{
				//tCapture.
				if (tCapture.grab())
				{

					Mat frame;

					//cout << videoData.size() << endl;
					tCapture.retrieve(frame);
				//	cout << frame.data[2] << endl;
					
					width = frame.cols;
					height = frame.rows;
					channel = frame.channels();
					uchar * tdata = new uchar[width*height*channel];
					
					for (int j = 0; j < height; j++)
					{
						for (int k = 0; k < width; k++) 
						{
							for (int i = 0; i < channel; i++)
							{
								int srcJ = height-j;
								int srcK =  k;
								tdata[(j*width + k)*channel + i] = frame.data[(srcJ*width + srcK)*channel + i];
							}
						}
					}
					

					videoData.push_back(tdata);
				}
				else
					break;
			}
			tCapture.release();
		}
		/*
		void Write() 
		{
			double rate = 25.0;//ÊÓÆµµÄÖ¡ÂÊ  
			Size videoSize(1280, 960);
			VideoWriter writer("VideoTest.avi", CV_FOURCC('M', 'J', 'P', 'G'), rate, videoSize);
			Mat frame;
			
		}
		*/
		void Load(string path, string name_, string mformat_)
		{
			setFormat(mformat_);
			setName(name_);
			Load(path);
			Init();
		}

		void Init() 
		{
			ResourceManager<Video>::getInstance()->registerData(this, name);
		}

		int getFrameCount()
		{
			return videoData.size();
		}

		int getWidth() 
		{
			return width;
		}

		int getHeight() 
		{
			return height;
		}

		int getChannel() 
		{
			return channel;
		}

		uchar* getData(int index)
		{
			return videoData[index];
		}

		void output(int index) 
		{
			for (int i = 0; i < width*height*3; i++) 
			{
				cout << (int)videoData[index][i] << endl;
			}
		}

	private:
		vector<uchar*> videoData;
		int width; 
		int height;
		int channel;
	};
}