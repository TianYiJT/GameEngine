#include"CMGEInput.h"

using namespace CMGEInput;

bool * InputSystem::keydownState = new bool[KEYBOARDSIZE];
bool * InputSystem::keyState = new bool[KEYBOARDSIZE];
bool * InputSystem::keyupState = new bool[KEYBOARDSIZE];
float * InputSystem::keyTime = new float[KEYBOARDSIZE];
float * InputSystem::MouseTime = new float[KEYBOARDSIZE];
bool * InputSystem::mousebuttonupState = new bool[MOUSETYPESIZE];
bool * InputSystem::mousebuttondownState = new bool[MOUSETYPESIZE];
bool * InputSystem::mousebuttonState = new bool[MOUSETYPESIZE];
map<string, vector<KeyBoard>> InputSystem::buttonList = map<string, vector<KeyBoard>>();
map<string, bool> InputSystem::buttonListState = map<string, bool>();
double InputSystem::NowTime = 0.0f;
double InputSystem::LastTime = 0.0f;
double InputSystem::deltaTime = 0.0f;
Vector2 InputSystem::mousePosition = Vector2(0.0f, 0.0f);
Vector2 InputSystem::mouseVelocity = Vector2(0.0f, 0.0f);
Vector2 InputSystem::LastMousePosition = Vector2(0.0f, 0.0f);
double InputSystem::StartTime = 0.0f;
float InputSystem::bufferTime = 1.0f;
float InputSystem::timer_keybuffer = 0.0f;
vector<KeyBoard> InputSystem::KeyBoardBuffer = vector<KeyBoard>();



float InputSystem::TransferTime(SYSTEMTIME st)
{
	float Day = st.wDay;
	float Hour = st.wHour;
	float Minute = st.wMinute;
	float second = st.wSecond;
	float MiniSecond = st.wMilliseconds;
	float f = Day * 24 * 60 * 60 + Hour * 60 * 60 + Minute * 60 + second + MiniSecond / 1000.0f;
//	cout << f << endl;
	return f;
}


Vector2 InputSystem::GetMousePosition() 
{
	return mousePosition;
}

bool InputSystem::GetMouseButton(MouseButton mb) 
{
	return mousebuttonState[mb];
}

bool InputSystem::GetMouseButtonDown(MouseButton mb) 
{
	return mousebuttondownState[mb];
}

bool InputSystem::GetMouseButtonUp(MouseButton mb) 
{
	return mousebuttonupState[mb];
}


Vector2 InputSystem::GetMouseDeltaVelocity() 
{
	return mouseVelocity;
}

bool InputSystem::GetKey(KeyBoard kb) 
{
	return keyState[kb];
}

bool InputSystem::GetKeyDown(KeyBoard kb) 
{
	return keydownState[kb];
}

bool InputSystem::GetKeyUp(KeyBoard kb) 
{
	return keyupState[kb];
}

float InputSystem::GetdeltaTime()
{
	return deltaTime;
}

bool InputSystem::GetButtonDown(string name)
{
	return buttonListState[name];
}

float InputSystem::GetKeyTime(KeyBoard kb) 
{
	return keyTime[kb];
}

float InputSystem::GetMouseTime(MouseButton mb)
{
	return MouseTime[mb];
}

void InputSystem::registerButton(vector<KeyBoard> kbList, string name)
{
	buttonList.insert(pair<string, vector<KeyBoard>>(name,kbList));
	buttonListState.insert(pair<string, bool>(name, false));
}

void InputSystem::deleteButton(string name)
{
	//buttonList.
	buttonList.erase(name);
	buttonListState.erase(name);
}

