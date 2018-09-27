#pragma once
#include "IGameObject.h"


class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();
private:
	typedef std::list<IGameObject*>	GameObjectList;
	std::vector<GameObjectList>	m_gameObjectListArray;					//!<�Q�[���I�u�W�F�N�g�̗D��x�t�����X�g�B
	std::vector<GameObjectList>	m_deleteObjectArray[2];					//!<�폜����I�u�W�F�N�g�̃��X�g�B�폜�������s���Ă���Œ���DeleteGameObject���Ă΂��\���������̂ŁA�_�u���o�b�t�@���B
	int m_currentDeleteObjectBufferNo = 0;								//!<���݂̍폜�I�u�W�F�N�g�̃o�b�t�@�ԍ��B
	static const unsigned char 			GAME_OBJECT_PRIO_MAX = 255;		//!<�Q�[���I�u�W�F�N�g�̗D��x�̍ő�l�B
};

