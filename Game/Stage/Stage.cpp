#include "stdafx.h"
#include "Stage.h"
//#include "Background.h"
//#include "Dungeon.h"
#include "../Player\Player.h"
#include "../level/Level.h"
#include "../GameCamera.h"
#include "../LightCamera.h"
#include "../UI.h"
#include "../Enemy\Enemy.h"
#include "../EnemyBos/EnemyBos.h"
#include "../Sky.h"
#include "../Game.h"

Stage::Stage()
{
}

Stage::~Stage()
{
	//m_save->TypeSaveStage = m_type;
}

void Stage::Destroy()
{
	Release();
}
void Stage::Release()
{
	if (m_ui != nullptr) g_gameObjM->DeleteGO(m_ui);
	//if(m_background!=nullptr) g_gameObjM->DeleteGO(m_background);
	//if (m_dungeon != nullptr) g_gameObjM->DeleteGO(m_dungeon);
	if (m_gameCamera != nullptr) g_gameObjM->DeleteGO(m_gameCamera);
	if (m_light != nullptr) g_gameObjM->DeleteGO(m_light);
	if (m_player != nullptr) g_gameObjM->DeleteGO(m_player);
	//動的に確保したインスタンスを破棄。
	for (auto& enemy : m_enemyList) {
		if(enemy!=nullptr) g_gameObjM->DeleteGO(enemy);
	}
	if (m_enemyBos!=nullptr) g_gameObjM->DeleteGO(m_enemyBos);
	if (m_sky) g_gameObjM->DeleteGO(m_sky);
	m_save->SaveStage = m_type;
}
bool Stage::EnemyDelete(Enemy* enemy)
{
	if (enemy != nullptr) {
		m_enemyList.erase(std::remove(m_enemyList.begin(), m_enemyList.end(), enemy)
			, m_enemyList.end());
		//delete enemy;
		g_gameObjM->DeleteGO(enemy);
		return true;
	}
	return false;
}


bool Stage::Start()
{
	//BGM
	m_bgm.Init(L"Assets/sound/bgm_Dungeon.wav");
	m_bgm_bos.Init(L"Assets/sound/bgm_Bos.wav");
	m_bgm.Play(true);
	m_bgm.SetVolume(0.07f);
	m_bgm_bos.SetVolume(0.1f);
	return false;
}

void Stage::StageSelect(StageType type)
{
	m_save = g_gameObjM->FindGO<Save>("Save");
	m_type = type;
	switch (m_type)
	{
	case StageType::Stage_Dungeon:
		//レベルを初期化。
		DungeonNew();
		break;
	case StageType::Stage_Ground:
		GroundNew();
		break;
	}
}


void Stage::DungeonNew()
{
	m_level.Init(
		L"Assets/level/Dungeon_01.tkl",
		[&](LevelObjectData& objData) {
		/*if (objData.EqualName(L"Dungeon") == true) {
			m_dungeon = g_gameObjM->NewGO<Dungeon>(0, "Dungeon");
			m_dungeon->SetPosition(objData.position);
			m_dungeon->SetRotation(objData.rotation);
			return true;
		}*/
		return false;
	});
	//レベルを初期化。
	m_level.Init(
		L"Assets/level/player_01.tkl",
		[&](LevelObjectData& objData) {
		if (objData.EqualName(L"plpath") == true) {
			m_player = g_gameObjM->NewGO<Player>(0, "Player");
			m_player->SetPosition(objData.position);
			m_player->SetRotation(objData.rotation);
			//ステータスの設定
			m_player->GetStatus()->SetLv(m_save->PlayerLv);
			return true;
		}
		return true;
	});
	m_level.Init(
		L"Assets/level/enemy_01.tkl",
		[&](LevelObjectData& objData) {
		if (objData.EqualName(L"enpath") == true) {
			//エネミー！！！
			char enemyName[256];
			sprintf(enemyName, "EnemyS%d", m_enemyNo++);
			auto enemy = g_gameObjM->NewGO<Enemy>(0, enemyName);
			enemy->SetEnemyType(Enemy::EnemyType::type_skeleton);
			enemy->SetPosition(objData.position);
			enemy->SetRotation(objData.rotation);
			enemy->SetPlayer(m_player);
			m_enemyList.push_back(enemy);
			//エネミーのレベル
			//enemy->GetStatus()->SetLv(m_save->PlayerLv + 2);
			enemy->GetStatus()->SetLv(3);
			return true;
		}
		if (objData.EqualName(L"enpath2") == true) {
			//エネミー！！！
			char enemyName[256];
			sprintf(enemyName, "EnemyT%d", m_enemyNo++);
			auto enemy = g_gameObjM->NewGO<Enemy>(0, enemyName);
			enemy->SetEnemyType(Enemy::EnemyType::type_troll);
			enemy->SetPosition(objData.position);
			enemy->SetRotation(objData.rotation);
			enemy->SetPlayer(m_player);
			m_enemyList.push_back(enemy);
			//エネミーのレベル
			//enemy->GetStatus()->SetLv(m_save->PlayerLv + 2);
			enemy->GetStatus()->SetLv(3);
			return true;
		}
		return true;
	});
	m_level.Init(
		L"Assets/level/enemy_02.tkl",
		[&](LevelObjectData& objData) {
		if (objData.EqualName(L"enpath") == true) {
			//エネミー！！！
			char enemyName[256];
			sprintf(enemyName, "EnemyS%d", m_enemyNo++);
			auto enemy = g_gameObjM->NewGO<Enemy>(0, enemyName);
			enemy->SetEnemyType(Enemy::EnemyType::type_skeleton);
			enemy->SetPosition(objData.position);
			enemy->SetRotation(objData.rotation);
			enemy->SetPlayer(m_player);
			m_enemyList.push_back(enemy);
			//エネミーのレベル
			//enemy->GetStatus()->SetLv(m_save->PlayerLv + 6);
			enemy->GetStatus()->SetLv(7);
			enemy->GetStatus()->StatusUp();
			return true;
		}
		if (objData.EqualName(L"enpath2") == true) {
			//エネミー！！！
			char enemyName[256];
			sprintf(enemyName, "EnemyT%d", m_enemyNo++);
			auto enemy = g_gameObjM->NewGO<Enemy>(0, enemyName);
			enemy->SetEnemyType(Enemy::EnemyType::type_troll);
			enemy->SetPosition(objData.position);
			enemy->SetRotation(objData.rotation);
			enemy->SetPlayer(m_player);
			m_enemyList.push_back(enemy);
			//エネミーのレベル
			//enemy->GetStatus()->SetLv(m_save->PlayerLv + 5);
			enemy->GetStatus()->SetLv(6); 
			enemy->GetStatus()->StatusUp();
			return true;
		}
		return true;
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
			//m_enemyBos->GetStatus()->SetLv(m_save->PlayerLv + 7);
			m_enemyBos->GetStatus()->SetLv(9);
			return true;
		}
		return true;
	});
	m_gameCamera = g_gameObjM->NewGO<GameCamera>(1, "GameCamera");
	m_gameCamera->SetPlayer(m_player);
	m_light = g_gameObjM->NewGO<LightCamera>(1, "LightCamera");
	m_light->SetPlayer(m_player);
	m_ui = g_gameObjM->NewGO<UI>(1, "UI");
	m_sky = nullptr;
}

