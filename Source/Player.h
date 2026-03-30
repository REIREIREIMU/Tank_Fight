#pragma once
#include "Bullet.h"
#include "../Library/GameObject.h"

#include <vector>
#include <memory>

class Object;
class Enemy;

class Player : public GameObject {
public:
	Player(Object* object, std::vector<Enemy*>* enemies);
	~Player();

	void Update() override;
	void Draw() override;

	void Shoot();   // 弾を発射
	void IsDead();  // 死亡関数

	bool IsAlive() const { return m_alive; }

	VECTOR GetPosition() const { return Position; }
	void SetPosition(const VECTOR& pos) { Position = pos; }

	// 弾を描画
	void DrawBullets();

	// 弾全削除
	void ClearBullets();

	// 残機関連
	static int  GetLives();
	static void ResetLives(int lives = 2);
	static void DecreaseLives();

	// ステージ関連
	static int GetStage();
	static void NextStage();
	static void ResetStage();

	// 無敵制御
	static void SetInvincible(bool v);
	static bool IsInvincible();

	// 操作停止制御
	static void SetControlEnabled(bool v);
	static bool IsControlEnabled();

	// 現在生きている弾数を数える関数
	int CountAliveBullets() const;

private:
	bool m_alive;			      // 生存フラグ（Player用）
	static bool s_invincible;     // 無敵フラグ
	static bool s_controlEnabled; // 操作停止フラグ
	static int m_lives;		      // 残機
	static int s_stage;		      // ステージ

	bool  m_Exploding;          // 爆発中のフラグ
	int   m_Explosion_Handle;   // 爆発モデルのハンドル
	float m_Explosion_Scale;    // 現在サイズ（最初は小さい）

	int	Body_m_handle,  // 車体のハンドル
		Head_m_handle;  // 砲塔のハンドル

	float pos_x, 
		  pos_y1,	// 車体用 Y座標
		  pos_y2,	// 砲塔用 Y座標
		  pos_z;	// プレイヤー用座標（x,y,z）

	// プレイヤー移動量
	float move_x, move_z;

	// プレイヤー用ベクター
	VECTOR Position;		// 移動
	VECTOR Body_Rotation,	// 回転(車体)
		   Head_Rotation;	// 回転(砲塔)

	// 向いている方向に前進後進する為のベクター
	VECTOR forward;

	// 弾管理
	std::vector<std::unique_ptr<Bullet>> bullets;

	// 最大同時発射数
	static constexpr int Max_Player_Bullets = 3;

	// 当たり判定用（壁）
	Object* object = nullptr;
	std::vector<Enemy*>* enemies;

	// マウスポインター用ベクター
	VECTOR GetMouseWorldPos();
};