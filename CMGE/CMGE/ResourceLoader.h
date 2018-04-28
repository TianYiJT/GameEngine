#pragma once
#include<string>
#include"CMGEMathEngine.h"
#include"stb_image.h"
#include<fbxsdk.h>
#include<vector>
using namespace std;
using namespace CMGEMathEngine;
using namespace fbxsdk;
namespace CMGETool
{
	class ResourceLoader
	{
	public:
		ResourceLoader() {}
		~ResourceLoader() {}
		void Load(string path) {}
	protected:
		string mformat;
	private:

	};
	struct CMGEFBXNode
	{
		Vector3 * mPosition;
		Vector3 * mNormal;
		Vector2 * mUVCoord;
		Vector2 * mUVCoordNormal;
		Vector4 * mColor;
		Vector3 * mTangent;
		int * mTriangleIndex;
		int num_Position;
		int num_Normal;
		int num_UVCoord;
		int num_UvCoordNormal;
		int num_Color;
		int num_Tangent;
		int num_Index;

	};
	class CMGEFBXLoader:ResourceLoader 
	{
	public:
		CMGEFBXLoader() { mformat = "fbx"; }
		~CMGEFBXLoader() {}
		static CMGEFBXLoader * fbxInstance;
		static CMGEFBXLoader* GetInstance()
		{
			if(fbxInstance==NULL)
			{
				fbxInstance = new CMGEFBXLoader();
				return fbxInstance;
			}
			else
			{
				return fbxInstance;
			}
		}


		bool Load(string path) 
		{
			mFbxImporter = FbxImporter::Create(mFbxManager, "");
			if (mFbxManager == NULL) 
			{
				return false;
			}
			if (!mFbxImporter->Initialize(path.c_str(),-1,mFbxManager->GetIOSettings()))
			{
				printf("Call to FbxImporter::Initialize() failed.\n");
				printf("Error returned: %s\n\n", mFbxImporter->GetStatus().GetErrorString());
				exit(-1);
			}
			mFbxScene->Clear();
			mFbxImporter->Import(mFbxScene);
			mFbxImporter->Destroy();
			FbxNode * top = mFbxScene->GetRootNode();
			ProcessNode(top);
		}

		void Clear() 
		{
			mMesh.clear();
		}

		void Init() 
		{
			mFbxManager = FbxManager::Create();
			mFbxIOSettings = FbxIOSettings::Create(mFbxManager, IOSROOT);
			mFbxManager->SetIOSettings(mFbxIOSettings);
			FbxString Extension = "dll";
			FbxString Path = FbxGetApplicationDirectory();
			mFbxManager->LoadPluginsDirectory(Path.Buffer(),Extension.Buffer());
			mFbxScene = FbxScene::Create(mFbxManager,"");
		}

		void ProcessNode(FbxNode* pNode) 
		{
			FbxNodeAttribute::EType attributeType;
			if (pNode->GetNodeAttribute())
			{
				switch (pNode->GetNodeAttribute()->GetAttributeType())
				{
				case FbxNodeAttribute::eMesh:
					ProcessMesh(pNode);
					break;
				case FbxNodeAttribute::eSkeleton:
					break;
				case FbxNodeAttribute::eLight:
					break;
				case FbxNodeAttribute::eCamera:
					break;
				}
			}
			for (int i = 0; i < pNode->GetChildCount(); ++i)
			{
				ProcessNode(pNode->GetChild(i));
			}
		}

