#include "stdafx.h"
#include "Upgrade.h"
#include "TextureManager.h"

Upgrade::Upgrade(Point2f pos, bool isMachine, bool BlastUp)
	:m_IsMachine{isMachine}
	, m_spUpgrade{TextureManager::GetInstance().GetUpgrade()}
	,m_active{true}
{
	m_Source.bottom = 0.0f;
	m_Source.height = m_spUpgrade->GetHeight();
	m_Source.width = m_spUpgrade->GetWidth() / 10;
	
	m_Pos.bottom = pos.y;
	m_Pos.left = pos.x;
	m_Pos.width = m_Source.width / 2;
	m_Pos.height = m_Source.height / 2;

	m_Speed = m_MinSpeed;

	if (isMachine)
	{
		m_Source.left = m_Source.width;
	}
	else
	{
		m_Source.left = m_Source.width * 4;
	}

	if (BlastUp)
	{
		Bump();
	}
}
void Upgrade::Draw()
{
	m_spUpgrade->Draw(m_Pos, m_Source);
}
void Upgrade::Update(std::unique_ptr<Avatar>& avatar, float elapsedSec)
{
	if (m_Speed > m_MinSpeed)
	{
		m_Speed += m_gravity * elapsedSec;
	}
	if (m_Fall)
	{
		m_Pos.bottom += m_Speed * elapsedSec;
	}

	if (utils::IsOverlapping(avatar->GetShape(), m_Pos))
	{
		trigger(avatar);
	}
}
void Upgrade::Bump()
{
	m_Fall = true;
	m_Speed = 130.0f;
}
void Upgrade::StopFall()
{
	m_Fall = false;
}
Rectf Upgrade::GetShape()
{
	return m_Pos;
}
bool Upgrade::GetFall()
{
	return m_Fall;
}
bool Upgrade::GetUpOrNot()
{
	if (m_Speed > 0)
	{
		return true;
	}

	return false;
}
bool Upgrade::GetActive()
{
	return m_active;
}
void Upgrade::trigger(std::unique_ptr<Avatar>& avatar)
{
	if (m_IsMachine)
	{
		avatar->ActivateMachineGun();
	}
	else
	{
		avatar->ActivateSpreadGun();
	}
	m_active = false;
}