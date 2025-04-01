//===============================================
// ÉQÅ[ÉÄÉ}ÉEÉXêßå‰[gamemouse.h]
// Date:2024/02/02	Auther:ñÏë∫óIêl
//===============================================
#pragma once
#include "main.h"

class GameMouse
{
private:
	int m_tex = 0;				//âÊëúäiî[óp
	D3DXCOLOR m_color = { 1.0f,1.0f,1.0f,1.0f };
	D3DXVECTOR2 m_pos = { 0.0f,0.0f };
	D3DXVECTOR2 m_size = { 75.0f,75.0f };
	bool m_flag = false;
public:
	GameMouse();
	~GameMouse();

	void Update();
	void Draw() ;

	void SetPos(D3DXVECTOR2 sPos) { m_pos = sPos; }
	void SetSize(D3DXVECTOR2 sSize) { m_size = sSize; }

	GameMouse* GetGameMouse() { return this; }
	D3DXVECTOR2 GetPos() const { return m_pos; }
	D3DXVECTOR2 GetSize() const { return m_size; }
	void SetPauseFlag(bool s){
		m_flag = s;
	}
};