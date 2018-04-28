#include"Material.h"

void Material::SetMaterialParam(string name,void *data) 
{
	int index = -1;
	for (int i = 0; i < mMaterialParam.size(); i++) 
	{
	//	cout << mMaterialParam[i]->name << endl;
		if (mMaterialParam[i]->name == name)
		{

			index = i;
			break;
		}
	}
	ShaderParamType spt = mMaterialParam[index]->spt;
	float * f = new float();
	Vector2 * v2 = new Vector2();
	Vector3 * v3 = new Vector3();
	Vector4 * v4 = new Vector4();
	int * i_ = new int();
	Matrix4x4 * m = new Matrix4x4();
	bool * b = new bool();
	Texture2D * t = new Texture2D();
	CubeTexture *ct = new CubeTexture();
	switch (spt)
	{
	case CMGERenderEngine::FLOAT:
		*f = *((float*)data);
		mMaterialParam[index]->data = f;
		delete v2; delete v3; delete v4; delete i_; delete m; delete b; delete t; delete ct;
		break;
	case CMGERenderEngine::VECTOR2:
		*v2 = *((Vector2*)data);
		mMaterialParam[index]->data = v2;
		delete f; delete v3; delete v4; delete i_; delete m; delete b; delete t; delete ct;
		break;
	case CMGERenderEngine::VECTOR3:
		*v3 = *((Vector3*)data);
		mMaterialParam[index]->data = v3;
		delete f; delete v2; delete v4; delete i_; delete m; delete b; delete t; delete ct;
		break;
	case CMGERenderEngine::VECTOR4:
		*v4 = *((Vector4*)data);
		mMaterialParam[index]->data = v4;
		delete f; delete v2; delete v3; delete i_; delete m; delete b; delete t; delete ct;
		break;
	case CMGERenderEngine::MATRIX4X4:
		*m = *((Matrix4x4*)data);
		mMaterialParam[index]->data = m;
		delete f; delete v2; delete v3; delete i_; delete v4; delete b; delete t; delete ct;
		break;
	case CMGERenderEngine::INT:
		*i_ = *((int*)data);
		mMaterialParam[index]->data = i_;
		delete f; delete v2; delete v3; delete m; delete v4; delete b; delete t; delete ct;
		break;
	case CMGERenderEngine::BOOL:
		*b = *((bool*)data);
		mMaterialParam[index]->data = b;
		delete f; delete v2; delete v3; delete m; delete v4; delete i_; delete t; delete ct;
		break;
	case CMGERenderEngine::TEXTURE2D:
		*t = *((Texture2D*)data);
		mMaterialParam[index]->data = t;
		delete f; delete v2; delete v3; delete m; delete v4; delete i_; delete b; delete ct;
		break;
	case CMGERenderEngine::TEXTURE_CUBE:
		*ct = *((CubeTexture*)data);
	//	cout << "TRUE";
		mMaterialParam[index]->data = ct;
		delete f; delete v2; delete v3; delete m; delete v4; delete i_; delete b; delete t;
		break;
	case CMGERenderEngine::TEXTURE3D:
		break;
	default:
		break;
	}
}


void Material::Update() 
{
	for (int i = 0; i < mMaterialParam.size(); i++) 
	{
		ShaderParamType spt = mMaterialParam[i]->spt;
	//	cout << i << endl;
		switch (spt)
		{
		case CMGERenderEngine::FLOAT:
			if(mMaterialParam[i]->data!=NULL)
				SetFloat(mMaterialParam[i]->name,*((float*)mMaterialParam[i]->data),mMaterialParam[i]->isCMGE);
			break;
		case CMGERenderEngine::VECTOR2:
			if (mMaterialParam[i]->data != NULL)
				SetVector2(mMaterialParam[i]->name, *((Vector2*)mMaterialParam[i]->data),mMaterialParam[i]->isCMGE);
			break;
		case CMGERenderEngine::VECTOR3:
			if (mMaterialParam[i]->data != NULL)
				SetVector3(mMaterialParam[i]->name, *((Vector3*)mMaterialParam[i]->data),mMaterialParam[i]->isCMGE);
			break;
		case CMGERenderEngine::VECTOR4:
			if (mMaterialParam[i]->data != NULL) 
			{
				//((Vector4*)mMaterialParam[i]->data)->output();
				SetVector4(mMaterialParam[i]->name, *((Vector4*)mMaterialParam[i]->data),mMaterialParam[i]->isCMGE);
			}
			break;
		case CMGERenderEngine::MATRIX4X4:
			if (mMaterialParam[i]->data != NULL)
				SetMatrix4x4(mMaterialParam[i]->name, *((Matrix4x4*)mMaterialParam[i]->data),mMaterialParam[i]->isCMGE);
			break;
		case CMGERenderEngine::INT:
			if (mMaterialParam[i]->data != NULL)
				SetInt(mMaterialParam[i]->name, *((int*)mMaterialParam[i]->data),mMaterialParam[i]->isCMGE);
			break;
		case CMGERenderEngine::BOOL:
			//SetBool(mMaterialParam[i]->name, *((Matrix4x4*)mMaterialParam[i]->data));
			break;
		case CMGERenderEngine::TEXTURE2D:
			if (mMaterialParam[i]->data != NULL) {
				SetTexture2D(mMaterialParam[i]->name, *((Texture2D*)mMaterialParam[i]->data), mMaterialParam[i]->isCMGE);
			}
			break;
		case CMGERenderEngine::TEXTURE_CUBE:
			if (mMaterialParam[i]->data != NULL)
				SetTexture_Cube(mMaterialParam[i]->name, *((CubeTexture*)mMaterialParam[i]->data), mMaterialParam[i]->isCMGE);
			break;
		case CMGERenderEngine::TEXTURE3D:
			break;
		default:
			break;
		}
	}
}

