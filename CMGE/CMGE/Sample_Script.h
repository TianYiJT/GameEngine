#pragma once
#include"Component.h"
#include"CMGEInput.h"
#include"GameObject.h"
#include"Camera.h"

using namespace CMGEComponent;
using namespace CMGEInput;
using namespace CMGERenderEngine;

class Sample_Script:public Component
{
public:
	float scale;
	Sample_Script()
	{

	}

	~Sample_Script()
	{

	}

	void Start()
	{
		scale = ((GameObject*)mGameObject)->transform.Scalition.x;
		vector<KeyBoard> vkb;
		vkb.push_back(KeyBoard::KEY_C);
		vkb.push_back(KeyBoard::KEY_B);
		InputSystem::registerButton(vkb,"CB");
	}

	void Update()
	{
		if(InputSystem::GetKey(KeyBoard::KEY_UP))
		{
			((GameObject*)mGameObject)->transform.Translate(((GameObject*)mGameObject)->transform.forward()*InputSystem::GetdeltaTime());
		}
		if (InputSystem::GetKey(KeyBoard::KEY_DOWN))
		{
			((GameObject*)mGameObject)->transform.Translate(-((GameObject*)mGameObject)->transform.forward()*InputSystem::GetdeltaTime());
		}
		if (InputSystem::GetKey(KeyBoard::KEY_RIGHT))
		{
			((GameObject*)mGameObject)->transform.Translate(((GameObject*)mGameObject)->transform.right()*InputSystem::GetdeltaTime());
		}
		if (InputSystem::GetKey(KeyBoard::KEY_LEFT))
		{
			((GameObject*)mGameObject)->transform.Translate(-((GameObject*)mGameObject)->transform.right()*InputSystem::GetdeltaTime());
		}
		if (InputSystem::GetKeyDown(KeyBoard::KEY_Q))
		{
			((GameObject*)mGameObject)->isEnable = false;
		}
		if(InputSystem::GetButtonDown("CB"))
		{
		//	cout << "asd" << endl;
			((GameObject*)mGameObject)->isEnable = false;
		}
		Vector2 vmouse = InputSystem::GetMouseDeltaVelocity();
		Camera::MainCamera->Rotate(Vector3(vmouse.y/40.0f,vmouse.x/40,0));

	}

	static void* createComponent()
	{
		return new Sample_Script;
	}

	void Init()
	{
		ClassName = "Sample_Script";
		ComponentFactory::getInstance().registClass(ClassName, (CreateComponent)createComponent);
		(fatherName).push_back(string("Component"));
	}

private:

};
