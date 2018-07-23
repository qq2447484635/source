#pragma once
#include "Resource.h"
#include "Box2D/Box2D.h"
#include "Component.h"


#pragma comment(lib,"Box2D.lib")
class GameTest
	:public Component
{
public:
	b2World world;
	GameTest():
		world(b2Vec2(0.0f, -10.0f))
	{
		SetVisiable(true);

		b2BodyDef grounddef;
		grounddef.position.Set(4, -10);
		groundshape.SetAsBox(50, 10);
		groundbody = world.CreateBody(&grounddef);
		groundbody->CreateFixture(&groundshape, 0.0f);

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0.4f, 4.0f);
		//bodyDef.angularVelocity = 20;
		boxbody=world.CreateBody(&bodyDef);
		dynamicBox.SetAsBox(1.0, 1.0);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		boxbody->CreateFixture(&fixtureDef);
	}

	b2Body* boxbody;
	b2PolygonShape dynamicBox;
	b2Body* groundbody;
	b2PolygonShape groundshape;


	void Update(double deltatime)
	{
		world.Step(deltatime, 6, 2);
	}

	void Render(ID2D1HwndRenderTarget *rt)
	{

		ComPtr<ID2D1SolidColorBrush> Brush;
		rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray), &Brush);
		D2D1_MATRIX_3X2_F old;
		rt->GetTransform(&old);
		b2Vec2 p;

		p=boxbody->GetPosition();
		rt->SetTransform(
			D2D1::Matrix3x2F::Translation(5+p.x,5+p.y+Application::RESOLUTION_H)*
			D2D1::Matrix3x2F::Rotation(boxbody->GetAngle(),D2D1::Point2F(p.x,p.y)));
		DrawPolygon(dynamicBox.m_vertices, dynamicBox.m_count, rt, Brush.Get());
		rt->SetTransform(old);

		p = groundbody->GetPosition();
		rt->SetTransform(
			D2D1::Matrix3x2F::Translation(p.x+50*5, +10*5+p.y+Application::RESOLUTION_H)*
			D2D1::Matrix3x2F::Rotation(groundbody->GetAngle(), D2D1::Point2F(p.x, p.y)));
		DrawPolygon(groundshape.m_vertices, groundshape.m_count, rt, Brush.Get());
		rt->SetTransform(old);
	}

	void DrawPolygon(b2Vec2 *vexs, int count, ID2D1HwndRenderTarget* rt, ID2D1Brush*Brush)
	{
		b2Vec2 p1 = vexs[count - 1],p2;
		for (int i = 0; i < count; i++)
		{
			p2 = vexs[i];
			rt->DrawLine(
				D2D1::Point2F(p1.x*5,-p1.y*5),
				D2D1::Point2F(p2.x*5,-p2.y*5),
				Brush);
			p1 = p2;
		}
	}
};