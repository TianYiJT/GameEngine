#pragma once
#include<map>
#include<string>
using namespace std;
namespace CMGEComponent 
{
	typedef void* (*CreateComponent)();

	class ComponentFactory
	{
	private:
		map<string, CreateComponent> Map;
		ComponentFactory() {};
	public:
		void* getClassByName(string className)
		{
			map<string, CreateComponent>::const_iterator iter;
			iter = Map.find(className);
			if (iter == Map.end())
				return NULL;
			else
				return (iter->second)();
		}
		void registClass(string name, CreateComponent method)
		{
			pair<string, CreateComponent> temp(name, method);
			Map.insert(temp);
		}
		static ComponentFactory& getInstance()
		{
			static ComponentFactory slo_factory;
			return slo_factory;
		}
	};
}

