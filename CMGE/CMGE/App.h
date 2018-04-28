#pragma once
#include"SceneManager.h"


using namespace CMGERenderEngine;
using namespace CMGEInput;

void GLUpdate() 
{
	if (SceneManager::tempScene != NULL)
		SceneManager::tempScene->Update();
	InputSystem::Update();
}

void GLIdle()
{
	GLUpdate();
}

void GLReshape(int x,int y)
{
	Camera::MainCamera->SetScreenVector(Vector2(x,y));
}

class App
{
public: 
	int mWidth, mHeight;
	App(int argc,char** argv)
	{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	}
	App() {}
	virtual void Start() 
	{
		Texture2D::GlobalStartUp();
		Shader::GlobalStartUp();
		Mesh::GlobalStartUp();
		Material::GlobalStartUp();
		CubeTexture::GlobalStartUp();
		AudioPlayer::ALStartUp();

		glutInitWindowSize(mWidth, mHeight);
		glutCreateWindow("Game");
		glutInitContextVersion(4, 2);
		glutInitContextProfile(GLUT_CORE_PROFILE);

		if (glewInit())
		{
			exit(EXIT_FAILURE);
		}
		Renderer::RendererInit();
		glutDisplayFunc(GLUpdate);
		glutReshapeFunc(GLReshape);
		glutSpecialFunc(InputSystem::OnSpecial);
		glutSpecialUpFunc(InputSystem::OnSpecialUp);
		glutKeyboardFunc(InputSystem::OnKeyboard);
		glutKeyboardUpFunc(InputSystem::OnKeyboardUp);
		glutMouseFunc(InputSystem::OnMouse);
	//	glutMotionFunc(InputSystem::OnMotion);
		glutPassiveMotionFunc(InputSystem::OnMotion);
		glutIdleFunc(GLIdle);
	}
};