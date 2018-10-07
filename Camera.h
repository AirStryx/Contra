#pragma once
#include "stdafx.h"
#include "utils.h"
class SoundManager;
class Camera
{
public:
	Camera(float width, float height);
	~Camera();
	void Update(float elapsedSec);
	void SetBoundaries(const Rectf& boundaries);
	Point2f GetPosition(const Rectf& toTrack);
	Point2f GetPos();
	void ShiftToEnd();
	void Reset();
private:
	float m_Width;
	float m_Height;
	Rectf m_Boundaries;

	Point2f m_Pos;
	bool m_Shift;
	bool m_FinishedShift;

	const float m_ShiftSpeed{ 45.0f };
	Point2f Track(const Rectf& toTrack);
	void Clamp(Point2f& bottomLeftPos);
};