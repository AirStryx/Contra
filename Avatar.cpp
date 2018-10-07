#include "stdafx.h"
#include "Avatar.h"
#include "TextureManager.h"

Avatar::Avatar()
	:m_Shape{ 50,185,23,33 }
	,m_Velocity{0.0f, 0.0f}
	,m_Acceleration{0.0, -981.0f}
	, m_AccuTransformSec{ 0.0f }
	, m_spAvatar{TextureManager::GetInstance().GetAvatar()}
	, m_spAvatarTransparent{TextureManager::GetInstance().GetTransparentPlayer()}
	, m_Legsrc{ 0.0f, 0.0f, m_ClipWidth, m_ClipHeight }
	, m_Chestsrc{ 0.0f, 0.0f, m_ClipWidth, m_ClipHeight }
	, m_IsAlive{true}
	, m_AnimTimeChest{ 0.0f }
	, m_AnimTimeJump{0.0f}
	, m_AnimTimeLegs{0.0f}
	, m_AnimTimeSwim{0.0f}
	, m_AnimFrameChest{0}
	, m_AnimFrameJump{0}
	, m_AnimFrameLegs{0}
	, m_AnimFrameSwim{0}
	, m_UsingSpread{false}
	, m_UsingMachine{false}
{
	m_Legsrc.width = m_ClipWidth - 1;
	m_Legsrc.height = m_ClipHeight - 1;

	m_Chestsrc.width = m_ClipWidth - 1;
	m_Chestsrc.height = m_ClipHeight - 1;
}


