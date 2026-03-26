#include "Bullet.h"

Bullet::Bullet(const VECTOR& startPos, const VECTOR& dir):
    Bullet_m_handle(-1)
{
    // 3Dモデルの読み込み
    Bullet_m_handle = MV1LoadModel("Assets/Bullet.mv1");

    m_pos     = startPos;
    m_vel     = VScale(VNorm(dir), 0.025f);
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

    // 向き計算
    float yaw  = atan2f(-m_vel.x, -m_vel.z);
    VECTOR rot = VGet(0.0f, yaw, 0.0f);

    // 弾の位置更新
    MV1SetPosition(Bullet_m_handle, m_pos);
    MV1SetRotationXYZ(Bullet_m_handle, rot);

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
    const float MIN_X = 0.0f;
    const float MAX_X = 100.0f;
    const float MIN_Z = 0.0f;
    const float MAX_Z = 100.0f;

    // X方向の壁
    if (m_pos.x < MIN_X)
    {
        outNormal = VGet(1, 0, 0);
        m_pos.x = MIN_X;
        return true;
    }
    if (m_pos.x > MAX_X)
    {
        outNormal = VGet(-1, 0, 0);
        m_pos.x = MAX_X;
        return true;
    }
    // Z方向の壁
    if (m_pos.z < MIN_Z)
    {
        outNormal = VGet(0, 0, 1);
        m_pos.z = MIN_Z;
        return true;
    }
    if (m_pos.z > MAX_Z)
    {
        outNormal = VGet(0, 0, -1);
        m_pos.z = MAX_Z;
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
