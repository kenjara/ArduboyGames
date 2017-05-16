#include "Arduboy.h"

// make an instance of arduboy used for many functions
Arduboy arduboy;

struct coords
{
  int X = 0;
  int Y = 0; 
};

typedef struct coords Coords;

struct player
{
  int X = 0;
  int Y = 0;
  char dir = 'R';

  Player()
  {
    
  }
};

typedef struct player Player;

Player p1;
Coords foodLocation;

int maxX = 127;
int minX = 0;
int maxY = 63;
int minY = 0;

int score = 0;

void GenerateFood()
{
  foodLocation.X = 0;
  foodLocation.Y = 0;
}

// This function runs once in your game.
// use it for anything that needs to be set only once in your game.
void setup() {
  // initiate arduboy instance
  arduboy.begin();

  // here we set the framerate to 15, we do not need to run at
  // default 60 and it saves us battery life
  arduboy.setFrameRate(15);

  p1.X = 1;
  p1.Y = 1;
}


// our main game loop, this runs once every cycle/frame.
// this is where our game logic goes.
void loop() {
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;

  // first we clear our screen to black
  arduboy.clear();

  // the next couple of lines will deal with checking if the D-pad buttons
  // are pressed and move our text accordingly.
  // We check to make sure that x and y stay within a range that keeps the
  // text on the screen.

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
  // (positions start at 0, 0) 
  //arduboy.setCursor(4, 9);

  // then we print to screen what is in the Quotation marks ""
  //arduboy.print(F("Hello, world!"));

  // then we finaly we tell the arduboy to display what we just wrote to the display
  arduboy.display();
}
