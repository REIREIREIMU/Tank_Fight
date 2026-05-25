#pragma once
#include "../Library/GameObject.h"
#include <vector>

class Object;
class Player;
class Enemy;

class Bullet : public GameObject {
public:
    Bullet(
        const VECTOR& startPos,
        const VECTOR& dir,
        Object* obj,
        Player* owner,
        std::vector<Enemy*>* enemies
    );

    ~Bullet();

    void Update() override;
    void Draw() override;

    // =========== 状態 ===========
    bool IsAlive() const { return Alive || !m_trail.empty(); }

private:
    // ======= ポインター群 =======
    Player* player = nullptr;		// Playerのポインタを作成
    Object* object = nullptr;		// Objectのポインタを作成
    std::vector<Enemy*>* enemies;	// Enemyのポインタを作成

    // ===== 弾の更新処理 =====
    void Move();
    void PlayerHit();
    void EnemyHit();

    void CheckWallCollision();          // 壁との当たり判定
    void Reflect(const VECTOR& normal); // 反射処理

    void Trail();
    void Transform();

    // ========== モデル ==========
    int Bullet_handle;   // 弾のハンドル

    // =========== 状態 ===========
    bool   Alive;        // 生存フラグ
    int    ReflectCount;      // 弾の反射回数

    // ========= 物理処理 =========
    VECTOR pos;          // 弾の位置
    VECTOR vel;          // 弾の速度

    // ========= 軌道処理 =========
    std::vector<VECTOR> m_trail;                // 弾の軌道履歴
    bool   TrailGrowing;                        // 軌道生成フラグ
    static constexpr int MaxTrailPoints = 60;   // 軌道線の残る長さ
};