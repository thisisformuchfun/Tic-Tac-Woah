#include <Encabulator.h>
#include <Wire.h>

byte buttons[] = {2,3,4,5,6,7,8,9,10};
int LASTBUTTON=0;

int loopCounter = 0;
int DebugDelay=15;
int tempR = 0;
int tempG = 0;
int tempB = 0;


float tempX = 0.0;
float tempY = 0.0;
int palindrome = 0; 
int Xs[]={0,0,0,0,0,0,0,0,0};// holds position data
int Os[]={0,0,0,0,0,0,0,0,0};// holds position data
int board[]={0,0,0,0,0,0,0,0,0};// holds position data
// 0 is blank, 1 is nought (human), 2 is cross (human 2)
int winner=0; // 0 = no winner, 1 = noughts, 2 = crosses, 3 = draw
int buttonA=7;
int buttonB=6;

void setup()
{
  
  Serial.println("setup begin");
    // Make input & enable pull-up resistors on switch pins
  for (int i=0; i< 10; i++) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
    }
  
  randomSeed(analogRead(0));
  pinMode(buttonA, INPUT); // digital zero
  pinMode(buttonB, INPUT); // digital one
  // secret startup sequence 
  
  Encabulator.upUpDownDownLeftRightLeftRightBA();

  // enable some debugging on the serial port

  Encabulator.setVerbose(true);

  // check the temperature
  
  float weatherReport = Encabulator.temperature.getFahrenheit();
  Serial.print("Currrent operating temperature is: ");
  Serial.print(weatherReport);
  Serial.println("F");

  // check EEPROM   

  for (uint16_t i = 0 ; i < 52 ; i++) {
    Serial.print((char) Encabulator.eeprom.readAddress(i));
  }  
  Serial.println();

  // check the DAC

  Encabulator.dac.write(100);

  // check the XYZ

  int x = Encabulator.accelerometer.x();
  int y = Encabulator.accelerometer.y();
  int z = Encabulator.accelerometer.z();

  Serial.print("Heads or Tails?");

  if (z < 0.0) {
    Serial.println("  ...Heads!");
  } else {
    Serial.println("  ...Tails!");
  }
  
  // finish startup sequence with two seconds of global test pattern
//  
//  Encabulator.testPattern();
//  
//  delay(1000);
  
  Encabulator.blackout();
  
  delay(1000);
  
//  Encabulator.testPattern();
  


  delay(500);

  // fade up to white

  for (int i = 1 ; i < 5 ; i++) {  
    Encabulator.stripBankA.fadeHeaderToRGB(i,255,255,255,5); 
    Encabulator.stripBankB.fadeHeaderToRGB(5-i,255,255,255,5);
    delay(500);
  }
  Encabulator.stripBankA.fadeHeaderToRGB(4,255,0,0,5); 
    Encabulator.stripBankB.fadeHeaderToRGB(4,255,0,0,5);
//  
//  Encabulator.stripBankA.autoFadeHeaderBetweenRGBs(4,255,255,255,0,0,0, 10);
//  Encabulator.stripBankB.autoFadeHeaderBetweenRGBs(4,255,255,255,0,0,0, 10);
    Serial.println("setup end");

}

