#include "stdafx.h"
#include "Game.h"
#include "Player\Player.h"
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
	//m_level.Init(L"Assets/level/stage_02.tkl",&m_shadowMap, nullptr);
	m_level.Init(L"Assets/level/stage_02.tkl",&m_shadowMap, [&](LevelObjectData& objData) {
		if (objData.EqualName(L"plpath") == true) {
			m_player = g_gameObjM->NewGameObject<Player>();
			m_player->SetPosition(objData.position);
			m_player->SetShadow(&m_shadowMap);
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
			m_enemy->SetShadow(&m_shadowMap);
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
			m_enemy->SetShadow(&m_shadowMap);
			return true;
		}
		return false;
	});
	m_gameCamera=g_gameObjM->NewGameObject<GameCamera>(4);
	m_gameCamera->SetPlayer(m_player);
}


Game::~Game()
{
	g_gameObjM->DeleteGameObject(m_player);
	//���I�Ɋm�ۂ����C���X�^���X��j���B
	for (auto& enemy : m_enemyList) {
		g_gameObjM->DeleteGameObject(enemy);
	}
	g_gameObjM->DeleteGameObject(m_gameCamera);
}

void Game::Update()
{
	if (g_pad[0].IsTrigger(enButtonB)) {
		g_gameObjM->DeleteGameObject(this);
		//�^�C�g���V�[���̍쐬�B
		g_gameObjM->NewGameObject<Title>();
	}
	//�V���h�E�}�b�v���X�V�B
	m_shadowMap.UpdateFromLightTarget(
		{ 1000.0f, 1000.0f, 1000.0f },
		{ 0.0f, 0.0f, 0.0f }
	);
}

void Game::Draw()
{
	//�`��J�n�B
	g_graphicsEngine->BegineRender();
	///////////////////////////////////////////////
	//�V���h�E�}�b�v�Ƀ����_�����O
	///////////////////////////////////////////////
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	//���݂̃����_�����O�^�[�Q�b�g���o�b�N�A�b�v���Ă����B
	ID3D11RenderTargetView* oldRenderTargetView;
	ID3D11DepthStencilView* oldDepthStencilView;
	d3dDeviceContext->OMGetRenderTargets(
		1,
		&oldRenderTargetView,
		&oldDepthStencilView
	);
	//�r���[�|�[�g���o�b�N�A�b�v������Ă����B
	unsigned int numViewport = 1;
	D3D11_VIEWPORT oldViewports;
	d3dDeviceContext->RSGetViewports(&numViewport, &oldViewports);

	//�V���h�E�}�b�v�Ƀ����_�����O
	m_shadowMap.RenderToShadowMap();

	//���ɖ߂��B
	d3dDeviceContext->OMSetRenderTargets(
		1,
		&oldRenderTargetView,
		oldDepthStencilView
	);
	d3dDeviceContext->RSSetViewports(numViewport, &oldViewports);
	//�����_�����O�^�[�Q�b�g�ƃf�v�X�X�e���V���̎Q�ƃJ�E���^�������B
	oldRenderTargetView->Release();
	oldDepthStencilView->Release();

	///////////////////////////////////////////////
	//��������ʏ탌���_�����O�B
	///////////////////////////////////////////////
	m_level.Draw();
	m_player->Draw();
	for (auto& enemy : m_enemyList) {
		enemy->Draw();
	}
	//�`��I���B
	g_graphicsEngine->EndRender();
}

void Game::Destroy()
{
}
