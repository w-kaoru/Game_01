#pragma once
#include "level/Level.h"
#include "graphics\RenderTarget.h"
#include "graphics\SkinModel.h"


//�����͑O���錾�ł悢�I
//class Background;
//class Dungeon;
class Enemy;
class EnemyBos;
class Player;
class GameCamera;
class LightCamera;
class UI;
class WarpPoint;
class Sky;
//�Q�[���N���X�B
class Stage : public IGameObject
{
public:
	enum StageType {
		Stage_Dungeon,
		Stage_Ground,
		Stage_Num
	};
	Stage();
	~Stage();
	bool Start() override;
	void Update() override;
	void Draw() override;
	void Destroy() override;
	void Release();
	void StageSelect(StageType type);
	StageType GetStageType()
	{
		return m_type;
	}
	bool EnemyDelete(Enemy* ememy);
	void DungeonNew();
	void GroundNew();
private:
	//Background* m_background = nullptr;
	//Dungeon* m_dungeon = nullptr;
	Player* m_player = nullptr;				//�v���C���[
	EnemyBos* m_enemyBos = nullptr;			//�G�l�~�[�{�X
	GameCamera* m_gameCamera= nullptr;		//�Q�[���J�����B
	LightCamera* m_light = nullptr;			//���C�g
	WarpPoint* m_warpPoint = nullptr;
	Sky* m_sky = nullptr;					//��
	Level m_level;							//���x�����������B
	std::vector< Enemy* > m_enemyList;		//�G�l�~�[�̃��X�g�B
	UI* m_ui = nullptr;
	int m_enemyNo = 0;
	CSoundSource m_bgm;						//BGM�B
	CSoundSource m_bgm_bos;					//BOS��BGM
	StageType m_type = StageType::Stage_Num;
	int m_middleEnemyNum = 0;
	int m_enemyMaxNum = 7;
	int m_enemyMaxLv = 40;
};

