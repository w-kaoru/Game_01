#pragma once
#include "IGameObject.h"


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
	template<class T>
	T* NewGameObject(int i = 0)
	{
		T* newObj = new T;
		m_gameObjectList[i].push_back(newObj);
		return newObj;
	}

	void DeleteGameObject(IGameObject* gameObject)
	{
		if (gameObject != nullptr) {
			gameObject->Destroy();
			for (int i = 0; i < 5; i++) {
				m_gameObjectList[i].erase(
					std::remove(m_gameObjectList[i].begin(), m_gameObjectList[i].end(), gameObject),
					m_gameObjectList[i].end()
				);
			}
		}
		delete gameObject;

	}
private:
	typedef std::list<IGameObject*>	GameObjectList;
	std::vector<GameObjectList>	m_gameObjectList;					//ゲームオブジェクトのリスト。
};
extern GameObjectManager* g_gameObjM;
