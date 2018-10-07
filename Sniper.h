#pragma once
#include "Enemies.h"
#include "Texture.h"
class TextureManager;
class Sniper : public Enemies
{
public:
	Sniper(Point2f pos);
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
		up,
		straight,
		down
	};
private:
	std::shared_ptr<Texture> m_spSniperTex;
	Aim m_Aim;
	Point2f m_Middle;
	bool m_IsBack;

	bool m_IsActive;
	Rectf m_Range;

	bool m_CanShoot;
	float m_AccShoot;
	const float m_ShootTime{ 0.2f };
	const float m_PauseTime{ 1.5f };
	int m_BurstCount;
	bool m_InBurst;
	const int m_BeginLives{ 1 };
	int m_Lives;

	void Detect(Rectf avatarShape);
	DeathExplosion m_DeathExp;
};

