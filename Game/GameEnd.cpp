#include "stdafx.h"
#include "GameEnd.h"
#include "Title.h"
#include "Game.h"


GameEnd::GameEnd()
{
}

GameEnd::~GameEnd()
{
}

void GameEnd::Destroy()
{
}

bool GameEnd::Start()
{
	//2D���������B
	//�w�i�F�X�v���C�g
	m_titelBackSprite.Init(L"Assets/sprite/TitelBack.dds", 1280.0f, 720.0f);
	//�Q�[���I�[�o�[�X�v���C�g
	m_gameOverSprite.Init(L"Assets/sprite/GameOver.dds", 1280.0f, 720.0f);
	//�Q�[���N���A�X�v���C�g
	m_gameClearedSprite.Init(L"Assets/sprite/GameCleared.dds", 1280.0f, 720.0f);
	m_gameEndSprite.Init(L"Assets/sprite/GameEnd.dds", 1280.0f, 720.0f);
	//���X�v���C�g
	m_ArrowSprite.Init(L"Assets/sprite/Yazirusi.dds", 60.0f, 80.0f);
	m_ArrowPos = m_ArrowYesPos;
	return false;
}

void GameEnd::SelectYesNo(bool flag)
{
	if (g_pad[0].IsTrigger(enButtonUp)) {
		m_yesNoFlag = true;
		m_ArrowPos = m_ArrowYesPos;
	}
	if (g_pad[0].IsTrigger(enButtonDown)) {
		m_yesNoFlag = false;
		m_ArrowPos = m_ArrowNoPos;
	}
	if (g_pad[0].IsTrigger(enButtonA)) {
		if (m_yesNoFlag) {
			g_gameObjM->DeleteGO(this);
			g_gameObjM->NewGO<Title>(0,"Title");
			g_gameObjM->FindGO<Save>("Save")->NeworLoadGame(1, 0.0f);
		}
		else {
			g_gameObjM->DeleteGO(this);
			g_gameObjM->NewGO<Game>(0,"Game");
			if (flag) {
				g_gameObjM->FindGO<Save>("Save")->NeworLoadGame(1, 0.0f);
			}
		}
	}
}

void GameEnd::Update()
{
	//�w�i�X�v���C�g�̍X�V�B
	m_titelBackSprite.Update(
		CVector3::Zero(),
		CQuaternion::Identity(),
		CVector3::One()
	);
	switch (m_ges)
	{
	case gameCleared:
		m_gameClearedSprite.Update(
			CVector3::Zero(),
			CQuaternion::Identity(),
			CVector3::One()
		);
		SelectYesNo(true);
		m_ArrowSprite.Update(
			m_ArrowPos,
			CQuaternion::Identity(),
			CVector3::One()
		);
		break;
	case gameOver:
		m_gameOverSprite.Update(
			CVector3::Zero(),
			CQuaternion::Identity(),
			CVector3::One()
		);
		SelectYesNo(false);
		m_ArrowSprite.Update(
			m_ArrowPos,
			CQuaternion::Identity(),
			CVector3::One()
		);
		break;
	case Ground:
		g_gameObjM->DeleteGO(this);
		g_gameObjM->NewGO<Game>(0, "Game");
	case gameDefault:
		m_gameEndSprite.Update(
			CVector3::Zero(),
			CQuaternion::Identity(),
			CVector3::One()
		);
		SelectYesNo(false);
		m_ArrowSprite.Update(
			m_ArrowPos,
			CQuaternion::Identity(),
			CVector3::One()
		);
		break;
	}
}

void GameEnd::PostDraw()
{
	//�w�i�̐F�`��B
	m_titelBackSprite.Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
	switch (m_ges)
	{
	case gameCleared:
		m_gameClearedSprite.Draw(
			g_camera2D.GetViewMatrix(),
			g_camera2D.GetProjectionMatrix()
		);
		//���`��B
		m_ArrowSprite.Draw(
			g_camera2D.GetViewMatrix(),
			g_camera2D.GetProjectionMatrix()
		);
		break;
	case gameOver:
		m_gameOverSprite.Draw(
			g_camera2D.GetViewMatrix(),
			g_camera2D.GetProjectionMatrix()
		);
		//���`��B
		m_ArrowSprite.Draw(
			g_camera2D.GetViewMatrix(),
			g_camera2D.GetProjectionMatrix()
		);
		break;
	case gameDefault:
		m_gameEndSprite.Draw(
			g_camera2D.GetViewMatrix(),
			g_camera2D.GetProjectionMatrix()
		);
		//���`��B
		m_ArrowSprite.Draw(
			g_camera2D.GetViewMatrix(),
			g_camera2D.GetProjectionMatrix()
		);
		break;
	}
}
