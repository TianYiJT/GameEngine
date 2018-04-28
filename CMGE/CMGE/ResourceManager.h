#pragma once
#include<vector>
#include<string>
using namespace std;

template<class T>
struct dataNode
{
	T* data;
	string name;
};

template<class T>
class ResourceManager
{
public:
	static ResourceManager<T> *singleManager;

	static ResourceManager<T>* getInstance();

	void registerData(T * data_, string name);


	void deleteData(string name);


	T* getData(string name);

private:
	vector<dataNode<T>> allData;
};

template<class T> ResourceManager<T>* ResourceManager<T>::singleManager = new ResourceManager<T>();

template<class T>
ResourceManager<T>* ResourceManager<T>::getInstance()
{
	if (singleManager == NULL)
	{
		singleManager = new ResourceManager<T>();
	}
	return singleManager;
}

template<class T>
void ResourceManager<T>::registerData(T * data_, string name)
{
	dataNode<T> dn;
	dn.name = name;
	dn.data = data_;
	allData.push_back(dn);
}

template<class T>
void ResourceManager<T>::deleteData(string name)
{
	int index = -1;
	for (int i = 0; i < allData.size(); i++)
	{
		dataNode<T> temp = allData.at(i);
		if (temp.name == name)
		{
			index = i;
			break;
		}
	}
	if (index != -1)
	{
		dataNode<T> change;
		change = allData[index];
		allData[index] = allData[allData.size() - 1];
		allData.pop_back();
	}
}

template<class T>
T* ResourceManager<T>::getData(string name)
{
	int index = -1;
	for (int i = 0; i < allData.size(); i++)
	{
		dataNode<T> temp = allData.at(i);
		if (temp.name == name)
		{
			index = i;
			break;
		}
	}
	if (index != -1)
	{
		dataNode<T> des = allData.at(index);
		return des.data;
	}
	else
	{
		return NULL;
	}
}