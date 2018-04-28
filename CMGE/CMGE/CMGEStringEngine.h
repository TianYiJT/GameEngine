#pragma once
#include<string>
#include<vector>
#include<iostream>
using namespace std;
namespace CMGEStringEngine
{
	class Stringf
	{
	public:
		/*
		@param string s : 被分解成词的句子
		demo: "abc bca 123"->"abc"/"bca"/"123"
		*/
		static vector<string> Participle(string s) 
		{
			string temp = "";
			vector<string> des;
			bool starting = false;
			for (int i = 0; i < s.size(); i++) 
			{
				if (starting) 
				{
					if(s[i]==' '||s[i]=='\n')
					{
						string temp1 = temp;
						des.push_back(temp1);
						temp = "";
						starting = false;
					}
					else
					{
						temp += s[i];
					}
					if (i == s.size() - 1)
					{
						string temp1 = temp;
						des.push_back(temp1);
						temp = "";
						starting = false;
					}
				}
				else
				{
					if (s[i] != ' '&&s[i] != '\n') 
					{
						starting = true;
						temp += s[i];
						if (i == s.size() - 1)
						{
							string temp1 = temp;
							des.push_back(temp1);
							temp = "";
							starting = false;
						}
					}
				}
			}
			return des;
		}
		/*
		@param s 检测目标
		@param suffix 后缀
		检测字符串具不具有这样的后缀
		s = "abc.txt" suffix="txt" -> true
		s = "abc.fbx" suffix="txt" -> false
		*/
		static bool checkSuffix(string s,string suffix) 
		{
			string temp = "";
			for (int i = s.length()-1; i >= 0; i--) 
			{
				if(s[i]!='.')
				{
					temp += s[i];
				}
				else
				{
					break;
				}
			}
			string inverse_t = inverse(temp);
			if (inverse_t == suffix)
				return true;
			else
				return false;

		}
		/*
		@ param s 目标
		字符串求反
		"demo" -> "omed"
		*/
		static string inverse(string s) 
		{
			string temp = "";
			for (int i = 0; i < s.length(); i++) 
			{
				temp += s[s.length()-1-i];
			}
			return temp;
		}
	};
};