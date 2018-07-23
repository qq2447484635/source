#pragma once
#include <list>

using namespace std;

 class Component
{
public:
	Component()
	{
		bVisiable = false;
	};

	bool isVisiable()
	{
		return bVisiable;
	};

	void SetVisiable(bool s)
	{
		bVisiable = s;
	};

	virtual void Update(double deltatime) 
	{
		for (std::list<Component*>::iterator i = components.begin(); i != components.end(); i++)
		{
			(*i)->Update(deltatime);
		}
	};

	virtual void Render(ID2D1HwndRenderTarget *rt)
	{
		if (!isVisiable()) return;
		for (std::list<Component*>::iterator i = components.begin(); i != components.end(); i++)
		{
			(*i)->Render(rt);
		}
	};

	void AddComponent(Component &c) { components.push_back(&c); };
	void AddComponent(Component *c) { components.push_back(c); };

protected:
	list<Component*> components;

private:
	bool bVisiable;
 };