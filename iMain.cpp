#include "iGraphics.h"
#include <iostream>
#include "iSound.h"
using namespace std;

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 700
#define MAX_BULLETS 50

int homeidx =-1, mainidx=-1;
int sound_check = 0;
int fullscreen = 0;
int gamestate = 1;
Image home, diff, mainbg;
int homesound, mainsound;
Sprite met;
Sprite spaceship, enem1, enem2, enem3, enem4, enem5, enem6, bullet_sprites[MAX_BULLETS], ebulsprite[MAX_BULLETS];
Image s_boost[6], s_exp[7], s_shoot[4], idle[1], bullet_img[1], mete[1];
Image e1exp[10], e1idle[1];
Image e2exp[12], e2idle[1];
Image e1bul[1];
int move_ud = 280, move_lf = 0;
int bullet_x[MAX_BULLETS], bullet_y[MAX_BULLETS], bullet_active[MAX_BULLETS];
int ebullet_x[MAX_BULLETS], ebullet_y[MAX_BULLETS], ebullet_active[MAX_BULLETS];
int enem1_fire_timer = 0; // Timer for enem1 firing
int enem2_fire_timer = 0; // Timer for enem2 firing
enum { IDLE, BOOST, SHOOT, EXP };
int ship_state = IDLE;
int boost_idx = 0;
int shoot_idx = 0;
int exp_idx=0;
int metx=550, mety=1000;
bool enem1_active = false; // Tracks if enem1 is active
int enem1_x = 1200, enem1_y = 500; // enem1 position
int bul1_x=enem1_x-8;
bool enem1_bul_active = false; // Tracks if enem1 bullet is active
int enem1_respawn_timer = 0; // Timer for respawn (ms)
bool enem1_exploding = false; // Tracks if enem1 is exploding
int enem1_exp_idx = 0; // Explosion animation index
bool enem2_active = false; // Tracks if enem2 is active
int enem2_x = 1200, enem2_y = 100; // enem2 position
int enem2_respawn_timer = 0; // Timer for respawn (ms)
bool enem2_exploding = false; // Tracks if enem2 is exploding
int enem2_exp_idx = 0; // Explosion animation index
bool shipexp=false;
bool meteor=false;
bool key_w = false, key_a = false, key_s = false, key_d = false; // Track key states
int enem1hp=1, enem2hp=2, enem3hp=3, enem4hp=4, enem5hp=5;
char des[7][100]; int a=0;

void loadresources() {
    // Load images
    iLoadImage(&home, "assets/images/homepage_w_menu.png");
    iLoadImage(&diff, "assets/images/difficulty.png");
    iLoadImage(&mainbg, "assets/images/mainbg.png");
    //meteor
    iInitSprite(&met);
    iLoadFramesFromFolder(mete, "assets/images/sprites/Meteors/mainmet/");
    iChangeSpriteFrames(&met, mete, 1);
    iSetSpritePosition(&met, metx, mety);
    iScaleSprite(&met, .3);
    // Load spaceship frames
    iLoadImage(&idle[0], "assets/images/sprites/Spaceship/Idle.png");
    iLoadFramesFromFolder(s_boost, "assets/images/sprites/Spaceship/boost/");
    iLoadFramesFromFolder(s_shoot, "assets/images/sprites/Spaceship/shoot/");
    iLoadFramesFromFolder(s_exp, "assets/images/sprites/Spaceship/explosion/");
    // Load enemy frames
    iLoadImage(&e1idle[0], "assets/images/sprites/enemy/Ship1/Ship1.png");
    iLoadFramesFromFolder(e1exp, "assets/images/sprites/enemy/explosions/Ship1_Explosion/");
    iLoadImage(&e2idle[0], "assets/images/sprites/enemy/Ship2/Ship2.png");
    iLoadFramesFromFolder(e2exp, "assets/images/sprites/enemy/explosions/Ship2_Explosion/");
    // Initialize spaceship sprite
    iInitSprite(&spaceship);
    iChangeSpriteFrames(&spaceship, idle, 1);
    iSetSpritePosition(&spaceship, move_lf, move_ud);
    // Load bullet image (for both spaceship and enemy bullets)
    iLoadImage(&bullet_img[0], "assets/images/sprites/Spaceship/Charge_2.png");
    for (int i = 0; i < MAX_BULLETS; i++) {
        iInitSprite(&bullet_sprites[i]);
        iChangeSpriteFrames(&bullet_sprites[i], bullet_img, 1);
        bullet_active[i] = 0; // Initially inactive
    }
    // Enemy bullet (using same image as spaceship)
    iLoadImage(&e1bul[0], "assets/images/sprites/enemy/Shots/Shot1/enembullet.png");
    for (int i = 0; i < MAX_BULLETS; i++) {
        iInitSprite(&ebulsprite[i]);
        iChangeSpriteFrames(&ebulsprite[i], e1bul, 1);
        ebullet_active[i] = 0; // Initially inactive
    }
    // Initialize enemy sprites
    iInitSprite(&enem1);
    iChangeSpriteFrames(&enem1, e1idle, 1);
    iSetSpritePosition(&enem1, enem1_x, enem1_y);
    iInitSprite(&enem2);
    iScaleSprite(&enem2, 1.7);
    iChangeSpriteFrames(&enem2, e2idle, 1);
    iSetSpritePosition(&enem2, enem2_x, enem2_y);
    iInitSprite(&enem3);
    iInitSprite(&enem4);
    iInitSprite(&enem5);
    iInitSprite(&enem6);
}

