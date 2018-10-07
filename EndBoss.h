#pragma once
#include <memory>
#include "Texture.h"
#include "EndBossBullet.h"
class TextureManager;
class SoundManager;
class EndBoss
{
public:
	EndBoss(Point2f pos);
	void Draw();
	void Update(float elapsedSec);
	void setDeath();

	Rectf getDoor();
	Rectf getLeftGun();
	Rectf getRightGun();

	bool ShootUpdateLeft();
	bool ShootUpdateRight();

	EndBossBullet ShootLeft();
	EndBossBullet ShootRight();

	void HitDoor(int hit);
	void HitLeftGun(int hit);
	void HitRightGun(int hit);

	bool DoorAlive();
	bool LeftGunAlive();
	bool RightGunAlive();

	void Detect();
	bool IsDetecting();

	void Reset();
	bool Destroyed();
private:
	Point2f m_Pos;
	std::shared_ptr<Texture> m_spBossBuilding;
	std::shared_ptr<Texture> m_spBossDoor;
	std::shared_ptr<Texture> m_spBossGuns;

	Rectf m_RectBuilding;
	Rectf m_RectDoor;
	Rectf m_RectLeftGun;
	Rectf m_RectRightGun;

	Rectf m_SrcDoor;
	Rectf m_SrcBuilding;
	Rectf m_SrcLeftGun;
	Rectf m_SrcRightGun;

	float m_AccBreath;
	const float m_Breath{ 0.5f };
	int m_Frame;
	const int  m_TotFrames{ 3 };

	float m_RemovePixels;
	float m_accDestroy;
	const float m_DestroySec{ 0.4f };
	int m_Count;

	bool m_Destroyed;
	bool m_alive;
	bool m_LeftGunAlive;
	bool m_RightGunAlive;

	bool m_LeftGunCanShoot;
	bool m_RightGunCanShoot;
	bool m_LeftGunInAnim;
	bool m_RightGunInAnim;

	float m_AccShotLeft;
	float m_AccShotRight;
	float m_AccAnimLeft;
	float m_AccAnimRight;
	const float m_ShootCooldown{ 2.0f };
	const float m_AnimTime{ 0.5f };

	int m_DoorLives;
	int m_LeftGunLives;
	int m_RightGunLives;
	bool m_Detected;
	bool m_DeathPlayed;
};

