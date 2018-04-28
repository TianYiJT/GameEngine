#include"Camera.h"

Camera* Camera::MainCamera = new Camera();

Camera::Camera()
{
	ScreenHeight = 600.0f;
	ScreenWidth = 800.0f;
	aspect = ScreenWidth / ScreenHeight;

	MainColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	nearClipPlane = 1.0f;
	farClipPlane = 500.0f;
	Component::Component();
}

Camera::~Camera() 
{
	
}

void Camera::SetFrameMode(bool isFLOAT) 
{
	if (isFLOAT) 
	{
		frameTex->mTextureType = TextureType::FLOATFRAME;
		MainTex->mTextureType = TextureType::FLOATFRAME;
	}
	else 
	{
		frameTex->mTextureType = TextureType::COLOR;
		MainTex->mTextureType = TextureType::COLOR;
	}
}

void Camera::Start()
{
	frameTex = new Texture2D();
	frameTex->name = "CMGE_FrameTex";
	frameTex->mTextureType = TextureType::COLOR;
	DepthTex = new Texture2D();
	DepthTex->name = "CMGE_DepthTex";
	DepthTex->mTextureType = TextureType::DEPTH;
	MainTex = new Texture2D();
	MainTex->name = "CMGE_MainTex";
	MainTex->mTextureType = TextureType::COLOR;

	frameTex->SetPixels(NULL, ScreenWidth, ScreenHeight);
	frameTex->Init();
	DepthTex->SetPixels(NULL, ScreenWidth, ScreenHeight);
	DepthTex->Init();
	MainTex->SetPixels(NULL, ScreenWidth, ScreenHeight);
	MainTex->Init();
	//cout << MainTex->GetIndex() << endl;

	glGenRenderbuffers(1,&renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
		ScreenWidth, ScreenHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenRenderbuffers(1, &Rbo_Depth);
	glBindRenderbuffer(GL_RENDERBUFFER, Rbo_Depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
		ScreenWidth, ScreenHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameTex->GetIndex(), 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, renderbuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenFramebuffers(1, &framebuffer_Main);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_Main);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, MainTex->GetIndex(), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthTex->GetIndex(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Camera::SetScreenVector(Vector2 v)
{
	ScreenHeight = v.y;
	ScreenWidth = v.x;
	aspect = ScreenWidth / ScreenHeight;

	frameTex->SetPixels(NULL, ScreenWidth, ScreenHeight);
	MainTex->SetPixels(NULL,ScreenWidth,ScreenHeight);
	DepthTex->SetPixels(NULL,ScreenWidth,ScreenHeight);

	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
		ScreenWidth, ScreenHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, Rbo_Depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
		ScreenWidth, ScreenHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Camera::FrameStart(bool isMain) 
{
	if(!isMain)
		glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
	else 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_Main);
	}
}

void Camera::FrameEnd(bool isMain) 
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	if (!isMain)
	{
		frameTex->frameReturnRead();
	}
	else
	{
		//glTexBuffer(GL_TEXTURE_2D, GL_DEPTH_ATTACHMENT, Rbo_Depth);
		
		DepthTex->frameReturnRead();
		MainTex->frameReturnRead();
	}
}

Texture2D* Camera::GetFrame() 
{
	return frameTex;
}

void Camera::SetMainCamera() 
{
	if(Camera::MainCamera!=this)
	{
		Camera::MainCamera = this;
	}
}


Vector3 Camera::WorldToScreenPoint(Vector3 v) 
{
	Vector4 v1(v.x,v.y,v.z,1.0f);
	Vector4 v2 = worldToCameraMatrix*cameraToScreenMatrix*v1;
	Vector3 v3(v2.x,v2.y,v2.z);
	return v3;
}

Vector3 Camera::ScreenToWorldPoint(Vector3 v)
{
	Vector4 v1(v.x, v.y, v.z, 1.0f);
	Vector4 v2 = ScreenToCameraMatrix*cameraToWorldMatrix*v1;
	Vector3 v3(v2.x, v2.y, v2.z);
	return v3;
}

Texture2D* Camera::GetDepthTex() 
{
	return DepthTex;
}

Texture2D* Camera::GetMainTex() 
{
	return MainTex;
}

void Camera::PostReRender()
{
	glViewport(0, 0, ScreenWidth,ScreenHeight);
	glClearColor(MainColor.x, MainColor.y, MainColor.z, MainColor.w);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < RenderGameObject.size(); i++)
	{
		if (RenderGameObject[i]->isEnable)
			RenderGameObject[i]->Render();
	}
}

void Camera::Update() 
{
	if (this == Camera::MainCamera) 
	{
		Vector3 camerapos = ((GameObject*)mGameObject)->transform.Position;
		Vector3 forward = ((GameObject*)mGameObject)->transform.forward();
		Vector3 up = ((GameObject*)mGameObject)->transform.up();
		Vector3 at = camerapos + forward;
		worldToCameraMatrix = Matrix4x4::LookAt(camerapos,at,up);
	//	worldToCameraMatrix.output();
		cameraToWorldMatrix = worldToCameraMatrix.inverse();
		if (OrthoOrNot) 
		{
			cameraToScreenMatrix = Matrix4x4::Ortho(-1.0f,1.0f,-aspect,aspect,nearClipPlane,farClipPlane);
			ScreenToCameraMatrix = cameraToScreenMatrix.inverse();
		}
		else
		{
			cameraToScreenMatrix = Matrix4x4::Frustum(-1.0f, 1.0f, -aspect, aspect, nearClipPlane, farClipPlane);
			ScreenToCameraMatrix = cameraToScreenMatrix.inverse();
		}
	}
}