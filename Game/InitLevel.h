#pragma once
#include "level/Level.h"
class Enemy;
class EnemyBos;
class Player;
class Background;
class InitLevel:public IGameObject
{
public:
	InitLevel();
	~InitLevel();
	bool Start() override;
	void Update() override;
	void Destroy() override;
private:
	Level m_level;
	Background* m_background = nullptr;
	Player* m_player = nullptr;				//プレイヤー
	Enemy* m_enemy = nullptr;				//エネミー
	EnemyBos* m_enemyBos = nullptr;			//エネミーボス
	std::vector< Enemy* > m_enemyList;		//エネミーのリスト。
	int m_enemyNo = 0;
};

