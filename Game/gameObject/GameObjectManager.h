#pragma once
#include "IGameObject.h"
#include "../graphics/Sprite.h"

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
	T* NewGO(int i = 0, const char* name = "NULL")
	{
		T* newObj = new T;
		newObj->SetName(name);
		m_gameObjectList[i].push_back(newObj);
		return newObj;
	}

	bool DeleteGO(IGameObject* gameObject);

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
		// 名前を使ったインスタンス検索
		//（複数ある場合は最初に積んだものが返ってくるので注意！！）
		//（検索は重たいので注意！！）
		// 検索したいオブジェクトにつけた名前
		// 検索成功で検索したインスタンスのポインタ
	template<class T>
	T* FindGO(char* Name)
	{
		for (auto& list : m_gameObjectList) {
			for (auto& obj : list) {
				if (Name == obj->GetName()) {
					return (T*)obj;
				}
			}
		}
		return nullptr;
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
