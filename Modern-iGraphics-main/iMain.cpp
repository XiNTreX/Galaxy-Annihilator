#include "iGraphics.h"
#include "iSound.h"
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 700
/*
function iDraw() is called again and again by the system.
*/
void iDraw()
{
    // place your drawing codes here
    //iClear();
    iSetColor(255,255,255) ;
    iFilledCircle(10,20,1);
    iFilledCircle(50,20,1);
    iFilledCircle(15,30,1);
    iFilledCircle(24,45,1);
    iFilledCircle(45,100,1);
    iFilledCircle(60,70,1);
    iFilledCircle(550,150,1);
    iFilledCircle(50,680,1);
    iFilledCircle(400,400,1);
    iFilledCircle(350,150,1);
    iFilledCircle(190,190,1);
    iFilledCircle(210,290,1);
    iFilledCircle(250,250,1);
    iFilledCircle(370,370,1);
    iFilledCircle(490,360,1);
    iFilledCircle(541,659,1);
    iFilledCircle(598,600,1);
    iFilledCircle(520,520,1);
    iFilledCircle(520,30,1);
    iFilledCircle(520,450,1);
    iFilledCircle(459,100,1);
    iFilledCircle(380,420,1);
    iFilledCircle(120,150,1);
    iFilledCircle(550,670,1);
    iFilledCircle(400,490,1);
    iFilledCircle(350,170,1);
    iFilledCircle(190,590,1);
    iFilledCircle(260,290,1);
    iFilledCircle(395,291,1);
    iFilledCircle(355,451,1);
    iFilledCircle(100,367,1);
    iFilledCircle(501,65,1);
    iFilledCircle(190,230,1);
    iFilledCircle(530,290,1);
    iFilledCircle(115,300,1);
    iFilledCircle(214,409,1);
    iFilledCircle(415,190,1);
    iFilledCircle(460,610,1);
    iFilledCircle(540,190,1);
    iFilledCircle(60,610,1);
    iFilledCircle(445,460,1);
    iFilledCircle(390,550,1);
    iFilledCircle(390,490,1);
    iFilledCircle(215,299,1);
    iFilledCircle(251,150,1);
    iFilledCircle(290,310,1);
    iFilledCircle(491,36,1);
    iFilledCircle(541,65,1);
    iFilledCircle(598,60,1);
    iFilledCircle(520,52,1);
    iFilledCircle(520,30,1);
    iFilledCircle(520,45,1);
    iFilledCircle(459,10,1);
    iFilledCircle(380,42,1);
    iFilledCircle(120,15,1);
    iFilledCircle(550,67,1);
    iFilledCircle(400,49,1);
    iFilledCircle(350,17,1);
    iFilledCircle(190,59,1);
    iFilledCircle(260,29,1);
    iFilledCircle(395,29,1);
    iFilledCircle(355,45,1);
    iFilledCircle(100,36,1);
    iFilledCircle(501,65,1);
    iFilledCircle(10,200,1);
    iFilledCircle(50,250,1);
    iFilledCircle(15,31,1);
    iFilledCircle(24,450,1);
    iFilledCircle(45,190,1);
    iFilledCircle(60,690,1);
    iFilledCircle(550,650,1);
    iFilledCircle(70,680,1);
    iFilledCircle(40,400,1);
    iFilledCircle(35,150,1);
    iFilledCircle(19,190,1);
    iFilledCircle(21,290,1);
    iFilledCircle(25,250,1);
    iFilledCircle(37,370,1);
    iFilledCircle(49,360,1);
    iFilledCircle(54,659,1);
    iFilledCircle(59,600,1);
    iFilledCircle(52,520,1);
    iFilledCircle(52,300,1);
    iFilledCircle(52,450,1);
    iFilledCircle(45,100,1);
    iFilledCircle(38,420,1);
    iFilledCircle(12,150,1);
    iFilledCircle(55,670,1);
    iFilledCircle(40,490,1);
    iFilledCircle(35,170,1);
    iFilledCircle(19,590,1);
    iFilledCircle(26,290,1);
    iFilledCircle(39,291,1);
    iFilledCircle(35,451,1);
    iFilledCircle(10,367,1);
    iFilledCircle(501,650,1);
    iFilledCircle(300,590,1);
    iFilledCircle(260,290,1);
    iFilledCircle(315,529,1);
    iFilledCircle(355,545,1);
    iFilledCircle(300,536,1);
    iFilledCircle(311,595,1);
    iFilledCircle(310,690,1);
    iFilledCircle(350,650,1);
    iFilledCircle(345,631,1);
    iFilledCircle(324,450,1);
    iFilledCircle(345,690,1);
    iFilledCircle(360,690,1);
    iFilledCircle(350,650,1);
    iFilledCircle(370,680,1);
     iFilledCircle(200,590,1);
    iFilledCircle(210,290,1);
    iFilledCircle(215,529,1);
    iFilledCircle(255,545,1);
    iFilledCircle(200,536,1);
    iFilledCircle(211,595,1);
    iFilledCircle(210,690,1);
    iFilledCircle(250,650,1);
    iFilledCircle(145,631,1);
    iFilledCircle(224,450,1);
    iFilledCircle(245,690,1);
    iFilledCircle(260,690,1);
    iFilledCircle(250,650,1);
    iFilledCircle(270,680,1);
    iFilledCircle(610,20,1);
    iFilledCircle(650,20,1);
    iFilledCircle(615,30,1);
    iFilledCircle(624,45,1);
    iFilledCircle(645,100,1);
    iFilledCircle(660,70,1);
    iFilledCircle(1150,150,1);
    iFilledCircle(650,680,1);
    iFilledCircle(1000,400,1);
    iFilledCircle(950,150,1);
    iFilledCircle(790,190,1);
    iFilledCircle(810,290,1);
    iFilledCircle(850,250,1);
    iFilledCircle(970,370,1);
    iFilledCircle(1090,360,1);
    iFilledCircle(1141,659,1);
    iFilledCircle(1198,600,1);
    iFilledCircle(1120,520,1);
    iFilledCircle(1120,30,1);
    iFilledCircle(1120,450,1);
    iFilledCircle(1059,100,1);
    iFilledCircle(980,420,1);
    iFilledCircle(720,150,1);
    iFilledCircle(1050,670,1);
    iFilledCircle(1900,490,1);
    iFilledCircle(950,170,1);
    iFilledCircle(790,590,1);
    iFilledCircle(860,290,1);
    iFilledCircle(995,291,1);
    iFilledCircle(955,451,1);
    iFilledCircle(700,367,1);
    iFilledCircle(1001,65,1);
    iFilledCircle(790,230,1);
    iFilledCircle(1030,290,1);
    iFilledCircle(715,300,1);
    iFilledCircle(814,409,1);
    iFilledCircle(1015,190,1);
    iFilledCircle(1060,610,1);
    iFilledCircle(1140,190,1);
    iFilledCircle(660,610,1);
    iFilledCircle(1045,460,1);
    iFilledCircle(990,550,1);
    iFilledCircle(990,490,1);
    iFilledCircle(815,299,1);
    iFilledCircle(851,150,1);
    iFilledCircle(890,310,1);
    iFilledCircle(1091,36,1);
    iFilledCircle(1141,65,1);
    iFilledCircle(1198,60,1);
    iFilledCircle(1120,52,1);
    iFilledCircle(1120,30,1);
    iFilledCircle(1120,45,1);
    iFilledCircle(1059,10,1);
    iFilledCircle(980,42,1);
    iFilledCircle(720,15,1);
    iFilledCircle(1150,67,1);
    iFilledCircle(1000,49,1);
    iFilledCircle(950,17,1);
    iFilledCircle(790,59,1);
    iFilledCircle(860,29,1);
    iFilledCircle(995,29,1);
    iFilledCircle(955,45,1);
    iFilledCircle(700,36,1);
    iFilledCircle(1001,65,1);
    iFilledCircle(610,200,1);
    iFilledCircle(650,250,1);
    iFilledCircle(615,31,1);
    iFilledCircle(624,450,1);
    iFilledCircle(645,190,1);
    iFilledCircle(660,690,1);
    iFilledCircle(1050,650,1);
    iFilledCircle(670,680,1);
    iFilledCircle(640,400,1);
    iFilledCircle(635,150,1);
    iFilledCircle(619,190,1);
    iFilledCircle(621,290,1);
    iFilledCircle(625,250,1);
    iFilledCircle(637,370,1);
    iFilledCircle(649,360,1);
    iFilledCircle(654,659,1);
    iFilledCircle(659,600,1);
    iFilledCircle(652,520,1);
    iFilledCircle(652,300,1);
    iFilledCircle(652,450,1);
    iFilledCircle(645,100,1);
    iFilledCircle(638,420,1);
    iFilledCircle(612,150,1);
    iFilledCircle(655,670,1);
    iFilledCircle(640,490,1);
    iFilledCircle(635,170,1);
    iFilledCircle(619,590,1);
    iFilledCircle(626,290,1);
    iFilledCircle(639,291,1);
    iFilledCircle(635,451,1);
    iFilledCircle(610,367,1);
    iFilledCircle(1001,650,1);
    iFilledCircle(900,590,1);
    iFilledCircle(860,290,1);
    iFilledCircle(915,529,1);
    iFilledCircle(955,545,1);
    iFilledCircle(900,536,1);
    iFilledCircle(911,595,1);
    iFilledCircle(910,690,1);
    iFilledCircle(950,650,1);
    iFilledCircle(945,631,1);
    iFilledCircle(924,450,1);
    iFilledCircle(945,690,1);
    iFilledCircle(960,690,1);
    iFilledCircle(950,650,1);
    iFilledCircle(970,680,1);
     iFilledCircle(800,590,1);
    iFilledCircle(810,290,1);
    iFilledCircle(815,529,1);
    iFilledCircle(855,545,1);
    iFilledCircle(800,536,1);
    iFilledCircle(811,595,1);
    iFilledCircle(810,690,1);
    iFilledCircle(850,650,1);
    iFilledCircle(745,631,1);
    iFilledCircle(824,450,1);
    iFilledCircle(845,690,1);
    iFilledCircle(860,690,1);
    iFilledCircle(850,650,1);
    iFilledCircle(870,680,1);

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
        // place your codes here
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
    iPlaySound("assets/sounds/menu bgm.mp3",true);
    iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "SpaceShooter");

    return 0;
}
