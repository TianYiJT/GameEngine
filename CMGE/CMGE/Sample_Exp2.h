#pragma once
#include"CameraCapture.h"
#include"App.h"

class Sample_Exp2 :public App 
{
public:
	Sample_Exp2(int argc, char**argv)
	{
		App::App(argc, argv);
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
		cameraCompponent->MainColor = Vector4(1, 0, 0, 1);
		cameraCompponent->OrthoOrNot = true;
		cameraCompponent->enabled = true;
		GameObject * camera = new GameObject();
		camera->AddComponent<Camera>(cameraCompponent);
		camera->isEnable = true;
		camera->transform.Translate(Vector3(0, 0, -5));



		//tVideo->output(0);

		CameraCapture * vp = new CameraCapture();
		vp->Init();
		vp->enabled = true;

		GameObject * g = new GameObject(); g->isEnable = true;
		g->AddComponent<CameraCapture>(vp);


		SceneManager::tempScene->AddGameObject(g);
		SceneManager::tempScene->AddGameObject(vp->mVideoObject);
		SceneManager::tempScene->AddGameObject(camera);


		if (SceneManager::tempScene != NULL)
			SceneManager::tempScene->Start();

	}
};

