#pragma once

#include "gameObject\IGameObject.h"
#include "Player\Player.h"
#include "level/Level.h"
#include "GameCamera.h"
#include "Enemy\Enemy.h"

//これらは前方宣言でよい！
struct PathPoint;
class Enemy;

//ゲームクラス。
class Game : public IGameObject
{
public:
	Game();
	~Game();
	//bool Start();
	void Update() override;
	void Draw() override;
	void Destroy();
private:
	Player* m_player = nullptr;				//プレイヤー
	Enemy* m_enemy = nullptr;				//エネミー
	GameCamera* m_gameCamera= nullptr;		//ゲームカメラ。
	Level m_level;							//レベルを初期化。
	//Level* m_level = nullptr;				//レベルを初期化。
	std::vector< Enemy* > m_enemyList;		//エネミーのリスト。
};

//グローバルなアクセスポイントをグローバル変数として提供する。
extern Game* g_game;


