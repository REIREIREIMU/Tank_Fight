#pragma once
#include "../Library/GameObject.h"
#include <cmath>

class Object;
class Player;

class Enemy : public GameObject
{
public:
    Enemy(const VECTOR& spawnPos, Object* object, Player* player);
    ~Enemy();

    void Update() override;
    void Draw() override;

    bool IsAlive() const { return m_alive; }
    VECTOR GetPosition() const { return Position; }
    void Kill();

private:
    bool m_alive;

    // ƒ‚ƒfƒ‹
    int Body_m_handle;
    int Head_m_handle;

    // ˆÊ’uE‰ñ“]
    float pos_x;
    float pos_z;
    float body_y; // Ô‘Ì‰ñ“]
    float head_y; // –C“ƒ‰ñ“]

    VECTOR Position;
    VECTOR Body_Rotation;
    VECTOR Head_Rotation;

    Object* object;
    Player* player;

    static constexpr float Speed = 0.01f;
    static constexpr float Enemy_Half = 0.4f;
};