		void ProcessMesh(FbxNode* pNode) 
		{
			FbxMesh * pMesh = pNode->GetMesh();
			if (pMesh == NULL) 
			{
				return;
			}
			CMGEFBXNode * tempMesh = new CMGEFBXNode();
			int numVertices = pMesh->GetControlPointsCount();
			Vector3 * tempVert = new Vector3[numVertices];
			for (int i = 0; i < numVertices; i++) 
			{
				FbxVector4 coord = pMesh->GetControlPointAt(i);
				tempVert[i].x = (float)coord[0];
				tempVert[i].y = (float)coord[1];
				tempVert[i].z = (float)coord[2];
			}

			tempMesh->num_Position = numVertices;
			tempMesh->mPosition = tempVert;
			int numIndices = pMesh->GetPolygonVertexCount();
			int* indices = pMesh->GetPolygonVertices();
			tempMesh->mTriangleIndex = indices;
			tempMesh->num_Index = numIndices;

			FbxGeometryElementNormal* normalElement = pMesh->GetElementNormal();
			FbxGeometryElementUV * UVElement = pMesh->GetElementUV(0);
			FbxGeometryElementUV * UVElementNor = pMesh->GetElementUV(1);
			FbxGeometryElementVertexColor * ColorElement = pMesh->GetElementVertexColor();
			FbxGeometryElementTangent * TangentElement = pMesh->GetElementTangent();
			//FbxGeometryElementBinormal * BinormalElement = pMesh->GetElementBinormal();
			//if (BinormalElement != NULL)
			//	cout << "NOTNULL" << endl;

			int triangleCount = pMesh->GetPolygonCount();

			int vertexCounter = 0;
			int num_normal = triangleCount * 3;
			int num_UV = triangleCount * 3;
			int num_UVNormal = triangleCount * 3;
			int num_Color = triangleCount * 3;
			int num_Tangent = triangleCount * 3;

			Vector3 * normal = new Vector3[num_normal];
			Vector2 * UV = new Vector2[num_UV];
			Vector2 * UVNormal = new Vector2[num_UVNormal];
			Vector4 * Color = new Vector4[num_Color];
			Vector3 * Tangent = new Vector3[num_Tangent];

			for (int i = 0; i < triangleCount; i++) 
			{
				for (int j = 0; j < 3; j++) 
				{
					int ctrlPointIndex = pMesh->GetPolygonVertex(i, j);
					if(ColorElement!=NULL)
						Color[vertexCounter] = ReadColor(ColorElement, ctrlPointIndex, vertexCounter);
					if (normalElement != NULL)
						normal[vertexCounter] = ReadNormal(normalElement,ctrlPointIndex,vertexCounter);
					if (UVElement != NULL)
						UV[vertexCounter] = ReadUV(UVElement, ctrlPointIndex, pMesh->GetTextureUVIndex(i, j));
					if (UVElementNor != NULL)
						UVNormal[vertexCounter] = ReadUV(UVElementNor, ctrlPointIndex, pMesh->GetTextureUVIndex(i, j));
					if (TangentElement != NULL)
						Tangent[vertexCounter] = ReadTangent(TangentElement,ctrlPointIndex,vertexCounter);
					vertexCounter++;
				}
			}

			if (ColorElement != NULL) 
			{
				tempMesh->mColor = Color;
				tempMesh->num_Color = num_Color;
			}

			if (normalElement != NULL)
			{
				tempMesh->mNormal = normal;
				tempMesh->num_Normal = num_normal;
			}

			if (UVElement != NULL)
			{
				tempMesh->mUVCoord = UV;
				tempMesh->num_UVCoord = num_UV;
			}

			if (UVElementNor != NULL)
			{
				tempMesh->mUVCoordNormal = UVNormal;
				tempMesh->num_UvCoordNormal = num_UVNormal;
			}

			if (TangentElement != NULL)
			{
				tempMesh->mTangent = Tangent;
				tempMesh->num_Tangent = num_Tangent;
			}

			mMesh.push_back(tempMesh);
		}

		Vector4 ReadColor(FbxGeometryElementVertexColor* pColorElement, int ctrlPointIndex, int vertexCounter)
		{
			Vector4 col;
			switch (pColorElement->GetMappingMode())
			{
			case FbxGeometryElement::eByControlPoint:
			{
				switch (pColorElement->GetReferenceMode())
				{
				case FbxGeometryElement::eDirect:
				{
					col.x = pColorElement->GetDirectArray().GetAt(ctrlPointIndex).mRed;
					col.y = pColorElement->GetDirectArray().GetAt(ctrlPointIndex).mGreen;
					col.z= pColorElement->GetDirectArray().GetAt(ctrlPointIndex).mBlue;
					col.w = pColorElement->GetDirectArray().GetAt(ctrlPointIndex).mAlpha;
				}
				break;

				case FbxGeometryElement::eIndexToDirect:
				{
					int id = pColorElement->GetIndexArray().GetAt(ctrlPointIndex);
					col.x = pColorElement->GetDirectArray().GetAt(id).mRed;
					col.y = pColorElement->GetDirectArray().GetAt(id).mGreen;
					col.z= pColorElement->GetDirectArray().GetAt(id).mBlue;
					col.w = pColorElement->GetDirectArray().GetAt(id).mAlpha;
				}
				break;

				default:
					break;
				}
			}
			break;

			case FbxGeometryElement::eByPolygonVertex:
			{
				switch (pColorElement->GetReferenceMode())
				{
				case FbxGeometryElement::eDirect:
				{
					col.x = pColorElement->GetDirectArray().GetAt(vertexCounter).mRed;
					col.y = pColorElement->GetDirectArray().GetAt(vertexCounter).mGreen;
					col.z = pColorElement->GetDirectArray().GetAt(vertexCounter).mBlue;
					col.w = pColorElement->GetDirectArray().GetAt(vertexCounter).mAlpha;
				}
				break;
				case FbxGeometryElement::eIndexToDirect:
				{
					int id = pColorElement->GetIndexArray().GetAt(vertexCounter);
					col.x = pColorElement->GetDirectArray().GetAt(id).mRed;
					col.y= pColorElement->GetDirectArray().GetAt(id).mGreen;
					col.z = pColorElement->GetDirectArray().GetAt(id).mBlue;
					col.w = pColorElement->GetDirectArray().GetAt(id).mAlpha;
				}
				break;
				default:
					break;
				}
			}
			break;
			}
			return col;
		}

