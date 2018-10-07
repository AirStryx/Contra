#pragma once
#include "Enemies.h"
#include "Texture.h"
#include "Bullet.h"
#include "SoundEffect.h"
class TextureManager;
class SoundManager;
class Turret : public Enemies
{
public:
	Turret(Point2f pos);
	void Draw() const;
	void Update(float elapsedSec, Rectf avatarShape);
	bool ShootUpdate();
	Bullet Shoot();
	void Hit(int damage = 1);
	Rectf GetShape();
	bool IsAlive();
	bool IsRunner();
	void TurnAround();
	void Jump();
	void TouchGround();
	bool HasTouchedGround();
	bool GetSpawnCheck();
	void Checked();
	void ResetHeight();
	float GetVerticalVel();
	enum Aim
	{
		straightUp,
		up,
		straight,
		down,
		straightDown,
		backDown,
		backStraight,
		backUp
	};
private:
	bool m_IsPowered;
	bool m_IsActive;

	float m_AccBreath;
	const float m_Breath{ 0.5f };
	int m_Frame;
	const int  m_TotFrames{ 3 };

	float m_AccPowered;
	const float m_Powered{ 0.5f };

	std::shared_ptr<Texture> m_spTurretTex;
	Aim m_Aim;
	Point2f m_Middle;
	bool m_IsBack;
	Rectf m_Range;

	bool m_CanShoot;
	float m_AccShoot;
	const float m_ShootTime{ 0.2f };
	const float m_PauseTime{ 1.5f };
	int m_BurstCount;
	bool m_InBurst;

	const int m_BeginLives{ 7 };
	int m_Lives;

	void Detect(Rectf shape);
	DeathExplosion m_DeathExp;
};

