#include "stdafx.h"
#include "Bridge.h"
#include "TextureManager.h"


Bridge::Bridge(Point2f botLeftPos)
	: m_BotLeft{botLeftPos}
	, m_spLeftTex{ TextureManager::GetInstance().GetBridge01()}
	, m_spMidTex{ TextureManager::GetInstance().GetBridge02() }
	, m_spRightTex{ TextureManager::GetInstance().GetBridge03() }
	, m_IsRed{false}
	, m_NumDeleted{0}
	, m_Destroy{false}
	, m_AccBreakTime{10.0f}
	, m_spBridgeEx1{ std::make_shared<BridgeExplosion>() }
	, m_spBridgeEx2{ std::make_shared<BridgeExplosion>() }
	, m_spBridgeEx3{ std::make_shared<BridgeExplosion>() }
	, m_spBridgeEx4{ std::make_shared<BridgeExplosion>() }
{
	InitRects();
	InitExpl();
}


Bridge::~Bridge()
{
}
void Bridge::Update(float elapsedSec)
{
	m_spBridgeEx1->Update(elapsedSec);
	m_spBridgeEx2->Update(elapsedSec);
	m_spBridgeEx3->Update(elapsedSec);
	m_spBridgeEx4->Update(elapsedSec);
	m_AccTime += elapsedSec;
	if (m_AccTime >= m_ChangeTime)
	{
		m_AccTime = 0.0f;
		if (m_IsRed)
		{
			m_SourceRect.left = 0.0f;
			m_IsRed = false;
		}
		else
		{
			m_SourceRect.left = m_spLeftTex->GetWidth() / 2;
			m_IsRed = true;
		}
	}
	if (m_Destroy && m_NumDeleted < 4)
	{
		m_AccBreakTime += elapsedSec;
		if (m_AccBreakTime >= m_BreakTime)
		{
			m_AccBreakTime = 0;
			m_NumDeleted += 1;
		}
	}
}
void Bridge::Draw()
{
	switch (m_NumDeleted)
	{
	case 0:
		m_spLeftTex->Draw(m_Bridgep1, m_SourceRect);
		m_spMidTex->Draw(m_Bridgep2, m_SourceRect);
		m_spMidTex->Draw(m_Bridgep3, m_SourceRect);
		m_spRightTex->Draw(m_Bridgep4, m_SourceRect);
		break;
	case 1:
		m_spMidTex->Draw(m_Bridgep2, m_SourceRect);
		m_spMidTex->Draw(m_Bridgep3, m_SourceRect);
		m_spRightTex->Draw(m_Bridgep4, m_SourceRect);
		break;
	case 2:
		m_spMidTex->Draw(m_Bridgep3, m_SourceRect);
		m_spRightTex->Draw(m_Bridgep4, m_SourceRect);
		break;
	case 3:
		m_spRightTex->Draw(m_Bridgep4, m_SourceRect);
		break;
	default:
		break;
	}
	m_spBridgeEx1->Draw();
	m_spBridgeEx2->Draw();
	m_spBridgeEx3->Draw();
	m_spBridgeEx4->Draw();
	
}
void Bridge::BlowUp()
{
	// create explosion

	m_Destroy = true;
}

std::vector<std::vector<Point2f>> Bridge::GetBridge()
{
	std::vector<std::vector<Point2f>> points;
	switch (m_NumDeleted)
	{
	case 0:
		points.push_back(m_Bridgep1.GetVertices());
		points.push_back(m_Bridgep2.GetVertices());
		points.push_back(m_Bridgep3.GetVertices());
		points.push_back(m_Bridgep4.GetVertices());
		break;
	case 1:
		m_spBridgeEx1->Activate();
		points.push_back(m_Bridgep2.GetVertices());
		points.push_back(m_Bridgep3.GetVertices());
		points.push_back(m_Bridgep4.GetVertices());
		break;
	case 2:
		m_spBridgeEx2->Activate();
		points.push_back(m_Bridgep3.GetVertices());
		points.push_back(m_Bridgep4.GetVertices());
		break;
	case 3:
		m_spBridgeEx3->Activate();
		points.push_back(m_Bridgep4.GetVertices());
		break;
	case 4:
		m_spBridgeEx4->Activate();
		break;
	default:
		break;
	}
	
	for (size_t i = 0; i < points.size(); i++)
	{
		for (size_t t = 0; t < points[i].size(); t++)
		{
			points[i][t].y -= 5;
		}
	}
	return points;
}
void Bridge::InitRects()
{
	m_Bridgep1.bottom = m_BotLeft.y;
	m_Bridgep1.left = m_BotLeft.x;
	m_Bridgep1.width = m_spLeftTex->GetWidth() / 2;
	m_Bridgep1.height = m_spLeftTex->GetHeight();

	m_Bridgep2.bottom = m_BotLeft.y;
	m_Bridgep2.left = m_BotLeft.x + (m_spLeftTex->GetWidth() / 2);
	m_Bridgep2.width = m_spLeftTex->GetWidth() / 2;
	m_Bridgep2.height = m_spLeftTex->GetHeight();

	m_Bridgep3.bottom = m_BotLeft.y;
	m_Bridgep3.left = m_BotLeft.x + ((m_spLeftTex->GetWidth() / 2) * 2);
	m_Bridgep3.width = m_spLeftTex->GetWidth() / 2;
	m_Bridgep3.height = m_spLeftTex->GetHeight();

	m_Bridgep4.bottom = m_BotLeft.y;
	m_Bridgep4.left = m_BotLeft.x + ((m_spLeftTex->GetWidth() / 2) * 3);
	m_Bridgep4.width = m_spLeftTex->GetWidth() / 2;
	m_Bridgep4.height = m_spLeftTex->GetHeight();

	m_SourceRect.bottom = 0;
	m_SourceRect.left = 0;
	m_SourceRect.width = m_spLeftTex->GetWidth() / 2;
	m_SourceRect.height = m_spLeftTex->GetHeight();
}
void Bridge::InitExpl()
{
	m_spBridgeEx1->SetPos(Point2f(m_Bridgep1.left, m_Bridgep1.bottom));
	m_spBridgeEx2->SetPos(Point2f(m_Bridgep2.left, m_Bridgep2.bottom));
	m_spBridgeEx3->SetPos(Point2f(m_Bridgep3.left, m_Bridgep3.bottom));
	m_spBridgeEx4->SetPos(Point2f(m_Bridgep4.left, m_Bridgep4.bottom));
}
void Bridge::Reset()
{
	m_Destroy = false;
	m_NumDeleted = 0;
	m_spBridgeEx1->Reset();
	m_spBridgeEx2->Reset();
	m_spBridgeEx3->Reset();
	m_spBridgeEx4->Reset();
}