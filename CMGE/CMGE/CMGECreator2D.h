#pragma once
#include"CMGEMathEngine.h"
#include"CMGEInput.h"
#include"Component.h"
#include"GameObject.h"
#include"LineRenderer.h"
#include"SceneManager.h"
#include<vector>
#include<math.h>
#define MIN(a,b) a>b?b:a
#define MAX(a,b) a>b?a:b
#define PIE 3.1415926f

using namespace CMGEMathEngine;
using namespace CMGEInput;
using namespace CMGEComponent;
using namespace CMGERenderEngine;

namespace CMGETool
{
	struct relateEdge
	{
		 int id1;
		 int id2;
		 vector<Vector2> vertices;
	};

	class Model2D 
	{
	public:
		void AddPolygon(vector<Vector2> pvset) 
		{
			polygons.push_back(pvset);
			polygonColors.push_back(Vector4(1,1,1,1));

			Vector4 boundthis(1000,1000,-1000,-1000);
			for (int i = 0; i < pvset.size(); i++) 
			{
				if (pvset[i].x < boundthis.x)
					boundthis.x = pvset[i].x;
				if (pvset[i].x > boundthis.z)
					boundthis.z = pvset[i].x;
				if (pvset[i].y < boundthis.y)
					boundthis.y = pvset[i].y;
				if (pvset[i].y > boundthis.w)
					boundthis.w = pvset[i].y;
			}

			bounds.push_back(boundthis);
		}
		void FullPolygonWithColor(Vector2 pcenter, Vector4 pcolor) 
		{
			int index = findPolygonsByPoint(pcenter);
			if (index != -1)
				polygonColors[index] = pcolor;
		}
		void Bezier(Vector2 pTopLeft, Vector2 pBottomRight) {}
		void DeletePolygon(Vector2 pcenter) 
		{
			int index = findPolygonsByPoint(pcenter);
			if (index != -1) 
			{
				polygons[index] = polygons[polygons.size()-1];
				polygons.pop_back();
				polygonColors[index] = polygonColors[polygonColors.size() - 1];
				polygonColors.pop_back();
				bounds[index] = bounds[bounds.size() - 1];
				bounds.pop_back();
			}
		}
		vector<int> polygon2Triangels(vector<Vector2> pPolygon)
		{
			int size = pPolygon.size();
			vector<int> vi;
			for (int i = 1; i < size - 1; i++) 
			{
				vi.push_back(0);
				vi.push_back(i);
				vi.push_back(i+1);
			}
			return vi;
		}
		Mesh* ToMeshData() 
		{
			vector<vec3> tp;
			vector<vec4> tc;
			vector<int> ti;
			for (int i = 0; i < polygons.size(); i++) 
			{
				int ts = tp.size();
				for (int j = 0; j < polygons[i].size(); j++) 
				{
					tc.push_back(vec4(polygonColors[i].x,polygonColors[i].y,polygonColors[i].z,polygonColors[i].w));
					tp.push_back(vec3(polygons[i][j].x,polygons[i][j].y,0.0f));
				}
				vector<int> tii = polygon2Triangels(polygons[i]);
				for (int i = 0; i < tii.size(); i++) 
				{
					ti.push_back(tii[i]+ts);
				}
			}
			mMesh->num_Position = tp.size();
			delete[] mMesh->glpos;
			mMesh->glpos = new vec3[mMesh->num_Position];
			for (int i = 0; i < mMesh->num_Position; i++)
				mMesh->glpos[i] = tp[i];
			mMesh->num_Color = tc.size();
			delete[] mMesh->glcolor;
			mMesh->glcolor = new vec4[mMesh->num_Color];
			for (int i = 0; i < mMesh->num_Color; i++)
				mMesh->glcolor[i] = tc[i];
			mMesh->num_Index = ti.size();
			delete[] mMesh->mTriangleIndex;
			mMesh->mTriangleIndex = new GLuint[mMesh->num_Index];
			for (int i = 0; i < mMesh->num_Index; i++)
				mMesh->mTriangleIndex[i] = ti[i];
			return mMesh;
		}
	private: 
		int findPolygonsByPoint(Vector2 p) 
		{
		//	p.output();
			for (int i = 0; i < polygons.size(); i++) 
			{
				int count = 0;
				float sum = 0;
				Vector2 min_ = Vector2(bounds[i].x,bounds[i].y);
				Vector2 max_ = Vector2(bounds[i].z,bounds[i].w);
				if (p.x > min_.x&&p.x<max_.x&&p.y>min_.y&&p.y < max_.y)
					return i;
			}
			return -1;
		}
		vector<vector<Vector2>> polygons;
		//vector<relateEdge> relateEdges;
		vector<Vector4> polygonColors;
		vector<Vector4> bounds;
		Mesh* mMesh = new Mesh();
	};

