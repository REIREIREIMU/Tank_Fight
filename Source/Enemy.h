#pragma once
#include "Bullet.h"
#include "../Library/GameObject.h"
#include <vector>
#include <memory>

class Player;
class Object;

// 敵の種類
enum class EnemyType
{
	Turret,	// 固定砲台型
	Chaser	// 尾行型
};

class Enemy : public GameObject {
public:
	Enemy(const VECTOR& spawnPos, EnemyType type,
		Object* object, Player* player);
	~Enemy();

	void Update() override;
	void Draw() override;

	// =========== 行動 ===========
	void Shoot();   // 弾を発射
	void IsDead();  // 死亡関数

	// =========== 状態 ===========
	bool IsAlive() const { return Alive; }

	// =========== 位置 ===========
	VECTOR GetPosition() const { return Position; }
	void SetPosition(const VECTOR& pos) { Position = pos; }

	// ========== 弾関連 ==========
	void DrawBullets();				// 弾を描画
	void ClearBullets();			// 弾全削除
	int CountAliveBullets() const;	// 現在生きている弾数を数える関数

	// ========= 無敵制御 =========
	static void SetInvincible(bool v);
	static bool IsInvincible();

private:
	EnemyType type;		// Enemy のタイプ

	// ======== 敵の更新処理 ========
	void Bullets();
	void EnemyAI();
	void Shooting();
	void Rotation();
	void Explosion();

	// ========= 各種の更新 =========
	void UpdateTurret();   // 固定砲台
	void UpdateChaser();   // 追従タイプ

	// ======= ポインター群 =======
	Player* player;		// Player ポインタ参照
	Object* object;		// Object ポインタ参照

	// ======= パラメータ =======
	const float Speed;	// 移動速度
	const float Angle;	// 車体の回転速度

	// 尾行型のみのパラメータ
	const float ApproachDistance = 8.0f; // 接近距離
	const float OrbitDistance = 5.0f; // 回り込み距離

	// =========== 状態 ===========
	bool Alive;			// 生存フラグ
	bool Exploding;		// 死亡フラグ

	// ========== モデル ==========
	int	Body_handle;		// 車体のハンドル
	int Head_handle;		// 砲塔のハンドル
	int Explosion_Handle;	// 爆発モデルのハンドル

	// =========== 爆発 ===========
	float Explosion_Scale;    // 現在サイズ（最初は小さい）

	// ======== 移動・回転 ========
	float pos_x, pos_z;
	float pos_y1;	// 車体回転
	float pos_y2;	// 砲塔回転

	// 敵用ベクター
	VECTOR Position;		// 移動
	VECTOR Body_Rotation,	// 回転(車体)
		Head_Rotation;	// 回転(砲塔)

	// 向いている方向に前進後進する為のベクター
	VECTOR forward;

	// ============ 弾 ============
	std::vector<std::unique_ptr<Bullet>> bullets;	// 弾管理
	static constexpr int Max_Enemy_Bullets = 3;		// 最大同時発射数(敵個人の)

	// =========== 射撃 ===========
	int ShootTimer;								// 発射タイマー
	static constexpr int ShootInterval = 300;	// 5秒間隔で発射

	// ============================
	static bool Invincible; // 無敵フラグ
};