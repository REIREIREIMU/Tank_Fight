#include "TitleScene.h"
#include "Player.h"
#include <DxLib.h>

TitleScene::TitleScene()
{
	image_ = LoadGraph("Assets/TITLE.png");
}

TitleScene::~TitleScene() {}

void TitleScene::Update()
{
	if (CheckHitKey(KEY_INPUT_SPACE)) {
		Player::ResetLives(2);    // プレイヤーの残機表示ををリセット
		Player::ResetStage();     // ステージの状態をリセット
		Player::ResetEnemyKill(); // キル数リセット
		SceneManager::ChangeScene("READY");
	}
}

// タイトルの画像を表示
void TitleScene::Draw()
{
	int screenW, screenH;
	GetDrawScreenSize(&screenW, &screenH);
	DrawExtendGraph(0, 0, screenW, screenH, image_, TRUE);
}
