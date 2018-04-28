#pragma once
#include"Component.h"
#include"Material.h"
#include"glew.h"
#include<freeglut.h>
#include"GameObject.h"
#include"Camera.h"
#define BUFFER_OFFSET(offset) ((GLvoid*) (NULL + offset))
using namespace CMGEComponent;
using namespace CMGERenderEngine;
namespace CMGERenderEngine
{
	class Renderer:public Component 
	{
	public:
		Renderer();
		~Renderer();
		static void* createComponent()
		{
			return new Renderer;
		}
		static void RendererInit() 
		{
			glGenVertexArrays(1, VAO);
			glBindVertexArray(VAO[0]);
			glGenBuffers(1, VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		//	glGenBuffers(1, &EBO);
		//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		}
		void Init()
		{
			ClassName = "Renderer";
			fatherName.push_back("Component");
			ComponentFactory::getInstance().registClass(ClassName, (CreateComponent)createComponent);
		}
		virtual void Draw(int preornot) 
		{
			DepthMode dm = mMaterial->GetShader()->mDepthMode;
			CullMode cm = mMaterial->GetShader()->mCullMode;
			BlendMode bm = mMaterial->GetShader()->mBlendMode;
			BlendFactor des = mMaterial->GetShader()->desFactor;
			BlendFactor src = mMaterial->GetShader()->srcFactor;

			switch (dm)
			{
			case CMGERenderEngine::NOTTEST:
				glDisable(GL_DEPTH_TEST);
				break;
			case CMGERenderEngine::NOTWRITE:
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LEQUAL);
				glDepthMask(GL_FALSE);
				break;
			case CMGERenderEngine::WRITE:
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LEQUAL);
				glDepthMask(GL_TRUE);
				break;
			default:
				break;
			}

			switch (cm)
			{
			case CMGERenderEngine::CULLOFF:
				glDisable(GL_CULL_FACE);
				break;
			case CMGERenderEngine::CULLBACK:
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				break;
			case CMGERenderEngine::CULLFRONT:
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);
				break;
			default:
				break;
			}

			switch (bm)
			{
			case CMGERenderEngine::NOTBLEND:
				glDisable(GL_BLEND);
				break;
			case CMGERenderEngine::BLEND:
				glEnable(GL_BLEND);
				glBlendFunc(Shader::mapBlendFactorGL[src],Shader::mapBlendFactorGL[des]);
				break;
			default:
				break;
			}
			Camera * tempMain = Camera::MainCamera;
			Matrix4x4 MVP = tempMain->cameraToScreenMatrix*tempMain->worldToCameraMatrix*((GameObject*)mGameObject)->transform.Model();
			Matrix4x4 O2W = ((GameObject*)mGameObject)->transform.Model();
			Matrix4x4 W2O = ((GameObject*)mGameObject)->transform.Model().inverse();

			Matrix4x4 S2W = tempMain->cameraToWorldMatrix*tempMain->ScreenToCameraMatrix;
			Matrix4x4 W2S = tempMain->cameraToScreenMatrix*tempMain->worldToCameraMatrix;

			Vector3 view = ((GameObject*)tempMain->GetGameObject())->transform.Position;
			Vector2 screen = Vector2(tempMain->ScreenWidth, tempMain->ScreenHeight);
			int LOD = tempMain->lod(((GameObject*)mGameObject)->transform.Position);
			Vector4 MainColor = mMaterial->MainColor;
			mMaterial->SetMaterialParam("CMGE_MVP", &MVP);
			mMaterial->SetMaterialParam("CMGE_O2W", &O2W);
			mMaterial->SetMaterialParam("CMGE_W2O", &W2O);
			mMaterial->SetMaterialParam("CMGE_S2W", &S2W);
			mMaterial->SetMaterialParam("CMGE_W2S", &W2S);

			mMaterial->SetMaterialParam("CMGE_VIEW", &view);
			mMaterial->SetMaterialParam("CMGE_SCREEN", &screen);
			mMaterial->SetMaterialParam("CMGE_LOD", &LOD);
			mMaterial->SetMaterialParam("CMGE_MAIN_COLOR", &MainColor);
			mMaterial->SetMaterialParam("CMGE_PRE_RENDER_OR_NOT",&preornot);
			mMaterial->Update();
		//	mMaterial->output();
		}
		void SetMaterial(Material* m);
		Material* GetMaterial();
		void Start();
		void Update();
		static GLuint VAO[1];
		static GLuint VBO[1];
	//	static GLuint EBO;

	protected:
		Material *mMaterial;

	private:


	};
};