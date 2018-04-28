#pragma once
#include"CMGEMathEngine.h"
#include"Resource.h"
#include"ResourceManager.h"
#include"ResourceLoader.h"
#include<iostream>
#include"vmath.h"
#include"glew.h"
#include<freeglut.h>

using namespace CMGEMathEngine;
using namespace vmath;
using namespace CMGETool;
using namespace std;
namespace CMGERenderEngine 
{

#define PLANE_PATH "Plane.FBX"
#define CUBE_PATH "Cube.FBX"
#define SPERE_PATH "Sphere.FBX"
#define CYLINDER_PATH "Cylinder.FBX"
#define PIPE_PATH "Pipe.FBX"
#define RECTANGULAR_PYRAMID_PATH "Rectangular_pyramid.FBX"
	
	class Mesh:public Resource
	{
	public:
		vec3 *glpos;
		vec3 *glnor;
		vec2 *gluvcoord;
		vec2 *gluvcoordnormal;
		vec4 *glcolor;
		vec3 *gltangent;
		bool ImportWithTriangeNot = false;
		GLuint * mTriangleIndex;
		int num_Position;
		int num_Normal;
		int num_UVCoord;
		int num_UvCoordNormal;
		int num_Color;
		int num_Tangent;
		int num_Index;
		static Mesh Plane;
		static Mesh Cube;
		static Mesh Sphere;
		static Mesh Cylinder;
		static Mesh Pipe;
		static Mesh Rectangular_pyramid;
		void Init()
		{
			ResourceManager<Mesh>::getInstance()->registerData(this,name);
		}

		void output() 
		{
			cout << "Index:" << endl;
			cout << num_Index << endl;
			for (int i = 0; i < num_Index; i++)
				cout << mTriangleIndex[i] << endl;

			//cout<<glnor[]
			cout << "Normal:" << endl;
			cout << num_Normal << endl;
			for (int i = 0; i < num_Normal; i++)
				cout << glnor[i][0] << " " << glnor[i][1] << " " << glnor[i][2] << endl;

			cout << "Position:" << endl;
			cout << num_Position << endl;
			for (int i = 0; i < num_Position; i++)
				cout << glpos[i][0] << " " << glpos[i][1] << " " << glpos[i][2] << endl;


			cout << "UV:" << endl;
			cout << num_UVCoord << endl;
			for (int i = 0; i < num_UVCoord; i++)
				cout << gluvcoord[i][0] << " " << gluvcoord[i][1] << endl;

			cout << "UVNormal:" << endl;
			cout << num_UvCoordNormal << endl;
			for (int i = 0; i < num_UvCoordNormal; i++)
				cout << gluvcoordnormal[i][0] << " " << gluvcoordnormal[i][1] << " " << gluvcoordnormal[i][2] << endl;

			cout << "Color:" << endl;
			cout << num_Color << endl;
			for (int i = 0; i < num_Color; i++)
				cout << glcolor[i][0] << " " << glcolor[i][1] << " " << glcolor[i][2] <<" "<<glcolor[i][3]<<endl;
			
			cout << "Tangent:" << endl;
			cout << num_Tangent << endl;
			for (int i = 0; i < num_Tangent; i++)
				cout << gltangent[i][0] << " " << gltangent[i][1] << " " << gltangent[i][2] << endl;


		}

		void Load(string path, string name_, string mformat_)
		{
			name = name_;
			mformat = mformat_;
			Load(path);
			Init();
		}

		void normalized(Vector3 center,Vector3 maxLength)
		{
			for(int i=0;i<num_Position;i++)
			{
				Vector3 v1 = Vector3(glpos[i][0]-center.x,glpos[i][1]-center.y,glpos[i][2]-center.z);
				v1 = Vector3(v1.x/maxLength.x,v1.y/maxLength.y,v1.z/maxLength.z);
				glpos[i] = vec3(v1.x,v1.y,v1.z);
			}
		}

