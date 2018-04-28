#include"Shader.h"
#include <fstream>  
#include<queue>
#include<string>
#include"CMGEStringEngine.h"
#include"LoadShaders.h"
using namespace CMGERenderEngine;
using namespace CMGEStringEngine;

map<string, ShaderParamType> Shader::mapShaderParamType = map<string, ShaderParamType>();
map<ShaderParamType, string> Shader::mapInverseShaderParamType = map<ShaderParamType, string>();
map<string, CullMode> Shader::mapCullMode = map<string, CullMode>();
map<string, DepthMode> Shader::mapDepthMode = map<string, DepthMode>();
map<string, BlendFactor> Shader::mapBlendFactor = map<string, BlendFactor>();
GLenum* Shader::TexEnum = new GLenum[32];
map<BlendFactor, GLenum> Shader::mapBlendFactorGL = map<BlendFactor, GLenum>();


void Shader::output() 
{
	ofstream out1;
	out1.open("Phong.vert", ios::out);
	out1 << mVertexScript;

	ofstream out2;
	out2.open("Phong.frag", ios::out);
	out2 << mFragmentScript;
}


GLuint Shader::LoadShader(const char * context, GLuint _type)
{
	GLuint shader_code = glCreateShader(_type);
	glShaderSource(shader_code, 1, &context, NULL);
	glCompileShader(shader_code);
	return shader_code;
}

GLuint Shader::GetProgram() 
{
	return Program;
}

