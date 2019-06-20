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
	Player* m_player = nullptr;				//�v���C���[
	Enemy* m_enemy = nullptr;				//�G�l�~�[
	EnemyBos* m_enemyBos = nullptr;			//�G�l�~�[�{�X
	std::vector< Enemy* > m_enemyList;		//�G�l�~�[�̃��X�g�B
	int m_enemyNo = 0;
};

