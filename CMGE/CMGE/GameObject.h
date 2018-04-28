#pragma once
#include"Component.h"
#include<vector>
#include<string>
#include"CMGEMathEngine.h"
#include"ComponentFactory.h"


using namespace std;
using namespace CMGEMathEngine;
using namespace CMGEComponent;


namespace CMGERenderEngine 
{
	class GameObject
	{
	public:

		string name;
		string tag;
		bool isStatic;
		bool isEnable;
		Transform transform;
		GameObject() 
		{
			transform.Position = Vector3(0,0,0);
			transform.PositionMatrix = Matrix4x4::TranslateMatrix(transform.Position);
			transform.Rotation = Vector3(0,0,0);
			transform.RotationMatrix = Matrix4x4::RotateMatrix(transform.Rotation);
			transform.Scalition = Vector3(1,1,1);
			transform.ScaleMatrix = Matrix4x4::ScaleMatrix(transform.Scalition);
		}
		void AddChild(GameObject* g) 
		{
			childs.push_back(g);
		}

		void DeleteChild(GameObject* g) 
		{
			for (int i = 0; i < childs.size(); i++) 
			{
				if (g == childs[i]) 
				{
					swap(childs[i],childs[childs.size()-1]);
					childs.pop_back();
					break;
				}
			}
		}

		void SetParent(GameObject* g) 
		{
			parent = g;
			g->AddChild(this);
		}

		template<class T>
		T* GetComponent(string name) 
		{
			for (int i = 0; i < myComponent.size(); i++) 
			{
				if (myComponent[i]->getclassName() == name)
				{
					return (T*)myComponent[i];
				}
				for (int j = 0; j < myComponent[i]->fatherName.size(); j++) 
				{
					if (myComponent[i]->fatherName[j] == name) 
					{
						return (T*)myComponent[i];
					}
				}
			}
			return NULL;
		}

		template<class T>
		void AddComponent(string name) 
		{
			T * temp = (T*)ComponentFactory::getInstance().getClassByName(name);
			myComponent.push_back(temp);
			myComponent[myComponent.size() - 1]->SetGameObject(this);
		}

		template<class T>
		void AddComponent(T * c) 
		{
			myComponent.push_back(c);
			myComponent[myComponent.size() - 1]->SetGameObject(this);
		}


		template<class T>
		void DeleteComponent(string name) 
		{
			for (int i = 0; i < myComponent.size(); i++)
			{
				if (myComponent[i]->getclassName() == name)
				{
					myComponent.swap(i,myComponent.size()-1);
					myComponent.pop_back();
				}
			}
		}

		void Start() 
		{
			for (int i = 0; i < myComponent.size(); i++) 
			{
				myComponent[i]->Start();
			}
		}

		void Update() 
		{
			for (int i = 0; i < myComponent.size(); i++) 
			{
				if(myComponent[i]->enabled)
					myComponent[i]->Update();
			}
		}


		vector<Component*> GetAllComponent() 
		{
			return myComponent;
		}

		void Render()
		{
			for (int i = 0; i < myComponent.size(); i++)
			{
				if (myComponent[i]->FindClassByName("PostProcessRenderer"))
				{
					continue;
				}
				if (myComponent[i]->getclassName() == "Renderer")
				{
					if (myComponent[i]->enabled)
						myComponent[i]->Update();
					continue;
				}
				for (int j = 0; j < myComponent[i]->fatherName.size(); j++)
				{
					if (myComponent[i]->fatherName[j] == "Renderer")
					{
						if (myComponent[i]->enabled)
							myComponent[i]->Update();
						break;
					}
				}
			}
		}

	protected:
		vector<Component*> myComponent;
		GameObject * parent = NULL;
		vector<GameObject*> childs;
	};
}