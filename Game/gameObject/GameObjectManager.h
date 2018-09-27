#pragma once
#include "IGameObject.h"


class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();
private:
	typedef std::list<IGameObject*>	GameObjectList;
	std::vector<GameObjectList>	m_gameObjectListArray;					//!<ゲームオブジェクトの優先度付きリスト。
	std::vector<GameObjectList>	m_deleteObjectArray[2];					//!<削除するオブジェクトのリスト。削除処理を行っている最中にDeleteGameObjectが呼ばれる可能性が高いので、ダブルバッファ化。
	int m_currentDeleteObjectBufferNo = 0;								//!<現在の削除オブジェクトのバッファ番号。
	static const unsigned char 			GAME_OBJECT_PRIO_MAX = 255;		//!<ゲームオブジェクトの優先度の最大値。
};

