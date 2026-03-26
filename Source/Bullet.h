#pragma once
#include "../Library/GameObject.h"

class Bullet : public GameObject {
public:
	Bullet(const VECTOR& startPos, const VECTOR& dir);
    ~Bullet();

    void Update()override;
    void Draw() override;

    bool IsAlive() const { return m_alive; }

private:
    int Bullet_m_handle; // 弾のハンドル

    VECTOR m_pos;        // 弾の位置
    VECTOR m_vel;        // 弾の速度
    int    m_reflect;    // 弾の反射回数
    bool   m_alive;      // 生存フラグ

    // 壁との当たり判定
    void CheckWallCollision();
    bool CheckHitWall(VECTOR& outNormal);

    // 反射処理
    void Reflect(const VECTOR& normal);
};
