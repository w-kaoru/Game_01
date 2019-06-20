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
class Dungeon;
//�Q�[���N���X�B
class Stage : public IGameObject
{
public:
	enum StageType
	{
		type_Dungeon,
		type_Ground,
		type_Num
	};
	Stage();
	~Stage();
	bool Start() override;
	void Update() override;
	void Draw() override;
	void Destroy() override;
	void StageSelect(StageType st);
	bool EnemyDelete(Enemy* ememy);
private:
	Background* m_background = nullptr;
	Dungeon* m_dungeon = nullptr;
	Player* m_player = nullptr;				//�v���C���[
	EnemyBos* m_enemyBos = nullptr;			//�G�l�~�[�{�X
	GameCamera* m_gameCamera= nullptr;		//�Q�[���J�����B
	LightCamera* m_light = nullptr;			//���C�g
	Level m_level;							//���x�����������B
	std::vector< Enemy* > m_enemyList;		//�G�l�~�[�̃��X�g�B
	StageType m_type = StageType::type_Dungeon;
	int m_enemyNo = 0;
	CSoundSource m_bgm;						//BGM�B
	CSoundSource m_bgm_bos;					//BOS��BGM
};