void drawBoard() 
// redraws the player board with current piece positions
{
// draw the dividing lines
Serial.println("drawBoard");
    //              first 12               #  ,X1 ,X2 ,X3  , fade in fast)  
    Encabulator.stripBankA.fadeHeaderToRGB(1 , (Xs[0]) , (Xs[1]) , (Xs[2]) , 5);
//    Serial.print("1-3: ");Serial.print((Xs[0]));Serial.print(" ");Serial.print((Xs[1]));Serial.print(" ");Serial.print((Xs[2]));
    Encabulator.stripBankA.fadeHeaderToRGB(2 , (Xs[3]) , (Xs[4]) , (Xs[5]) , 5);
//    Serial.print("4-6: ");Serial.print((Xs[3]));Serial.print(" ");Serial.print((Xs[4]));Serial.print(" ");Serial.print((Xs[5]));
    Encabulator.stripBankA.fadeHeaderToRGB(3 , (Xs[6]) , (Xs[7]) , (Xs[8]) , 5);
//    Serial.print("7-9: ");Serial.print((Xs[6]));Serial.print(" ");Serial.print((Xs[7]));Serial.print(" ");Serial.print((Xs[8]));
    
    Encabulator.stripBankB.fadeHeaderToRGB(1 , (Os[0]) , (Os[1]) , (Os[2]) , 5);
//    Serial.print("1-3: ");Serial.print((Os[0]));Serial.print(" ");Serial.print((Os[1]));Serial.print(" ");Serial.print((Os[2]));
    Encabulator.stripBankB.fadeHeaderToRGB(2 , (Os[3]) , (Os[4]) , (Os[5]) , 5);
//    Serial.print("4-6: ");Serial.print((Os[3]));Serial.print(" ");Serial.print((Os[4]));Serial.print(" ");Serial.print((Os[5]));
    Encabulator.stripBankB.fadeHeaderToRGB(3 , (Os[6]) , (Os[7]) , (Os[8]) , 5);
//    Serial.print("7-9: ");Serial.print((Os[6]));Serial.print(" ");Serial.print((Os[7]));Serial.print(" ");Serial.print((Os[8]));
//   delay(4*DebugDelay); 
}

void checkWinner() 
// checks board to see if there is a winner
// places result in the global variable 'winner'
{
  Serial.println("checkWinner");
  int qq=0;
  // noughts win?
  if (board[0]==1 && board[1]==1 && board[2]==1) { winner=1; }
  if (board[3]==1 && board[4]==1 && board[5]==1) { winner=1; }
  if (board[6]==1 && board[7]==1 && board[8]==1) { winner=1; }  
  if (board[0]==1 && board[3]==1 && board[6]==1) { winner=1; }
  if (board[1]==1 && board[4]==1 && board[7]==1) { winner=1; }
  if (board[2]==1 && board[5]==1 && board[8]==1) { winner=1; }  
  if (board[0]==1 && board[4]==1 && board[8]==1) { winner=1; }
  if (board[2]==1 && board[4]==1 && board[6]==1) { winner=1; }
  // crosses win?
  if (board[0]==2 && board[1]==2 && board[2]==2) { winner=2; }
  if (board[3]==2 && board[4]==2 && board[5]==2) { winner=2; }
  if (board[6]==2 && board[7]==2 && board[8]==2) { winner=2; }  
  if (board[0]==2 && board[3]==2 && board[6]==2) { winner=2; }
  if (board[1]==2 && board[4]==2 && board[7]==2) { winner=2; }
  if (board[2]==2 && board[5]==2 && board[8]==2) { winner=2; }  
  if (board[0]==2 && board[4]==2 && board[8]==2) { winner=2; }
  if (board[2]==2 && board[4]==2 && board[6]==2) { winner=2; }
  // is it a draw?
  if (winner==0)
  {
     drawTest();
  }
}

void drawTest()
// checks for a draw
// if there isn't a winner detected, and the board is full then it's a draw
{
  Serial.println("drawTest");
  int n=0;
  int c=0;
  for (int a=0; a<9; a++)
  {
    if (board[a]==1) { n++; }
    if (board[a]==2) { c++; }
  }
  if (n==5 && c==4) { winner=3; }
  if (n==4 && c==5) { winner=3; }
  Serial.print("n= "); Serial.println(n);
  Serial.print("c= "); Serial.println(c);
}

void introScreen() 
{
  //Make cool intro stuffs and sounds here
  for (int i=0; i< 10; i++) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
    }
    //grid
