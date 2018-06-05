#include "Arduboy.h"

// make an instance of arduboy used for many functions
Arduboy arduboy;

PROGMEM const unsigned char playerShip [] = {0x10, 0x38, 0x7C, 0xFE};

//byte playerWidth = 7;
//byte playerHeight = 4;

PROGMEM const unsigned char ship1 [] = {0x38, 0x7C, 0xFE, 0xFE, 0x82, 0xC6};

byte maxX = 127;
byte minX = 0;
byte maxY = 63;
byte minY = 10;
char gameState = 'T';
bool sound = false;

const byte maxProjs = 3;
byte fireDelay = 0;


struct Proj
{
  //public:
  byte X = 0;
  byte Y = 0;
  bool Active = false;
};

struct player
{
  byte X = 0;
  byte Y = 0;
  byte W = 7;
  byte H = 4;
};

typedef struct player Player;

Player p1;

Proj projs[maxProjs];

int score = 0;
int arrayPosition = 0;

void StartGame()
{
  p1.X = (maxX / 2) - (p1.W / 2);
  p1.Y = maxY - p1.H;
  score = 0;
  
  gameState = 'G';
}

void EndGame()
{
  if(sound == true)
  {
    arduboy.tunes.tone(200, 200);
    delay(400);
    arduboy.tunes.tone(100, 400);
    delay(600);
    arduboy.tunes.tone(50, 2000);
  }
    
  gameState = 'E';
}

void CollisionDetection()
{

}

void DrawFrame()
{
  //arduboy.drawRect(minX,minY, maxX +1, (maxY - minY) +1, WHITE);
  
  for (int i = 0; i < maxProjs; i++) 
  {
    Serial.print("\nproj ");
    Serial.print(i);
    Serial.print("\nActive ");
    Serial.print(projs[i].Active);
    Serial.print("\nX ");
    Serial.print(projs[i].X);
    Serial.print("\nY ");
    Serial.print(projs[i].Y);
      if(projs[i].Active == true)
      {
        projs[i].Y -= 1;
        if(projs[i].Y == 0)
        {
          projs[i].Active = false;
        }
        else
        {
          arduboy.drawPixel(projs[i].X, projs[i].Y, WHITE);       
        }
      }
      Serial.print("\nActive ");
      Serial.print(projs[i].Active);
  }

  arduboy.drawSlowXYBitmap(p1.X,p1.Y, playerShip, p1.W, p1.H, WHITE);
  arduboy.drawSlowXYBitmap(5,5, ship1, 8 , 6, WHITE);

  
}

// This function runs once in your game.
// use it for anything that needs to be set only once in your game.
void setup() {
  // initiate arduboy instance
  arduboy.begin();

  // here we set the framerate to 15, we do not need to run at
  // default 60 and it saves us battery life
  arduboy.setFrameRate(30);
}


// our main game loop, this runs once every cycle/frame.
// this is where our game logic goes.
void loop() {
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;

  // first we clear our screen to black
  arduboy.clear();

  if(gameState == 'G')
  {
    arduboy.setFrameRate(60);
    DrawFrame();
  
    if(arduboy.pressed(RIGHT_BUTTON)) {
      if((p1.X) < (maxX - p1.W))
      {
        p1.X += 1;
      }
    }
  
    if(arduboy.pressed(LEFT_BUTTON)) {
      if(p1.X > 0)
      {
        p1.X -= 1;
      }
    }

    if(arduboy.pressed(B_BUTTON))
    {
      if(fireDelay == 0)
      {
        for (int i = 0; i < maxProjs; i++) 
        {
          if(projs[i].Active == false)
          {
            projs[i].X = p1.X + (p1.W /2);
            projs[i].Y = p1.Y - 1;
            projs[i].Active = true;
            fireDelay = 20;
            break;
          }
        }
      }
    }
  
  


    CollisionDetection();
      
    arduboy.setCursor(63, 0);  
    arduboy.print("Score ");
  
    char scoreBuff[5];
    sprintf(scoreBuff, "%05d", score);
    
    arduboy.print(scoreBuff);

    if(fireDelay > 0)
    {
      fireDelay --;
    }
  }
  else if(gameState == 'E')
  {
    arduboy.setFrameRate(10);
    arduboy.setCursor(20, 20);  
    arduboy.print("Game Over");
    
    arduboy.setCursor(20, 40);  
    arduboy.print("Score ");
  
    char scoreBuff[5];
    sprintf(scoreBuff, "%05d", score);
    
    arduboy.print(scoreBuff);

    if(arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON)) {
      gameState = 'T';
    }

  }
  else if(gameState == 'T')
  {
    arduboy.setFrameRate(10);
    arduboy.setCursor(50, 1);  
    arduboy.print("Space");

    arduboy.setCursor(20, 15);  
    arduboy.print("By Shane Powell");
    
    arduboy.setCursor(50, 55);  
    arduboy.print("Sound");

    arduboy.setCursor(90, 55);  
    arduboy.print("On");

    arduboy.setCursor(109, 55);  
    arduboy.print("Off");
   
    if(arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON)) {
      StartGame();
    }
    else if(arduboy.pressed(LEFT_BUTTON))
    {
      sound = true;
    }
    else if(arduboy.pressed(RIGHT_BUTTON))
    {
      sound = false;
    }

    if(sound == true)
    {
          arduboy.drawRect(88,53, 18, 11, WHITE);
    }
    else
    {
          arduboy.drawRect(107,53, 21, 11, WHITE);
    }
  }

  
  

  arduboy.display();
}
