#pragma once
#include"Shader.h"
#include"CMGEMathEngine.h"
#include"Texture2D.h"
#include"CubeTexture.h"
#include<string>
#include"Resource.h"
#include"ResourceManager.h"
#include"CMGEStringEngine.h"
#include<fstream>
using namespace CMGERenderEngine;
using namespace CMGEMathEngine;
using namespace CMGEStringEngine;
using namespace std;
namespace CMGERenderEngine 
{
	
	struct MaterialParam
	{
		ShaderParamType spt;
		void* data;
		string name;
		bool isCMGE;
		MaterialParam(ShaderParamType spt_, void* data_,string name_,bool isCMGE_) 
		{
			spt = spt_;
			data = data_;
			name = name_;
			isCMGE = isCMGE_;
		}

		void output() 
		{
			cout << Shader::mapInverseShaderParamType[spt] << " ";
			cout << name << " ";
			switch (spt)
			{
			case CMGERenderEngine::FLOAT:
				if (data != NULL)
					cout << *((float*)data) << endl;
				else
					cout << "NULL" << endl;
				break;
			case CMGERenderEngine::VECTOR2:
				if (data != NULL)
					((Vector2*)data)->output();
				else
					cout << "NULL" << endl;
				break;
			case CMGERenderEngine::VECTOR3:
				if (data != NULL)
					((Vector3*)data)->output();
				else
					cout << "NULL" << endl;
				break;
			case CMGERenderEngine::VECTOR4:
				if (data != NULL)
					((Vector4*)data)->output();
				else
					cout << "NULL" << endl;
				break;
			case CMGERenderEngine::MATRIX4X4:
				if (data != NULL)
					((Matrix4x4*)data)->output();
				else
					cout << "NULL" << endl;
				break;
			case CMGERenderEngine::INT:
				if (data != NULL)
					cout << *((int*)data) << endl;
				else
					cout << "NULL" << endl;
				break;
			case CMGERenderEngine::BOOL:
				break;
			case CMGERenderEngine::TEXTURE2D:
				if (data != NULL)
					cout << ((Texture2D*)data)->name << endl;
				else
					cout << "NULL" << endl;
				break;
			case CMGERenderEngine::TEXTURE_CUBE:
				if (data != NULL)
					cout << ((CubeTexture*)data)->name << endl;
				else
					cout << "NULL" << endl;
				break;
			case CMGERenderEngine::TEXTURE3D:
				break;
			default:
				break;
			}
		}
	};

	class Material :public Resource
	{
	public:
		void SetShader(Shader * s);
		void SetMaterialParam(string name,void* data);
		void Update();
		void Init();
		void Load(string path);
		static void GlobalStartUp();
		Shader* GetShader();
		GLuint GetRenderQueue();
		void SetRenderQueue(GLuint RQ);
		void SetColor(Vector4 v);
		void output() 
		{
			cout << mShader->name << endl;
			for (int i = 0; i < mMaterialParam.size(); i++)
				mMaterialParam[i]->output();
		}

		void Load(string path, string name_, string mformat_)
		{
			name = name_;
			mformat = mformat_;
			Load(path);
			Init();
		}

		Vector4 MainColor;
	private:

		void SetFloat(string name, float f,bool isCMGE);
		void SetInt(string name, int i,bool isCMGE);
		void SetVector2(string name, Vector2 v,bool isCMGE);
		void SetVector3(string name, Vector3 v,bool isCMGE);
		void SetVector4(string name, Vector4 v,bool isCMGE);
		void SetMatrix4x4(string name, Matrix4x4 m,bool isCMGE);
		void SetTexture2D(string name, Texture2D t,bool isCMGE);
		void SetTexture_Cube(string name, CubeTexture t, bool isCMGE);

		Shader * mShader;
		GLuint cacheIndex;
		string cacheString;
		GLuint cacheTexIndex;
		GLuint mRenderQueue;
		vector<MaterialParam*> mMaterialParam;
	};
}