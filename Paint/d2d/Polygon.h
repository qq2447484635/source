#pragma once
#include <math.h>
class Vec2
{
public:
	float x, y;
	float operator * (Vec2 b)const
	{
		return x * b.x + y * b.y;
	};

	Vec2 operator * (float b)const
	{
		Vec2 Val = {x*b,y*b};
		return Val;
	};

	Vec2 operator + (Vec2 b)const
	{
		Vec2 Val;
		Val.x = x + b.x;
		Val.y = y + b.y;
		return Val;
	};

	float operator ()()const
	{
		return sqrt(x*x + y * y);
	};

	Vec2 operator += (Vec2 &b)
	{
		x += b.x;
		y += b.y;
	}

	static float Arc(Vec2 a, Vec2 b)
	{
		return acos((a*b) / (a()*b()));
	};
	

};

class Polygon
{
public:
	Polygon();
	~Polygon();
};

