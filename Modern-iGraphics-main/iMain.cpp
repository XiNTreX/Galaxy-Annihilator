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
int ship1_health=1,ship2_health=2,ship3_health=3,ship4_health=4,ship5_health=5;

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
int enemstate=1;
int state = IDLE;
int move_idx = 0;
int des_idx = 0;
int shoot_idx = 0;
int exp1_idx=0;
int space_x = 10, space_y = 300;
int enem_x1=1200,enem_x2=1200,enem_y1=500,enem_y2=100;
int shi1=1,shi2=1,shi3=1,shi4=1,shi5=1;
char *space_image;
char *shoot_image;
char *exp1;

Sprite bullet_sprites[MAX_BULLETS];  
int bullet_on_off[MAX_BULLETS];
int bullet_x[MAX_BULLETS];  
int bullet_y[MAX_BULLETS]; 

char shoot1[1][100];

int b;
Image bg,ship11[1],ship22[1],ship33[1],ship44[1],ship55[1],ship66[1],main11[1],bullet_img[1];
Sprite main1,ship1,ship2,ship3,ship4,ship5,ship6;
char exp11[10][100];
void loadReasources(){
    iInitSprite(&ship1,-1);
	iLoadFramesFromFolder(ship11, "assets/images/sprites/enemy/Ship1/");
    iInitSprite(&ship2,-1);
	iLoadFramesFromFolder(ship22, "assets/images/sprites/enemy/Ship2/");
    iInitSprite(&ship3,-1);
	iLoadFramesFromFolder(ship33, "assets/images/sprites/enemy/Ship3/");
    iInitSprite(&ship4,-1);
	iLoadFramesFromFolder(ship44, "assets/images/sprites/enemy/Ship4/");
    iInitSprite(&ship5,-1);
	iLoadFramesFromFolder(ship55, "assets/images/sprites/enemy/Ship5/");
    iInitSprite(&ship6,-1);
	iLoadFramesFromFolder(ship66, "assets/images/sprites/enemy/Ship6/");
    iInitSprite(&main1,-1);
	iLoadFramesFromFolder(main11, "assets/images/sprites/Spaceship/Idle.png");
    
    // Initialize bullet sprites
    for(int i = 0; i < MAX_BULLETS; i++){
        iInitSprite(&bullet_sprites[i], -1);
        iLoadFramesFromFolder(bullet_img, "assets/images/sprites/Spaceship/bullet/");
        iChangeSpriteFrames(&bullet_sprites[i], bullet_img, 1);
        
    }
    
    iScaleSprite(&ship1,1.7);
    iScaleSprite(&ship2,1.7);
    iScaleSprite(&ship3,1.7);
    iScaleSprite(&ship4,1.7);
    iScaleSprite(&ship5,1.7);
    iScaleSprite(&ship6,1.7);
   
    iSetSpritePosition(&ship1,enem_x1,enem_y1);
    iSetSpritePosition(&ship2,enem_x1,enem_y2);
    iChangeSpriteFrames(&ship1, ship11, 1);
    iChangeSpriteFrames(&ship2, ship22, 1);
}

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
      for (int i = 0; i < 1; i++)
    {
        sprintf(exp11[i], "assets/images/sprites/enemy/explosions/Ship1_Explosion/Ship1_Explosion_%03d.png", i);
    }
    exp1=exp11[0];
}

void homepage()
{
    iShowImage(0, 0, "assets/images/homepage_w_menu.png");
    iLoadImage(&bg, "assets/images/mainbg.png");
    iMirrorSprite(&ship1,HORIZONTAL);
    iMirrorSprite(&ship2,HORIZONTAL);
    iMirrorSprite(&ship3,HORIZONTAL);
    iMirrorSprite(&ship4,HORIZONTAL);
    iMirrorSprite(&ship5,HORIZONTAL);
    iMirrorSprite(&ship6,HORIZONTAL);
   //iMirrorSprite(&exp11,HORIZONTAL);//
}

void difficulty()
{
    iShowImage(0, 0, "assets/images/difficulty.png");
    iPauseSound(mbgSoundIdx);
    iResumeSound(bgSoundIdx);
}


void checkBulletEnemyCollision() {
    
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullet_on_off[i] == 1) {
            
            if (iCheckCollision(&bullet_sprites[i], &ship1)) {
                bullet_on_off[i] = 0;
                ship1_health--;
                if(ship1_health<=0){
               /* iInitSprite(&exp11, -1);
	            iLoadFramesFromFolder(exp_1, "assets/images/sprites/enemy/explosions/Ship1_Explosion/");
	            iChangeSpriteFrames(&exp11, exp_1, 10);
	            iSetSpritePosition(&exp11,enem_x1,enem_y1);
                iAnimateSprite(&exp11);
                iShowSprite(&exp11);*/
                iShowImage(enem_x1,enem_x2,exp1);
                shi1=0;
                ship1_health=1;
                 }
                  // Deactivate bullet
                // Handle enemy hit (reduce health, play sound, etc.)
                
            }
            
            // Check collision with ship2
            if (iCheckCollision(&bullet_sprites[i], &ship2)) {
               bullet_on_off[i] = 0;  // Deactivate bullet
                // Handle enemy hit (reduce health, play sound, etc.)
                
            }
        }
    }
}

