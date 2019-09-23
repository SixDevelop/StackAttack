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
			if((sceneGrid[player.x+1][player.y]!=' ' && player.x+1<rows) || (player.x==rows-1) )
			{	
				playerUp = sceneGrid[player.x][player.y - 2];

			// destroy block
				if (playerUp == '#')
				sceneGrid[player.x - 2][player.y] = ' ';

			// move player up
				player.x -= 1;
			}
			return true;
		case 'd':
			// player moves right
			// check for blocks & try to move block & move player
			if (!(player.y < cols - 1))
				return true;
			
			playerSideBottom = sceneGrid[player.x][player.y + 1];
			 playerSideUp = sceneGrid[player.x - 1][player.y + 1];
			
			if (playerSideBottom == ' ' )
			{
				if (playerSideUp == ' ')
					player.y += 1;
				else return true;

			}
			else
			{
				if (sceneGrid[player.x][player.y + 2] == ' ' && sceneGrid[player.x - 1][player.y + 1] !='#')
				{
					sceneGrid[player.x][player.y + 2] = '#';
					sceneGrid[player.x][player.y + 1] = ' ';
					player.y += 1;
				}
			}
			
			return true;
		case 'a':
			// player moves left
			// check for blocks & try to move block & move player
			if (!player.y > 0)
				return true;

			 playerSideBottom = sceneGrid[player.x][player.y - 1];
			 playerSideUp = sceneGrid[player.x - 1][player.y - 1];

			if (playerSideBottom == ' ')
			{
				if (playerSideUp == ' ')
					player.y -= 1;
				else return true;

			}
			else
			{
				if (sceneGrid[player.x][player.y - 2] == ' ')
				{
					sceneGrid[player.x][player.y - 2] = '#';
					sceneGrid[player.x][player.y - 1] = ' ';
					player.y -= 1;
				}
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
				sceneGrid[i][j] = ' ';

		// spawn play & align it vertically & horizontally (imperfect as grid width is not even)
		player = { 6, 6 };

		// draw player renders
		// sceneGrid[6][6] = '%';
		// sceneGrid[5][6] = '%';
	}

	void Render()
	{
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < cols; ++j)
			{
				if (sceneGrid[player.x + 1][j] != ' ' && (player.x+1)!=rows-1)
				{
					if ((player.x == i && player.y == j) ||
						(player.x - 1 == i && player.y == j))
						std::cout << "% ";
					else std::cout << sceneGrid[i][j];
				}
				else 
				{
					//Ќужно сделать падение 
					if ((player.x+1 == i && player.y == j) ||
						(player.x == i && player.y == j))
						std::cout << "% ";
					else std::cout << sceneGrid[i][j];
				}
			}
		
			std::cout << std::endl;
		}
	}


	void block_rand()
	{
		srand(time(NULL));
		int y = rand() % rows;
		sceneGrid[0][y] = '#';
	}



	bool Update(const char& input)
	{
		if (UpdatePlayer(input))
		{
			// sceneGrid[player]
			for (int i = 0; i < rows; i++)
			{
				for(int j = 0; j < cols;j++)
				{
					if (player.x-1 == 0 || player.x-1 == 1)
						return false;
					else
						if (sceneGrid[i][j] == '#' && i != rows-1)
						{
							if (player.x - 1 == i && player.y == j)
							{
								return false;
							}
							else
							{
								//Ќужно сделать падение блока только на одну клетку за цикл
								sceneGrid[i][j] = ' ';
								sceneGrid[i + 1][j] = '#';
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