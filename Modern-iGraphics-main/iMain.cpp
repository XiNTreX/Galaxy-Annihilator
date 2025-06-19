#include "iGraphics.h"
#include <iostream>
#include "iSound.h"
using namespace std;

int bgSoundIdx = -1;
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 700

int gamestate = 1;
/*
function iDraw() is called again and again by the system.
*/

void homepage()
{
    iShowImage(0, 0, "assets/images/homepage_w_menu.png");
}
void difficulty()
{
    iShowImage(0, 0, "assets/images/difficulty.png");
}
void mainpage1()
{
    iShowImage(0, 0, "assets/images/mainbg.png");
}
void iDraw()
{
    // place your drawing codes here
    iClear();
    switch (gamestate)
    {
    case 1:
        homepage();
        break;
    case 2:
        difficulty();
        break;
    case 21:
        mainpage1();
        break;
    }
}
/*
function iMouseMove() is called when the user moves the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
    // place your codes here
}

/*
function iMouseDrag() is called when the user presses and drags the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseDrag(int mx, int my)
{
    // place your codes here
}

/*
function iMouse() is called when the user presses/releases the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        switch (gamestate)
        {
        case 1:
            if ((400 <= mx && mx <= 800) && (370 <= my && my <= 430)) gamestate = 2;
            
            else if ((400 <= mx && mx <= 800) && (290 <= my && my <= 350)) gamestate = 3;
      
            else if ((400 <= mx && mx <= 800) && (210 <= my && my <= 270)) gamestate = 4;
            
            else if ((400 <= mx && mx <= 800) && (130 <= my && my <= 190)) gamestate = 5;
            
            break;
        case 2:
            if ((400 <= mx && mx <= 800) && (370 <= my && my <= 430)) gamestate = 21;
            
            else if ((400 <= mx && mx <= 800) && (290 <= my && my <= 350)) gamestate = 22;
            
            else if ((400 <= mx && mx <= 800) && (210 <= my && my <= 270)) gamestate = 23;
            
            break;
        }
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        // place your codes here
    }
}

/*
function iMouseWheel() is called when the user scrolls the mouse wheel.
dir = 1 for up, -1 for down.
*/
void iMouseWheel(int dir, int mx, int my)
{
    // place your code here
}

/*
function iKeyboard() is called whenever the user hits a key in keyboard.
key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
    switch (key)
    {
    case 'q':
        // do something with 'q'
        break;
    // place your codes for other keys here
    default:
        break;
    }
}

/*
function iSpecialKeyboard() is called whenver user hits special keys likefunction
keys, home, end, pg up, pg down, arraows etc. you have to use
appropriate constants to detect them. A list is:
GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11,
GLUT_KEY_F12, GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN,
GLUT_KEY_PAGE_UP, GLUT_KEY_PAGE_DOWN, GLUT_KEY_HOME, GLUT_KEY_END,
GLUT_KEY_INSERT */
void iSpecialKeyboard(unsigned char key)
{
    switch (key)
    {
    case GLUT_KEY_END:
        // do something
        break;
    // place your codes for other keys here
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    // place your own initialization codes here.
    iInitializeSound();
    iPlaySound("assets/sounds/menubg.wav", true);
    iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "SpaceShooter");

    return 0;
}
