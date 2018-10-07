#pragma once
#include "Texture.h"
#include "Vector2f.h"
#include "utils.h"
#include "Bridge.h"
#include "SoundEffect.h"
class Upgrade;
class TextureManager;
class SoundManager;
class Enemies;
class Level
{
public:
	Level();
	void Update(float elapsedSec);
	void DrawBackground();
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity);
	void RunnerCollision(std::shared_ptr<Enemies> enemy);
	void UpgradeCollision(std::vector<std::shared_ptr<Upgrade>>& upgrades);
	bool IsOnGround(const Rectf& actorShape);
	bool IsOnWater(const Rectf& actorShape) const;
	void DropDown();
	bool GetHold();
	void setHold(bool fall);

	void Reset();

	bool DidFall();

	Rectf GetBoundaries();
private:
	// SVG
	std::vector<std::vector<Point2f>> m_Levels;
	std::vector<Point2f> m_Water;
	std::vector<Point2f>m_Death;

	// Texture
	std::shared_ptr<Texture> m_spBackgroundTexture;
	Point2f m_LastPos;

	// Scale
	const float m_Scale{ 2.0f };


	Rectf m_Boundaries;

	//DropDown
	bool m_GoThroughFloor;
	bool m_FallHold;
	const float m_FallTime{ 0.2f };
	float m_AccFallTime;

	// Bridges
	Bridge m_Bridge1;
	Bridge m_Bridge2;

	std::vector<std::vector<Point2f>> m_Bridge1Vec;
	std::vector<std::vector<Point2f>> m_Bridge2Vec;

	const float m_BossWall{ 3237.0f };

	bool m_Fell{};

	float m_AccLPos;
	const float m_totalLPos{ 0.60f };
	//fall
	void Death(Rectf& actorShape, Vector2f& actorVelocity);
	
};