void Stage::GroundNew()
{
	m_level.Init(
		L"Assets/level/Map.tkl",
		[&](LevelObjectData& objData) {
		/*if (objData.EqualName(L"ground") == true) {
			m_background = g_gameObjM->NewGO<Background>(0, "Ground");
			m_background->SetPosition(objData.position);
			m_background->SetRotation(objData.rotation);
			return false;
		}*/
		return false;
	});
	//レベルを初期化。
		m_level.Init(
			L"Assets/level/player_02.tkl",
			[&](LevelObjectData& objData) {
			if (objData.EqualName(L"plpath") == true) {
				m_player = g_gameObjM->NewGO<Player>(0, "Player");
				m_player->SetPosition(objData.position);
				m_player->SetRotation(objData.rotation);
				//ステータスの設定
				m_player->GetStatus()->SetLv(m_save->PlayerLv);
				return true;
			}
			return true;
		});
	m_level.Init(
		L"Assets/level/BosHouse.tkl",
		[&](LevelObjectData& objData) {
		if (objData.EqualName(L"BosHouse") == true) {
			//エネミー！！！
			auto rot = objData.rotation;
			rot.SetRotationDeg(CVector3::AxisY(), 180.0f);
			m_enemyBos = g_gameObjM->NewGO<EnemyBos>(0, "EnemyBos");
			m_enemyBos->SetPosition(objData.position);
			m_enemyBos->SetRotation(rot);
			m_enemyBos->GetPlayer(m_player);
			m_enemyBos->GetStatus()->SetLv(35);
			return false;
		}
		return true;
	});
	m_gameCamera = g_gameObjM->NewGO<GameCamera>(1, "GameCamera");
	m_gameCamera->SetPlayer(m_player);
	m_light = g_gameObjM->NewGO<LightCamera>(1, "LightCamera");
	m_light->SetPlayer(m_player);
	m_ui = g_gameObjM->NewGO<UI>(1, "UI");
	m_sky = g_gameObjM->NewGO<Sky>(2, "Sky");
}

//簡易スポーン
void Stage::EnemySpawn()
{
	if (m_type == StageType::Stage_Ground) {
		if (m_middleEnemyNum < m_enemyMaxNum * 4) {
			m_level.Init(
				L"Assets/level/SpawnEnemy.tkl",
				[&](LevelObjectData& objData) {
				if (objData.EqualName(L"enpath") == true) {
					auto pos = objData.position;
					pos.y -= 150.0f;
					if (m_enemyList.size() <= 0) {
						for (int i = 0; i < m_enemyMaxNum; i++) {
							pos.x -= 200.0f;
							auto rot = objData.rotation;
							rot.SetRotationDeg(CVector3::AxisY(), 180.0f);
							char enemyName[256];
							sprintf(enemyName, "EnemyS%d", m_enemyNo++);
							auto enemy = g_gameObjM->NewGO<Enemy>(0, enemyName);
							if (i % 2 == 0) {
								enemy->SetEnemyType(Enemy::EnemyType::type_skeleton);
							}
							else if (i % 2 == 1)
							{
								enemy->SetEnemyType(Enemy::EnemyType::type_troll);
							}
							enemy->SetPosition(pos);
							enemy->SetRotation(rot);
							enemy->SetPlayer(m_player);
							m_enemyList.push_back(enemy);
							//エネミーのレベル
							enemy->GetStatus()->SetLv(22);
							enemy->GetStatus()->StatusUp();
							m_middleEnemyNum++;
						}
					}
					return true;
				}
				return true;
			});
		}
	}

}

void Stage::Update()
{
	EnemySpawn();
	if (m_enemyBos != nullptr) {
		if (!m_enemyBos->GetBGMFlag()) {

			m_bgm_bos.Stop();
			m_bgm.Play(true);
		}
		else {
			m_bgm.Stop();
			m_bgm_bos.Play(true);
		}
	}
	if (g_gameObjM->FindGO<Game>("Game")->GetUpdateStop()) {
		m_bgm.Stop();
		m_bgm_bos.Stop();
	}

}

void Stage::Draw()
{
	m_level.Draw();
}

