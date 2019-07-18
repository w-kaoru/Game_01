#include "stdafx.h"
#include "Game.h"
#include "Stage/Stage.h"
#include "Title.h"
#include "GameEnd.h"
#include "Player/Player.h"


Game::Game()
{
}

Game::~Game()
{
}

void Game::Destroy()
{
	g_gameObjM->DeleteGO(m_stage);
}

bool Game::Start()
{
	m_stage = g_gameObjM->NewGO<Stage>(0, "Stage");
	m_stage->StageSelect((StageType)g_gameObjM->FindGO<Save>("Save")->SaveStage);
	return false;
}

void Game::Update()
{
	if (g_pad[0].IsTrigger(enButtonSelect)) {
		//g_gameObjM->DeleteGO(this);
		m_updateStop = true;
		//タイトルシーンの作成。
		g_gameObjM->NewGO<GameEnd>(0,"GameEnd")->SetGameEnd(GameEnd::GameEndState::gameDefault);
	}
}

void Game::PreDraw()
{
}

void Game::Draw()
{
}

