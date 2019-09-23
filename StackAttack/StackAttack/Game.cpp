#include "Game.h"
#include <ctime>
#include <cstdlib>
#include<iostream>
void Game::Move(char button)//moving player
{
	
	
	switch (button)
	{
		case 'w':
			if (mas[body.y - 1][body.x] != 1 && body.y-1>=0 && body.y+1	!=0)
				body.y++;
			break;
		case 'a':
			if (mas[body.y][body.x - 1] != 1 && body.x - 1 >= 0)
				body.x--;
			else if (mas[body.y][body.x - 1] == 1 && body.x - 1 >= 0)
			{
				if (mas[body.y][body.x - 2] == 0 && body.x - 2 >= 0)
				{
					mas[body.y][body.x - 1] = 1;
					body.x--;
				}
				//else if(mas[body.y][body.x - 2] != 0 && body.x - 2 >= 0)

			}
			break;
		case 'd':
			if (mas[body.y][body.x + 1] != 1 && body.x + 1 <29)
				body.x++;
			else if (mas[body.y][body.x + 1] == 1 && body.x + 1 <29)
			{
				if (mas[body.y][body.x + 2] == 0 && body.x + 2 <29)
				{
					mas[body.y][body.x + 1] = 1;
					body.x++;
				}
				//else if(mas[body.y][body.x + 2] != 0 && body.x +2 <29)

			}
			break;


			
	}

}


void Game::Drop()
{
	body.x=this->Random();

	int blocks = 1;
	for(int i=0;i<rows-2;i++)
		for (int j = 0; j < cols; j++)
		{
			if (mas[i][j] == 1)
				blocks++;

		}
	for(int i=0;i<rows-2;i++)
		for (int j = 0; j < cols; j++)
		{
			if (mas[i + 1][j] == 0)
			{
				mas[i][j] = 0;
				mas[i + 1][j] = 1;
			}
			if (i == 0 && j == body.x)
				mas[i][j] = 1;
		}
	

}


int Game::Random()
{
	std::srand(std::time(NULL));
	return rand() % 30;
}

void Game::Show()
{
	for(int i=0;i<rows;i++)
		for (int j = 0; j < cols; j++)
		{
			if (mas[i][j] == 0)
				std::cout << " ";
			else if (mas[i][j] == 1)
				std::cout << (char)(219);
			else if (
				(body.y == i && body.x == j) || 
				(body.y-1 == i && body.x == j))
				std::cout << "0";
		}

}