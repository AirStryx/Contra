#pragma once
#include "Enemies.h"
#include "Texture.h"
class TextureManager;
class Runner : public Enemies
{
public:
	Runner(Point2f pos);
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

private:
	std::shared_ptr<Texture> m_spRunnerTex;
	Point2f m_Middle;
	bool m_IsBack;

	bool m_IsActive;
	Rectf m_Range;

	const bool m_CanShoot{ false };
	const int m_BeginLives{ 1 };
	int m_Lives;

	Vector2f m_Movement;
	const float m_Gravity{ 150.0f };
	bool m_IsGoingLeft;

	Rectf m_Source;
	float m_accAnim;
	const float m_AnimNextFrame{0.2f};
	int m_Frame;
	const int m_MaxFrame{ 5 };
	bool m_TouchedGround;
	bool m_SpawnCheck;

	void Detect(Rectf avatarShape);
	DeathExplosion m_DeathExp;
};

