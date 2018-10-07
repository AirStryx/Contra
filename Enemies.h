#pragma once
#include "Bullet.h"
#include "DeathExplosion.h"
class Enemies
{
public:
	Enemies(int lives, Point2f pos);
	virtual ~Enemies();
	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec, Rectf avatarShape) = 0;
	virtual bool ShootUpdate() = 0;
	virtual Bullet Shoot() = 0;
	virtual void Hit(int damage = 1) = 0;
	virtual Rectf GetShape() = 0;
	virtual bool IsAlive() = 0;
	virtual float GetVerticalVel() = 0;

	//functions only meant for the runner
	virtual bool IsRunner() = 0;
	virtual void TurnAround() = 0;
	virtual void Jump() = 0;
	virtual void TouchGround() = 0;
	virtual bool HasTouchedGround() = 0;
	virtual bool GetSpawnCheck() = 0;
	virtual void Checked() = 0;
	virtual void ResetHeight() = 0;
protected:
	int m_Lives;
	Point2f m_Pos;
	Rectf m_Shape;
	bool m_IsAlive;
};

