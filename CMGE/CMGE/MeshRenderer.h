#pragma once
#include"Renderer.h"
#include"Mesh.h"
using namespace CMGERenderEngine;
class MeshRenderer :public Renderer 
{
public:
	MeshRenderer()
	{
		Renderer::Renderer();
		
	}
	static void* createComponent()
	{
		return new MeshRenderer;
	}

	void Init()
	{
		ClassName = "MeshRenderer";
	//	cout << Renderer::fatherName.size() << endl;
		for (int i = 0; i < Renderer::fatherName.size(); i++)
			fatherName.push_back(Renderer::fatherName[i]);
		fatherName.push_back("Renderer");

		ComponentFactory::getInstance().registClass(ClassName, (CreateComponent)createComponent);
	}

	void Start() 
	{ 
		Renderer::Start(); 
	}

	void Update() 
	{ 
		Renderer::Update(); 
	}

	void Draw(int preornot) 
	{
		Renderer::Draw(preornot);
#ifdef DEBUG
		float const vertexPosition[] = {
			1.0f, -1.0f,
			-1.0f, -1.0f,
			1.0f, 1.0f,
			-1.0f, 1.0f
		};
		float const textureCoord[] = {
			1.0f, 0.0f,
			0.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f
		};
		float const norCoord[] = {
			1.0f, 0.0f,
			0.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f
		};
		glUseProgram(mMaterial->GetShader()->GetProgram());
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPosition) + sizeof(textureCoord)+sizeof(norCoord), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexPosition), vertexPosition);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexPosition), sizeof(textureCoord), textureCoord);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexPosition)+sizeof(norCoord), sizeof(norCoord), norCoord);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertexPosition)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertexPosition)+sizeof(norCoord)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glEnd();
		return;
#endif // DEBUG
	//	cout << "sda" << endl;
		if (mMesh != NULL)
		{

			glUseProgram(mMaterial->GetShader()->GetProgram());
		//	mMesh->output();
			int  allSize = mMesh->num_Position * sizeof(vec3) + mMesh->num_Normal * sizeof(vec3)
				+ mMesh->num_Tangent * sizeof(vec3) + mMesh->num_Color * sizeof(vec4) +
				mMesh->num_UVCoord * sizeof(vec2) + mMesh->num_UvCoordNormal * sizeof(vec2);
			glBufferData(GL_ARRAY_BUFFER, allSize, NULL, GL_STATIC_DRAW);
			int tempSize = 0;
			glBufferSubData(GL_ARRAY_BUFFER, tempSize, sizeof(vec3)*mMesh->num_Position, mMesh->glpos);
			tempSize += sizeof(vec3)*mMesh->num_Position;
			glBufferSubData(GL_ARRAY_BUFFER, tempSize, sizeof(vec3)*mMesh->num_Normal, mMesh->glnor);
			tempSize += sizeof(vec3)*mMesh->num_Normal;
			glBufferSubData(GL_ARRAY_BUFFER, tempSize, sizeof(vec2)*mMesh->num_UVCoord, mMesh->gluvcoord);
			tempSize += sizeof(vec2)*mMesh->num_UVCoord;
			glBufferSubData(GL_ARRAY_BUFFER, tempSize, sizeof(vec2)*mMesh->num_UvCoordNormal, mMesh->gluvcoordnormal);
			tempSize += sizeof(vec2)*mMesh->num_UvCoordNormal;
			glBufferSubData(GL_ARRAY_BUFFER, tempSize, sizeof(vec3)*mMesh->num_Tangent, mMesh->gltangent);
			tempSize += sizeof(vec3)*mMesh->num_Tangent;
			glBufferSubData(GL_ARRAY_BUFFER, tempSize, sizeof(vec4)*mMesh->num_Color, mMesh->glcolor);

		//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMesh->num_Index * sizeof(int), mMesh->mTriangleIndex, GL_STATIC_DRAW);

			int tempSize2 = 0;
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
				sizeof(vec3), BUFFER_OFFSET(tempSize2));


			tempSize2 += sizeof(vec3)*mMesh->num_Position;
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE,
				sizeof(vec3), BUFFER_OFFSET(tempSize2));


			tempSize2 += sizeof(vec3)*mMesh->num_Normal;
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
				sizeof(vec2), BUFFER_OFFSET(tempSize2));


			tempSize2 += sizeof(vec2)*mMesh->num_UVCoord;
			glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,
				sizeof(vec2), BUFFER_OFFSET(tempSize2));


			tempSize2 += sizeof(vec2)*mMesh->num_UvCoordNormal;
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE,
				sizeof(vec3), BUFFER_OFFSET(tempSize2));


			tempSize2 += sizeof(vec3)*mMesh->num_Tangent;
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE,
				sizeof(vec4), BUFFER_OFFSET(tempSize2));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);
			glEnableVertexAttribArray(4);
			glEnableVertexAttribArray(5);


			glBindVertexArray(VAO[0]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
			glDrawElements(GL_TRIANGLES, mMesh->num_Index, GL_UNSIGNED_INT, mMesh->mTriangleIndex);
			glEnd();
		}
	}

	void SetMesh(Mesh* m)
	{ 
		mMesh = m; 
	}

	Mesh* GetMesh()
	{ 
		return mMesh; 
	}

private:
	Mesh* mMesh;
};