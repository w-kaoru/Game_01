#pragma once
#include "IScene.h"

class Title : public IScene
{
public:
	Title();
	~Title();
	//�I�[�o�[���C�h�����֐��ɂ�override�C���q�����Ă���ƍK���ɂȂ��B
	void Update() override;
	//�I�[�o�[���C�h�����֐��ɂ�override�C���q�����Ă���ƍK���ɂȂ��B
	//why?
	//���̊֐��̂悤�Ƀ^�C�v�~�X�������Ƃ��ɃR���p�C���G���[�ɂȂ��ċ����Ă����B
	//void Drow() override;
	void Draw() override;
private:
};
