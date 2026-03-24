#include "Object.h"

Object::Object() :Block_m_handle(-1),
pos_x(0.0f), pos_y(0.0f), pos_z(0.0f)
{
	// 3Dモデルの読み込み
	Block_m_handle  = MV1LoadModel("Assets/Block.mv1");
	//Ground_m_handle = MV1LoadModel("Assets/Ground.mv1");

	// モデルの位置を設定する
	MV1SetPosition(Block_m_handle,  VGet(pos_x, pos_y, pos_z));	      // 位置座標
	//MV1SetPosition(Ground_m_handle, VGet(pos_x , pos_y, pos_z));	  // 位置座標
}

Object::~Object()
{
	// モデルの削除
	MV1DeleteModel(Block_m_handle);
	MV1DeleteModel(Ground_m_handle);
}

void Object::Update()
{
}

void Object::Draw()
{
	// モデルの表示
	MV1DrawModel(Block_m_handle);
	//MV1DrawModel(Ground_m_handle);
}
