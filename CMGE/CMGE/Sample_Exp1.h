#pragma once
/*
ʵ�� ֱ����Ƥ�� �������Ƥ��
ʵ�� ������� 
ʵ�ֹ���
ʵ���ݶ���Ӱ
*/

#include"App.h"

#include"CMGECreator2D.h"

using namespace CMGETool;

class Sample_Exp1 :public App 
{
public:
	Sample_Exp1(int argc, char**argv)
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
		cameraCompponent->MainColor = Vector4(0, 0, 0, 1);
		cameraCompponent->OrthoOrNot = true;
		cameraCompponent->enabled = true;
		GameObject * camera = new GameObject();
		camera->AddComponent<Camera>(cameraCompponent);
		camera->isEnable = true;
		camera->transform.Translate(Vector3(0, 0, -5));

		CMGECreator2D * tCMGECreator2D = new CMGECreator2D();
		tCMGECreator2D->Init();
		tCMGECreator2D->enabled = true;
		GameObject * gCreator2D = new GameObject();
		gCreator2D->AddComponent<CMGECreator2D>(tCMGECreator2D);
		gCreator2D->isEnable=true;

		SceneManager::tempScene->AddGameObject(camera);
		SceneManager::tempScene->AddGameObject(gCreator2D);

		if (SceneManager::tempScene != NULL)
			SceneManager::tempScene->Start();

	}

};