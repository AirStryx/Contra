#pragma once
#include "Texture.h"
#include <memory>
#include "BridgeExplosion.h"
class TextureManager;
class Bridge
{
public:
	Bridge(Point2f botLeft);
	~Bridge();
	void Update(float elapsedSec);
	void Draw();
	void BlowUp();
	void Reset();
	std::vector<std::vector<Point2f>> GetBridge();

private:
	Point2f m_BotLeft;

	std::shared_ptr<Texture> m_spLeftTex;
	std::shared_ptr<Texture> m_spMidTex;
	std::shared_ptr<Texture> m_spRightTex;

	Rectf m_Bridgep1;
	Rectf m_Bridgep2;
	Rectf m_Bridgep3;
	Rectf m_Bridgep4;

	std::shared_ptr<BridgeExplosion> m_spBridgeEx1;
	std::shared_ptr<BridgeExplosion> m_spBridgeEx2;
	std::shared_ptr<BridgeExplosion> m_spBridgeEx3;
	std::shared_ptr<BridgeExplosion> m_spBridgeEx4;

	Rectf m_SourceRect;

	//color change bridge
	float m_AccTime;
	const float m_ChangeTime{ 1.0f };
	bool m_IsRed;

	// explosion / destroy
	int m_NumDeleted;
	bool m_Destroy;

	float m_AccBreakTime;
	const float m_BreakTime{ 1.0f };

	void InitRects();
	void InitExpl();
};