		Vector2 ReadUV(FbxGeometryElementUV* pUVElement, int ctrlPointIndex, int textureUVIndex)
		{
			Vector2 pUV;
			switch (pUVElement->GetMappingMode())
			{
			case FbxGeometryElement::eByControlPoint:
			{
				switch (pUVElement->GetReferenceMode())
				{
				case FbxGeometryElement::eDirect:
				{
					pUV.x = pUVElement->GetDirectArray().GetAt(ctrlPointIndex)[0];
					pUV.y = pUVElement->GetDirectArray().GetAt(ctrlPointIndex)[1];
				}
				break;

				case FbxGeometryElement::eIndexToDirect:
				{
					int id = pUVElement->GetIndexArray().GetAt(ctrlPointIndex);
					pUV.x = pUVElement->GetDirectArray().GetAt(id)[0];
					pUV.y = pUVElement->GetDirectArray().GetAt(id)[1];
				}
				break;

				default:
					break;
				}
			}
			break;

			case FbxGeometryElement::eByPolygonVertex:
			{
				switch (pUVElement->GetReferenceMode())
				{
				case FbxGeometryElement::eDirect:
				case FbxGeometryElement::eIndexToDirect:
				{
					pUV.x = pUVElement->GetDirectArray().GetAt(textureUVIndex)[0];
					pUV.y = pUVElement->GetDirectArray().GetAt(textureUVIndex)[1];
				}
				break;

				default:
					break;
				}
			}
			break;
			}
			return pUV;
		}

		Vector3 ReadNormal(FbxGeometryElementNormal* pNormalElement, int ctrlPointIndex, int vertexCounter)
		{
			Vector3 pNormal;
			switch (pNormalElement->GetMappingMode())
			{
			case FbxGeometryElement::eByControlPoint:
			{
				switch (pNormalElement->GetReferenceMode())
				{
				case FbxGeometryElement::eDirect:
				{
					pNormal.x = pNormalElement->GetDirectArray().GetAt(ctrlPointIndex)[0];
					pNormal.y = pNormalElement->GetDirectArray().GetAt(ctrlPointIndex)[1];
					pNormal.z = pNormalElement->GetDirectArray().GetAt(ctrlPointIndex)[2];
				}
				break;

				case FbxGeometryElement::eIndexToDirect:
				{
					int id = pNormalElement->GetIndexArray().GetAt(ctrlPointIndex);
					pNormal.x = pNormalElement->GetDirectArray().GetAt(id)[0];
					pNormal.y = pNormalElement->GetDirectArray().GetAt(id)[1];
					pNormal.z = pNormalElement->GetDirectArray().GetAt(id)[2];
				}
				break;

				default:
					break;
				}
			}
			break;

			case FbxGeometryElement::eByPolygonVertex:
			{
				switch (pNormalElement->GetReferenceMode())
				{
				case FbxGeometryElement::eDirect:
				{
					pNormal.x = pNormalElement->GetDirectArray().GetAt(vertexCounter)[0];
					pNormal.y = pNormalElement->GetDirectArray().GetAt(vertexCounter)[1];
					pNormal.z = pNormalElement->GetDirectArray().GetAt(vertexCounter)[2];
				}
				break;

				case FbxGeometryElement::eIndexToDirect:
				{
					int id = pNormalElement->GetIndexArray().GetAt(vertexCounter);
					pNormal.x = pNormalElement->GetDirectArray().GetAt(id)[0];
					pNormal.y = pNormalElement->GetDirectArray().GetAt(id)[1];
					pNormal.z = pNormalElement->GetDirectArray().GetAt(id)[2];
				}
				break;

				default:
					break;
				}
			}
			break;
			}
			return pNormal;
		}

