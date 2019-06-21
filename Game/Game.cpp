#include "stdafx.h"
#include "Game.h"
#include "Stage/Stage.h"
#include "Title.h"
#include "GameEnd.h"


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
	m_stage->StageSelect(Stage::StageType::Stage_Ground);
	return false;
}

void Game::Update()
{
	if (g_pad[0].IsTrigger(enButtonUp)) {
		g_gameObjM->DeleteGO(m_stage);

		m_stage = g_gameObjM->NewGO<Stage>(0, "Stage");
		m_stage->StageSelect(Stage::StageType::Stage_Dungeon);
	}
	else if (g_pad[0].IsTrigger(enButtonDown)) {
		g_gameObjM->DeleteGO(m_stage);

		m_stage = g_gameObjM->NewGO<Stage>(0, "Stage");
		m_stage->StageSelect(Stage::StageType::Stage_Ground);
	}
	if (g_pad[0].IsTrigger(enButtonSelect)) {
		g_gameObjM->DeleteGO(this);
		//タイトルシーンの作成。
		//g_gameObjM->NewGameObject<Title>();
		g_gameObjM->NewGO<GameEnd>(0,"GameEnd")->SetGameEnd(GameEnd::GameEndState::gameDefault);
	}
}

void Game::PreDraw()
{
}

void Game::Draw()
{
}

