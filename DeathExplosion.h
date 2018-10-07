#pragma once
#include <memory>
#include "Texture.h"
class TextureManager;
class SoundManager;
class DeathExplosion
{
public:
	DeathExplosion();
	void Draw() const;
	void Activate();
	void Update(float elapsedSec);
	void SetPos(Point2f pos);
private:
	bool m_Done;
	bool m_Active;

	Point2f m_pos;
	Rectf m_Shape;
	std::shared_ptr<Texture> m_spExplosionTex;

	Rectf m_SourceRect;
	float m_AccFrameTime;
	const float m_FrameTime{ 0.2f };
	int m_Frame;
};

