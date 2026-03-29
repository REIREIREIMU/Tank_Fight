#include "Bullet.h"
#include "Object.h"
#include "Player.h"
#include "Enemy.h"
#include "Config.h"
#include <cmath>

Bullet::Bullet
(const VECTOR& startPos, const VECTOR& dir, Object* obj, Player* ownerPlayer, std::vector<Enemy*>* enemyList)
    : m_pos(startPos),
    m_alive(true),
    m_trailGrowing(true),
    object(obj), 
    owner(ownerPlayer),
    enemies(enemyList),
    m_reflect(2),
    Bullet_m_handle(-1)
{
    // 3Dモデルの読み込み
    Bullet_m_handle = MV1LoadModel("Assets/Bullet.mv1");

    m_pos          = startPos;
    m_vel          = VScale(VNorm(dir), 0.015f);
    m_alive        = true;
    m_trailGrowing = true;

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
    if (m_alive) {
        // 移動
        m_pos = VAdd(m_pos, m_vel);

        // プレイヤーとの当たり判定
        if(owner && owner->IsAlive())
        {
            VECTOR p = owner->GetPosition();

            if (fabs(m_pos.x - p.x) < (Config::Bullet_Half + Config::Player_Half) &&
                fabs(m_pos.z - p.z) < (Config::Bullet_Half + Config::Player_Half))
            {
                // プレイヤー消滅
                owner->IsDead();

                // 弾も消す
                m_alive = false;
                return;
            }
        }

        // 敵との当たり判定
        if (enemies)
        {
            for (Enemy* e : *enemies)
            {
                if (!e || !e->IsAlive()) continue;

                VECTOR ep = e->GetPosition();
                if (fabs(m_pos.x - ep.x) < (Config::Bullet_Half + Config::Enemy_Half) &&
                    fabs(m_pos.z - ep.z) < (Config::Bullet_Half + Config::Enemy_Half))
                {
                    e->IsDead();     // 敵死亡
                    m_alive = false; // 弾消滅
                    return;
                }
            }
        }

        // 弾が生きている間は軌道生成
        if (m_trailGrowing) {
            // 軌道を保存
            m_trail.push_back(m_pos);

            // 軌道線の後ろが徐々に消える
            if ((int)m_trail.size() > MaxTrailPoints) {
                m_trail.erase(m_trail.begin());
            }
        }

        // 壁との当たり判定
        CheckWallCollision();

        // 向き計算
        float yaw = atan2f(-m_vel.x, -m_vel.z);
        VECTOR rot = VGet(0.0f, yaw, 0.0f);

        // 弾の位置更新
        MV1SetPosition(Bullet_m_handle, m_pos);
        MV1SetRotationXYZ(Bullet_m_handle, rot);
    }
    else{
        // 軌道を生成はしない
        m_trailGrowing = false;

        // 軌道線の後ろが徐々に消える
        if (!m_trail.empty()) {
            m_trail.erase(m_trail.begin());
        }
    }
}

void Bullet::Draw()
{
    if (m_alive)
    {  
        // モデルの表示
        MV1DrawModel(Bullet_m_handle);
    }

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

    // X方向
    if (object->CheckHit(nextPos.x, m_pos.z, Config::Bullet_Half))
    {
        VECTOR normal = VGet(
            (m_vel.x > 0.0f) ? -1.0f : 1.0f,
            0.0f,
            0.0f
        );
        Reflect(normal);
        return;
    }

    // Z方向
    if (object->CheckHit(m_pos.x, nextPos.z, Config::Bullet_Half))
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

void Bullet::Reflect(const VECTOR& normal)
{
    // 反射が終えたら消える
    if (m_reflect <= 0)
    {
        m_alive = false;
        return;
    }

    VECTOR n = VNorm(normal);

    float dot = VDot(m_vel, n);
    m_vel = VSub(m_vel, VScale(n, 2.0f * dot));

    // めり込み防止
    m_pos = VAdd(m_pos, VScale(m_vel, 0.1f));

    m_reflect--;
}