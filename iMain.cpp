#include "iGraphics.h"
#include <iostream>
#include "iSound.h"
#include <stdlib.h>
#include "iFont.h"
using namespace std;

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 700
#define MAX_BULLETS 50

bool enem6_exploding = false;
int enem6_exp_idx = 0;
int enem6_x = 1250, enem6_y = 50;
int scorecolour = 0;
int scorecolourtimer = 0;
double meteorRotationAngle = 0.0;
double hprotangle = 0.0, rockrotangle = 0.0, shieldrotangle = 0.0;
int survival_score_timer = 0;
bool just_reset = false;
int just_reset_timer = 0;
int gocount = 0;
Image go1, go2, go3, go4, go5, go6, go7, go8, gameoverscreen, score1, score2, score3;
Image num1, num2, num3, num4, num5, num6, num7, num8, num9, num0;
int wrap;
Image paused;
bool shield_active = false;
int shield_hp = 3;
Image shieldimg[1];
Sprite shieldsprt;
int shieldx, shieldy;
int bonushpx = 700, bonusrocketx = 500, bonusshieldx = 1000;
int bonushpy = 1500, bonusrockety = -1800, bonusshieldy = 800;
bool bonusrocket = false, bonusshield = false, bonushp = false;
Image score;
int meteor_spawn_timer = 0;
int scorenumber = 0;
char scoretext[100];
int health = 3;
int invincibility_end_time = 0;
int homeidx = -1, mainidx = -1;
int sound_check = 0;
int fullscreen = 0;
int gamestate = 1;
Image home, diff, mainbg, hp1, hp2, hp3, hpbonus[1], rocket[1], shield[1];
Sprite bo_roc, bo_shi, bo_hp;
int homesound, mainsound;
Sprite met;
Sprite spaceship, enem1, enem2, enem3, enem4, enem5, enem6, bullet_sprites[MAX_BULLETS], ebulsprite[MAX_BULLETS], e2bulsprite[MAX_BULLETS], e3bulsprite[MAX_BULLETS], e4bulsprite[MAX_BULLETS], e5bulsprite[MAX_BULLETS], e6bulsprite[MAX_BULLETS];
Image s_boost[6], s_exp[7], s_shoot[4], idle[1], bullet_img[1], mete[1];
Image e1exp[10], e1idle[1];
Image e2exp[12], e2idle[1];
Image e3exp[11], e3idle[1];
Image e4exp[11], e4idle[1];
Image e5exp[11], e5idle[1];
Image e6exp[11], e6idle[1];
Image e1bul[1], e2bul[1], e3bul[1], e4bul[1], e5bul[1], e6bul[1];
int move_ud = 280, move_lf = 0;
int bullet_x[MAX_BULLETS], bullet_y[MAX_BULLETS], bullet_active[MAX_BULLETS];
int e2bullet_x[MAX_BULLETS], e2bullet_y[MAX_BULLETS], e6bullet_x[MAX_BULLETS], e6bullet_y[MAX_BULLETS];
int ebullet_x[MAX_BULLETS], ebullet_y[MAX_BULLETS], ebullet_active[MAX_BULLETS], e2bullet_active[MAX_BULLETS], e3bullet_active[MAX_BULLETS], e4bullet_active[MAX_BULLETS], e5bullet_active[MAX_BULLETS], e6bullet_active[MAX_BULLETS];
int enem1_fire_timer = 0;
int enem2_fire_timer = 0;
int enem6_fire_timer = 0;
enum { IDLE, BOOST, SHOOT, EXP };
int ship_state = IDLE;
int boost_idx = 0;
int shoot_idx = 0;
int exp_idx = 0;
int metx = 550, mety = 1000;
bool enem1_active = false;
int enem1_x = 1200, enem1_y = 500;
int bul1_x = enem1_x - 8;
bool enem1_bul_active = false;
int enem1_respawn_timer = 0;
bool enem1_exploding = false;
int enem1_exp_idx = 0;
bool enem2_active = false;
bool enem6_active = false;
int enem2_x = 1200, enem2_y = 100;
int enem2_respawn_timer = 0;
bool enem2_exploding = false;
int enem2_exp_idx = 0;
bool shipexp = false;
bool meteor = false;
bool key_w = false, key_a = false, key_s = false, key_d = false;
int enem1hp = 1, enem2hp = 4, enem3hp = 3, enem4hp = 4, enem5hp = 5, enem6hp = 50;
char des[7][100];
int a = 0;
int hpbonus_spawn_timer = 0;
int rocket_spawn_timer = 0;
int shield_spawn_timer = 0;
bool rocket_powerup_active = false;
int rocket_powerup_end_time = 0;
int rocket_powerup_start_time = 0;
bool game_paused = false;
int timer_id, animation_timer_id;

// New bounding box collision function
bool boundingBoxCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
}

void resetGame() {
    health = 3;
    move_ud = 280;
    move_lf = 0;
    ship_state = IDLE;
    shipexp = false;
    boost_idx = 0;
    shoot_idx = 0;
    exp_idx = 0;
    iSetSpritePosition(&spaceship, move_lf, move_ud);
    iChangeSpriteFrames(&spaceship, idle, 1);
    scorenumber = 0;
    sprintf(scoretext, "%d", scorenumber);
    enem1_active = false;
    enem1_x = 1200;
    enem1_y = 500;
    enem1_respawn_timer = 0;
    enem1_exploding = false;
    enem1_exp_idx = 0;
    enem1_fire_timer = 0;
    iSetSpritePosition(&enem1, enem1_x, enem1_y);
    iChangeSpriteFrames(&enem1, e1idle, 1);
    enem2_active = false;
    enem2_x = 1200;
    enem2_y = 100;
    enem6_x = 1250;
    enem6_y = 50;
    enem6_exploding = false;
    enem6_exp_idx = 0;
    enem6_active = false;
    enem2_respawn_timer = 0;
    enem2_exploding = false;
    enem2_exp_idx = 0;
    enem2_fire_timer = 0;
    enem2hp = 4;
    iSetSpritePosition(&enem2, enem2_x, enem2_y);
    iChangeSpriteFrames(&enem2, e2idle, 1);
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullet_active[i] = 0;
        ebullet_active[i] = 0;
        e2bullet_active[i] = 0;
        e3bullet_active[i] = 0;
        e4bullet_active[i] = 0;
        e5bullet_active[i] = 0;
        e6bullet_active[i] = 0;
    }
    meteor = false;
    metx = 550;
    mety = 1000;
    meteor_spawn_timer = 0;
    iSetSpritePosition(&met, metx, mety);
    bonushp = false;
    bonusrocket = false;
    bonusshield = false;
    bonushpx = 700;
    bonushpy = 1500;
    bonusrocketx = 500;
    bonusrockety = -1800;
    bonusshieldx = 1000;
    bonusshieldy = 800;
    hpbonus_spawn_timer = 0;
    rocket_spawn_timer = 0;
    shield_spawn_timer = 0;
    rocket_powerup_active = false;
    rocket_powerup_end_time = 0;
    rocket_powerup_start_time = 0;
    iSetSpritePosition(&bo_hp, bonushpx, bonushpy);
    iSetSpritePosition(&bo_roc, bonusrocketx, bonusrockety);
    iSetSpritePosition(&bo_shi, bonusshieldx, bonusshieldy);
    shield_active = false;
    shield_hp = 3;
    iSetSpritePosition(&shieldsprt, -100, -100);
    meteorRotationAngle = 0.0;
    hprotangle = 0.0;
    rockrotangle = 0.0;
    shieldrotangle = 0.0;
    invincibility_end_time = 0;
    just_reset = true;
    just_reset_timer = 2000;
    survival_score_timer = 0;
    iResumeTimer(timer_id);
    iResumeTimer(animation_timer_id);
}

