#pragma once
/// <summary>
/// GPU��̒萔�o�b�t�@�B
/// </summary>
class ConstantBufferGPU
{
public:
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~ConstantBufferGPU();
	/// <summary>
	/// VRAM��ɒ萔�o�b�t�@���쐬����B
	/// </summary>
	/// <param name="pInitData">
	/// �������f�[�^�B
	/// nullptr���w�肵���ꍇ�́A���������̒萔�o�b�t�@���쐬����܂��B
	/// </param>
	/// <param name="bufferSize">
	/// �쐬����o�b�t�@�̃T�C�Y�B
	/// ���̊֐�����16�o�C�g�A���C�����g�ɐ؂�グ����܂��B
	/// </param>
	void Create(const void* pInitData, int bufferSize);
	/// <summary>
	/// ���萔�o�b�t�@���擾�B
	/// </summary>
	/// <returns></returns>
	ID3D11Buffer* GetD3D11Buffer() const
	{
		return m_constantBuffer;
	}

private:
	ID3D11Buffer*		m_constantBuffer = nullptr;	//!<GPUBuffe��̒萔�o�b�t�@�B

};

