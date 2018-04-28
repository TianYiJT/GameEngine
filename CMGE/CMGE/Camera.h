#pragma once
#include"Component.h"
#include"GameObject.h"
#include"Texture2D.h"
#include"glew.h"
#include<freeglut.h>

using namespace CMGEComponent;
using namespace CMGERenderEngine;
namespace CMGERenderEngine 
{
	class Camera:public Component
	{
	public:
		Camera();
		~Camera();
		void Start();
		void Update();
		void FrameStart(bool isMain);
		void FrameEnd(bool isMain);
		Texture2D* GetFrame();
		Texture2D* GetDepthTex();
		Texture2D* GetMainTex();
		void PostReRender();
		Vector3 WorldToScreenPoint(Vector3 v);
		Vector3 ScreenToWorldPoint(Vector3 v);
		void SetMainCamera();
		void SetScreenVector(Vector2 v);
		static Camera* MainCamera;
		bool OrthoOrNot;
		static void* createComponent()
		{
			return new Camera;
		}
		void Rotate(Vector3 v)
		{
			((GameObject*)mGameObject)->transform.Rotate(v);
		}
		void Init()
		{
			ClassName = "Camera";
			ComponentFactory::getInstance().registClass(ClassName, (CreateComponent)createComponent);
			(fatherName).push_back(string("Component"));
		}
		int lod(Vector3 v)
		{
			float dis = Vector3::Distance(v,((GameObject*)mGameObject)->transform.Position);
			return (dis / farClipPlane)*MAX_LOD;
		}

		Vector2 TransformMousePosition(Vector2 v) 
		{
			v.y = ScreenHeight - v.y;
			Vector2 v1 = Vector2(v.x / ScreenWidth, v.y / ScreenHeight);
			v1 = Vector2(v1.x*2-1,v1.y*2-1);
			return v1;
		}

		void SetFrameMode(bool isFloat);

		float nearClipPlane;
		float farClipPlane;
		Matrix4x4 cameraToWorldMatrix;
		Matrix4x4 cameraToScreenMatrix;
		Matrix4x4 worldToCameraMatrix;
		Matrix4x4 ScreenToCameraMatrix;
		float aspect;
		Vector4 MainColor;
		float ScreenWidth, ScreenHeight;
		vector<GameObject*> RenderGameObject;
	private:
		Texture2D *frameTex;
		GLuint framebuffer;
		GLuint renderbuffer;
		Texture2D* DepthTex;
		Texture2D* MainTex;
		GLuint framebuffer_Main;
		GLuint Rbo_Depth;
		int MAX_LOD = 10;
	};
}

