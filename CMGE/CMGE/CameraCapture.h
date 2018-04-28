#pragma once
#include <opencv2\highgui\highgui.hpp>
#include<iostream>
#include"Renderer.h"
#include"MeshRenderer.h"

using namespace CMGERenderEngine;
using namespace cv;
using namespace CMGEComponent;
using namespace std;

class CameraCapture:public Component
{
public:

	static void* createComponent()
	{
		return new CameraCapture;
	}

	void Init()
	{
		ClassName = "CameraCapture";

		fatherName.push_back("Component");

		ComponentFactory::getInstance().registClass(ClassName, (CreateComponent)createComponent);
	}

	void Start() 
	{
		mVideoCapture = VideoCapture(0);

		if (!mVideoCapture.isOpened())
		{
			cout << "不能打开摄像头！" << endl;
			return;
		}
		mTexture2D->setName("CameraCapture");
		mTexture2D->mTextureType = TextureType::COLOR;
		mTexture2D->SetEnum(GL_RGB);
		mTexture2D->SetPixels(NULL, 800.0f, 600.0f);
		mTexture2D->Init();

		Mesh * tMesh = new Mesh();
		vec3 vp[] = { { -1,-1,0 },{ 1,-1,0 },{ -1,1,0 },{ 1,1,0 } };
		vec2 uv[] = { { 0,0 },{ 1,0 },{ 0,1 },{ 1,1 } };
		int index[] = { 0,1,2,2,3,1 };
		tMesh->num_Position = 4;
		tMesh->glpos = new vec3[4];
		for (int i = 0; i < 4; i++)
			tMesh->glpos[i] = vp[i];
		//delete[] vp;
		tMesh->num_UVCoord = 4;
		tMesh->gluvcoord = new vec2[4];
		for (int i = 0; i < 4; i++)
			tMesh->gluvcoord[i] = uv[i];
		//delete[] uv;
		tMesh->num_Index = 6;
		tMesh->mTriangleIndex = new GLuint[6];
		for (int i = 0; i < 6; i++)
			tMesh->mTriangleIndex[i] = index[i];
		//	delete[] index;

		MeshRenderer * mr = new MeshRenderer();
		mr->SetMesh(tMesh);
		Shader * s_st = new Shader();
		s_st->Load("SolidTexture.shader", "SolidTexture", "shader");

		Material * mat = new Material();
		mat->SetShader(s_st);
		mat->SetMaterialParam("MainTexture", mTexture2D);
		mr->SetMaterial(mat);
		mr->enabled = true;

		mVideoObject->AddComponent<MeshRenderer>(mr);
		mVideoObject->transform.Translate(Vector3(0, 0, 0));
		mVideoObject->isEnable = true;
	}

	void Update()
	{


			mVideoCapture>>tempFrame;

			if (tempFrame.data == NULL)
				return;
			width = tempFrame.cols;
			height = tempFrame.rows;
			channel = tempFrame.channels();
			//cout << channel << endl;
			
			tdata = new uchar[width*height*channel];
			for (int j = 0; j < height; j++)
			{
				for (int k = 0; k < width; k++)
				{
					for (int i = 0; i < channel; i++)
					{
						int srcJ = height - j;
						int srcK = k;
						tdata[(j*width + k)*channel + i] = tempFrame.data[(srcJ*width + srcK)*channel + i];
					//	cout << (int)tdata[(j*width + k)*channel + i] << endl;
					}
				}
			}
			
			mTexture2D->SetPixels(tdata, width, height);
			mTexture2D->Init();
		
	}

	GameObject * mVideoObject = new GameObject();
private:
	Texture2D * mTexture2D = new Texture2D();
	VideoCapture mVideoCapture;
	int width, height, channel;
	uchar * tdata;
	Mat tempFrame;
};