#include "PlayScene.h"

PlayScene::PlayScene()
{
    camera = new Camera();  //cameraの初期化 
    player = new Player();  //playerの初期化
    object = new Object();  //objectの初期化
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
   if (CheckHitKey(KEY_INPUT_G)) {
       SceneManager::ChangeScene("GAME OVER");
   }
   else if (CheckHitKey(KEY_INPUT_C)) {
       SceneManager::ChangeScene("CLEAR");
   }

   // 各自Update
   camera->Update();
   player->Update();
   object->Update();
}

void PlayScene::Draw()
{
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
   object->Draw();  // ブロック系統のモデルを表示
}
