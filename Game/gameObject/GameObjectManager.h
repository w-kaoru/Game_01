#pragma once
#include "IGameObject.h"


class GameObjectManager
{
public:
	GameObjectManager()
	{
		m_gameObjectList.resize(1);
	}
	~GameObjectManager();
	//実行
	void Execute();
	/*!
	 *@brief	インスタンスの取得。
	 */
	static GameObjectManager& Instance()
	{
		static GameObjectManager instance;
		return instance;
	}
	template<class T>
	T* NewGameObject()
	{
		T* newObj = new T;
		m_gameObjectList[0].push_back(newObj);
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
extern GameObjectManager* g_gameObjM;

//static inline GameObjectManager& gameObjectManager()
//{
//	return GameObjectManager::Instance();
//}	
//
///*!
//*@brief	ゲームオブジェクト生成のヘルパー関数。
//*/
//template<class T>
//static inline T* NewGo()
//{
//	gameObjectManager().NewGameObject<T>();
//}
///*!
//*@brief	削除するゲームオブジェクト。
//*/
//static inline void DeleteGo(IGameObject* go)
//{
//	gameObjectManager().DeleteGameObject(go);
//}