void loadresources() {
    iMirrorSprite(&enem1, HORIZONTAL);
    iMirrorSprite(&enem2, HORIZONTAL);
    iMirrorSprite(&enem6, HORIZONTAL);
    iMirrorSprite(&enem3, HORIZONTAL);
    iMirrorSprite(&enem4, HORIZONTAL);
    iMirrorSprite(&enem5, HORIZONTAL);
    iLoadImage(&score1, "assets/images/gameover/score1.png");
    iLoadImage(&score2, "assets/images/gameover/score2.png");
    iLoadImage(&score3, "assets/images/gameover/score3.png");
    iScaleImage(&score1, 0.17);
    iScaleImage(&score2, 0.17);
    iScaleImage(&score3, 0.17);
    iLoadImage(&gameoverscreen, "assets/images/gotemp.png");
    iLoadImage(&go1, "assets/images/gameover/1.png");
    iLoadImage(&go2, "assets/images/gameover/2.png");
    iLoadImage(&go3, "assets/images/gameover/3.png");
    iLoadImage(&go4, "assets/images/gameover/4.png");
    iLoadImage(&go5, "assets/images/gameover/5.png");
    iLoadImage(&go6, "assets/images/gameover/6.png");
    iLoadImage(&go7, "assets/images/gameover/7.png");
    iLoadImage(&go8, "assets/images/gameover/8.png");
    iScaleImage(&go1, 0.3);
    iScaleImage(&go2, 0.3);
    iScaleImage(&go3, 0.3);
    iScaleImage(&go4, 0.3);
    iScaleImage(&go5, 0.3);
    iScaleImage(&go6, 0.3);
    iScaleImage(&go7, 0.3);
    iScaleImage(&go8, 0.3);
    iLoadImage(&num0, "assets/images/numbers/0/1000009435.png");
    iLoadImage(&num1, "assets/images/numbers/1/1000009434.png");
    iLoadImage(&num2, "assets/images/numbers/2/1000009433.png");
    iLoadImage(&num3, "assets/images/numbers/3/1000009432.png");
    iLoadImage(&num4, "assets/images/numbers/4/1000009431.png");
    iLoadImage(&num5, "assets/images/numbers/5/1000009430.png");
    iLoadImage(&num6, "assets/images/numbers/6/1000009429.png");
    iLoadImage(&num7, "assets/images/numbers/7/1000009428.png");
    iLoadImage(&num8, "assets/images/numbers/8/1000009427.png");
    iLoadImage(&num9, "assets/images/numbers/9/1000009426.png");
    iScaleImage(&num0, 0.04);
    iScaleImage(&num1, 0.04);
    iScaleImage(&num2, 0.04);
    iScaleImage(&num3, 0.04);
    iScaleImage(&num4, 0.04);
    iScaleImage(&num5, 0.04);
    iScaleImage(&num6, 0.04);
    iScaleImage(&num7, 0.04);
    iScaleImage(&num8, 0.04);
    iScaleImage(&num9, 0.04);
    sprintf(scoretext, "%d", scorenumber);
    iInitSprite(&shieldsprt);
    iLoadFramesFromFolder(shieldimg, "assets/images/main_shield/");
    iChangeSpriteFrames(&shieldsprt, shieldimg, 1);
    iScaleSprite(&shieldsprt, 0.2);
    iLoadImage(&home, "assets/images/homepage_w_menu.png");
    iLoadImage(&diff, "assets/images/difficulty.png");
    iLoadImage(&mainbg, "assets/images/mainbg.png");
    iInitSprite(&met);
    iLoadFramesFromFolder(mete, "assets/images/sprites/Meteors/mainmet/");
    iChangeSpriteFrames(&met, mete, 1);
    iSetSpritePosition(&met, metx, mety);
    iScaleSprite(&met, .3);
    iLoadImage(&idle[0], "assets/images/sprites/Spaceship/Idle.png");
    iLoadFramesFromFolder(s_boost, "assets/images/sprites/Spaceship/boost/");
    iLoadFramesFromFolder(s_shoot, "assets/images/sprites/Spaceship/shoot/");
    iLoadFramesFromFolder(s_exp, "assets/images/sprites/Spaceship/explosion/");
    iLoadImage(&e1idle[0], "assets/images/sprites/enemy/Ship1/Ship1.png");
    iLoadFramesFromFolder(e1exp, "assets/images/sprites/enemy/explosions/Ship1_Explosion/");
    iLoadImage(&e2idle[0], "assets/images/sprites/enemy/Ship2/Ship2.png");
    iLoadFramesFromFolder(e2exp, "assets/images/sprites/enemy/explosions/Ship2_Explosion/");
    iLoadImage(&e6idle[0], "assets/images/sprites/enemy/Ship6/Ship6.png");
    iLoadFramesFromFolder(e6exp, "assets/images/sprites/enemy/explosions/Ship6_Explosion/");
    iInitSprite(&spaceship);
    iChangeSpriteFrames(&spaceship, idle, 1);
    iSetSpritePosition(&spaceship, move_lf, move_ud);
    iLoadImage(&bullet_img[0], "assets/images/sprites/Spaceship/Charge_2.png");
    for (int i = 0; i < MAX_BULLETS; i++) {
        iInitSprite(&bullet_sprites[i]);
        iChangeSpriteFrames(&bullet_sprites[i], bullet_img, 1);
        bullet_active[i] = 0;
    }
    iLoadImage(&e1bul[0], "assets/images/sprites/enemy/Shots/Shot1/enembullet.png");
    for (int i = 0; i < MAX_BULLETS; i++) {
        iInitSprite(&ebulsprite[i]);
        iScaleSprite(&ebulsprite[i], 1.7);
        iChangeSpriteFrames(&ebulsprite[i], e1bul, 1);
        ebullet_active[i] = 0;
    }
    iLoadImage(&e2bul[0], "assets/images/sprites/enemy/Shots/Shot2/shot2_asset.png");
    for (int i = 0; i < MAX_BULLETS; i++) {
        iInitSprite(&e2bulsprite[i]);
        iScaleSprite(&e2bulsprite[i], 1.3);
        iChangeSpriteFrames(&e2bulsprite[i], e2bul, 1);
        e2bullet_active[i] = 0;
    }
    iLoadImage(&e3bul[0], "assets/images/sprites/enemy/Shots/Shot3/shot3_asset.png");
    for (int i = 0; i < MAX_BULLETS; i++) {
        iInitSprite(&e3bulsprite[i]);
        iScaleSprite(&e3bulsprite[i], 1.7);
        iChangeSpriteFrames(&e3bulsprite[i], e3bul, 1);
        e3bullet_active[i] = 0;
    }
    iLoadImage(&e4bul[0], "assets/images/sprites/enemy/Shots/Shot4/shot4_3.png");
    for (int i = 0; i < MAX_BULLETS; i++) {
        iInitSprite(&e4bulsprite[i]);
        iScaleSprite(&e4bulsprite[i], 1.7);
        iChangeSpriteFrames(&e4bulsprite[i], e4bul, 1);
        e4bullet_active[i] = 0;
    }
    iLoadImage(&e5bul[0], "assets/images/sprites/enemy/Shots/Shot5/shot5_4.png");
    for (int i = 0; i < MAX_BULLETS; i++) {
        iInitSprite(&e5bulsprite[i]);
        iScaleSprite(&e5bulsprite[i], 1.7);
        iChangeSpriteFrames(&e5bulsprite[i], e5bul, 1);
        e5bullet_active[i] = 0;
    }
    iLoadImage(&e6bul[0], "assets/images/sprites/enemy/Shots/Shot6/mainbull/shot6_2.png");
    for (int i = 0; i < MAX_BULLETS; i++) {
        iInitSprite(&e6bulsprite[i]);
        iScaleSprite(&e6bulsprite[i], 4);
        iMirrorSprite(&e6bulsprite[i], HORIZONTAL);
        iChangeSpriteFrames(&e6bulsprite[i], e6bul, 1);
        e6bullet_active[i] = 0;
    }
    iLoadImage(&hp1, "assets/images/hp/full.png");
    iLoadImage(&hp2, "assets/images/hp/medium.png");
    iLoadImage(&hp3, "assets/images/hp/low.png");
    iScaleImage(&hp1, 0.2);
    iScaleImage(&hp2, 0.2);
    iScaleImage(&hp3, 0.2);
    iLoadImage(&score, "assets/images/score.png");
    iScaleImage(&score, 0.085);
    iInitSprite(&bo_roc);
    iLoadFramesFromFolder(rocket, "assets/images/rocket/");
    iScaleSprite(&bo_roc, .28);
    iChangeSpriteFrames(&bo_roc, rocket, 1);
    iSetSpritePosition(&bo_roc, bonusrocketx, bonusrockety);
    iInitSprite(&bo_shi);
    iLoadFramesFromFolder(shield, "assets/images/shield/");
    iScaleSprite(&bo_shi, .28);
    iChangeSpriteFrames(&bo_shi, shield, 1);
    iSetSpritePosition(&bo_shi, bonusshieldx, bonusshieldy);
    iInitSprite(&bo_hp);
    iLoadFramesFromFolder(hpbonus, "assets/images/health/");
    iScaleSprite(&bo_hp, .28);
    iChangeSpriteFrames(&bo_hp, hpbonus, 1);
    iSetSpritePosition(&bo_hp, bonushpx, bonushpy);
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
    iScaleSprite(&enem6, 4);
    iChangeSpriteFrames(&enem6, e6idle, 1);
    iSetSpritePosition(&enem6, enem6_x, enem6_y);
    iLoadImage(&paused, "assets/images/paused.png");
    iResizeImage(&paused, 400, 400);
}

