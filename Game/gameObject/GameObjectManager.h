#pragma once
#include "IGameObject.h"
#include "../Render/Sprite.h"

class GameObjectManager
{
public:
	GameObjectManager()
	{
		m_gameObjectList.resize(5);
	}
	~GameObjectManager();
	//実行
	void Execute();
	void DeleteExecute();

	template<class T>
	T* NewGameObject(int i = 0)
	{
		T* newObj = new T;
		m_gameObjectList[i].push_back(newObj);
		return newObj;
	}

	bool DeleteGameObject(IGameObject* gameObject);

	bool DeleteGameobject(IGameObject* gameObject)
	{
		//*
		if (gameObject != nullptr) {
			for (int i = 0; i < m_gameObjectList.size(); i++)
			{
				m_gameObjectList[i].erase(
					std::remove(m_gameObjectList[i].begin(), m_gameObjectList[i].end(), gameObject),
					m_gameObjectList[i].end()
				);
				
			}	
			delete gameObject;
			return true;
			//m_gameObjectList.clear();
		}//*/
		return false;
	}
private:
	//ゲームオブジェクトのリスト。 
	//typedef std::list<IGameObject*>	GameObjectList;
	std::vector<std::list<IGameObject*>>	m_gameObjectList;
	std::list<IGameObject*>	m_deleteList[2];
	bool startDed = false;
	int m_deleteNo = 0;
};
extern GameObjectManager* g_gameObjM;
