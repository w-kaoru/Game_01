#pragma once
//�V�[���̃C���^�[�t�F�[�X�N���X�B
class IScene
{
public:
	IScene()
	{
	}
	//���N���X�̃f�X�g���N�^�͉��z�֐��ɂ���K�v������B
	//�ڂ����m�肽���l��Effective C++��ǂނ悤�ɁB
	virtual ~IScene()
	{
	}
	virtual void Update() = 0;
	virtual void Draw() = 0;
};
extern IScene* g_currentScene;