void updateAnimation() {
    if (ship_state == BOOST) {
        boost_idx = (boost_idx + 1) % 6; // Cycle through 6 boost frames
        iChangeSpriteFrames(&spaceship, s_boost, 6);
    }
    if (ship_state == SHOOT) {
        shoot_idx = (shoot_idx + 1) % 4; // Cycle through 4 shoot frames
        iChangeSpriteFrames(&spaceship, s_shoot, 4);
    }
    if (ship_state == EXP && shipexp) {
        exp_idx = (exp_idx + 1) % 7;
        exp_idx++;
        shipexp = false;
        iChangeSpriteFrames(&spaceship, s_exp, 7);
    }
}

void moveSpaceship() {
    if (gamestate == 21) { // Only move in main game
        if (key_w) {
            move_ud += 25;
            if (move_ud >= 555) move_ud = 555;
        }
        if (key_s) {
            move_ud -= 25;
            if (move_ud < -48) move_ud = -48;
        }
        if (key_a) {
            move_lf -= 25;
            if (move_lf < 0) move_lf = 0;
            ship_state = BOOST;
            iChangeSpriteFrames(&spaceship, s_boost, 6);
        }
        if (key_d) {
            move_lf += 25;
            if (move_lf > 1010) move_lf = 1010;
            ship_state = BOOST;
            iChangeSpriteFrames(&spaceship, s_boost, 6);
        }
        if (!key_a && !key_d && ship_state == BOOST) {
            ship_state = IDLE;
            iChangeSpriteFrames(&spaceship, idle, 1);
        }
        iSetSpritePosition(&spaceship, move_lf, move_ud);
    }
}

double meteorRotationAngle = 0.0;
int xthik=0;
void updatemeteor() {
    metx -= 30;
    mety -= 30;
    if (metx < -50 || mety < -70) {
        metx = 550 + xthik;
        mety = 750;
        meteor = false;  // Reset the flag after repositioning
        xthik += 200; // Increment xthik for next repositioning
        if (xthik > 500) {
            xthik = 0; // Reset xthik to avoid overflow
        }
    }
    iSetSpritePosition(&met, metx, mety);   // Update sprite position
    double rotationCenterX = metx + 45.0;
    double rotationCenterY = mety + 39.3;
    meteorRotationAngle += 15.0;  // Rotate 15 degrees per frame
    if (meteorRotationAngle >= 360.0) {
        meteorRotationAngle = 0.0;
    }
    iRotateSprite(&met, rotationCenterX, rotationCenterY, meteorRotationAngle);
}