int getNonOverlappingXPosition(int existing_x1, int existing_x2, int min_distance) {
    int new_x;
    do {
        new_x = rand() % (SCREEN_WIDTH - 50);
    } while ((existing_x1 != -1 && abs(new_x - existing_x1) < min_distance) ||
             (existing_x2 != -1 && abs(new_x - existing_x2) < min_distance));
    return new_x;
}

void updateAnimation() {
    if (game_paused) return;
    if (ship_state == BOOST) {
        boost_idx = (boost_idx + 1) % 6;
        iChangeSpriteFrames(&spaceship, s_boost, 6);
    }
    if (ship_state == SHOOT) {
        shoot_idx = (shoot_idx + 1) % 4;
        iChangeSpriteFrames(&spaceship, s_shoot, 4);
    }
    if (ship_state == EXP && shipexp) {
        exp_idx++;
        if (exp_idx < 7) {
            iChangeSpriteFrames(&spaceship, s_exp, 7);
            spaceship.currentFrame = exp_idx;
        } else {
            shipexp = false;
            ship_state = IDLE;
            gamestate = 211;
            iChangeSpriteFrames(&spaceship, idle, 1);
            exp_idx = 0;
        }
    }
}

void moveSpaceship() {
    if (game_paused || ship_state == EXP) return;
    if (gamestate == 21) {
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
        if (shield_active) {
            shieldx = move_lf - 5;
            shieldy = move_ud - 5;
            iSetSpritePosition(&shieldsprt, shieldx, shieldy);
        }
    }
}

void updatemeteor() {
    if (game_paused) return;
    if (gamestate == 21) {
        meteor_spawn_timer += 100; // Adjusted to 100ms
        if (!meteor && meteor_spawn_timer >= 5000) {
            meteor = true;
            metx = 100 + (rand() % (SCREEN_WIDTH - 50));
            mety = SCREEN_HEIGHT + 50

;
            iSetSpritePosition(&met, metx, mety);
            meteor_spawn_timer = 0;
        }
        if (meteor) {
            metx -= 15;
            mety -= 15;
            iSetSpritePosition(&met, metx, mety);
            double rotationCenterX = metx + 45.0;
            double rotationCenterY = mety + 39.3;
            meteorRotationAngle += 10.0;
            if (meteorRotationAngle >= 360.0) {
                meteorRotationAngle = 0.0;
            }
            iRotateSprite(&met, rotationCenterX, rotationCenterY, meteorRotationAngle);
            if (metx < -50 || mety < -70) {
                meteor = false;
                meteor_spawn_timer = 0;
            }
        }
    }
}

