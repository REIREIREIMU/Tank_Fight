#include "ClearScene.h"
#include "Player.h"

ClearScene::ClearScene() 
{
	Player::ResetLives(2);
	Player::ResetStage();

	image_ = LoadGraph("Assets/CLEAR.png");
}

ClearScene::~ClearScene()
{
}

void ClearScene::Update()
{
	if (CheckHitKey(KEY_INPUT_RETURN)) {
		SceneManager::ChangeScene("TITLE");
	}
}

void ClearScene::Draw()
{
	int screenW, screenH;
	GetDrawScreenSize(&screenW, &screenH);

	DrawExtendGraph(0, 0, screenW, screenH, image_, TRUE);

	SetFontSize(60);
	DrawString(screenW / 2 - 120, screenH / 2 - 240, "åãâ î≠ï\", GetColor(0, 0, 0));

	SetFontSize(45);
	DrawFormatString(
		screenW / 2 - 30,
		screenH / 2,
		GetColor(0, 0, 0),
		"Å~ %d",
		Player::GetTotalEnemyKill()
	);

}