void InputSystem::Update() 
{
	if(StartTime = 0.0f)
	{
		StartTime = GetTickCount();
	}
	//SYSTEMTIME sys2;
//	GetLocalTime(&sys2);
	
	NowTime = GetTickCount() - StartTime;
	//cout << NowTime<<endl;

	deltaTime = NowTime - LastTime;
	LastTime = NowTime;
	if (timer_keybuffer<bufferTime)
	{
		timer_keybuffer += deltaTime;
	}
	else
	{
		timer_keybuffer = 0.0f;
		KeyBoardBuffer.clear();
	}
	for(int i=0;i<MouseButton::MOUSETYPESIZE;i++)
	{
		if(mousebuttonState[i])
		{
			MouseTime[i] += deltaTime;
		}
		else
		{
			MouseTime[i] = 0.0f;
		}
	}

	for (int i = 0; i<KeyBoard::KEYBOARDSIZE; i++)
	{
		if (keyState[i])
		{
			keyTime[i] += deltaTime;
		}
		else
		{
			keyTime[i] = 0.0f;
		}
		if(keydownState[i])
		{
			KeyBoardBuffer.push_back((KeyBoard)i);
		}
	}

	map<string, vector<KeyBoard>>::iterator it;
	it = buttonList.begin();
	while(it!=buttonList.end())
	{
		string name = it->first;
		vector<KeyBoard> kb = it->second;
		bool buttonState = true;
		int last = 0;
		vector<int> index;
		for(int i=0;i<kb.size();i++)
		{
			if(last==KeyBoardBuffer.size())
			{
				buttonState = false;
				break;
			}
			for(int j=last;j<KeyBoardBuffer.size();j++)
			{
				if(kb[i]==KeyBoardBuffer[j])
				{
					last = j + 1;
					index.push_back(j);
					break;
				}
				else if (j == KeyBoardBuffer.size() - 1) 
				{
					buttonState = false;
				}
			}
		}
		if (KeyBoardBuffer.size() == 0)
			buttonState = false;
		//cout << kb.size() << endl;
		buttonListState[name] = buttonState;
		if(buttonState)
		{
			for(int i=0;i<index.size();i++)
			{
				for(int j=index[i]+1;j<KeyBoardBuffer.size();j++)
				{
					KeyBoardBuffer[j - 1] = KeyBoardBuffer[j];
				}
				for(int j=i+1;j<index.size();j++)
				{
					if(index[j]>index[i])
					{
						index[j]--;
					}
				}
			}
		}
		it++;
	}
	for(int i=0;i<MOUSETYPESIZE;i++)
	{
		mousebuttondownState[i] = false;
		mousebuttonupState[i] = false;
	}
	for(int i=0;i<KEYBOARDSIZE;i++)
	{
		keydownState[i] = false;
		keyupState[i] = false;
	}
}

void InputSystem::OnMotion(int x,int y)
{
	mousePosition = Vector2(x,y);
	mouseVelocity = mousePosition - LastMousePosition;
	LastMousePosition = mousePosition;
	//cout << "sdfa" << endl;
}

