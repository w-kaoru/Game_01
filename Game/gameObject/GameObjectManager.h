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
	//���s
	void Execute();
	/*!
	 *@brief	�C���X�^���X�̎擾�B
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
	std::vector<GameObjectList>	m_gameObjectList;					//!<�Q�[���I�u�W�F�N�g�̗D��x�t�����X�g�B
};
extern GameObjectManager* g_gameObjM;

//static inline GameObjectManager& gameObjectManager()
//{
//	return GameObjectManager::Instance();
//}	
//
///*!
//*@brief	�Q�[���I�u�W�F�N�g�����̃w���p�[�֐��B
//*/
//template<class T>
//static inline T* NewGo()
//{
//	gameObjectManager().NewGameObject<T>();
//}
///*!
//*@brief	�폜����Q�[���I�u�W�F�N�g�B
//*/
//static inline void DeleteGo(IGameObject* go)
//{
//	gameObjectManager().DeleteGameObject(go);
//}