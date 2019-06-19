#pragma once
#include "level/Level.h"
#include "graphics\RenderTarget.h"
#include "graphics\SkinModel.h"


//�����͑O���錾�ł悢�I
class Enemy;
class EnemyBos;
class Player;
class GameCamera;
class LightCamera;
class UI;
class Background;

//�Q�[���N���X�B
class Game : public IGameObject
{
public:
	Game();
	~Game(); 
	bool Start() override;
	void Update() override;
	void Draw() override;
	void PreDraw() override;
	void Destroy() override;

	void EnemyDeath()
	{
		m_enemyDeath++;
	}
private:
	Background* m_background = nullptr;
	Player* m_player = nullptr;				//�v���C���[
	Enemy* m_enemy = nullptr;				//�G�l�~�[
	EnemyBos* m_enemyBos = nullptr;			//�G�l�~�[�{�X
	GameCamera* m_gameCamera= nullptr;		//�Q�[���J�����B
	LightCamera* m_light = nullptr;			//���C�g
	Level m_level;							//���x�����������B
	std::vector< Enemy* > m_enemyList;		//�G�l�~�[�̃��X�g�B
	int m_enemyDeath = 0;					//�G�l�~�[�̎��񂾐��B
	CSoundSource m_bgm;						//BGM�B
	CSoundSource m_bgm_bos;					//BOS��BGM
	UI* m_ui = nullptr;
	int m_enemyNo = 0;
};

