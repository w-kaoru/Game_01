#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "level/Level.h"
#include "GameCamera.h"
#include "Title.h"

//グローバルなアクセスポイントをグローバル変数として提供する。
Game* g_game = nullptr;

Game::Game()
{
	m_player = g_gameObjM->NewGameObject<Player>();
	g_game = this;
	//レベルを初期化。
	//m_level.Init(L"Assets/level/stage_00.tkl", nullptr);
	//m_player.SetPosition({ 0.0f,0.0f,0.0f });
	m_level.Init(L"Assets/level/stage_00.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualName(L"Thethief_H") == true) {
			//Unityちゃん。
			m_player->SetPosition(objData.position);
			return true;
		}
		else if (objData.EqualName(L"enemy") == true) {
			//エネミー！！！
			//auto enemy = new Enemy(objData.position, objData.rotation, &m_player);
			auto m_enemy = g_gameObjM->NewGameObject<Enemy>();
			m_enemyList.push_back(m_enemy);
			return true;
		}
		return false;
	});
	m_gameCamera.SetPlayer(m_player);
	//return false;
}


Game::~Game()
{
	g_game = nullptr;
	//動的に確保したインスタンスを破棄。
	for (auto& enemy : m_enemyList) {
		delete enemy;
	}
}

void Game::Update()
{
	//プレイヤーの更新。
	
	m_player->Update();
	//Enemyを更新。
	for (auto& enemy : m_enemyList) {
		enemy->Update();
		if (enemy->GetEnemydead() == true) {
			m_enemyList.erase(
				std::remove(m_enemyList.begin(), m_enemyList.end(), enemy),
				m_enemyList.end()
			);
		}
	}
	m_gameCamera.Update();
	if (g_pad[0].IsTrigger(enButtonB)) {
		delete g_game;
		//タイトルシーンの作成。
		g_gameObjM->NewGameObject<Title>();
	}
	
}

void Game::Draw()
{
	//プレイヤーの描画。
	m_player->Draw();
	//レベルを描画。
	m_level.Draw();
	//エネミーを描画。
	for (auto& enemy : m_enemyList) {
		enemy->Draw();
	}
}