//  Encabulator.stripBankA.autoFadeHeaderToRandomRGBs(4, 10);
//  Encabulator.stripBankB.autoFadeHeaderToRandomRGBs(4, 10);
   //Xs Os
  Encabulator.stripBankA.autoFadeHeaderToRandomRGBs(1, 2);
  Encabulator.stripBankA.autoFadeHeaderToRandomRGBs(2, 2);
  Encabulator.stripBankA.autoFadeHeaderToRandomRGBs(3, 2);
  Encabulator.stripBankB.autoFadeHeaderToRandomRGBs(1, 2);
  Encabulator.stripBankB.autoFadeHeaderToRandomRGBs(2, 2);
  Encabulator.stripBankB.autoFadeHeaderToRandomRGBs(3, 2);
  int check=0;
  int buttonState=0;
  while (check == LOW)
          {//this is where we check for button presses!
          Serial.println("Check button states");
//          delay(DebugDelay);
          //check a grid of buttons!
              for (int i=0; i< 9; i++) 
                {
                
                    //Check buttons one by one
                    buttonState = digitalRead(buttons[i]);
//                    Serial.println(buttonState);
                    

                            if (buttonState == LOW) {    
                                                    check = 1;   //exit while loop
                                                    break;          //exit for loop too, we only need to catch one button press, not two.
                                                    }
                      
                    
//                    delay(10000); 
                }
          }
  clearBoard();
  drawBoard();  //finally draw the board once you escape the loop waiting for input in the above section
}

void humanMove() 
// Human's turn
{
    for (int i=0; i< 10; i++) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
    }
  Serial.println("humanMove");
  //Check until button press
  int check=0;
  int buttonState=0;
       while (check == LOW)
          {//this is where we check for button presses!
          Serial.println("Check button states");
//          delay(DebugDelay);
          //check a grid of buttons!
              for (int i=0; i< 9; i++) 
                {
                
                    //Check buttons one by one
                    buttonState = digitalRead(buttons[i]);
                    Serial.println(buttonState);
                    if (buttonState == LOW && board[i] == 0) {     //if a button has been pressed, and never pressed before!:
                          
                                        LASTBUTTON=i;   // remember what button was just pressed!
                                        board[i]=1;     //set the board for X for this button
                                        check = 1;   //exit while loop
                                        break;          //exit for loop too, we only need to catch one button press, not two.
                                      }
                                     delay(DebugDelay); 
                }
          }
  
      //button was pressed, do switch case - Make sure they don't do a button they've already done!
      switch(LASTBUTTON)
        {
          case 0:  Xs[0]=255; /*SOUND;*/ break;
          case 1:  Xs[1]=255; /*SOUND;*/ break;
          case 2:  Xs[2]=255; /*SOUND;*/ break;          
          case 3:  Xs[3]=255; /*SOUND;*/ break;
          case 4:  Xs[4]=255; /*SOUND;*/ break;
          case 5:  Xs[5]=255; /*SOUND;*/ break;
          case 6:  Xs[6]=255; /*SOUND;*/ break;
          case 7:  Xs[7]=255; /*SOUND;*/ break;
          case 8:  Xs[8]=255; /*SOUND;*/ break;
        }
        drawBoard();
}

void humanMove2() 
// Human's turn
{
    for (int i=0; i< 10; i++) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
    }
  Serial.println("humanMove");
  //Check until button press
  int check=0;
  int buttonState=0;
      while (check == LOW)
          {//this is where we check for button presses!
          Serial.println("Check button states");
//          delay(DebugDelay);
          //check a grid of buttons!
              for (int i=0; i< 9; i++) 
                {
                
                    //Check buttons one by one
                    buttonState = digitalRead(buttons[i]);
                    Serial.println(buttonState);
                    if (buttonState == LOW && board[i] == 0) {     //if a button has been pressed, and never pressed before!:
                          
                                        LASTBUTTON=i;   // remember what button was just pressed!
                                        board[i]=2;     //set the board for X for this button
                                        check = 1;   //exit while loop
                                        break;          //exit for loop too, we only need to catch one button press, not two.
                                      }
                                     delay(DebugDelay); 
                }
          }
  
      //button was pressed, do switch case - Make sure they don't do a button they've already done!
      switch(LASTBUTTON)
        {
          case 0:  Os[0]=255; /*SOUND;*/ break;
          case 1:  Os[1]=255; /*SOUND;*/ break;
          case 2:  Os[2]=255; /*SOUND;*/ break;          
          case 3:  Os[3]=255; /*SOUND;*/ break;
          case 4:  Os[4]=255; /*SOUND;*/ break;
          case 5:  Os[5]=255; /*SOUND;*/ break;
          case 6:  Os[6]=255; /*SOUND;*/ break;
          case 7:  Os[7]=255; /*SOUND;*/ break;
          case 8:  Os[8]=255; /*SOUND;*/ break;
        }
        drawBoard();
}


