//===============================================
// ゲームマウス制御[gamemouse.cpp]
// Date:2024/02/02	Auther:野村悠人
//===============================================
#include "memleakcheck.h" // メモリリークチェック用のヘッダーを最初にインクルード

#ifdef _DEBUG
#pragma push_macro("new")
#undef new
#endif

#include "planePolygon.h"

#ifdef _DEBUG
#pragma pop_macro("new")
#endif
#include "gamemouse.h"
#include "Texture.h"
#include "camera.h"

GameMouse::GameMouse()
{
	m_tex = LoadTexture((char*)"data/TEXTURE/Mouse_Cursor.png");
}

GameMouse::~GameMouse()
{

}

void GameMouse::Update()
{
	m_pos.x = GetMousePosX();
	m_pos.y = GetMousePosY();
}

void GameMouse::Draw()
{
	// TPS視点時以外の時に表示
	if (GetCamera()->state != STATE_TPS || m_flag) 
	{
		DrawSprite(m_tex, m_pos.x, m_pos.y, m_size.x, m_size.y, 1.0f, 1.0f, 1.0f, 1.0f, m_color);
	}
}
