#include <TFT_eSPI.h>

//create the screen variable from the library
TFT_eSPI tft = TFT_eSPI();

// Set up variables for the cursor and counter. Cursor starts in the middle of the screen.
int paddleX = 50;
int paddleY = 0;
int oldPaddleX, oldPaddleY;
int ballDirectionX = 1;
int ballDirectionY = 1;
int ballX, ballY, oldBallX, oldBallY;
int ballSpeed = 10; // lower numbers are faster
int width = 10;
int height = 40;
int player1;
int player2;

// Define colours in 4-digit hex                                  
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

// Setting the joystick pins here so we can easily change them
#define JOYSTICK_X_PIN A14
#define JOYSTICK_Y_PIN A15
#define JOYSTICK_BUTTON_PIN 23

// initialize variables for menu selection
int menuIndex = 0;
const int MENU_ITEMS = 2;


void setup() {
  // put your setup code here, to run once:
  // Initalize the screen and set the orientation correctly, then make sure it's clear.
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  
  //set up the game menu
  tft.setTextSize(2);
  tft.setCursor(30, 50);
  tft.println("PONG");
  tft.setTextSize(1);
  tft.println("Made by JOMM");
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // menu loop
  int joyY = analogRead(A15);
  
  // use joystick to select menu options
  if (joyY < 100) {
    menuIndex--;
    if (menuIndex < 0) {
      menuIndex = MENU_ITEMS - 1;
    }
  }
  else if (joyY > 900) {
    menuIndex++;
    if (menuIndex >= MENU_ITEMS) {
      menuIndex = 0;
    }
  }
  
  // display current menu selection
  tft.setCursor(20, 150);
  switch(menuIndex) {
    case 0:
      tft.println("> Start");
      tft.println("High Scores");
      break;
    case 1:
      tft.println("Start");
      tft.println("> High Scores");
      break;
  }   
  
    // wait for user to press button to select menu option
  if (digitalRead(23) == LOW) {
    switch(menuIndex) {
      case 0:
        // start game
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(20, 50);
        tft.println("Starting game...");
        
        // code for Pong game        
        int myWidth = tft.width();
        int myHeight = tft.height();

        paddleY = paddleY + map(analogRead(A15), 0, 1023, -1, 1);

        if (0 >= paddleY)
          paddleY = 0;

        if (280 <= (paddleY))
          paddleY = 280;

        if (oldPaddleY != paddleY) {
          tft.fillRect(oldPaddleX, oldPaddleY, width, height, BLACK);
        }

        tft.fillRect(paddleX, paddleY, width, height, WHITE);

        oldPaddleX = paddleX;
        oldPaddleY = paddleY;

        // update the ball's position and draw it on screen
        if (millis() % ballSpeed < 2) {
          moveBall();
        }


      }

      void moveBall() {

        if (ballX > tft.width()) {
          player2 += 1;
        }

        if (ballX < 0) {
          player1 += 1;
        }

        if (ballY > tft.height() || ballY < 0) {

          ballDirectionY = -ballDirectionY;

        }

        if (inPaddle(ballX, ballY, paddleX, paddleY, width, height)) {

          ballDirectionX = -ballDirectionX;

        }

        ballX += ballDirectionX;
        ballY += ballDirectionY;

        if (oldBallX != ballX || oldBallY != ballY) {

          tft.fillCircle(oldBallX, oldBallY, 5, BLACK);

        }

        tft.fillCircle(ballX, ballY, 5, WHITE);

        oldBallX = ballX;

        oldBallY = ballY;
      }

      boolean inPaddle(int x, int y, int rectX, int rectY, int rectWidth, int rectHeight) {

        boolean result = false;

        if ((x >= rectX && x <= (rectX + rectWidth)) &&

          (y >= rectY && y <= (rectY + rectHeight))) {

          result = true;

        }

        return result;
      }
        break;
    
      case 1:
        // High Scores
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(20, 50);
        tft.println("High Scores");
        // code for high scores menu goes here
        break;
    }
    // wait for user to release button
    while (digitalRead(23) == LOW) {}
  }
  delay(50);
}
