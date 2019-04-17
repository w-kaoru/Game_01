#pragma once
#include "gameObject\IGameObject.h"

class Title:public IGameObject
{
public:
	Title();
	~Title();
	bool Start() override;
	//�I�[�o�[���C�h�����֐��ɂ�override�C���q�����Ă���ƍK���ɂȂ��B
	void Update() override;
	//�I�[�o�[���C�h�����֐��ɂ�override�C���q�����Ă���ƍK���ɂȂ��B
	//why?
	//���̊֐��̂悤�Ƀ^�C�v�~�X�������Ƃ��ɃR���p�C���G���[�ɂȂ��ċ����Ă����B
	void PostDraw() override;
private:
	Sprite m_titelBackSprite;
	Sprite m_titelSprite;
	CSoundSource m_bgm;						//BGM�B
};
