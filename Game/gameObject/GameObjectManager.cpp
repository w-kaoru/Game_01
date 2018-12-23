#include "stdafx.h"
#include "GameObjectManager.h"




GameObjectManager::~GameObjectManager()
{
}

//���s����
void GameObjectManager::Execute()
{
	for (GameObjectList objList : m_gameObjectList) {
		for (IGameObject* obj : objList) {
			obj->StartDed();
		}
	}
	
	for (GameObjectList objList : m_gameObjectList) {
		for (IGameObject* obj:objList)	{
			obj->UpdateStart();
		}
	}

	if (g_graphicsEngine->GetShadowMap() != nullptr) {
		g_graphicsEngine->GetShadowMap()->ShadowMapDraw();
	}
	for (GameObjectList objList : m_gameObjectList) {
		for (IGameObject* obj : objList) {
			obj->DrawStart();
		}
	}
}