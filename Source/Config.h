#pragma once

namespace Config {
	// 当たり判定
	constexpr float Player_Half = 0.4f; // Player用（箱型）
	constexpr float Enemy_Half  = 0.4f; // Enemy用
	constexpr float Block_Half  = 0.5f; // Block用
	constexpr float Bullet_Half = 0.1f; // Bullet用
	// 爆発演出用 定数
	static constexpr float ExplosionStartScale = 0.2f;		// 最初の大きさ（爆発演出）
	static constexpr float ExplosionEndScale   = 1.2f;		// 最大の大きさ（爆発演出）
	static constexpr float ExplosionGrowSpeed  = 0.03f;		// 拡大速度（爆発演出）
	// 敵のAI
	static constexpr int EnemyShoot_Interval = 450;	// 敵の発砲時間
}
