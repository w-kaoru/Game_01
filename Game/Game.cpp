#include "stdafx.h"
#include "Game.h"
#include "Player\Player.h"
#include "level/Level.h"
#include "GameCamera.h"
#include "Title.h"

//グローバルなアクセスポイントをグローバル変数として提供する。
Game* g_game = nullptr;

Game::Game()
{
	//m_player = g_gameObjM->NewGameObject<Player>();

}

Game::~Game()
{
	g_gameObjM->DeleteGameObject(m_player);
	//動的に確保したインスタンスを破棄。
	for (auto& enemy : m_enemyList) {
		g_gameObjM->DeleteGameObject(enemy);
	}
	g_gameObjM->DeleteGameObject(m_gameCamera);
}

bool Game::Start()
{
	g_graphicsEngine->SetShadowMap(&m_shadowMap);
	//レベルを初期化。
	//m_level.Init(L"Assets/level/stage_02.tkl",&m_shadowMap, nullptr);
	m_level.Init(L"Assets/level/stage_02.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualName(L"plpath") == true) {
			m_player = g_gameObjM->NewGameObject<Player>();
			m_player->SetPosition(objData.position);
			return true;
		}
		else if (objData.EqualName(L"enpath") == true) {
			//エネミー！！！
			m_enemy = g_gameObjM->NewGameObject<Enemy>();
			m_enemy->SetEnemySelect(1);
			m_enemy->SetPosition(objData.position);
			m_enemy->SetRotation(objData.rotation);
			m_enemy->GetPlayer(m_player);
			m_enemyList.push_back(m_enemy);
			return true;
		}
		else if (objData.EqualName(L"enpath2") == true) {
			//エネミー！！！
			m_enemy = g_gameObjM->NewGameObject<Enemy>();
			m_enemy->SetEnemySelect(2);
			m_enemy->SetPosition(objData.position);
			m_enemy->SetRotation(objData.rotation);
			m_enemy->GetPlayer(m_player);
			m_enemyList.push_back(m_enemy);
			return true;
		}
		return false;
	});
	m_gameCamera=g_gameObjM->NewGameObject<GameCamera>(4);
	m_gameCamera->SetPlayer(m_player);
	g_game = this;
	return false;
}

void Game::Update()
{
	if (g_pad[0].IsTrigger(enButtonB)) {
		g_gameObjM->DeleteGameObject(this);
		//タイトルシーンの作成。
		g_gameObjM->NewGameObject<Title>();
	}
	//シャドウマップを更新。
	m_shadowMap.UpdateFromLightTarget(
		{ 1000.0f, 1000.0f, 1000.0f },
		{ 0.0f, 0.0f, 0.0f }
	);
}

void Game::Draw()
{
	m_level.Draw();
}

void Game::Destroy()
{
}