void playGame()
{
  Serial.println("playGame");
  // take turns until someone wins or a draw
  do
  {
    if (winner==0)
    {
      humanMove();
      checkWinner();
    }
    if (winner==0)
    {
      humanMove2();
      checkWinner();
    }     
  }
  while (winner==0);
  switch(winner)
  {
    case 1: humanWin(); break; 
    case 2: humanWin2(); break;
    case 3: drawWin(); break;
  }
}

void humanWin()
// You won!
{
  Serial.println("humanWin");
  //Make cool winning pattern?
    
        Encabulator.stripBankB.fadeHeaderToRGB(1 , 0 , 0 , 0 , 5);
    Encabulator.stripBankB.fadeHeaderToRGB(2 , 0 , 0 , 0 , 5);
    Encabulator.stripBankB.fadeHeaderToRGB(3 , 0 , 0 , 0 , 5);
    
    Encabulator.stripBankA.autoJumpHeaderBetweenRGBs(1 , 255 , 255 , 255 ,0,0,0, 5);
    Encabulator.stripBankA.autoJumpHeaderBetweenRGBs(2 , 255 , 0 , 255 ,0,0,0, 5);
    Encabulator.stripBankA.autoJumpHeaderBetweenRGBs(3 , 255 , 255 , 255 ,0,0,0, 5);
  delay(5000);
  
  
  do {/*sound saying press any button to continue;*/} while (digitalRead(buttonA)==LOW); //waits for button press to continue
  clearBoard();
}
void humanWin2()
// You won!
{
  Serial.println("humanWin2");
  //Make cool winning pattern?
  
      Encabulator.stripBankB.autoJumpHeaderBetweenRGBs(1 , 255 , 0 , 255,0,0,0, 5);
    Encabulator.stripBankB.autoJumpHeaderBetweenRGBs(2 , 0 , 255 , 0 ,0,0,0, 5);
    Encabulator.stripBankB.autoJumpHeaderBetweenRGBs(3 , 255 , 0 , 255 ,0,0,0, 5);
    
    Encabulator.stripBankA.fadeHeaderToRGB(1 , 0 , 0 , 0 , 5);
    Encabulator.stripBankA.fadeHeaderToRGB(2 , 0 , 0 , 0 , 5);
    Encabulator.stripBankA.fadeHeaderToRGB(3 , 0 , 0 , 0 , 5);
   delay(5000);
  do {/*sound saying press any button to continue;*/} while (digitalRead(buttonA)==LOW); //waits for button press to continue
  clearBoard();
}


void drawWin()
// a draw
{
  Serial.println("drawWin");
  //Make cool DRAW pattern? 
      Encabulator.stripBankA.autoJumpHeaderBetweenRGBs(1 , 255 , 255 , 255 ,0,0,0, 5);
    Encabulator.stripBankA.autoJumpHeaderBetweenRGBs(2 , 255 , 0 , 0 ,0,0,0, 5);
    Encabulator.stripBankA.autoJumpHeaderBetweenRGBs(3 , 255 , 255 , 255 ,0,0,0, 5);
    
    Encabulator.stripBankB.autoJumpHeaderBetweenRGBs(1 , 255 , 255 , 255 ,0,0,0, 5);
    Encabulator.stripBankB.autoJumpHeaderBetweenRGBs(2 , 255 , 0 , 0 ,0,0,0, 5);
    Encabulator.stripBankB.autoJumpHeaderBetweenRGBs(3 , 255 , 255 , 255 ,0,0,0, 5);
   delay(50900);
  do {/*sound saying press any button to continue;*/} while (digitalRead(buttonA)==LOW); //waits for button press to continue
  clearBoard();
}

void clearBoard()
// clears board of pieces
{
//  Serial.println("clearBoard");
  //Reset to initialize screen

for (int x=0; x<9;x++){
  Xs[x]=0;// holds position data
  Os[x]=0;// holds position data
  board[x]=0;// holds position data
}
  winner=0;
}

void loop()
{
  introScreen();
  drawBoard();
  playGame();
}