void updateBonuses() {
    if (game_paused) return;
    if (gamestate == 21) {
        hpbonus_spawn_timer += 100; // Adjusted to 100ms
        if (!bonushp && hpbonus_spawn_timer >= 5000) {
            bonushp = true;
            bonushpx = getNonOverlappingXPosition(bonusrocketx * bonusrocket, bonusshieldx * bonusshield, 100);
            bonushpy = SCREEN_HEIGHT + 50;
            iSetSpritePosition(&bo_hp, bonushpx, bonushpy);
            hpbonus_spawn_timer = 0;
        }
        if (bonushp) {
            bonushpy -= 10;
            iSetSpritePosition(&bo_hp, bonushpx, bonushpy);
            double hprotcenx = bonushpx + 42;
            double hprotceny = bonushpy + 42;
            hprotangle += 10;
            if (hprotangle >= 360) {
                hprotangle = 0.0;
            }
            iRotateSprite(&bo_hp, hprotcenx, hprotceny, hprotangle);
            if (bonushpy < -50) {
                bonushp = false;
                hpbonus_spawn_timer = 0;
            }
        }

        rocket_spawn_timer += 100; // Adjusted to 100ms
        if (!bonusrocket && rocket_spawn_timer >= 12000) {
            bonusrocket = true;
            bonusrocketx = getNonOverlappingXPosition(bonushpx * bonushp, bonusshieldx * bonusshield, 100);
            bonusrockety = SCREEN_HEIGHT + 50;
            iSetSpritePosition(&bo_roc, bonusrocketx, bonusrockety);
            rocket_spawn_timer = 0;
        }
        if (bonusrocket) {
            bonusrockety -= 10;
            iSetSpritePosition(&bo_roc, bonusrocketx, bonusrockety);
            double rocrotcenx = bonusrocketx + 42;
            double rocrotceny = bonusrockety + 42;
            rockrotangle += 10;
            if (rockrotangle >= 360) {
                rockrotangle = 0.0;
            }
            iRotateSprite(&bo_roc, rocrotcenx, rocrotceny, rockrotangle);
            if (bonusrockety < -50) {
                bonusrocket = false;
                rocket_spawn_timer = 0;
            }
        }

        shield_spawn_timer += 100; // Adjusted to 100ms
        if (!bonusshield && shield_spawn_timer >= 17000) {
            bonusshield = true;
            bonusshieldx = getNonOverlappingXPosition(bonushpx * bonushp, bonusrocketx * bonusrocket, 100);
            bonusshieldy = SCREEN_HEIGHT + 50;
            iSetSpritePosition(&bo_shi, bonusshieldx, bonusshieldy);
            shield_spawn_timer = 0;
        }
        if (bonusshield) {
            bonusshieldy -= 10;
            iSetSpritePosition(&bo_shi, bonusshieldx, bonusshieldy);
            double shirotcenx = bonusshieldx + 42;
            double shirotceny = bonusshieldy + 42;
            shieldrotangle += 10;
            if (shieldrotangle >= 360) {
                shieldrotangle = 0.0;
            }
            iRotateSprite(&bo_shi, shirotcenx, shirotceny, shieldrotangle);
            if (bonusshieldy < -50) {
                bonusshield = false;
                shield_spawn_timer = 0;
            }
        }

        if (rocket_powerup_active && glutGet(GLUT_ELAPSED_TIME) > rocket_powerup_end_time) {
            rocket_powerup_active = false;
        }
    }
}

void enem_shoot() {
    if (game_paused) return;
    if (gamestate == 21) {
        if (enem1_active && !enem1_exploding) {
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (!ebullet_active[i]) {
                    ebullet_x[i] = enem1_x - 10;
                    ebullet_y[i] = enem1_y + 40;
                    iSetSpritePosition(&ebulsprite[i], ebullet_x[i], ebullet_y[i]);
                    ebullet_active[i] = 1;
                    break;
                }
            }
        }
        if (enem2_active && !enem2_exploding) {
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (!e2bullet_active[i]) {
                    e2bullet_x[i] = enem2_x - 10;
                    e2bullet_y[i] = enem2_y + 70;
                    iSetSpritePosition(&e2bulsprite[i], e2bullet_x[i], e2bullet_y[i]);
                    e2bullet_active[i] = 1;
                    break;
                }
            }
        }
        if (enem6_active && !enem6_exploding) {
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (!e6bullet_active[i]) {
                    e6bullet_x[i] = enem6_x - 200;
                    e6bullet_y[i] = enem6_y ;
                    iSetSpritePosition(&e6bulsprite[i], e6bullet_x[i], e6bullet_y[i]);
                    e6bullet_active[i] = 1;
                    break;
                }
            }
        }
    }
}

void updateEnemy() {
    if (game_paused) return;
    if (gamestate == 21) {
        if (!enem1_active && !enem1_exploding && !enem6_active) {
            enem1_respawn_timer += 100; // Adjusted to 100ms
            if (enem1_respawn_timer >= 1000) {
                enem1_active = true;
                enem1_x = 1200;
                enem1_y = rand() % SCREEN_HEIGHT;
                if (enem1_y > SCREEN_HEIGHT - 100) enem1_y = SCREEN_HEIGHT - 250;
                else if (enem1_y < 50) enem1_y = 50;
                iSetSpritePosition(&enem1, enem1_x, enem1_y);
                iChangeSpriteFrames(&enem1, e1idle, 1);
                enem1_respawn_timer = 0;
                enem1_fire_timer = 0;
            }
        }
          if (!enem1_active && !enem1_exploding && enem6_active) {
            enem1_respawn_timer += 100; // Adjusted to 100ms
            if (enem1_respawn_timer >= 1000) {
                enem1_active = true;
                enem1_x = 1200;
                enem1_y =600;
                iSetSpritePosition(&enem1, enem1_x, enem1_y);
                iChangeSpriteFrames(&enem1, e1idle, 1);
                enem1_respawn_timer = 0;
                enem1_fire_timer = 0;
            }
        }
        if (enem1_active && !enem1_exploding ) {
            enem1_x -= 10;
           
            iSetSpritePosition(&enem1, enem1_x, enem1_y);
            if (enem1_x < -100) {
                enem1_active = false;
            }
            enem1_fire_timer += 50; // Adjusted to 100ms
            if (enem1_fire_timer >= 1000) {
                enem_shoot();
                enem1_fire_timer = 0;
            }
        }
        if (!enem2_active && !enem2_exploding && !enem6_active) {
            enem2_respawn_timer += 100; // Adjusted to 100ms
            if (enem2_respawn_timer >= 1000) {
                enem2_active = true;
                enem2hp = 4;
                enem2_x = 1200;
                if (enem1_y > 350) enem2_y = enem1_y - 400;
                else enem2_y = enem1_y + 300;
                if (enem2_y > SCREEN_HEIGHT - 100) enem2_y = SCREEN_HEIGHT - 250;
                iSetSpritePosition(&enem2, enem2_x, enem2_y);
                iChangeSpriteFrames(&enem2, e2idle, 1);
                enem2_respawn_timer = 0;
                enem2_fire_timer = 0;
            }
        }
         if (!enem2_active && !enem2_exploding && enem6_active) {
            enem2_respawn_timer += 100; // Adjusted to 100ms
            if (enem2_respawn_timer >= 1000) {
                enem2_active = true;
                enem2hp = 4;
                enem2_x = 1200;
               enem2_y =30;
                iSetSpritePosition(&enem2, enem2_x, enem2_y);
                iChangeSpriteFrames(&enem2, e2idle, 1);
                enem2_respawn_timer = 0;
                enem2_fire_timer = 0;
            }
        }
        if (enem2_active && !enem2_exploding) {
            enem2_x -= 10;
            
            iSetSpritePosition(&enem2, enem2_x, enem2_y);
            if (enem2_x < -100) {
                enem2_active = false;
            }
            enem2_fire_timer += 50; // Adjusted to 100ms
            if (enem2_fire_timer >= 1000) {
                enem_shoot();
                enem2_fire_timer = 0;
            }
        }
        if (scorenumber >= 500) {
            enem6_active = true;
        }
        if (enem6_active && !enem6_exploding) {
            enem6_x -= 10;
            iSetSpritePosition(&enem6, enem6_x, enem6_y);
            if (enem6_x < 800) {
                enem6_x = 800;
            }
            enem6_fire_timer += 50; // Adjusted to 100ms
            if (enem6_fire_timer >= 1000) {
                enem_shoot();
                enem6_fire_timer = 0;
            }
        }
    }
}