void mainpage1()
{
    iShowLoadedImage(0, 0, &bg);
	iWrapImage(&bg, -1.5);
    iPauseSound(bgSoundIdx);

    iShowImage(space_x, space_y, space_image);
  

    iShowSprite(&ship1);
    iShowSprite(&ship2);
    
    // Display active bullet sprites
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullet_on_off[i] == 1)
        {
            iShowSprite(&bullet_sprites[i]);
        }
    }
    
    // Check for collisions
    checkBulletEnemyCollision();
}

void update_enemy(){
    if(gamestate==21){
        switch(enemstate){
            case 1:
            enem_x1-=20;
            enem_x2-=20;
            iSetSpritePosition(&ship1,enem_x1,enem_y1);
            iSetSpritePosition(&ship2,enem_x2,enem_y2);
            iAnimateSprite(&ship1);
            iAnimateSprite(&ship2);
            if(shi1==0){
                enem_x1=1300;
                shi1=1;
            }
            
            if(enem_x1<-100){
                enem_x1=1200;
                enem_y1-=30;enem_y2-=30;
                iSetSpritePosition(&ship1,enem_x1,enem_y1);
                iSetSpritePosition(&ship2,enem_x2,enem_y2);
            }
            if(enem_x2<-100){
                enem_x2=1200;
                enem_y1-=30;enem_y2-=30;
                iSetSpritePosition(&ship1,enem_x1,enem_y1);
                iSetSpritePosition(&ship2,enem_x2,enem_y2);
            }
            if(enem_y1<0){
                enem_y1=200;
            }if(enem_y2<0){
                enem_y2=600;
            }
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
void explosion(){
      if(ship1_health==0){
        exp1=exp11[exp1_idx];
        exp1_idx=(exp1_idx+1)%10;
    }
}

// Updated shoot function for sprite bullets
void shoot()
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullet_on_off[i] == 1)
        {
            // Move bullet to the right
            bullet_x[i] += 80;
            
            // Update sprite position
            iSetSpritePosition(&bullet_sprites[i], bullet_x[i], bullet_y[i]);
            
            // Deactivate bullet if it goes off screen
            if (bullet_x[i] > SCREEN_WIDTH)
            {
                bullet_on_off[i] = 0;
            }
        }
    }
}

void iDraw()
{
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

void iMouseMove(int mx, int my)
{
}

void iMouseDrag(int mx, int my)
{
}

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
}

void iMouseWheel(int dir, int mx, int my)
{
}

void iKeyboard(unsigned char key)
{
    switch (key)
    {
    case 'w':
        space_y += 15;
        if (space_y > 580)
        {
            space_y = 580;
        }
        break;
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
        if (space_x > 1080)
        {
            space_x = 1080;
        }
        state = MOVE;
        break;
    case 's':
        space_y -= 15;
        if (space_y < 0)
        {
            space_y = 0;
        }
        break;
    case 'm':
        if (s_on_off == 0)
        {
            iPauseSound(bgSoundIdx);
            iPauseSound(mbgSoundIdx);
            s_on_off++;
        }
        else if (s_on_off == 1)
        {
            iResumeSound(bgSoundIdx);
            iResumeSound(mbgSoundIdx);
            s_on_off--;
        }
        break;
    case 'p':
        // Create two bullet sprites (dual row)
        for (int i = 0; i < MAX_BULLETS - 1; i += 2)  // Increment by 2 for pairs
        {
            if (bullet_on_off[i] == 0 && bullet_on_off[i+1] == 0)
            {
                // First bullet (upper row)
                bullet_x[i] = space_x + 190;
                bullet_y[i] = space_y + 90;
                iSetSpritePosition(&bullet_sprites[i], bullet_x[i], bullet_y[i]);
                bullet_on_off[i] = 1;
                
                // Second bullet (lower row)
                bullet_x[i+1] = space_x + 190;
                bullet_y[i+1] = space_y + 77;
                iSetSpritePosition(&bullet_sprites[i+1], bullet_x[i+1], bullet_y[i+1]);
                
                bullet_on_off[i+1] = 1;
                
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

void iSpecialKeyboard(unsigned char key)
{
    switch (key)
    {
    case GLUT_KEY_END:
        if (gamestate == 2 || gamestate == 3 || gamestate == 4 || gamestate == 5)
            gamestate = 1;
        else if (gamestate == 21 || gamestate == 22 || gamestate == 23)
            gamestate = 2;
        break;
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    populate_space_images();
    loadReasources();
    iInitializeSound();
    bgSoundIdx = iPlaySound("assets/sounds/menubg.wav", true);

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullet_on_off[i] = 0;
    }

    iSetTimer(100, update_space);
    iSetTimer(50, shoot);
    iSetTimer(100,update_enemy);
    iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "SpaceShooter");

    return 0;
}