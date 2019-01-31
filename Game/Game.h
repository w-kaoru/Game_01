#pragma once

#include "gameObject\IGameObject.h"
//#include "Player\Player.h"
#include "level/Level.h"
//#include "GameCamera.h"
#include "Enemy\Enemy.h"
#include "Render\RenderTarget.h"
#include "graphics\SkinModel.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//これらは前方宣言でよい！
struct PathPoint;
class Enemy;
class Player;
class GameCamera;
class LightCamera;

//ゲームクラス。
class Game : public IGameObject
{
public:
	Game();
	~Game(); 
	bool Start();
	void Update() override;
	void Draw() override;
	void Destroy();

	ShadowMap* GetShadowMap()
	{
		return &m_shadowMap;
	}
private:
	Player* m_player = nullptr;				//プレイヤー
	Enemy* m_enemy = nullptr;				//エネミー
	GameCamera* m_gameCamera= nullptr;		//ゲームカメラ。
	LightCamera* m_light = nullptr;			//ライト
	Level m_level;							//レベルを初期化。
	//Level* m_level = nullptr;				//レベルを初期化。
	std::vector< Enemy* > m_enemyList;		//エネミーのリスト。

	ShadowMap m_shadowMap;					//シャドウマップ。

	//SkinModelEffect *m_skinmodeleffect = nullptr;

	CSoundEngine m_soundEngine;				//サウンドエンジン。
	CSoundSource m_bgm;						//BGM。

};

//グローバルなアクセスポイントをグローバル変数として提供する。
//extern Game* g_game;


