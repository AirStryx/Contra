#include "stdafx.h"
#include "level.h"
#include "SVGParser.h"
#include "Upgrade.h"
#include "TextureManager.h"
#include "Enemies.h"
#include "SoundManager.h"
#include <string>

Level::Level()
	: m_spBackgroundTexture{TextureManager::GetInstance().GetLevel()}
	, m_Boundaries{ 0,0,m_spBackgroundTexture->GetWidth(), m_spBackgroundTexture->GetHeight()}
	, m_LastPos{ 0.0f, 0.0f }
	, m_GoThroughFloor{ false }
	, m_FallHold{false}
	, m_Bridge1{Point2f(768, 95)}
	, m_Bridge2{ Point2f(1056, 95) }
{
	for (size_t i = 0; i < 39; i++)
	{
		std::vector<Point2f> m_Vertices;
		SVGParser::GetVerticesFromSvgFile("resources/SVG/ground" + std::to_string(i+1)  + ".svg", m_Vertices);
		m_Levels.push_back(m_Vertices);
	}
	SVGParser::GetVerticesFromSvgFile("resources/SVG/LevelWater1.svg", m_Water);
	SVGParser::GetVerticesFromSvgFile("resources/SVG/LevelDeath1.svg", m_Death);

	for (size_t i = 0; i < m_Death.size(); i++)
	{
		m_Death[i].y -= 50;
	}
	
}
void Level::Update(float elapsedSec)
{
	if (m_GoThroughFloor)
	{
		m_AccFallTime += elapsedSec;

		if (m_AccFallTime >= m_FallTime)
		{
			m_GoThroughFloor = false;
			m_AccFallTime = 0.0f;
		}
	}
	
	m_AccLPos += elapsedSec;
	m_Bridge1.Update(elapsedSec);
	m_Bridge2.Update(elapsedSec);
	m_Bridge1Vec = m_Bridge1.GetBridge();
	m_Bridge2Vec = m_Bridge2.GetBridge();
}
void Level::DrawBackground()
{
	Rectf DestRect{ 0.0f, 0.0f, m_spBackgroundTexture->GetWidth(), m_spBackgroundTexture->GetHeight()};
	m_spBackgroundTexture->Draw(DestRect);
	
	m_Bridge1.Draw();
	m_Bridge2.Draw();

}

bool Level::DidFall()
{
	if (m_Fell == true)
	{
		m_Fell = false;
		return true;
	}
	else
	{
		return false;
	}
}

