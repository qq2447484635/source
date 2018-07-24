#pragma once
#include "Resource.h"
#include "Box2D/Box2D.h"
#include "Framework.h"
#include "RenderData.h"
#include "../Box2D-master/Testbed/Framework/DebugDraw.h"


#pragma comment(lib,"Box2D.lib")
class GameTest
	:public Component
{
public:
	b2World world;
	GameTest() :
		world(b2Vec2(0.0f, -10.0f)),
		kz('Z'),
		kc('C'),
		add('Q')
	{
		SetVisiable(true);

		//Ground
		b2Body* groundbody;
		b2BodyDef grounddef;
		grounddef.position.Set(0, -10);
		groundbody = world.CreateBody(&grounddef);
		b2PolygonShape groundshape;
		groundshape.SetAsBox(60, 10);
		groundbody->CreateFixture(&groundshape, 0.0f);
		//Box
		b2Body* boxbody;
		b2PolygonShape dynamicBox;
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.allowSleep = true;
		bodyDef.position.Set(0.0f, 40.0f);
		boxbody=world.CreateBody(&bodyDef);
		dynamicBox.SetAsBox(2.0, 2.0);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		boxbody->CreateFixture(&fixtureDef);
		boxbody->SetTransform(boxbody->GetPosition(), 30);

		AddComponent(kz);
		AddComponent(kc);
		AddComponent(add);
		kz.KeyPressed = [&]() {cx--; };
		kc.KeyPressed = [&]() {cx++; };
		add.KeyClicked = [&]()
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.position.Set(rand()%40+30, rand()%10+60);
			b2Body *b=world.CreateBody(&bodyDef);
			b2Vec2 ps[5] = {
			b2Vec2(5 + rand() % 3,5 + rand() % 3),
			b2Vec2(5 + +rand() % 3,-5 - rand() % 3),
			b2Vec2(-5 - rand() % 3 ,-5 - rand() % 3),
			b2Vec2(-5 - rand() % 3,5 + rand() % 3),
			b2Vec2(-2,-2)
			};
			b2PolygonShape shape;
			shape.Set(ps, 4);
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &shape;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.3f;
			b->CreateFixture(&fixtureDef);
		};

	}


	KeyListener kz, kc,add;
	int cx=200, cy=100;
	int scalerate = 5;

	void Update(double deltatime)
	{
		Component::Update(deltatime);
		world.Step(deltatime, 6, 2);
	}

	void Render(ID2D1HwndRenderTarget *rt)
	{
		ComPtr<ID2D1SolidColorBrush> Brush;
		rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &Brush);
		D2D1_MATRIX_3X2_F old;
		rt->GetTransform(&old);

		for (b2Body *body = world.GetBodyList(); body != NULL; body = body->GetNext())
		{
			b2Transform xf = body->GetTransform();
			for (b2Fixture *fixture = body->GetFixtureList(); fixture != NULL; fixture = fixture->GetNext())
			{
				ComPtr<ID2D1TransformedGeometry> tgeo;
				b2PolygonShape *b2shape = (b2PolygonShape*)(fixture->GetShape());
				tgeo = b2ShapeToD2DGeometry(rt, (b2PolygonShape*)(b2shape), xf);
				rt->SetTransform(D2D1::Matrix3x2F(1, 0, 0, -1, 0, 0)*D2D1::Matrix3x2F::Translation(Application::RESOLUTION_W/2,Application::RESOLUTION_H-200));
				rt->DrawGeometry(tgeo.Get(), Brush.Get());
			}
		}

		rt->SetTransform(old);
	}



	ComPtr<ID2D1TransformedGeometry> b2ShapeToD2DGeometry(ID2D1HwndRenderTarget *rt,b2PolygonShape *b2shape,b2Transform &xf)
	{
		ComPtr<ID2D1Factory> pFactory;
		ComPtr<ID2D1PathGeometry> Geo;
		rt->GetFactory(&pFactory);
		pFactory->CreatePathGeometry(&Geo);
		ComPtr<ID2D1GeometrySink> Sink;

		Geo->Open(&Sink);

		b2Vec2 p = b2shape->m_vertices[0];
		p=b2Mul(xf, p);
		Sink->BeginFigure(
			D2D1::Point2F(p.x, p.y),
			D2D1_FIGURE_BEGIN_FILLED
		);

		int vcount = b2shape->m_count;
		for (int i = 1; i < vcount; i++)
		{
			p = b2Mul(xf,b2shape->m_vertices[i]);
			Sink->AddLine(D2D1::Point2F(p.x, p.y));
		}

		Sink->EndFigure(D2D1_FIGURE_END_CLOSED);
		Sink->Close();

		ComPtr<ID2D1TransformedGeometry> tgeo;
		pFactory->CreateTransformedGeometry(Geo.Get(), D2D1::Matrix3x2F::Scale(scalerate, scalerate, D2D1::Point2F(0, 0)), &tgeo);
		return tgeo;
	}
};