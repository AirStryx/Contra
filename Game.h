#pragma once
#include "Avatar.h"
#include "level.h"
#include "Camera.h"
#include "Bullet.h"
#include "Hud.h"
#include "Enemies.h"
#include "Sniper.h"
#include "Turret.h"
#include "Tower.h"
#include "Runner.h"
#include "Upgrade.h"
#include "UpgradeContainer.h"
#include "FlyingUpgradeContainer.h"
#include "SoundStream.h"
#include "SoundEffect.h"
#include "EndBoss.h"
#include "EndBossBullet.h"
#include "DrawFont.h"
class TextureManager;
class SoundManager;
class Game
{
public:
	explicit Game( const Window& window );
	Game( const Game& other ) = delete;
	Game& operator=( const Game& other ) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( );

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	Window m_Window;

	std::unique_ptr<Avatar> m_upAvatar;
	std::unique_ptr<Level> m_upLevel;
	std::unique_ptr<Camera> m_upCamera;
	std::unique_ptr<EndBoss> m_upEndBoss;
	std::unique_ptr<Hud> m_upHud;
	std::vector<std::shared_ptr<Bullet>> m_spBullets;
	std::vector<std::shared_ptr<EndBossBullet>> m_spEndBossBullets;
	std::vector<std::shared_ptr<Enemies>> m_spEnemies;
	std::vector<std::shared_ptr<Upgrade>> m_spUpgrades;
	std::vector<std::shared_ptr<UpgradeContainer>> m_spUpgradeContainers;
	std::vector<std::shared_ptr<FlyingUpgradeContainer>> m_spFlyingUpgradeContainers;
	std::vector<std::shared_ptr<DrawFont>> m_spDeathScreenText;
	std::vector<std::shared_ptr<DrawFont>> m_spVictoryScreenText;

	bool m_Invincible;
	bool m_ShowDeathText;
	bool m_FirstFlyActivated;
	bool m_SecondThirdFlyActivated;
	bool m_PlayedVictory;
	bool m_AvatarVisible;
	bool m_BulletPause;
	bool m_ShowVictoryScreen;
	
	float m_DeathTextTick;
	float m_AccDeathText;
	float m_AccInvinceTime;
	float m_AccRunnerSpawn;
	float m_AccVictory;
	float m_bulletAccTime;
	float m_AccBlink;

	const float m_MaxInvinceTime{ 2.0f };
	const float m_BulletPauseTime{ 0.2f };
	const float m_CamWidth{256.0f};
	const float m_CamHeight{224.0f};
	const float m_BossStart{ 3100.0f };
	const float m_TotRunnerSpawn{ 3.0f };
	const float m_VictoryTime{ 2.0f };
	const float m_FirstFlyActivate{ 384.0f };
	const float m_SecondThirdFlyActivate{2453.0f};
	const float m_TotBlink{ 0.2f };

	// FUNCTIONS
	void Initialize( );
	void InitEnemies();
	void InitPowerUps();
	void Cleanup( );
	void ClearBackground( );
	void Shoot(float elapsedSec);
	void ButtonDetection();
	void HandleEnemies(float elapsedSec);
	void HandleBullets(float elapsedSec);
	void HandleEndBossBullets(float elapsedSec);
	void HandleEndBoss(float elapsedSec);
	void HandleUpgrades(float elapsedSec);
	void HandleUpgradeContainers(float elapsedSec);
	void SpawnRunners();
	void StartBoss();
	void Reset();
};