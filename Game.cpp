#include "stdafx.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Game.h"

Game::Game( const Window& window ) 
	: m_Window{ window }
	, m_upCamera{ std::make_unique<Camera>(256.0f, 224.0f) }
	, m_BulletPause{true}
	, m_upHud {std::make_unique<Hud>(Point2f(6.0f, 200.0f), 3)}
	, m_upAvatar{std::make_unique<Avatar>()}
	, m_upLevel{std::make_unique<Level>()}
	, m_upEndBoss{std::make_unique<EndBoss>(Point2f(3217.0f, 8.0f))}
	, m_ShowDeathText{false}
	, m_AccDeathText{ 0.0f }
	, m_DeathTextTick{0.5f}
	, m_FirstFlyActivated{false}
	, m_SecondThirdFlyActivated{false}
	, m_PlayedVictory{false}
	, m_AccBlink{0.0f}
	, m_AvatarVisible{true}
	, m_ShowVictoryScreen{false}
{
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	SoundManager::GetInstance().PlayJungleTheme();
	m_upCamera->SetBoundaries(m_upLevel->GetBoundaries());
	InitEnemies();
	InitPowerUps();
	
	// texts shown when death or victory
	m_spDeathScreenText.push_back(std::make_shared<DrawFont>(Point2f(220.0f / 3, m_Window.height / 2 / 3), "You died"));
	m_spDeathScreenText.push_back(std::make_shared<DrawFont>(Point2f(50.0f / 3, (m_Window.height / 2 - 40) / 3), "Press 'r' to restart."));
	m_spVictoryScreenText.push_back(std::make_shared<DrawFont>(Point2f(104.0f / 3, m_Window.height / 2 / 3 -2), "Thanks for playing.", 0.5f));
	m_spVictoryScreenText.push_back(std::make_shared<DrawFont>(Point2f(35.0f, (m_Window.height / 2 - 40) / 3), "Made by: Van Gasse Ewout", 0.5f));
	m_spVictoryScreenText.push_back(std::make_shared<DrawFont>(Point2f(106.0f / 3, m_Window.height / 2 / 3 - 25), "From: 1DAE08", 0.5f));
}
void Game::InitEnemies()
{
	//Towers
	m_spEnemies.push_back(std::make_shared<Tower>(Point2f(2047.0f, 64.0f)));
	m_spEnemies.push_back(std::make_shared<Tower>(Point2f(2180.0f, 160.0f)));
	m_spEnemies.push_back(std::make_shared<Tower>(Point2f(2754.0f, 95.0f)));

	//Turrets
	m_spEnemies.push_back(std::make_shared<Turret>(Point2f(1242.0f, 60.0f)));
	m_spEnemies.push_back(std::make_shared<Turret>(Point2f(1608.0f, 86.0f)));
	m_spEnemies.push_back(std::make_shared<Turret>(Point2f(1825.0f, 91.0f)));
	m_spEnemies.push_back(std::make_shared<Turret>(Point2f(2949.0f, 46.0f)));
	m_spEnemies.push_back(std::make_shared<Turret>(Point2f(3087.0f, 30.0f)));

	//Snipers
	m_spEnemies.push_back(std::make_shared<Sniper>(Point2f(300.0f, 30.0f)));
	m_spEnemies.push_back(std::make_shared<Sniper>(Point2f(626.0f, 30.0f)));
	m_spEnemies.push_back(std::make_shared<Sniper>(Point2f(1525.0f, 157.0f)));
	m_spEnemies.push_back(std::make_shared<Sniper>(Point2f(2340.0f, 93.0f)));
	m_spEnemies.push_back(std::make_shared<Sniper>(Point2f(3220.0f, 154.0f)));
}
void Game::InitPowerUps()
{
	//Flying
	m_spFlyingUpgradeContainers.push_back(std::make_shared<FlyingUpgradeContainer>(Point2f(368.0f, 193.0f), true));
	m_spFlyingUpgradeContainers.push_back(std::make_shared<FlyingUpgradeContainer>(Point2f(2418.0f, 185.0f), true));
	m_spFlyingUpgradeContainers.push_back(std::make_shared<FlyingUpgradeContainer>(Point2f(2418.0f, 65.0f), false));

	//Stationary
	m_spUpgradeContainers.push_back(std::make_shared<UpgradeContainer>(Point2f(320.0f, 75.0f), true));
	m_spUpgradeContainers.push_back(std::make_shared<UpgradeContainer>(Point2f(2266.0f, 55.0f), false));
}
void Game::Cleanup( )
{
	delete &TextureManager::GetInstance();
	delete &SoundManager::GetInstance();
}

