#include "stdafx.h"
#include "system/system.h"
#include "Title.h"
#include <algorithm>
#include"gameObject/GameObjectManager.h"
//#include "sound/SoundEngine.h"


///////////////////////////////////////////////////////////////////
//�Q�[���̍X�V�����B
//���W�̍X�V�Ƃ��̏����͂����ŌĂяo���Ƃ悢�����H
///////////////////////////////////////////////////////////////////
void UpdateGame()
{
	//�Q�[���p�b�h�̍X�V�B	
	for (auto& pad : g_pad) {
		pad.Update();
	}
	//�����G���W���̍X�V�B
	g_physics.Update();
	//���݂̃V�[���̍X�V�B
	//g_gameObjM->Update();
}
///////////////////////////////////////////////////////////////////
// �Q�[���̕`�揈���B
// �`��ƍX�V�͕����Ă��ق����ǂ������H
///////////////////////////////////////////////////////////////////
void RenderGame()
{
	//�`��J�n�B
	g_graphicsEngine->BegineRender();
	g_gameObjM->Execute();
	//�`��I���B
	g_graphicsEngine->EndRender();
}

///////////////////////////////////////////////////////////////////
// �Q�[���̏I�������B
///////////////////////////////////////////////////////////////////
void TermnateGame()
{

}
///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
HitObject* g_hitObject;
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	g_camera2D.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	g_camera2D.SetWidth(FRAME_BUFFER_W);
	g_camera2D.SetHeight(FRAME_BUFFER_H);
	g_camera2D.SetPosition({ 0.0f, 0.0f, -10.0f });
	g_camera2D.SetTarget(CVector3::Zero());
	g_camera2D.Update();
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");
	CSoundEngine soundEngine;				//�T�E���h�G���W���B
	soundEngine.Init();
	//�^�C�g���V�[���̍쐬�B
	g_hitObject = new HitObject;
	g_gameObjM->NewGO<Title>(0,"Title");

	//�Q�[�����[�v�B
	while (DispatchWindowMessage() == true)
	{
		//�Q�[���̍X�V�B
		UpdateGame();
		//���y�f�[�^�̍X�V
		soundEngine.Update();
		//�Q�[���̕`�揈���B
		RenderGame();
	}
	//�Q�[���̏I������
	TermnateGame();
}