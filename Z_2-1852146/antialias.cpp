#include"antialias.h"

void Antialias::set(int windowX, int windowY)
{
	windowW = windowX;
	windowH = windowY;
}
void Antialias::setorigin_p(int x, int y)
{
	originX = x;
	originY = y;
}
void Antialias::drawLine(Line L)
{
	BYTE* Buffer = (BYTE*)GetImageBuffer();
	Point p;//渲染点p
	long long r1, r2;
	double h, d, alpha;
	for (p.x = ((L.p1.x < L.p2.x) ? L.p1.x - L.width : L.p2.x - L.width); p.x <= ((L.p1.x > L.p2.x) ? L.p1.x + L.width : L.p2.x + L.width); p.x++)
	{
		for (p.y = ((L.p1.y < L.p2.y) ? L.p1.y - L.width : L.p2.y - L.width); p.y <= ((L.p1.y > L.p2.y) ? L.p1.y + L.width : L.p2.y + L.width); p.y++)
		{
			//p点在p1点范围
			if ((double)((L.p1.x - p.x) * (L.p1.x - L.p2.x) + (L.p1.y - p.y) * (L.p1.y - L.p2.y)) < 0)
			{
				r1 = (L.p1.x - p.x) * (L.p1.x - p.x) + (L.p1.y - p.y) * (L.p1.y - p.y);//p点到p1点的距离
				d = sqrt(r1) - (double)L.width / 2;
			}
			//p点在p2点范围
			else if ((double)((L.p2.x - p.x) * (L.p2.x - L.p1.x) + (L.p2.y - p.y) * (L.p2.y - L.p1.y)) < 0)
			{
				r2 = (L.p2.x - p.x) * (L.p2.x - p.x) + (L.p2.y - p.y) * (L.p2.y - p.y);//p点到p2点的距离
				d = sqrt(r2) - (double)L.width / 2;
			}
			//p在直线两侧范围
			else
			{
				if (L.p1.x == L.p2.x)
				{
					d = abs(p.x - L.p1.x) - (double)L.width / 2;
				}
				else if (L.p1.y == L.p2.y)
				{
					d = abs(p.y - L.p1.y) - (double)L.width / 2;
				}
				else
				{
					h = (double)((L.p1.x - p.x) * (L.p1.x - L.p2.x) + (L.p1.y - p.y) * (L.p1.y - L.p2.y)) / (double)((L.p1.x - L.p2.x) * (L.p1.x - L.p2.x) + (L.p1.y - L.p2.y) * (L.p1.y - L.p2.y));
					d = ((L.p1.x - p.x) * (L.p1.x - p.x) + (L.p1.y - p.y) * (L.p1.y - p.y)) - ((L.p1.x - L.p2.x) * (L.p1.x - L.p2.x) + (L.p1.y - L.p2.y) * (L.p1.y - L.p2.y)) * h * h;
					d = sqrt(d) - (double)L.width / 2;
				}
			}
			alpha = 0.5 - d;
			if (alpha > 1)
				alpha = 1;
			if (alpha > 0)
			{
				Buffer[(p.y + originY) * windowW * 4 + (p.x + originX) * 4 + 2] = (BYTE)(Buffer[(p.y + originY) * windowW * 4 + (p.x + originX) * 4 + 2] * (1 - alpha) + L.L_RGB.R * alpha);
				Buffer[(p.y + originY) * windowW * 4 + (p.x + originX) * 4 + 1] = (BYTE)(Buffer[(p.y + originY) * windowW * 4 + (p.x + originX) * 4 + 1] * (1 - alpha) + L.L_RGB.G * alpha);
				Buffer[(p.y + originY) * windowW * 4 + (p.x + originX) * 4] = (BYTE)(Buffer[(p.y + originY) * windowW * 4 + (p.x + originX) * 4] * (1 - alpha) + L.L_RGB.B * alpha);
			}
		}
	}
}
void Antialias::drawCircle(Circle C)
{
	BYTE* Buffer = (BYTE*)GetImageBuffer();
	Color BKCOLOR;
	BKCOLOR.R = GetRValue(getbkcolor());
	BKCOLOR.G = GetGValue(getbkcolor());
	BKCOLOR.B = GetBValue(getbkcolor());
	Point p;
	double d, alpha;
	for (p.x = C.P.x - C.R - C.width; p.x <= C.P.x + C.R + C.width; p.x++)
	{
		for (p.y = C.P.y - C.R - C.width; p.y <= C.P.y + C.R + C.width; p.y++)
		{
			d = sqrt((p.x - C.P.x) * (p.x - C.P.x) + (p.y - C.P.y) * (p.y - C.P.y)) - C.R;
			d = abs(d) - (double)C.width / 2;
			alpha = 0.5 - d;
			if (alpha > 1)
				alpha = 1;
			if (alpha > 0)
			{
				Buffer[(p.y + originY) * windowW * 4 + (p.x + originX) * 4 + 2] = (BYTE)(BKCOLOR.R * (1 - alpha) + C.C_RGB.R * alpha);
				Buffer[(p.y + originY) * windowW * 4 + (p.x + originX) * 4 + 1] = (BYTE)(BKCOLOR.G * (1 - alpha) + C.C_RGB.G * alpha);
				Buffer[(p.y + originY) * windowW * 4 + (p.x + originX) * 4] = (BYTE)(BKCOLOR.B * (1 - alpha) + C.C_RGB.B * alpha);
			}
		}
	}
}
void Antialias::floodCircle(Circle C, BYTE mode)
{
	BYTE* Buffer = (BYTE*)GetImageBuffer();
	Color BKCOLOR;
	BKCOLOR.R = GetRValue(getbkcolor());
	BKCOLOR.G = GetGValue(getbkcolor());
	BKCOLOR.B = GetBValue(getbkcolor());
	Point p;
	double d, alpha;
	if (mode == NORMAL)
	{
		for (p.x = C.P.x - C.R - C.width; p.x <= C.P.x + C.R + C.width; p.x++)
		{
			for (p.y = C.P.y - C.R - C.width; p.y <= C.P.y + C.R + C.width; p.y++)
			{
				d = sqrt((p.x - C.P.x) * (p.x - C.P.x) + (p.y - C.P.y) * (p.y - C.P.y));
				if (d <= C.R)
				{
					Buffer[(p.y + originY) * windowW * 4 + (p.x + originX) * 4 + 2] = C.C_RGB.R;
					Buffer[(p.y + originY) * windowW * 4 + (p.x + originX) * 4 + 1] = C.C_RGB.G;
					Buffer[(p.y + originY) * windowW * 4 + (p.x + originX) * 4] = C.C_RGB.B;
				}
				else
				{
					d = d - C.R - (double)C.width / 2;
					alpha = 0.5 - d;
					if (alpha > 1)
						alpha = 1;
					if (alpha > 0)
					{
						Buffer[(p.y + originY) * windowW * 4 + (p.x + originX) * 4 + 2] = (BYTE)(BKCOLOR.R * (1 - alpha) + C.C_RGB.R * alpha);
						Buffer[(p.y + originY) * windowW * 4 + (p.x + originX) * 4 + 1] = (BYTE)(BKCOLOR.G * (1 - alpha) + C.C_RGB.G * alpha);
						Buffer[(p.y + originY) * windowW * 4 + (p.x + originX) * 4] = (BYTE)(BKCOLOR.B * (1 - alpha) + C.C_RGB.B * alpha);
					}
				}
			}
		}
	}
	else if (mode == GRADUAL)
	{
		for (p.x = C.P.x - C.R - C.width; p.x <= C.P.x + C.R + C.width; p.x++)
		{
			for (p.y = C.P.y - C.R - C.width; p.y <= C.P.y + C.R + C.width; p.y++)
			{
				d = sqrt((p.x - C.P.x) * (p.x - C.P.x) + (p.y - C.P.y) * (p.y - C.P.y));
				alpha = 1 - d / C.R;
				if (alpha > 1)
					alpha = 1;
				if (alpha > 0)
				{
					if ((p.y + originY) >= 0 && (p.y + originY) < getheight() && (p.x + originX) >= 0 && (p.x + originX) < getwidth())
					{
						Buffer[(p.y + originY) * windowW * 4 + (p.x + originX) * 4 + 2] = (BYTE)(BKCOLOR.R * (1 - alpha) + C.C_RGB.R * alpha);
						Buffer[(p.y + originY) * windowW * 4 + (p.x + originX) * 4 + 1] = (BYTE)(BKCOLOR.G * (1 - alpha) + C.C_RGB.G * alpha);
						Buffer[(p.y + originY) * windowW * 4 + (p.x + originX) * 4] = (BYTE)(BKCOLOR.B * (1 - alpha) + C.C_RGB.B * alpha);
					}
				}
			}
		}
	}
}