void Game::Update( float elapsedSec )
{
	//Read Input for volume and reset
	ButtonDetection();

	//Checks if boss can start
	StartBoss();

	//bunch of updates and handling of enemies for detection etc
	if (!m_ShowVictoryScreen)
	{
		m_upCamera->Update(elapsedSec);
		m_upAvatar->Update(elapsedSec, m_upLevel);
		m_upAvatar->Clamp(m_upCamera->GetPos());
		m_upLevel->Update(elapsedSec);
		HandleEndBoss(elapsedSec);
		HandleEndBossBullets(elapsedSec);
		if (m_upHud->GetAlive())
		{
			//player shoot
			Shoot(elapsedSec);
			HandleEnemies(elapsedSec);
			HandleBullets(elapsedSec);
			HandleUpgrades(elapsedSec);
			HandleUpgradeContainers(elapsedSec);

			m_AccRunnerSpawn += elapsedSec;
			if (m_AccRunnerSpawn >= m_TotRunnerSpawn)
			{
				m_AccRunnerSpawn = 0.0f;
				SpawnRunners();
			}
			if (m_upHud->GetInvincible() && m_Invincible == false)
			{
				m_Invincible = true;
			}

			if (m_upLevel->DidFall() == true)
			{
				m_upHud->Damaged();
			}
		}
		else
		{
			m_upAvatar->Died();
		}

		//counting down to end invincibility after dmg, also used for blink
		if (m_Invincible)
		{
			m_AccInvinceTime += elapsedSec;
			m_AccBlink += elapsedSec;
			if (m_AccInvinceTime >= m_MaxInvinceTime)
			{
				m_AccInvinceTime = 0.0f;
				m_Invincible = false;
			}

			if (m_AccBlink >= m_TotBlink)
			{
				m_AccBlink = 0.0f;
				if (m_AvatarVisible)
				{
					m_AvatarVisible = false;
				}
				else
				{
					m_AvatarVisible = true;
				}
			}
		}
		else
		{
			m_AvatarVisible = true;
		}

		//blinking the text on deathscreen
		if (!m_upHud->GetAlive())
		{
			m_AccDeathText += elapsedSec;

			if (m_AccDeathText >= m_DeathTextTick)
			{
				m_AccDeathText = 0.0f;
				if (m_ShowDeathText)
				{
					m_ShowDeathText = false;
					m_DeathTextTick = 0.5f;
				}
				else
				{
					m_ShowDeathText = true;
					m_DeathTextTick = 1.0f;
				}
			}
		}
	}
	
	//Play victory music and end all sounds
	if (m_upEndBoss->Destroyed() && !m_PlayedVictory)
	{
		m_AccVictory += elapsedSec;
		if (m_AccVictory >= m_VictoryTime)
		{
			m_ShowVictoryScreen = true;
			SoundManager::GetInstance().StopSoundEffects();
			SoundManager::GetInstance().PlayVictory();
			m_PlayedVictory = true;
		}
		
	}
}

