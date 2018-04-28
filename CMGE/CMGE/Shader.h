#ifndef SHADER_H

#define SHADER_H
#include<stdio.h>  
#include<stdlib.h>
#include"CMGEMathEngine.h"
#include<glew.h> 
#include"freeglut.h"
#include<vector>
#include<string>
#include<map>
#include"Resource.h"
#include"ResourceManager.h"

using namespace CMGEMathEngine;
using namespace std;
namespace CMGERenderEngine 
{
	enum ShaderParamType :unsigned int
	{
		FLOAT = 0,
		VECTOR2,
		VECTOR3,
		VECTOR4,
		MATRIX4X4,
		INT,
		BOOL,
		TEXTURE2D,
		TEXTURE3D,
		TEXTURE_CUBE
	};
	enum DepthMode:unsigned int
	{
		NOTTEST = 0,
		NOTWRITE,
		WRITE
	};

	enum BlendMode:unsigned int
	{
		NOTBLEND = 0,
		BLEND
	};

	enum BlendFactor:unsigned int
	{
		DST_ALPHA = 0,
		DST_COLOR,
		ONE,
		ONE_MINUS_DST_ALPHA,
		ONE_MINUS_DST_COLOR,
		ONE_MINUS_SRC_ALPHA,
		SRC_ALPHA,
		SRC_ALPHA_SATURATE,
		ZERO
	};

	enum CullMode:unsigned int
	{
		CULLOFF = 0,
		CULLBACK,
		CULLFRONT
	};

	struct ShaderParam
	{
		GLuint mIndex;
		ShaderParamType mShaderParamType;
		string mName;

		GLuint IndexForTex;

		ShaderParam() 
		{
			IndexForTex = -1;
		}

		ShaderParam(GLuint Index, ShaderParamType shaderparamtype, string Name)
		{
			mIndex = Index;
			mShaderParamType = shaderparamtype;
			mName = Name;
			IndexForTex = -1;
		}

		ShaderParam(ShaderParamType spt, string name) 
		{
			mShaderParamType = spt;
			mName = name;
			IndexForTex = -1;
		}

	};


	class Shader:public Resource
	{
	public:
		static  map<string, ShaderParamType> mapShaderParamType;
		static  map<ShaderParamType, string> mapInverseShaderParamType;
		static  map<string, CullMode> mapCullMode;
		static  map<string, DepthMode> mapDepthMode;
		static  map<string, BlendFactor> mapBlendFactor;
		static  map<BlendFactor, GLenum> mapBlendFactorGL;
		static  GLenum * TexEnum;
		Shader();
		~Shader();
		void Load(string path);
		void Transfer();
		void Init();
		void Start();

		void Load(string path, string name_, string mformat_) 
		{
			setName(name_);
			setFormat(mformat_);
			Init();
			Load(path);
			Transfer();
			Start();
		}

		void output();

		GLuint GetProgram();
		GLuint GetParamIndex(string s,bool type);
		GLuint GetParamTexIndex(string s,bool type);

		static void GlobalStartUp();

		CullMode mCullMode;
		DepthMode mDepthMode;
		BlendMode mBlendMode;
		BlendFactor srcFactor;
		BlendFactor desFactor;

		vector<ShaderParam> mCMGEVariety;
		vector<ShaderParam> mShaderVariety;

	private:

		void ProcessCull(vector<string> s);
		void ProcessDepth(vector<string> s);
		void ProcessBlend(vector<string> s);
		void ProcessParam(vector<string> s);
		void copyToUniformchar(ShaderParam sp);
		void copyToVertexScript(string s);
		void copyToVFragScript(string s);
		static GLuint LoadShader(const char * context, GLuint _type);

		char* ShaderScript;
		char* mVertexScript;
		char* mFragmentScript;
		char* uniformchar;
		int tempLength=0;
		int tempLength_vertex=0;
		int tempLength_frag=0;
		GLuint Program; 


		GLuint tempTexAlloc = 2;
	};


}
#endif // !SHADER_H