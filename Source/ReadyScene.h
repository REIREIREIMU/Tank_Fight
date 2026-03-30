#pragma once
#include "../Library/SceneBase.h"

class Player;

class ReadyScene : public SceneBase
{
public:
	ReadyScene();
	ReadyScene(int lives);
	~ReadyScene();
	void Update() override;
	void Draw() override;

	void SetLives(int lives);

private:
	Player* player = nullptr;		// Playerのポインタを作成

	int image_;
	int timer;
	int lives;  // 表示用残機
};