Avatar::~Avatar()
{
}
void Avatar::Update(float elapsedSec, std::unique_ptr<Level>& level)
{
	//initialising hitbox
	m_Hitbox = m_Shape;
	if (m_IsAlive)
	{
	//read keyboard inputs for moving
	Move(elapsedSec, level);

	//read keyboard inputs for aiming
	Aim();

	//deciding on actionstate
	if (m_ActionState != ActionState::ground)
	{
		if (m_Velocity.x > 0 || m_Velocity.x < 0 || m_Velocity.y > 0 || m_Velocity.y < 0)
		{
			if (m_Velocity.y == 0.0f)
			{
				m_ActionState = ActionState::walking;
			}
			else
			{
				m_ActionState = ActionState::jumping;
			}

			m_Shape.left += m_Velocity.x * elapsedSec;
			m_Shape.bottom += m_Velocity.y * elapsedSec;
			level->HandleCollision(m_Shape, m_Velocity);
		}
		else
		{
			m_ActionState = ActionState::waiting;
			m_Legsrc.bottom = m_ClipHeight / 3;
			m_Legsrc.left = 0.0f;
		}
	}
	else
	{
		m_Shape.bottom += m_Velocity.y * elapsedSec;
		level->HandleCollision(m_Shape, m_Velocity);
	}
	if (level->IsOnWater(m_Shape) == true)
	{
		m_ActionState = ActionState::swimming;
	}

	}
	else
	{
		m_Legsrc.left = m_ClipWidth * 4;
		m_Legsrc.bottom = m_ClipHeight * 3;
		m_Chestsrc.left = m_ClipWidth * 3;
		m_Chestsrc.bottom = m_ClipHeight * 3;
	}
	AvatarCalculation(elapsedSec);
	HelpTeleports();
}
void Avatar::Draw(bool invincible)
{
	glPushMatrix();
	FlipSprite();
	Rectf drawRect{ m_Shape.left, m_Shape.bottom, m_Shape.width, m_Shape.height};
	if (m_ActionState == ActionState::swimming || m_ActionState == ActionState::jumping)
	{
		if (invincible)
		{
			m_spAvatarTransparent->Draw(drawRect, m_Legsrc);
		}
		else
		{
			m_spAvatar->Draw(drawRect, m_Legsrc);
		}
		
	}
	else
	{
		if (m_IsAlive)
		{
			m_spAvatar->Draw(drawRect, m_Legsrc);
			if (m_AimDirection == AimDirection::up && m_ActionState != ActionState::ground)
			{
				drawRect.bottom += 12;
				drawRect.left += 3;
			}
			if (m_ActionState == ActionState::ground)
			{
				drawRect.left += drawRect.width;
			}
			if (invincible)
			{
				m_spAvatarTransparent->Draw(drawRect, m_Chestsrc);
			}
			else
			{
				m_spAvatar->Draw(drawRect, m_Chestsrc);
			}
			
		}
		
	}

	if (m_IsAlive == false)
	{
		m_spAvatar->Draw(drawRect, m_Legsrc);
		drawRect.left -= 22;
		m_spAvatar->Draw(drawRect, m_Chestsrc);
	}
	glPopMatrix();
	
}
void Avatar::Died()
{
	m_IsAlive = false;
}
Rectf Avatar::GetHitbox()
{
	return m_Hitbox;
}
Rectf Avatar::GetShape()
{
	return m_Shape;
}
void Avatar::Move(float elapsedSec, std::unique_ptr<Level>& level)
{
	if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_A] == 1)
	{
		m_Velocity.x = -m_HorSpeed;
	}
	else
	{
		if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_D] == 1)
		{
			m_Velocity.x = m_HorSpeed;
		}
		else
		{
			m_Velocity.x = 0.0f;
		}
	}
	if ((level->IsOnGround(m_Shape) == true && m_Velocity.y == 0) || level->IsOnWater(m_Shape) == true)
	{
		if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_W] == 1)
		{
			m_Velocity.y = m_JumpSpeed;
		}
	}
	else
	{
		m_Velocity.y += m_Acceleration.y *elapsedSec;
	}

	if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_S] == 1)
	{
		m_ActionState = ActionState::ground;
		m_Hitbox.height = m_Shape.width - 5;
		m_Hitbox.width = m_Shape.height;

	}
	else
	{
		m_ActionState = ActionState::waiting;
		m_Shape.width = 23.0f;
		m_Shape.height = 33.0f;

	}

	if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_S] == 1 && SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_SPACE] == 1  && level->GetHold() == false)// && level.IsOnGround(m_Shape) == true)
	{
		level->DropDown();
	}

	if ((SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_S] == 0 || SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_SPACE] == 0) && level->GetHold() == true)
	{
		level->setHold(false);
	}
}
void Avatar::Aim()
{
	if (m_IsAlive)
	{
		if (m_ActionState != ActionState::ground)
		{
			if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_8] == 1)
			{
				m_AimDirection = AimDirection::up;
			}
			else
			{
				if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_9] == 1 || SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_7] == 1)
				{
					m_AimDirection = AimDirection::halfup;
				}
				else
				{
					if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_6] == 1 || SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_4] == 1)
					{
						m_AimDirection = AimDirection::straight;
					}
					else
					{
						if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_3] == 1 || SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_1] == 1)
						{
							m_AimDirection = AimDirection::halfdown;
						}
						else
						{
							m_AimDirection = AimDirection::nothing;
						}
					}
				}
			}
		}

	}

	
}
void Avatar::AvatarCalculation(float elapsedSec)
{
	if (m_IsAlive)
	{
		switch (m_ActionState)
		{
		case Avatar::waiting:
			m_Legsrc.bottom = m_ClipHeight * 2;
			m_Legsrc.left = 0.0f;

			m_Chestsrc.bottom = m_ClipHeight;
			m_Chestsrc.left = m_ClipWidth;

			break;

		case Avatar::walking:
			m_AnimTimeLegs += elapsedSec;

			if (m_AnimTimeLegs >= elapsedSec * m_NrFramesPerSec)
			{
				m_AnimTimeLegs = 0;
				m_AnimFrameLegs += 1;

				if (m_AnimFrameLegs == m_NrOfFramesLeg)
				{
					m_AnimFrameLegs = 0;
				}
			}

			m_AnimTimeChest += elapsedSec;
			if (m_AnimTimeChest >= elapsedSec * m_NrFramesPerSec)
			{
				m_AnimTimeChest = 0;
				m_AnimFrameChest += 1;

				if (m_AnimFrameChest == m_NrOfFramesChest)
				{
					m_AnimFrameChest = 0;
				}
			}

			m_Chestsrc.bottom = m_ClipHeight;
			m_Chestsrc.left = 4 * m_ClipWidth + (m_ClipWidth * m_AnimFrameChest);

			m_Legsrc.bottom = m_ClipHeight * 2;
			m_Legsrc.left = 0.0f + (m_ClipWidth * m_AnimFrameLegs);
			break;

		case Avatar::swimming:
			m_AnimTimeSwim += elapsedSec;
			if (m_AnimTimeSwim >= elapsedSec * m_NrFramesPerSecSwim)
			{
				m_AnimTimeSwim = 0;
				m_AnimFrameSwim += 1;

				if (m_AnimFrameSwim == m_NrOfFramesSwim)
				{
					m_AnimFrameSwim = 0;
				}
			}

			m_Legsrc.bottom = m_ClipHeight * 2;
			m_Legsrc.left = 5.0f * m_ClipWidth + (m_ClipWidth * m_AnimFrameSwim);
			break;
		case Avatar::jumping:
			m_AnimTimeJump += elapsedSec;
			if (m_AnimTimeJump >= elapsedSec * m_NrFramesPerSec)
			{
				m_AnimTimeJump = 0;
				m_AnimFrameJump += 1;

				if (m_AnimFrameJump == m_NrOfFramesJump)
				{
					m_AnimFrameJump = 0;
				}
			}

			m_Legsrc.bottom = m_ClipHeight * 3;
			m_Legsrc.left = 5.0f * m_ClipWidth + (m_ClipWidth * m_AnimFrameJump);

			break;

		case Avatar::ground:
			m_Legsrc.bottom = m_ClipHeight * 3;
			m_Legsrc.left = m_ClipWidth;

			m_Chestsrc.bottom = m_ClipHeight * 3;
			m_Chestsrc.left = m_ClipWidth * 2;
			break;
		default:
			break;
		}
		if (m_ActionState != Avatar::ground)
		{
			switch (m_AimDirection)
			{
			case Avatar::straight:
				m_Chestsrc.left = m_ClipWidth;
				break;
			case Avatar::halfdown:
				m_Chestsrc.left = 0.0f;
				break;
			case Avatar::halfup:
				m_Chestsrc.left = 2 * m_ClipWidth;
				break;
			case Avatar::up:
				m_Chestsrc.left = 3 * m_ClipWidth;
				break;
			case Avatar::nothing:
				break;
			default:
				break;
			}
		}
	}
	else
	{
		m_Chestsrc.left = 3 * m_ClipWidth;
		m_Chestsrc.bottom = m_ClipHeight * 3;

		m_Legsrc.left = 4 * m_ClipWidth;
		m_Legsrc.bottom = m_ClipHeight * 3;
	}
	
}
void Avatar::ActivateMachineGun()
{
	m_UsingSpread = false;
	m_UsingMachine = true;
}
void Avatar::ActivateSpreadGun()
{
	m_UsingSpread = true;
	m_UsingMachine = false;
}
void Avatar::DeactivateGuns()
{
	m_UsingSpread = false;
	m_UsingMachine = false;
}
void Avatar::Reset()
{
	m_Shape.left = 50.0f;
	m_Shape.bottom = 185.0f;
	m_IsAlive = true;
}
bool Avatar::GetMachineGun()
{
	return m_UsingMachine;
}
bool Avatar::GetSpreadGun()
{
	return m_UsingSpread;
}
void Avatar::Clamp(Point2f& pos)
{
	if (m_Shape.left <= pos.x)
	{
		m_Shape.left = pos.x;
	}
}
void Avatar::FlipSprite()
{
	if (m_Velocity.x < 0 || SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_7] == 1 || SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_4] == 1 || SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_1] == 1)
	{
		glTranslatef(m_Shape.left + m_Shape.width / 2, m_Shape.bottom, 0);
		glScalef(-1, 1, 1);
		glTranslatef(-m_Shape.left - m_Shape.width / 2, -m_Shape.bottom, 0);

	}
}
void Avatar::HelpTeleports()
{
	if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_B] == 1)
	{
		m_Shape.left = 3022.0f;
		m_Shape.bottom = 120.0f;
	}
	if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_M] == 1)
	{
		ActivateMachineGun();
	}
	if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_N] == 1)
	{
		ActivateSpreadGun();
	}
}
