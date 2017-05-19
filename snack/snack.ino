#include "Arduboy.h"

// make an instance of arduboy used for many functions
Arduboy arduboy;

byte maxX = 127;
byte minX = 0;
byte maxY = 63;
byte minY = 10;
bool gameRunning = true;

class coords
{
  public:
  byte X = 0;
  byte Y = 0; 
};

typedef struct coords Coords;

struct player
{
  byte X = 0;
  byte Y = 0;
  char dir = 'R';
  int Len = 1;
  static const int MaxLength = 100;
  Coords *Moves[MaxLength] = {NULL};
};

typedef struct player Player;

Player p1;
Coords foodLocation;
Coords previousPosition;

int score = 0;
int arrayPosition = 0;

void GenerateFood()
{
  arduboy.initRandomSeed();
  foodLocation.X = random(minX + 1, maxX - 1);
  foodLocation.Y = random(minY + 1, maxY - 1);
}

void StartGame()
{
  p1.X = maxX / 2;
  p1.Y = (maxY / 2) + 10;
  GenerateFood();
}

void LogMove()
{
  if(p1.Moves[arrayPosition] == NULL)
  {
      p1.Moves[arrayPosition] = new Coords();
    
  }
      p1.Moves[arrayPosition]->X = p1.X;
      p1.Moves[arrayPosition]->Y = p1.Y;

  if(arrayPosition < p1.Len -1)
  {
    arrayPosition += 1;
    
  }
  else
  {
    arrayPosition = 0;
  }
}

void CollisionDetection()
{

  //Check if hit wall

  if(p1.X >= maxX || p1.X <= minX || p1.Y >= maxY || p1.Y <= minY)
  {
    gameRunning = false; 
  }

  //Check if hit tail
  for (int i=0; i < p1.MaxLength; i++){
      if(p1.Moves[i] != NULL)
      {
        if(p1.Moves[i]->X == p1.X && p1.Moves[i]->Y == p1.Y)
        {
          gameRunning = false;
        }
      }
  }

  // Check if on food
  if(p1.X == foodLocation.X && p1.Y == foodLocation.Y)
  {
    if(p1.Len < p1.MaxLength)
    {
      p1.Len += 1;
    }
    
    score += 1;
    LogMove();
    GenerateFood();
  }
}

void DrawTail()
{
  int tailPosition = arrayPosition;
  for (int i=0; i < p1.Len; i++){
      if(p1.Moves[tailPosition] != NULL)
      {
         arduboy.drawPixel(p1.Moves[tailPosition]->X, p1.Moves[tailPosition]->Y, 1);
      }
      else
      {
        return;
      }

      if(tailPosition > 0)
      {
        tailPosition -= 1;
      }
      else
      {
        tailPosition = p1.Len -1;
      }
   } 
}

void DrawFrame()
{
  arduboy.drawRect(minX,minY, maxX +1, (maxY - minY) +1, WHITE);
}

// This function runs once in your game.
// use it for anything that needs to be set only once in your game.
void setup() {
  // initiate arduboy instance
  arduboy.begin();

  // here we set the framerate to 15, we do not need to run at
  // default 60 and it saves us battery life
  arduboy.setFrameRate(30);

 StartGame(); 
}


// our main game loop, this runs once every cycle/frame.
// this is where our game logic goes.
void loop() {
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;

  // first we clear our screen to black
  arduboy.clear();

  if(gameRunning == true)
  {
    DrawFrame();
  
    if(arduboy.pressed(RIGHT_BUTTON)) {
      p1.dir = 'R';
    }
  
    if(arduboy.pressed(LEFT_BUTTON)) {
      p1.dir = 'L';
    }
  
    if(arduboy.pressed(UP_BUTTON)) {
      p1.dir = 'U';
    }
  
    if(arduboy.pressed(DOWN_BUTTON)) {
      p1.dir = 'D';  
    }
  
    switch(p1.dir)
    {
      case 'U':
        if(p1.Y > minY)
        {
          p1.Y -= 1;
        }
        break;
      case 'D':
        if(p1.Y < maxY)
        {
          p1.Y += 1;
        }
        break;
      case 'L':
        if(p1.X > minX)
        {
          p1.X-= 1;
        }
        break;
      case 'R':
        if(p1.X < maxX)
        {
          p1.X+= 1;
        }
      
      break;
    }

    CollisionDetection();
  
    arduboy.drawPixel(p1.X, p1.Y, 1);
    arduboy.drawPixel(foodLocation.X, foodLocation.Y, 1);
    
    if(p1.X != previousPosition.X || p1.Y != previousPosition.Y)
    {
      previousPosition.X = p1.X;
      previousPosition.Y = p1.Y;
      LogMove();
    }
    DrawTail();    
    
    arduboy.setCursor(63, 0);  
    arduboy.print("Score ");
  
    char scoreBuff[5];
    sprintf(scoreBuff, "%05d", score);
    
    arduboy.print(scoreBuff);
  }
  else
  {
    arduboy.setCursor(20, 20);  
    arduboy.print("Game Over");
    
    arduboy.setCursor(20, 40);  
    arduboy.print("Score ");
  
    char scoreBuff[5];
    sprintf(scoreBuff, "%05d", score);
    
    arduboy.print(scoreBuff);
  }

  
  

  // then we finaly we tell the arduboy to display what we just wrote to the display
  arduboy.display();
}