void InputSystem::OnMouse(int button, int state, int x, int y)
{

	if(button== GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
	{
		mousebuttondownState[LEFTBUTTON] = true;
		mousebuttonState[LEFTBUTTON] = true;
	}
	if (button == GLUT_LEFT_BUTTON&&state == GLUT_UP)
	{
		mousebuttonupState[LEFTBUTTON] = true;
		mousebuttonState[LEFTBUTTON] = false;
	}
	if (button == GLUT_RIGHT_BUTTON&&state == GLUT_DOWN)
	{
		mousebuttondownState[RIGHTBUTTON] = true;
		mousebuttonState[RIGHTBUTTON] = true;
	}
	if (button == GLUT_RIGHT_BUTTON&&state == GLUT_UP)
	{
		mousebuttonupState[RIGHTBUTTON] = true;
		mousebuttonState[RIGHTBUTTON] = false;
	}
	if (button == GLUT_MIDDLE_BUTTON&&state == GLUT_DOWN)
	{
		mousebuttondownState[MIDDLEBUTTON] = true;
		mousebuttonState[MIDDLEBUTTON] = true;
	}
	if (button == GLUT_MIDDLE_BUTTON&&state == GLUT_UP)
	{
		mousebuttonupState[MIDDLEBUTTON] = true;
		mousebuttonState[MIDDLEBUTTON] = false;
	}
}

void InputSystem::OnKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		keydownState[KEY_A] = true;
		keyState[KEY_A] = true;
		break;
	case 'b':
		keydownState[KEY_B] = true;
		keyState[KEY_B] = true;
		break;
	case 'c':
		keydownState[KEY_C] = true;
		keyState[KEY_C] = true;
		break;
	case 'd':
		keydownState[KEY_D] = true;
		keyState[KEY_D] = true;
		break;
	case 'e':
		keydownState[KEY_E] = true;
		keyState[KEY_E] = true;
		break;
	case 'f':
		keydownState[KEY_F] = true;
		keyState[KEY_F] = true;
		break;
	case 'g':
		keydownState[KEY_G] = true;
		keyState[KEY_G] = true;
		break;
	case 'h':
		keydownState[KEY_H] = true;
		keyState[KEY_H] = true;
		break;
	case 'i':
		keydownState[KEY_I] = true;
		keyState[KEY_I] = true;
		break;
	case 'j':
		keydownState[KEY_J] = true;
		keyState[KEY_J] = true;
		break;
	case 'k':
		keydownState[KEY_K] = true;
		keyState[KEY_K] = true;
		break;
	case 'l':
		keydownState[KEY_L] = true;
		keyState[KEY_L] = true;
		break;
	case 'm':
		keydownState[KEY_M] = true;
		keyState[KEY_M] = true;
		break;
	case 'n':
		keydownState[KEY_N] = true;
		keyState[KEY_N] = true;
		break;
	case 'o':
		keydownState[KEY_O] = true;
		keyState[KEY_G] = true;
		break;
	case 'p':
		keydownState[KEY_P] = true;
		keyState[KEY_P] = true;
		break;
	case 'r':
		keydownState[KEY_R] = true;
		keyState[KEY_R] = true;
		break;
	case 's':
		keydownState[KEY_S] = true;
		keyState[KEY_S] = true;
		break;
	case 't':
		keydownState[KEY_T] = true;
		keyState[KEY_T] = true;
		break;
	case 'q':
		keydownState[KEY_Q] = true;
		keyState[KEY_Q] = true;
		break;
	case 'u':
		keydownState[KEY_U] = true;
		keyState[KEY_U] = true;
		break;
	case 'v':
		keydownState[KEY_V] = true;
		keyState[KEY_V] = true;
		break;
	case 'w':
		keydownState[KEY_W] = true;
		keyState[KEY_W] = true;
		break;
	case 'x':
		keydownState[KEY_X] = true;
		keyState[KEY_X] = true;
		break;
	case 'y':
		keydownState[KEY_Y] = true;
		keyState[KEY_Y] = true;
		break;
	case 'z':
		keydownState[KEY_Z] = true;
		keyState[KEY_Z] = true;
		break;
	case '1':
		keydownState[KEY_1] = true;
		keyState[KEY_1] = true;
		break;
	case '2':
		keydownState[KEY_2] = true;
		keyState[KEY_2] = true;
		break;
	case '3':
		keydownState[KEY_3] = true;
		keyState[KEY_3] = true;
		break;
	case '4':
		keydownState[KEY_4] = true;
		keyState[KEY_4] = true;
		break;
	case '5':
		keydownState[KEY_5] = true;
		keyState[KEY_5] = true;
		break;
	case '6':
		keydownState[KEY_6] = true;
		keyState[KEY_6] = true;
		break;
	case '7':
		keydownState[KEY_7] = true;
		keyState[KEY_7] = true;
		break;
	case '8':
		keydownState[KEY_8] = true;
		keyState[KEY_8] = true;
		break;
	case '9':
		keydownState[KEY_9] = true;
		keyState[KEY_9] = true;
		break;
	case '0':
		keydownState[KEY_0] = true;
		keyState[KEY_0] = true;
		break;
	case '+':
		keydownState[KEY_ADD] = true;
		keyState[KEY_ADD] = true;
		break;
	case '-':
		keydownState[KEY_SUB] = true;
		keyState[KEY_SUB] = true;
		break;
	case '~':
		keydownState[KEY_DUN] = true;
		keyState[KEY_DUN] = true;
		break;
	case ',':
		keydownState[KEY_LEFT_ANGLE_BRACKET] = true;
		keyState[KEY_LEFT_ANGLE_BRACKET] = true;
		break;
	case '.':
		keydownState[KEY_RIGHT_ANGLE_BRACKET] = true;
		keyState[KEY_RIGHT_ANGLE_BRACKET] = true;
		break;
	case '[':
		keydownState[KEY_LEFT_SQUARE_BRACKET] = true;
		keyState[KEY_LEFT_SQUARE_BRACKET] = true;
		break;
	case ']':
		keydownState[KEY_RIGHT_SQUARE_BRACKET] = true;
		keyState[KEY_RIGHT_SQUARE_BRACKET] = true;
		break;
	}
}

