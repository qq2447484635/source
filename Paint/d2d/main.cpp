#include "Canvas2D.h"
#pragma comment(lib,"winmm.lib")
#include <deque>

int main()
{
	Canvas2D::Show();
	return 0;
}

class Bezier
{
public:
	std::vector<Vec2> P;
	Vec2 point[3];
	Vec2 v[3];
	RECT *Rect;
	void init(RECT *R);
	void Updata(float DeltaTime);
	void OnRender(ID2D1RenderTarget *RT);
	void CalculatePoint();
};

void Bezier::init(RECT *R)
{
	Rect = R;
	memset(point, 0, sizeof(point));
	v[0] = { 100 + rand() % 100 * 1.0f,100 + rand() % 100 * 1.0f };
	v[1] = { 100 + rand() % 100 * 1.0f,100 + rand() % 100 * 1.0f };
	v[2] = { 100 + rand() % 100 * 1.0f,100 + rand() % 100 * 1.0f };
}

void Bezier::Updata(float DeltaTime)
{
	for (int i = 0; i < 3; i++)
	{
		point[i].x += v[i].x*DeltaTime;
		point[i].y += v[i].y*DeltaTime;
		if (point[i].x > Rect->right / 2.0 || point[i].x < -Rect->right / 2.0)
		{
			v[i].x *= -1;
		}
		if (point[i].y > Rect->bottom / 2.0 || point[i].y < -Rect->bottom / 2.0)
		{
			v[i].y *= -1;
		}
	}
	CalculatePoint();
}

void Bezier::OnRender(ID2D1RenderTarget * RT)
{
	ID2D1SolidColorBrush *Brush = NULL;
	RT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &Brush);
	for (int i = 0; i < P.size() - 1; i++)
	{
		RT->DrawLine(D2D1::Point2F(P[i].x, P[i].y), D2D1::Point2F(P[i + 1].x, P[i + 1].y), Brush);
	}
	SafeRelease(&Brush);
}

void Bezier::CalculatePoint()
{
	Vec2 B;
	P.clear();
	for (float t = 0; t < 1.0; t+=0.02f)
	{
		B = point[0] * pow(1 - t, 2) + point[1] * ((1 - t) * 2 * t) + point[2] * t*t;
		P.push_back(B);
	}
}