void Shader::GlobalStartUp() 
{

	mapBlendFactorGL.insert(pair<BlendFactor,GLenum>(ONE,GL_ONE));
	mapBlendFactorGL.insert(pair<BlendFactor, GLenum>(ZERO, GL_ZERO));
	mapBlendFactorGL.insert(pair<BlendFactor,GLenum>(ONE_MINUS_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA));
	mapBlendFactorGL.insert(pair<BlendFactor, GLenum>(ONE_MINUS_DST_COLOR, GL_ONE_MINUS_DST_COLOR));
	mapBlendFactorGL.insert(pair<BlendFactor, GLenum>(ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	mapBlendFactorGL.insert(pair<BlendFactor, GLenum>(SRC_ALPHA, GL_SRC_ALPHA));
	mapBlendFactorGL.insert(pair<BlendFactor, GLenum>(SRC_ALPHA_SATURATE, GL_SRC_ALPHA_SATURATE));
	mapBlendFactorGL.insert(pair<BlendFactor, GLenum>(DST_COLOR, GL_DST_COLOR));
	mapBlendFactorGL.insert(pair<BlendFactor, GLenum>(DST_ALPHA, GL_DST_ALPHA));

	mapShaderParamType.insert(pair<string, ShaderParamType>("int",INT));
	mapShaderParamType.insert(pair<string, ShaderParamType>("float", FLOAT));
	mapShaderParamType.insert(pair<string, ShaderParamType>("bool", BOOL));
	mapShaderParamType.insert(pair<string, ShaderParamType>("Vector2", VECTOR2));
	mapShaderParamType.insert(pair<string, ShaderParamType>("Vector3", VECTOR3));
	mapShaderParamType.insert(pair<string, ShaderParamType>("Vector4", VECTOR4));
	mapShaderParamType.insert(pair<string, ShaderParamType>("Matrix4x4", MATRIX4X4));
	mapShaderParamType.insert(pair<string, ShaderParamType>("Texture2D", TEXTURE2D));
	mapShaderParamType.insert(pair<string, ShaderParamType>("Texture3D", TEXTURE3D));
	mapShaderParamType.insert(pair<string, ShaderParamType>("Texture_Cube", TEXTURE_CUBE));

	mapInverseShaderParamType.insert(pair<ShaderParamType, string>(INT, "int"));
	mapInverseShaderParamType.insert(pair<ShaderParamType, string>(FLOAT,"float"));
	mapInverseShaderParamType.insert(pair<ShaderParamType, string>(BOOL, "bool"));
	mapInverseShaderParamType.insert(pair<ShaderParamType, string>(VECTOR2, "vec2"));
	mapInverseShaderParamType.insert(pair<ShaderParamType, string>(VECTOR3, "vec3"));
	mapInverseShaderParamType.insert(pair<ShaderParamType, string>(VECTOR4, "vec4"));
	mapInverseShaderParamType.insert(pair<ShaderParamType, string>(MATRIX4X4, "mat4"));
	mapInverseShaderParamType.insert(pair<ShaderParamType, string>(TEXTURE2D, "sampler2D"));
	mapInverseShaderParamType.insert(pair<ShaderParamType, string>(TEXTURE3D, "sampler3D"));
	mapInverseShaderParamType.insert(pair<ShaderParamType, string>(TEXTURE_CUBE, "samplerCube"));

	mapCullMode.insert(pair<string,CullMode>("off",CULLOFF));
	mapCullMode.insert(pair<string, CullMode>("back", CULLBACK));
	mapCullMode.insert(pair<string, CullMode>("front", CULLFRONT));


	mapDepthMode.insert(pair<string,DepthMode>("TestOff",NOTTEST));
	mapDepthMode.insert(pair<string, DepthMode>("WriteOff", NOTWRITE));
	mapDepthMode.insert(pair<string, DepthMode>("On", WRITE));


	mapBlendFactor.insert(pair<string,BlendFactor>("ONE",ONE));
	mapBlendFactor.insert(pair<string, BlendFactor>("ZERO", ZERO));
	mapBlendFactor.insert(pair<string, BlendFactor>("DST_COLOR", DST_COLOR));
	mapBlendFactor.insert(pair<string, BlendFactor>("DST_ALPHA", DST_ALPHA));
	mapBlendFactor.insert(pair<string, BlendFactor>("ONE_MINUS_DST_ALPHA", ONE_MINUS_DST_ALPHA));
	mapBlendFactor.insert(pair<string, BlendFactor>("ONE_MINUS_DST_COLOR", ONE_MINUS_DST_COLOR));
	mapBlendFactor.insert(pair<string, BlendFactor>("ONE_MINUS_SRC_ALPHA", ONE_MINUS_SRC_ALPHA));
	mapBlendFactor.insert(pair<string, BlendFactor>("SRC_ALPHA", SRC_ALPHA));
	mapBlendFactor.insert(pair<string, BlendFactor>("SRC_ALPHA_SATURATE", SRC_ALPHA_SATURATE));

	TexEnum[0] = GL_TEXTURE0;
	TexEnum[1] = GL_TEXTURE1;
	TexEnum[2] = GL_TEXTURE2;
	TexEnum[3] = GL_TEXTURE3;
	TexEnum[4] = GL_TEXTURE4;
	TexEnum[5] = GL_TEXTURE5;
	TexEnum[6] = GL_TEXTURE6;
	TexEnum[7] = GL_TEXTURE7;

	TexEnum[8] = GL_TEXTURE8;
	TexEnum[9] = GL_TEXTURE9;
	TexEnum[10] = GL_TEXTURE10;
	TexEnum[11] = GL_TEXTURE11;
	TexEnum[12] = GL_TEXTURE12;
	TexEnum[13] = GL_TEXTURE13;
	TexEnum[14] = GL_TEXTURE14;
	TexEnum[15] = GL_TEXTURE15;

	TexEnum[16] = GL_TEXTURE16;
	TexEnum[17] = GL_TEXTURE17;
	TexEnum[18] = GL_TEXTURE18;
	TexEnum[19] = GL_TEXTURE19;
	TexEnum[20] = GL_TEXTURE20;
	TexEnum[21] = GL_TEXTURE21;
	TexEnum[22] = GL_TEXTURE22;
	TexEnum[23] = GL_TEXTURE23;

	TexEnum[24] = GL_TEXTURE24;
	TexEnum[25] = GL_TEXTURE25;
	TexEnum[26] = GL_TEXTURE26;
	TexEnum[27] = GL_TEXTURE27;
	TexEnum[28] = GL_TEXTURE28;
	TexEnum[29] = GL_TEXTURE29;
	TexEnum[30] = GL_TEXTURE30;
	TexEnum[31] = GL_TEXTURE31;
}

GLuint Shader::GetParamIndex(string s,bool type)
{
	if (type) 
	{
		GLuint index = -1;
		for (int i = 0; i < mCMGEVariety.size(); i++) 
		{
			if (s == mCMGEVariety[i].mName) 
			{
				index = mCMGEVariety[i].mIndex;
				break;
			}
		}
		return index;
	}
	else
	{
		GLuint index = -1;
		for (int i = 0; i < mShaderVariety.size(); i++)
		{
			if (s == mShaderVariety[i].mName)
			{
				index = mShaderVariety[i].mIndex;
				break;
			}
		}
		return index;
	}
}

GLuint Shader::GetParamTexIndex(string s, bool type)
{
	if (type)
	{
		GLuint index = -1;
		for (int i = 0; i < mCMGEVariety.size(); i++)
		{
			if (s == mCMGEVariety[i].mName&&(mCMGEVariety[i].mShaderParamType==TEXTURE2D|| mCMGEVariety[i].mShaderParamType == TEXTURE3D)||mCMGEVariety[i].mShaderParamType==TEXTURE_CUBE)
			{
				index = mCMGEVariety[i].IndexForTex;
				break;
			}
		}
		return index;
	}
	else
	{
		GLuint index = -1;
		for (int i = 0; i < mShaderVariety.size(); i++)
		{
			if (s == mShaderVariety[i].mName && (mShaderVariety[i].mShaderParamType == TEXTURE2D || mShaderVariety[i].mShaderParamType == TEXTURE3D)||mShaderVariety[i].mShaderParamType==TEXTURE_CUBE)
			{
				index = mShaderVariety[i].IndexForTex;
				break;
			}
		}
		return index;
	}
}

Shader::~Shader()
{
	delete[] mVertexScript;
	delete[] mFragmentScript;
	delete[] ShaderScript;
	delete[] uniformchar;
}

Shader::Shader()
{
	mVertexScript = new char[10000];
	mFragmentScript = new char[50000];
	ShaderScript = new char[60000];
	uniformchar = new char[10000];
}

void Shader::Load(string path) 
{
	Resource::Load(path);
	FILE *fp;
	int count = 0;
	if (path != "")
	{
		fp = fopen(path.c_str(), "rt");
		if (fp != NULL)
		{
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			if (count>0)
			{
				ShaderScript = (char*)malloc(sizeof(char)*(count + 1));
				count = fread(ShaderScript, sizeof(char), count, fp);
				ShaderScript[count] = '\0';
			}
			fclose(fp);
		}
	}
}

void Shader::ProcessBlend(vector<string> s)
{
	string key1 = s[1];
	string key2 = s[2];
	srcFactor = mapBlendFactor[key1];
	desFactor = mapBlendFactor[key2];
	mBlendMode = BLEND;
}

void Shader::ProcessCull(vector<string> s) 
{
	string key1 = s[1];
	mCullMode = mapCullMode[key1];
}

void Shader::ProcessDepth(vector<string> s) 
{
	string key1 = s[1];
	mDepthMode = mapDepthMode[key1];
}

void Shader::ProcessParam(vector<string> s) 
{
	string key1 = s[0];
	ShaderParamType tempType= mapShaderParamType[key1];
	ShaderParam tempSP;
	tempSP.mShaderParamType = tempType;
	tempSP.mName = s[1];
	if (tempSP.mShaderParamType == TEXTURE2D || tempSP.mShaderParamType == TEXTURE3D||tempSP.mShaderParamType==TEXTURE_CUBE) 
	{
		tempSP.IndexForTex = tempTexAlloc;
		tempTexAlloc++;
	}
	mShaderVariety.push_back(tempSP);
}

void Shader::copyToUniformchar(ShaderParam sp) 
{
	string s = "uniform ";
	s += mapInverseShaderParamType[sp.mShaderParamType];
	s += " ";
	s += sp.mName;
	s += ";";
	s += "\n";
	for (int i = 0; i < s.size(); i++) 
	{
		uniformchar[tempLength] = s[i];
		tempLength++;
	}
}

void Shader::copyToVertexScript(string s) 
{
	for (int i = 0; i < s.size(); i++)
	{
		mVertexScript[tempLength_vertex] = s[i];
		tempLength_vertex++;
	}
}

void Shader::copyToVFragScript(string s)
{
	for (int i = 0; i < s.size(); i++)
	{
		mFragmentScript[tempLength_frag] = s[i];
		tempLength_frag++;
	}
}

void Shader::Start() 
{
	Program = glCreateProgram();

	GLuint vertex_shader = LoadShader(mVertexScript,GL_VERTEX_SHADER);
	GLuint frag_shader = LoadShader(mFragmentScript,GL_FRAGMENT_SHADER);

	//glgetprre

	glAttachShader(Program,vertex_shader);
	glAttachShader(Program, frag_shader);

	glLinkProgram(Program);

	glUseProgram(Program);
	for (int i = 0; i < mCMGEVariety.size(); i++) 
	{
		mCMGEVariety[i].mIndex = glGetUniformLocation(Program,mCMGEVariety[i].mName.c_str());
	}

	for (int i = 0; i < mShaderVariety.size(); i++)
	{
		mShaderVariety[i].mIndex = glGetUniformLocation(Program, mShaderVariety[i].mName.c_str());
	}
}

void Shader::Init() 
{
	ShaderParam sp_mvp(MATRIX4X4,"CMGE_MVP");
	ShaderParam sp_o2w(MATRIX4X4, "CMGE_O2W");
	ShaderParam sp_w2o(MATRIX4X4, "CMGE_W2O");
	ShaderParam sp_s2w(MATRIX4X4, "CMGE_S2W");
	ShaderParam sp_w2s(MATRIX4X4, "CMGE_W2S");

	ShaderParam sp_view(VECTOR3, "CMGE_VIEW");
	ShaderParam sp_Screen(VECTOR2,"CMGE_SCREEN");
	ShaderParam sp_lod(INT, "CMGE_LOD");
	ShaderParam sp_maincolor(VECTOR4, "CMGE_MAIN_COLOR");
	ShaderParam sp_prerender(INT,"CMGE_PRE_RENDER_OR_NOT");

	ShaderParam sp_depth(TEXTURE2D, "CMGE_DEPTHTEX");
	sp_depth.IndexForTex = 0;
	ShaderParam sp_mainTex(TEXTURE2D, "CMGE_MAINTEX");
	sp_mainTex.IndexForTex = 1;

	ShaderParam sp_light0(MATRIX4X4,"CMGE_LIGHT0");
	ShaderParam sp_light1(MATRIX4X4, "CMGE_LIGHT1");
	ShaderParam sp_light2(MATRIX4X4, "CMGE_LIGHT2");
	ShaderParam sp_light3(MATRIX4X4, "CMGE_LIGHT3");
	ShaderParam sp_light4(MATRIX4X4, "CMGE_LIGHT4");
	ShaderParam sp_light5(MATRIX4X4, "CMGE_LIGHT5");
	ShaderParam sp_light6(MATRIX4X4, "CMGE_LIGHT6");

	mCMGEVariety.push_back(sp_mvp);
	mCMGEVariety.push_back(sp_o2w);
	mCMGEVariety.push_back(sp_w2o);
	mCMGEVariety.push_back(sp_s2w);
	mCMGEVariety.push_back(sp_w2s);

	mCMGEVariety.push_back(sp_view);
	mCMGEVariety.push_back(sp_Screen);
	mCMGEVariety.push_back(sp_lod);
	mCMGEVariety.push_back(sp_prerender);
	mCMGEVariety.push_back(sp_maincolor);

	mCMGEVariety.push_back(sp_depth);
	mCMGEVariety.push_back(sp_mainTex);

	mCMGEVariety.push_back(sp_light0);
	mCMGEVariety.push_back(sp_light1);
	mCMGEVariety.push_back(sp_light2);
	mCMGEVariety.push_back(sp_light3);
	mCMGEVariety.push_back(sp_light4);
	mCMGEVariety.push_back(sp_light5);
	mCMGEVariety.push_back(sp_light6);

	ResourceManager<Shader>::getInstance()->registerData(this,name);
}

void Shader::Transfer() 
{
	queue<string> queue_uniform;
	queue<string> queue_vertex;
	queue<string> queue_frag;
	bool uniform_=false, vertex_=false, frag_ = false;
	int lastIndex = 0;
	string s = "";
	for (int i = 0; i < 60000; i++)
	{
		if (ShaderScript[i] == '\0')
			break;
		else
		{
			if (ShaderScript[i] == '\n') 
			{
				vector<string> temp = Stringf::Participle(s);
				if (temp.size() != 0) 
				{
					string key1 = temp[0];
					if (key1 == "#uniform")
					{
						uniform_ = true;
					}
					else if (key1 == "#vertex")
					{
						vertex_ = true;
					}
					else if (key1 == "#fragment")
					{
						frag_ = true;
					}
					else if (key1 == "#enduniform")
					{
						uniform_ = false;
					}
					else if (key1 == "#endvertex")
					{
						vertex_ = false;
					}
					else if (key1 == "#endfragment")
					{
						frag_ = false;
					}
					else
					{
						if (uniform_)
							queue_uniform.push(s);
						if (vertex_)
							queue_vertex.push(s);
						if (frag_)
							queue_frag.push(s);
					}
				}
				s = "";
			}
			else
			{
				s += ShaderScript[i];
			}
		}
	}

	while (!queue_uniform.empty()) 
	{
		string temp = queue_uniform.front();
		queue_uniform.pop();
		vector<string> vstemp = Stringf::Participle(temp);
		if (vstemp[0] == "Blend")
			ProcessBlend(vstemp);
		else if (vstemp[0] == "Cull")
			ProcessCull(vstemp);
		else if (vstemp[0] == "Depth")
			ProcessDepth(vstemp);
		else
			ProcessParam(vstemp);
	}
	for (int i = 0; i < mShaderVariety.size(); i++) 
	{
		copyToUniformchar(mShaderVariety[i]);
	}
	for (int i = 0; i < mCMGEVariety.size(); i++)
	{
		copyToUniformchar(mCMGEVariety[i]);
	}
	string version = "#version 420 core";
	version += '\n';
	string vertex = "layout(location = 0) in vec3 pos;";
	vertex += '\n';
	string normal = "layout(location = 1) in vec3 nor;";
	normal += '\n';
	string uv = "layout(location = 2) in vec2 uv;";
	uv += '\n';
	string uvnormal = "layout(location = 3) in vec2 uvnormal;";
	uvnormal += '\n';
	string tangent = "layout(location = 4) in vec3 tangent;";
	tangent += '\n';
	string color = "layout(location = 5) in vec4 color;";
	color += '\n';
	string output_color = "layout(location=0) out vec4 col;";
	output_color += '\n';
	copyToVertexScript(version);
	copyToVFragScript(version);
	copyToVFragScript(output_color);
	copyToVertexScript(vertex);
	copyToVertexScript(normal);
	copyToVertexScript(uv);
	copyToVertexScript(uvnormal);
	copyToVertexScript(tangent);
	copyToVertexScript(color);

	for (int i = 0; i < tempLength; i++) 
	{
		mVertexScript[tempLength_vertex] = uniformchar[i];
		mFragmentScript[tempLength_frag] = uniformchar[i];
		tempLength_vertex++;
		tempLength_frag++;
	}
	while (!queue_vertex.empty()) 
	{
		string temp = queue_vertex.front();
		queue_vertex.pop();
		copyToVertexScript(temp);
		mVertexScript[tempLength_vertex] = '\n';
		tempLength_vertex++;
	}
	while (!queue_frag.empty())
	{
		string temp = queue_frag.front();
		queue_frag.pop();
		copyToVFragScript(temp);
		mFragmentScript[tempLength_frag] = '\n';
		tempLength_frag++;
	}
//	cout << mVertexScript;
}