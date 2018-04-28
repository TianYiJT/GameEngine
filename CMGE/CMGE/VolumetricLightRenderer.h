#pragma once
#include"PostProcessRenderer.h"
#include"CMGEMathEngine.h"

using namespace CMGEMathEngine;

class VolumetricLightRenderer :public PostProcessRenderer 
{
public:
	Vector2 LightPos;

	VolumetricLightRenderer()
	{
		PostProcessRenderer::PostProcessRenderer();
	}

	static void* createComponent()
	{
		return new VolumetricLightRenderer;
	}

	void Init()
	{
		ClassName = "VolumetricLightRenderer";
		for (int i = 0; i < PostProcessRenderer::fatherName.size(); i++)
			fatherName.push_back(PostProcessRenderer::fatherName[i]);
		fatherName.push_back("PostProcessRenderer");
		ComponentFactory::getInstance().registClass(ClassName, (CreateComponent)createComponent);
	}

	void Start()
	{
		PostProcessRenderer::Start();
		Shader * mShaderRadioLight = new Shader();
		mShaderRadioLight->Load("RadioLight.shader","RadioLight","shader");
	//	mShaderRadioLight->output();
		Material * mMaterialRadio = new Material();
		mMaterialRadio->SetShader(mShaderRadioLight);
		mMaterialRadio->SetMaterialParam("LightPos",&LightPos);
		mMaterialRadio->SetMaterialParam("LightColor",&LightColor);
		mMaterialRadio->SetMaterialParam("maxDistance",&maxDistance);
		mRadioLightPPR = new PostProcessRenderer();
		mRadioLightPPR->Init();
		mRadioLightPPR->SetMaterial(mMaterialRadio);
		mRadioLightPPR->enabled = true;
		mRadioPPRObject = new GameObject();
		mRadioPPRObject->AddComponent<PostProcessRenderer>(mRadioLightPPR);

		Shader * mShaderVolumetricLight = new Shader();
		mShaderVolumetricLight->Load("VolumetricLight.shader","VolumetricLight","shader");
		mShaderVolumetricLight->output();
		Material * mMaterialVolumetricLight = new Material();
		mMaterialVolumetricLight->SetShader(mShaderVolumetricLight);
		mMaterialVolumetricLight->SetMaterialParam("NUM_SAMPLES",&NUM_SAMPLES);
		mMaterialVolumetricLight->SetMaterialParam("Decay",&Decay);
		mMaterialVolumetricLight->SetMaterialParam("weight",&weight);
		mMaterialVolumetricLight->SetMaterialParam("Exposure", &Exposure);
		mMaterialVolumetricLight->SetMaterialParam("Density", &Density);
		mMaterialVolumetricLight->SetMaterialParam("lightcoord", &LightPos);
		mMaterial = mMaterialVolumetricLight;
	}

	void Update()
	{
		Camera *temp = Camera::MainCamera;
		temp->FrameStart(false);
		glViewport(0, 0, temp->ScreenWidth, temp->ScreenHeight);
		glClearColor(temp->MainColor.x, temp->MainColor.y, temp->MainColor.z, temp->MainColor.w);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mRadioPPRObject->Update();
		temp->FrameEnd(false);

		mMaterial->SetMaterialParam("RadioLightTex", temp->GetFrame());

		PostProcessRenderer::Update();

	}

	void Draw(int preornot)
	{
		PostProcessRenderer::Draw(preornot);
	}

private:
	PostProcessRenderer * mRadioLightPPR;
	GameObject * mRadioPPRObject;
	Vector4 LightColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	float maxDistance = 35.0f;
	int NUM_SAMPLES = 128;
	float Decay = 0.999f;
	float weight = 1.0f / 128.0f;
	float Density = 1.0f;
	float Exposure = 0.75f;
};

