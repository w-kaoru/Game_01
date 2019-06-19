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

//ゲームクラス。
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
	Player* m_player = nullptr;				//プレイヤー
	Enemy* m_enemy = nullptr;				//エネミー
	EnemyBos* m_enemyBos = nullptr;			//エネミーボス
	GameCamera* m_gameCamera= nullptr;		//ゲームカメラ。
	LightCamera* m_light = nullptr;			//ライト
	Level m_level;							//レベルを初期化。
	std::vector< Enemy* > m_enemyList;		//エネミーのリスト。
	int m_enemyDeath = 0;					//エネミーの死んだ数。
	CSoundSource m_bgm;						//BGM。
	CSoundSource m_bgm_bos;					//BOSのBGM
	UI* m_ui = nullptr;
	int m_enemyNo = 0;
};

