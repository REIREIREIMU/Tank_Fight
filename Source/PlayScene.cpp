#include "PlayScene.h"

PlayScene::PlayScene()
{
    camera = new Camera();  //cameraの初期化 
    player = new Player();  //playerの初期化
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
}

void PlayScene::Draw()
{
   DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
   DrawString(100, 400, "Push [G]Key To GameOverScene", GetColor(255, 255, 255));

   DrawString(100, 500, "Push [C]Key To Clear", GetColor(255, 255, 255));

   player->Draw();  // プレイヤーのモデルを表示
}