		Vector3 ReadTangent(FbxGeometryElementTangent* pTangentElement, int ctrlPointIndex, int vertecCounter)
		{
			Vector3 pTangent;

			switch (pTangentElement->GetMappingMode())
			{
			case FbxGeometryElement::eByControlPoint:
			{
				switch (pTangentElement->GetReferenceMode())
				{
				case FbxGeometryElement::eDirect:
				{
					pTangent.x = pTangentElement->GetDirectArray().GetAt(ctrlPointIndex)[0];
					pTangent.y = pTangentElement->GetDirectArray().GetAt(ctrlPointIndex)[1];
					pTangent.z = pTangentElement->GetDirectArray().GetAt(ctrlPointIndex)[2];
				}
				break;

				case FbxGeometryElement::eIndexToDirect:
				{
					int id = pTangentElement->GetIndexArray().GetAt(ctrlPointIndex);
					pTangent.x = pTangentElement->GetDirectArray().GetAt(id)[0];
					pTangent.y = pTangentElement->GetDirectArray().GetAt(id)[1];
					pTangent.z = pTangentElement->GetDirectArray().GetAt(id)[2];
				}
				break;

				default:
					break;
				}
			}
			break;

			case FbxGeometryElement::eByPolygonVertex:
			{
				switch (pTangentElement->GetReferenceMode())
				{
				case FbxGeometryElement::eDirect:
				{
					pTangent.x = pTangentElement->GetDirectArray().GetAt(vertecCounter)[0];
					pTangent.y = pTangentElement->GetDirectArray().GetAt(vertecCounter)[1];
					pTangent.z = pTangentElement->GetDirectArray().GetAt(vertecCounter)[2];
				}
				break;

				case FbxGeometryElement::eIndexToDirect:
				{
					int id = pTangentElement->GetIndexArray().GetAt(vertecCounter);
					pTangent.x = pTangentElement->GetDirectArray().GetAt(id)[0];
					pTangent.y = pTangentElement->GetDirectArray().GetAt(id)[1];
					pTangent.z = pTangentElement->GetDirectArray().GetAt(id)[2];
				}
				break;

				default:
					break;
				}
			}
			break;
			}
			return pTangent;
		}

		vector<CMGEFBXNode*> mMesh;
		FbxManager* mFbxManager;
		FbxIOSettings * mFbxIOSettings;
		FbxImporter * mFbxImporter;
		FbxScene * mFbxScene;
		
	};
	class TextureLoader :ResourceLoader 
	{
	public:
		TextureLoader() {}
		~TextureLoader() {}
		void Load(const char* path) 
		{
			int width,height,numComponents;
		    mData=stbi_load(path, &width, &height, &numComponents, 4);
			mWidth = width;
			mHeight = height;
			
			unsigned char* mDataTemp = new unsigned char[width*height*4];
			for (int i = 0; i < width; i++) 
			{
				for (int j = 0; j < height; j++) 
				{
					int realX = width-i-1;
					int realY = j;
					mDataTemp[(i*height + j) * 4] = mData[(realX*height + realY) * 4];
					mDataTemp[(i*height + j) * 4+1] = mData[(realX*height + realY) * 4+1];
					mDataTemp[(i*height + j) * 4+2] = mData[(realX*height + realY) * 4+2];
					mDataTemp[(i*height + j) * 4 + 3] = mData[(realX*height + realY) * 4 + 3];
				}
			}
			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height; j++)
				{
					for (int k = 0; k < 4; k++) 
					{
						mData[(i*height + j) * 4 + k] = mDataTemp[(i*height+j)*4+k];
					}
				}
			}
			delete mDataTemp;
			mComponents = numComponents;
		}
		unsigned char* mData;
		int mWidth,mHeight,mComponents;
	};
	class TextureLoader_BMP :ResourceLoader
	{
	public:
		TextureLoader_BMP() {}
		~TextureLoader_BMP() {}
		void Load(const char* path)
		{
			int width, height, numComponents;
			mData = stbi_load(path, &width, &height, &numComponents, 4);
			mWidth = width;
			mHeight = height;
			mComponents = numComponents;
		}
		unsigned char* mData;
		int mWidth, mHeight, mComponents;
	};
}
