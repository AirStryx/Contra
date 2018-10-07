#include "stdafx.h"
#include "Runner.h"
#include "TextureManager.h"

Runner::Runner(Point2f pos)
	: Enemies(m_BeginLives, pos)
		, m_spRunnerTex{ TextureManager::GetInstance().GetEnemyWalking() }
		, m_IsActive{ false }
		, m_Lives{ m_BeginLives }
		, m_IsGoingLeft{true}
		, m_TouchedGround{false}
		, m_accAnim{0.0f}
		, m_Frame{0}
		, m_Movement{50.0f, 0.0f}
		, m_SpawnCheck{false }
{
		m_Shape.bottom = pos.y;
		m_Shape.left = pos.x;
		m_Shape.height = m_spRunnerTex->GetHeight() / 2;
		m_Shape.width = m_spRunnerTex->GetWidth() / 6;

		m_Middle.x = m_Shape.left + m_Shape.width / 2;
		m_Middle.y = m_Shape.bottom + m_Shape.height / 2;

		m_Range.bottom = m_Shape.bottom - 300;
		m_Range.left = m_Shape.left - 150;
		m_Range.width = m_Shape.width + 300;
		m_Range.height = m_Shape.height + 600;

		m_Source.left = 0.0f;
		m_Source.bottom = 0.0f;
		m_Source.width = m_spRunnerTex->GetWidth() / 6;
		m_Source.height = m_spRunnerTex->GetHeight() / 2;
}
void Runner::Draw() const
{
	if (m_IsAlive)
	{
		m_spRunnerTex->Draw(m_Shape, m_Source);
	}
	m_DeathExp.Draw();
	
}
void Runner::Update(float elapsedSec, Rectf avatarShape)
{
	m_accAnim += elapsedSec;
	m_DeathExp.Update(elapsedSec);
	Detect(avatarShape);
	if (!m_TouchedGround)
	{
		m_Shape.bottom += m_Movement.y * elapsedSec;

		m_Movement.y -= m_Gravity * elapsedSec;
	}

	if (m_IsActive)
	{
		m_Shape.left -= m_Movement.x * elapsedSec;
		if (!m_TouchedGround)
		{
			if (m_IsGoingLeft)
			{
				m_Source.bottom = 0.0f;
				m_Source.left = 0;			
			}
			else
			{
				m_Source.bottom = m_Source.height;
				m_Source.left = m_Source.width * 5;
			}
		}
		else
		{
			m_Movement.y = 0.0f;
			if (m_accAnim >= m_AnimNextFrame)
			{
				m_accAnim = 0.0f;
				++m_Frame;
				if (m_Frame == m_MaxFrame)
				{
					m_Frame = 0;
				}
			}
			if (m_accAnim >= m_AnimNextFrame)
			{
				m_accAnim = 0.0f;
				++m_Frame;
				if (m_Frame == m_MaxFrame)
				{
					m_Frame = 0;
				}
			}
			if (m_IsGoingLeft)
			{
				m_Source.bottom = 0.0f;
				m_Source.left = m_Source.width + (m_Source.width * m_Frame);
			}
			else
			{
				m_Source.bottom = m_Source.height;
				m_Source.left = m_Source.width * m_Frame;
			}
		}
	}	
}
bool Runner::ShootUpdate()
{
	return false;
}
Bullet Runner::Shoot()
{
	return Bullet(Point2f(0.0f, 0.0f), Vector2f(0.0f, 0.0f), true);
}
void Runner::Hit(int damage)
{
	m_Lives -= damage;

	if (m_Lives <= 0)
	{
		m_IsAlive = false;
		m_DeathExp.SetPos(Point2f(m_Shape.left, m_Shape.bottom));
		m_DeathExp.Activate();
	}
}
Rectf Runner::GetShape()
{
	return m_Shape;
}
bool Runner::IsAlive()
{
	return m_IsAlive;
}
void Runner::Detect(Rectf avatarShape)
{
	if (utils::IsOverlapping(avatarShape, m_Range))
	{
		m_IsActive = true;
	}
}
bool Runner::IsRunner()
{
	return true;
}
void Runner::TurnAround()
{
	if (m_IsGoingLeft)
	{
		m_IsGoingLeft = false;
		m_Shape.left += 5.0f;
	}
	else
	{
		m_IsGoingLeft = true;
		m_Shape.left -= 5.0f;
	}
}
void Runner::Jump()
{
	m_Movement.y = 100.0f;
	m_TouchedGround = false;	
}
void Runner::TouchGround()
{
	m_TouchedGround = true;
	m_Movement.y = 0.0f;
}
bool Runner::HasTouchedGround()
{
	return m_TouchedGround;
}
bool Runner::GetSpawnCheck()
{
	return m_SpawnCheck;
}
void Runner::Checked()
{
	m_SpawnCheck = true;
}
void Runner::ResetHeight()
{
	m_Shape.bottom = rand() % 196 + 30.0f;
}
float Runner::GetVerticalVel()
{
	return m_Movement.y;
}