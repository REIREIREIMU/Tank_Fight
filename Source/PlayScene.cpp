#include "PlayScene.h"
#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include "Object.h"
#include "Config.h"

// 円同士の衝突を解決（X-Z 平面）
static void ResolveCollision(
    VECTOR& posA, float radiusA,
    VECTOR& posB, float radiusB
)
{
    VECTOR delta = VSub(posA, posB);

    float dist = sqrtf(delta.x * delta.x + delta.z * delta.z);
    float minDist = radiusA + radiusB;

    // 重なっていない or 距離計算不能
    if (dist >= minDist || dist < 0.0001f)
        return;

    float overlap = minDist - dist;
    VECTOR pushDir = VNorm(delta);

    // 半分ずつ押し戻す
    posA = VAdd(posA, VScale(pushDir, overlap * 0.5f));
    posB = VSub(posB, VScale(pushDir, overlap * 0.5f));
}

PlayScene::PlayScene()
{
    camera = new Camera();                  //cameraの初期化
    object = new Object();                  //objectの初期化
    player = new Player(object, &enemies);  //playerの初期化

    // Enemy生成
    object->GetEnemySpawnPos(enemies, player);
}

PlayScene::~PlayScene()
{
    camera = nullptr;
    player = nullptr;
    //e  = nullptr;
    object = nullptr;
}

void PlayScene::Update()
{
   if (CheckHitKey(KEY_INPUT_G)) {
       SceneManager::ChangeScene("GAME OVER");
       return;
   }
   else if (CheckHitKey(KEY_INPUT_C)) {
       SceneManager::ChangeScene("CLEAR");
       return;
   }

   // 各自Update
   camera->Update();
   player->Update();
   for (auto e : enemies)
       e->Update();
   object->Update();

   // 衝突処理（プレイヤーと敵）
   for (Enemy* e : enemies)
   {
       if (!e || !e->IsAlive()) continue;

       VECTOR pPos = player->GetPosition();
       VECTOR ePos = e->GetPosition();

       ResolveCollision(
           pPos, Config::Player_Half,
           ePos, Config::Enemy_Half
       );

       player->SetPosition(pPos);
       e->SetPosition(ePos);
   }

   // 敵と敵
   for (size_t i = 0; i < enemies.size(); i++)
   {
       for (size_t j = i + 1; j < enemies.size(); j++)
       {
           Enemy* a = enemies[i];
           Enemy* b = enemies[j];

           if (!a || !b) continue;
           if (!a->IsAlive() || !b->IsAlive()) continue;

           VECTOR posA = a->GetPosition();
           VECTOR posB = b->GetPosition();

           ResolveCollision(
               posA, Config::Enemy_Half,
               posB, Config::Enemy_Half
           );

           a->SetPosition(posA);
           b->SetPosition(posB);
       }
   }
}

void PlayScene::Draw()
{
    if (!camera || !player || !object) return;

    // グリット線表示(デバック用)
    for (int i = -Grid_Half; i <= Grid_Half; i++)
    {
        float pos = i * Grid_Size;

        // Z方向に線表示
        DrawLine3D(
            VGet(pos, 0.0f, -Grid_Half * Grid_Size),
            VGet(pos, 0.0f, Grid_Half * Grid_Size),
            // X軸の原点のみ「赤」
            (i == 0) ? xAxisColor : GridColor
        );

        // X方向に線表示
        DrawLine3D(
            VGet(-Grid_Half * Grid_Size, 0.0f, pos),
            VGet( Grid_Half * Grid_Size, 0.0f, pos),
            // Z軸の原点のみ「青」
            (i == 0) ? zAxisColor : GridColor
        );
    }

   DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
   DrawString(100, 400, "Push [G]Key To GameOverScene", GetColor(255, 255, 255));

   DrawString(100, 500, "Push [C]Key To Clear", GetColor(255, 255, 255));

   player->Draw();  // プレイヤーのモデルを表示
   for (auto e : enemies)
       e->Draw();   // 敵のモデルを表示
   object->Draw();  // ブロック系統のモデルを表示
}