#pragma once

#include "../Library/GameObject.h"

class Enemy : public GameObject
{
public:
    // ‰ŠúˆÊ’u‚ğó‚¯æ‚ê‚é‚æ‚¤‚É‚·‚é
    Enemy(const VECTOR& spawnPos);
    ~Enemy();

    void Update() override;
    void Draw() override;

    VECTOR GetPosition() const { return Position; }

private:
    int Body_m_handle;   // Ô‘Ìƒ‚ƒfƒ‹
    int Head_m_handle;   // –C“ƒƒ‚ƒfƒ‹

    float pos_x;
    float pos_y1;        // Ô‘ÌY‰ñ“]
    float pos_y2;        // –C“ƒY‰ñ“]
    float pos_z;

    VECTOR Position;
    VECTOR Body_Rotation;
    VECTOR Head_Rotation;
};
