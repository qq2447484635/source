#pragma once
#include <list>
#include "Framework.h"
class Tear
{
public:
	struct Point2F
	{
		float x;
		float y;
	}v,pos;
	Tear()
	{
		pos.x = rand() % Application::RESOLUTION_W;
		pos.y = -rand()%Application::RESOLUTION_H;
		v.x = 0.0;
		v.y = rand() % 150 + 300;
	}
};

class Rain
{
public:
	Rain()
	{
		for (int i = 0; i < 400; i++)
		{
			tears.push_back(Tear());
		}
	}
	~Rain();
	std::list<Tear> tears;
	void render(ID2D1HwndRenderTarget *rt,ID2D1Brush *brush)
	{
		for (list<Tear>::iterator i = tears.begin(); i != tears.end(); i++)
		{
			rt->FillEllipse(D2D1::Ellipse(D2D1::Point2F(i->pos.x, i->pos.y), 1, 5), brush);
		}
	}

	void update(double deltatime)
	{
		for (list<Tear>::iterator i = tears.begin(); i != tears.end();)
		{
			i->pos.x += i->v.x*deltatime;
			i->pos.y += i->v.y*deltatime;
			if (i->pos.y > Application::RESOLUTION_H)
			{
				tears.push_front(Tear());
			}
			if (i->pos.y > Application::RESOLUTION_H)
			{
				tears.erase(i++); continue;
			}
			i++;
		}
	}

};

