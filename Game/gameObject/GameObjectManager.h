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
	//���s
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
		// ���O���g�����C���X�^���X����
		//�i��������ꍇ�͍ŏ��ɐς񂾂��̂��Ԃ��Ă���̂Œ��ӁI�I�j
		//�i�����͏d�����̂Œ��ӁI�I�j
		// �����������I�u�W�F�N�g�ɂ������O
		// ���������Ō��������C���X�^���X�̃|�C���^
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
	//�Q�[���I�u�W�F�N�g�̃��X�g�B 
	//typedef std::list<IGameObject*>	GameObjectList;
	std::vector<std::list<IGameObject*>>	m_gameObjectList;
	std::list<IGameObject*>	m_deleteList[2];
	bool startDed = false;
	int m_deleteNo = 0;
};
extern GameObjectManager* g_gameObjM;
