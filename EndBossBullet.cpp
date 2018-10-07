#include "stdafx.h"
#include "EndBossBullet.h"
#include "TextureManager.h"
#include <ctime>

EndBossBullet::EndBossBullet(Point2f pos)
	:m_spBullet{TextureManager::GetInstance().GetBullet()}
	, m_IsActive{true}
	, m_velocity{0.0f, 0.0f}
{
	//srand(unsigned int(time(NULL)));
	m_Shape.bottom = pos.y;
	m_Shape.left = pos.x;
	m_Shape.width = m_spBullet->GetWidth();
	m_Shape.height = m_spBullet->GetHeight();

	m_velocity.x = rand() % 130 + 30.0f;
}
void EndBossBullet::Draw()
{
	m_spBullet->Draw(m_Shape);
}
void EndBossBullet::Update(float elapsedsec)
{
	m_velocity.y -= m_HorSpeed * elapsedsec;

	m_Shape.left -= m_velocity.x * elapsedsec;
	m_Shape.bottom += m_velocity.y * elapsedsec;
}
bool EndBossBullet::GetActive()
{
	return m_IsActive;
}
void EndBossBullet::Collision(Rectf & cam, std::unique_ptr<Hud> & hud, std::unique_ptr<Avatar>& avatar, bool invincible)
{
	if (m_Shape.left >= cam.left + cam.width || m_Shape.bottom >= cam.bottom + cam.height || m_Shape.left + m_Shape.width <= cam.left || m_Shape.bottom + m_Shape.height <= cam.bottom)
	{
		m_IsActive = false;
	}
	if (!invincible)
	{
		if (utils::IsOverlapping(avatar->GetHitbox(), m_Shape))
		{
			hud->Damaged();
			m_IsActive = false;
		}
	}
	
}