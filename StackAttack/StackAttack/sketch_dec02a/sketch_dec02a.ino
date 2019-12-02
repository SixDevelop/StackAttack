#include <LCD5110_Graph.h>
#include <EEPROM.h>
LCD5110 LCD(9,10,11,12,13);
extern unsigned char SmallFont[];
#define ANALOG_X_pin 0
#define ANALOG_Y_pin 1
#define RIGHT_pin 3
#define LEFT_pin 5
#define UP_pin 2
#define DOWN_pin 4
#define BUTTON_E 6
#define BUTTON_F 7
#define BUTTON_G 8

struct Point
{
public:
  int x;
  int y;
};

class StackAttack
{
private:
  static const uint8_t rows = 7;
  static const uint8_t cols = 12;
  bool gameover = false;
  uint8_t _isPlaying = 0;
  //settings
    uint8_t size = 2;
    uint8_t contrast = 70;
    uint8_t difficulty = 0;
    int gameSpeed = 1000;
    
    //chosen params
    uint8_t _chosenOption = 0;
    uint8_t _section = 0;
  //record table
    String Top1 = "ZZZ";
    int Top1score = 0;

    String Top2 = "ZZZ";
    int Top2score = 0;

    String Top3 = "ZZZ";
    int Top3score = 0;

  char sceneGrid[rows][cols];
  Point player;

