#include <Adafruit_GFX.h>    
#include <Adafruit_ST7735.h> 
#include <SPI.h>

#define cs   10
#define dc   9
#define rst  8  
Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);


#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

int pot1 = A0;
int pot2 = A5;
const int listSize = 10;
int avgArray [listSize] = {0,0,0,0,0,0,0,0,0,0}; 

// variables for the position of the ball and paddle
int paddleX1 = 0;
int paddleY1 = 0;
int oldpaddleX1, oldpaddleY1;
int paddleX2 = 0;
int paddleY2 = 123;
int oldpaddleX2, oldpaddleY2;
int ballDirectionX = 1;
int ballDirectionY = 1;

int ballX, ballY, oldBallX, oldBallY;

long lastFrameTime;
long frameTimeAsMilliseconds;
long count;

void setup() {

  Serial.begin(9600);


  tft.initR(INITR_144GREENTAB);
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(3);
}

void loop() {

  int myWidth = 104;
  int myHeight = 144;
  
  Serial.println( analogRead( pot1 ) );
  paddleX1 = map( analogRead( pot1 ), 0, 1023, myWidth, 0); 
  paddleX2 = map( analogRead( pot2 ), 0, 1023, myWidth, 0); 

  // erase the previous position of the paddle if different from present
  if (oldpaddleX1 != paddleX1 || oldpaddleY1 != paddleY1) {
    tft.fillRect(oldpaddleX1, oldpaddleY1, 20, 5, BLACK);
  }

  if (oldpaddleX2 != paddleX2 || oldpaddleY2 != paddleY2) {
    tft.fillRect(oldpaddleX2, oldpaddleY2, 20, 5, BLACK);
  }

  // draw the paddle on screen, save the current position
  // as the previous.
  tft.fillRect(paddleX1, paddleY1, 20, 5, YELLOW);
  oldpaddleX1 = paddleX1;
  oldpaddleY1 = paddleY1;

  tft.fillRect(paddleX2, paddleY2, 20, 5, MAGENTA);
  oldpaddleX2 = paddleX2;
  oldpaddleY2 = paddleY2;

  moveBall();
  
  delayMicroseconds( count % 16383  );
  count++;
}

void moveBall() {
  // if the ball goes offscreen, reverse the direction:
  if (ballX > tft.width() || ballX < 0) {
    ballDirectionX = -ballDirectionX;
  }

  if (ballY > tft.height() || ballY < 0) {
    ballDirectionY = -ballDirectionY;
  }  


  if (inPaddle(ballX, ballY, paddleX1, paddleY1, 20, 5)) {
    ballDirectionY = -ballDirectionY;
  }

 if (inPaddle(ballX, ballY, paddleX2, paddleY2, 20, 5)) {
    ballDirectionY = -ballDirectionY;
  }

  ballX += ballDirectionX;
  ballY += ballDirectionY;


  if (oldBallX != ballX || oldBallY != ballY) {
    tft.fillRect(oldBallX, oldBallY, 5, 5, BLACK);
  }
  
  tft.fillRect(ballX, ballY, 5, 5, CYAN);

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
