#pragma once
#include "Vector2f.h"
#include "Texture.h"
#include "Avatar.h"
#include "Hud.h"
#include "SoundEffect.h"

class Enemies;
class UpgradeContainer;
class FlyingUpgradeContainer;
class EndBoss;
class TextureManager;
class SoundManager;
class Bullet
{
public:
	Bullet(Point2f pos,  const Vector2f& aim, bool fromEnemy, bool machineGun = false, bool spreadGun = false);
	Bullet(const Bullet & bullet);
	Bullet operator=(const Bullet& bullet);
	void Update(float elapsedsec);
	void Draw();
	void Collision(Rectf & cam, std::unique_ptr<Hud> & hud, std::unique_ptr<Avatar>& avatar, std::vector<std::shared_ptr<Enemies>>& enemies, std::vector<std::shared_ptr<UpgradeContainer>>& upgradeContainers, std::vector<std::shared_ptr<FlyingUpgradeContainer>>& flyingUpgradeContainers, std::unique_ptr<EndBoss>& endBoss, bool avatarInv = false);
	bool GetActive();
private:

	std::shared_ptr<Texture> m_spBulletTexture;
	bool m_Active;
	Vector2f m_Velocity;
	Rectf m_Shape;
	float m_Speed;
	bool m_IncreaseDmg;

	bool m_FromEnemy;
};