void updateEnemyExplosion() {
    if (game_paused) return;
    if (enem1_exploding) {
        enem1_exp_idx++;
        if (enem1_exp_idx < 10) {
            iChangeSpriteFrames(&enem1, e1exp, 10);
            enem1.currentFrame = enem1_exp_idx;
            iSetSpritePosition(&enem1, enem1_x, enem1_y);
        } else {
            enem1_exploding = false;
            enem1_active = false;
            enem1_exp_idx = 0;
            iChangeSpriteFrames(&enem1, e1idle, 1);
            enem1_x = 1200;
            enem1_y = rand() % SCREEN_HEIGHT;
            if (enem1_y > SCREEN_HEIGHT - 100) enem1_y = SCREEN_HEIGHT - 250;
            else if (enem1_y < 50) enem1_y = 50;
            iSetSpritePosition(&enem1, enem1_x, enem1_y);
        }
    }
    if (enem2_exploding) {
        enem2_exp_idx++;
        if (enem2_exp_idx < 12) {
            iChangeSpriteFrames(&enem2, e2exp, 12);
            enem2.currentFrame = enem2_exp_idx;
            iSetSpritePosition(&enem2, enem2_x, enem2_y);
        } else {
            enem2_exploding = false;
            enem2_active = false;
            enem2_exp_idx = 0;
            enem2hp = 4;
            iChangeSpriteFrames(&enem2, e2idle, 1);
            enem2_x = 1200;
            if (enem1_y > 350) enem2_y = enem1_y - 400;
            else enem2_y = enem1_y + 300;
            if (enem2_y > SCREEN_HEIGHT - 100) enem2_y = SCREEN_HEIGHT - 250;
            iSetSpritePosition(&enem2, enem2_x, enem2_y);
        }
    }
    if (enem6_exploding) {
        enem6_exp_idx++;
        if (enem6_exp_idx < 12) {
            iChangeSpriteFrames(&enem6, e6exp, 11);
            enem6.currentFrame = enem6_exp_idx;
            iSetSpritePosition(&enem6, enem6_x, enem6_y);
        }
        if(enem6_exp_idx >= 11) {
            enem6_exploding = false;
            enem6_active = false;
            gamestate = 211;
    }
}
}
void checkshieldcollision() {
    if (game_paused) return;
    if (shield_active) {
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (ebullet_active[i]) {
                if (iCheckCollision(&shieldsprt, &ebulsprite[i])) {
                    ebullet_active[i] = 0;
                    shield_hp--;
                    if (shield_hp <= 0) {
                        shield_active = false;
                        shield_hp = 3;
                        iSetSpritePosition(&shieldsprt, -100, -100);
                    }
                }
            }
        }
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (e2bullet_active[i]) {
                if (iCheckCollision(&shieldsprt, &e2bulsprite[i])) {
                    e2bullet_active[i] = 0;
                    shield_hp--;
                    if (shield_hp <= 0) {
                        shield_active = false;
                        shield_hp = 3;
                        iSetSpritePosition(&shieldsprt, -100, -100);
                    }
                }
            }
        }
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (e6bullet_active[i]) {
                if (iCheckCollision(&shieldsprt, &e6bulsprite[i])) {
                    e6bullet_active[i] = 0;
                    shield_hp-=3;
                    if (shield_hp <= 0) {
                        shield_active = false;
                        shield_hp = 3;
                        iSetSpritePosition(&shieldsprt, -100, -100);
                    }
                }
            }
        }
        if (iCheckCollision(&shieldsprt, &met)) {
            meteor = false;
            iSetSpritePosition(&met, -100, -100);
            meteor_spawn_timer = 0;
            shield_hp -= 3;
            if (shield_hp <= 0) {
                shield_active = false;
                shield_hp = 3;
                iSetSpritePosition(&shieldsprt, -100, -100);
            }
        }
        if (iCheckCollision(&shieldsprt, &enem1)) {
            enem1_active = false;
            enem1_exploding = true;
            enem1_exp_idx = 0;
            iChangeSpriteFrames(&enem1, e1exp, 10);
            shield_hp -= 3;
            if (shield_hp <= 0) {
                shield_active = false;
                shield_hp = 3;
                iSetSpritePosition(&shieldsprt, -100, -100);
            }
        }
          if (iCheckCollision(&shieldsprt, &enem2)) {
            enem2_active = false;
            enem2_exploding = true;
            enem2_exp_idx = 0;
            iChangeSpriteFrames(&enem2, e2exp, 12);
            shield_hp -= 3;
            if (shield_hp <= 0) {
                shield_active = false;
                shield_hp = 3;
                iSetSpritePosition(&shieldsprt, -100, -100);
            }
        }
        if (iCheckCollision(&shieldsprt, &enem6)) {
            shield_hp -= 3;
            if (shield_hp <= 0) {
                shield_active = false;
                shield_hp = 3;
                iSetSpritePosition(&shieldsprt, -100, -100);
            }
        }
    }
}

