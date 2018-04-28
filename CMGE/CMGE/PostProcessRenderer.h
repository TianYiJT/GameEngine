#pragma once
#include"Renderer.h"

using namespace CMGERenderEngine;

namespace CMGERenderEngine
{
	class PostProcessRenderer:public Renderer
	{
	public:
		PostProcessRenderer()
		{
			Renderer::Renderer();

		}
		static void* createComponent()
		{
			return new PostProcessRenderer;
		}

		void Init()
		{
			ClassName = "PostProcessRenderer";
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

			Camera::MainCamera->SetFrameMode(isFLOAT);

			Renderer::Update();
		}

		void SetFrameMode(bool isfloat) 
		{
			isFLOAT = isfloat;
		}

		void Draw(int preornot)
		{
			Renderer::Draw(preornot);
			if(preornot==1)
			{
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
			//	cout << mMaterial->GetShader()->GetProgram() << endl;
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPosition) + sizeof(textureCoord) + sizeof(norCoord), NULL, GL_STATIC_DRAW);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexPosition), vertexPosition);
				glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexPosition), sizeof(norCoord), norCoord);
				glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexPosition) + sizeof(textureCoord), sizeof(textureCoord), textureCoord);
				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertexPosition)));
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertexPosition) + sizeof(norCoord)));
				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);
				glEnableVertexAttribArray(2);
				glBindVertexArray(VAO[0]);
				glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				glEnd();
			}
		}

	private:
		bool isFLOAT = false;
	};
}