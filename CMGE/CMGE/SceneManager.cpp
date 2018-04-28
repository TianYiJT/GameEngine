#include"SceneManager.h"

SceneManager *SceneManager::tempScene = new SceneManager();
vector<SceneManager*> SceneManager::allScene = vector<SceneManager*>();

void SceneManager::SetMainScene() 
{
	if(SceneManager::tempScene!=this)
	{
		SceneManager::tempScene = this;
	}
}

void SceneManager::AddGameObject(GameObject* g) 
{
	mGameObjectList.push_back(g);
}


void SceneManager::FixedUpdate()
{
	for (int i = 0; i < mGameObjectList.size(); i++) 
	{
		for (int j = 0; j < mGameObjectList[i]->GetAllComponent().size(); j++) 
		{
			if (mGameObjectList[i]->GetAllComponent()[j]->frameRate == 0)
				continue;
			if (mGameObjectList[i]->GetAllComponent()[j]->framecounter <1.0f/(float)mGameObjectList[i]->GetAllComponent()[j]->frameRate) 
			{
				mGameObjectList[i]->GetAllComponent()[j]->framecounter += InputSystem::GetdeltaTime();
		//		cout << mGameObjectList[i]->GetAllComponent()[j]->framecounter << endl;
			}
			else 
			{
				mGameObjectList[i]->GetAllComponent()[j]->framecounter = 0.0f;
				mGameObjectList[i]->GetAllComponent()[j]->FixedUpdate();
			}
		}
	}
}

void SceneManager::DeleteGameObject(GameObject * g) 
{
	int index=-1;
	for (int i = 0; i < mGameObjectList.size(); i++) 
	{
		if(mGameObjectList[i]==g)
		{
			index = i;
			break;
		}
	}
	if(index!=-1)
	{
		GameObject * temp = mGameObjectList[index];
		mGameObjectList[index] = mGameObjectList[mGameObjectList.size() - 1];
		mGameObjectList.pop_back();
	}
}

GameObject* SceneManager::Find(string s) 
{
	vector<GameObject*> tempList = SceneManager::tempScene->mGameObjectList;
	for (int i = 0; i < tempList.size(); i++) 
	{
		if (tempList[i]->name == s) 
		{
			return tempList[i];
		}
	}
	return NULL;
}

void SceneManager::changeScene(string s) 
{
	for (int i = 0; i < SceneManager::allScene.size(); i++) 
	{
		if(SceneManager::allScene[i]->mScenename==s)
		{
			SceneManager::allScene[i]->SetMainScene();
			return;
		}
	}
}

SceneManager* SceneManager::CreateScene(string s)
{
	SceneManager *temp = new SceneManager();
	temp->mScenename = s;
	SceneManager::allScene.push_back(temp);
	return temp;
}

void SceneManager::Start() 
{
	for (int i = 0; i < mGameObjectList.size(); i++) 
	{
		mGameObjectList[i]->Start();
	}
}


void SceneManager::Update() 
{
	
	FixedUpdate();
	Camera* mainCamera = Camera::MainCamera;
	
	mainCamera->RenderGameObject.clear();
	for (int i = 0; i < mGameObjectList.size(); i++)
	{
		if (mGameObjectList[i] != mainCamera->mGameObject)
			mainCamera->RenderGameObject.push_back(mGameObjectList[i]);
	}
	

	mainCamera->FrameStart(true);
	
	mainCamera->Update();

	glViewport(0, 0, mainCamera->ScreenWidth, mainCamera->ScreenHeight);
	glClearColor(mainCamera->MainColor.x, mainCamera->MainColor.y, mainCamera->MainColor.z, mainCamera->MainColor.w);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < mGameObjectList.size(); i++)
	{
		if (mGameObjectList[i]->isEnable)
			mGameObjectList[i]->Render();
	}
	mainCamera->FrameEnd(true);
	
	glViewport(0, 0, mainCamera->ScreenWidth, mainCamera->ScreenHeight);
	glClearColor(mainCamera->MainColor.x, mainCamera->MainColor.y, mainCamera->MainColor.z, mainCamera->MainColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < mGameObjectList.size(); i++)
	{
		if(mGameObjectList[i]->isEnable)
			mGameObjectList[i]->Update();
	}

	glutSwapBuffers();
	
}