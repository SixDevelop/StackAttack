#pragma once
#include <ctime>

struct Point
{
public:
	int x;
	int y;
};

/*
0-пустое 
1-блок
2-тело
*/

class Game
{
private:
	static const int cols = 30;
	static const int rows = 39;
	Point body;
	unsigned char mas[rows][cols];
	Point block;

public:

	Game() : mas{0}
	{
		for (int i = 0; i < 30; ++i)
			mas[38][i] = 1;
		body.x = 14;
		body.y = 12;
	}


	void Move(char);
	void Drop();
	int Random();
	void Show();
};

