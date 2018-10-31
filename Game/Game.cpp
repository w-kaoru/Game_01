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
	//m_player = g_gameObjM->NewGameObject<Player>();
	g_game = this;
	//レベルを初期化。
	//m_level.Init(L"Assets/level/stage_01.tkl", nullptr);
	//m_level.Init(L"Assets/level/stage_01.tkl", [&](LevelObjectData& objData) {
	//	if (objData.EqualName(L"plpath") == true) {
	//		m_player = g_gameObjM->NewGameObject<Player>();
	//		m_player->SetPosition(objData.position);
	//		return true;
	//	}
	//	else if (objData.EqualName(L"enpath") == true) {
	//		//エネミー！！！
	//		m_enemy = g_gameObjM->NewGameObject<Enemy>();
	//		m_enemy->SetPosition(objData.position);
	//		m_enemy->SetRotation(objData.rotation);
	//		m_enemy->GetPlayer(m_player);
	//		m_enemyList.push_back(m_enemy);
	//		return true;
	//	}
	//	return false;
	//});

	//レベルを初期化。
	m_level.Init(L"Assets/level/stage_02.tkl", nullptr);
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
	m_gameCamera.SetPlayer(m_player);
}


Game::~Game()
{
	g_gameObjM->DeleteGameObject(m_player);
	//動的に確保したインスタンスを破棄。
	for (auto& enemy : m_enemyList) {
		g_gameObjM->DeleteGameObject(enemy);
	}

}

void Game::Update()
{
	//プレイヤーの更新。

	m_gameCamera.Update();
	//m_player->Update();
	////Enemyを更新。
	//for (auto& enemy : m_enemyList) {
	//	enemy->Update();
	//	if (enemy->GetEnemydead() == true) {
	//		m_enemyList.erase(
	//			std::remove(m_enemyList.begin(), m_enemyList.end(), enemy),
	//			m_enemyList.end()
	//		);
	//	}
	//}
	if (g_pad[0].IsTrigger(enButtonB)) {
		g_gameObjM->DeleteGameObject(this);
		//タイトルシーンの作成。
		g_gameObjM->NewGameObject<Title>();
	}
}

void Game::Draw()
{
	//プレイヤーの描画。
	//m_player->Draw();
	//レベルを描画。
	m_level.Draw();
	//エネミーを描画。
	//for (auto& enemy : m_enemyList) {
	//	enemy->Draw();
	//}
}

void Game::Destroy()
{
}