void enem_shoot() {
    if (gamestate == 21) {
        // Fire for enem1
        if (enem1_active && !enem1_exploding) {
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (!ebullet_active[i]) {
                    ebullet_x[i] = enem1_x - 10; // Start left of enemy
                    ebullet_y[i] = enem1_y + 50; // Approximate center of enemy
                    iSetSpritePosition(&ebulsprite[i], ebullet_x[i], ebullet_y[i]);
                    ebullet_active[i] = 1;
                    break;
                }
            }
        }
        // Fire for enem2
        if (enem2_active && !enem2_exploding) {
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (!ebullet_active[i]) {
                    ebullet_x[i] = enem2_x - 10; // Start left of enemy
                    ebullet_y[i] = enem2_y + 90; // Approximate center of enemy
                    iSetSpritePosition(&ebulsprite[i], ebullet_x[i], ebullet_y[i]);
                    ebullet_active[i] = 1;
                    break;
                }
            }
        }
    }
}

void updateEnemy() {
    if (gamestate == 21) { // Only update in main game
        // Respawn logic
        if (!enem1_active && !enem1_exploding) {
            enem1_respawn_timer += 50; // Increment timer (50ms per call)
            if (enem1_respawn_timer >= 1000) { // Respawn every 1 second
                enem1_active = true;
                enem1_x = 1200; // Start at right edge
                enem1_y = 500; // Fixed y-position
                iSetSpritePosition(&enem1, enem1_x, enem1_y);
                iChangeSpriteFrames(&enem1, e1idle, 1);
                enem1_respawn_timer = 0; // Reset timer
                enem1_fire_timer = 0; // Reset firing timer
            }
        }
        // Movement and firing logic for enem1
        if (enem1_active && !enem1_exploding) {
            enem1_x -= 10; // Move left at 10 pixels per 50ms
            iSetSpritePosition(&enem1, enem1_x, enem1_y);
            if (enem1_x < -100) { // Off-screen
                enem1_active = false; // Deactivate until next respawn
            }
            // Firing logic
            enem1_fire_timer += 50; // Increment every 50ms
            if (enem1_fire_timer >= 1000) { // Fire every 1 second
                enem_shoot();
                enem1_fire_timer = 0;
            }
        }
        if (!enem2_active && !enem2_exploding) {
            enem2_respawn_timer += 50; // Increment timer (50ms per call)
            if (enem2_respawn_timer >= 1000) { // Respawn every 1 second
                enem2_active = true;
                enem2_x = 1200; // Start at right edge
                enem2_y = 100; // Fixed y-position
                iSetSpritePosition(&enem2, enem2_x, enem2_y);
                iChangeSpriteFrames(&enem2, e2idle, 1);
                enem2_respawn_timer = 0; // Reset timer
                enem2_fire_timer = 0; // Reset firing timer
            }
        }
        // Movement and firing logic for enem2
        if (enem2_active && !enem2_exploding) {
            enem2_x -= 10; // Move left at 10 pixels per 50ms
            iSetSpritePosition(&enem2, enem2_x, enem2_y);
            if (enem2_x < -100) { // Off-screen
                enem2_active = false; // Deactivate until next respawn
            }
            // Firing logic
            enem2_fire_timer += 50; // Increment every 50ms
            if (enem2_fire_timer >= 1000) { // Fire every 1 second
                enem_shoot();
                enem2_fire_timer = 0;
            }
        }
    }
}

void updateEnemyExplosion() {
    if (enem1_exploding) {
        enem1_exp_idx++; // Advance frame
        iChangeSpriteFrames(&enem1, e1exp, 10);
        iSetSpritePosition(&enem1, enem1_x, enem1_y);
        for (int i = 0; i < 10; i++)
            iAnimateSprite(&enem1);
        if (enem1_exp_idx >= 10) { // Animation complete
            enem1_exploding = false;
            enem1_active = false; // Vanish after explosion
            enem1_exp_idx = 0;
        }
    }
    if (enem2_exploding) {
        enem2_exp_idx++; // Advance frame
        iChangeSpriteFrames(&enem2, e2exp, 12);
        iSetSpritePosition(&enem2, enem2_x, enem2_y);
        for (int i = 0; i < 12; i++)
            iAnimateSprite(&enem2);
        if (enem2_exp_idx >= 12) { // Animation complete
            enem2_exploding = false;
            enem2_active = false; // Vanish after explosion
            enem2_exp_idx = 0;
        }
    }
}

