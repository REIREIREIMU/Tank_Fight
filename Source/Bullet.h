#pragma once
#include "../Library/GameObject.h"
#include <vector>

class Object;

class Bullet : public GameObject {
public:
	Bullet(const VECTOR& startPos, const VECTOR& dir, Object* obj);
    ~Bullet();

    void Update()override;
    void Draw() override;

    bool IsAlive() const { return m_alive || !m_trail.empty(); }

private:
    int Bullet_m_handle;   // 弾のハンドル

    VECTOR m_pos;          // 弾の位置
    VECTOR m_vel;          // 弾の速度
    int    m_reflect;      // 弾の反射回数
    bool   m_alive;        // 生存フラグ
    bool   m_trailGrowing; // 軌道生成フラグ

    // 壁との当たり判定
    void CheckWallCollision();
    bool CheckHitWall(VECTOR& outNormal);

    // 反射処理
    void Reflect(const VECTOR& normal);

    // 弾の軌道履歴
    std::vector<VECTOR> m_trail;

    // 軌道線の残る長さ
    static constexpr int MaxTrailPoints = 60;

    // 当たり判定用（壁）
    Object* object = nullptr;
};
