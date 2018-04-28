#pragma once
#include"Video.h"
#include"Renderer.h"
#include"MeshRenderer.h"

using namespace CMGERenderEngine;

class VideoPlayer:public Component
{
public:

	static void* createComponent()
	{
		return new VideoPlayer;
	}

	void Init()
	{
		ClassName = "VideoPlayer";

		fatherName.push_back("Component");

		ComponentFactory::getInstance().registClass(ClassName, (CreateComponent)createComponent);
	}

	void Start() 
	{
		mTexture2D->setName("VideoFrame");
		mTexture2D->mTextureType = TextureType::COLOR;
		if (mVideo->getChannel() == 3)
			mTexture2D->SetEnum(GL_RGB);
		mTexture2D->SetPixels(NULL,mVideo->getWidth(),mVideo->getHeight());
		mTexture2D->Init();

		Mesh * tMesh = new Mesh();
		vec3 vp[] = { {-1,-1,0},{1,-1,0},{-1,1,0},{1,1,0} };
		vec2 uv[] = { {0,0},{1,0},{0,1},{1,1} };
		int index[] = {0,1,2,2,3,1};
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
		s_st->Load("SolidTexture.shader","SolidTexture","shader");

		Material * mat = new Material();
		mat->SetShader(s_st);
		mat->SetMaterialParam("MainTexture",mTexture2D);
		mr->SetMaterial(mat);
		mr->enabled = true;

		mVideoObject->AddComponent<MeshRenderer>(mr);
		mVideoObject->transform.Translate(Vector3(0, 0, 0));
		frameRate = 48;
	}

	void FixedUpdate()
	{
		//cout << "sad" << endl;
		if (isPlay) 
		{
			mTexture2D->SetPixels(mVideo->getData(tempFrame), mVideo->getWidth(), mVideo->getHeight());
			mTexture2D->Init();
			tempFrame++;
		//	cout << mVideo->getFrameCount() <<"  "<<tempFrame<<endl;
			if (tempFrame == mVideo->getFrameCount() && !isLoop)
			{
				isPlay = false;
				mVideoObject->isEnable = false;
				Camera::MainCamera->OrthoOrNot = false;
			}
			if (tempFrame == mVideo->getFrameCount()&& isLoop)
			{
				tempFrame = 0;
			}
		}
	}

	void Play() 
	{
		isPlay = true;
		mVideoObject->isEnable = true;
		Camera::MainCamera->OrthoOrNot = true;
		((GameObject*)Camera::MainCamera->mGameObject)->transform.Position = Vector3(0,0,-5);
	}

	void SetLoop(bool Looping) 
	{
		isLoop = Looping;
	}

	void SetVideo(Video * pVideo) 
	{
		mVideo = pVideo;
	}
	GameObject * mVideoObject = new GameObject();
private:
	Video * mVideo = new Video();

	Texture2D * mTexture2D = new Texture2D();
	bool isLoop;
	bool isPlay;
	int tempFrame = 0;
};