void Material::SetShader(Shader * s)
{

	mShader = s;
	mMaterialParam.clear();
	for (int i = 0; i < mShader->mShaderVariety.size(); i++) 
	{
		MaterialParam *mp = new MaterialParam(mShader->mShaderVariety[i].mShaderParamType,NULL,mShader->mShaderVariety[i].mName,false);
		mMaterialParam.push_back(mp);
	}
	for (int i = 0; i < mShader->mCMGEVariety.size(); i++)
	{
		MaterialParam *mp1 = new MaterialParam(mShader->mCMGEVariety[i].mShaderParamType, NULL, mShader->mCMGEVariety[i].mName,true);
		mMaterialParam.push_back(mp1);
	}

}


GLuint Material::GetRenderQueue() 
{
	return mRenderQueue;
}


Shader* Material::GetShader() 
{
	return mShader;
}

void Material::SetFloat(string name,float s,bool isCMGE)
{
	if(name==cacheString)
	{
		glUseProgram(mShader->GetProgram());
		glUniform1f(cacheIndex, s);
	}
	else
	{
		GLuint index = mShader->GetParamIndex(name, isCMGE);
		if(index!=-1)
		{
			cacheIndex = index;
			cacheString = name;
			glUseProgram(mShader->GetProgram());
			glUniform1f(cacheIndex, s);
		}
	}
}

void Material::SetInt(string name,int b,bool isCMGE) 
{
	if (name == cacheString)
	{
		glUseProgram(mShader->GetProgram());
		glUniform1i(cacheIndex,b);
	}
	else
	{
		GLuint index = mShader->GetParamIndex(name, isCMGE);
		if (index != -1)
		{
			cacheIndex = index;
			cacheString = name;
			glUseProgram(mShader->GetProgram());
			glUniform1i(cacheIndex, b);
		}
	}
}

void Material::SetVector2(string name,Vector2 v,bool isCMGE) 
{
	if (name == cacheString)
	{
		glUseProgram(mShader->GetProgram());
		glUniform2f(cacheIndex,v.x,v.y);
	}
	else
	{
		GLuint index = mShader->GetParamIndex(name, isCMGE);
		if (index != -1)
		{
			cacheIndex = index;
			cacheString = name;
			glUseProgram(mShader->GetProgram());
			glUniform2f(cacheIndex, v.x, v.y);
		}
	}
}

void Material::SetVector3(string name, Vector3 v,bool isCMGE)
{
	if (name == cacheString)
	{
		glUseProgram(mShader->GetProgram());
		glUniform3f(cacheIndex,v.x, v.y,v.z);
	}
	else
	{
		GLuint index = mShader->GetParamIndex(name, isCMGE);
		if (index != -1)
		{
			cacheIndex = index;
			cacheString = name;
			glUseProgram(mShader->GetProgram());
			glUniform3f(cacheIndex,v.x, v.y,v.z);
		}
	}
}

void Material::SetVector4(string name, Vector4 v,bool isCMGE)
{
	if (name == cacheString)
	{
		glUseProgram(mShader->GetProgram());
		glUniform4f(cacheIndex, v.x, v.y, v.z,v.w);
	}
	else
	{
		GLuint index = mShader->GetParamIndex(name, isCMGE);
		if (index != -1)
		{
		
			cacheIndex = index;
			cacheString = name;
			glUseProgram(mShader->GetProgram());
			glUniform4f(cacheIndex, v.x, v.y, v.z,v.w);
		}
	}
}

void Material::SetMatrix4x4(string name,Matrix4x4 m,bool isCMGE) 
{
	if (name == cacheString)
	{
		glUseProgram(mShader->GetProgram());
		glUniformMatrix4fv(cacheIndex,1,GL_TRUE,m.data);
	}
	else
	{
		GLuint index = mShader->GetParamIndex(name, isCMGE);
		if (index != -1)
		{
			cacheIndex = index;
			cacheString = name;
			glUseProgram(mShader->GetProgram());
			glUniformMatrix4fv(cacheIndex, 1, GL_TRUE, m.data);
		}
	}
}

