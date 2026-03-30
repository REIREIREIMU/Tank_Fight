#include "ClearScene.h"
#include "Player.h"

ClearScene::ClearScene() 
{
	Player::ResetLives(2);
}

ClearScene::~ClearScene()
{
}

void ClearScene::Update()
{
	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
}

void ClearScene::Draw()
{
	DrawString(0, 0, "CLEAR SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}
