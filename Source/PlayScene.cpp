#include "PlayScene.h"
#include <DxLib.h>

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
	else if (CheckHitKey(KEY_INPUT_G)) {
		SceneManager::ChangeScene("GAME OVER");
	}
	else if (CheckHitKey(KEY_INPUT_C)) {
		SceneManager::ChangeScene("CLEAR");
	}
}

void PlayScene::Draw()
{
	DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));

	DrawString(100, 500, "Push [G]Key To GameOverScene", GetColor(255, 255, 255));

	DrawString(100, 600, "Push [C]Key To Clear", GetColor(255, 255, 255));
}
