#include "stdafx.h"
#include "TextureManager.h"


TextureManager::TextureManager()
	: m_spBridge01{make_shared<Texture>("resources/Images/Bridge01.png")}
	, m_spBridge02{ make_shared<Texture>("resources/Images/Bridge02.png")}
	, m_spBridge03{ make_shared<Texture>("resources/Images/Bridge03.png")}
	, m_spBullet{ make_shared<Texture>("resources/Images/Bullet.png")}
	, m_spBullet2{ make_shared<Texture>("resources/Images/Bullet2.png")}
	, m_spCannon{ make_shared<Texture>("resources/Images/cannon.png")}
	, m_spEnemySniper{ make_shared<Texture>("resources/Images/EnemySniper.png")}
	, m_spEnemyWalking{ make_shared<Texture>("resources/Images/EnemyWalking.png")}
	, m_spBridgeExplosion{ make_shared<Texture>("resources/Images/Explosion.png")}
	, m_spDeathExplosion{ make_shared<Texture>("resources/Images/Explosion2.png")}
	, m_spLives{ make_shared<Texture>("resources/Images/Lives.png")}
	, m_spAvatar{ make_shared<Texture>("resources/Images/Player.png")}
	, m_spPowerUpContainer{ make_shared<Texture>("resources/Images/PowerUpBox.png")}
	, m_spTurret{ make_shared<Texture>("resources/Images/Turret01.png")}
	, m_spUpgrade{ make_shared<Texture>("resources/Images/Upgrade.png")}
	, m_spLevel{ make_shared<Texture>("resources/Images/Background1.png")}
	, m_spEndBossBuilding{ make_shared<Texture>("resources/Images/EndBossBuilding.png") }
	, m_spEndBossDoor{ make_shared<Texture>("resources/Images/EndBossDoor.png") }
	, m_spEndBossGuns{ make_shared<Texture>("resources/Images/EndBossGuns.png") }
	, m_spFont{ make_shared<Texture>("resources/Images/Font.png") }
	, m_spTransparentPlayer{ make_shared<Texture>("resources/Images/PlayerTransparent.png") }
{
}

TextureManager & TextureManager::GetInstance()
{
	static TextureManager* pInstance{ new TextureManager() };
	return *pInstance;
}
shared_ptr<Texture>& TextureManager::GetBridge01()
{
	return m_spBridge01;
}
shared_ptr<Texture>& TextureManager::GetBridge02()
{
	return m_spBridge02;
}
shared_ptr<Texture>& TextureManager::GetBridge03()
{
	return m_spBridge03;
}
shared_ptr<Texture>& TextureManager::GetBullet()
{
	return m_spBullet;
}
shared_ptr<Texture>& TextureManager::GetBullet2()
{
	return m_spBullet2;
}
shared_ptr<Texture>& TextureManager::GetCannon()
{
	return m_spCannon;
}
shared_ptr<Texture>& TextureManager::GetEnemySniper()
{
	return m_spEnemySniper;
}
shared_ptr<Texture>& TextureManager::GetEnemyWalking()
{
	return m_spEnemyWalking;
}
shared_ptr<Texture>& TextureManager::GetBridgeExplosion()
{
	return m_spBridgeExplosion;
}
shared_ptr<Texture>& TextureManager::GetDeathExplosion()
{
	return m_spDeathExplosion;
}
shared_ptr<Texture>& TextureManager::GetLives()
{
	return m_spLives;
}
shared_ptr<Texture>& TextureManager::GetAvatar()
{
	return m_spAvatar;
}
shared_ptr<Texture>& TextureManager::GetPowerUpContainer()
{
	return m_spPowerUpContainer;
}
shared_ptr<Texture>& TextureManager::GetTurret()
{
	return m_spTurret;
}
shared_ptr<Texture>& TextureManager::GetUpgrade()
{
	return m_spUpgrade;
}
shared_ptr<Texture>& TextureManager::GetLevel()
{
	return m_spLevel;
}
shared_ptr<Texture>& TextureManager::GetEndBossBuilding()
{
	return m_spEndBossBuilding;
}
shared_ptr<Texture>& TextureManager::GetEndBossDoor()
{
	return m_spEndBossDoor;
}
shared_ptr<Texture>& TextureManager::GetEndBossGuns()
{
	return m_spEndBossGuns;
}
shared_ptr<Texture>& TextureManager::GetFont()
{
	return m_spFont;
}
shared_ptr<Texture>& TextureManager::GetTransparentPlayer()
{
	return m_spTransparentPlayer;
}