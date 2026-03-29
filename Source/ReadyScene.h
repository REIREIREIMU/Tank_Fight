#pragma once
#include "../Library/SceneBase.h"

class ReadyScene : public SceneBase
{
public:
	ReadyScene();
	~ReadyScene();
	void Update() override;
	void Draw() override;
};
