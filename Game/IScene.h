#pragma once
//シーンのインターフェースクラス。
class IScene
{
public:
	IScene()
	{
	}
	//基底クラスのデストラクタは仮想関数にする必要がある。
	//詳しく知りたい人はEffective C++を読むように。
	virtual ~IScene()
	{
	}
	virtual void Update() = 0;
	virtual void Draw() = 0;
};
extern IScene* g_currentScene;
