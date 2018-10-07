#include "stdafx.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "DeathExplosion.h"


DeathExplosion::DeathExplosion()
	: m_Active{ false }
	, m_Done{ false }
	, m_spExplosionTex{ TextureManager::GetInstance().GetDeathExplosion()}
	, m_AccFrameTime{ 0.0f }
	, m_Frame{ 0 }
{
	m_Shape.left = m_pos.x;
	m_Shape.bottom = m_pos.y;
	m_Shape.width = m_spExplosionTex->GetWidth() / 3;
	m_Shape.height = m_spExplosionTex->GetHeight();

	m_SourceRect.bottom = 0.0f;
	m_SourceRect.left = 0.0f;
	m_SourceRect.width = m_Shape.width;
	m_SourceRect.height = m_Shape.height;
}

void DeathExplosion::Activate()
{
	m_Active = true;
	SoundManager::GetInstance().PlayDeath();
}
void DeathExplosion::Draw() const
{
	if (m_Active == true && m_Done == false)
	{
		m_spExplosionTex->Draw(m_Shape, m_SourceRect);
	}
}
void DeathExplosion::Update(float elapsedSec)
{
	if (m_Active == true && m_Done == false)
	{
		m_AccFrameTime += elapsedSec;
		if (m_AccFrameTime >= m_FrameTime)
		{
			m_AccFrameTime = 0.0f;
			++m_Frame;
		}
		m_SourceRect.left = m_SourceRect.width * m_Frame;

		if (m_Frame == 3)
		{
			m_Done = true;
		}
	}
}
void DeathExplosion::SetPos(Point2f pos)
{
	m_pos = pos;
	m_Shape.left = pos.x;
	m_Shape.bottom = pos.y;
}