#pragma once
#include"CMGERenderEngine.h"
#include"CMGEInput.h"
#include"AudioPlayer.h"

using namespace CMGERenderEngine;
using namespace CMGEInput;

class SceneManager
{
public:
	void AddGameObject(GameObject *g);
	void DeleteGameObject(GameObject *g);
	static GameObject* Find(string name);
	static void changeScene(string name);
	static SceneManager* CreateScene(string name);
	void Start();
	void Update();
	void FixedUpdate();
	void SetMainScene();
	static SceneManager *tempScene;
	static vector<SceneManager*> allScene;
	vector<GameObject*> mGameObjectList;
	string mScenename;
private:
	//vector<thread*> mAudioThread;
};