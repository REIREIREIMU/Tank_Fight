#include "Bullet.h"
#include "Object.h"

constexpr float Bullet_Half = 0.1f; // Bullet用

Bullet::Bullet
(const VECTOR& startPos, const VECTOR& dir, Object* obj)
    : object(obj), Bullet_m_handle(-1)
{
    // 3Dモデルの読み込み
    Bullet_m_handle = MV1LoadModel("Assets/Bullet.mv1");

    m_pos     = startPos;
    m_vel     = VScale(VNorm(dir), 0.025f);
    m_reflect = 2;
    m_alive   = true;

    // 最初の位置を記録
    m_trail.push_back(m_pos);

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

    // 軌道を保存
    m_trail.push_back(m_pos);

    // 軌道線の後ろが徐々に消える
    if ((int)m_trail.size() > MaxTrailPoints)
    {
        m_trail.erase(m_trail.begin());
    }

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

    // 軌道線の表示
    for (size_t i = 1; i < m_trail.size(); i++)
    {
        DrawLine3D(
            m_trail[i - 1],
            m_trail[i],
            GetColor(255, 255, 255)
        );
    }

    // デバッグ用表示
    {
      /*  printfDx("Bullet Draw Pos:(%.2f, %.2f, %.2f)\n",
            m_pos.x, m_pos.y, m_pos.z);*/

    }

}

void Bullet::CheckWallCollision()
{
    if (!object) return;

    VECTOR nextPos = VAdd(m_pos, m_vel);

    // まず X 方向だけ動かして判定
    if (object->CheckHit(nextPos.x, m_pos.z, Bullet_Half))
    {
        VECTOR normal = VGet(
            (m_vel.x > 0.0f) ? -1.0f : 1.0f,
            0.0f,
            0.0f
        );
        Reflect(normal);
        return;
    }

    // 次に Z 方向
    if (object->CheckHit(m_pos.x, nextPos.z, Bullet_Half))
    {
        VECTOR normal = VGet(
            0.0f,
            0.0f,
            (m_vel.z > 0.0f) ? -1.0f : 1.0f
        );
        Reflect(normal);
        return;
    }
}

bool Bullet::CheckHitWall(VECTOR& outNormal)
{
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
