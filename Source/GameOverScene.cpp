#include "GameOverScene.h"
#include <DxLib.h>

GameOverScene::GameOverScene()
{
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Update()
{
	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}

}

void GameOverScene::Draw()
{
	DrawString(0, 0, "GAME OVER SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}
