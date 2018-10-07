#include "stdafx.h"
#include "Camera.h"
#include "SoundManager.h"

Camera::Camera(float width, float height)
	: m_Width{width}
	, m_Height{height}
	, m_Boundaries{0,0,width,height}
	, m_Pos{0,0}
	, m_Shift{false}
	, m_FinishedShift{false}
{

}

Camera::~Camera()
{

}

void Camera::SetBoundaries(const Rectf& boundaries)
{
	m_Boundaries = boundaries;
}
void Camera::Update(float elapsedSec)
{
	if (m_Shift)
	{
		m_Pos.x += m_ShiftSpeed * elapsedSec;
	}
}
Point2f Camera::GetPosition(const Rectf& toTrack)
{
	return Track(toTrack);
}

Point2f Camera::Track(const Rectf& toTrack)
{
	if (!m_Shift)
	{
		if (toTrack.left + toTrack.width / 2 >= m_Pos.x + m_Width / 2)
		{
			// -12 to remove camera stutter
			m_Pos.x = toTrack.left + toTrack.width - 12 - m_Width / 2;
		}
	}
	

	Clamp(m_Pos);
	Rectf temp{ m_Pos.x, m_Pos.y, m_Width, m_Height };
	utils::DrawRect(temp);
	return m_Pos;
}

void Camera::Clamp(Point2f& bottomLeftPos)
{
	if (bottomLeftPos.x + m_Width >= m_Boundaries.left + m_Boundaries.width && m_Shift && !m_FinishedShift)
	{
		m_FinishedShift = true;
	}
	if (bottomLeftPos.x <= m_Boundaries.left)
	{
		bottomLeftPos.x = m_Boundaries.left;
	}

	if (bottomLeftPos.y <= m_Boundaries.bottom)
	{
		bottomLeftPos.y = m_Boundaries.bottom;
	}
	if (bottomLeftPos.x + m_Width >= m_Boundaries.left + m_Boundaries.width)
	{
		bottomLeftPos.x = m_Boundaries.width - m_Width;
	}
	if (bottomLeftPos.y + m_Height >= m_Boundaries.bottom + m_Boundaries.height)
	{
		bottomLeftPos.y = m_Boundaries.height - m_Height;
	}
}
Point2f Camera::GetPos()
{
	return m_Pos;
}
void Camera::ShiftToEnd()
{
	m_Shift = true;
}
void Camera::Reset()
{
	m_Pos.x = 0.0f;
	m_Pos.y = 0.0f;
	m_FinishedShift = false;
	m_Shift = false;
}
