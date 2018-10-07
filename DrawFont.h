#pragma once
#include <string>
#include <memory>
#include "Texture.h"
class TextureManager;
class DrawFont
{
public:
	DrawFont(Point2f pos, std::string Text, float scale = 1.0f);
	void Draw();

private:
	std::shared_ptr<Texture> m_spFont;
	Rectf m_Pos;
	float m_Scale;
	std::string m_Text;
};