void Material::SetTexture2D(string name,Texture2D t,bool isCMGE) 
{
	if (name == cacheString)
	{
		glUseProgram(mShader->GetProgram());
		glActiveTexture(Shader::TexEnum[cacheTexIndex]);
		glBindTexture(GL_TEXTURE_2D,t.GetIndex());
		glUniform1i(cacheIndex,cacheTexIndex);
	}
	else
	{
		GLuint index = mShader->GetParamIndex(name, isCMGE);
		GLuint TexIndex = mShader->GetParamTexIndex(name, isCMGE);
		if (index != -1)
		{
			cacheIndex = index;
			cacheString = name;
			cacheTexIndex = TexIndex;
			glUseProgram(mShader->GetProgram());
			glActiveTexture(Shader::TexEnum[cacheTexIndex]);
			glBindTexture(GL_TEXTURE_2D, t.GetIndex());
			glUniform1i(cacheIndex, cacheTexIndex);
		}
	}
}

void Material::SetColor(Vector4 v) 
{
	MainColor = v;
}

void Material::SetRenderQueue(GLuint RQ)
{
	mRenderQueue = RQ;
}

void Material::Init() 
{
	ResourceManager<Material>::getInstance()->registerData(this,name);
}

void Material::GlobalStartUp() 
{

}
void Material::SetTexture_Cube(string name, CubeTexture t, bool isCMGE)
{
	if (name == cacheString)
	{
		glUseProgram(mShader->GetProgram());
		glActiveTexture(Shader::TexEnum[cacheTexIndex]);
		glBindTexture(GL_TEXTURE_CUBE_MAP, t.GetCubeMapIndex());
		glUniform1i(cacheIndex, cacheTexIndex);
	}
	else
	{
		GLuint index = mShader->GetParamIndex(name, isCMGE);
		GLuint TexIndex = mShader->GetParamTexIndex(name, isCMGE);
		if (index != -1)
		{
			cacheIndex = index;
			cacheString = name;
			cacheTexIndex = TexIndex;
			glUseProgram(mShader->GetProgram());
			glActiveTexture(Shader::TexEnum[cacheTexIndex]);
			glBindTexture(GL_TEXTURE_CUBE_MAP, t.GetCubeMapIndex());
			glUniform1i(cacheIndex, cacheTexIndex);
		}
	}

}
void Material::Load(string path)
{
	Resource::Load(path);

	fstream in;
	in.open(path,ios::in);
	int i=0;
	while (!in.eof()) 
	{
	//	i++;
		//cout << i << endl;
		string temp;
		char tempchar[200];
		in.getline(tempchar,200);
		temp = tempchar;
		vector<string> key = Stringf::Participle(temp);
		if (key.size() == 0)
			continue;
		if(key[0]=="Shader")
		{
			SetShader(ResourceManager<Shader>::getInstance()->getData(key[1]));
		}
		else
		{
			ShaderParamType spt = Shader::mapShaderParamType[key[0]];
			string paraname = key[1];
			Vector2 v1;
			Vector3 v2;
			Vector4 v3;
			Matrix4x4 m;
			float * d = new float[16];
			int i_;
			Texture2D * t;
			CubeTexture * ct;
			float data;
			switch (spt)
			{
			case CMGERenderEngine::FLOAT:
				data = stof(key[2]);
				SetMaterialParam(paraname,&data);
				break;
			case CMGERenderEngine::VECTOR2:
				v1.x = stof(key[2]);
				v1.y = stof(key[3]);
				SetMaterialParam(paraname, &v1);
				break;
			case CMGERenderEngine::VECTOR3:
				v2.x = stof(key[2]);
				v2.y = stof(key[3]);
				v2.z = stof(key[4]);
				SetMaterialParam(paraname, &v2);
				break;
			case CMGERenderEngine::VECTOR4:
				v3.x = stof(key[2]);
				v3.y = stof(key[3]);
				v3.z = stof(key[4]);
				v3.w = stof(key[5]);
				SetMaterialParam(paraname, &v3);
				break;
			case CMGERenderEngine::MATRIX4X4:
				for (int i = 0; i < 16; i++) 
				{
					d[i] = stof(key[2+i]);
				}
				m.data = d;
				SetMaterialParam(paraname, &m);

				break;
			case CMGERenderEngine::INT:
			   i_ = stoi(key[2]);
			   SetMaterialParam(paraname, &i_);
				break;
			case CMGERenderEngine::BOOL:
				break;
			case CMGERenderEngine::TEXTURE2D:
				t = ResourceManager<Texture2D>::getInstance()->getData(key[2]);
				SetMaterialParam(paraname, t);
				break;
			case CMGERenderEngine::TEXTURE_CUBE:
				ct = ResourceManager<CubeTexture>::getInstance()->getData(key[2]);
				SetMaterialParam(paraname, ct);
				break;
			case CMGERenderEngine::TEXTURE3D:
				break;
			default:
				break;
			}

		}
	}
}
