#pragma once
#include "App.h"
#include "Rain.h"
class DemoApp:
	public App
{
public:
	DemoApp();
	~DemoApp();
	void Render();
	void Update(double);
private:
	D2D1_POINT_2F center;
	int length;
	float Current_Radian;
	float RadianPerSec;
	const float PI = 3.1415926;
	const float _2PI = 3.1415926 * 2;
	Rain rain;
};

