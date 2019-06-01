#pragma once
class CharacterStatus
{
public:
	CharacterStatus();
	~CharacterStatus();
	void StatusUp()
	{
		//ステータスをUPさせる計算
		if (m_lv > 1) {
			if (m_lv <= m_maxLv) {
				m_lvUp = (float(m_lv)) / 10.0f;
				m_lvUp += 1.2f;
			}
			else
			{
				m_lv = m_maxLv;
			}
		}
		else {
			m_lvUp = 1.0f;
		}
		m_hp = m_standardHp * m_lvUp;
		m_atk = m_standardAtk * m_lvUp;
		m_def = m_standardDef * m_lvUp;
		m_maxHp = m_standardHp * m_lvUp;
		//m_agi = m_standardAgi * m_lvUp;
	}
	void LvUp()
	{
		if (m_lv < m_maxLv) {
			m_lv++;
		}
		if (m_lv < 1) {
			m_lv = 1;
		}
	}
	//セッター
	void SetLv(int lv)
	{
		m_lv = lv;
	}
	void SetHp(float hp)
	{
		m_hp = hp;
		m_standardHp = hp;
	}
	void SetAtk(float atk)
	{
		m_atk = atk;
		m_standardAtk = atk;
	}
	void SetDef(float def)
	{
		m_def = def;
		m_standardDef = def;
	}
	void SetAgi(float agi)
	{
		m_agi = agi;
		m_standardAgi = agi;
	}
	void SetMaxLv(int MaxLv)
	{
		m_maxLv = MaxLv;
	}

	void SetMaxHp(float MaxHp)
	{
		m_maxHp = MaxHp;
	}
	//ゲッター
	int GetLv()
	{
		return m_lv;
	}
	float GetHp()
	{
		return m_hp;
	}
	float GetAtk()
	{
		return m_atk;
	}
	float GetDef()
	{
		return m_def;
	}
	float GetAgi()
	{
		return m_agi;
	}
	int GetMaxLv()
	{
		return m_maxLv;
	}
	float GetMaxHp()
	{
		return m_maxHp;
	}
private:
	//キャラクターのステータス
	int		m_lv = 0;		//レベル
	float	m_hp = 0.0f;	//体力
	float	m_atk = 0.0f;	//攻撃力
	float	m_def = 0.0f;	//硬さ
	float	m_agi = 0.0f;	//素早さ
	float	m_lvUp = 0.0f;	
	int		m_maxLv = 100;//レベルの最大値	
	float	m_maxHp = 100;
	//基準のステータス
	float	m_standardHp = 0.0f;	//体力
	float	m_standardAtk = 0.0f;	//攻撃力
	float	m_standardDef = 0.0f;	//硬さ
	float	m_standardAgi = 0.0f;	//素早さ
};
