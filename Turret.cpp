#include "stdafx.h"
#include "Turret.h"
#include "Vector2f.h"
#include "utils.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include <cmath>

Turret::Turret(Point2f pos)
	: Enemies(m_BeginLives, pos)
	, m_spTurretTex{ TextureManager::GetInstance().GetTurret()}
	, m_IsActive{false}
	, m_IsPowered{false}
	, m_Frame{0}
	, m_AccBreath{0.0f}
	, m_AccPowered(0.0f)
	, m_AccShoot{0.0f}
	, m_CanShoot{false}
	, m_InBurst{ true }
	, m_BurstCount{ 0 }
	, m_Lives{ m_BeginLives }
{
	m_Shape.bottom = pos.y;
	m_Shape.left = pos.x;
	m_Shape.height = m_spTurretTex->GetHeight() / 4;
	m_Shape.width = m_spTurretTex->GetWidth() / 8;

	m_Middle.x = m_Shape.left + m_Shape.width / 2;
	m_Middle.y = m_Shape.bottom + m_Shape.height / 2;

	m_Range.bottom = m_Shape.bottom - 115;
	m_Range.left = m_Shape.left - 115;
	m_Range.width = m_Shape.width + 230;
	m_Range.height = m_Shape.height + 230;
}


void Turret::Draw() const
{
	if (m_IsAlive)
	{
		Rectf sourceGun{ 0.0f,0.0f, m_Shape.width, m_Shape.height };
		Rectf sourceBody{ 0.0f,0.0f, m_Shape.width, m_Shape.height };
		Rectf sourceShield{ 0.0f, m_Shape.height * 3, m_Shape.width, m_Shape.height };

		sourceBody.left = m_Frame * m_Shape.width;

		if (m_IsPowered)
		{
			sourceBody.bottom = m_Shape.height * 2;
			m_spTurretTex->Draw(m_Shape, sourceBody);
		}
		else
		{
			sourceBody.bottom = m_Shape.height;
			m_spTurretTex->Draw(m_Shape, sourceBody);
		}
		if (m_IsActive)
		{
			m_spTurretTex->Draw(m_Shape, sourceShield);
			switch (m_Aim)
			{
			case Turret::straightUp:
				sourceGun.left = m_Shape.width * 6;
				m_spTurretTex->Draw(m_Shape, sourceGun);
				break;
			case Turret::up:
				sourceGun.left = m_Shape.width * 7;
				m_spTurretTex->Draw(m_Shape, sourceGun);
				break;
			case Turret::straight:
				sourceGun.left = 0;
				m_spTurretTex->Draw(m_Shape, sourceGun);
				break;
			case Turret::down:
				sourceGun.left = m_Shape.width * 1;
				m_spTurretTex->Draw(m_Shape, sourceGun);
				break;
			case Turret::straightDown:
				sourceGun.left = m_Shape.width * 2;
				m_spTurretTex->Draw(m_Shape, sourceGun);
				break;
			case Turret::backDown:
				sourceGun.left = m_Shape.width * 3;
				m_spTurretTex->Draw(m_Shape, sourceGun);
				break;
			case Turret::backStraight:
				sourceGun.left = m_Shape.width * 4;
				m_spTurretTex->Draw(m_Shape, sourceGun);
				break;
			case Turret::backUp:
				sourceGun.left = m_Shape.width * 5;
				m_spTurretTex->Draw(m_Shape, sourceGun);
				break;
			default:
				break;
			}
		}
	}
	m_DeathExp.Draw();
}
void Turret::Update(float elapsedSec, Rectf avatarShape)
{
	if (m_IsAlive)
	{
		Detect(avatarShape);

		if (!m_IsActive)
		{
			m_AccBreath += elapsedSec;
			if (m_AccBreath >= m_Breath)
			{
				m_AccBreath = 0.0f;
				m_Frame += 1;
				if (m_Frame == m_TotFrames)
				{
					m_Frame = 0;
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

		if (m_IsPowered)
		{
			m_AccPowered += elapsedSec;

			if (m_AccPowered >= m_Powered)
			{
				m_IsActive = true;
				m_AccPowered = 0.0f;
			}
		}

		if (m_IsActive)
		{
			Point2f avatarMid{ avatarShape.left + avatarShape.width / 2, avatarShape.bottom + avatarShape.height / 2 };
			Vector2f vec{ m_Middle, avatarMid };
			Vector2f normal = vec.Normalized();

			if (avatarMid.x < m_Middle.x)
			{
				if (abs(normal.x) < 0.15f && normal.y > 0)
				{
					m_Aim = Aim::straightUp;
				}
				else if (abs(normal.x) < 0.15f && normal.y < 0)
				{
					m_Aim = Aim::straightDown;
				}
				else if (abs(normal.x) < 0.80f && normal.y > 0)
				{
					m_Aim = Aim::backUp;
				}
				else if (abs(normal.x) < 0.80f && normal.y < 0)
				{
					m_Aim = Aim::backDown;
				}
				else
				{
					m_Aim = Aim::backStraight;
				}
				m_IsBack = true;
			}
			else
			{
				if (normal.x < 0.15f && normal.y > 0)
				{
					m_Aim = Aim::straightUp;
				}
				else if (normal.x < 0.15f && normal.y < 0)
				{
					m_Aim = Aim::straightDown;
				}
				else if (normal.x < 0.80f && normal.y > 0)
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
	}
	m_DeathExp.Update(elapsedSec);
}
Bullet Turret::Shoot()
{
	switch (m_Aim)
	{
	case Turret::straightUp:{
		Bullet bullet(Point2f(m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height), Vector2f(0.0f, 1.0f), true);
		return bullet;
	}
		break;
	case Turret::up:
	{
		Bullet bullet(Point2f(m_Shape.left + m_Shape.width, m_Shape.bottom + m_Shape.height), Vector2f(0.5f, 0.5f), true);
		return bullet;
	}
		break;
	case Turret::straight:
	{
		Bullet bullet(Point2f(m_Shape.left + m_Shape.width, m_Shape.bottom + m_Shape.height / 2), Vector2f(1.0f, 0.0f), true);
		return bullet;
	}
		break;
	case Turret::down:
	{
		Bullet bullet(Point2f(m_Shape.left + m_Shape.width, m_Shape.bottom), Vector2f(0.5f, -0.5f), true);
		return bullet;
	}
		break;
	case Turret::straightDown:
	{
		Bullet bullet(Point2f(m_Shape.left + m_Shape.width / 2, m_Shape.bottom), Vector2f(0.0f, -1.0f), true);
		return bullet;
	}
		break;
	case Turret::backDown:
	{
		Bullet bullet(Point2f(m_Shape.left, 0), Vector2f(-0.5f, -0.5f), true);
		return bullet;
	}
		break;
	case Turret::backStraight:
	{
		Bullet bullet(Point2f(m_Shape.left, m_Shape.bottom + m_Shape.height/2), Vector2f(-1.0f, 0.0f), true);
		return bullet;
	}
		break;
	case Turret::backUp:
	{
		Bullet bullet(Point2f(m_Shape.left, m_Shape.bottom + m_Shape.height), Vector2f(-0.5f, 0.5f), true);
		return bullet;
	}
		break;
	default:
		return Bullet(Point2f(0.0f, 0.0f), Vector2f(0.0f, 0.0f), true);
		break;
	}
}
bool Turret::ShootUpdate()
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

void Turret::Detect(Rectf shape)
{
	if (m_IsAlive)
	{
		if (utils::IsOverlapping(shape, m_Range))
		{
			m_IsPowered = true;
		}
		else
		{
			m_IsPowered = false;
			m_IsActive = false;
			m_AccPowered = 0.0f;
			m_AccShoot = 0.0f;
		}

	}
}
void Turret::Hit(int damage)
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
Rectf Turret::GetShape()
{
	return m_Shape;
}
bool Turret::IsAlive()
{
	return m_IsAlive;
}
bool Turret::IsRunner()
{
	return false;
}
void Turret::TurnAround()
{
	//nothing needed for Turret, here for virtual function
}
void Turret::TouchGround()
{
	//nothing needed for Turret, here for virtual function
}

void Turret::Jump()
{
	//nothing needed for Turret, here for virtual function
}
bool Turret::HasTouchedGround()
{
	return true;
}
bool Turret::GetSpawnCheck()
{
	return true;
}
void Turret::Checked()
{
	//nothing needed for Turret, here for virtual function
}
void Turret::ResetHeight()
{
	//nothing needed for Turret, here for virtual function
}
float Turret::GetVerticalVel()
{
	return 0.0f;
}