  bool UpdatePlayer()
  {
    char playerSideBottom;
    char playerSideUp;
    char playerUp;

    if (digitalRead(UP_pin)==LOW||(analogRead(ANALOG_Y_pin)> 553))
      {
        // player jumps
      // check for blocks & destroy if there is one
      if(sceneGrid[player.x+1][player.y] == '#' || (player.x) == rows - 1)
      { 
        if (sceneGrid[player.x - 1][player.y] == '#') // destroy block in top of player
        {
          for(int i = player.x; i > 1; i--)
            sceneGrid[i][player.y] = '*';
          
          return true;
        }
        else  // just stay in air
        {
          player.x -= 1;
          return true;
        }
      }
      else if (sceneGrid[player.x + 1][player.y] == '*') // drop if there is empty in the bottom
      {
        player.x += 1;
        return true;
      }
      else 
        return false;
      } 

    if (digitalRead(RIGHT_pin)==LOW||(analogRead(ANALOG_X_pin)> 553))
      {
        // player moves right
      // check for blocks & try to move block & move player
      if (!(player.y < cols - 1)) 
        return true;
      
      playerSideBottom = sceneGrid[player.x][player.y + 1]; // right place
       playerSideUp = sceneGrid[player.x - 1][player.y + 1]; // right and top place
      
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
      }   


    if (digitalRead(LEFT_pin)==LOW||(analogRead(ANALOG_X_pin)< 320))
      {
        // player moves left
      // check for blocks & try to move block & move player
      if (!player.y > 0)
        return true;

       playerSideBottom = sceneGrid[player.x][player.y - 1];
       playerSideUp = sceneGrid[player.x - 1][player.y - 1];

      if (playerSideBottom == '*') // empty near
      {
        if (playerSideUp == '*')  // enty in top
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
      }
  }

public:
  uint8_t score = 0;

    void GameOver()
    {
      //write info and score
      LCD.clrScr();
      LCD.print("Game Over",0,0);
      
      LCD.print("Score:",0,16);
      LCD.printNumI(score,40,16);
      LCD.print("Press Any Key",0,24);
      LCD.print("To Continue",0,32);
      LCD.update();
      //unlimited cikle
      while (true)
        if (digitalRead(UP_pin)==LOW||digitalRead(DOWN_pin)==LOW||digitalRead(LEFT_pin)==LOW||digitalRead(RIGHT_pin)==LOW||digitalRead(BUTTON_E)==LOW||digitalRead(BUTTON_F)==LOW||digitalRead(BUTTON_G)==LOW)
        {
          String _name = "AAA";
          uint8_t _chosenChar = 0;
          while (true)
          {
            //write name of player
            if(digitalRead(LEFT_pin)==LOW)
              if(_chosenChar == 0)
                _chosenChar = 2;
                else _chosenChar -= 1;

            if(digitalRead(RIGHT_pin)==LOW)
              if(_chosenChar == 2)
                _chosenChar = 0;
                else _chosenChar += 1;
                
            if(digitalRead(DOWN_pin)==LOW)
              if(_name[_chosenChar] == 'A')
                _name[_chosenChar] = 'Z';
                else _name[_chosenChar] -= 1;

            if(digitalRead(UP_pin)==LOW)
              if(_name[_chosenChar] == 'Z')
                _name[_chosenChar] = 'A';
                else _name[_chosenChar] += 1;
 
            LCD.clrScr();
            LCD.print("Enter Name:",0,0);
            LCD.print(_name,64,0);
            LCD.drawRect(64 + _chosenChar*6,8,70 + _chosenChar*6,10);

            LCD.print("Press G",0,12);
            LCD.print("To Try Again",0,20);
            LCD.print("Press E,F",0,28); 
            LCD.print("To Menu",0,36);

            

            LCD.update();
            delay(200);
            //check if we should change score place for G
            //In case of E and F go to menu

            if (digitalRead(BUTTON_G)==LOW)
            {
              
              delay(1000);
              CalcRecords(_name, score);
              
              gameover = false;
              LCD.clrScr();
              LoopGame();
            }
            if (digitalRead(BUTTON_E)==LOW||digitalRead(BUTTON_F)==LOW)
            {
              LCD.clrScr();
              CalcRecords(_name, score);
              _isPlaying = 0;
              gameover = false;
              //return;
              LCD.update();
              delay(1000);
             ShowMenu();
              
              
            }
          }
        }
    }
    
    void CalcRecords(String _name, int _score)
    {
      
      if (_score >= Top1score)
      {
        if (_score == Top1score)
        {
          if (_name.compareTo(Top1) < 0)
          {
            for (int i = 0; i<3;++i)
              Top3[i] = Top2[i];
            Top3score = Top2score;
            
  
            for (int i = 0; i<3;++i)
              Top2[i] = Top1[i];
            Top2score = Top1score;
            
            
            for (int i = 0; i<3;++i)
              Top1[i] = _name[i];
            Top1score = _score;
            
          }
          else if(_name.compareTo(Top1) > 0)
          {
            for (int i = 0; i<3;++i)
              Top3[i] = Top2[i];
            Top3score = Top2score;
           
  
            for (int i = 0; i<3;++i)
              Top2[i] = _name[i];
            Top2score = _score;
            
          }
          else if (_name.compareTo(Top1) == 0)
          {
            for (int i = 0; i<3;++i)
              Top1[i] = _name[i];
            Top1score = _score;
            
          }
        }
        else
        {
            for (int i = 0; i<3;++i)
              Top3[i] = Top2[i];
            Top3score = Top2score;
            
  
            for (int i = 0; i<3;++i)
              Top2[i] = Top1[i];
            Top2score = Top1score;
            
            
            for (int i = 0; i<3;++i)
              Top1[i] = _name[i];
            Top1score = _score;
            
        }
      }
      else if (_score >= Top2score)
      {
        if (_score == Top2score)
        {
          if(_name.compareTo(Top2) < 0)
          {
            for (int i = 0; i<3;++i)
              Top3[i] = Top2[i];
            Top3score = Top2score;
            
  
            for (int i = 0; i<3;++i)
              Top2[i] = _name[i];
            Top2score = _score;
           
          }
          else if(_name.compareTo(Top2) > 0)
          {
            for (int i = 0; i<3;++i)
              Top3[i] = _name[i];
            Top3score = _score;
            
          }
          else if(_name.compareTo(Top2) == 0)
          {
            for (int i = 0; i<3;++i)
              Top2[i] = _name[i];
            Top2score = _score;
            
          }
        }
        else
        {
            for (int i = 0; i<3;++i)
              Top3[i] = Top2[i];
            Top3score = Top2score;
            
  
            for (int i = 0; i<3;++i)
              Top2[i] = _name[i];
            Top2score = _score;
            
        }
      }
      else if (_score >= Top3score)
      {
            for (int i = 0; i<3;++i)
              Top3[i] = _name[i];
            Top3score = _score;
            
      }

      //save top 3 in memory
      for (int i = 0; i < 3; ++i)
        EEPROM.write(237 + i*9, Top1[i]);
      for (int i = 0; i < 3; ++i)
        EEPROM.write(362 + i*9, Top2[i]);
      for (int i = 0; i < 3; ++i)
        EEPROM.write(487 + i*9, Top3[i]);

      EEPROM.put(111, Top1score);
      EEPROM.put(128, Top2score);
      EEPROM.put(145, Top3score);
      
    }
    
    void LoopGame()
    {
      /*
      int counter = 0;
      do
      {
        if (counter % 5 == 0)
        {
          block_rand();
        }
        LCD.clrScr();
        Render();
        LCD.update();
        
        //std::cin >> input;
        
        counter++;
      } while (Update());
      LCD.clrScr();
        
        GameOver();*/
        int gameSpeed = 1000;
        gameSpeed -= difficulty * 100;
        int counter = 0;
        do
      {
        
        if (counter % 7 == 0)
        {
          block_rand();
        }
        LCD.clrScr();
        Render();
        LCD.update();
        
        //std::cin >> input;
        counter++;
        delay(gameSpeed);
      } while (Update());
      LCD.clrScr();
        
        GameOver();
        
    }
  //call menu
    int ShowMenu()
    {
      while(true)
      {
        //change option buy up or down buttons
        if(digitalRead(UP_pin)==LOW)
          if(_chosenOption == 0)
            _chosenOption = 3;
            else _chosenOption -= 1;

        if(digitalRead(DOWN_pin)==LOW)
          if(_chosenOption == 3)
            _chosenOption = 0;
            else _chosenOption += 1;
        LCD.clrScr();
        LCD.drawRect(0,5 + _chosenOption*8, 6,11 + _chosenOption*8);

        //writing of default menu
        if(_section == 0)
        {
              LCD.print("START",27,5);
              LCD.print("RECORDS",20,13);
              LCD.print("SETTINGS",18,21);
              LCD.print("EXIT",30,29);
              LCD.update();
              if(digitalRead(BUTTON_E)==LOW||digitalRead(BUTTON_G)==LOW)
                //Actions for button START
                if(_chosenOption == 0)
                {
                  //Begin game
                  LCD.clrScr();
                  _isPlaying = 1;
                  LoopGame();
                }
                //For button RECORDS
                else if(_chosenOption == 1)
                {
                  //Way to record table
                  LCD.clrScr();
                  _section = 2;
                  _chosenOption = 0;
                  delay(100);
                }
                //For button SETTINGS
                else if(_chosenOption == 2)
                {
                  //Way to settings
                  LCD.clrScr();
                  _section = 1;
                  _chosenOption = 0;
                  delay(100);
                }
                //For button EXIT
                else if(_chosenOption == 3)
                {
                  //Writing buy message
                  LCD.clrScr();
                  LCD.print("Goodbye!",0,0);
                  LCD.update();
                  delay(1000);
                  LCD.clrScr();
                  LCD.update();
                  LCD.enableSleep();
                  _isPlaying = 5;
                }
        }
        //Show settings menu
        else if(_section == 1)
        {
              
              LCD.print("BACK",8,5);
              LCD.print("SIZE",8,13);
              LCD.printNumI(size,76,13);
              LCD.print("DIFFICULTY",8,21);
              LCD.printNumI(difficulty,76,21);
              LCD.print("CONTRAST",8,29);
              LCD.printNumI(contrast,70,29);
              LCD.update();
              if(digitalRead(BUTTON_E)==LOW||digitalRead(BUTTON_G)==LOW)
                if(_chosenOption == 0)
                {
                  //Return to the default menu
                  LCD.clrScr();
                  _section = 0;
                  _chosenOption = 0;
                  delay(300);
                }
                if(_chosenOption == 1)
                {
                  //change size from 1 to 3
                  if(digitalRead(RIGHT_pin)==LOW)
                  if(size == 3)
                    size = 1;
                  else size++;
                else if(digitalRead(LEFT_pin)==LOW)
                  if(size == 1)
                    size = 3;
                  else size--;
                  EEPROM.put(0, size);
                }
                else if(_chosenOption == 2)
                {
                  //Меняем сложность(скорость) от 0 до 3
                  if(digitalRead(RIGHT_pin)==LOW)
                  if(difficulty == 3)
                    difficulty = 0;
                  else difficulty++;
                else if(digitalRead(LEFT_pin)==LOW)
                  if(difficulty == 0)
                    difficulty = 3;
                  else difficulty--;
                  EEPROM.put(9, difficulty);
                }
                else if(_chosenOption == 3)
                {
                  //change kontrast
                  if(digitalRead(RIGHT_pin)==LOW)
                  if(contrast == 70)
                    contrast = 50;
                    else contrast += 2;
                else if(digitalRead(LEFT_pin)==LOW)
                  if(contrast == 50)
                    contrast = 70;
                    else contrast -= 2;
                    LCD.setContrast(contrast);

                   EEPROM.put(18, contrast);
                }
        }
        //out of records table
        else if (_section == 2)
        {
          LCD.print("BACK",8,5);
          LCD.print("1]",8,13);
          LCD.print(Top1,20,13);
          
          LCD.printNumI(Top1score,47,13);

          LCD.print("2]",8,21);
          LCD.print(Top2,20,21);
          
          LCD.printNumI(Top2score,47,21);

          LCD.print("3]",8,29);
          LCD.print(Top3,20,29);
          
          LCD.printNumI(Top3score,47,29);

          LCD.update();
          if(digitalRead(BUTTON_E)==LOW||digitalRead(BUTTON_F)==LOW||digitalRead(BUTTON_G)==LOW)
                if(_chosenOption == 0)
                {
                  LCD.clrScr();
                  _section = 0;
                  _chosenOption = 0;
                  delay(300);
                }
        }
        delay(100);        
      }
    }
  
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

  void Print(uint8_t i, uint8_t j)
    {
      
        for (uint8_t _i = 0; _i < size; ++_i)
          for (uint8_t _j = 0; _j < size; ++_j)
              LCD.setPixel(size*j+_j,size*i+_i);
       
    }

  void Render()
  {
    for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < cols; j++)
      {
        
        if (sceneGrid[i][j] != '*'  || (player.x - 1 == i && player.y == j) ||
         (player.x == i && player.y == j))
        {
          Print(i,j);
        }
        //else
        //  std::cout << sceneGrid[i][j];

        
      }
      //std::cout << std::endl;
    }
  }


  void block_rand()
  {
    
    int y = rand() % cols;
    sceneGrid[0][y] = '#';
  }



  bool Update()
  {
    if (UpdatePlayer())
    {
      // scenegrid[player]
      // ïðîâåðèòü, åñòü ëè ïîëíûé íèæíèé ðÿä, óáðàòü åãî, ñäâèíóòü ìàòðèöó íà ñòðîêó âíèç

      if (player.x - 1 == 0 || player.x - 1 == 1)  // åñëè íà âåðõíèõ 2 ëèíèÿõ - ñðàçó ïðîèãðàë 
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

  void SetVals()
    {
      size = 2;
      contrast = 70;

      Top1 = "ZZZ";
    
      Top1score = 0;

      Top2 = "ZZZ";

      Top2score = 0;

      Top3 = "ZZZ";

      Top3score = 0;
      
      if (EEPROM.read(0) >= 1 && EEPROM.read(0) <= 3)
        EEPROM.get(0,size);
      if (EEPROM.read(9) >= 0 && EEPROM.read(9) <= 3)
        EEPROM.get(9,difficulty);
      if (EEPROM.read(18) >= 50 && EEPROM.read(18) <= 70 && EEPROM.read(18) % 2 == 0)
        EEPROM.get(18,contrast);

        int flag = 0;
        for (int i = 0; i < 3; ++i)
          if(EEPROM.read(237 + i*9) >= 'A' && EEPROM.read(237 + i*9) <= 'Z')
            flag++;
            
        for (int i = 0; i < 3; ++i)
        if(flag == 3)
          EEPROM.get(237 + i*9, Top1[i]);

        flag = 0;
        for (int i = 0; i < 3; ++i)
          if(EEPROM.read(362 + i*9) >= 'A' && EEPROM.read(362 + i*9) <= 'Z')
            flag++;
          
        for (int i = 0; i < 3; ++i)
        if(flag == 3)
          EEPROM.get(362 + i*9, Top2[i]);

        flag = 0;
        for (int i = 0; i < 3; ++i)
          if(EEPROM.read(487 + i*9) >= 'A' && EEPROM.read(487 + i*9) <= 'Z')
            flag++;
          
        for (int i = 0; i < 3; ++i)
        if(flag == 3)
          EEPROM.get(487 + i*9, Top3[i]);
        
      if(EEPROM.read(111) >= 0 && EEPROM.read(111) <= 32000)
        EEPROM.get(111, Top1score);
      if(EEPROM.read(128) >= 0 && EEPROM.read(128) <= 32000)
        EEPROM.get(128, Top2score);
      if(EEPROM.read(145) >= 0 && EEPROM.read(145) <= 32000)
        EEPROM.get(145, Top3score);

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
StackAttack game;

void setup() {
  pinMode(UP_pin, INPUT_PULLUP);
  pinMode(DOWN_pin, INPUT_PULLUP);
  pinMode(RIGHT_pin, INPUT_PULLUP);
  pinMode(LEFT_pin, INPUT_PULLUP);
  pinMode(BUTTON_E, INPUT_PULLUP);
  pinMode(BUTTON_F, INPUT_PULLUP);
  pinMode(BUTTON_G, INPUT_PULLUP);
  //pinMode(7, OUTPUT);
  //digitalWrite(7, LOW);
  srand(analogRead(0));
      
  LCD.InitLCD();
  LCD.setFont(SmallFont);
  game.SetVals();
  game.ShowMenu();

}

void loop() {
  game.ShowMenu();

}
