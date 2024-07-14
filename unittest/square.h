#pragma once
struct Square
{
	int mX;
	int mY;
	unsigned int mL;

	Square(int x, int y, int l): mX(x), mY(y), mL(l) {};
	int perimeter()
	{
		return 4*mL;
	}
	int area()
	{
		return mL*mL;
	}
	void scale( int s)
	{
		mL *= s;
	}
};

