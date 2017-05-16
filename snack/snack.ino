#include "Arduboy.h"

// make an instance of arduboy used for many functions
Arduboy arduboy;

class coords
{
  public:
  int X = 0;
  int Y = 0; 
};

typedef struct coords Coords;

struct player
{
  int X = 0;
  int Y = 0;
  char dir = 'R';
  int Len = 1;
  static const int MaxLength = 10;
  Coords *Moves[MaxLength] = {NULL};
};

typedef struct player Player;

Player p1;
Coords foodLocation;

int maxX = 127;
int minX = 0;
int maxY = 63;
int minY = 0;

int score = 0;
int arrayPosition = 0;

void GenerateFood()
{
  arduboy.initRandomSeed();
  foodLocation.X = random(minX, maxX);
  foodLocation.Y = random(minY, maxY);
}

void StartGame()
{
  p1.X = 1;
  p1.Y = 1;
  GenerateFood();
}

//TODO only log move if location has changed.
void LogMove()
{
  if(p1.Moves[arrayPosition] == NULL)
  {
      p1.Moves[arrayPosition] = new Coords();
    
  }
      p1.Moves[arrayPosition]->X = p1.X;
      p1.Moves[arrayPosition]->Y = p1.Y;

  if(arrayPosition < p1.Len)
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
  if(p1.X == foodLocation.X && p1.Y == foodLocation.Y)
  {
    p1.Len += 1;
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
        tailPosition = 0;
      }
   } 
}

// This function runs once in your game.
// use it for anything that needs to be set only once in your game.
void setup() {
  // initiate arduboy instance
  arduboy.begin();

  // here we set the framerate to 15, we do not need to run at
  // default 60 and it saves us battery life
  arduboy.setFrameRate(15);

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

  CollisionDetection();

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

  arduboy.drawPixel(p1.X, p1.Y, 1);
  arduboy.drawPixel(foodLocation.X, foodLocation.Y, 1);
  DrawTail();
  LogMove();
  
  arduboy.setCursor(0, 0);  
  arduboy.print("Score ");
  arduboy.print(score);
  

  // then we finaly we tell the arduboy to display what we just wrote to the display
  arduboy.display();
}
