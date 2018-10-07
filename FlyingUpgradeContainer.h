#pragma once
#include <memory>
#include "Texture.h"
class TextureManager;
class FlyingUpgradeContainer
{
public:
	FlyingUpgradeContainer(Point2f pos, bool isMachineGun);
	void Draw() const;
	void Update(float elapsedSec, Rectf avatarShape);
	void Hit();
	void Activate();
	Rectf GetShape();
	Point2f GetPos();

	bool IsAlive();
	bool IsActive();
	bool IsMachineGun();
private:
	Rectf m_Shape;
	bool m_IsMachineGun;

	bool m_IsActive;
	bool m_IsAlive;

	float m_MaxHeight;
	float m_MinHeight;

	const float m_SpeedX{ 50.0f };
	const float m_Speedy{ 100.0f };
	int m_Modifier;

	std::shared_ptr<Texture> m_spContainerTex;
};

