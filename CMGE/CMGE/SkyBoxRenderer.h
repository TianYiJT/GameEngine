#pragma once
#include"Mesh.h"
#include"Renderer.h"


using namespace CMGERenderEngine;

namespace CMGERenderEngine 
{
	class SkyBoxRenderer :public Renderer
	{
	public:
		SkyBoxRenderer()
		{
			Renderer::Renderer();

		}
		static void* createComponent()
		{
			return new SkyBoxRenderer;
		}

		void Init()
		{
			ClassName = "SkyBoxRenderer";
			for (int i = 0; i < Renderer::fatherName.size(); i++)
				fatherName.push_back(Renderer::fatherName[i]);
			fatherName.push_back("Renderer");
			ComponentFactory::getInstance().registClass(ClassName, (CreateComponent)createComponent);
		}

		void Start()
		{
			Renderer::Start();
			SkyBoxMesh = ResourceManager<Mesh>::getInstance()->getData("Cube");
		}

		void Update()
		{
			Renderer::Update();
		}

		void Draw(int preornot)
		{
			Renderer::Draw(preornot);
			Transform skyboxTransform;
			Transform mGameObjectTransform = ((GameObject*)mGameObject)->transform;
			skyboxTransform.Translate(mGameObjectTransform.Position+Vector3(0,0,0));
			skyboxTransform.Rotate(mGameObjectTransform.Rotation);
			skyboxTransform.Scale(Vector3(30.0f,30.0f,30.0f));
			Camera * tempMain = Camera::MainCamera;
			Matrix4x4 MVP = tempMain->cameraToScreenMatrix*tempMain->worldToCameraMatrix*skyboxTransform.Model();
			Matrix4x4 O2W = skyboxTransform.Model();
			Matrix4x4 W2O = skyboxTransform.Model().inverse();
			mMaterial->SetMaterialParam("CMGE_MVP", &MVP);
			mMaterial->SetMaterialParam("CMGE_O2W", &O2W);
			mMaterial->SetMaterialParam("CMGE_W2O", &W2O);
			mMaterial->Update();
			glUseProgram(mMaterial->GetShader()->GetProgram());
			int  allSize = SkyBoxMesh->num_Position * sizeof(vec3) + SkyBoxMesh->num_Normal * sizeof(vec3)
				+ SkyBoxMesh->num_Tangent * sizeof(vec3) + SkyBoxMesh->num_Color * sizeof(vec4) +
				SkyBoxMesh->num_UVCoord * sizeof(vec2) + SkyBoxMesh->num_UvCoordNormal * sizeof(vec2);
			glBufferData(GL_ARRAY_BUFFER, allSize, NULL, GL_STATIC_DRAW);
			int tempSize = 0;
			glBufferSubData(GL_ARRAY_BUFFER, tempSize, sizeof(vec3)*SkyBoxMesh->num_Position, SkyBoxMesh->glpos);
			tempSize += sizeof(vec3)*SkyBoxMesh->num_Position;
			glBufferSubData(GL_ARRAY_BUFFER, tempSize, sizeof(vec3)*SkyBoxMesh->num_Normal, SkyBoxMesh->glnor);
			tempSize += sizeof(vec3)*SkyBoxMesh->num_Normal;
			glBufferSubData(GL_ARRAY_BUFFER, tempSize, sizeof(vec2)*SkyBoxMesh->num_UVCoord, SkyBoxMesh->gluvcoord);
			tempSize += sizeof(vec2)*SkyBoxMesh->num_UVCoord;
			glBufferSubData(GL_ARRAY_BUFFER, tempSize, sizeof(vec2)*SkyBoxMesh->num_UvCoordNormal, SkyBoxMesh->gluvcoordnormal);
			tempSize += sizeof(vec2)*SkyBoxMesh->num_UvCoordNormal;
			glBufferSubData(GL_ARRAY_BUFFER, tempSize, sizeof(vec3)*SkyBoxMesh->num_Tangent, SkyBoxMesh->gltangent);
			tempSize += sizeof(vec3)*SkyBoxMesh->num_Tangent;
			glBufferSubData(GL_ARRAY_BUFFER, tempSize, sizeof(vec4)*SkyBoxMesh->num_Color, SkyBoxMesh->glcolor);

			//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMesh->num_Index * sizeof(int), mMesh->mTriangleIndex, GL_STATIC_DRAW);

			int tempSize2 = 0;
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
				sizeof(vec3), BUFFER_OFFSET(tempSize2));


			tempSize2 += sizeof(vec3)*SkyBoxMesh->num_Position;
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE,
				sizeof(vec3), BUFFER_OFFSET(tempSize2));


			tempSize2 += sizeof(vec3)*SkyBoxMesh->num_Normal;
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
				sizeof(vec2), BUFFER_OFFSET(tempSize2));


			tempSize2 += sizeof(vec2)*SkyBoxMesh->num_UVCoord;
			glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,
				sizeof(vec2), BUFFER_OFFSET(tempSize2));


			tempSize2 += sizeof(vec2)*SkyBoxMesh->num_UvCoordNormal;
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE,
				sizeof(vec3), BUFFER_OFFSET(tempSize2));


			tempSize2 += sizeof(vec3)*SkyBoxMesh->num_Tangent;
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE,
				sizeof(vec4), BUFFER_OFFSET(tempSize2));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);
			glEnableVertexAttribArray(4);
			glEnableVertexAttribArray(5);


			glBindVertexArray(VAO[0]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
			glDrawElements(GL_TRIANGLES, SkyBoxMesh->num_Index, GL_UNSIGNED_INT, SkyBoxMesh->mTriangleIndex);
			glEnd();
		}

	private:
		Mesh* SkyBoxMesh;
	};
}