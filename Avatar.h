#pragma once
#include "Vector2f.h"
#include "level.h"
class TextureManager;
class Avatar
{
public:
	Avatar();
	~Avatar();
	enum ActionState
	{
		waiting,
		walking,
		jumping,
		ground,
		swimming
	};

	enum AimDirection
	{
		straight,
		halfdown,
		halfup,
		up,
		nothing
	};
	void Update(float elapsedSec, std::unique_ptr<Level>& level);
	void Draw(bool invincible);
	void Died();
	Rectf GetShape();
	Rectf GetHitbox();
	void Clamp(Point2f& pos);
	void ActivateMachineGun();
	void ActivateSpreadGun();
	void DeactivateGuns();
	void Reset();
	bool GetMachineGun();
	bool GetSpreadGun();
private:
	Rectf m_Shape;
	Rectf m_Hitbox;

	Rectf m_Legsrc;
	Rectf m_Chestsrc;

	Vector2f m_Velocity;
	Vector2f m_Acceleration;
	ActionState m_ActionState;
	AimDirection m_AimDirection;
	float m_AccuTransformSec;

	std::shared_ptr<Texture> m_spAvatar;
	std::shared_ptr<Texture> m_spAvatarTransparent;

	const float m_HorSpeed{ 70.0f };
	const float m_JumpSpeed{ 350.0f };
	

	int m_AnimFrameLegs;
	int m_AnimFrameChest;
	int m_AnimFrameSwim;
	int m_AnimFrameJump;
	float m_AnimTimeLegs;
	float m_AnimTimeChest;
	float m_AnimTimeSwim;
	float m_AnimTimeJump;

	const float m_ClipHeight{ 33.0f };
	const float m_ClipWidth{ 23.0f };
	const int m_NrOfFramesLeg{ 3 };
	const int m_NrOfFramesChest{ 5 };
	const int m_NrOfFramesSwim{ 4 };
	const int m_NrOfFramesJump{ 4 };
	const int m_NrFramesPerSec{ 5 };
	const int m_NrFramesPerSecSwim{ 8 };

	bool m_UsingSpread;
	bool m_UsingMachine;
	bool m_IsAlive;

	void Move(float elapsedSec, std::unique_ptr<Level>& level);
	void Aim();
	void AvatarCalculation(float elapsedSec);
	void FlipSprite();
	void HelpTeleports();
};

