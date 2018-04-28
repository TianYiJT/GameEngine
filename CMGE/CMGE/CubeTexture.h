#pragma once
#include"glew.h"
#include"freeglut.h"
#include"Resource.h"
#include"Texture2D.h"
#include"ResourceManager.h"

using namespace CMGERenderEngine;

enum CubeFace:unsigned int
{
	UP=0,
	DOWN,
	RIGHT,
	LEFT,
	FORWARD,
	BACK,
	CUBEFACESIZE
};

class CubeTexture:public Resource
{
public:
	void Load(string path)
	{
		Resource::Init();
	}
	void Load(string path, string name_, string mformat_)
	{
		name = name_;
		mformat = mformat_;
		Load(path);
		Init();
	}
	void Init() 
	{
		glGenTextures(1, &mCubeMapIndex);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeMapIndex);
		for(int i=0;i<6;i++)
		{
			glTexImage2D(TexFormat[i], 0, GL_RGBA, CubeTex[i].GetWidth(), CubeTex[i].GetHeight(), 0, GL_RGBA,
				GL_UNSIGNED_BYTE, CubeTex[i].GetPixels());
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}
		ResourceManager<CubeTexture>::getInstance()->registerData(this, name);
	}

	void Init(Texture2D* t_up,Texture2D*t_down,Texture2D* t_right,Texture2D* t_left,Texture2D* t_forward,Texture2D* t_back,string name_,string mformat_)
	{
		name = name_;
		mformat = mformat_;
		CubeTex[CubeFace::UP] = *t_up;
		CubeTex[CubeFace::DOWN] = *t_down;
		CubeTex[CubeFace::RIGHT] = *t_right;
		CubeTex[CubeFace::LEFT] = *t_left;
		CubeTex[CubeFace::FORWARD] = *t_forward;
		CubeTex[CubeFace::BACK] = *t_back;
		Init();
	}

	void SetTexture(Texture2D * t,CubeFace cf)
	{
		CubeTex[cf] = *t;
	}

	GLuint GetCubeMapIndex()
	{
		return mCubeMapIndex;
	}

	static void GlobalStartUp() 
	{
		TexFormat[CubeFace::UP] = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
		TexFormat[CubeFace::DOWN] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
		TexFormat[CubeFace::RIGHT] = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		TexFormat[CubeFace::LEFT] = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
		TexFormat[CubeFace::FORWARD] = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
		TexFormat[CubeFace::BACK] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
	}
private:
	GLuint mCubeMapIndex;
	Texture2D CubeTex[CUBEFACESIZE];
	static GLenum* TexFormat;
};
