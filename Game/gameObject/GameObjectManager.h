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
	//���s
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
	std::vector<GameObjectList>	m_gameObjectList;					//�Q�[���I�u�W�F�N�g�̃��X�g�B


	D3D11_VIEWPORT m_frameBufferViewports;			//�t���[���o�b�t�@�̃r���[�|�[�g�B
	ID3D11RenderTargetView* m_frameBufferRenderTargetView = nullptr;	//�t���[���o�b�t�@�̃����_�����O�^�[�Q�b�g�r���[�B
	ID3D11DepthStencilView* m_frameBufferDepthStencilView = nullptr;	//�t���[���o�b�t�@�̃f�v�X�X�e���V���r���[�B
};
extern GameObjectManager* g_gameObjM;
