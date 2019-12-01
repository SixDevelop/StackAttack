#include<iostream>;
#include "Game.h";
#include<conio.h>;
#include <ctime>;
// constants stand for settings & iterating over scene grid


class StackAttack
{
private:
	static const int rows = 7;
	static const int cols = 12;

	char sceneGrid[rows][cols];
	Point player;

	bool UpdatePlayer(const int& input)
	{
		char playerSideBottom;
		char playerSideUp;
		char playerUp;
		switch (input)
		{
		case 'w':
			// player jumps
			// check for blocks & destroy if there is one
			if(sceneGrid[player.x+1][player.y] == '#' || (player.x) == rows - 1)
			{	
				if (sceneGrid[player.x - 1][player.y] == '#') // Если сверху блок - разбиваем
				{
					sceneGrid[player.x - 1][player.y] == '*';
					player.x -= 1;
					return true;
				}
				else  // просто зависаем
				{
					player.x -= 1;
					return true;
				}
			}
			else if (sceneGrid[player.x + 1][player.y] == '*') // снизу пусто - падаем
			{
				player.x += 1;
				return true;
			}
			else 
				return false;

			

		case 'd':
			// player moves right
			// check for blocks & try to move block & move player
			if (!(player.y < cols - 1)) // Если хотим просто сдвигать в бок за поле
				return true;
			
			playerSideBottom = sceneGrid[player.x][player.y + 1]; // правая клетка
			 playerSideUp = sceneGrid[player.x - 1][player.y + 1]; // голова правой клетки
			
			if (playerSideBottom == '*' )
			{
				if (playerSideUp == '*')
				{
					if (sceneGrid[player.x + 1][player.y + 1] == '*')
					{
						player.y += 1;
						player.x += 1;
						return true;
					}
					else
					{
						player.y += 1;
						return true;
					}

				}
				else 
				{
					return false;
				}

			}
			else
			{
				if (sceneGrid[player.x - 1][player.y + 1] == '*')
				{
					if (sceneGrid[player.x][player.y + 2] == '*')
					{
						sceneGrid[player.x][player.y + 2] = '#';
						sceneGrid[player.x][player.y + 1] = '*';
						player.y += 1;
						return true;
					}
					else
					{
						return true;
					}
				}
				else
					return true;

				
			}
			
			return true;
		case 'a':
			// player moves left
			// check for blocks & try to move block & move player
			if (!player.y > 0)
				return true;

			 playerSideBottom = sceneGrid[player.x][player.y - 1];
			 playerSideUp = sceneGrid[player.x - 1][player.y - 1];

			if (playerSideBottom == '*') // рядом пусто
			{
				if (playerSideUp == '*')  // сверху пусто
				{
					if (sceneGrid[player.x + 1][player.y - 1] == '*')
					{
						player.x += 1;
						player.y -= 1;
						return true;
					}
					else
					{
						player.y -= 1;
						return true;
					}
				}
				else 
					return false;

			}
			else
			{
				if (sceneGrid[player.x - 1][player.y - 1] == '*')
				{
					if (sceneGrid[player.x][player.y - 2] == '*')
					{
						sceneGrid[player.x][player.y - 2] = '#';
						sceneGrid[player.x][player.y - 1] = '*';
						player.y -= 1;
						return true;
					}
					else
					{
						return true;
					}
				}
				else
					return true;

				
			}
			return true;
		default: 
			return false;
		}
	}

public:
	StackAttack()
	{
		// initilize scene grid with empty space
		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < cols; ++j)
				sceneGrid[i][j] = '*';

		// spawn play & align it vertically & horizontally (imperfect as grid width is not even)
		player = { 6, 6 };

		// draw player renders
		// sceneGrid[6][6] = '%';
		// sceneGrid[5][6] = '%';
	}

	void Render()
	{
		int c = 0;
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				
				if ((player.x - 1 == i && player.y == j) ||
					(player.x == i && player.y == j))
				{
					std::cout << "%";
				}
				else
					std::cout << sceneGrid[i][j];

				
			}
			std::cout << std::endl;
		}
	}


	void block_rand()
	{
		srand(time(NULL));
		int y = rand() % cols;
		sceneGrid[0][y] = '#';
	}



	bool Update(const char& input)
	{
		if (UpdatePlayer(input))
		{
			// scenegrid[player]
			// проверить, есть ли полный нижний ряд, убрать его, сдвинуть матрицу на строку вниз

			for (int i = 0; i < rows; i++)
			{
				for(int j = 0; j < cols; j++)
				{
					if (player.x-1 == 0 || player.x-1 == 1)  // если на верхних 2 линиях - сразу проиграл 
						return false;
					else
					{
						if (sceneGrid[i][j] == '#' && i != rows - 1) // нашли кубик
						{
							if (sceneGrid[i + 1][j] == '*' ) // если под ним пусто - падает на 1 
							{
								sceneGrid[i][j] = '*';
								sceneGrid[i + 1][j] = '#';
								i = i + 1;
							}
							else // # or floor под ним кубик или пол
							{
								return true;
							}
							
						}
					}

				}
			}


		}
		return true;
	}

	
};

/*
	Game: 
		public: 
			Update(input) : takes player input handled in main & runs update for all gameobjects in the scene
				Handle input (send data to Player Game Object)
				If input is correct than run update for block game objects (block logic)
				Update render buffer 
			Game() : constructor that initializes empty scene & spawns player
*/

int main()
{
	// Game a;
	// 
	// do
	// {
	// 	a.Show();
	// 	a.Move(_getch());
	// 	a.Drop();
	// 
	// 
	// 	system("cls");
	// } while (true);

	StackAttack game;

	char input;
	int counter = 0;
	do
	{
		if (counter % 5 == 0)
		{
			game.block_rand();
		}
		system("cls");
		game.Render();
		
		
		//std::cin >> input;
		
		counter++;
	} while (game.Update(_getch()));

	std::cout << std::endl << "You lost" << std::endl;

	return 0;
}