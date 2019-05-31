#pragma once
/// <summary>
/// タイマークラス
/// </summary>
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
class Timer
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	Timer();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~Timer();
	/// <summary>
	/// タイマーのスタート
	/// （これを呼んだ瞬間から計測）
	/// </summary>
	void Start();
	/// <summary>
	/// タイマーをリスタートします。
	/// （総合時間を保持したままもう一度スタートします）
	/// </summary>
	void Restart();
	/// <summary>
	/// ストップ関数
	/// （ストップウォッチのラップのような関数）
	/// 時間を取りたいときに必ず呼んでください！！
	/// </summary>
	void Stop();
	/// <summary>
	/// ラップスタートからの秒を返す
	/// </summary>
	/// <returns>
	/// 経過秒（float）
	/// </returns>
	const float GetSeconds()
	{
		return (float)m_elapsed;
	}
	/// <summary>
	/// ラップスタートからの分（時間）を返す
	/// </summary>
	/// <returns>
	/// 経過　分（時間）（float）
	/// </returns>
	int GetMinute()
	{
		return (int)m_elapsed / 60;
	}
	/// <summary>
	/// スタートからの秒を返す
	/// （ラップ関係なしのすべての合計）
	/// </summary>
	/// <returns>
	/// 合計経過秒（float）
	/// </returns>
	float GetAllSeconds()
	{
		return (float)m_Allelapsed;
	}

	/// <summary>	
	/// スタートからの分を返す
	/// （ラップ関係なしのすべての合計）
	/// </summary>
	/// <returns>
	/// 合計経過　分（時間）(float)
	/// </returns>
	int GetAllMinute()
	{
		return (int)m_Allelapsed / 60.0f;
	}
private:
	LONGLONG m_start = 0;				//スタートした時の秒数
	LONGLONG m_stop = 0;				//ストップした時の秒数
	LONGLONG m_fleam = 0;				//ストップした時のフレーム数
	double m_elapsed = 0;				//経過時間(単位：秒)
	double m_Allelapsed = 0.0;			//すべての経過時間(単位：秒)
};