void  checkEnemSpaceCollision() {
    if (just_reset) return;
    if (game_paused) return;
    if (gamestate == 21) {
        if (iCheckCollision(&spaceship, &enem1) && !shield_active && !shipexp) {
            enem1_active = false;
            enem1_exploding = true;
            enem1_exp_idx = 0;
            iChangeSpriteFrames(&enem1, e1exp, 10);
            shipexp = true;
            ship_state = EXP;
            exp_idx = 0;
            iChangeSpriteFrames(&spaceship, s_exp, 7);
            spaceship.currentFrame = 0;
        }
        if (iCheckCollision(&spaceship, &enem2) && !shield_active && !shipexp) {
            enem2_active = false;
            enem2_exploding = true;
            enem2_exp_idx = 0;
            iChangeSpriteFrames(&enem2, e2exp, 12);
            shipexp = true;
            ship_state = EXP;
            exp_idx = 0;
            iChangeSpriteFrames(&spaceship, s_exp, 7);
            spaceship.currentFrame = 0;
        }
        if (meteor && iCheckCollision(&spaceship, &met) && !shield_active && !shipexp) {
            meteor = false;
            meteor_spawn_timer = 0;
            metx = 550;
            mety = 1000;
            iSetSpritePosition(&met, metx, mety);
            shipexp = true;
            ship_state = EXP;
            exp_idx = 0;
            iChangeSpriteFrames(&spaceship, s_exp, 7);
            spaceship.currentFrame = 0;
        }
        if (iCheckCollision(&spaceship, &enem6) && !shield_active && !shipexp) {
            shipexp = true;
            ship_state = EXP;
            exp_idx = 0;
            iChangeSpriteFrames(&spaceship, s_exp, 7);
            spaceship.currentFrame = 0;
        }
        if (bonushp && iCheckCollision(&spaceship, &bo_hp)) {
            if (health < 3) {
                health++;
            }
            bonushp = false;
            hpbonus_spawn_timer = 0;
        }
        if (bonusrocket && iCheckCollision(&spaceship, &bo_roc)) {
            rocket_powerup_active = true;
            rocket_powerup_start_time = glutGet(GLUT_ELAPSED_TIME);
            rocket_powerup_end_time = rocket_powerup_start_time + 10000;
            bonusrocket = false;
            rocket_spawn_timer = 0;
        }
        if (bonusshield && iCheckCollision(&spaceship, &bo_shi)) {
            shield_active = true;
            shieldx = move_lf - 5;
            shieldy = move_ud - 5;
            iSetSpritePosition(&shieldsprt, shieldx, shieldy);
            bonusshield = false;
            shield_spawn_timer = 0;
        }
    }
}

void checkBulletEnemyCollision() {
    if (just_reset) return;
    if (game_paused) return;
    if (gamestate == 21) {
        if (enem1_active && !enem1_exploding) {
            int enem1_w = 100; // Adjust based on sprite size
            int enem1_h = 100;
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bullet_active[i]) {
                    int bullet_w = 10; // Adjust based on sprite size
                    int bullet_h = 10;
                    if (boundingBoxCollision(bullet_x[i], bullet_y[i], bullet_w, bullet_h, enem1_x, enem1_y, enem1_w, enem1_h)) {
                        if (iCheckCollision(&bullet_sprites[i], &enem1)) {
                            bullet_active[i] = 0;
                            enem1_active = false;
                            enem1_exploding = true;
                            scorenumber += 100;
                            sprintf(scoretext, "%d", scorenumber);
                            enem1_exp_idx = 0;
                            iChangeSpriteFrames(&enem1, e1exp, 10);
                            iSetSpritePosition(&enem1, enem1_x, enem1_y);
                        }
                    }
                }
            }
        }
        if (enem2_active && !enem2_exploding) {
            int enem2_w = 170; // Adjusted for scale 1.7
            int enem2_h = 170;
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bullet_active[i]) {
                    //int bullet_w = 10;
                    //int bullet_h = 10;
                    //if (boundingBoxCollision(bullet_x[i], bullet_y[i], bullet_w, bullet_h, enem2_x, enem2_y, enem2_w, enem2_h)) {
                        if (iCheckCollision(&bullet_sprites[i], &enem2)) {
                            bullet_active[i] = 0;
                            enem2hp--;
                            if (enem2hp <= 0) {
                                enem2_active = false;
                                enem2_exploding = true;
                                scorenumber += 100;
                                sprintf(scoretext, "%d", scorenumber);
                                enem2_exp_idx = 0;
                                iChangeSpriteFrames(&enem2, e2exp, 12);
                                iSetSpritePosition(&enem2, enem2_x, enem2_y);
                            }
                        }
                    //}
                }
            }
        }
        if (meteor) {
            int met_w = 90; // Adjusted for scale 0.3
            int met_h = 90;
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bullet_active[i]) {
                    int bullet_w = 10;
                    int bullet_h = 10;
                    if (boundingBoxCollision(bullet_x[i], bullet_y[i], bullet_w, bullet_h, metx, mety, met_w, met_h)) {
                        if (iCheckCollision(&bullet_sprites[i], &met)) {
                            bullet_active[i] = 0;
                            bullet_x[i] = -100;
                            bullet_y[i] = -100;
                            iSetSpritePosition(&bullet_sprites[i], -100, -100);
                            meteor = false;
                            meteor_spawn_timer = 0;
                            metx = 550;
                            mety = 1000;
                            iSetSpritePosition(&met, metx, mety);
                            scorenumber += 50;
                            sprintf(scoretext, "%d", scorenumber);
                        }
                    }
                }
            }
        }
        if (enem6_active && !enem6_exploding) {
            int enem6_w = 220; // Adjusted for scale 2.2
            int enem6_h = 220;
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bullet_active[i]) {
                    int bullet_w = 10;
                    int bullet_h = 10;
                    if (boundingBoxCollision(bullet_x[i], bullet_y[i], bullet_w, bullet_h, enem6_x, enem6_y, enem6_w, enem6_h)) {
                        if (iCheckCollision(&bullet_sprites[i], &enem6)) {
                            bullet_active[i] = 0;
                            enem6hp--;
                            if (enem6hp <= 0) {
                                enem6_active = false;
                                enem6_exploding = true;
                                scorenumber += 1000;
                                sprintf(scoretext, "%d", scorenumber);
                                enem6_exp_idx = 0;
                                iChangeSpriteFrames(&enem6, e6exp, 11);
                                iSetSpritePosition(&enem6, enem6_x, enem6_y);
                            }
                        }
                    }
                }
            }
        }
    }
}

