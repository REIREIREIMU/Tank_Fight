#pragma once
#include "Bullet.h"
#include "../Library/GameObject.h"
#include <vector>
#include <memory>

class Player;
class Object;

enum class EnemyType
{
	Turret,
	Chaser
};

class Enemy : public GameObject {
public:
	Enemy(const VECTOR& spawnPos, EnemyType type, Object* object, Player* player);
	~Enemy();

	void Update() override;
	void Draw() override;

	void Shoot();   // 弾を発射
	void IsDead();  // 死亡関数

	void UpdateTurret();   // 固定砲台
	void UpdateChaser();   // 追従タイプ

	bool IsAlive() const { return m_alive; }
	VECTOR GetPosition() const { return Position; }

private:
	EnemyType type;		// Enemy のタイプ

	Player* player;		// Player ポインタ参照
	Object* object;		// Object ポインタ参照

	int	Body_m_handle,  // 車体のハンドル
		Head_m_handle;  // 砲塔のハンドル

	bool m_alive;		// 生存フラグ（Player用）

	bool  m_Exploding;          // 爆発中のフラグ
	int   m_Explosion_Handle;   // 爆発モデルのハンドル
	float m_Explosion_Scale;    // 現在サイズ（最初は小さい）

	float pos_x,
		pos_y1,	// 車体用 Y座標
		pos_y2,	// 砲塔用 Y座標
		pos_z;	// 敵用座標（x,y,z）

	// 敵の移動量
	//float move_x, move_z;

	// 敵用ベクター
	VECTOR Position;		// 移動
	VECTOR Body_Rotation,	// 回転(車体)
		Head_Rotation;		// 回転(砲塔)

	// 向いている方向に前進後進する為のベクター
	VECTOR forward;

	// 弾管理
	std::vector<std::unique_ptr<Bullet>> bullets;

	// 最大同時発射数
	//static constexpr int Max_Player_Bullets = 9;

	int shootTimer;	// 発射タイマー
	static constexpr int ShootInterval = 300;
};