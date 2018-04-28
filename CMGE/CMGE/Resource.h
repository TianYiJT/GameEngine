#pragma once
#include<string>
#include"CMGEStringEngine.h"
using namespace CMGEStringEngine;
using namespace std;

class Resource
{
public:
	virtual void Load(string path)
	{
		if (!Stringf::checkSuffix(path, mformat)) 
		{
			return;
		}
	}
	virtual void Load(string path,string name_,string mformat_)
	{
		name = name_;
		mformat = mformat_;
		Load(path);
		Init();
	}
	void setFormat(string s) { mformat = s; };
	void setName(string s) { name = s; }
	virtual void Init() {}
	static void GlobalStartUp() {}
	string name;
	string mformat;
private:
};