	enum CreateMode:unsigned int
	{
		CREATEPOLYGON = 0,
		SELECTPOINT ,
		FULLCOLOR,
		DELETEPOLYGON,
		SCALETRANSFORM,
		TRANSLATETRANSFORM,
		ROTATIONTRANSFORM
	};

	class CMGECreator2D:public Component
	{
	public:
		static void* createComponent()
		{
			return new CMGECreator2D;
		}
		void Init()
		{
			ClassName = "CMGECreator2D";
			ComponentFactory::getInstance().registClass(ClassName, (CreateComponent)createComponent);
			(fatherName).push_back(string("Component"));
			LineRenderer * lr = new LineRenderer();
			lr->Init();
			lr->enabled = true;
			tempLine->AddComponent<LineRenderer>(lr);
			tempLine->transform.Translate(Vector3(0,0,0));
			tempLine->isEnable = true;


			LineRenderer * lr1 = new LineRenderer();
			lr1->Init();
			lr1->enabled = true;
			LineLeftdown->AddComponent<LineRenderer>(lr1);
			LineLeftdown->transform.Translate(Vector3(0, 0, 0));
			LineLeftdown->isEnable = true;

			LineRenderer * lr2 = new LineRenderer();
			lr2->Init();
			lr2->enabled = true;
			LineRightUp->AddComponent<LineRenderer>(lr2);
			LineRightUp->transform.Translate(Vector3(0, 0, 0));
			LineRightUp->isEnable = true;

			MeshRenderer * mr = new MeshRenderer();
			mr->Init();
			mr->enabled = true;
			tempModel->AddComponent<MeshRenderer>(mr);
			tempModel->transform.Translate(Vector3(0,0,0));
			tempModel->isEnable = true;
			Shader * s_solid = new Shader();
			s_solid->Load("SolidColor.shader", "SolidColor1", "shader");
			Material *mMaterial = new Material();
			mMaterial->SetShader(s_solid);
			Vector4 *white = new Vector4(1.0, 1.0, 1.0, 1.0f);
			mMaterial->SetMaterialParam("mColor", white);
			mr->SetMaterial(mMaterial);
			SceneManager::tempScene->AddGameObject(tempLine);
			SceneManager::tempScene->AddGameObject(tempModel);

		}
		void Update() 
		{
			if (InputSystem::GetKeyDown(KeyBoard::KEY_Q))
				SetMode(CreateMode::CREATEPOLYGON);
			if (InputSystem::GetKeyDown(KeyBoard::KEY_W))
				SetMode(CreateMode::FULLCOLOR);
			if (InputSystem::GetKeyDown(KeyBoard::KEY_E))
				SetMode(CreateMode::DELETEPOLYGON);
			if (InputSystem::GetKeyDown(KeyBoard::KEY_R))
				SetMode(CreateMode::TRANSLATETRANSFORM);
			if (InputSystem::GetKeyDown(KeyBoard::KEY_T))
				SetMode(CreateMode::SCALETRANSFORM);
			if (InputSystem::GetKeyDown(KeyBoard::KEY_Y))
				SetMode(CreateMode::ROTATIONTRANSFORM);
			if (InputSystem::GetKeyDown(KeyBoard::KEY_U))
				SetMode(CreateMode::SELECTPOINT);

			switch (tcm)
			{
			case CMGETool::CREATEPOLYGON:
				tempLine->isEnable = true;
				LineLeftdown->isEnable = false;
				LineRightUp->isEnable = false;
				tempLine->GetComponent<LineRenderer>("LineRenderer")->DeletePoint();
			//	cout << tempLine->GetComponent<LineRenderer>("LineRenderer")->tempCount << endl;
				tvbuffer = InputSystem::GetMousePosition();
				tvbuffer = Camera::MainCamera->TransformMousePosition(tvbuffer);
				if (InputSystem::GetMouseButtonDown(MouseButton::LEFTBUTTON)) 
				{

					Vector2 vpos = tvbuffer;
					tempLine->GetComponent<LineRenderer>("LineRenderer")->AddPoint(vec3(vpos.x,vpos.y,0));
					if (tpbuffer.size() == 0)
					{
						Vector2 vpos1 = Vector2(vpos.x,vpos.y);
						tpbuffer.push_back(vpos1);
						tempLine->GetComponent<LineRenderer>("LineRenderer")->AddPoint(vec3(tvbuffer.x, tvbuffer.y, 0));
						return;
					}
				//	tpbuffer[0].output();
				//	tempLine->GetComponent<LineRenderer>("LineRenderer")->getPoint(0).output();
					
					if (Vector2::Distance(tvbuffer, tpbuffer[0]) < 0.14f&&tpbuffer.size()>=3)
					{
					//	cout << "debug" << endl;
						tempData->AddPolygon(tpbuffer);
						tempModel->GetComponent<MeshRenderer>("MeshRenderer")->SetMesh(tempData->ToMeshData());
						tpbuffer.clear();
						tempLine->GetComponent<LineRenderer>("LineRenderer")->Clear();
					}
					else 
					{
						Vector2 vpos1 = Vector2(vpos.x, vpos.y);
						tpbuffer.push_back(vpos1);
					}
				}
				tempLine->GetComponent<LineRenderer>("LineRenderer")->AddPoint(vec3(tvbuffer.x, tvbuffer.y, 0));

				break;
			case CMGETool::SELECTPOINT:
				LineLeftdown->isEnable = true;
				LineRightUp->isEnable = true;
				tvbuffer = InputSystem::GetMousePosition();
				tvbuffer = Camera::MainCamera->TransformMousePosition(tvbuffer);
				if (InputSystem::GetMouseButtonDown(MouseButton::LEFTBUTTON)) 
				{
					isStart = true;
					fir_ = tvbuffer;
				}
				if (InputSystem::GetMouseButtonUp(MouseButton::LEFTBUTTON)) 
				{
					isStart = false;
				}
				if (isStart) 
				{

				}

				break;
			case CMGETool::FULLCOLOR:
				tempLine->isEnable = false;
				LineLeftdown->isEnable = false;
				LineRightUp->isEnable = false;
				tvbuffer = InputSystem::GetMousePosition();
				tvbuffer = Camera::MainCamera->TransformMousePosition(tvbuffer);
				if (InputSystem::GetMouseButtonDown(MouseButton::LEFTBUTTON)) 
				{
					svbuffer = tvbuffer;
					float r, g, b, a;
					cin >> r; cin >> g; cin >> b; cin >> a;
					Vector4 tColor = Vector4(r, g, b, a);
					tempData->FullPolygonWithColor(svbuffer, tColor);
					tempModel->GetComponent<MeshRenderer>("MeshRenderer")->SetMesh(tempData->ToMeshData());
				}
				break;
			case CMGETool::DELETEPOLYGON:
				tempLine->isEnable = false;
				LineLeftdown->isEnable = false;
				LineRightUp->isEnable = false;
				tvbuffer = InputSystem::GetMousePosition();
				tvbuffer = Camera::MainCamera->TransformMousePosition(tvbuffer);
				if (InputSystem::GetMouseButtonDown(MouseButton::LEFTBUTTON)) 
				{
					tempData->DeletePolygon(tvbuffer);
					tempModel->GetComponent<MeshRenderer>("MeshRenderer")->SetMesh(tempData->ToMeshData());
				}
				break;
			case CMGETool::SCALETRANSFORM:
				break;
			case CMGETool::TRANSLATETRANSFORM:
				break;
			case CMGETool::ROTATIONTRANSFORM:
				break;
			default:
				break;
			}
		}
		void Start() 
		{
			
		}
	private:
		
		void SetMode(CreateMode pcm) 
		{
			tcm = pcm;
		}

		void AddPolygon() {}
		void DeletePolygon() {}
		void BezierPolygon() {}
		void FullPolygon() {}
		vector<Vector2> tpbuffer;
		bool isStart = false;
		Vector2 tvbuffer;
		Vector2 svbuffer;
		Vector4 tcbuffer;
		CreateMode tcm;
		GameObject *tempLine = new GameObject();
		GameObject *tempModel = new GameObject();
		Model2D * tempData = new Model2D();
		vector<Vector2> selectPoints;
		Vector2 fir_;
		GameObject * LineLeftdown = new GameObject();
		GameObject * LineRightUp = new GameObject();
	};
}
