#pragma once

#include "gameObject\IGameObject.h"
//#include "Player\Player.h"
#include "level/Level.h"
//#include "GameCamera.h"
#include "Enemy\Enemy.h"
#include "EnemyBos/EnemyBos.h"
#include "Render\RenderTarget.h"
#include "graphics\SkinModel.h"


//これらは前方宣言でよい！
struct PathPoint;
class Enemy;
class EnemyBos;
class Player;
class GameCamera;
class LightCamera;

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
	void PostDraw() override;
	void Destroy() override;

	void EnemyDeath()
	{
		m_enemyDeath++;
	}
	void SetGameEndFlag(bool flag)
	{
		m_endFlag = flag;
	}
private:
	Player* m_player = nullptr;				//プレイヤー
	Enemy* m_enemy = nullptr;				//エネミー
	EnemyBos* m_enemyBos = nullptr;			//エネミーボス
	GameCamera* m_gameCamera= nullptr;		//ゲームカメラ。
	LightCamera* m_light = nullptr;			//ライト
	Level m_level;							//レベルを初期化。
	//Level* m_level = nullptr;				//レベルを初期化。
	std::vector< Enemy* > m_enemyList;		//エネミーのリスト。
	int m_enemyDeath = 0;					//エネミーの死んだ数。
	CSoundSource m_bgm;						//BGM。
	CSoundSource m_bgm_bos;					//BOSのBGM
	bool m_endFlag = false;					//ゲージの終了。
};

//グローバルなアクセスポイントをグローバル変数として提供する。
//extern Game* g_game;
