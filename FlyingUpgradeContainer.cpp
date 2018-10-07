#include "stdafx.h"
#include "TextureManager.h"
#include "FlyingUpgradeContainer.h"


FlyingUpgradeContainer::FlyingUpgradeContainer(Point2f pos, bool isMachineGun)
	: m_IsMachineGun{ isMachineGun }
	, m_spContainerTex{ TextureManager::GetInstance().GetUpgrade()}
	, m_IsActive{ false }
	, m_IsAlive{ true }
	, m_Modifier{1}
{
	m_Shape.bottom = pos.y;
	m_Shape.left = pos.x;
	m_Shape.width = m_spContainerTex->GetWidth() / 20;
	m_Shape.height = m_spContainerTex->GetHeight() / 2;

	m_MaxHeight = pos.y + 25;
	m_MinHeight = pos.y - 25;
}

void FlyingUpgradeContainer::Draw() const
{
	if (m_IsActive)
	{
		Rectf source{ 0,0, m_Shape.width * 2, m_Shape.height * 2 };
		m_spContainerTex->Draw(m_Shape, source);
	}
}
void FlyingUpgradeContainer::Update(float elapsedSec, Rectf camera)
{
	if (m_IsActive)
	{

		m_Shape.left += m_SpeedX * elapsedSec;
		m_Shape.bottom += m_Speedy * elapsedSec * m_Modifier;

		if (m_Shape.bottom >= m_MaxHeight || m_Shape.bottom - m_Shape.height <= m_MinHeight)
		{
			m_Modifier *= -1;
		}

	
		if (m_Shape.left > camera.left + camera.width)
		{
			m_IsAlive = false;
		}
	}
}
void FlyingUpgradeContainer::Hit()
{
	m_IsAlive = false;
}
void FlyingUpgradeContainer::Activate()
{
	m_IsActive = true;
}
Rectf FlyingUpgradeContainer::GetShape()
{
	return m_Shape;
}
Point2f FlyingUpgradeContainer::GetPos()
{
	Point2f p{ m_Shape.left, m_Shape.bottom };
	return p;
}
bool FlyingUpgradeContainer::IsAlive()
{
	return m_IsAlive;
}
bool FlyingUpgradeContainer::IsActive()
{
	return m_IsActive;
}
bool FlyingUpgradeContainer::IsMachineGun()
{
	return m_IsMachineGun;
}