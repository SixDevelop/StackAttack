#include<iostream>;
#include<conio.h>;
#include <ctime>;
// constants stand for settings & iterating over scene grid

struct Point
{
public:
	int x;
	int y;
};

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
				if (sceneGrid[player.x - 2][player.y] == '#') // Если сверху блок - разбиваем
				{
					for (int i = player.x; i > 1; i--) {
						sceneGrid[i][player.y] = '*';
					}
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
	short score = 0;

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

			if (player.x - 1 == 0 || player.x - 1 == 1)  // если на верхних 2 линиях - сразу проиграл 
				return false;

			for (int i = rows - 2; i > -1; --i)
			{
				for (int j = cols - 1; j > -1; --j)
				{
					if (sceneGrid[i][j] == '#')
					{
						if (sceneGrid[i + 1][j] == '*')
						{
							sceneGrid[i][j] = '*';
							sceneGrid[i + 1][j] = '#';
						}
						if(sceneGrid[i+1][j] == sceneGrid[player.x-1][player.y])
						{
							return false;
						}

						/*switch (sceneGrid[i+1][j])
						{
						case '*':
							sceneGrid[i][j] = '*';
							sceneGrid[i + 1][j] = '#';
							break;
						case '%':
							return false;
							break;

						default:
							break;
						}*/
					}
					
				}
			}
			ProcessBottomLine();
		}
		return true;
	}

	void ProcessBottomLine()
	{
		bool canDelete = true;
		for (int i = 0; i < cols; ++i)
		{
			if (sceneGrid[rows - 1][i] != '#')
			{
				canDelete = false;
				break;
			}
		}

		if (canDelete) {
			for (int i = 0; i < cols; ++i)
				sceneGrid[rows - 1][i] = '*';
			score++;
		}
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

	std::cout << std::endl << "You lost, your score is " << game.score << std::endl;

	return 0;
}