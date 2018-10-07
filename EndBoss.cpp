#include "stdafx.h"
#include "EndBoss.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include <time.h>


EndBoss::EndBoss(Point2f pos)
	: m_Pos{pos}
	, m_spBossBuilding{TextureManager::GetInstance().GetEndBossBuilding()}
	, m_spBossDoor{ TextureManager::GetInstance().GetEndBossDoor()}
	, m_spBossGuns{TextureManager::GetInstance().GetEndBossGuns()}
	, m_Frame{0}
	, m_alive{true}
	, m_Destroyed{false}
	, m_Count{0}
	, m_LeftGunAlive{true}
	, m_RightGunAlive{ true }
	, m_AccShotLeft{ 1.0f }
	, m_AccShotRight{ 0.0f }
	, m_AccAnimLeft{0.0f}
	, m_AccAnimRight{0.0f}
	, m_DoorLives{40}
	, m_LeftGunLives{10}
	, m_RightGunLives{10}
	, m_Detected{false}
	, m_DeathPlayed{false}
{
	srand(unsigned int(time(NULL)));
	m_RectBuilding.bottom = m_Pos.y;
	m_RectBuilding.left = m_Pos.x;
	m_RectBuilding.width = m_spBossBuilding->GetWidth();
	m_RectBuilding.height = m_spBossBuilding->GetHeight();

	m_RectDoor.bottom = m_Pos.y + 30;
	m_RectDoor.left = m_Pos.x + 6;
	m_RectDoor.width = m_spBossDoor->GetWidth() / 3;
	m_RectDoor.height = m_spBossDoor->GetHeight();

	m_RectLeftGun.bottom = m_Pos.y + 97;
	m_RectLeftGun.left = m_Pos.x + 2;
	m_RectLeftGun.width = m_spBossGuns->GetWidth();
	m_RectLeftGun.height = m_spBossGuns->GetHeight() / 2;

	m_RectRightGun.bottom = m_Pos.y + 97;
	m_RectRightGun.left = m_Pos.x + 25;
	m_RectRightGun.width = m_spBossGuns->GetWidth();
	m_RectRightGun.height = m_spBossGuns->GetHeight() / 2;

	m_SrcDoor.bottom = 0.0f;
	m_SrcDoor.left = 0.0f;
	m_SrcDoor.width = m_RectDoor.width;
	m_SrcDoor.height = m_RectDoor.height;

	m_SrcBuilding.bottom = -1.0f;
	m_SrcBuilding.left = 1.0f;
	m_SrcBuilding.width = m_RectBuilding.width;
	m_SrcBuilding.height = m_RectBuilding.height - 2;

	m_SrcLeftGun.bottom = 0.0f;
	m_SrcLeftGun.left = 0.0f;
	m_SrcLeftGun.width = m_RectLeftGun.width;
	m_SrcLeftGun.height = m_RectLeftGun.height;

	m_SrcRightGun.bottom = m_RectLeftGun.height;
	m_SrcRightGun.left = 0.0f;
	m_SrcRightGun.width = m_RectLeftGun.width;
	m_SrcRightGun.height = m_RectLeftGun.height;

	
	m_RemovePixels = m_spBossBuilding->GetWidth() / 10;
}
void EndBoss::Draw()
{
	if (!m_Destroyed)
	{
		m_spBossBuilding->Draw(m_RectBuilding, m_SrcBuilding);
	}
	if (m_alive)
	{
		m_spBossDoor->Draw(m_RectDoor, m_SrcDoor);
	}
	if (m_LeftGunAlive && m_alive)
	{
		m_spBossGuns->Draw(m_RectLeftGun, m_SrcLeftGun);
	}
	if (m_RightGunAlive&& m_alive)
	{
		m_spBossGuns->Draw(m_RectRightGun, m_SrcRightGun);
	}
}
void EndBoss::Update(float elapsedSec)
{
	if (m_Detected)
	{
		m_AccBreath += elapsedSec;
		if (m_LeftGunAlive && m_alive)
		{
			m_AccShotLeft += elapsedSec * ((rand() % 4 + 1) / 2.0f);//randomizing shot timing a bit
		}
		if (m_RightGunAlive && m_alive)
		{
			m_AccShotRight += elapsedSec * ((rand() % 4 + 1) / 2.0f);//randomizing shot timing a bit
		}

		if (m_AccBreath >= m_Breath)
		{
			m_AccBreath = 0.0f;
			++m_Frame;
			if (m_Frame == m_TotFrames)
			{
				m_Frame = 0;
			}
		}
		m_SrcDoor.left = m_RectDoor.width * m_Frame;

		if (!m_alive && !m_Destroyed)
		{
			m_accDestroy += elapsedSec;

			if (m_accDestroy >= m_DestroySec)
			{
				m_accDestroy = 0.0f;

				m_SrcBuilding.width -= m_RemovePixels;
				m_SrcBuilding.left += m_RemovePixels;
				m_RectBuilding.width -= m_RemovePixels;
				m_RectBuilding.left += m_RemovePixels;

				++m_Count;
				if (m_Count == 10)
				{
					m_Destroyed = true;
				}
			}
		}

		// checking if guns are ready to fire
		if (m_AccShotLeft >= m_ShootCooldown)
		{
			m_AccShotLeft = 0.0f;
			m_LeftGunCanShoot = true;
		}
		if (m_AccShotRight >= m_ShootCooldown)
		{
			m_AccShotRight = 0.0f;
			m_RightGunCanShoot = true;
		}

		// starting the stretching
		if (m_LeftGunCanShoot)
		{
			m_LeftGunInAnim = true;
			m_AccAnimLeft = 0.0f;
		}
		if (m_RightGunCanShoot)
		{
			m_RightGunInAnim = true;
			m_AccAnimRight = 0.0f;
		}

		// stretching guns when they shoot
		if (m_LeftGunInAnim)
		{
			m_RectLeftGun.width = m_spBossGuns->GetWidth() + 4;
			m_RectLeftGun.left = m_Pos.x - 2;
			m_AccAnimLeft += elapsedSec;
			if (m_AccAnimLeft >= m_AnimTime)
			{
				m_LeftGunInAnim = false;
				m_RectLeftGun.width = m_spBossGuns->GetWidth();
				m_RectLeftGun.left = m_Pos.x + 2;
			}
		}
		if (m_RightGunInAnim)
		{
			m_RectRightGun.width = m_spBossGuns->GetWidth() + 4;
			m_RectRightGun.left = m_Pos.x + 21;
			m_AccAnimRight += elapsedSec;
			if (m_AccAnimRight >= m_AnimTime)
			{
				m_RightGunInAnim = false;
				m_RectRightGun.width = m_spBossGuns->GetWidth();
				m_RectRightGun.left = m_Pos.x + 25;
			}
		}
		if (m_DoorLives <= 0)
		{
			m_alive = false;
			if (!m_DeathPlayed)
			{
				SoundManager::GetInstance().PlayEndBossDeath();
				m_DeathPlayed = true;
			}
			
		}
		if (m_LeftGunLives <= 0)
		{
			m_LeftGunAlive = false;
		}
		if (m_RightGunLives <= 0)
		{
			m_RightGunAlive = false;
		}
	}
}
void EndBoss::setDeath()
{
	m_alive = false;
}
Rectf EndBoss::getDoor()
{
	return m_RectDoor;
}
Rectf EndBoss::getLeftGun()
{
	return m_RectLeftGun;
}
Rectf EndBoss::getRightGun()
{
	return m_RectRightGun;
}
bool EndBoss::ShootUpdateLeft()
{
		if (m_LeftGunCanShoot)
		{
			m_LeftGunCanShoot = false;
			return true;
		}
		else
		{
			return false;
		}
	return false;
}
bool EndBoss::ShootUpdateRight()
{
		if (m_RightGunCanShoot)
		{
			m_RightGunCanShoot = false;
			return true;
		}
		else
		{
			return false;
		}
}
EndBossBullet EndBoss::ShootLeft()
{
	return EndBossBullet(Point2f(m_RectLeftGun.left, m_RectLeftGun.bottom));
}
EndBossBullet EndBoss::ShootRight()
{
	return EndBossBullet(Point2f(m_RectRightGun.left, m_RectRightGun.bottom));
}
void EndBoss::HitDoor(int hit)
{
	m_DoorLives -= hit;
	SoundManager::GetInstance().PlayDamage();
}
void EndBoss::HitLeftGun(int hit)
{
	m_LeftGunLives -= hit;
	SoundManager::GetInstance().PlayDamage();
}
void EndBoss::HitRightGun(int hit)
{
	m_RightGunLives -= hit;
	SoundManager::GetInstance().PlayDamage();
}
bool EndBoss::DoorAlive()
{
	return m_alive;
}
bool EndBoss::LeftGunAlive()
{
	return m_LeftGunAlive;
}
bool EndBoss::RightGunAlive()
{
	return m_RightGunAlive;
}

void EndBoss::Detect()
{
	m_Detected = true;
}
bool EndBoss::IsDetecting()
{
	return m_Detected;
}
void EndBoss::Reset()
{
	m_alive = true;
	m_LeftGunAlive = true;
	m_RightGunAlive = true;
	m_Detected = false;
	m_DoorLives = 60;
	m_LeftGunLives = 20;
	m_RightGunLives = 20;
}
bool EndBoss::Destroyed()
{
	return m_Destroyed;
}