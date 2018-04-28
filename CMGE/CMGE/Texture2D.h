#pragma once
#include"CMGEMathEngine.h"
#include"glew.h"
#include"freeglut.h"
#include"Resource.h"
#include"ResourceManager.h"
#include"ResourceLoader.h"

using namespace CMGEMathEngine;
using namespace CMGETool;

namespace CMGERenderEngine
{

	enum TextureType:unsigned int
	{
		COLOR = 0,
		DEPTH,
		STENCIL,
		FLOATFRAME
	};

	class Texture2D :public Resource
	{
	public:
		TextureType mTextureType;
		Vector4 GetPixel(Vector2 uv) 
		{
			Vector4 col;
			int u0 = (int)uv.x;
			int v0 = (int)uv.y;
			int u1 = u0 + 1, v1 = v0 + 1;
			Vector4 col1(ucTof(data[(u0*height + v0) * 4]), ucTof(data[(u0*height + v0) * 4 + 1]), ucTof(data[(u0*height + v0) * 4 + 2]), ucTof(data[(u0*height + v0) * 4 + 3]));
			Vector4 col2(ucTof(data[(u0*height + v1) * 4]), ucTof(data[(u0*height + v1) * 4 + 1]), ucTof(data[(u0*height + v1) * 4 + 2]), ucTof(data[(u0*height + v1) * 4 + 3]));
			Vector4 col3(ucTof(data[(u1*height + v0) * 4]), ucTof(data[(u1*height + v0) * 4 + 1]), ucTof(data[(u1*height + v0) * 4 + 2]), ucTof(data[(u1*height + v0) * 4 + 3]));
			Vector4 col4(ucTof(data[(u1*height + v1) * 4]), ucTof(data[(u1*height + v1) * 4 + 1]), ucTof(data[(u1*height + v1) * 4 + 2]), ucTof(data[(u1*height + v1) * 4 + 3]));
			return col1*(1 - uv.x + u0)*(1 - uv.y + v0) + col2*(1 - uv.x + u0)*(uv.y - v0) + col3*(uv.x - u0)*(1 - uv.y + v0) + col4*(uv.x - u0)*(uv.y - v0);
		}
		
		void SetPixels(unsigned char* data_,int w,int h) 
		{
			
			width = w;
			height = h;

			if (data == NULL&&mTextureType==TextureType::COLOR)
				data = new unsigned char[width*height * 4];
			if (floatdata == NULL && (mTextureType == TextureType::DEPTH || mTextureType == TextureType::STENCIL))
				floatdata = new float[width*height*4];
			if(data_!=NULL)
			{
				data = data_;
			}
		}

		void output() 
		{

		}

		unsigned char* GetPixels()
		{
			return data;
		}

		void SetPixel(Vector4 color,int w,int h)
		{
			data[(w*height + h) * 4] = ftouc(color.x);
			data[(w*height + h) * 4+1] = ftouc(color.y);
			data[(w*height + h) * 4+2] = ftouc(color.z);
			data[(w*height + h) * 4+3] = ftouc(color.w);
		}

		void Init() 
		{
			if (TexIndex == 99999) 
			{
				glGenTextures(1, &TexIndex);
				ResourceManager<Texture2D>::getInstance()->registerData(this, name);
			}
			glBindTexture(GL_TEXTURE_2D,TexIndex);
			switch (mTextureType)
			{
			case CMGERenderEngine::COLOR:
				glTexImage2D(GL_TEXTURE_2D, 0, mEnum, width, height,
					0, mEnum, GL_UNSIGNED_BYTE, data);
				break;
			case CMGERenderEngine::DEPTH:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height,
					0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
				break;
			case CMGERenderEngine::STENCIL:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, width, height,
					0, GL_STENCIL_INDEX, GL_FLOAT, floatdata);
				break;
			case CMGERenderEngine::TextureType::FLOATFRAME:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height,
					0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				break;
			default:
				glTexImage2D(GL_TEXTURE_2D, 0, mEnum, width, height,
					0, mEnum, GL_UNSIGNED_BYTE, data);
				break;
			}
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glBindTexture(GL_TEXTURE_2D, 0);
			


		}

		void Load(string path)
		{
			Resource::Load(path);
			if (mformat != "bmp") 
			{
				TextureLoader *texLoader = new TextureLoader();
				texLoader->Load(path.c_str());
				SetPixels(texLoader->mData, texLoader->mWidth, texLoader->mHeight);
			}
			else
			{
				TextureLoader_BMP *texLoader = new TextureLoader_BMP();
				texLoader->Load(path.c_str());
				SetPixels(texLoader->mData, texLoader->mWidth, texLoader->mHeight);
			}


		}

		void Load(string path, string name_, string mformat_)
		{
			setFormat(mformat_);
			setName(name_);
			Load(path);
			Init();
		}

		static void GlobalStartUp() 
		{

		}

		void frameReturnRead() 
		{
			glBindTexture(GL_TEXTURE_2D,TexIndex);
			switch (mTextureType)
			{
			case CMGERenderEngine::COLOR:
				glGetTexImage(GL_TEXTURE_2D, 0, mEnum, GL_UNSIGNED_BYTE, data);
				break;
			case CMGERenderEngine::DEPTH:
				glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, floatdata);
				break;
			case CMGERenderEngine::STENCIL:
				glGetTexImage(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, GL_FLOAT, floatdata);
				break;
			case CMGERenderEngine::TextureType::FLOATFRAME:
				glGetTexImage(GL_TEXTURE_2D, 0, mEnum, GL_UNSIGNED_BYTE, data);
					break;
			default:
				glGetTexImage(GL_TEXTURE_2D, 0, mEnum, GL_UNSIGNED_BYTE, data);
				break;
			}
		}

		GLuint GetIndex() 
		{
			return TexIndex;
		}

		int GetWidth() { return width; }

		int GetHeight() { return height; }

		void SetEnum(GLenum enum_) { mEnum = enum_; }

	private:
		unsigned char* data=NULL;
		float* floatdata = NULL;
		int width, height;
		float ucTof(unsigned char uc) 
		{
			return ((int)uc) / (float)256;
		}

		unsigned char ftouc(float f) 
		{
			return (unsigned char)(f*256);
		}

		GLuint TexIndex=99999;
		GLenum mEnum = GL_RGBA;
	};
};