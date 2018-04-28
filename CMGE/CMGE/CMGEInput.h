#pragma once
#include"CMGEMathEngine.h"
#include <windows.h> 
#include<vector>
#include<string>
#include<map>
#include"freeglut.h"


using namespace CMGEMathEngine;
using namespace std;
namespace CMGEInput
{

	enum KeyBoard:unsigned int
	{
		 KEY_Q = 0,
		 KEY_W,KEY_E,KEY_R,KEY_T,KEY_Y,KEY_U,KEY_I,KEY_O,KEY_P,KEY_A,KEY_S,KEY_D,KEY_F,KEY_G,KEY_H,KEY_J,
		 KEY_K,KEY_L,KEY_Z,KEY_X,KEY_C,KEY_V,KEY_B,KEY_N,KEY_M,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,
		 KEY_8,KEY_9,KEY_0,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,
		 KEY_F12,KEY_TAB,KEY_BACK,KEY_LEFT_SHIFT,KEY_LEFT_CONTROL,KEY_LEFT_ALT,KEY_SPACE,KEY_ENTER,KEY_RIGHT_SHIFT,
		 KEY_RIGHT_CONTROL,KEY_RIGHT_ALT,KEY_UP,KEY_DOWN,KEY_LEFT,KEY_RIGHT,KEY_ESC,KEY_SUB,KEY_ADD,KEY_CAPSLK,
		 KEY_LEFT_ANGLE_BRACKET,KEY_RIGHT_ANGLE_BRACKET,KEY_LEFT_SQUARE_BRACKET,KEY_RIGHT_SQUARE_BRACKET,KEY_DUN,KEYBOARDSIZE
	};

	enum  MouseButton:unsigned int
	{
		 LEFTBUTTON = 0,
		 MIDDLEBUTTON,
		 RIGHTBUTTON,
		 MOUSETYPESIZE
	};

	class InputSystem 
	{
	public:
		static void OnMouse(int button, int state, int x, int y);
		static void OnKeyboard(unsigned char key, int x, int y);
		static void OnKeyboardUp(unsigned char key, int x, int y);
		static void OnSpecial(int key, int x, int y);
		static void OnSpecialUp(int key, int x, int y);
		static void OnMotion(int x,int y);
		static Vector2 GetMousePosition();
		static bool GetMouseButton(MouseButton mb);
		static bool GetMouseButtonDown(MouseButton mb);
		static bool GetMouseButtonUp(MouseButton mb);
		static Vector2 GetMouseDeltaVelocity();
		static bool GetKey(KeyBoard kb);
		static bool GetKeyDown(KeyBoard kb);
		static bool GetKeyUp(KeyBoard kb);
		static float GetdeltaTime();
		static void Update();
		static bool GetButtonDown(string name);
		static float GetMouseTime(MouseButton kb);
		static float GetKeyTime(KeyBoard kb);
		static void registerButton(vector<KeyBoard> kbList,string name);
		static void deleteButton(string name);
	private:
		static float TransferTime(SYSTEMTIME  st);
		static Vector2 mousePosition;
		static Vector2 LastMousePosition;
		static Vector2 mouseVelocity;
		static map<string, vector<KeyBoard>> buttonList;
		static map<string, bool> buttonListState;
		static double deltaTime;
		static bool* mousebuttonState;
		static bool* mousebuttonupState;
		static bool* mousebuttondownState;
		static bool* keyState;
		static bool* keyupState;
		static bool* keydownState;
		static float* keyTime;
		static float* MouseTime;
		static double LastTime;
		static double NowTime;
		static double StartTime;
		static float bufferTime;
		static vector<KeyBoard> KeyBoardBuffer;
		static float timer_keybuffer;
	};
}