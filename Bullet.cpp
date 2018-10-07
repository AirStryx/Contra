#include "stdafx.h"
#include "Bullet.h"
#include "Enemies.h"
#include "TextureManager.h"
#include "UpgradeContainer.h"
#include "FlyingUpgradeContainer.h"
#include "SoundManager.h"
#include "EndBoss.h"

Bullet::Bullet(Point2f pos, const Vector2f& aim, bool fromEnemy, bool machineGun, bool spreadGun)
	: m_Shape{ pos.x, pos.y, 0.0f, 0.0f }
	, m_FromEnemy{fromEnemy}
	, m_Active{true}
{
	if (machineGun)
	{
		m_Speed = 250.0f;
		m_spBulletTexture = TextureManager::GetInstance().GetBullet();
		SoundManager::GetInstance().PlayMachineGun();
	}
	else if(spreadGun)
	{
		m_Speed = 250.0f;
		m_spBulletTexture = TextureManager::GetInstance().GetBullet();
		m_IncreaseDmg = true;
	}
	else
	{
		m_Speed = 200.0f;
		m_spBulletTexture = TextureManager::GetInstance().GetBullet2();
		m_IncreaseDmg = false;
		SoundManager::GetInstance().PlayShoot();
	}
	m_Shape.width = 4.0f;
	m_Shape.height = 5.0f;
	m_Velocity.x = m_Speed * aim.x;
	m_Velocity.y = m_Speed * aim.y;

}
Bullet::Bullet(const Bullet & bullet)
	:m_Shape{bullet.m_Shape}
	,m_spBulletTexture{ bullet.m_spBulletTexture}
	,m_Active{ bullet.m_Active }
	,m_Velocity{ bullet.m_Velocity }
	,m_FromEnemy{bullet.m_FromEnemy}
{

}

Bullet Bullet::operator=(const Bullet& bullet)
{
	m_Shape = bullet.m_Shape;
	m_Active = bullet.m_Active;
	m_Velocity = bullet.m_Velocity;
	m_FromEnemy = bullet.m_FromEnemy;
	m_spBulletTexture = bullet.m_spBulletTexture;
	return bullet;
}
void Bullet::Update(float elapsedsec)
{
	m_Shape.left += m_Velocity.x * elapsedsec;
	m_Shape.bottom += m_Velocity.y* elapsedsec;
}
void Bullet::Draw()
{
	m_spBulletTexture->Draw(m_Shape);
}
void Bullet::Collision(Rectf& cam, std::unique_ptr<Hud>& hud, std::unique_ptr<Avatar>& avatar, std::vector<std::shared_ptr<Enemies>>& enemies, std::vector<std::shared_ptr<UpgradeContainer>>& upgradeContainers, std::vector<std::shared_ptr<FlyingUpgradeContainer>>& flyingUpgradeContainers, std::unique_ptr<EndBoss>& endBoss, bool avatarInv)
{
	if (m_Shape.left >= cam.left + cam.width || m_Shape.bottom >= cam.bottom + cam.height || m_Shape.left + m_Shape.width <= cam.left || m_Shape.bottom + m_Shape.height <= cam.bottom)
	{
		m_Active = false;
	}

	if (m_FromEnemy && !avatarInv)
	{
		if (utils::IsOverlapping(avatar->GetHitbox(), m_Shape))
		{
			hud->Damaged();
			avatar->DeactivateGuns();
			m_Active = false;		
		}
	}
	else if(m_FromEnemy == false)
	{
		if (endBoss->DoorAlive())
		{
			if (utils::IsOverlapping(m_Shape, endBoss->getDoor()))
			{
				if (m_IncreaseDmg)
				{
					endBoss->HitDoor(1);
					m_Active = false;
				}
				else
				{
					endBoss->HitDoor(2);
					m_Active = false;
				}
			}
		}
		
		if (endBoss->LeftGunAlive())
		{
			if (utils::IsOverlapping(m_Shape, endBoss->getLeftGun()))
			{
				if (m_IncreaseDmg)
				{
					endBoss->HitLeftGun(1);
					m_Active = false;
				}
				else
				{
					endBoss->HitLeftGun(2);
					m_Active = false;
				}
			}
		}
		
		if (endBoss->RightGunAlive())
		{
			if (utils::IsOverlapping(m_Shape, endBoss->getRightGun()))
			{
				if (m_IncreaseDmg)
				{
					endBoss->HitRightGun(1);
					m_Active = false;
				}
				else
				{
					endBoss->HitRightGun(2);
					m_Active = false;
				}
			}
		}
		
		for (size_t i = 0; i < enemies.size(); i++)
		{
			if (enemies[i]->IsAlive())
			{
				if (utils::IsOverlapping(m_Shape, enemies[i]->GetShape()))
				{
					if (m_IncreaseDmg)
					{
						enemies[i]->Hit(2);
						m_Active = false;
						i = enemies.size() + 1;
					}
					else
					{
						enemies[i]->Hit();
						m_Active = false;
						i = enemies.size() + 1;
					}		
				}
			}
		}
		for (size_t i = 0; i < upgradeContainers.size(); i++)
		{
			if (upgradeContainers[i]->IsAlive())
			{
				if (utils::IsOverlapping(m_Shape, upgradeContainers[i]->GetShape()))
				{
					if (m_IncreaseDmg)
					{
						upgradeContainers[i]->Hit(2);
						m_Active = false;
						i = upgradeContainers.size() + 1;
					}
					else
					{
						upgradeContainers[i]->Hit();
						m_Active = false;
						i = upgradeContainers.size() + 1;
					}
				}
			}
		}
		for (size_t i = 0; i < flyingUpgradeContainers.size(); i++)
		{
			if (flyingUpgradeContainers[i]->IsAlive() && flyingUpgradeContainers[i]->IsActive())
			{
				if (utils::IsOverlapping(m_Shape, flyingUpgradeContainers[i]->GetShape()))
				{
						flyingUpgradeContainers[i]->Hit();
						m_Active = false;
						i = flyingUpgradeContainers.size() + 1;
				}
			}
		}
	}
}
bool Bullet::GetActive()
{
	return m_Active;
}