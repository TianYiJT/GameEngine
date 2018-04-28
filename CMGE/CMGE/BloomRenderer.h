#pragma once
#include"PostProcessRenderer.h"

class BloomRenderer :public PostProcessRenderer 
{
public:
	int blurPixels;
	float bloomIntensity;
	BloomRenderer()
	{
		PostProcessRenderer::PostProcessRenderer();

	}
	static void* createComponent()
	{
		return new BloomRenderer;
	}

	void Init()
	{
		ClassName = "BloomRenderer";
		for (int i = 0; i < PostProcessRenderer::fatherName.size(); i++)
			fatherName.push_back(PostProcessRenderer::fatherName[i]);
		fatherName.push_back("PostProcessRenderer");
		ComponentFactory::getInstance().registClass(ClassName, (CreateComponent)createComponent);
	}

	void Start()
	{
		PostProcessRenderer::Start();
		Shader * s_bloom = new Shader();
		s_bloom->Load("Bloom.shader", "Bloom", "shader");
		Material * m_bloom = new Material();
		m_bloom->SetShader(s_bloom);
		int i1 = 9;
		m_bloom->SetMaterialParam("BlurIntensity", &i1);
		float f1 = 1.3f;
		m_bloom->SetMaterialParam("BloomIntensity", &f1);
		SetMaterial(m_bloom);

		Shader * s_bloomTex = new Shader();
		s_bloomTex->Load("BloomTex.shader","BloomTex","shader");
		Material * m_bloomTex = new Material();
		m_bloomTex->SetShader(s_bloomTex);
		int i2 = 0;
		m_bloomTex->SetMaterialParam("BlurIntensity",&i2);
		mbloomTex= new PostProcessRenderer();
		mbloomTex->Init();
		mbloomTex->SetMaterial(m_bloomTex);
		mbloomTex->enabled = true;

		bloomTex = new Texture2D();
		bloomTex->name = "BloomTex";
		bloomTex->mTextureType = TextureType::COLOR;
		bloomTex->SetPixels(NULL,Camera::MainCamera->ScreenWidth,Camera::MainCamera->ScreenHeight);
		bloomTex->Init();

		mBloomGameObject->AddComponent<PostProcessRenderer>(mbloomTex);
	//	cout << "start" << endl;
	}

	void Update()
	{
		Camera *temp = Camera::MainCamera;
		temp->FrameStart(false);
		glViewport(0, 0, temp->ScreenWidth, temp->ScreenHeight);
		glClearColor(temp->MainColor.x, temp->MainColor.y, temp->MainColor.z, temp->MainColor.w);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mBloomGameObject->Update();
		temp->FrameEnd(false);
		bloomTex->SetPixels(temp->GetFrame()->GetPixels(),temp->ScreenWidth,temp->ScreenHeight);
		bloomTex->Init();
		mMaterial->SetMaterialParam("BloomTex", temp->GetFrame());
		PostProcessRenderer::Update();
	}

	void Draw(int preornot) 
	{
		PostProcessRenderer::Draw(preornot);
	}

private:
	PostProcessRenderer * mbloomTex;
	GameObject * mBloomGameObject = new GameObject();
	Texture2D * bloomTex;
};