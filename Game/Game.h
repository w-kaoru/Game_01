#pragma once

#include "gameObject\IGameObject.h"
//#include "Player\Player.h"
#include "level/Level.h"
//#include "GameCamera.h"
#include "Enemy\Enemy.h"
#include "Render\RenderTarget.h"
#include "graphics\SkinModel.h"

//これらは前方宣言でよい！
struct PathPoint;
class Enemy;
class Player;
class GameCamera;

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
	Level m_level;							//レベルを初期化。
	//Level* m_level = nullptr;				//レベルを初期化。
	std::vector< Enemy* > m_enemyList;		//エネミーのリスト。

	ShadowMap m_shadowMap;					//シャドウマップ。
	SkinModelEffect *m_skinmodeleffect = nullptr;
};

//グローバルなアクセスポイントをグローバル変数として提供する。
//extern Game* g_game;


