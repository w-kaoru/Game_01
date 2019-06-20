#pragma once
#include "level/Level.h"
#include "graphics\RenderTarget.h"
#include "graphics\SkinModel.h"


//これらは前方宣言でよい！
class Enemy;
class EnemyBos;
class Player;
class GameCamera;
class LightCamera;
class UI;
class Background;
class Dungeon;
//ゲームクラス。
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
	Player* m_player = nullptr;				//プレイヤー
	EnemyBos* m_enemyBos = nullptr;			//エネミーボス
	GameCamera* m_gameCamera= nullptr;		//ゲームカメラ。
	LightCamera* m_light = nullptr;			//ライト
	Level m_level;							//レベルを初期化。
	std::vector< Enemy* > m_enemyList;		//エネミーのリスト。
	StageType m_type = StageType::type_Dungeon;
	int m_enemyNo = 0;
	CSoundSource m_bgm;						//BGM。
	CSoundSource m_bgm_bos;					//BOSのBGM
};