		void Load(string path) 
		{
			Resource::Load(path);
			CMGEFBXLoader* fbxloader = new CMGEFBXLoader();
			fbxloader->Init();
			fbxloader->Load(path);
			if (fbxloader->mMesh.size()!=0) 
			{
				CMGEFBXNode* node = (fbxloader->mMesh)[0];
			//	cout << node->num_Position;
				if (node->num_Index != 0)
				{
					if (!ImportWithTriangeNot) 
					{
						num_Index = node->num_Index;
						mTriangleIndex = new GLuint[num_Index];
						for (int i = 0; i < num_Index; i++)
							mTriangleIndex[i] = node->mTriangleIndex[i];
					}
					else
					{
						num_Index = node->num_Index / 4 * 6;
						mTriangleIndex = new GLuint[num_Index];
						int index = 0;
						for(int i=0;i<node->num_Index;i+=4)
						{
							mTriangleIndex[index] = node->mTriangleIndex[i]; index++;
							mTriangleIndex[index] = node->mTriangleIndex[i+1]; index++;
							mTriangleIndex[index] = node->mTriangleIndex[i+2]; index++;
							mTriangleIndex[index] = node->mTriangleIndex[i+2]; index++;
							mTriangleIndex[index] = node->mTriangleIndex[i+0]; index++;
							mTriangleIndex[index] = node->mTriangleIndex[i+3]; index++;
						}
					}
				}
				if (node->num_Position != 0)
				{
					glpos = new vec3[node->num_Position];
					num_Position = node->num_Position;
					for (int i = 0; i < num_Position; i++)
						glpos[i] = vec3((node->mPosition)[i].x, node->mPosition[i].y, node->mPosition[i].z);
				}
				if (node->num_Normal!=0)
				{
					glnor = new vec3[node->num_Position];
					num_Normal = node->num_Position;
					for (int i = 0; i < node->num_Normal; i++)
					{
						glnor[mTriangleIndex[i]] = vec3(node->mNormal[i].x, node->mNormal[i].y, node->mNormal[i].z);
					}
				}
				else
				{
					glnor = new vec3[node->num_Position];
					num_Normal = node->num_Position;
				}
				if (node->num_UVCoord!=0) 
				{
					num_UVCoord = node->num_Position;
					gluvcoord = new vec2[num_UVCoord];
					for (int i = 0; i < node->num_UVCoord; i++)
					{
						gluvcoord[mTriangleIndex[i]] = vec2(node->mUVCoord[i].x, node->mUVCoord[i].y);
					//	cout << node->mUVCoord[i].x << " " << node->mUVCoord[i].y <<" "<<mTriangleIndex[i] << endl;
					}
				}
				else
				{
					num_UVCoord = node->num_Position;
					gluvcoord = new vec2[num_UVCoord];
				}
				if (node->num_UvCoordNormal != 0) 
				{
					gluvcoordnormal = new vec2[node->num_Position];
					num_UvCoordNormal = node->num_Position;
					for (int i = 0; i < node->num_UvCoordNormal; i++)
						gluvcoordnormal[mTriangleIndex[i]] = vec2(node->mUVCoordNormal[i].x, node->mUVCoordNormal[i].y);
				}
				else
				{
					gluvcoordnormal = new vec2[node->num_Position];
					num_UvCoordNormal = node->num_Position;
				}
				if (node->num_Color != 0)
				{
					num_Color = node->num_Position;
					glcolor = new vec4[num_Color];
					for (int i = 0; i < node->num_Color; i++)
						glcolor[mTriangleIndex[i]] = vec4(node->mColor[i].x, node->mColor[i].y, node->mColor[i].z, node->mColor[i].w);
				}
				else
				{
					num_Color = node->num_Position;
					glcolor = new vec4[num_Color];
				}
				if (node->num_Tangent != 0) 
				{
				//	cout << "hello" << endl;
					num_Tangent = node->num_Position;
					gltangent = new vec3[num_Tangent];
					for (int i = 0; i < node->num_Tangent; i++)
						gltangent[mTriangleIndex[i]] = vec3(node->mTangent[i].x, node->mTangent[i].y, node->mTangent[i].z);
				}
				else
				{
					num_Tangent = node->num_Position;
					gltangent = new vec3[num_Tangent];
					for (int i = 0; i < num_Tangent; i++) 
					{
						Vector3 v1 = Vector3::randomTangent(Vector3(glnor[i][0],glnor[i][1],glnor[i][2]));
						gltangent[i] = vec3(v1.x,v1.y,v1.z);
					}
				}
			}
		}

		static void GlobalStartUp() 
		{
			
			Mesh::Plane.mformat = "FBX";
			Mesh::Plane.name = "Plane";
			Mesh::Plane.Load(PLANE_PATH);
			Mesh::Plane.Init();
		//	Mesh::Plane.output();

			Mesh::Cube.mformat = "FBX";
			Mesh::Cube.name = "Cube";
			Mesh::Cube.ImportWithTriangeNot = true;
			Mesh::Cube.Load(CUBE_PATH);
			Mesh::Cube.Init();
			Mesh::Cube.normalized(Vector3(0,0,16.2007f),Vector3(15.1878f,15.7292f,16.2007f));

			Mesh::Sphere.mformat = "FBX";
			Mesh::Sphere.name = "Sphere";
			Mesh::Sphere.Load(SPERE_PATH);
			Mesh::Sphere.Init();

			Mesh::Cylinder.mformat = "FBX";
			Mesh::Cylinder.name = "Cylinder";
			Mesh::Cylinder.Load(CYLINDER_PATH);
			Mesh::Cylinder.Init();


			Mesh::Pipe.mformat = "FBX";
			Mesh::Pipe.name = "Pipe";
			Mesh::Pipe.ImportWithTriangeNot = true;
			Mesh::Pipe.Load(PIPE_PATH);
			Mesh::Pipe.Init();
			
			Mesh::Rectangular_pyramid.mformat = "FBX";
			Mesh::Rectangular_pyramid.name = "Rectangular_pyramid";
			Mesh::Rectangular_pyramid.Load(RECTANGULAR_PYRAMID_PATH);
			Mesh::Rectangular_pyramid.Init();

		}
	};

}