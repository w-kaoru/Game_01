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
			obj->Update();
		}
	}
	for (GameObjectList objList : m_gameObjectList) {
		for (IGameObject* obj : objList) {
			obj->Draw();
		}
	}
}