void InputSystem::OnSpecial(int key,int x,int y)
{
	switch (key)
	{
	case GLUT_KEY_ALT_L:
		keydownState[KEY_LEFT_ALT] = true;
		keyState[KEY_LEFT_ALT] = true;
		break;
	case GLUT_KEY_ALT_R:
		keydownState[KEY_RIGHT_ALT] = true;
		keyState[KEY_RIGHT_ALT] = true;
		break;
	case GLUT_KEY_CTRL_L:
		keydownState[KEY_LEFT_CONTROL] = true;
		keyState[KEY_LEFT_CONTROL] = true;
		break;
	case GLUT_KEY_CTRL_R:
		keydownState[KEY_RIGHT_CONTROL] = true;
		keyState[KEY_RIGHT_CONTROL] = true;
		break;
	case GLUT_KEY_DOWN:
		keydownState[KEY_DOWN] = true;
		keyState[KEY_DOWN] = true;
		break;
	case GLUT_KEY_F1:
		keydownState[KEY_F1] = true;
		keyState[KEY_F1] = true;
		break;
	case GLUT_KEY_F2:
		keydownState[KEY_F2] = true;
		keyState[KEY_F2] = true;
		break;
	case GLUT_KEY_F3:
		keydownState[KEY_F3] = true;
		keyState[KEY_F3] = true;
		break;
	case GLUT_KEY_F4:
		keydownState[KEY_F4] = true;
		keyState[KEY_F4] = true;
		break;
	case GLUT_KEY_F5:
		keydownState[KEY_F5] = true;
		keyState[KEY_F5] = true;
		break;
	case GLUT_KEY_F6:
		keydownState[KEY_F6] = true;
		keyState[KEY_F6] = true;
		break;
	case GLUT_KEY_F7:
		keydownState[KEY_F7] = true;
		keyState[KEY_F7] = true;
		break;
	case GLUT_KEY_F8:
		keydownState[KEY_F8] = true;
		keyState[KEY_F8] = true;
		break;
	case GLUT_KEY_F9:
		keydownState[KEY_F9] = true;
		keyState[KEY_F3] = true;
		break;
	case GLUT_KEY_F10:
		keydownState[KEY_F10] = true;
		keyState[KEY_F10] = true;
		break;
	case GLUT_KEY_F11:
		keydownState[KEY_F11] = true;
		keyState[KEY_F11] = true;
		break;
	case GLUT_KEY_F12:
		keydownState[KEY_F12] = true;
		keyState[KEY_F12] = true;
		break;
	case GLUT_KEY_LEFT:
		keydownState[KEY_DOWN] = true;
		keyState[KEY_DOWN] = true;
		break;
	case GLUT_KEY_RIGHT:
		keydownState[KEY_RIGHT] = true;
		keyState[KEY_RIGHT] = true;
		break;
	case GLUT_KEY_UP:
		keydownState[KEY_UP] = true;
		keyState[KEY_UP] = true;
		break;
	case GLUT_KEY_SHIFT_L:
		keydownState[KEY_LEFT_SHIFT] = true;
		keyState[KEY_LEFT_SHIFT] = true;
		break;
	case GLUT_KEY_SHIFT_R:
		keydownState[KEY_RIGHT_SHIFT] = true;
		keyState[KEY_RIGHT_SHIFT] = true;
		break;
	default:
		break;
	}
}

