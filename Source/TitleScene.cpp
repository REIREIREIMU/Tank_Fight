#include "TitleScene.h"
#include "Player.h"
#include <DxLib.h>

TitleScene::TitleScene()
{
	image_ = LoadGraph("Assets/TITLE.png");
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update()
{
	if (CheckHitKey(KEY_INPUT_SPACE)) {

		Player::ResetLives(2);
		Player::ResetStage();
		Player::ResetEnemyKill();

		SceneManager::ChangeScene("READY");
	}
}

void TitleScene::Draw()
{
	int screenW, screenH;
	GetDrawScreenSize(&screenW, &screenH);

	DrawExtendGraph( 0, 0, screenW, screenH, image_, TRUE);
}
