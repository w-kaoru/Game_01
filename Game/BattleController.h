#pragma once
class Player;
class Enemy;
class BattleController :public IGameObject
{
public:
	BattleController();
	~BattleController();
	//bool Start();
	void Attack(float atk);
	void Damage();
	//void Update();
private:

};