void InputSystem::OnSpecialUp(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_ALT_L:
		keyupState[KEY_LEFT_ALT] = true;
		keyState[KEY_LEFT_ALT] = false;
		break;
	case GLUT_KEY_ALT_R:
		keyupState[KEY_RIGHT_ALT] = true;
		keyState[KEY_RIGHT_ALT] = false;
		break;
	case GLUT_KEY_CTRL_L:
		keyupState[KEY_LEFT_CONTROL] = true;
		keyState[KEY_LEFT_CONTROL] = false;
		break;
	case GLUT_KEY_CTRL_R:
		keyupState[KEY_RIGHT_CONTROL] = true;
		keyState[KEY_RIGHT_CONTROL] = false;
		break;
	case GLUT_KEY_DOWN:
		keyupState[KEY_DOWN] = true;
		keyState[KEY_DOWN] = false;
		break;
	case GLUT_KEY_F1:
		keyupState[KEY_F1] = true;
		keyState[KEY_F1] = false;
		break;
	case GLUT_KEY_F2:
		keyupState[KEY_F2] = true;
		keyState[KEY_F2] = false;
		break;
	case GLUT_KEY_F3:
		keyupState[KEY_F3] = true;
		keyState[KEY_F3] = false;
		break;
	case GLUT_KEY_F4:
		keyupState[KEY_F4] = true;
		keyState[KEY_F4] = false;
		break;
	case GLUT_KEY_F5:
		keyupState[KEY_F5] = true;
		keyState[KEY_F5] = false;
		break;
	case GLUT_KEY_F6:
		keyupState[KEY_F6] = true;
		keyState[KEY_F6] = false;
		break;
	case GLUT_KEY_F7:
		keyupState[KEY_F7] = true;
		keyState[KEY_F7] = false;
		break;
	case GLUT_KEY_F8:
		keyupState[KEY_F8] = true;
		keyState[KEY_F8] = false;
		break;
	case GLUT_KEY_F9:
		keyupState[KEY_F9] = true;
		keyState[KEY_F3] = false;
		break;
	case GLUT_KEY_F10:
		keyupState[KEY_F10] = true;
		keyState[KEY_F10] = false;
		break;
	case GLUT_KEY_F11:
		keyupState[KEY_F11] = true;
		keyState[KEY_F11] = false;
		break;
	case GLUT_KEY_F12:
		keyupState[KEY_F12] = true;
		keyState[KEY_F12] = false;
		break;
	case GLUT_KEY_LEFT:
		keyupState[KEY_DOWN] = true;
		keyState[KEY_DOWN] = false;
		break;
	case GLUT_KEY_RIGHT:
		keyupState[KEY_RIGHT] = true;
		keyState[KEY_RIGHT] = false;
		break;
	case GLUT_KEY_UP:
		keyupState[KEY_UP] = true;
		keyState[KEY_UP] = false;
		break;
	case GLUT_KEY_SHIFT_L:
		keyupState[KEY_LEFT_SHIFT] = true;
		keyState[KEY_LEFT_SHIFT] = false;
		break;
	case GLUT_KEY_SHIFT_R:
		keyupState[KEY_RIGHT_SHIFT] = true;
		keyState[KEY_RIGHT_SHIFT] = false;
		break;
	default:
		break;
	}
}

