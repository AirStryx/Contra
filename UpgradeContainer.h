#pragma once
#include <memory>
#include "utils.h"
#include "Texture.h"
#include "SoundEffect.h"
#include "DeathExplosion.h"
class TextureManager;
class SoundManager;
class UpgradeContainer
{
public:
	UpgradeContainer(Point2f pos, bool isMachineGun);
	void Draw() const;
	void Update(float elapsedSec, Rectf avatarShape);
	void Hit(int damage = 1);
	Rectf GetShape();
	Point2f GetPos();
	bool IsAlive();
	bool IsMachineGun();
private:
	Rectf m_Shape;
	bool m_IsMachineGun;

	bool m_IsActive;

	float m_AccBreath;
	const float m_Breath{ 0.5f };
	int m_Frame;
	const int  m_TotFrames{ 5 };

	std::shared_ptr<Texture> m_spContainerTex;
	const int m_BeginLives{ 7 };
	int m_Lives;
	bool m_IsAlive;

	Rectf m_Range;

	void Detect(Rectf shape);
};

