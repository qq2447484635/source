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
		pos.x = rand() % Application::RESOLUTION_W-100;
		pos.y = -rand()%Application::RESOLUTION_H;
		v.x = rand()%50+50;
		v.y = rand() % 150 + 300;
	}
};

class Rain :
	public Component
{
public:
	Rain()
	{
		SetVisiable(true);
	}

	~Rain();

	std::list<Tear> tears;

	void Render(ID2D1HwndRenderTarget *rt)
	{
		Component::Render(rt);
		ComPtr<ID2D1SolidColorBrush> brush;
		rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);
		brush->SetOpacity(0.6);
		for (list<Tear>::iterator i = tears.begin(); i != tears.end(); i++)
		{
			rt->FillEllipse(D2D1::Ellipse(D2D1::Point2F(i->pos.x, i->pos.y), 1, 5), brush.Get());
		}
	}

	void Update(double deltatime)
	{
		Component::Update(deltatime);
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

	void AddTears(int num)
	{
		for (int i = 0; i < num; i++)
		{
			tears.push_back(Tear());
		}
	}

	void ReduceTears(int num)
	{
		if (tears.size() >= num)
		{
			for (int i = 0; i < num; i++)
			{
				tears.pop_back();
			}
		}
	}
};

