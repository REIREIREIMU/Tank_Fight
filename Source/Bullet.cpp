#include "Bullet.h"
#include "Object.h"
#include "Player.h"
#include "Enemy.h"
#include "Config.h"
#include <cmath>

Bullet::Bullet
(const VECTOR& startPos, const VECTOR& dir, Object* obj, Player* ownerPlayer, std::vector<Enemy*>* enemyList) :
    pos(startPos),
    Alive(true),
    TrailGrowing(true),
    object(obj), 
    player(ownerPlayer),
    enemies(enemyList),
    ReflectCount(2),
    Bullet_handle(-1)
{
    // 3Dƒ‚ƒfƒ‹‚Ì“Ç‚İ‚İ
    Bullet_handle = MV1LoadModel("Assets/Bullet.mv1");

    pos          = startPos;
    vel          = VScale(VNorm(dir), 0.015f);
    Alive        = true;
    TrailGrowing = true;

    // Å‰‚ÌˆÊ’u‚ğ‹L˜^
    m_trail.push_back(pos);

    // ‰ŠúˆÊ’uİ’è
    MV1SetPosition(Bullet_handle, pos);
}

Bullet::~Bullet()
{
    MV1DeleteModel(Bullet_handle);
}

void Bullet::Update()
{
    // ’e¶‘¶’†
    if (Alive) {
        Move();                 // ˆÚ“®
        PlayerHit();            // ƒvƒŒƒCƒ„[‚Æ‚Ì“–‚½‚è”»’è
        EnemyHit();             // “G‚Æ‚Ì“–‚½‚è”»’è
        CheckWallCollision();   // •Ç‚Æ‚Ì“–‚½‚èÕ“Ë
        Trail();                // ’e‚Ì‹O“¹¶¬
        Transform();            // ’e‚ÌŒü‚«
    }
    // ’e‚ªÁ–Å
    else{
        // ‹O“¹‚ğ¶¬‚Í‚µ‚È‚¢
        TrailGrowing = false;

        // ‹O“¹ü‚ÌŒã‚ë‚ª™X‚ÉÁ‚¦‚é
        if (!m_trail.empty()) {
            m_trail.erase(m_trail.begin());
        }
    }
}

// ˆÚ“®ˆ—
void Bullet::Move()
{
    pos = VAdd(pos, vel);
}

// ƒvƒŒƒCƒ„[‚Æ‚ÌÕ“Ë”»’è
void Bullet::PlayerHit()
{
    if (player && player->IsAlive()){
        // –³“G’†‚ÍŒø‚©‚È‚¢
        if (Player::IsInvincible()) return;

        VECTOR p = player->GetPosition();

        if (fabs(pos.x - p.x) < (Config::Bullet_Half + Config::Player_Half) &&
            fabs(pos.z - p.z) < (Config::Bullet_Half + Config::Player_Half))
        {
            player->IsDead();   // ƒvƒŒƒCƒ„[Á–Å
            Alive = false;      // ’e‚àÁ‚·
            return;
        }
    }
}

// “G‚Æ‚ÌÕ“Ë”»’è
void Bullet::EnemyHit()
{
    if (enemies){
        for (Enemy* e : *enemies)
        {
            //if (!e || !e->IsAlive()) continue;

            if (e && e->IsAlive()) {
                VECTOR ep = e->GetPosition();

                if (fabs(pos.x - ep.x) < (Config::Bullet_Half + Config::Enemy_Half) &&
                    fabs(pos.z - ep.z) < (Config::Bullet_Half + Config::Enemy_Half))
                {
                    e->IsDead();     // “G€–S
                    Alive = false;   // ’eÁ–Å
                    return;
                }
            }
        }
    }
}

// •ÇÕ“Ë
void Bullet::CheckWallCollision()
{
    if (!object) return;

    VECTOR nextPos = VAdd(pos, vel);

    // X•ûŒü
    if (object->CheckHit(nextPos.x, pos.z, Config::Bullet_Half)) {
        VECTOR normal = VGet((vel.x > 0.0f) ? -1.0f : 1.0f, 0.0f, 0.0f);
        Reflect(normal);
        return;
    }

    // Z•ûŒü
    if (object->CheckHit(pos.x, nextPos.z, Config::Bullet_Half)){
        VECTOR normal = VGet(0.0f, 0.0f, (vel.z > 0.0f) ? -1.0f : 1.0f);
        Reflect(normal);
        return;
    }
}

// ”½Ë
void Bullet::Reflect(const VECTOR& normal)
{
    // ”½Ë‚ªI‚¦‚½‚çÁ‚¦‚é
    if (ReflectCount <= 0) {
        Alive = false; // ’eÁ–Å
        return;
    }

    VECTOR n = VNorm(normal);
    float dot = VDot(vel, n);

    // ”½Ë‚ÌŒvZ
    vel = VSub(vel, VScale(n, 2.0f * dot));

    // ‚ß‚è‚İ–h~
    pos = VAdd(pos, VScale(vel, 0.1f));

    // ‚±‚±‚Å”½Ë‚Ì‰ñ”‚ğŒ¸‚ç‚·
    ReflectCount--;
}

// ’e‚Ì‹O“¹¶¬
void Bullet::Trail()
{
    if (TrailGrowing) {
        // ‹O“¹‚ğ•Û‘¶
        m_trail.push_back(pos);

        // ‹O“¹ü‚ÌŒã‚ë‚ª™X‚ÉÁ‚¦‚é
        if ((int)m_trail.size() > MaxTrailPoints) {
            m_trail.erase(m_trail.begin());
        }
    }
}

// ’e‚ÌŒü‚«
void Bullet::Transform()
{
    // Œü‚«ŒvZ
    float yaw = atan2f(-vel.x, -vel.z);
    VECTOR rot = VGet(0.0f, yaw, 0.0f);

    // ’e‚ÌˆÊ’uXV
    MV1SetPosition(Bullet_handle, pos);
    MV1SetRotationXYZ(Bullet_handle, rot);
}

void Bullet::Draw()
{
    if (Alive)
    {  
        // ƒ‚ƒfƒ‹‚Ì•\¦
        MV1DrawModel(Bullet_handle);
    }

    // ‹O“¹ü‚Ì•\¦
    for (size_t i = 1; i < m_trail.size(); i++)
    {
        DrawLine3D(
            m_trail[i - 1],
            m_trail[i],
            GetColor(255, 255, 255)
        );
    }

    // ƒfƒoƒbƒO—p•\¦
    {
      /*  printfDx("Bullet Draw Pos:(%.2f, %.2f, %.2f)\n",
            m_pos.x, m_pos.y, m_pos.z);*/
    }
}