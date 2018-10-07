#include "stdafx.h"
#include "DrawFont.h"
#include "TextureManager.h"


DrawFont::DrawFont(Point2f pos, std::string Text, float scale)
	: m_spFont{TextureManager::GetInstance().GetFont()}
	, m_Text{Text}
	, m_Scale{scale}
{
	m_Pos.bottom = pos.y;
	m_Pos.left = pos.x;
	m_Pos.width = m_spFont->GetWidth() / 26;
	m_Pos.height = m_spFont->GetHeight() / 3;
}
void DrawFont::Draw()
{
	Rectf drawRect = m_Pos;
	drawRect.width = drawRect.width / 4 * m_Scale;
	drawRect.height = drawRect.height / 4 * m_Scale;

	for (size_t i = 0; i < m_Text.size(); i++)
	{
		Rectf source = m_Pos;
		int ascii = m_Text[i];

		if (ascii > 64 && ascii < 91)
		{
			source.left = source.width * (ascii - 65);
			source.bottom = 1 * source.height - 1;
			drawRect.left += drawRect.width;
			drawRect.bottom += 2.0f;
			m_spFont->Draw(drawRect, source);
			drawRect.bottom -= 2.0f;
		}
		if (ascii > 96 && ascii < 123)
		{
			source.left = source.width * (ascii - 97);
			source.bottom = 2 * source.height;
			drawRect.left += drawRect.width;
			m_spFont->Draw(drawRect, source);
		}
		if (ascii > 47 && ascii < 58)
		{
			source.left = source.width * (ascii - 48);
			source.bottom = 0 * source.height;
			drawRect.left += drawRect.width;
			drawRect.bottom += 2.0f;
			m_spFont->Draw(drawRect, source);
			drawRect.bottom -= 2.0f;
		}
		//special signs unordered
		if (ascii == 44)
		{
			source.left = source.width * 10;
			source.bottom = 0 * source.height;
			drawRect.left += drawRect.width;
			drawRect.bottom += 2.0f;
			m_spFont->Draw(drawRect, source);
			drawRect.bottom -= 2.0f;
		}
		if (ascii == 46)
		{
			source.left = source.width * 11;
			source.bottom = 0 * source.height;
			drawRect.left += drawRect.width;
			drawRect.bottom += 2.0f;
			m_spFont->Draw(drawRect, source);
			drawRect.bottom -= 2.0f;
		}
		if (ascii == 39)
		{
			source.left = source.width * 12;
			source.bottom = 0 * source.height;
			drawRect.left += drawRect.width;
			drawRect.bottom += 2.0f;
			m_spFont->Draw(drawRect, source);
			drawRect.bottom -= 2.0f;
		}
		if (ascii == 58)
		{
			source.left = source.width * 13;
			source.bottom = 0 * source.height;
			drawRect.left += drawRect.width;
			drawRect.bottom += 2.0f;
			m_spFont->Draw(drawRect, source);
			drawRect.bottom -= 2.0f;
		}
		if (ascii == 32)
		{
			source.left = source.width * 14;
			source.bottom = 0 * source.height;
			drawRect.left += drawRect.width;
			drawRect.bottom += 2.0f;
			m_spFont->Draw(drawRect, source);
			drawRect.bottom -= 2.0f;
		}
	}
}