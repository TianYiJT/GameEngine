#pragma once
#include"App.h"

#include"VideoPlayer.h"

#define DEMOVEDIO "sample2.mp4"
#define DEMOAUDIO "demo.wav"
#define FIVEAUDIO "fiveptone.wav"

class Sample_Exp4 :public App
{
public:
	Sample_Exp4(int argc, char**argv)
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

		AudioPlayer * mAudioPlayer = new AudioPlayer(DEMOAUDIO);
		AudioPlayer * mAudioPlayer1 = new AudioPlayer(FIVEAUDIO);
		mAudioPlayer->SetLoop(true);
		mAudioPlayer1->SetLoop(true);
		mAudioPlayer->Play();
		mAudioPlayer1->Play();

		Video * tVideo = new Video();
		tVideo->Load(DEMOVEDIO,"demo","mp4");

		//tVideo->output(0);

		VideoPlayer * vp = new VideoPlayer();
		vp->Init();
		vp->SetVideo(tVideo);
		vp->enabled = true;
		vp->Play();
		vp->SetLoop(true);

		GameObject * g = new GameObject(); g->isEnable = true;
		g->AddComponent<VideoPlayer>(vp);

		
		SceneManager::tempScene->AddGameObject(g);
		SceneManager::tempScene->AddGameObject(vp->mVideoObject);
		SceneManager::tempScene->AddGameObject(camera);


		if (SceneManager::tempScene != NULL)
			SceneManager::tempScene->Start();

	}

};