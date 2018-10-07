#include "stdafx.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "UpgradeContainer.h"


UpgradeContainer::UpgradeContainer(Point2f pos, bool isMachineGun)
	: m_IsMachineGun{isMachineGun}
	, m_spContainerTex{TextureManager::GetInstance().GetPowerUpContainer()}
	, m_IsActive{ false }
	, m_Frame{ 0 }
	, m_AccBreath{ 0.0f }
	, m_Lives{ m_BeginLives }
	, m_IsAlive{true}
{

		m_Shape.bottom = pos.y;
		m_Shape.left = pos.x;
		m_Shape.width = m_spContainerTex->GetWidth() / 5;
		m_Shape.height = m_spContainerTex->GetHeight();

		m_Range.bottom = m_Shape.bottom - 115;
		m_Range.left = m_Shape.left - 115;
		m_Range.width = m_Shape.width + 230;
		m_Range.height = m_Shape.height + 230;
}

void UpgradeContainer::Draw() const
{
	Rectf source{ m_Shape.width * m_Frame, 0, m_Shape.width, m_Shape.height };
	m_spContainerTex->Draw(m_Shape, source);
}
void UpgradeContainer::Update(float elapsedSec, Rectf avatarShape)
{
	if (m_IsAlive)
	{
		Detect(avatarShape);
		if (m_IsActive)
		{
			m_AccBreath += elapsedSec;
			if (m_AccBreath >= m_Breath)
			{
				++m_Frame;
				m_AccBreath = 0.0f;
				if (m_Frame == m_TotFrames)
				{
					m_Frame = 2;
				}
			}
		}
		else
		{
			m_Frame = 0;
		}
	}
}
void UpgradeContainer::Hit(int damage)
{
	m_Lives -= damage;

	if (m_Lives <= 0)
	{
		m_IsAlive = false;
	}
	else
	{
		SoundManager::GetInstance().PlayDamage();
	}
}
Rectf UpgradeContainer::GetShape()
{
	return m_Shape;
}
Point2f UpgradeContainer::GetPos()
{
	Point2f pos;
	pos.x = m_Shape.left;
	pos.y = m_Shape.bottom;
	return pos;
}
bool UpgradeContainer::IsAlive()
{
	return m_IsAlive;
}
bool UpgradeContainer::IsMachineGun()
{
	return m_IsMachineGun;
}
void UpgradeContainer::Detect(Rectf shape)
{
	if (m_IsAlive)
	{
		if (utils::IsOverlapping(shape, m_Range))
		{
			m_IsActive = true;
		}
		else
		{
			m_IsActive = false;
			m_AccBreath = 0.0f;
		}

	}
}