void InputSystem::OnKeyboardUp(unsigned char key, int x, int y) 
{
	switch (key)
	{
	case 'a':
		keyupState[KEY_A] = true;
		keyState[KEY_A] = false;
		break;
	case 'b':
		keyupState[KEY_B] = true;
		keyState[KEY_B] = false;
		break;
	case 'c':
		keyupState[KEY_C] = true;
		keyState[KEY_C] = false;
		break;
	case 'd':
		keyupState[KEY_D] = true;
		keyState[KEY_D] = false;
		break;
	case 'e':
		keyupState[KEY_E] = true;
		keyState[KEY_E] = false;
		break;
	case 'f':
		keyupState[KEY_F] = true;
		keyState[KEY_F] = false;
		break;
	case 'g':
		keyupState[KEY_G] = true;
		keyState[KEY_G] = false;
		break;
	case 'h':
		keyupState[KEY_H] = true;
		keyState[KEY_H] = false;
		break;
	case 'i':
		keyupState[KEY_I] = true;
		keyState[KEY_I] = false;
		break;
	case 'j':
		keyupState[KEY_J] = true;
		keyState[KEY_J] = false;
		break;
	case 'k':
		keyupState[KEY_K] = true;
		keyState[KEY_K] = false;
		break;
	case 'l':
		keyupState[KEY_L] = true;
		keyState[KEY_L] = false;
		break;
	case 'm':
		keyupState[KEY_M] = true;
		keyState[KEY_M] = false;
		break;
	case 'n':
		keyupState[KEY_N] = true;
		keyState[KEY_N] = false;
		break;
	case 'o':
		keyupState[KEY_O] = true;
		keyState[KEY_G] = false;
		break;
	case 'p':
		keyupState[KEY_P] = true;
		keyState[KEY_P] = false;
		break;
	case 'r':
		keyupState[KEY_R] = true;
		keyState[KEY_R] = false;
		break;
	case 's':
		keyupState[KEY_S] = true;
		keyState[KEY_S] = false;
		break;
	case 't':
		keyupState[KEY_T] = true;
		keyState[KEY_T] = false;
		break;
	case 'q':
		keyupState[KEY_Q] = true;
		keyState[KEY_Q] = false;
		break;
	case 'u':
		keyupState[KEY_U] = true;
		keyState[KEY_U] = false;
		break;
	case 'v':
		keyupState[KEY_V] = true;
		keyState[KEY_V] = false;
		break;
	case 'w':
		keyupState[KEY_W] = true;
		keyState[KEY_W] = false;
		break;
	case 'x':
		keyupState[KEY_X] = true;
		keyState[KEY_X] = false;
		break;
	case 'y':
		keyupState[KEY_Y] = true;
		keyState[KEY_Y] = false;
		break;
	case 'z':
		keyupState[KEY_Z] = true;
		keyState[KEY_Z] = false;
		break;
	case '1':
		keyupState[KEY_1] = true;
		keyState[KEY_1] = false;
		break;
	case '2':
		keyupState[KEY_2] = true;
		keyState[KEY_2] = false;
		break;
	case '3':
		keyupState[KEY_3] = true;
		keyState[KEY_3] = false;
		break;
	case '4':
		keyupState[KEY_4] = true;
		keyState[KEY_4] = false;
		break;
	case '5':
		keyupState[KEY_5] = true;
		keyState[KEY_5] = false;
		break;
	case '6':
		keyupState[KEY_6] = true;
		keyState[KEY_6] = false;
		break;
	case '7':
		keyupState[KEY_7] = true;
		keyState[KEY_7] = false;
		break;
	case '8':
		keyupState[KEY_8] = true;
		keyState[KEY_8] = false;
		break;
	case '9':
		keyupState[KEY_9] = true;
		keyState[KEY_9] = false;
		break;
	case '0':
		keyupState[KEY_0] = true;
		keyState[KEY_0] = false;
		break;
	case '+':
		keyupState[KEY_ADD] = true;
		keyState[KEY_ADD] = false;
		break;
	case '-':
		keyupState[KEY_SUB] = true;
		keyState[KEY_SUB] = false;
		break;
	case '~':
		keyupState[KEY_DUN] = true;
		keyState[KEY_DUN] = false;
		break;
	case ',':
		keyupState[KEY_LEFT_ANGLE_BRACKET] = true;
		keyState[KEY_LEFT_ANGLE_BRACKET] = false;
		break;
	case '.':
		keyupState[KEY_RIGHT_ANGLE_BRACKET] = true;
		keyState[KEY_RIGHT_ANGLE_BRACKET] = false;
		break;
	case '[':
		keyupState[KEY_LEFT_SQUARE_BRACKET] = true;
		keyState[KEY_LEFT_SQUARE_BRACKET] = false;
		break;
	case ']':
		keyupState[KEY_RIGHT_SQUARE_BRACKET] = true;
		keyState[KEY_RIGHT_SQUARE_BRACKET] = false;
		break;
	}
}

