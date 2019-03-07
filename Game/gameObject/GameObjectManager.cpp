#include "stdafx.h"
#include "GameObjectManager.h"




GameObjectManager::~GameObjectManager()
{
}

//ŽÀsˆ—
void GameObjectManager::Execute()
{
	for (GameObjectList objList : m_gameObjectList) {
		for (IGameObject* obj : objList) {
			obj->StartWrapper();
		}
	}
	
	for (GameObjectList objList : m_gameObjectList) {
		for (IGameObject* obj:objList)	{
			obj->UpdateWrapper();
		}
	}

	for (GameObjectList objList : m_gameObjectList) {
		for (IGameObject* obj : objList) {
			obj->PreDrawWrapper();
		}
	}
	g_graphicsEngine->ShadowDraw();
	//g_graphicsEngine->ChangeMainRenderTarget();
	for (GameObjectList objList : m_gameObjectList) {
		for (IGameObject* obj : objList) {
			obj->DrawWrapper();
		}
	}
	g_graphicsEngine->PostEffectDraw();
	for (GameObjectList objList : m_gameObjectList) {
		for (IGameObject* obj : objList) {
			obj->PostDrawWrapper();
		}
	}
}