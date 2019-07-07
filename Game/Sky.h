#pragma once
class Sky :public IGameObject
{
public:
	Sky();
	~Sky();
	bool Start() override;
	void Update() override;
	void Draw() override;
private:
	SkinModel m_model;
	ID3D11ShaderResourceView* m_cubeMap = nullptr;
};