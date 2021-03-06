#pragma once
#include "level/Level.h"
#include "graphics\RenderTarget.h"
#include "graphics\SkinModel.h"


//これらは前方宣言でよい！
//class Background;
//class Dungeon;
class Enemy;
class EnemyBos;
class Player;
class GameCamera;
class LightCamera;
class UI;
class Sky;
//ゲームクラス。
class Stage : public IGameObject
{
public:
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
	void EnemySpawn();
private:
	//Background* m_background = nullptr;
	//Dungeon* m_dungeon = nullptr;
	Player* m_player = nullptr;				//プレイヤー
	EnemyBos* m_enemyBos = nullptr;			//エネミーボス
	GameCamera* m_gameCamera= nullptr;		//ゲームカメラ。
	LightCamera* m_light = nullptr;			//ライト
	Sky* m_sky = nullptr;					//空
	Level m_level;							//レベルを初期化。
	std::vector< Enemy* > m_enemyList;		//エネミーのリスト。
	UI* m_ui = nullptr;
	int m_enemyNo = 0;
	CSoundSource m_bgm;						//BGM。
	CSoundSource m_bgm_bos;					//BOSのBGM
	StageType m_type = StageType::Stage_Num;
	int m_middleEnemyNum = 0;
	int m_enemyMaxNum = 7;
	Save* m_save = nullptr;
};

