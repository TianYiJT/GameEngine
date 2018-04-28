#include"Renderer.h"
GLuint Renderer::VAO[1] = {999999};
GLuint Renderer::VBO[1] = {999999};
//GLuint Renderer::EBO = 999999;
Renderer::Renderer() 
{
	Component::Component();
}

Renderer::~Renderer() 
{

}

void Renderer::SetMaterial(Material * m) 
{
	mMaterial = m;
}

Material* Renderer::GetMaterial()
{
	return mMaterial;
}

void Renderer::Start() 
{
	
}

void Renderer::Update()
{
	Camera * tempMain = Camera::MainCamera;
	Texture2D * main_ = tempMain->GetMainTex();
	Texture2D * depth_ = tempMain->GetDepthTex();
	/*
	for(int i=0;i<main->GetWidth();i++)
	{
		for(int j=0;j<main->GetHeight();j++)
		{
			main->GetPixel(Vector2(i, j)).output();
		}
	}
	*/
	// 光照还没加上
	mMaterial->SetMaterialParam("CMGE_MAINTEX",main_);
	mMaterial->SetMaterialParam("CMGE_DEPTHTEX",depth_);

	//mMaterial->output();

	Draw(1);
}
