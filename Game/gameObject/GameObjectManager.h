#pragma once
#include "IGameObject.h"


class GameObjectManager
{
public:
	GameObjectManager()
	{
		m_gameObjectList.resize(11);
	}
	~GameObjectManager();
	//実行
	void Execute();
	template<class T>
	T* NewGameObject(int i=0)
	{
		T* newObj = new T;
		m_gameObjectList[i].push_back(newObj);
		return newObj;
	}

	void DeleteGameObject(IGameObject* gameObject)
	{
		if (gameObject != nullptr) {
			gameObject->Destroy();
			m_gameObjectList[0].erase(
				std::remove(m_gameObjectList[0].begin(), m_gameObjectList[0].end(), gameObject),
				m_gameObjectList[0].end()
			);
		}
		delete gameObject;

	}
private:
	typedef std::list<IGameObject*>	GameObjectList;
	std::vector<GameObjectList>	m_gameObjectList;					//ゲームオブジェクトのリスト。
};
extern GameObjectManager* g_gameObjM;
