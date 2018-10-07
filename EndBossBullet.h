#pragma once
#include "Texture.h"
#include "Vector2f.h"
#include "Avatar.h"
#include "Hud.h"
#include <memory>
class TextureManager;
class EndBossBullet
{
public:
	EndBossBullet(Point2f pos);
	void Draw();
	void Update(float elapsedsec);
	bool GetActive();
	void Collision(Rectf & cam, std::unique_ptr<Hud> & hud, std::unique_ptr<Avatar>& avatar, bool invincible);
private:
	std::shared_ptr<Texture> m_spBullet;
	Rectf m_Shape;
	Vector2f m_velocity;
	const float m_HorSpeed{ 110.0f };
	bool m_IsActive;
};