void Canvas2D::FrameUpdata(float DeltaTime)
{
	Scale = D2D1::Matrix3x2F::Scale(ScaleRate, ScaleRate, D2D1::Point2F(Rect.right / 2.0, Rect.bottom / 2.0));
	Extra = D2D1::Matrix3x2F(1, 0, -1, 1, 0, 0);
	RT->SetTransform(ToScreemCenter*Position*Scale);
	RT->Clear(D2D1::ColorF(D2D1::ColorF::White));
	
	{//Mesh
		for (int i = -2000; i < 2000; i += 50)
		{
			Brush->SetColor(D2D1::ColorF(D2D1::ColorF::LightGray));
			RT->DrawLine(D2D1::Point2F(i, 2000), D2D1::Point2F(i, -2000), Brush);
		}
		for (int i = -2000; i < 2000; i += 50)
		{
			RT->DrawLine(D2D1::Point2F(2000, i), D2D1::Point2F(-2000, i), Brush);
		}
	}

	{
			D2D1_SIZE_F Axissize;
			AxisRT->GetBitmap(&Axis);
			Axissize = Axis->GetSize();
			RT->DrawBitmap(Axis, D2D1::RectF(-Axissize.width / 2.0, -Axissize.height / 2.0, Axissize.width / 2.0, Axissize.height / 2.0));
	}

	{
		D2D1_SIZE_F papersize;
		MousePainting(DeltaTime);
		bitmapRT->GetBitmap(&Paper);
		if (Paper)
		{
			papersize = Paper->GetSize();
			RT->DrawBitmap(Paper, D2D1::RectF(-papersize.width / 2.0, -papersize.height / 2.0, papersize.width / 2.0, papersize.height / 2.0));
		}
		SafeRelease(&Paper);
	}


	{
		float dx=100, dy=-100;
		float x, y;
		float v = 180;
		static float Time = 0.0f;
		float total_time;
		Time += DeltaTime;
		total_time = 14 * 50 / v;
		if (Time > total_time)
		{
			Time -= total_time;
		}

		if (Time<total_time*3.f / 14.f)
		{
			x = dx; y = -v * Time +dy;
		}
		else if (Time<total_time * 11 / 14.f)
		{
			x = (Time - total_time * 3 / 14.f) *v + dx; y = dy-150.f;
		}
		else
		{
			x = dx+400.f; y = v * (Time - 11.f / 14.f*total_time) + dy-150.f;
		}
		Vec2 p;
		std::deque<Vec2>points;
		p = { dx,dy };
		points.push_back(p);
		p = { x,y };
		RT->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), 3, 3), Brush);
		points.push_back(p);
		p = { dx+400,dy };
		points.push_back(p);
		std::vector<Vec2> L;
		for (float t = 0.0f; t < 1.0f; t += 0.01)
		{
			L.push_back(BezierV(points, 2, t));
		}
		for (int i = 0; i < L.size() - 1; i++)
		{
			RT->DrawLine(D2D1::Point2F(L[i].x, L[i].y), D2D1::Point2F(L[i + 1].x, L[i + 1].y), Brush);
		}
	}

	{
		float dx = 500, dy = -100;
		float x, y;
		float v = 180;
		static float Time = 0.5f;
		float total_time;
		Time += DeltaTime;
		total_time = 14 * 50 / v;
		if (Time > total_time)
		{
			Time -= total_time;
		}

		if (Time<total_time*3.f / 14.f)
		{
			x = dx; y = -v * Time + dy;
		}
		else if (Time<total_time * 11 / 14.f)
		{
			x = (Time - total_time * 3 / 14.f) *v + dx; y = dy - 150.f;
		}
		else
		{
			x = dx + 400.f; y = v * (Time - 11.f / 14.f*total_time) + dy - 150.f;
		}
		Vec2 p;
		std::deque<Vec2>points;
		p = { dx,dy };
		points.push_back(p);
		p = { x,y };
		RT->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), 3, 3), Brush);
		points.push_back(p);
		p = { dx + 400,dy };
		points.push_back(p);
		std::vector<Vec2> L;
		for (float t = 0.0f; t < 1.0f; t += 0.01)
		{
			L.push_back(BezierV(points, 2, t));
		}
		for (int i = 0; i < L.size() - 1; i++)
		{
			RT->DrawLine(D2D1::Point2F(L[i].x, L[i].y), D2D1::Point2F(L[i + 1].x, L[i + 1].y), Brush);
		}
	}

	{
		float dx = 900, dy = -100;
		float x, y;
		float v = 180;
		static float Time = 1.0f;
		float total_time;
		Time += DeltaTime;
		total_time = 14 * 50 / v;
		if (Time > total_time)
		{
			Time -= total_time;
		}

		if (Time<total_time*3.f / 14.f)
		{
			x = dx; y = -v * Time + dy;
		}
		else if (Time<total_time * 11 / 14.f)
		{
			x = (Time - total_time * 3 / 14.f) *v + dx; y = dy - 150.f;
		}
		else
		{
			x = dx + 400.f; y = v * (Time - 11.f / 14.f*total_time) + dy - 150.f;
		}
		Vec2 p;
		std::deque<Vec2>points;
		p = { dx,dy };
		points.push_back(p);
		p = { x,y };
		RT->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), 3, 3), Brush);
		points.push_back(p);
		p = { dx + 400,dy };
		points.push_back(p);
		std::vector<Vec2> L;
		for (float t = 0.0f; t < 1.0f; t += 0.01)
		{
			L.push_back(BezierV(points, 2, t));
		}
		for (int i = 0; i < L.size() - 1; i++)
		{
			RT->DrawLine(D2D1::Point2F(L[i].x, L[i].y), D2D1::Point2F(L[i + 1].x, L[i + 1].y), Brush);
		}
	}

	{
		return;
		static Bezier b[10];
		static bool binit = false;
		if (!binit)
		{
			for (int i=0; i < 10; i++)
			{
				b[i].init(&Rect);
			}
			binit = true;
		}
		for (int j = 1; j<10; j++)
		{
			b[j].point[1] = b[0].point[1];
			b[j].point[2] = b[0].point[2];
		}
		for (int i = 0; i < 10; i++)
		{
	
			b[i].Updata(DeltaTime);
			b[i].point[1] = { 0,0 };
			b[i].point[2] = { 0,600 };
			b[i].OnRender(RT);
		}
		
	}

}

void Canvas2D::OnUpdata(float DeltaTime)
{
	if (GetKeyState('W') < 0)
	{
		y -= 400 * DeltaTime;
	}
	if (GetKeyState('S') < 0)
	{
		y += 400 * DeltaTime;
	}
	if (GetKeyState('A') < 0)
	{
		x -= 400 * DeltaTime;
	}
	if (GetKeyState('D') < 0)
	{
		x += 400 * DeltaTime;
	}
	if (GetKeyState('C') < 0)
	{
		bitmapRT->BeginDraw();
		bitmapRT->Clear();
		bitmapRT->EndDraw();
	}
	if (GetKeyState('R') < 0)
	{
		x = 0; y = 0;
	}
	static bool Q_Pressed = false, E_Pressed = false;
	if (GetKeyState('E') < 0)
	{
		if (!E_Pressed&&stage>1)
		{
			stage--;
		}	
		E_Pressed = true;
	}
	else
	{
		E_Pressed = false;
	}
	if (GetKeyState('Q') < 0)
	{
		if (!Q_Pressed)
		{
			stage++;
		}
		Q_Pressed = true;
	}
	else
	{
		Q_Pressed = false;
	}
}