void enemBulletCollision() {
    if (just_reset) return;
    if (game_paused) return;
    if (gamestate == 21) {
        int current_time = glutGet(GLUT_ELAPSED_TIME);
        if (current_time < invincibility_end_time) {
            return;
        }
        bool hit_this_frame = false;
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (ebullet_active[i] && iCheckCollision(&spaceship, &ebulsprite[i]) && !shipexp) {
                ebullet_active[i] = 0;
                ebullet_x[i] = -100;
                ebullet_y[i] = -100;
                iSetSpritePosition(&ebulsprite[i], -100, -100);
                if (!shield_active) {
                    health--;
                } else {
                    shield_hp--;
                    if (shield_hp <= 0) {
                        shield_active = false;
                        shield_hp = 3;
                        iSetSpritePosition(&shieldsprt, -100, -100);
                    }
                }
                hit_this_frame = true;
            }
        }
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (e2bullet_active[i] && iCheckCollision(&spaceship, &e2bulsprite[i]) && !shipexp) {
                e2bullet_active[i] = 0;
                e2bullet_x[i] = -100;
                e2bullet_y[i] = -100;
                iSetSpritePosition(&e2bulsprite[i], -100, -100);
                if (!shield_active) {
                    health--;
                } else {
                    shield_hp--;
                    if (shield_hp <= 0) {
                        shield_active = false;
                        shield_hp = 3;
                        iSetSpritePosition(&shieldsprt, -100, -100);
                    }
                }
                hit_this_frame = true;
            }
        }
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (e6bullet_active[i] && iCheckCollision(&spaceship, &e6bulsprite[i]) && !shipexp) {
                e6bullet_active[i] = 0;
                e6bullet_x[i] = -100;
                e6bullet_y[i] = -100;
                iSetSpritePosition(&e6bulsprite[i], -100, -100);
                if (!shield_active) {
                    health--;
                } else {
                    shield_hp -= 3;
                    if (shield_hp <= 0) {
                        shield_active = false;
                        shield_hp = 3;
                        iSetSpritePosition(&shieldsprt, -100, -100);
                    }
                }
                hit_this_frame = true;
            }
        }
        if (hit_this_frame) {
            invincibility_end_time = current_time + 500;
            if (health <= 0) {
                shipexp = true;
                ship_state = EXP;
                exp_idx = 0;
                iChangeSpriteFrames(&spaceship, s_exp, 7);
                spaceship.currentFrame = 0;
            }
        }
    }
}

void homepage() {
    iShowLoadedImage(0, 0, &home);
    scorenumber = 0;
    sprintf(scoretext, "%d", scorenumber);
}

void difficulty() {
    iShowLoadedImage(0, 0, &diff);
}

void sound_manage() {
    if (gamestate == 1) {
        homeidx = iPlaySound("assets/sounds/menubg.wav", true, 70);
    } else if (gamestate == 21) {
        iPauseSound(homeidx);
        mainidx = iPlaySound("assets/sounds/mainbg.wav", true, 70);
    }
}

void mainpage1() {
    iShowLoadedImage(0, 0, &mainbg);
    wrap = game_paused ? 0 : -2;
    iWrapImage(&mainbg, wrap);
    if (shield_active) {
        iShowSprite(&shieldsprt);
    }
    iShowSprite(&spaceship);
    iShowLoadedImage2(1060, 615, &score);
    if (enem1_active || enem1_exploding) iShowSprite(&enem1);
    if (enem2_active || enem2_exploding) iShowSprite(&enem2);
    if (enem6_active || enem6_exploding) iShowSprite(&enem6);
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
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (e2bullet_active[i]) {
            iShowSprite(&e2bulsprite[i]);
        }
    }
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (e6bullet_active[i]) { // Removed enem6_x <= 400 condition
            iShowSprite(&e6bulsprite[i]);
        }
    }
    iText(1150, 650, scoretext, GLUT_BITMAP_TIMES_ROMAN_24);
    if (meteor) iShowSprite(&met);
    if (bonushp) iShowSprite(&bo_hp);
    checkBulletEnemyCollision();
    checkEnemSpaceCollision();
    enemBulletCollision();
    checkshieldcollision();
    iShowSpeed(1110, 20);
    if (health == 3) {
        iShowLoadedImage(-10, 550, &hp1);
    } else if (health == 2) {
        iShowLoadedImage(-10, 550, &hp2);
    } else if (health == 1) {
        iShowLoadedImage(-10, 550, &hp3);
    }
    if (bonusrocket) iShowSprite(&bo_roc);
    if (bonusshield) iShowSprite(&bo_shi);
    if (rocket_powerup_active) {
        int current_time = glutGet(GLUT_ELAPSED_TIME);
        int elapsed = current_time - rocket_powerup_start_time;
        int display_number = 9 - (elapsed / 1000);
        if (display_number >= 0 && display_number <= 9) {
            if (display_number == 9) iShowLoadedImage(180, 630, &num9);
            else if (display_number == 8) iShowLoadedImage(180, 630, &num8);
            else if (display_number == 7) iShowLoadedImage(180, 630, &num7);
            else if (display_number == 6) iShowLoadedImage(180, 630, &num6);
            else if (display_number == 5) iShowLoadedImage(180, 630, &num5);
            else if (display_number == 4) iShowLoadedImage(180, 630, &num4);
            else if (display_number == 3) iShowLoadedImage(180, 630, &num3);
            else if (display_number == 2) iShowLoadedImage(180, 630, &num2);
            else if (display_number == 1) iShowLoadedImage(180, 630, &num1);
            else if (display_number == 0) iShowLoadedImage(180, 630, &num0);
        }
    }
    just_reset = false;
}

void moveBullets() {
    if (game_paused) return;
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullet_active[i]) {
            bullet_x[i] += 30;
            iSetSpritePosition(&bullet_sprites[i], bullet_x[i], bullet_y[i]);
            if (bullet_x[i] > SCREEN_WIDTH) {
                bullet_active[i] = 0;
                bullet_x[i] = -100;
                bullet_y[i] = -100;
                iSetSpritePosition(&bullet_sprites[i], -100, -100);
            }
        }
    }
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (ebullet_active[i]) {
            ebullet_x[i] -= 30;
            iSetSpritePosition(&ebulsprite[i], ebullet_x[i], ebullet_y[i]);
            if (ebullet_x[i] < 0) {
                ebullet_active[i] = 0;
                ebullet_x[i] = -100;
                ebullet_y[i] = -100;
                iSetSpritePosition(&ebulsprite[i], -100, -100);
            }
        }
    }
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (e2bullet_active[i]) {
            e2bullet_x[i] -= 30;
            iSetSpritePosition(&e2bulsprite[i], e2bullet_x[i], e2bullet_y[i]);
            if (e2bullet_x[i] < 0) {
                e2bullet_active[i] = 0;
                e2bullet_x[i] = -100;
                e2bullet_y[i] = -100;
                iSetSpritePosition(&e2bulsprite[i], -100, -100);
            }
        }
    }
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (e6bullet_active[i]) {
            e6bullet_x[i] -= 15; // Reduced speed from 30 to 15
            iSetSpritePosition(&e6bulsprite[i], e6bullet_x[i], e6bullet_y[i]);
            if (e6bullet_x[i] < -170) {
                e6bullet_active[i] = 0;
                e6bullet_x[i] = -100;
                e6bullet_y[i] = -100;
                iSetSpritePosition(&e6bulsprite[i], -100, -100);
            }
        }
    }
}