void Game::Draw( )
{
	ClearBackground();
	glPushMatrix();


	glScalef(3, 3, 1);
	glTranslatef(-m_upCamera->GetPosition(m_upAvatar->GetShape()).x, -m_upCamera->GetPosition(m_upAvatar->GetShape()).y, 0);
	
	//Draw Level
	m_upLevel->DrawBackground();

	//Draw EndBoss
	m_upEndBoss->Draw();

	//Draw Enemies
	for (size_t i = 0; i < m_spEnemies.size(); i++)
	{
		m_spEnemies[i]->Draw();
	}

	//Draw Bullets
	for (size_t i = 0; i < m_spBullets.size(); i++)
	{
		m_spBullets[i]->Draw();
	}

	//Draw Upgrades
	for (size_t i = 0; i < m_spUpgrades.size(); i++)
	{
		m_spUpgrades[i]->Draw();
	}

	//Draw UpgradeContainers (stationary)
	for (size_t i = 0; i < m_spUpgradeContainers.size(); i++)
	{
		m_spUpgradeContainers[i]->Draw();
	}

	//Draw UpgradeContainers (Flying)
	for (size_t i = 0; i < m_spFlyingUpgradeContainers.size(); i++)
	{
		m_spFlyingUpgradeContainers[i]->Draw();
	}

	//Draw bullets from boss
	for (size_t i = 0; i < m_spEndBossBullets.size(); i++)
	{
		m_spEndBossBullets[i]->Draw();
	}

	//Draw Avatar
	if (m_AvatarVisible)
	{
		m_upAvatar->Draw(m_Invincible);
	}
	glPopMatrix();

	glPushMatrix();
	glScalef(3, 3, 1);

	//Draw Hud
	m_upHud->Draw();

	// DeathScreen
	if (!m_upHud->GetAlive())
	{
		utils::SetColor(Color4f(0.0f, 0.0f, 0.0f, 0.7f));
		utils::FillRect(Rectf(0.0f, 0.0f, m_Window.width, m_Window.height));
		if (m_ShowDeathText)
		{
			for (size_t i = 0; i < m_spDeathScreenText.size(); i++)
			{
				m_spDeathScreenText[i]->Draw();
			}
		}
	}

	// EndScreen
	if (m_ShowVictoryScreen)
	{
		utils::SetColor(Color4f(0.0f, 0.0f, 0.0f, 1.0f));
		utils::FillRect(Rectf(0.0f, 0.0f, m_Window.width, m_Window.height));
		for (size_t i = 0; i < m_spVictoryScreenText.size(); i++)
		{
			m_spVictoryScreenText[i]->Draw();
		}
	}
	glPopMatrix();
	
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( )
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
void Game::Shoot(float elapsedSec)
{
	if (m_BulletPause && m_upHud->GetAlive() == true)
	{
		if (m_upAvatar->GetSpreadGun())
		{
			if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_8] == 1)
			{
				std::shared_ptr<Bullet> bullet{ std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width / 2, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height), Vector2f(0.0f, 1.0f), false, false, true) };
				m_spBullets.push_back(bullet);
				bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width / 2, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height), Vector2f(0.2f, 0.8f), false, false, true);
				m_spBullets.push_back(bullet);
				bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width / 2, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height), Vector2f(-0.2f, 0.8f), false, false, true);
				m_spBullets.push_back(bullet);
				bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width / 2, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height), Vector2f(0.1f, 0.9f), false, false, true);
				m_spBullets.push_back(bullet);
				bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width / 2, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height), Vector2f(-0.1f, 0.9f), false, false, true);
				m_spBullets.push_back(bullet);
				SoundManager::GetInstance().PlaySpreadGun();
				m_BulletPause = false;
			}
			else
			{
				if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_9] == 1)
				{
					std::shared_ptr<Bullet> bullet{ std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height), Vector2f(0.5f, 0.5f), false, false, true) };
					m_spBullets.push_back(bullet);
					bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height), Vector2f(0.3f, 0.7f), false, false, true);
					m_spBullets.push_back(bullet);
					bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height), Vector2f(0.4f, 0.6f), false, false, true);
					m_spBullets.push_back(bullet);
					bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height), Vector2f(0.6f, 0.4f), false, false, true);
					m_spBullets.push_back(bullet);
					bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height), Vector2f(0.7f, 0.3f), false, false, true);
					m_spBullets.push_back(bullet);
					SoundManager::GetInstance().PlaySpreadGun();
					m_BulletPause = false;
				}
				else
				{
					if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_6] == 1)
					{
						std::shared_ptr<Bullet> bullet{ std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 2), Vector2f(1.0f, 0.0f), false, false, true) };
						m_spBullets.push_back(bullet);
						bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 2), Vector2f(0.9f, 0.1f), false, false, true);
						m_spBullets.push_back(bullet);
						bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 2), Vector2f(0.9f, -0.1f), false, false, true);
						m_spBullets.push_back(bullet);
						bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 2), Vector2f(0.8f, 0.2f), false, false, true);
						m_spBullets.push_back(bullet);
						bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 2), Vector2f(0.8f, -0.2f), false, false, true);
						m_spBullets.push_back(bullet);
						SoundManager::GetInstance().PlaySpreadGun();
						m_BulletPause = false;
					}
					else
					{
						if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_3] == 1)
						{
							std::shared_ptr<Bullet> bullet{ std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 4), Vector2f(0.5f, -0.5f), false, false, true) };
							m_spBullets.push_back(bullet);
							bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 4), Vector2f(0.6f, -0.4f), false, false, true);
							m_spBullets.push_back(bullet);
							bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 4), Vector2f(0.7f, -0.3f), false, false, true);
							m_spBullets.push_back(bullet);
							bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 4), Vector2f(0.4f, -0.6f), false, false, true);
							m_spBullets.push_back(bullet);
							bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 4), Vector2f(0.3f, -0.7f), false, false, true);
							m_spBullets.push_back(bullet);
							SoundManager::GetInstance().PlaySpreadGun();
							m_BulletPause = false;
						}
						else
						{
							if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_7] == 1)
							{
								std::shared_ptr<Bullet> bullet{ std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height), Vector2f(-0.5f, 0.5f), false, false, true) };
								m_spBullets.push_back(bullet);
								bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height), Vector2f(-0.3f, 0.7f), false, false, true);
								m_spBullets.push_back(bullet);
								bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height), Vector2f(-0.4f, 0.6f), false, false, true);
								m_spBullets.push_back(bullet);
								bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height), Vector2f(-0.6f, 0.4f), false, false, true);
								m_spBullets.push_back(bullet);
								bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height), Vector2f(-0.7f, 0.3f), false, false, true);
								m_spBullets.push_back(bullet);
								SoundManager::GetInstance().PlaySpreadGun();
								m_BulletPause = false;
							}
							else
							{
								if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_4] == 1)
								{
									std::shared_ptr<Bullet> bullet{ std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 2), Vector2f(-1.0f, 0.0f), false, false, true) };
									m_spBullets.push_back(bullet);
									bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 2), Vector2f(-0.9f, 0.1f), false, false, true);
									m_spBullets.push_back(bullet);
									bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 2), Vector2f(-0.9f, -0.1f), false, false, true);
									m_spBullets.push_back(bullet);
									bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 2), Vector2f(-0.8f, 0.2f), false, false, true);
									m_spBullets.push_back(bullet);
									bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 2), Vector2f(-0.8f, -0.2f), false, false, true);
									m_spBullets.push_back(bullet);
									SoundManager::GetInstance().PlaySpreadGun();
									m_BulletPause = false;
								}
								else
								{
									if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_1] == 1)
									{
										std::shared_ptr<Bullet> bullet{ std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 4), Vector2f(-0.5f, -0.5f), false, false, true) };
										m_spBullets.push_back(bullet);
										bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 4), Vector2f(-0.6f, -0.4f), false, false, true);
										m_spBullets.push_back(bullet);
										bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 4), Vector2f(-0.7f, -0.3f), false, false, true);
										m_spBullets.push_back(bullet);
										bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 4), Vector2f(-0.4f, -0.6f), false, false, true);
										m_spBullets.push_back(bullet);
										bullet = std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 4), Vector2f(-0.3f, -0.7f), false, false, true );
										m_spBullets.push_back(bullet);
										SoundManager::GetInstance().PlaySpreadGun();
										m_BulletPause = false;
									}
								}
							}
						}
					}
				}
			}
		}
		else if (m_upAvatar->GetMachineGun())
		{
			if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_8] == 1)
			{
				std::shared_ptr<Bullet> bullet{ std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width / 2, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height), Vector2f(0.0f, 1.0f), false, true) };
				m_spBullets.push_back(bullet);
				m_BulletPause = false;
			}
			else
			{
				if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_9] == 1)
				{
					std::shared_ptr<Bullet> bullet{ std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height), Vector2f(0.5f, 0.5f), false, true) };
					m_spBullets.push_back(bullet);
					m_BulletPause = false;
				}
				else
				{
					if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_6] == 1)
					{
						std::shared_ptr<Bullet> bullet{ std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 2), Vector2f(1.0f, 0.0f), false, true) };
						m_spBullets.push_back(bullet);
						m_BulletPause = false;
					}
					else
					{
						if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_3] == 1)
						{
							std::shared_ptr<Bullet> bullet{ std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 4), Vector2f(0.5f, -0.5f), false, true) };
							m_spBullets.push_back(bullet);
							m_BulletPause = false;
						}
						else
						{
							if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_7] == 1)
							{
								std::shared_ptr<Bullet> bullet{ std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height), Vector2f(-0.5f, 0.5f), false, true) };
								m_spBullets.push_back(bullet);
								m_BulletPause = false;
							}
							else
							{
								if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_4] == 1)
								{
									std::shared_ptr<Bullet> bullet{ std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 2), Vector2f(-1.0f, 0.0f), false, true) };
									m_spBullets.push_back(bullet);
									m_BulletPause = false;
								}
								else
								{
									if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_1] == 1)
									{
										std::shared_ptr<Bullet> bullet{ std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 4), Vector2f(-0.5f, -0.5f), false, true) };
										m_spBullets.push_back(bullet);
										m_BulletPause = false;
									}
								}
							}
						}
					}
				}
			}
		}
		else
		{
			if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_8] == 1)
			{
				std::shared_ptr<Bullet> bullet{ std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width / 2, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height), Vector2f(0.0f, 1.0f), false) };
				m_spBullets.push_back(bullet);
				m_BulletPause = false;
			}
			else
			{
				if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_9] == 1)
				{
					std::shared_ptr<Bullet> bullet{ std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height), Vector2f(0.5f, 0.5f), false) };
					m_spBullets.push_back(bullet);
					m_BulletPause = false;
				}
				else
				{
					if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_6] == 1)
					{
						std::shared_ptr<Bullet> bullet{ std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 2), Vector2f(1.0f, 0.0f), false) };
						m_spBullets.push_back(bullet);
						m_BulletPause = false;
					}
					else
					{
						if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_3] == 1)
						{
							std::shared_ptr<Bullet> bullet{ std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left + m_upAvatar->GetShape().width, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 4), Vector2f(0.5f, -0.5f), false) };
							m_spBullets.push_back(bullet);
							m_BulletPause = false;
						}
						else
						{
							if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_7] == 1)
							{
								std::shared_ptr<Bullet> bullet{ std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height), Vector2f(-0.5f, 0.5f), false) };
								m_spBullets.push_back(bullet);
								m_BulletPause = false;
							}
							else
							{
								if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_4] == 1)
								{
									std::shared_ptr<Bullet> bullet{ std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 2), Vector2f(-1.0f, 0.0f), false) };
									m_spBullets.push_back(bullet);
									m_BulletPause = false;
								}
								else
								{
									if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_1] == 1)
									{
										std::shared_ptr<Bullet> bullet{ std::make_shared<Bullet>(Point2f(m_upAvatar->GetShape().left, m_upAvatar->GetShape().bottom + m_upAvatar->GetShape().height / 4), Vector2f(-0.5f, -0.5f), false) };
										m_spBullets.push_back(bullet);
										m_BulletPause = false;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		m_bulletAccTime += elapsedSec;		

		if (m_bulletAccTime >= m_BulletPauseTime)
		{
			m_BulletPause = true;
			m_bulletAccTime = 0.0f;
		}
	}
}
void Game::ButtonDetection()
{
	if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_R] == 1 && !m_upHud->GetAlive())
	{
		Reset();
	}
	if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_MINUS] == 1)
	{
		SoundManager::GetInstance().LowerAudio();
	}
	if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_PLUS] == 1)
	{
		SoundManager::GetInstance().HigherAudio();
	}
}
void Game::HandleEnemies(float elapsedSec)
{
	for (size_t i = 0; i < m_spEnemies.size(); i++)
	{
		m_spEnemies[i]->Update(elapsedSec, m_upAvatar->GetHitbox());
		if (m_spEnemies[i]->ShootUpdate() == true)
		{
			m_spBullets.push_back(std::make_shared<Bullet>(m_spEnemies[i]->Shoot()));
		}
		if (m_spEnemies[i]->IsRunner())
		{
			m_upLevel->RunnerCollision(m_spEnemies[i]);
			if (utils::IsOverlapping(m_upAvatar->GetShape(), m_spEnemies[i]->GetShape()) && !m_Invincible && m_spEnemies[i]->IsAlive())
			{
				m_upHud->Damaged();
			}
		}
	}
}
void Game::HandleBullets(float elapsedSec)
{
	for (size_t i = 0; i < m_spBullets.size(); i++)
	{
		m_spBullets[i]->Update(elapsedSec);
		m_spBullets[i]->Collision(Rectf(m_upCamera->GetPos().x, m_upCamera->GetPos().y, m_CamWidth, m_CamHeight), m_upHud, m_upAvatar, m_spEnemies,m_spUpgradeContainers, m_spFlyingUpgradeContainers, m_upEndBoss, m_Invincible);
		if (!m_spBullets[i]->GetActive())
		{
			std::shared_ptr<Bullet> temp = m_spBullets[i];
			m_spBullets[i] = m_spBullets.back();
			m_spBullets.pop_back();
		}
	}
}
void Game::HandleEndBossBullets(float elapsedSec)
{
	for (size_t i = 0; i < m_spEndBossBullets.size(); i++)
	{
		m_spEndBossBullets[i]->Update(elapsedSec);
		m_spEndBossBullets[i]->Collision(Rectf(m_upCamera->GetPos().x, m_upCamera->GetPos().y, m_CamWidth, m_CamHeight), m_upHud, m_upAvatar, m_Invincible);
		if (!m_spEndBossBullets[i]->GetActive())
		{
			std::shared_ptr<EndBossBullet> temp = m_spEndBossBullets[i];
			m_spEndBossBullets[i] = m_spEndBossBullets.back();
			m_spEndBossBullets.pop_back();
		}
	}
}
void Game::HandleEndBoss(float elapsedSec)
{
	m_upEndBoss->Update(elapsedSec);

	if (m_upEndBoss->ShootUpdateLeft())
	{
		m_spEndBossBullets.push_back(std::make_shared<EndBossBullet>(m_upEndBoss->ShootLeft()));
	}
	if (m_upEndBoss->ShootUpdateRight())
	{
		m_spEndBossBullets.push_back(std::make_shared<EndBossBullet>(m_upEndBoss->ShootRight()));
	}
}
void Game::HandleUpgrades(float elapsedSec)
{
	for (size_t i = 0; i < m_spUpgrades.size(); i++)
	{
		m_spUpgrades[i]->Update(m_upAvatar, elapsedSec);

		if (!m_spUpgrades[i]->GetActive())
		{
			m_spUpgrades[i] = m_spUpgrades.back();
			m_spUpgrades.pop_back();
		}
	}
	m_upLevel->UpgradeCollision(m_spUpgrades);
}
void Game::HandleUpgradeContainers(float elapsedSec)
{
	for (size_t i = 0; i < m_spUpgradeContainers.size(); i++)
	{
		m_spUpgradeContainers[i]->Update(elapsedSec, m_upAvatar->GetHitbox());

		if (!m_spUpgradeContainers[i]->IsAlive())
		{
			SoundManager::GetInstance().PlayDeath();
			m_spUpgrades.push_back(std::make_shared<Upgrade>(m_spUpgradeContainers[i]->GetPos(), m_spUpgradeContainers[i]->IsMachineGun(), true));
			m_spUpgradeContainers[i] = m_spUpgradeContainers.back();
			m_spUpgradeContainers.pop_back();
		}
	}
	for (size_t i = 0; i < m_spFlyingUpgradeContainers.size(); i++)
	{
		m_spFlyingUpgradeContainers[i]->Update(elapsedSec, Rectf{m_upCamera->GetPos().x, m_upCamera->GetPos().y, m_CamWidth, m_CamHeight });


		if (!m_spFlyingUpgradeContainers[i]->IsAlive())
		{
			SoundManager::GetInstance().PlayDeath();
			m_spUpgrades.push_back(std::make_shared<Upgrade>(m_spFlyingUpgradeContainers[i]->GetPos(), m_spFlyingUpgradeContainers[i]->IsMachineGun(), false));
			m_spFlyingUpgradeContainers[i] = m_spFlyingUpgradeContainers.back();
			m_spFlyingUpgradeContainers.pop_back();
		}
	}
	if (m_upAvatar->GetShape().left >= m_FirstFlyActivate && !m_FirstFlyActivated)
	{
		m_spFlyingUpgradeContainers[0]->Activate();
		m_FirstFlyActivated = true;
	}
	if (m_upAvatar->GetShape().left >= m_SecondThirdFlyActivate && !m_SecondThirdFlyActivated)
	{
		m_spFlyingUpgradeContainers[0]->Activate();
		m_spFlyingUpgradeContainers[1]->Activate();
		m_SecondThirdFlyActivated = true;
	}
}
void Game::StartBoss()
{
	if (m_upAvatar->GetShape().left >= m_BossStart && !m_upEndBoss->IsDetecting())
	{
		m_upCamera->ShiftToEnd();
		m_upEndBoss->Detect();
		SoundManager::GetInstance().PlayEndBossShift();
	}
}
void Game::SpawnRunners()
{
	int quantity = rand() % 4;

	for (int i = 0; i < quantity; i++)
	{
		m_spEnemies.push_back(std::make_shared<Runner>(Point2f(m_upCamera->GetPos().x + m_CamWidth + 100.0f + (10 * i), rand() % 196 + 30.0f)));
	}
}
void Game::Reset()
{
	m_spBullets.clear();
	m_spEndBossBullets.clear();
	m_spEnemies.clear();
	m_spFlyingUpgradeContainers.clear();
	m_spUpgradeContainers.clear();
	m_spUpgrades.clear();
	m_spEndBossBullets.clear();
	m_upCamera->Reset();
	m_upAvatar->Reset();
	m_upEndBoss->Reset();
	m_upHud->Reset();
	m_upLevel->Reset();
	m_ShowDeathText = false;
	m_FirstFlyActivated = false;
	m_SecondThirdFlyActivated = false;

	InitEnemies();
	InitPowerUps();
}