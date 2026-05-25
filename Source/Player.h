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

	// ========= 残機関連 =========
	static int  GetLives();
	static void ResetLives(int lives = 2);
	static void DecreaseLives();

	// ======= ステージ関連 =======
	static int GetStage();
	static void NextStage();
	static void ResetStage();

	// ========== 撃破数 ==========
	static int  GetTotalEnemyKill();
	static void AddEnemyKill(int count);
	static void ResetEnemyKill();

	// ========= 無敵制御 =========
	static void SetInvincible(bool v);
	static bool IsInvincible();

	// ======= 操作停止制御 =======
	static void SetControlEnabled(bool v);
	static bool IsControlEnabled();

private:
	// ==== プレイヤーの更新処理 ====
	void Bullets();
	void Move();
	void Shooting();
	void Rotation();
	void Explosion();

	// マウスポインター用ベクター
	VECTOR GetMouseWorldPos();

	// ======= パラメータ =======
	const float Speed;	// 移動速度
	const float Angle;	// 車体の回転速度

	// =========== 状態 ===========
	bool Alive;				// 生存フラグ
	bool Exploding;			// 死亡フラグ

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

	// プレイヤー移動量
	float move_x, move_z;

	// プレイヤー用ベクター
	VECTOR Position;		// 移動
	VECTOR Body_Rotation,	// 回転(車体)
		Head_Rotation;	// 回転(砲塔)

	// 向いている方向に前進後進する為のベクター
	VECTOR forward;

	// ============ 弾 ============
	std::vector<std::unique_ptr<Bullet>> bullets;	// 弾管理
	static constexpr int Max_Player_Bullets = 3;	// 最大同時発射数

	// ===== 当たり判定用（壁） =====
	Object* object = nullptr;
	std::vector<Enemy*>* enemies;

	// ============================
	static bool Invincible;     // 無敵フラグ
	static bool ControlEnabled; // 操作停止フラグ
	static int TotalEnemyKill;  // 倒した敵数
	static int Lives;		    // 残機
	static int Stage;		    // ステージ
};