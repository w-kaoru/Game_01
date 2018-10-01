#pragma once
#include "IGameObject.h"


class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();
	template<class T>
	T* NewGameObject()
	{
		T* newObj = new T;
		m_gameObjectList.push_back(newObj);
		return newObj;
	}

	void DeleteGameObject(IGameObject* gameObject)
	{
		if (gameObject != nullptr) {
			gameObject->Destroy();
			for (auto obj : m_gameObjectList) {
				m_gameObjectList.erase(
					std::remove(m_gameObjectList.begin(),m_gameObjectList.end(),obj),
					m_gameObjectList.end()
				);
			}
			delete gameObject;
		}
	}
private:
	typedef std::list<IGameObject*>	GameObjectList;
	std::vector<GameObjectList>	m_gameObjectList;					//!<ゲームオブジェクトの優先度付きリスト。
};

