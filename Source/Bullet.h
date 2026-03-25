#pragma once
#include "../Library/GameObject.h"

class Bullet : public GameObject {
public:
	Bullet();

    void Update()override;
    void Draw() override;

    // 発射
    void Fire(const VECTOR& startPos, float angle);
    // 生存判定
    bool IsAlive() const { return m_alive; }
    // 位置取得
    VECTOR GetPos() const { return m_pos; }

private:
    // 壁との当たり判定
    bool CheckHitWall(VECTOR& outNormal);

    // 反射処理
    void Reflect(const VECTOR& normal);

    VECTOR m_pos;     // 位置
    VECTOR m_vel;     // 速度
    float  m_radius;  // 当たり判定半径
    int    m_reflect; // 残り反射回数
    bool   m_alive;

};
