#pragma once
#include"Renderer.h"
#include"vmath.h"

using namespace vmath;

class LineRenderer :public Renderer 
{
public:
	LineRenderer()
	{
		Renderer::Renderer();

	}
	static void* createComponent()
	{
		return new LineRenderer;
	}

	void Init()
	{
		ClassName = "LineRenderer";

		for (int i = 0; i < Renderer::fatherName.size(); i++)
			fatherName.push_back(Renderer::fatherName[i]);
		fatherName.push_back("Renderer");

		ComponentFactory::getInstance().registClass(ClassName, (CreateComponent)createComponent);
	}

	void Start()
	{
		Renderer::Start();
		Shader * s_solid = new Shader();
		s_solid->Load("SolidColor.shader","SolidColor","shader");
		//s_solid->output();
		mMaterial = new Material();
		mMaterial->SetShader(s_solid);
		Vector4 *red = new Vector4(1.0,0.0,0.0,1.0f);
		mMaterial->SetMaterialParam("mColor",red);
	}

	void Update()
	{
		Renderer::Update();
	}

	void Draw(int preornot)
	{
		Renderer::Draw(preornot);
		if (tempCount != NULL)
		{
			glUseProgram(mMaterial->GetShader()->GetProgram());
			//	mMesh->output();
			int  allSize = tempCount * sizeof(vec3) + tempCount * sizeof(vec3)
				+ tempCount * sizeof(vec3) + tempCount * sizeof(vec4) +
				tempCount * sizeof(vec2) + tempCount * sizeof(vec2);
			glBufferData(GL_ARRAY_BUFFER, allSize, NULL, GL_STATIC_DRAW);
			int tempSize = 0;
			glBufferSubData(GL_ARRAY_BUFFER, tempSize, sizeof(vec3)*tempCount, LineVertexs);
			tempSize += sizeof(vec3)*tempCount;
			glBufferSubData(GL_ARRAY_BUFFER, tempSize, sizeof(vec3)*tempCount, LineVertexs);
			tempSize += sizeof(vec3)*tempCount;
			glBufferSubData(GL_ARRAY_BUFFER, tempSize, sizeof(vec2)*tempCount, LineVertexs);
			tempSize += sizeof(vec2)*tempCount;
			glBufferSubData(GL_ARRAY_BUFFER, tempSize, sizeof(vec2)*tempCount, LineVertexs);
			tempSize += sizeof(vec2)*tempCount;
			glBufferSubData(GL_ARRAY_BUFFER, tempSize, sizeof(vec3)*tempCount, LineVertexs);
			tempSize += sizeof(vec3)*tempCount;
			glBufferSubData(GL_ARRAY_BUFFER, tempSize, sizeof(vec4)*tempCount, LineColors);

			//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMesh->num_Index * sizeof(int), mMesh->mTriangleIndex, GL_STATIC_DRAW);

			int tempSize2 = 0;
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
				sizeof(vec3), BUFFER_OFFSET(tempSize2));


			tempSize2 += sizeof(vec3)*tempCount;
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE,
				sizeof(vec3), BUFFER_OFFSET(tempSize2));


			tempSize2 += sizeof(vec3)*tempCount;
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
				sizeof(vec2), BUFFER_OFFSET(tempSize2));


			tempSize2 += sizeof(vec2)*tempCount;
			glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,
				sizeof(vec2), BUFFER_OFFSET(tempSize2));


			tempSize2 += sizeof(vec2)*tempCount;
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE,
				sizeof(vec3), BUFFER_OFFSET(tempSize2));


			tempSize2 += sizeof(vec3)*tempCount;
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
			glDrawArrays(GL_LINE_STRIP, 0, tempCount);
			glEnd();
		}
	}

	void AddPoint(vec3 v)
	{
		LineVertexs[tempCount] = v;
		LineColors[tempCount] = vec4(1,0,0,1);
		tempCount++;
	}

	void DeletePoint()
	{
		if(tempCount>0)
			tempCount--;
	}

	Vector2 getPoint(int index) 
	{
		return Vector2(LineVertexs[index][0],LineVertexs[index][1]);
	}

	void Clear() 
	{
		tempCount = 0;
	}
	int tempCount = 0;
private:
	vec3 *LineVertexs = new vec3[2048];
	vec4 *LineColors = new vec4[2048];
};