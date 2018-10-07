#pragma once
#include <memory>
#include "Texture.h"
#include "SoundEffect.h"
class TextureManager;
class SoundManager;
class BridgeExplosion
{
public:
	BridgeExplosion();
	~BridgeExplosion();
	void SetPos(Point2f pos);
	void Activate();
	void Draw();
	void Update(float elapsedSec);
	void Reset();

private:
	bool m_Active;
	bool m_Done;

	Point2f m_pos;
	Rectf m_Shape;
	std::shared_ptr<Texture> m_spExplosionTex;

	Rectf m_SourceRect;
	float m_AccFrameTime;
	const float m_FrameTime{ 0.2f };
	int m_Frame;
	bool m_SoundPlayed;
};

