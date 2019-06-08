#include "stdafx.h"
#include "GameObjectManager.h"




GameObjectManager::~GameObjectManager()
{
}

//���s����
void GameObjectManager::Execute()
{
	for (auto& objList : m_gameObjectList) {
		for (auto& obj : objList) {
			obj->StartWrapper();
		}
	}
	
	for (auto& objList : m_gameObjectList) {
		for (auto& obj:objList)	{
			obj->UpdateWrapper();
		}
	}

	for (auto& objList : m_gameObjectList) {
		for (auto& obj : objList) {
			obj->PreDrawWrapper();
		}
	}
	g_graphicsEngine->ShadowDraw();
	//g_graphicsEngine->ChangeMainRenderTarget();
	for (auto& objList : m_gameObjectList) {
		for (auto& obj : objList) {
			obj->DrawWrapper();
		}
	}
	for (auto& objList : m_gameObjectList) {
		for (auto& obj : objList) {
			obj->PostDrawWrapper();
		}
	}
	for (auto& objList : m_gameObjectList) {
		for (auto& obj : objList) {
			obj->PostPostDrawWrapper();
		}
	}
	g_graphicsEngine->EffectDraw();
	g_graphicsEngine->PostEffectDraw();
	DeleteExecute();
}

void GameObjectManager::DeleteExecute()
{
	// /*
	auto nowNo = m_deleteNo;
	m_deleteNo = ++m_deleteNo % 2;
	for (auto Deleteobject : m_deleteList[nowNo]) {
		int no = Deleteobject->GetPriority();
		if (!DeleteGameobject(Deleteobject))
		{
			//�N���b�V����������
			throw;
		}
	}
	m_deleteList[nowNo].clear();
}
bool GameObjectManager::DeleteGO(IGameObject * gameObject)
{
	for (auto objedct1 : m_gameObjectList)
	{
		for (auto objedct : objedct1) {
			if (objedct == gameObject)
			{
				m_deleteList[m_deleteNo].push_back(gameObject);
				objedct->Destroy();
				return true;
			}
		}
	}
	return false;
}
