#pragma once
#include "Texture.h"
#include "SoundEffect.h"
#include <memory>
class TextureManager;
class SoundManager;
class Hud
{
public:
	Hud(const Point2f& topleft, int totalLives);
	void Draw() const;
	void Damaged();
	bool GetAlive();
	bool GetInvincible();
	void Reset();
private:
	Point2f m_BottomLeft;
	int m_Lives;
	std::shared_ptr<Texture> m_spLifeTexture;

	bool m_Alive;
	bool m_Invincible;
};