void checkEnemSpaceCollision() {
    if (gamestate == 21) {
        if (iCheckCollision(&spaceship, &enem1)) {
            enem1_active = false;
            enem1_exploding = true;
            enem1_exp_idx = 0;
            iChangeSpriteFrames(&enem1, e1exp, 10);
            shipexp = true;
            ship_state = EXP;
            exp_idx = 0;
        }
        if (iCheckCollision(&spaceship, &enem2)) {
            enem2_active = false;
            enem2_exploding = true;
            enem2_exp_idx = 0;
            iChangeSpriteFrames(&enem2, e2exp, 12);
            shipexp = true;
            ship_state = EXP;
            exp_idx = 0;
        }
        if (iCheckCollision(&spaceship, &met)) {
            metx = 550;
            mety = 750;
            meteor = true; // Set meteor flag to true
            shipexp = true;
            ship_state = EXP;
            exp_idx = 0;
        }
    }
}

void checkBulletEnemyCollision() {
    if (gamestate == 21) {
        // Check enem1 collisions
        if (enem1_active && !enem1_exploding) {
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bullet_active[i]) {
                    if (iCheckCollision(&bullet_sprites[i], &enem1)) {
                        bullet_active[i] = 0; // Deactivate bullet
                        enem1_active = false; // Stop enemy movement
                        enem1_exploding = true; // Start explosion
                        enem1_exp_idx = 0; // Reset animation
                        iChangeSpriteFrames(&enem1, e1exp, 10);
                        iSetSpritePosition(&enem1, enem1_x, enem1_y);
                    }
                }
            }
        }
        // Check enem2 collisions
        if (enem2_active && !enem2_exploding) {
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bullet_active[i]) {
                    if (iCheckCollision(&bullet_sprites[i], &enem2)) {
                        bullet_active[i] = 0; // Deactivate bullet
                        enem2hp--;
                        if (enem2hp <= 0) {
                            enem2_active = false; // Stop enemy movement
                            enem2_exploding = true; // Start explosion
                            enem2_exp_idx = 0; // Reset animation
                            iChangeSpriteFrames(&enem2, e2exp, 12);
                            iSetSpritePosition(&enem2, enem2_x, enem2_y);
                        }
                    }
                }
            }
        }
    }
}

void homepage() {
    iShowLoadedImage(0, 0, &home);
}

void difficulty() {
    iShowLoadedImage(0, 0, &diff);
}

void sound_manage() {
    if (gamestate == 1) {
        homeidx = iPlaySound("assets/sounds/menubg.wav", true, 70);
    }
    else if (gamestate == 21) {
        iPauseSound(homeidx);
        mainidx = iPlaySound("assets/sounds/mainbg.wav", true, 70);
    }
}

void mainpage1() {
    iShowLoadedImage(0, 0, &mainbg);
    iWrapImage(&mainbg, -2);
    iShowSprite(&spaceship);
    if (enem1_active || enem1_exploding) iShowSprite(&enem1);
    if (enem2_active || enem2_exploding) iShowSprite(&enem2);
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullet_active[i]) {
            iShowSprite(&bullet_sprites[i]);
        }
    }
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (ebullet_active[i]) {
            iShowSprite(&ebulsprite[i]);
        }
    }
    iShowSprite(&met);
    checkBulletEnemyCollision();
    checkEnemSpaceCollision();
}

void moveBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullet_active[i]) {
            bullet_x[i] += 30; // Move right (spaceship bullets)
            iSetSpritePosition(&bullet_sprites[i], bullet_x[i], bullet_y[i]);
            if (bullet_x[i] > SCREEN_WIDTH) {
                bullet_active[i] = 0; // Deactivate if off-screen
            }
        }
    }
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (ebullet_active[i]) {
            ebullet_x[i] -= 30; // Move left (enemy bullets)
            iSetSpritePosition(&ebulsprite[i], ebullet_x[i], ebullet_y[i]);
            if (ebullet_x[i] < 0) {
                ebullet_active[i] = 0; // Deactivate if off-screen
            }
        }
    }
}

