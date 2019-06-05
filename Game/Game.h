#pragma once

#include "gameObject\IGameObject.h"
#include "level/Level.h"
#include "Enemy\Enemy.h"
#include "EnemyBos/EnemyBos.h"
#include "graphics\RenderTarget.h"
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
	void PostPostDraw() override;
	void Destroy() override;

	void SpriteDraw();
	void EnemyDeath()
	{
		m_enemyDeath++;
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

	Sprite m_ui[4];		//UI
	float m_spriteScaleX = 100.0f;	//HPのスプライトの横幅
	float m_spriteScaleY = 100.0f;	//HPのスプライトの縦幅
	CQuaternion m_spriteRot = CQuaternion::Identity();
};

//グローバルなアクセスポイントをグローバル変数として提供する。
//extern Game* g_game;
