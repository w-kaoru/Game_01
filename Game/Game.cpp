#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "level/Level.h"
#include "GameCamera.h"
#include "Title.h"

//�O���[�o���ȃA�N�Z�X�|�C���g���O���[�o���ϐ��Ƃ��Ē񋟂���B
Game* g_game = nullptr;

Game::Game()
{
	//m_player = g_gameObjM->NewGameObject<Player>();
	g_game = this;
	//���x�����������B
	//m_level.Init(L"Assets/level/stage_01.tkl", nullptr);
	//m_level.Init(L"Assets/level/stage_01.tkl", [&](LevelObjectData& objData) {
	//	if (objData.EqualName(L"plpath") == true) {
	//		m_player = g_gameObjM->NewGameObject<Player>();
	//		m_player->SetPosition(objData.position);
	//		return true;
	//	}
	//	else if (objData.EqualName(L"enpath") == true) {
	//		//�G�l�~�[�I�I�I
	//		m_enemy = g_gameObjM->NewGameObject<Enemy>();
	//		m_enemy->SetPosition(objData.position);
	//		m_enemy->SetRotation(objData.rotation);
	//		m_enemy->GetPlayer(m_player);
	//		m_enemyList.push_back(m_enemy);
	//		return true;
	//	}
	//	return false;
	//});

	//���x�����������B
	m_level.Init(L"Assets/level/stage_02.tkl", nullptr);
	m_level.Init(L"Assets/level/stage_02.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualName(L"plpath") == true) {
			m_player = g_gameObjM->NewGameObject<Player>();
			m_player->SetPosition(objData.position);
			return true;
		}
		else if (objData.EqualName(L"enpath") == true) {
			//�G�l�~�[�I�I�I
			m_enemy = g_gameObjM->NewGameObject<Enemy>();
			m_enemy->SetEnemySelect(1);
			m_enemy->SetPosition(objData.position);
			m_enemy->SetRotation(objData.rotation);
			m_enemy->GetPlayer(m_player);
			m_enemyList.push_back(m_enemy);
			return true;
		}
		else if (objData.EqualName(L"enpath2") == true) {
			//�G�l�~�[�I�I�I
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
	//���I�Ɋm�ۂ����C���X�^���X��j���B
	for (auto& enemy : m_enemyList) {
		g_gameObjM->DeleteGameObject(enemy);
	}

}

void Game::Update()
{
	//�v���C���[�̍X�V�B

	m_gameCamera.Update();
	//m_player->Update();
	////Enemy���X�V�B
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
		//�^�C�g���V�[���̍쐬�B
		g_gameObjM->NewGameObject<Title>();
	}
}

void Game::Draw()
{
	//�v���C���[�̕`��B
	//m_player->Draw();
	//���x����`��B
	m_level.Draw();
	//�G�l�~�[��`��B
	//for (auto& enemy : m_enemyList) {
	//	enemy->Draw();
	//}
}

void Game::Destroy()
{
}
