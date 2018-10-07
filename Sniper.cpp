#include "stdafx.h"
#include "Sniper.h"
#include "Vector2f.h"
#include "TextureManager.h"
#include "utils.h"
#include <cmath>


Sniper::Sniper(Point2f pos)
	: Enemies(m_BeginLives, pos)
	, m_spSniperTex{TextureManager::GetInstance().GetEnemySniper()}
	, m_AccShoot{ 0.0f }
	, m_CanShoot{ false }
	, m_IsActive{false}
	, m_InBurst{ true }
	, m_BurstCount{ 0 }
	, m_Lives{ m_BeginLives }
{
	m_Shape.bottom = pos.y;
	m_Shape.left = pos.x;
	m_Shape.height = m_spSniperTex->GetHeight() / 2;
	m_Shape.width = m_spSniperTex->GetWidth() / 3;

	m_Middle.x = m_Shape.left + m_Shape.width / 2;
	m_Middle.y = m_Shape.bottom + m_Shape.height / 2;

	m_Range.bottom = m_Shape.bottom - 300;
	m_Range.left = m_Shape.left - 150;
	m_Range.width = m_Shape.width + 300;
	m_Range.height = m_Shape.height + 600;
}
void Sniper::Draw() const
{
	if (m_IsAlive)
	{
		Rectf source{ 0.0f,0.0f, m_Shape.width, m_Shape.height };
		if (m_IsBack)
		{
			source.bottom = 0.0f;
		}
		else
		{
			source.bottom = m_Shape.height;
		}

		switch (m_Aim)
		{
		case Sniper::up:
			source.left = m_Shape.width * 2;
			m_spSniperTex->Draw(m_Shape, source);
			break;
		case Sniper::straight:
			source.left = m_Shape.width * 1;
			m_spSniperTex->Draw(m_Shape, source);
			break;
		case Sniper::down:
			source.left = 0;
			m_spSniperTex->Draw(m_Shape, source);
			break;
		default:
			break;
		}
	}
	m_DeathExp.Draw();
}
void Sniper::Update(float elapsedSec, Rectf avatarShape)
{
	if (m_IsAlive)
	{
		Detect(avatarShape);
		Point2f avatarMid{ avatarShape.left + avatarShape.width / 2, avatarShape.bottom + avatarShape.height / 2 };
		Vector2f vec{ m_Middle, avatarMid };
		Vector2f normal = vec.Normalized();

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

		if (avatarMid.x < m_Middle.x)
		{
			if (abs(normal.x) < 0.80f && normal.y > 0)
			{
				m_Aim = Aim::up;
			}
			else if (abs(normal.x) < 0.80f && normal.y < 0)
			{
				m_Aim = Aim::down;
			}
			else
			{
				m_Aim = Aim::straight;
			}
			m_IsBack = true;
		}
		else
		{
			if (normal.x < 0.80f && normal.y > 0)
			{
				m_Aim = Aim::up;
			}
			else if (normal.x < 0.80f && normal.y < 0)
			{
				m_Aim = Aim::down;
			}
			else
			{
				m_Aim = Aim::straight;
			}
			m_IsBack = false;
		}
	}

	m_DeathExp.Update(elapsedSec);
}
bool Sniper::ShootUpdate()
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
Bullet Sniper::Shoot()
{
	if (m_IsAlive)
	{
		if (m_IsBack)
		{
			switch (m_Aim)
			{
			case Sniper::up:
			{
				Bullet bullet(Point2f(m_Shape.left, m_Shape.bottom + m_Shape.height), Vector2f(-0.5f, 0.5f), true);
				return bullet;
			}
			break;
			case Sniper::straight:
			{
				Bullet bullet(Point2f(m_Shape.left, m_Shape.bottom + m_Shape.height / 4 * 3), Vector2f(-1.0f, 0.0f), true);
				return bullet;
			}
			break;
			case Sniper::down:
			{
				Bullet bullet(Point2f(m_Shape.left, m_Shape.bottom), Vector2f(-0.5f, -0.5f), true);
				return bullet;
			}
			break;
			}
		}
		else
		{
			switch (m_Aim)
			{
			case Sniper::up:
			{
				Bullet bullet(Point2f(m_Shape.left + m_Shape.width, m_Shape.bottom + m_Shape.height), Vector2f(0.5f, 0.5f), true);
				return bullet;
			}
			break;
			case Sniper::straight:
			{
				Bullet bullet(Point2f(m_Shape.left + m_Shape.width, m_Shape.bottom + m_Shape.height / 4 * 3), Vector2f(1.0f, 0.0f), true);
				return bullet;
			}
			break;
			case Sniper::down:
			{
				Bullet bullet(Point2f(m_Shape.left + m_Shape.width, m_Shape.bottom), Vector2f(0.5f, -0.5f), true);
				return bullet;
			}
			break;
			}
		}
	}
	return Bullet(Point2f(0.0f, 0.0f), Vector2f(0.0f, 0.0f), true);
}
void Sniper::Detect(Rectf shape)
{
	if (utils::IsOverlapping(shape, m_Range))
	{
		m_IsActive = true;
	}
	else
	{
		m_IsActive = false;
	}
}
void Sniper::Hit(int damage)
{
	m_Lives -= damage;

	if (m_Lives <= 0)
	{
		m_IsAlive = false;
		m_DeathExp.SetPos(Point2f(m_Shape.left, m_Shape.bottom));
		m_DeathExp.Activate();
	}
}
Rectf Sniper::GetShape()
{
	return m_Shape;
}
bool Sniper::IsAlive()
{
	return m_IsAlive;
}
bool Sniper::IsRunner()
{
	return false;
}
void Sniper::TurnAround()
{
	//nothing needed for sniper, here for virtual function
}
void Sniper::TouchGround()
{
	//nothing needed for sniper, here for virtual function
}
void Sniper::Jump()
{
	//nothing needed for sniper, here for virtual function
}
bool Sniper::HasTouchedGround()
{
	return true;
}
bool Sniper::GetSpawnCheck()
{
	return true;
}
void Sniper::Checked()
{
	//nothing needed for sniper, here for virtual function
}
void Sniper::ResetHeight()
{
	//nothing needed for sniper, here for virtual function
}
float Sniper::GetVerticalVel()
{
	return 0.0f;
}