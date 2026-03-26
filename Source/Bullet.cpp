#include "Bullet.h"

Bullet::Bullet(const VECTOR& startPos, const VECTOR& dir):
    Bullet_m_handle(-1)
{
    // 3Dモデルの読み込み
    Bullet_m_handle = MV1LoadModel("Assets/Bullet.mv1");

    m_pos     = startPos;
    m_vel     = VScale(VNorm(dir), 0.005f);
    m_reflect = 2;
    m_alive   = true;

    // 初期位置設定
    MV1SetPosition(Bullet_m_handle, m_pos);

}

Bullet::~Bullet()
{
        MV1DeleteModel(Bullet_m_handle);
}

void Bullet::Update()
{
    if (!m_alive) return;

    // 移動
    m_pos = VAdd(m_pos, m_vel);

    CheckWallCollision();

    // 弾の位置更新
    MV1SetPosition(Bullet_m_handle, m_pos);

}

void Bullet::Draw()
{
    if (!m_alive) return;

    // モデルの表示
    MV1DrawModel(Bullet_m_handle);

    // デバッグ用表示
    {
        printfDx("Bullet Draw Pos:(%.2f, %.2f, %.2f)\n",
            m_pos.x, m_pos.y, m_pos.z);
    }

}

void Bullet::CheckWallCollision()
{
    VECTOR hitNormal;

    if (!CheckHitWall(hitNormal))
        return;

    Reflect(hitNormal);
}

bool Bullet::CheckHitWall(VECTOR& outNormal)
{
    // X方向の壁
    if (m_pos.x < 0.0f)
    {
        outNormal = VGet(1.0f, 0.0f, 0.0f);
        return true;
    }
    if (m_pos.x > 100.0f)
    {
        outNormal = VGet(-1.0f, 0.0f, 0.0f);
        return true;
    }
    return false;
}

void Bullet::Reflect(const VECTOR& normal)
{
    // 反射が終えたら消える
    if (m_reflect <= 0)
    {
        m_alive = false;
        return;
    }

    VECTOR n = VNorm(normal);

    // R = V - 2(V・N)N
    float dot = VDot(m_vel, n);
    m_vel = VSub(m_vel, VScale(n, 2.0f * dot));

    // めり込み防止
    m_pos = VAdd(m_pos, VScale(m_vel, 0.1f));
    
    m_reflect--;
}