void iDraw() {
    iClear();
    switch (gamestate) {
        case 1:
            ship_state = IDLE;
            homepage();
            break;
        case 2:
            difficulty();
            break;
        case 21:
            mainpage1();
            if (game_paused) {
                iShowLoadedImage(420, 400, &paused);
                iSetColor(255, 255, 255);
                iShowText(520, 370, "RESUME", "assets/fonts/mokoto.ttf");
                iShowText(510, 270, "RESTART", "assets/fonts/mokoto.ttf");
                iShowText(475, 170, "MAIN MENU", "assets/fonts/mokoto.ttf");
            }
            break;
        case 211:
            iShowLoadedImage(0, 0, &gameoverscreen);
            if (gocount == 0) {
                iShowLoadedImage(400, 300, &go1);
            } else if (gocount == 1) {
                iShowLoadedImage(400, 300, &go2);
            } else if (gocount == 2) {
                iShowLoadedImage(400, 300, &go3);
            } else if (gocount == 3) {
                iShowLoadedImage(400, 300, &go4);
            } else if (gocount == 4) {
                iShowLoadedImage(400, 300, &go5);
            } else if (gocount == 5) {
                iShowLoadedImage(400, 300, &go6);
            } else if (gocount == 6) {
                iShowLoadedImage(400, 300, &go7);
            } else if (gocount == 7) {
                iShowLoadedImage(400, 300, &go8);
            }
            if (scorecolour == 0) {
                iShowLoadedImage(400, 50, &score1);
            } else if (scorecolour == 1) {
                iShowLoadedImage(400, 50, &score2);
            } else if (scorecolour == 2) {
                iShowLoadedImage(400, 50, &score3);
            }
            sprintf(scoretext, "%d", scorenumber);
            iShowText(640, 140, scoretext, "assets/fonts/Orbitron-Medium.ttf");
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
            case 21:
                if (game_paused) {
                    if ((522 <= mx && mx <= 704) && (372 <= my && my <= 403)) {
                        game_paused = false;
                        iResumeTimer(timer_id);
                        iResumeTimer(animation_timer_id);
                        if (sound_check == 0)
                            iResumeSound(mainidx);
                    }
                    else if ((511 <= mx && mx <= 719) && (273 <= my && my <= 303)) {
                        game_paused = false;
                        resetGame();
                        gamestate = 21;
                        if (sound_check == 0) {
                            iResumeSound(mainidx);
                        }
                    }
                    else if ((476 <= mx && mx <= 746) && (172 <= my && my <= 203)) {
                        game_paused = false;
                        resetGame();
                        gamestate = 1;
                        if (sound_check == 0) {
                            iPauseSound(mainidx);
                            iResumeSound(homeidx);
                        }
                    }
                }
                break;
            case 211:
                if ((344 <= mx && mx <= 541) && (246 <= my && my <= 298)) {
                    game_paused = false;
                    resetGame();
                    gamestate = 21;
                    if (sound_check == 0) {
                        iResumeSound(mainidx);
                    }
                }
                if ((601 <= mx && mx <= 806) && (241 <= my && my <= 294)) {
                    game_paused = false;
                    resetGame();
                    gamestate = 1;
                    if (sound_check == 0) {
                        iPauseSound(mainidx);
                        iResumeSound(homeidx);
                    }
                }
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
                if (ship_state != EXP) {
                    ship_state = SHOOT;
                    iChangeSpriteFrames(&spaceship, s_shoot, 4);
                    iAnimateSprite(&spaceship);
                    if (rocket_powerup_active) {
                        int num_bullets = 5;
                        int y_offsets[5] = {83 + 30, 83 + 15, 83, 83 - 15, 83 - 30};
                        int fired = 0;
                        for (int i = 0; i < MAX_BULLETS && fired < num_bullets; i++) {
                            if (!bullet_active[i]) {
                                bullet_x[i] = move_lf + 190;
                                bullet_y[i] = move_ud + y_offsets[fired];
                                iSetSpritePosition(&bullet_sprites[i], bullet_x[i], bullet_y[i]);
                                bullet_active[i] = 1;
                                fired++;
                            }
                        }
                    } else {
                        int num_bullets = 2;
                        int y_offsets[2] = {90, 77};
                        int fired = 0;
                        for (int i = 0; i < MAX_BULLETS && fired < num_bullets; i++) {
                            if (!bullet_active[i]) {
                                bullet_x[i] = move_lf + 190;
                                bullet_y[i] = move_ud + y_offsets[fired];
                                iSetSpritePosition(&bullet_sprites[i], bullet_x[i], bullet_y[i]);
                                bullet_active[i] = 1;
                                fired++;
                            }
                        }
                    }
                }
                break;
            case 'f':
                if (fullscreen == 0) {
                    iEnterFullscreen();
                    fullscreen++;
                } else {
                    iLeaveFullscreen();
                    fullscreen--;
                }
                break;
            case 'o':
                if (gamestate == 21) {
                    game_paused = !game_paused;
                    if (game_paused) {
                        iPauseTimer(timer_id);
                        iPauseTimer(animation_timer_id);
                        if (sound_check == 0) {
                            iPauseSound(mainidx);
                        }
                    } else {
                        iResumeTimer(timer_id);
                        iResumeTimer(animation_timer_id);
                        if (sound_check == 0) {
                            iResumeSound(mainidx);
                        }
                    }
                }
                break;
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
                if (ship_state != EXP) {
                    ship_state = IDLE;
                    iChangeSpriteFrames(&spaceship, idle, 1);
                }
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
                } else {
                    if (gamestate == 1 || gamestate == 2) {
                        iResumeSound(homeidx);
                        sound_check = 0;
                    }
                    if (gamestate == 21) {
                        iResumeSound(mainidx);
                        sound_check = 0;
                    }
                }
                break;
            case 27:
                if (gamestate == 21) {
                    game_paused = true;
                    iPauseTimer(timer_id);
                    iPauseTimer(animation_timer_id);
                    if (sound_check == 0) {
                        iPauseSound(mainidx);
                    }
                }
                if (gamestate == 2) gamestate = 1;
                break;
        }
    }
}

void iSpecialKeyPress(unsigned char key) {
    switch (key) {
        default:
            break;
    }
}

void timer() {
    if (game_paused) return;
    moveBullets();
    moveSpaceship();
    updateBonuses();
    updateEnemy();
    updateEnemyExplosion();
    updatemeteor();
    if (just_reset && just_reset_timer > 0) {
        just_reset_timer -= 100; // Adjusted to 100ms
        if (just_reset_timer <= 0) {
            just_reset = false;
        }
    }
    if (gamestate == 211) {
        gocount = (gocount + 1) % 8;
        scorecolourtimer += 100; // Adjusted to 100ms
        if (scorecolourtimer >= 1000) {
            scorecolour = (scorecolour + 1) % 3;
            scorecolourtimer = 0;
        }
    }
    if (gamestate == 21) {
        survival_score_timer += 100; // Adjusted to 100ms
        if (survival_score_timer >= 70) {
            scorenumber += 1;
            sprintf(scoretext, "%d", scorenumber);
            survival_score_timer = 0;
        }
    }
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    iInitializeSound();
    loadresources();
    sound_manage();
    iInitializeFont();
    timer_id = iSetTimer(50, timer); // Increased from 50ms to 100ms
    animation_timer_id = iSetTimer(100, updateAnimation);
    iOpenWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Galaxy-Annihilator");
    return 0;
}
