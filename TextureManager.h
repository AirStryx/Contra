#pragma once
#include "Texture.h"
#include <memory>
using namespace std;
class TextureManager
{
public:
	static TextureManager & GetInstance();

	shared_ptr<Texture>& GetBridge01();
	shared_ptr<Texture>& GetBridge02();
	shared_ptr<Texture>& GetBridge03();
	shared_ptr<Texture>& GetBullet();
	shared_ptr<Texture>& GetBullet2();
	shared_ptr<Texture>& GetCannon();
	shared_ptr<Texture>& GetEnemySniper();
	shared_ptr<Texture>& GetEnemyWalking();
	shared_ptr<Texture>& GetBridgeExplosion();
	shared_ptr<Texture>& GetDeathExplosion();
	shared_ptr<Texture>& GetLives();
	shared_ptr<Texture>& GetAvatar();
	shared_ptr<Texture>& GetPowerUpContainer();
	shared_ptr<Texture>& GetTurret();
	shared_ptr<Texture>& GetUpgrade();
	shared_ptr<Texture>& GetLevel();
	shared_ptr<Texture>& GetEndBossBuilding();
	shared_ptr<Texture>& GetEndBossDoor();
	shared_ptr<Texture>& GetEndBossGuns();
	shared_ptr<Texture>& GetFont();
	shared_ptr<Texture>& GetTransparentPlayer();
private:
	//Textures
	shared_ptr<Texture> m_spBridge01;
	shared_ptr<Texture> m_spBridge02;
	shared_ptr<Texture> m_spBridge03;
	shared_ptr<Texture> m_spBullet;
	shared_ptr<Texture> m_spBullet2;
	shared_ptr<Texture> m_spCannon;
	shared_ptr<Texture> m_spEnemySniper;
	shared_ptr<Texture> m_spEnemyWalking;
	shared_ptr<Texture> m_spBridgeExplosion;
	shared_ptr<Texture> m_spDeathExplosion;
	shared_ptr<Texture> m_spLives;
	shared_ptr<Texture> m_spAvatar;
	shared_ptr<Texture> m_spPowerUpContainer;
	shared_ptr<Texture> m_spTurret;
	shared_ptr<Texture> m_spUpgrade;
	shared_ptr<Texture> m_spLevel;
	shared_ptr<Texture> m_spEndBossBuilding;
	shared_ptr<Texture> m_spEndBossDoor;
	shared_ptr<Texture> m_spEndBossGuns;
	shared_ptr<Texture> m_spFont;
	shared_ptr<Texture> m_spTransparentPlayer;
	//constructor
	TextureManager();
};

