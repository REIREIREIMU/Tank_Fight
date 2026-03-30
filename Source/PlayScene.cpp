#include "PlayScene.h"
#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include "Object.h"
#include "Config.h"

static const int DEATH_WAIT = 120;

// 円同士の衝突を解決
static void ResolveCollision(
    VECTOR& posA, float radiusA,
    VECTOR& posB, float radiusB
)
{
    VECTOR delta = VSub(posA, posB);

    float dist = sqrtf(delta.x * delta.x + delta.z * delta.z);
    float minDist = radiusA + radiusB;

    if (dist >= minDist || dist < 0.0001f)
        return;

    float overlap = minDist - dist;
    VECTOR pushDir = VNorm(delta);

    posA = VAdd(posA, VScale(pushDir, overlap * 0.5f));
    posB = VSub(posB, VScale(pushDir, overlap * 0.5f));
}

PlayScene::PlayScene():timer(0)
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
   //if (CheckHitKey(KEY_INPUT_G)) {
   //    SceneManager::ChangeScene("GAME OVER");
   //    return;
   //}
   //else if (CheckHitKey(KEY_INPUT_C)) {
   //    SceneManager::ChangeScene("CLEAR");
   //    return;
   //}

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

   // プレイヤー死亡後の処理
   if (!player->IsAlive())
   {
       if (timer < 0){
           timer = 0;
       }
       else
       {
           timer++;
           if (timer >= DEATH_WAIT)
           {  
               // 残機を1減らす
               player->Player::DecreaseLives();

               if (player->Player::GetLives() > 0)
               {
                   SceneManager::ChangeScene("READY");
               }
               else
               {
                   SceneManager::ChangeScene("CLEAR");
               }
               return;
           }
       }
   }
}

void PlayScene::Draw()
{
    // 画面クリア
    ClearDrawScreen();

    if (!camera || !player || !object) return;

    // グリット線表示(デバック用)
    /*
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

   //DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
   //DrawString(100, 400, "Push [G]Key To GameOverScene", GetColor(255, 255, 255));

   //DrawString(100, 500, "Push [C]Key To Clear", GetColor(255, 255, 255));
   */

   // 3D描画描画時
    {
        SetUseZBuffer3D(TRUE);
        SetWriteZBuffer3D(TRUE);

        player->Draw();        // プレイヤーのモデルを表示
        player->DrawBullets(); // プレイヤーの弾
        for (auto e : enemies) {
            e->Draw();          // 敵のモデルを表示
            e->DrawBullets();   // 敵の弾
        }
        object->Draw();  // ブロック系統のモデルを表示
    }

    // UI描画
    {
        SetUseZBuffer3D(FALSE);
        SetWriteZBuffer3D(FALSE);

        // UI用カメラ
        SetCameraPositionAndTarget_UpVecY(
            VGet(0.0f, 0.0f, 0.0f),
            VGet(0.0f, 0.0f, 0.0f)
        );

        SetFontSize(30);
        DrawFormatString(
            20, 20,
            GetColor(255, 255, 255),
            "残機：%d",
            player->Player::GetLives()
        );
    }
}