void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity)
{
	//Bridge detection
	std::vector<Point2f> bridge1{ Point2f(765.0f, 1000.0f), Point2f(765.0f, 0.0f) };
	std::vector<Point2f> bridge2{ Point2f(1052.0f, 1000.0f), Point2f(1052.0f, 0.0f) };

	utils::HitInfo hitInfo;
	Point2f point1{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 3 };
	Point2f point2{ point1.x, actorShape.bottom };
	Point2f point3{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height / 3 };
	//ground detection
	if (actorShape.left + actorShape.width > m_BossWall) // Boss Wall, non passable
	{
		actorShape.left = m_BossWall - actorShape.width;
	}
	if (!m_GoThroughFloor)
	{
		for (size_t i = 0; i < m_Levels.size(); i++)
		{

			if (utils::Raycast(m_Levels[i], point1, point2, hitInfo) == true && actorVelocity.y < 0)
			{
				actorShape.bottom = hitInfo.intersectPoint.y;
				actorVelocity.y = 0.0f;
				SoundManager::GetInstance().PlayLanding();
			}

		}
		for (size_t i = 0; i < m_Bridge1Vec.size(); i++)
		{
			if (utils::Raycast(m_Bridge1Vec[i], point1, point2, hitInfo) == true && actorVelocity.y < 0)
			{
				actorShape.bottom = hitInfo.intersectPoint.y;
				actorVelocity.y = 0.0f;
			}
		}
		for (size_t i = 0; i < m_Bridge2Vec.size(); i++)
		{
			if (utils::Raycast(m_Bridge2Vec[i], point1, point2, hitInfo) == true && actorVelocity.y < 0)
			{
				actorShape.bottom = hitInfo.intersectPoint.y;
				actorVelocity.y = 0.0f;
			}
		}
	}

	//water detection
	if (utils::Raycast(m_Water, point1, point2, hitInfo) == true)
	{
		actorShape.bottom = hitInfo.intersectPoint.y;
		actorVelocity.y = 0.0f;
	}

	//death detection
	if (utils::Raycast(m_Death, point1, point2, hitInfo) == true)
	{
		Death(actorShape, actorVelocity);
		actorVelocity.y = 0.0f;
	}

	//Bridge Explosion
	if (utils::Raycast(bridge1, point1, point3, hitInfo) == true)
	{
		m_Bridge1.BlowUp();
	}
	if (utils::Raycast(bridge2, point1, point3, hitInfo) == true)
	{
		m_Bridge2.BlowUp();
	}
}
void Level::RunnerCollision(std::shared_ptr<Enemies> enemy)
{
	utils::HitInfo hitInfo;
	Point2f point1{ enemy->GetShape().left + enemy->GetShape().width / 2, enemy->GetShape().bottom + enemy->GetShape().height / 3 };
	Point2f point2{ point1.x, enemy->GetShape().bottom };
	bool madeCollision{ false };
	int vertVelocity{ int(enemy->GetVerticalVel()) };

	for (size_t i = 0; i < m_Levels.size(); i++)
	{
		if (utils::Raycast(m_Levels[i], point1, point2, hitInfo) == true && vertVelocity <= 0)
		{
			madeCollision = true;
			enemy->TouchGround();
		}

	}
	for (size_t i = 0; i < m_Bridge1Vec.size(); i++)
	{
		if (utils::Raycast(m_Bridge1Vec[i], point1, point2, hitInfo) == true && vertVelocity <= 0)
		{
			madeCollision = true;
			enemy->TouchGround();
		}
	}
	for (size_t i = 0; i < m_Bridge2Vec.size(); i++)
	{
		if (utils::Raycast(m_Bridge2Vec[i], point1, point2, hitInfo) == true && vertVelocity <= 0)
		{
			madeCollision = true;
			enemy->TouchGround();
		}
	}
	if (madeCollision && !enemy->GetSpawnCheck())
	{
		enemy->ResetHeight();
	}
	if (!madeCollision && !enemy->GetSpawnCheck())
	{
		enemy->Checked();
	}

	if (!madeCollision && enemy->HasTouchedGround())
	{
		
		int choice = rand() % 5;
		if (choice == 0)
		{
			enemy->TurnAround();
		}
		else
		{
			enemy->Jump();
		}
	}
}
void Level::UpgradeCollision(std::vector<std::shared_ptr<Upgrade>>& upgrades)
{
	for (size_t i = 0; i < upgrades.size(); i++)
	{	
		if (upgrades[i]->GetFall() && upgrades[i]->GetUpOrNot() == false)
		{
			utils::HitInfo hitInfo;
			Point2f point1{ upgrades[i]->GetShape().left + upgrades[i]->GetShape().width / 2, upgrades[i]->GetShape().bottom + upgrades[i]->GetShape().height / 2 };
			Point2f point2{ point1.x, upgrades[i]->GetShape().bottom };

			for (size_t t = 0; t < m_Levels.size(); t++)
			{				
				if (utils::Raycast(m_Levels[t], point1, point2, hitInfo) == true)
				{
					upgrades[i]->StopFall();
				}

			}
		}
	}
}
bool Level::IsOnGround(const Rectf& actorShape)
{
	utils::HitInfo hitInfo;
	Point2f point1{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2 };
	Point2f point2{ point1.x, actorShape.bottom};
	if (!m_GoThroughFloor)
	{
		for (size_t i = 0; i < m_Levels.size(); i++)
		{
			if (utils::Raycast(m_Levels[i], point1, point2, hitInfo) == true)
			{
				if (m_AccLPos >= m_totalLPos)
				{
					m_LastPos.x = actorShape.left;
					m_LastPos.y = actorShape.bottom + 10;
					m_AccLPos = 0.0f;
				}			
				return true;
			}
			for (size_t i = 0; i < m_Bridge1Vec.size(); i++)
			{
				if (utils::Raycast(m_Bridge1Vec[i], point1, point2, hitInfo) == true)
				{
					if (m_AccLPos >= m_totalLPos)
					{
						m_LastPos.x = actorShape.left;
						m_LastPos.y = actorShape.bottom + 10;
						m_AccLPos = 0.0f;
					}
					return true;
				}
			}
			for (size_t i = 0; i < m_Bridge2Vec.size(); i++)
			{
				if (utils::Raycast(m_Bridge2Vec[i], point1, point2, hitInfo) == true)
				{
					if (m_AccLPos >= m_totalLPos)
					{
						m_LastPos.x = actorShape.left;
						m_LastPos.y = actorShape.bottom + 10;
						m_AccLPos = 0.0f;
					}
					return true;
				}
			}
		}

	}
	return false;
}
bool Level::IsOnWater(const Rectf& actorShape) const
{
	utils::HitInfo hitInfo;
	Point2f point1{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height };
	Point2f point2{ point1.x, actorShape.bottom - 1 };

	if (utils::Raycast(m_Water, point1, point2, hitInfo) == true)
	{
		utils::DrawPolygon(m_Water);
		return true;
	}
	else
	{
		return false;
	}
}
void Level::Reset()
{
	m_Bridge1.Reset();
	m_Bridge2.Reset();
}
Rectf Level::GetBoundaries()
{
	return m_Boundaries;
}
void Level::Death(Rectf& actorshape, Vector2f& actorVelocity)
{
	actorshape.bottom = m_LastPos.y;
	if (actorVelocity.x > 0)
	{
		actorshape.left = m_LastPos.x - 10;
		m_Fell = true;
	}
	else
	{
		actorshape.left = m_LastPos.x + 10;
		m_Fell = true;
	}
}
void Level::DropDown()
{

	m_GoThroughFloor = true;
	m_AccFallTime = 0.0f;
	m_FallHold = true;

	std::cout << m_FallHold << '\n';
}
bool Level::GetHold()
{
	return m_FallHold;
}
void Level::setHold(bool fall)
{
	m_FallHold = fall;
}