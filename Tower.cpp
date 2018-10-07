#include "stdafx.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Tower.h"


Tower::Tower(Point2f pos)
	: Enemies(m_BeginLives, pos)
	, m_spTurretTex{ TextureManager::GetInstance().GetCannon()}
	, m_IsActive{ false }
	, m_IsPowered{ false }
	, m_BootFrame{ 0 }
	, m_AccBoot{ 0.0f }
	, m_AccBreath{ 0.0f }
	, m_AccPowered(0.0f)
	, m_AccShoot{ 0.0f }
	, m_CanShoot{ false }
	, m_InBurst{true}
	, m_BurstCount{0}
	, m_BreathFrame{0}
	, m_Lives{m_BeginLives}
{
	m_Shape.bottom = pos.y;
	m_Shape.left = pos.x;
	m_Shape.height = m_spTurretTex->GetHeight() / 4;
	m_Shape.width = m_spTurretTex->GetWidth() / 3;

	m_Middle.x = m_Shape.left + m_Shape.width / 2;
	m_Middle.y = m_Shape.bottom + m_Shape.height / 2;

	m_Range.bottom = m_Shape.bottom - 100;
	m_Range.left = m_Shape.left - 100;
	m_Range.width = m_Shape.width + 150;
	m_Range.height = m_Shape.height + 200;
}
void Tower::Draw() const
{
	if (m_IsAlive)
	{
		Rectf source{ 0.0f,0.0f, m_Shape.width, m_Shape.height };

		if (m_IsPowered && !m_IsActive)
		{
			source.bottom = source.height * 4;
			source.left = source.width * m_BootFrame;
			m_spTurretTex->Draw(m_Shape, source);
		}
		if (m_IsActive)
		{
			source.left = m_BreathFrame * m_Shape.width;
			switch (m_Aim)
			{
			case Tower::higherUp:
				source.bottom = m_Shape.height * 3;
				m_spTurretTex->Draw(m_Shape, source);
				break;
			case Tower::up:
				source.bottom = m_Shape.height * 2;
				m_spTurretTex->Draw(m_Shape, source);
				break;
			case Tower::straight:
				source.bottom = m_Shape.height;
				m_spTurretTex->Draw(m_Shape, source);
				break;
			default:
				break;
			}
		}
	}
	m_DeathExp.Draw();
}
void Tower::Update(float elapsedSec, Rectf avatarShape)
{
	if (m_IsAlive)
	{
		Detect(avatarShape);

		if (!m_IsActive && m_IsPowered)
		{
			m_AccBoot += elapsedSec;
			if (m_AccBoot >= m_Boot)
			{
				m_AccBoot = 0.0f;
				m_BootFrame += 1;
				if (m_BootFrame == m_TotFrames)
				{
					m_IsActive = true;
				}
			}
		}
		if (!m_CanShoot && m_IsActive)
		{
			m_AccShoot += elapsedSec;
			if (m_AccShoot >= m_ShootTime && m_InBurst == true)
			{
				m_AccShoot = 0.0f;
				m_CanShoot = true;
				++m_BurstCount;
				if (m_BurstCount == 3)
				{
					m_InBurst = false;
					m_BurstCount = 0;
				}
			}
			else
			{
				if (m_AccShoot >= m_PauseTime)
				{
					m_InBurst = true;
					m_AccShoot = 0.0f;
				}
			}
		}
		if (m_IsActive)
		{
			Point2f avatarMid{ avatarShape.left + avatarShape.width / 2, avatarShape.bottom + avatarShape.height / 2 };
			Vector2f vec{ m_Middle, avatarMid };
			Vector2f normal = vec.Normalized();

				if (abs(normal.x) < 0.40f && normal.y > 0)
				{
					m_Aim = Aim::higherUp;
				}
				else if (abs(normal.x) < 0.50f && normal.y > 0)
				{
					m_Aim = Aim::up;
				}
				else
				{
					m_Aim = Aim::straight;
				}
				m_IsBack = false;

				m_AccBreath += elapsedSec;
				if (m_AccBreath >= m_Breath)
				{
					m_AccBreath = 0.0f;
					m_BreathFrame += 1;
					if (m_BreathFrame == m_BreathFrames)
					{
						m_BreathFrame = 0;
					}
				}
				//const float m_Breath{ 0.5f };
				//int m_BreathFrame;
				//const int  m_BreathFrames{ 3 };
			}
		}
	m_DeathExp.Update(elapsedSec);
}
Bullet Tower::Shoot()
{
	switch (m_Aim)
	{
	case Tower::up:
	{
		Bullet bullet(Point2f(m_Shape.left, m_Shape.bottom + m_Shape.height / 4 * 3), Vector2f(-0.5f, 0.5f), true);
		return bullet;
	}
	break;
	case Tower::straight:
	{
		Bullet bullet(Point2f(m_Shape.left, m_Shape.bottom + m_Shape.height / 2), Vector2f(-1.0f, 0.0f), true);
		return bullet;
	}
	break;
	case Tower::higherUp:
	{
		Bullet bullet(Point2f(m_Shape.left + 5, m_Shape.bottom+ m_Shape.height), Vector2f(-1 / 3.0f, 1 / 3.0f * 2.0f), true);
		return bullet;
	}
	break;
	}
	return Bullet(Point2f(0.0f, 0.0f), Vector2f(0.0f, 0.0f), true);
}
bool Tower::ShootUpdate()
{
	if (m_IsAlive)
	{
		if (m_CanShoot)
		{
			m_CanShoot = false;
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}
void Tower::Detect(Rectf shape)
{
	if (m_IsAlive)
	{
		if (utils::IsOverlapping(shape, m_Range))
		{
			m_IsPowered = true;
		}

	}
}
void Tower::Hit(int damage)
{
	m_Lives -= damage;

	if (m_Lives <= 0)
	{
		m_IsAlive = false;
		m_DeathExp.SetPos(Point2f(m_Shape.left, m_Shape.bottom));
		m_DeathExp.Activate();
	}
	else
	{
		SoundManager::GetInstance().PlayDamage();
	}
}
Rectf Tower::GetShape()
{
	return m_Shape;
}
bool Tower::IsAlive()
{
	return m_IsAlive;
}
bool Tower::IsRunner()
{
	return false;
}
void Tower::TurnAround()
{
	//nothing needed for Tower, here for virtual function
}
void Tower::TouchGround()
{
	//nothing needed for Tower, here for virtual function
}
void Tower::Jump()
{
	//nothing needed for Tower, here for virtual function
}
bool Tower::HasTouchedGround()
{
	return true;
}
bool Tower::GetSpawnCheck()
{
	return true;
}
void Tower::Checked()
{
	//nothing needed for Tower, here for virtual function
}
void Tower::ResetHeight()
{
	//nothing needed for Tower, here for virtual function
}
float Tower::GetVerticalVel()
{
	return 0.0f;
}