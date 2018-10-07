#pragma once
#include <memory>
#include "Texture.h"
#include "Avatar.h"

class TextureManager;
class Upgrade
{
public:
	Upgrade(Point2f pos, bool isMachine, bool BlastUp = false);
	void Draw();
	void Update(std::unique_ptr<Avatar>& avatar, float elapsedSec);
	void StopFall();
	Rectf GetShape();
	bool GetFall();
	bool GetUpOrNot();
	bool GetActive();
private:
	bool m_IsMachine;
	bool m_Fall;
	Rectf m_Pos;
	float m_Speed;
	const float m_MinSpeed{ -80.0f };
	const float m_gravity{ -80.0f };
	std::shared_ptr<Texture> m_spUpgrade;

	bool m_active;
	Rectf m_Source;
	void trigger(std::unique_ptr<Avatar>& avatar);
	void Bump();
};

