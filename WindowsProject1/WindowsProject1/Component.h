#pragma once
#include <list>

using namespace std;

 class Component
{
public:

	virtual void Update(double deltatime) 
	{
			for (std::list<Component*>::iterator i = components.begin(); i != components.end(); i++)
			{
				(*i)->Update(deltatime);
			}
	};

	virtual void Render(ID2D1HwndRenderTarget *rt)
	{
			for (std::list<Component*>::iterator i = components.begin(); i != components.end(); i++)
			{
				(*i)->Render(rt);
			}
	};

	void AddComponent(Component &c) { components.push_back(&c); };

protected:
	list<Component*> components;

 };