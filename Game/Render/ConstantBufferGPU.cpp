#include "stdafx.h"
#include "ConstantBufferGPU.h"


ConstantBufferGPU::~ConstantBufferGPU()
{
	if (m_constantBuffer != nullptr) {
		m_constantBuffer->Release();
	}
}

void ConstantBufferGPU::Create(const void* pInitData, int bufferSize)
{
	//�쐬����萔�o�b�t�@�̏���ݒ肷��B
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//16�o�C�g�A���C�����g�ɐ؂肠����B
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	auto d3dDevice = g_graphicsEngine->GetD3DDevice();
	if (pInitData != nullptr) {
		//�������f�[�^���w�肳��Ă���B
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = pInitData;
		d3dDevice->CreateBuffer(&bufferDesc, &InitData, &m_constantBuffer);
	}
	else {
		d3dDevice->CreateBuffer(&bufferDesc, nullptr, &m_constantBuffer);
	}
}