#pragma once
#include<string>
#include<vector>
#include"ComponentFactory.h"

using namespace std;
namespace CMGEComponent
{
	class Component
	{
	public:
		bool enabled;
		int Prority=0;
		int frameRate = 0;
		float framecounter = 0.0f;
		Component() 
		{
			
		}
		vector<string> fatherName;
		virtual void Start() {}
		virtual void Update() 
		{
			
		}

		virtual void FixedUpdate() {}

		void SetGameObject(void * _g)
		{
			mGameObject = _g;
		}
		void* GetGameObject()
		{
			return mGameObject;
		}
		string getclassName()
		{
			return ClassName;
		}

		bool FindClassByName(string name) 
		{
			if (ClassName == name)
				return true;
			for (int i = 0; i < fatherName.size(); i++)
				if (fatherName[i] == name)
					return true;
			return false;
		}

		static void* createComponent()
		{
			return new Component;
		}
		void Init()
		{
			ClassName = "Component";
			ComponentFactory::getInstance().registClass(ClassName, (CreateComponent)createComponent);
		}
		void * mGameObject;
	protected:

		string ClassName;


	};
}
