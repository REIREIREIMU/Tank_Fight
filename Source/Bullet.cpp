#include "Bullet.h"

Bullet::Bullet()
{
    m_pos = VGet(0, 0, 0);
    m_vel = VGet(0, 0, 0);
    m_radius = 5.0f;
    m_reflect = 0;
    m_alive = false;
}

void Bullet::Update()
{
    if (!m_alive) return;

    // ˆÚ“®
    m_pos = VAdd(m_pos, m_vel);

    // •Ç‚Æ‚Ì“–‚½‚è”»’è
    VECTOR normal;
    if (CheckHitWall(normal))
    {
        Reflect(normal);
    }
}

void Bullet::Draw()
{
    if (!m_alive) return;

    printfDx("Bullet Draw Pos:(%.2f, %.2f, %.2f)\n",
        m_pos.x, m_pos.y, m_pos.z);

    // ‰¼‚ÅŠÛ
    DrawSphere3D(
        m_pos,
        //m_radius,
        10,
        8,
        GetColor(255, 255, 0),
        GetColor(255, 255, 0),
        TRUE
    );
}

void Bullet::Fire(const VECTOR& startPos, float angle)
{
    m_pos = startPos;

    // X-Z•½–Ê•ûŒü
    VECTOR dir = VGet(
        cosf(angle),
        0.0f,
        sinf(angle)
    );

    m_vel = VScale(dir, 10.0f); // ’e‘¬
    m_radius = 5.0f;
    m_reflect = 2;              // ”½ŽË2‰ñ
    m_alive = true;
}

bool Bullet::CheckHitWall(VECTOR& outNormal)
{
    // X•ûŒü‚Ì•Ç
    if (m_pos.x < 0.0f)
    {
        outNormal = VGet(1.0f, 0.0f, 0.0f);
        return true;
    }
    if (m_pos.x > 1000.0f)
    {
        outNormal = VGet(-1.0f, 0.0f, 0.0f);
        return true;
    }
    // Z•ûŒü‚Ì•Ç
    if (m_pos.z < 0.0f)
    {
        outNormal = VGet(0.0f, 0.0f, 1.0f);
        return true;
    }
    if (m_pos.z > 1000.0f)
    {
        outNormal = VGet(0.0f, 0.0f, -1.0f);
        return true;
    }

    return false;
}

void Bullet::Reflect(const VECTOR& normal)
{
    if (m_reflect <= 0)
    {
        m_alive = false;
        return;
    }

    VECTOR n = VNorm(normal);

    // R = V - 2(VEN)N
    float dot = VDot(m_vel, n);
    m_vel = VSub(m_vel, VScale(n, 2.0f * dot));

    // ‚ß‚èž‚Ý–hŽ~
    m_pos = VAdd(m_pos, VScale(m_vel, 0.1f));
    
    m_reflect--;
}
