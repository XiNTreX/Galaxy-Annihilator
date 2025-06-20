#include "iGraphics.h"
#include <iostream>
#include "iSound.h"
using namespace std;
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 700
#define MAX_BULLETS 50

int bgSoundIdx = -1;
int mbgSoundIdx = -1;
int s_on_off = 0;
int a = 0;

char move_idle[6][100];
char des_idle[21][100];
char shoot_idle[4][100];
char idle[1][100];
enum
{
    IDLE,
    MOVE,
    SHOOT,
    DESTROY
};
int gamestate = 1;
int state = IDLE;
int move_idx = 0;
int des_idx = 0;
int shoot_idx = 0;
int space_x = 10, space_y = 300;

char *space_image;
char *shoot_image;
int shoot_x[MAX_BULLETS];
int shoot_y1[MAX_BULLETS];
int shoot_y2[MAX_BULLETS];
int bullet_active[MAX_BULLETS];

char shoot1[1][100];

int b;
/*
function iDraw() is called again and again by the system.
*/
void populate_space_images()
{
    for (int i = 0; i < 21; i++)
    {
        sprintf(des_idle[i], "assets/images/sprites/Spaceship/tile%03d.png", i);
    }
    for (int i = 0; i < 4; i++)
    {
        sprintf(shoot_idle[i], "assets/images/sprites/Spaceship/shoot%03d.png", i);
    }
    for (int i = 0; i < 6; i++)
    {
        sprintf(move_idle[i], "assets/images/sprites/Spaceship/move%03d.png", i);
    }
    for (int i = 0; i < 1; i++)
    {
        sprintf(idle[i], "assets/images/sprites/Spaceship/Idle.png", i);
    }
    space_image = idle[0];
    for (int i = 0; i < 1; i++)
    {
        sprintf(shoot1[i], "assets/images/sprites/Spaceship/Charge_2.png", i);
    }
    shoot_image = shoot1[0];
}

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
    iPauseSound(bgSoundIdx);

    iShowImage(0, 0, "assets/images/mainbg.png");
    // iWrapImage(bg_image,-50);//
    iShowImage(space_x, space_y, space_image);
    
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullet_active[i] == 1)
        {
            iShowImage(shoot_x[i], shoot_y1[i], shoot_image);
            iShowImage(shoot_x[i], shoot_y2[i], shoot_image);
        }
    }
}

void update_space()
{
    switch (state)
    {
    case IDLE:
        space_image = idle[0];
        break;
    case MOVE:
        space_image = move_idle[move_idx];
        move_idx = (move_idx + 1) % 6;
        if (move_idx == 0)
        {
            state = IDLE;
        }
        break;
    case DESTROY:
        space_image = des_idle[des_idx];
        des_idx = (des_idx + 1) % 21;
        break;
    case SHOOT:
        space_image = shoot_idle[shoot_idx];
        shoot_idx = (shoot_idx + 1) % 4;
        
        if (shoot_idx == 3)
        {
            state = IDLE;
        }
       
        break;
    }
}
void shoot()
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullet_active[i] == 1)
        {
            shoot_x[i] += 80;
            if (shoot_x[i] > SCREEN_WIDTH)
            {
                bullet_active[i] = 0;
            }
        }
    }
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
            if ((430 <= mx && mx <= 780) && (350 <= my && my <= 405))
                gamestate = 2;

            else if ((430 <= mx && mx <= 780) && (270 <= my && my <= 325))
                gamestate = 3;

            else if ((430 <= mx && mx <= 780) && (190 <= my && my <= 245))
                gamestate = 4;

            else if ((430 <= mx && mx <= 780) && (110 <= my && my <= 165))
                gamestate = 5;

            break;

        case 2:
            if ((430 <= mx && mx <= 780) && (325 <= my && my <= 380))
                gamestate = 21;

            else if ((430 <= mx && mx <= 780) && (225 <= my && my <= 280))
                gamestate = 22;

            else if ((430 <= mx && mx <= 780) && (120 <= my && my <= 180))
                gamestate = 23;

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
    case 'w':
        // do something with 'q'
        space_y += 15;
        if (space_y > 555)
        {
            space_y = 555;
        }

        break;
    // place your codes for other keys here
    case 'a':
        space_x -= 15;
        if (space_x < 0)
        {
            space_x = 0;
        }
        state = MOVE;
        break;
    case 'd':
        space_x += 15;
        if (space_x > 1010)
        {
            space_x = 1010;
        }
        state = MOVE;
        break;
    case 's':
        // do something with 'q'
        space_y -= 15;
        if (space_y < -48)
        {
            space_y = -48;
        }
        break;
    case 'm':
        if (s_on_off == 0)
        {
            iPauseSound(bgSoundIdx);
            //iPauseSound(mbgSoundIdx);
            s_on_off++;
        }
        else if (s_on_off == 1)
        {
            iResumeSound(bgSoundIdx);
            //iResumeSound(mbgSoundIdx);
            s_on_off--;
        }
        break;
    case 'p':
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (bullet_active[i] == 0)
            {
                shoot_x[i] = space_x + 190;
                shoot_y1[i] = space_y + 90;
                shoot_y2[i] = space_y + 77;
                bullet_active[i] = 1;
                break;
            }
        }
        state = SHOOT;
        shoot_idx = 0;

        break;
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
        if (gamestate == 2 || gamestate == 3 || gamestate == 4 || gamestate == 5)
            gamestate = 1;
        /*else if (gamestate == 21 || gamestate == 22 || gamestate == 23)
            gamestate = 2;*/

        break;
    // place your codes for other keys here
    case GLUT_KEY_INSERT:

    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    // place your own initialization codes here.
    populate_space_images();
    iInitializeSound();
    bgSoundIdx = iPlaySound("assets/sounds/menubg.wav", true);

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullet_active[i] = 0;
    }

    iSetTimer(100, update_space);
    iSetTimer(50, shoot);

    iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "SpaceShooter");

    return 0;
}