void iDraw() {
    iClear();
    switch (gamestate) {
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

void iMouseMove(int mx, int my) {}

void iMouseDrag(int mx, int my) {}

void iMouse(int button, int state, int mx, int my) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        switch (gamestate) {
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
                if ((430 <= mx && mx <= 780) && (325 <= my && my <= 380)) {
                    gamestate = 21;
                    sound_manage();
                }
                else if ((430 <= mx && mx <= 780) && (225 <= my && my <= 280))
                    gamestate = 22;
                else if ((430 <= mx && mx <= 780) && (120 <= my && my <= 180))
                    gamestate = 23;
                break;
        }
    }
}

void iMouseWheel(int dir, int mx, int my) {}

void iKeyboard(unsigned char key, int state) {
    if (state == GLUT_DOWN) {
        switch (key) {
            case 'q':
                iCloseWindow();
                break;
            case 'w':
                key_w = true;
                break;
            case 's':
                key_s = true;
                break;
            case 'a':
                key_a = true;
                break;
            case 'd':
                key_d = true;
                break;
            case 'p':
                ship_state = SHOOT;
                iChangeSpriteFrames(&spaceship, s_shoot, 4);
                iAnimateSprite(&spaceship);
                for (int i = 0; i < MAX_BULLETS - 1; i++) { // Ensure room for 2 bullets
                    if (!bullet_active[i] && !bullet_active[i + 1]) {
                        // First bullet (upper)
                        bullet_x[i] = move_lf + 190; // Start at spaceship's front
                        bullet_y[i] = move_ud + 90; // Above center
                        iSetSpritePosition(&bullet_sprites[i], bullet_x[i], bullet_y[i]);
                        bullet_active[i] = 1;
                        // Second bullet (lower)
                        bullet_x[i + 1] = move_lf + 190;
                        bullet_y[i + 1] = move_ud + 77; // Below center
                        iSetSpritePosition(&bullet_sprites[i + 1], bullet_x[i + 1], bullet_y[i + 1]);
                        bullet_active[i + 1] = 1;
                        break;
                    }
                }
            
                break;
            case 'f':
                
                if (fullscreen == 0) 
                {
                    iEnterFullscreen();
                    fullscreen++;
                    break;
                }
                else
                {
                    iLeaveFullscreen();
                    fullscreen--;
                    break;
                }
        }
    } else if (state == GLUT_UP) {
        switch (key) {
            case 'w':
                key_w = false;
                break;
            case 's':
                key_s = false;
                break;
            case 'a':
                key_a = false;
                break;
            case 'd':
                key_d = false;
                break;
            case 'p':
                ship_state = IDLE;
                iChangeSpriteFrames(&spaceship, idle, 1);
                break;
            case 'm':
                if (sound_check == 0) {
                    if (gamestate == 1 || gamestate == 2) {
                        iPauseSound(homeidx);
                        sound_check++;
                    }
                    if (gamestate == 21) {
                        iPauseSound(mainidx);
                        sound_check++;
                    }
                }
                else {
                    if (gamestate == 1 || gamestate == 2) {
                        iResumeSound(homeidx);
                        sound_check = 0;
                    }
                    if (gamestate == 21) {
                        iResumeSound(mainidx);
                        sound_check = 0;
                    }
                }
        }
    }
}

void iSpecialKeyboard(unsigned char key, int state) {
    switch (key) {
        default:
            break;
    }
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    iInitializeSound();
    loadresources();
    sound_manage();
    iSetTimer(50, moveBullets); // Update bullets every 50ms
    iSetTimer(50, moveSpaceship); // Update spaceship movement every 50ms
    iSetTimer(200, updateAnimation); // Update animation every 200ms
    iSetTimer(50, updateEnemy); // Update enemy every 50ms
    iSetTimer(50, updateEnemyExplosion); // Update explosion every 50ms
    iSetTimer(225, updatemeteor);
    iOpenWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Galaxy-Annihilator");
    return 0;
}