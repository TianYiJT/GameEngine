#pragma once
#include"App.h"
#include"Sample_Script.h"

class Sample1 :public App 
{
public:
	Sample1(int argc,char**argv) 
	{
		App::App(argc,argv);
	}

	void Start() 
	{
		mWidth = 800.0f;
		mHeight = 600.0f;
		App::Start();
		SceneManager * sc = SceneManager::CreateScene("first");
		sc->SetMainScene();

		Camera * cameraCompponent = new Camera();
		cameraCompponent->Init();
		cameraCompponent->SetMainCamera();
		cameraCompponent->MainColor = Vector4(0,0,0,1);
		cameraCompponent->OrthoOrNot = false;
		cameraCompponent->enabled = true;
		GameObject * camera = new GameObject();
		camera->AddComponent<Camera>(cameraCompponent);
		camera->isEnable = true;
		camera->transform.Translate(Vector3(0,5,15));
		camera->transform.Rotate(Vector3(0,0,0));

		
		Texture2D * t_left = new Texture2D(); t_left->mTextureType = TextureType::COLOR; t_left->Load("Left.bmp", "Left", "bmp");
		Texture2D * t_right = new Texture2D(); t_right->mTextureType = TextureType::COLOR;  t_right->Load("Right.bmp", "Right", "bmp");
		Texture2D * t_up = new Texture2D(); t_up->mTextureType = TextureType::COLOR; t_up->Load("Up.bmp", "Up", "bmp");
		Texture2D * t_down = new Texture2D(); t_down->mTextureType = TextureType::COLOR; t_down->Load("Down.bmp", "Down", "bmp");
		Texture2D * t_forward = new Texture2D(); t_forward->mTextureType = TextureType::COLOR; t_forward->Load("Front.bmp", "Front", "bmp");
		Texture2D * t_back = new Texture2D(); t_back->mTextureType = TextureType::COLOR; t_back->Load("Back.bmp", "Back", "bmp");

		CubeTexture * ct = new CubeTexture(); ct->Init(t_up, t_down, t_right, t_left, t_forward, t_back, "defaultskybox", "sky");
	//	cout<<ct->GetCubeMapIndex()<<endl;

		Shader * skys = new Shader(); skys->Load("DefaultSkyBox.shader","DefaultSkyBox","shader");
		skys->output();

		Material * m_sky = new Material();
		m_sky->SetShader(skys);
		m_sky->SetMaterialParam("Skybox",ResourceManager<CubeTexture>::getInstance()->getData("defaultskybox"));

		SkyBoxRenderer *skyboxRenderer = new SkyBoxRenderer();
		skyboxRenderer->Init();
		skyboxRenderer->SetMaterial(m_sky);
		skyboxRenderer->enabled = true;
		//camera->AddComponent<SkyBoxRenderer>(skyboxRenderer);
		


		Shader * s = new Shader();
		s->Load("Default.shader","Default","shader");
	//	s->output();

		Mesh * m = new Mesh();
		m->Load("run.FBX","run","FBX");

		Texture2D * t_MainTex = new Texture2D();
		t_MainTex->mTextureType = TextureType::COLOR;
		t_MainTex->Load("maintex.jpg", "MainTex", "jpg");

		Texture2D * t_NormalTex = new Texture2D();
		t_NormalTex->mTextureType = TextureType::COLOR;
		t_NormalTex->Load("normal.jpg","NormalTex","jpg");

		Material * m_Main = new Material();
		m_Main->SetShader(s);
		m_Main->SetMaterialParam("MainColor",&Vector4(0,0,1,1));
		m_Main->SetMaterialParam("mTex",ResourceManager<Texture2D>::getInstance()->getData("MainTex"));
		m_Main->SetMaterialParam("nTex", ResourceManager<Texture2D>::getInstance()->getData("NormalTex"));
		m_Main->SetColor(Vector4(1,0,0,1));

		MeshRenderer * mr = new MeshRenderer();
		mr->Init();
		mr->SetMesh(m);
		mr->SetMaterial(m_Main);
		mr->enabled = true;
		GameObject * gMesh = new GameObject();
		gMesh->AddComponent<MeshRenderer>(mr);
		gMesh->isEnable = true;
		gMesh->transform.Translate(Vector3(10,-6,35));
		gMesh->transform.Rotate(Vector3(0,90,90));
		gMesh->transform.Scale(Vector3(0.10,0.10,0.10));


		MeshRenderer * mrplane = new MeshRenderer();
		mrplane->Init();
		mrplane->SetMesh(ResourceManager<Mesh>::getInstance()->getData("Plane"));
		mrplane->enabled = true;
		Shader * s_solid = new Shader(); s_solid->Load("SolidColor.shader","SolidColor","shader");
		Material * m_solid = new Material(); m_solid->SetShader(s_solid);
		mrplane->SetMaterial(m_solid);

		GameObject * g_plane = new GameObject();
		g_plane->isEnable = true;
		g_plane->AddComponent<MeshRenderer>(mrplane);
		g_plane->transform.Translate(Vector3(0, -5, 40));
		g_plane->transform.Rotate(Vector3(0, 90, 90));
		g_plane->transform.Scale(Vector3(0.55, 0.55, 0.55));

		MeshRenderer * mrCube = new MeshRenderer();
		mrCube->Init();
		mrCube->SetMesh(ResourceManager<Mesh>::getInstance()->getData("Cube"));
		mrCube->enabled = true;
		mrCube->SetMaterial(m_solid);

		GameObject * g_Cube = new GameObject();
		g_Cube->isEnable = true;
		g_Cube->AddComponent<MeshRenderer>(mrCube);
		g_Cube->transform.Translate(Vector3(3, 1, 35));
		g_Cube->transform.Rotate(Vector3(0, 90, 90));
		g_Cube->transform.Scale(Vector3(1.5, 1.5, 1.5));

		Shader * s_hdr = new Shader();
		s_hdr->Load("EdgeDetection.shader", "EdgeDetection", "shader");
		Material * m_hdr = new Material();
		m_hdr->SetShader(s_hdr);
	//	float ff = 30.0f;
	//	m_hdr->SetMaterialParam("FocusDistance",&ff);
		
		VolumetricLightRenderer *ppr = new VolumetricLightRenderer();
		ppr->LightPos = Vector2(0.65f,0.4f);
		ppr->Init();
		ppr->enabled = true;
		ppr->SetFrameMode(false);
		

		GameObject * gPost = new GameObject();
		gPost->isEnable = true;
		gPost->AddComponent<VolumetricLightRenderer>(ppr);
	//	gPost->AddComponent<PostProcessRenderer>(ppr);

		sc->AddGameObject(camera);
		sc->AddGameObject(gMesh);
		sc->AddGameObject(gPost);
		sc->AddGameObject(g_plane);
		sc->AddGameObject(g_Cube);

		if (SceneManager::tempScene != NULL)
			SceneManager::tempScene->Start();
	}
};