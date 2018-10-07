#include "stdafx.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Hud.h"


Hud::Hud(const Point2f& botleft, int totalLives)
	: m_BottomLeft{ botleft }
	, m_spLifeTexture{ TextureManager::GetInstance().GetLives()}
	, m_Lives{ totalLives }
	, m_Alive{true}
	, m_Invincible{true}
{

}
void Hud::Draw() const
{
	if (m_Lives >= 0)
	{
	for (int i = 0; i < m_Lives; i++)
	{
		m_spLifeTexture->Draw(Point2f(m_BottomLeft.x + (i * m_spLifeTexture->GetWidth()), m_BottomLeft.y));
	}
	}
}
void Hud::Damaged()
{
	--m_Lives;
	SoundManager::GetInstance().PlayPlayerHit();
	if (m_Lives == 0)
	{
		m_Alive = false;
	}
	m_Invincible = true;
}
bool Hud::GetAlive()
{
	return m_Alive;
}
bool Hud::GetInvincible()
{
	if (m_Invincible)
	{
		m_Invincible = false;
		return true;
	}
	return false;
}
void Hud::Reset()
{
	m_Invincible = false;
	m_Lives = 3;
	m_Alive = true;
}