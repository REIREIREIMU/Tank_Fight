//#pragma once
//
//#include "../Library/GameObject.h"
//
//enum class EnemyType
//{
//    Turret,     // ŒÅ’è–C‘äƒ^ƒCƒv
//    Chaser      // ˆê’è‹——£‚ğ•Û‚Á‚Ä’Ç]‚·‚éƒ^ƒCƒv
//};
//
//class Player;
//class Object;
//
//class Enemy : public GameObject
//{
//public:
//    // ‰ŠúˆÊ’u‚ğó‚¯æ‚ê‚é‚æ‚¤‚É‚·‚é
//    Enemy(const VECTOR& spawnPos, EnemyType type,
//        Object* object, Player* player);
//    ~Enemy();
//
//    void Update() override;
//    void Draw() override;
//
//    void UpdateTurret();   // ŒÅ’è–C‘ä
//    void UpdateChaser();   // ’Ç]ƒ^ƒCƒv
//
//    //VECTOR GetPosition() const { return Position; }
//
//private:
//    EnemyType type;
//
//    Player* player;
//    Object* object;
//
//    int Body_m_handle;   // Ô‘Ìƒ‚ƒfƒ‹
//    int Head_m_handle;   // –C“ƒƒ‚ƒfƒ‹
//
//    float pos_x;
//    float pos_y1;        // Ô‘ÌY‰ñ“]
//    float pos_y2;        // –C“ƒY‰ñ“]
//    float pos_z;
//
//    VECTOR Position;
//    VECTOR Body_Rotation;
//    VECTOR Head_Rotation;
//};

#pragma once
#include "../Library/GameObject.h"

enum class EnemyType
{
    Turret,
    Chaser
};

class Player;
class Object;

class Enemy : public GameObject
{
public:
    Enemy(const VECTOR& spawnPos, EnemyType type, Object* object, Player* player);
    ~Enemy();

    void Update() override;
    void Draw() override;

private:
    void UpdateTurret();   // ŒÅ’è–C‘ä
    void UpdateChaser();   // ’Ç]ƒ^ƒCƒv

private:
    EnemyType type;

    Player* player;
    Object* object;

    int Body_m_handle;
    int Head_m_handle;

    VECTOR position;
    float bodyYaw;
    float headYaw;

    float moveSpeed;
};