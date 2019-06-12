#include "stdafx.h"
#include "Game.h"
#include "Player\Player.h"
#include "level/Level.h"
#include "GameCamera.h"
#include "Title.h"
#include "GameEnd.h"
#include "LightCamera.h"
#include "UI.h"


Game::Game()
{
}

Game::~Game()
{
}

void Game::Destroy()
{
	g_gameObjM->DeleteGO(m_ui);
	g_gameObjM->DeleteGO(m_gameCamera);
	g_gameObjM->DeleteGO(m_light);
	g_gameObjM->DeleteGO(m_player);
	//動的に確保したインスタンスを破棄。
	for (auto& enemy : m_enemyList) {
		g_gameObjM->DeleteGO(enemy);
	}
	if (m_enemyBos) {
		g_gameObjM->DeleteGO(m_enemyBos);
	}
}

bool Game::Start()
{
	//レベルを初期化。
	m_level.Init(
		L"Assets/level/Dungeon_01.tkl",
		[&](LevelObjectData& objData) {
		return false;
	});
	//レベルを初期化。
	m_level.Init(
		L"Assets/level/player_01.tkl",
		[&](LevelObjectData& objData) {
		if (objData.EqualName(L"plpath") == true) {
			m_player = g_gameObjM->NewGO<Player>(0,"Player");
			m_player->SetPosition(objData.position);
			//ステータスの設定
			m_player->GetStatus()->SetLv(1);
			m_player->GetStatus()->SetStandardHp(60);
			m_player->GetStatus()->SetStandardAgi(1150.0f);
			m_player->GetStatus()->SetStandardDef(1.0f);
			m_player->GetStatus()->SetStandardAtk(12.5f);
			m_player->GetStatus()->SetMaxLv(9);
			m_player->GetStatus()->StatusUp();
			return true;
		}
		return false;
	});
	m_level.Init(
		L"Assets/level/enemy_01.tkl",
		[&](LevelObjectData& objData) {
		if (objData.EqualName(L"enpath") == true) {
			//エネミー！！！
			m_enemy = g_gameObjM->NewGO<Enemy>(0,"Enemy00");
			m_enemy->SetEnemyType(Enemy::EnemyType::type_skeleton);
			m_enemy->SetPosition(objData.position);
			m_enemy->SetRotation(objData.rotation);
			m_enemy->SetPlayer(m_player);
			m_enemyList.push_back(m_enemy);
			//エネミーのレベル
			m_enemy->GetStatus()->SetLv(3);
			m_enemy->GetStatus()->SetStandardHp(30.0f);
			m_enemy->GetStatus()->SetStandardAgi(550.0f);
			m_enemy->GetStatus()->SetStandardDef(1.5f);
			m_enemy->GetStatus()->SetStandardAtk(3.0f);
			m_enemy->GetStatus()->StatusUp();
			return true;
		}
		if (objData.EqualName(L"enpath2") == true) {
			//エネミー！！！
			m_enemy = g_gameObjM->NewGO<Enemy>(0,"Enemy01");
			m_enemy->SetEnemyType(Enemy::EnemyType::type_troll);
			m_enemy->SetPosition(objData.position);
			m_enemy->SetRotation(objData.rotation);
			m_enemy->SetPlayer(m_player);
			m_enemyList.push_back(m_enemy);
			//エネミーのレベル
			m_enemy->GetStatus()->SetLv(3); 
			m_enemy->GetStatus()->SetStandardHp(35.0f);
			m_enemy->GetStatus()->SetStandardAgi(550.0f);
			m_enemy->GetStatus()->SetStandardDef(1.0f);
			m_enemy->GetStatus()->SetStandardAtk(3.5f);
			m_enemy->GetStatus()->StatusUp();
			return true;
		}
		return false;
	});
	m_level.Init(
		L"Assets/level/enemy_02.tkl",
		[&](LevelObjectData& objData) {
		if (objData.EqualName(L"enpath") == true) {
			//エネミー！！！
			m_enemy = g_gameObjM->NewGO<Enemy>(0,"Enemy02");
			m_enemy->SetEnemyType(Enemy::EnemyType::type_skeleton);
			m_enemy->SetPosition(objData.position);
			m_enemy->SetRotation(objData.rotation);
			m_enemy->SetPlayer(m_player);
			m_enemyList.push_back(m_enemy);
			//エネミーのレベル
			m_enemy->GetStatus()->SetLv(7);
			m_enemy->GetStatus()->SetStandardHp(30.0f);
			m_enemy->GetStatus()->SetStandardAgi(550.0f);
			m_enemy->GetStatus()->SetStandardDef(1.5f);
			m_enemy->GetStatus()->SetStandardAtk(3.0f);
			m_enemy->GetStatus()->StatusUp();
			return true;
		}
		if (objData.EqualName(L"enpath2") == true) {
			//エネミー！！！
			m_enemy = g_gameObjM->NewGO<Enemy>(0,"Enemy03");
			m_enemy->SetEnemyType(Enemy::EnemyType::type_troll);
			m_enemy->SetPosition(objData.position);
			m_enemy->SetRotation(objData.rotation);
			m_enemy->SetPlayer(m_player);
			m_enemyList.push_back(m_enemy);
			//エネミーのレベル
			m_enemy->GetStatus()->SetLv(6);
			m_enemy->GetStatus()->SetStandardHp(35.0f);
			m_enemy->GetStatus()->SetStandardAgi(550.0f);
			m_enemy->GetStatus()->SetStandardDef(1.0f);
			m_enemy->GetStatus()->SetStandardAtk(3.5f);
			m_enemy->GetStatus()->StatusUp();
			return true;
		}
		return false;
	});
	
	m_level.Init(
			L"Assets/level/enemyBos.tkl",
			[&](LevelObjectData& objData) {
			if (objData.EqualName(L"enpath") == true) {
				//エネミー！！！
				m_enemyBos = g_gameObjM->NewGO<EnemyBos>(0, "EnemyBos");
				m_enemyBos->SetPosition(objData.position);
				m_enemyBos->SetRotation(objData.rotation);
				m_enemyBos->GetPlayer(m_player);
				m_enemyBos->GetStatus()->SetLv(8);
				m_enemyBos->GetStatus()->SetStandardHp(100.0f);
				m_enemyBos->GetStatus()->SetStandardAgi(500.0f);
				m_enemyBos->GetStatus()->SetStandardDef(6.0f);
				m_enemyBos->GetStatus()->SetStandardAtk(17.0f);
				m_enemyBos->GetStatus()->StatusUp();
				return true;
			}
			return false;
		});
	m_gameCamera = g_gameObjM->NewGO<GameCamera>(1,"GameCamera");
	m_gameCamera->SetPlayer(m_player);
	m_light = g_gameObjM->NewGO<LightCamera>(1,"LightCamera");
	m_light->SetPlayer(m_player);
	m_ui = g_gameObjM->NewGO<UI>(1,"UI");
	//BGM
	m_bgm.Init(L"Assets/sound/bgm_Dungeon.wav");
	m_bgm_bos.Init(L"Assets/sound/bgm_Bos.wav");
	m_bgm.Play(true);
	m_bgm.SetVolume(0.17f);
	m_bgm_bos.SetVolume(0.17f);

	return false;
}

void Game::Update()
{
	if (g_pad[0].IsTrigger(enButtonSelect)) {
		g_gameObjM->DeleteGO(this);
		//タイトルシーンの作成。
		//g_gameObjM->NewGameObject<Title>();
		g_gameObjM->NewGO<GameEnd>(0,"GameEnd")->SetGameEnd(GameEnd::GameEndState::gameDefault);
	}
	if (!m_enemyBos->GetBGMFlag()) {

		m_bgm_bos.Stop();
		m_bgm.Play(true);
	}else{
		m_bgm.Stop();
		m_bgm_bos.Play(true);
	}
}

void Game::PreDraw()
{
}

void Game::Draw()
{
	m_level.Draw();
}

