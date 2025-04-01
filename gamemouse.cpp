//===============================================
// �Q�[���}�E�X����[gamemouse.cpp]
// Date:2024/02/02	Auther:�쑺�I�l
//===============================================
#include "memleakcheck.h" // ���������[�N�`�F�b�N�p�̃w�b�_�[���ŏ��ɃC���N���[�h

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
	// TPS���_���ȊO�̎��ɕ\��
	if (GetCamera()->state != STATE_TPS || m_flag) 
	{
		DrawSprite(m_tex, m_pos.x, m_pos.y, m_size.x, m_size.y, 1.0f, 1.0f, 1.0f, 1.0f, m_color);
	}
}
