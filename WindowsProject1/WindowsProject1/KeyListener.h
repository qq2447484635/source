#pragma once

#include <list>
#include "Component.h"

typedef char KEY;

class KeyListener:
	public Component
{
public:

	KeyListener(KEY _key)
	{
		key = _key;
	};

	void Update(double deltatime)
	{
		auto state = GetKeyState(key);
		if (state < 0 && LastKeyState>=0)
		{
			KeyClicked();
		}
		else if (state< 0)
		{
			KeyPressed();
		}
		else if (state >= 0 && LastKeyState<0)
		{
			KeyReleased();
		}
		LastKeyState = state;
	};

	void AttachTo(Component *c)
	{
		c->AddComponent(this);
	};

	void AttachTo(Component &c)
	{
		c.AddComponent(this);
	};

	typedef std::function<void()> KeyEvent;
	KeyEvent KeyClicked = []() {};
	KeyEvent KeyPressed = []() {};
	KeyEvent KeyReleased = []() {};
private:
	KEY key;
	int LastKeyState = 0;

};