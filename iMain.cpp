#include "iGraphics.h"
#include <iostream>
#include "iSound.h"
#include <stdlib.h>
#include "iFont.h"
#include <stdio.h>
#include <string.h>

using namespace std;

//=============================================================================
// CONSTANTS AND DEFINES
//=============================================================================

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 700
#define MAX_BULLETS 50
#define MAX_NAME_LENGTH 12                  // Maximum length for player name
char player_name[MAX_NAME_LENGTH + 1] = ""; // +1 for null terminator
int name_length = 0;
#define MAX_LEADERBOARD_ENTRIES 7

typedef struct {
    char name[MAX_NAME_LENGTH + 1]; // Matches player_name size
    int score;
} LeaderboardEntry;

LeaderboardEntry arcadeLeaderboard[MAX_LEADERBOARD_ENTRIES];
LeaderboardEntry bossLeaderboard[MAX_LEADERBOARD_ENTRIES];
LeaderboardEntry endlessLeaderboard[MAX_LEADERBOARD_ENTRIES];
int arcadeCount = 0, bossCount = 0, endlessCount = 0; // Track number of entries

#define ARCADE_SCORE_FILE "saves/arcade_scores.txt"
#define BOSS_SCORE_FILE "saves/boss_scores.txt"
#define ENDLESS_SCORE_FILE "saves/endless_scores.txt"
//=============================================================================
// ENUMS
//=============================================================================

enum
{
    IDLE,
    BOOST,
    SHOOT,
    EXP
};
typedef enum
{
    HOME,
    MODE,
    LEADERBOARD,
    HELP,
    ABOUT,
    ARCADE = 11,
    BOSS,
    ENDLESS,
    CONTROLS = 31,
    SOUND,
    QUIT,
    GAMEOVER,
    NAME,
    ARCADE_SCORE,
    BOSS_SCORE,
    ENDLESS_SCORE
} game;
game gamestate;
//=============================================================================
// GLOBAL VARIABLES
//=============================================================================

// Game State Variables

bool game_paused = false;
int timer_id, animation_timer_id;
bool just_reset = false;
int just_reset_timer = 0;
game prev_gamestate;

// Score and UI Variables
Image blu3,blu2,blu1,blugo,bla3,bla2,bla1,blago,pur3,pur2,pur1,purgo;
int blu=3, bla=3, pur=3,countertimer=0;
int scorenumber = 0;
char scoretext[100];
int scorecolour = 0;
int scorecolourtimer = 0;
int survival_score_timer = 0;
int gocount = 0;
int wrap;
int arcade_score, boss_score, endless_score;
// Player Variables
int move_ud = 280, move_lf = 0;
int health = 3;
int ship_state = IDLE;
bool shipexp = false;
int boost_idx = 0;
int shoot_idx = 0;
int exp_idx = 0;
int invincibility_end_time = 0;

// Movement Key States
bool key_w = false, key_a = false, key_s = false, key_d = false;

// Bullet Arrays
int bullet_x[MAX_BULLETS], bullet_y[MAX_BULLETS], bullet_active[MAX_BULLETS];
double bullet_angle[MAX_BULLETS];

int ebullet_x[MAX_BULLETS], ebullet_y[MAX_BULLETS], ebullet_active[MAX_BULLETS];
int e2bullet_x[MAX_BULLETS], e2bullet_y[MAX_BULLETS], e2bullet_active[MAX_BULLETS];
int e3bullet_x[MAX_BULLETS], e3bullet_y[MAX_BULLETS], e3bullet_active[MAX_BULLETS];
int e4bullet_x[MAX_BULLETS], e4bullet_y[MAX_BULLETS], e4bullet_active[MAX_BULLETS];
int e5bullet_x[MAX_BULLETS], e5bullet_y[MAX_BULLETS], e5bullet_active[MAX_BULLETS];
int e6bullet_x[MAX_BULLETS], e6bullet_y[MAX_BULLETS], e6bullet_active[MAX_BULLETS];
double e6bullet_vx[MAX_BULLETS], e6bullet_vy[MAX_BULLETS]; // Velocity arrays for enem6 bullets

// Enemy Variables
int enemy_wave = 0;
int wave_timer = 0;
bool enem1_active = false;
int enem1_x = 1200, enem1_y = 500;
int enem1_fire_timer = 0;
int enem1_respawn_timer = 0;
bool enem1_exploding = false;
int enem1_exp_idx = 0;
int enem1hp = 1;

bool enem2_active = false;
int enem2_x = 1200, enem2_y = 100;
int enem2_fire_timer = 0;
int enem2_respawn_timer = 0;
bool enem2_exploding = false;
int enem2_exp_idx = 0;
int enem2hp = 4;

bool enem3_active = false;
int enem3_x = 1200, enem3_y = 300;
int enem3_fire_timer = 0;
int enem3_respawn_timer = 0;
bool enem3_exploding = false;
int enem3_exp_idx = 0;
int enem3hp = 6;

bool enem4_active = false;
int enem4_x = 1200, enem4_y = 100;
int enem4_fire_timer = 0;
int enem4_respawn_timer = 0;
bool enem4_exploding = false;
int enem4_exp_idx = 0;
int enem4hp = 8;

bool enem5_active = false;
int enem5_x = 1200, enem5_y = 100;
int enem5_fire_timer = 0;
int enem5_respawn_timer = 0;
bool enem5_exploding = false;
int enem5_exp_idx = 0;
int enem5hp = 10;

bool enem6_active = false;
int enem6_x = SCREEN_WIDTH - 370, enem6_y = SCREEN_HEIGHT + 50;
int enem6_fire_timer = 0;
int enem6_respawn_timer = 0;
bool enem6_exploding = false;
int enem6_exp_idx = 0;
int enem6hp = 300;
int bosshp = 600;

bool enem6_spawned = false;    // Track if enem6 has already spawned
bool enem6_moving_down = true; // Tracks direction of enem6 movement

// Meteor Variables
bool meteor = false;
int metx = 550, mety = 1000;
int meteor_spawn_timer = 0;
double meteorRotationAngle = 0.0;
bool meteor2 = false;
int met2x = 550, met2y = -300;
int meteor2_spawn_timer = 0;
double meteor2RotationAngle = 0.0;

// Power-up Variables
bool bonushp = false, bonusrocket = false, bonusshield = false;
int bonushpx = 700, bonushpy = 1500;
int bonusrocketx = 500, bonusrockety = -1800;
int bonusshieldx = 1000, bonusshieldy = 800;
int hpbonus_spawn_timer = 0;
int rocket_spawn_timer = 0;
int shield_spawn_timer = 0;

// Power-up Effect Variables
bool rocket_powerup_active = false;
int rocket_powerup_end_time = 0;
int rocket_powerup_start_time = 0;
int rocket_powerup_count = 0; // Add after other global variables (e.g., after rocket_powerup_active)

bool shield_active = false;
int shield_hp = 3;
int shieldx, shieldy;

// Rotation Angles
double hprotangle = 0.0, rockrotangle = 0.0, shieldrotangle = 0.0;

// Sound Variables
int homeidx = -1, mainidx = -1;
int homesound, mainsound;
int sound_check = 0;
int fullscreen = 0;
int rocket_countidx=-1;

// Miscellaneous
char des[7][100];
int a = 0;
int bul1_x;
bool enem1_bul_active = false;

//=============================================================================
// IMAGE AND SPRITE DECLARATIONS
//=============================================================================

// UI Images
Image mainbg, paused, mainbg2, abg;
Image hp1, hp2, hp3, score;
Image gameoverscreen, score1, score2, score3;
Image go1, go2, go3, go4, go5, go6, go7, go8;
Image num1, num2, num3, num4, num5, num6, num7, num8, num9, num0;
Image home, mode, help, controls, quit, sound, name;
Image ascore, bscore, lscore;
// Spaceship Images and Sprites
Image idle[1], bullet_img[1];
Image s_boost[6], s_exp[7], s_shoot[4];
Sprite spaceship;
Sprite bullet_sprites[MAX_BULLETS];

// Enemy Images and Sprites
Image e1idle[1], e1exp[10], e1bul[1];
Image e6idle[1], e6exp[11], e6bul[1];
Image e2idle[1], e2exp[12], e2bul[1];
Image e3idle[1], e3exp[11], e3bul[1];
Image e4idle[1], e4exp[11], e4bul[1];
Image e5idle[1], e5exp[11], e5bul[1];
Sprite enem1, enem2, enem3, enem4, enem5, enem6;
Sprite ebulsprite[MAX_BULLETS], e2bulsprite[MAX_BULLETS];
Sprite e3bulsprite[MAX_BULLETS], e4bulsprite[MAX_BULLETS];
Sprite e5bulsprite[MAX_BULLETS], e6bulsprite[MAX_BULLETS];

// Meteor Images and Sprites
Image mete[1], mete2[1];
Sprite met, met2;

// Power-up Images and Sprites
Image hpbonus[1], rocket[1], shield[1], shieldimg[1];
Sprite bo_hp, bo_roc, bo_shi, shieldsprt;

//=============================================================================
// FUNCTION PROTOTYPES
//=============================================================================

// Core Game Functions
void resetGame();
void loadresources();
int getNonOverlappingXPosition(int existing_x1, int existing_x2, int min_distance);

// Update Functions
void updateAnimation();
void moveSpaceship();
void moveBullets();
void updatemeteor();
void updateBonuses();
void updateEnemy();
void updateEnemyExplosion();

// Collision Detection Functions
void checkBulletEnemyCollision();
void checkEnemSpaceCollision();
void enemBulletCollision();
void checkshieldcollision();

// Enemy Functions
void enem_shoot();

// Screen Display Functions
void homepage();
void difficulty();
void mainpage1();

// Utility Functions
void sound_manage();
void timer();

// iGraphics Callback Functions
void iDraw();
void iMouseMove(int mx, int my);
void iMouseDrag(int mx, int my);
void iMouse(int button, int state, int mx, int my);
void iMouseWheel(int dir, int mx, int my);
void iKeyboard(unsigned char key, int state);
void iSpecialKeyPress(unsigned char key);

//=============================================================================
// GAME RESET FUNCTION
//=============================================================================
int compareLeaderboard(const void *a, const void *b) {
    const LeaderboardEntry *entryA = (const LeaderboardEntry *)a;
    const LeaderboardEntry *entryB = (const LeaderboardEntry *)b;
    return entryB->score - entryA->score; // Descending
}

// Load leaderboard from file
void loadLeaderboard(const char *filename, LeaderboardEntry *leaderboard, int *count) {
    FILE *file = fopen(filename, "r");
    *count = 0;
    if (file) {
        while (*count < MAX_LEADERBOARD_ENTRIES &&
               fscanf(file, "%s %d", leaderboard[*count].name, &leaderboard[*count].score) == 2) {
            (*count)++;
        }
        fclose(file);
    }
    // Sort after loading
    qsort(leaderboard, *count, sizeof(LeaderboardEntry), compareLeaderboard);
}

// Save leaderboard to file
void saveLeaderboard(const char *filename, LeaderboardEntry *leaderboard, int count) {
    FILE *file = fopen(filename, "w");
    if (file) {
        for (int i = 0; i < count && i < MAX_LEADERBOARD_ENTRIES; i++) {
            fprintf(file, "%s %d\n", leaderboard[i].name, leaderboard[i].score);
        }
        fclose(file);
    }
}

// Add a new score to leaderboard
void addScoreToLeaderboard(LeaderboardEntry *leaderboard, int *count, const char *name, int score) {
    if (*count < MAX_LEADERBOARD_ENTRIES) {
        // Add new entry
        strcpy(leaderboard[*count].name, name);
        leaderboard[*count].score = score;
        (*count)++;
    } else if (score > leaderboard[MAX_LEADERBOARD_ENTRIES - 1].score) {
        // Replace lowest score if new score is higher
        strcpy(leaderboard[MAX_LEADERBOARD_ENTRIES - 1].name, name);
        leaderboard[MAX_LEADERBOARD_ENTRIES - 1].score = score;
    }
    // Sort leaderboard
    qsort(leaderboard, *count, sizeof(LeaderboardEntry), compareLeaderboard);
}

// Initialize all leaderboards at game start
void initLeaderboards() {
    loadLeaderboard(ARCADE_SCORE_FILE, arcadeLeaderboard, &arcadeCount);
    loadLeaderboard(BOSS_SCORE_FILE, bossLeaderboard, &bossCount);
    loadLeaderboard(ENDLESS_SCORE_FILE, endlessLeaderboard, &endlessCount);
}
void resetGame()

{   
    //player_name[13] =  "";
    //bla=blu=pur=3;
    bla =3;
    blu = 3;
    pur = 3;
    countertimer=0;
    wave_timer = 0;
    enemy_wave = 0;
    // Reset Player Stats
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

    // Reset Score
    scorenumber = 0;
    sprintf(scoretext, "%d", scorenumber);

    // Reset Enemy 1
    enem1_active = false;
    enem1_x = 1200;
    enem1_y = 500;
    enem1_respawn_timer = 0;
    enem1_exploding = false;
    enem1_exp_idx = 0;
    enem1_fire_timer = 0;
    iSetSpritePosition(&enem1, enem1_x, enem1_y);
    iChangeSpriteFrames(&enem1, e1idle, 1);

    // Reset Enemy 2
    enem2_active = false;
    enem2_x = 1200;
    enem2_y = 100;
    enem2_respawn_timer = 0;
    enem2_exploding = false;
    enem2_exp_idx = 0;
    enem2_fire_timer = 0;
    enem2hp = 4;
    iSetSpritePosition(&enem2, enem2_x, enem2_y);
    iChangeSpriteFrames(&enem2, e2idle, 1);

    // Reset Enemy 3

    enem3_active = false;
    enem3_x = 1200;
    enem3_y = 300;
    enem3_respawn_timer = 0;
    enem3_exploding = false;
    enem3_exp_idx = 0;
    enem3_fire_timer = 0;
    enem3hp = 6;
    iSetSpritePosition(&enem3, enem3_x, enem3_y);
    iChangeSpriteFrames(&enem3, e3idle, 1);

    // Reset Enemy 4
    enem4_active = false;
    enem4_x = 1200;
    enem4_y = 500;
    enem4_respawn_timer = 0;
    enem4_exploding = false;
    enem4_exp_idx = 0;
    enem4_fire_timer = 0;
    enem4hp = 8;
    iSetSpritePosition(&enem4, enem4_x, enem4_y);
    iChangeSpriteFrames(&enem4, e4idle, 1);

    // Reset Enemy 5
    enem5_active = false;
    enem5_x = 1200;
    enem5_y = 100;
    enem5_respawn_timer = 0;
    enem5_exploding = false;
    enem5_exp_idx = 0;
    enem5_fire_timer = 0;
    enem5hp = 10;
    iSetSpritePosition(&enem5, enem5_x, enem5_y);
    iChangeSpriteFrames(&enem5, e5idle, 1);

    // Reset Enemy 6
    enem6_active = (gamestate == BOSS);
    enem6_x = SCREEN_WIDTH - 370;
    enem6_y = SCREEN_HEIGHT + 50;
    enem6_respawn_timer = 0;
    enem6_exploding = false;
    enem6_exp_idx = 0;
    enem6_fire_timer = 0;
    enem6hp = 300;
    bosshp = 600;
    if (gamestate == ARCADE)
    {
        enem6_spawned = false;
    }
    else if (gamestate == BOSS)
    {
        enem6_spawned = true;
    }
    enem6_spawned = false; // Reset spawn flag
    enem6_moving_down = true;
    iSetSpritePosition(&enem6, enem6_x, enem6_y);
    iChangeSpriteFrames(&enem6, e6idle, 1);
    enem1_respawn_timer = 0; // Add this line
    enem2_respawn_timer = 0;

    // Reset All Bullets
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullet_active[i] = 0;
        ebullet_active[i] = 0;
        e2bullet_active[i] = 0;
        e3bullet_active[i] = 0;
        e4bullet_active[i] = 0;
        e5bullet_active[i] = 0;
        e6bullet_active[i] = 0;
        e6bullet_vx[i] = 0.0; // Add this line
        e6bullet_vy[i] = 0.0;
    }
    rocket_powerup_active = false;
    rocket_powerup_end_time = 0;
    rocket_powerup_start_time = 0;
    rocket_powerup_count = 0;
    // Reset Meteor
    meteor = false;
    metx = 550;
    mety = 1000;
    meteor_spawn_timer = 0;
    iSetSpritePosition(&met, metx, mety);
    meteor2 = false;
    met2x = 550;
    met2y = -300;
    meteor2_spawn_timer = 0;
    iSetSpritePosition(&met2, met2x, met2y);
    // Reset Power-ups
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

    // Reset Shield
    shield_active = false;
    shield_hp = 3;
    iSetSpritePosition(&shieldsprt, -100, -100);

    // Reset Rotation Angles
    meteorRotationAngle = 0.0;
    meteor2RotationAngle = 0.0;
    hprotangle = 0.0;
    rockrotangle = 0.0;
    shieldrotangle = 0.0;

    // Reset Game State
    invincibility_end_time = 0;
    just_reset = true;
    just_reset_timer = 2000;
    survival_score_timer = 0;
    player_name[0] = '\0'; // Clear the player_name array
    name_length = 0;
    // Resume Timers
    iResumeTimer(timer_id);
    iResumeTimer(animation_timer_id);
}

//=============================================================================
// RESOURCE LOADING FUNCTION
//=============================================================================

void loadresources()
{    //load countdown
    iLoadImage(&blu3,"assets/images/gs_blu/3.png");
    iLoadImage(&blu2,"assets/images/gs_blu/2.png");
    iLoadImage(&blu1,"assets/images/gs_blu/1.png");
    iLoadImage(&blugo,"assets/images/gs_blu/go.png");
    iScaleImage(&blu3, 0.1);
    iScaleImage(&blu2, 0.1);
    iScaleImage(&blu1, 0.1);
    iScaleImage(&blugo, 0.1);
    iLoadImage(&bla3,"assets/images/gs_bla/3.png");
    iLoadImage(&bla2,"assets/images/gs_bla/2.png");
    iLoadImage(&bla1,"assets/images/gs_bla/1.png");
    iLoadImage(&blago,"assets/images/gs_bla/go.png");
    iScaleImage(&bla3, 0.1);
    iScaleImage(&bla2, 0.1);
    iScaleImage(&bla1, 0.1);
    iScaleImage(&blago, 0.1);
    iLoadImage(&pur3,"assets/images/gs_purp/3.png");
    iLoadImage(&pur2,"assets/images/gs_purp/2.png");
    iLoadImage(&pur1,"assets/images/gs_purp/1.png");
    iLoadImage(&purgo,"assets/images/gs_purp/go.png");
    iScaleImage(&pur3, 0.1);
    iScaleImage(&pur2, 0.1);
    iScaleImage(&pur1, 0.1);
    iScaleImage(&purgo, 0.1);
    // Load Game Over Screen Images
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

    // Load Number Images
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

    // Load Shield Sprite
    iInitSprite(&shieldsprt);
    iLoadFramesFromFolder(shieldimg, "assets/images/main_shield/");
    iChangeSpriteFrames(&shieldsprt, shieldimg, 1);
    iScaleSprite(&shieldsprt, 0.2);

    // Load Menu and Background Images
    iLoadImage(&home, "assets/images/gamestate/home.png");
    iLoadImage(&mode, "assets/images/gamestate/mode.png");
    iLoadImage(&help, "assets/images/gamestate/help.png");
    iLoadImage(&controls, "assets/images/gamestate/controls.png");
    iLoadImage(&sound, "assets/images/gamestate/sound.png");
    iLoadImage(&quit, "assets/images/gamestate/quit.png");
    iLoadImage(&name, "assets/images/gamestate/name.png");
    iLoadImage(&ascore, "assets/images/gamestate/score.png");
    iLoadImage(&bscore, "assets/images/gamestate/score.png");
    iLoadImage(&lscore, "assets/images/gamestate/score.png");

    iLoadImage(&mainbg, "assets/images/mainbg.png");
    iLoadImage(&abg, "assets/images/BG.png");
    iLoadImage(&mainbg2, "assets/images/endlessbg.jpg");
    // Load and Initialize Meteor Sprite
    iInitSprite(&met);
    iLoadFramesFromFolder(mete, "assets/images/sprites/Meteors/mainmet/");
    iChangeSpriteFrames(&met, mete, 1);
    iSetSpritePosition(&met, metx, mety);
    iScaleSprite(&met, .3);
    iInitSprite(&met2);
    iLoadFramesFromFolder(mete2, "assets/images/sprites/Meteors/mainmet2/");
    iChangeSpriteFrames(&met2, mete2, 1);
    iSetSpritePosition(&met2, met2x, met2y);
    iScaleSprite(&met2, .3);
    // Load Spaceship Images
    iLoadImage(&idle[0], "assets/images/sprites/Spaceship/Idle.png");
    iLoadFramesFromFolder(s_boost, "assets/images/sprites/Spaceship/boost/");
    iLoadFramesFromFolder(s_shoot, "assets/images/sprites/Spaceship/shoot/");
    iLoadFramesFromFolder(s_exp, "assets/images/sprites/Spaceship/explosion/");

    // Load Enemy Images
    iLoadImage(&e1idle[0], "assets/images/sprites/enemy/Ship1/Ship1.png");
    iLoadFramesFromFolder(e1exp, "assets/images/sprites/enemy/explosions/Ship1_Explosion/");
    iLoadImage(&e2idle[0], "assets/images/sprites/enemy/Ship2/Ship2.png");
    iLoadFramesFromFolder(e2exp, "assets/images/sprites/enemy/explosions/Ship2_Explosion/");
    iLoadImage(&e6idle[0], "assets/images/sprites/enemy/Ship6/Ship6.png");
    iLoadFramesFromFolder(e6exp, "assets/images/sprites/enemy/explosions/Ship6_Explosion/");
    iLoadImage(&e3idle[0], "assets/images/sprites/enemy/Ship3/Ship3.png");
    iLoadFramesFromFolder(e3exp, "assets/images/sprites/enemy/explosions/Ship3_Explosion/");
    iLoadImage(&e4idle[0], "assets/images/sprites/enemy/Ship4/Ship4.png");
    iLoadFramesFromFolder(e4exp, "assets/images/sprites/enemy/explosions/Ship4_Explosion/");
    iLoadImage(&e5idle[0], "assets/images/sprites/enemy/Ship5/Ship5.png");
    iLoadFramesFromFolder(e5exp, "assets/images/sprites/enemy/explosions/Ship5_Explosion/");
    // Initialize Spaceship Sprite
    iInitSprite(&spaceship);
    iChangeSpriteFrames(&spaceship, idle, 1);
    iSetSpritePosition(&spaceship, move_lf, move_ud);

    // Initialize Player Bullets
    iLoadImage(&bullet_img[0], "assets/images/sprites/Spaceship/Charge_2.png");
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        iInitSprite(&bullet_sprites[i]);
        iChangeSpriteFrames(&bullet_sprites[i], bullet_img, 1);
        bullet_active[i] = 0;
    }

    // Initialize Enemy Bullets
    iLoadImage(&e1bul[0], "assets/images/sprites/enemy/Shots/Shot1/enembullet.png");
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        iInitSprite(&ebulsprite[i]);
        iScaleSprite(&ebulsprite[i], 1.7);
        iChangeSpriteFrames(&ebulsprite[i], e1bul, 1);
        ebullet_active[i] = 0;
    }

    iLoadImage(&e2bul[0], "assets/images/sprites/enemy/Shots/Shot2/shot2_asset.png");
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        iInitSprite(&e2bulsprite[i]);
        iScaleSprite(&e2bulsprite[i], 1.3);
        iChangeSpriteFrames(&e2bulsprite[i], e2bul, 1);
        e2bullet_active[i] = 0;
    }

    iLoadImage(&e3bul[0], "assets/images/sprites/enemy/Shots/Shot3/shot3_asset.png");
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        iInitSprite(&e3bulsprite[i]);
        iScaleSprite(&e3bulsprite[i], 1.2);
        iChangeSpriteFrames(&e3bulsprite[i], e3bul, 1);
        e3bullet_active[i] = 0;
    }

    iLoadImage(&e4bul[0], "assets/images/sprites/enemy/Shots/Shot4/shot4_3.png");
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        iInitSprite(&e4bulsprite[i]);
        iScaleSprite(&e4bulsprite[i], 1.2);
        iChangeSpriteFrames(&e4bulsprite[i], e4bul, 1);
        e4bullet_active[i] = 0;
    }

    iLoadImage(&e5bul[0], "assets/images/sprites/enemy/Shots/Shot5/shot5_4.png");
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        iInitSprite(&e5bulsprite[i]);
        iScaleSprite(&e5bulsprite[i], 1.2);
        iChangeSpriteFrames(&e5bulsprite[i], e5bul, 1);
        e5bullet_active[i] = 0;
    }

    iLoadImage(&e6bul[0], "assets/images/sprites/enemy/Shots/Shot6/mainbull/shot6_2.png");
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        iInitSprite(&e6bulsprite[i]);
        iScaleSprite(&e6bulsprite[i], 1.7);
        iChangeSpriteFrames(&e6bulsprite[i], e6bul, 1);
        e6bullet_active[i] = 0;
    }

    // Load Health and UI Images
    iLoadImage(&hp1, "assets/images/hp/full.png");
    iLoadImage(&hp2, "assets/images/hp/medium.png");
    iLoadImage(&hp3, "assets/images/hp/low.png");
    iScaleImage(&hp1, 0.2);
    iScaleImage(&hp2, 0.2);
    iScaleImage(&hp3, 0.2);

    iLoadImage(&score, "assets/images/score.png");
    iScaleImage(&score, 0.085);

    // Initialize Power-up Sprites
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

    // Initialize Enemy Sprites
    iInitSprite(&enem1);
    iChangeSpriteFrames(&enem1, e1idle, 1);
    iSetSpritePosition(&enem1, enem1_x, enem1_y);

    iInitSprite(&enem2);
    iScaleSprite(&enem2, 1.7);
    iChangeSpriteFrames(&enem2, e2idle, 1);
    iSetSpritePosition(&enem2, enem2_x, enem2_y);

    iInitSprite(&enem3);
    iScaleSprite(&enem3, 1.2);
    iChangeSpriteFrames(&enem3, e3idle, 1);
    iSetSpritePosition(&enem3, enem3_x, enem3_y);

    iInitSprite(&enem4);
    iScaleSprite(&enem4, 1.2);
    iChangeSpriteFrames(&enem4, e4idle, 1);
    iSetSpritePosition(&enem4, enem4_x, enem4_y);

    iInitSprite(&enem5);
    iScaleSprite(&enem5, 1.2);
    iChangeSpriteFrames(&enem5, e5idle, 1);
    iSetSpritePosition(&enem5, enem5_x, enem5_y);

    iInitSprite(&enem6);
    iScaleSprite(&enem6, 3); // Scale enem6 for visibility
    iChangeSpriteFrames(&enem6, e6idle, 1);
    iSetSpritePosition(&enem6, enem6_x, enem6_y);

    // Load Pause Screen
    iLoadImage(&paused, "assets/images/paused.png");
    iResizeImage(&paused, 400, 400);
}

//=============================================================================
// UTILITY FUNCTIONS
//=============================================================================

int getNonOverlappingXPosition(int existing_x1, int existing_x2, int min_distance)
{
    int new_x;
    do
    {
        new_x = rand() % (SCREEN_WIDTH - 50);
    } while ((existing_x1 != -1 && abs(new_x - existing_x1) < min_distance) ||
             (existing_x2 != -1 && abs(new_x - existing_x2) < min_distance));
    return new_x;
}

//=============================================================================
// ANIMATION AND MOVEMENT FUNCTIONS
//=============================================================================

void updateAnimation() {
    if (game_paused)
        return;

    if (ship_state == BOOST) {
        boost_idx = (boost_idx + 1) % 6;
        iChangeSpriteFrames(&spaceship, s_boost, 6);
    }
    if (ship_state == SHOOT) {
        shoot_idx = (shoot_idx + 1) % 4;
        iChangeSpriteFrames(&spaceship, s_shoot, 4);
    }
    if (ship_state == EXP && shipexp) {
        printf("Explosion frame: %d\n", exp_idx);
        exp_idx++;
        if (exp_idx < 7) {
            iChangeSpriteFrames(&spaceship, s_exp, 7);
            spaceship.currentFrame = exp_idx;
        } else {
            printf("Explosion complete, transitioning to game over\n");
            shipexp = false;
            ship_state = IDLE;
            
            if (gamestate == ARCADE) {
                arcade_score = scorenumber;
                addScoreToLeaderboard(arcadeLeaderboard, &arcadeCount, player_name, arcade_score);
                saveLeaderboard(ARCADE_SCORE_FILE, arcadeLeaderboard, arcadeCount);
                 prev_gamestate = gamestate;

                gamestate = GAMEOVER;
            } else if (gamestate == BOSS) {
                boss_score = scorenumber;
                addScoreToLeaderboard(bossLeaderboard, &bossCount, player_name, boss_score);
                saveLeaderboard(BOSS_SCORE_FILE, bossLeaderboard, bossCount);
                            prev_gamestate = gamestate;

                gamestate = GAMEOVER;
            } else if (gamestate == ENDLESS) {
                endless_score = scorenumber;
                addScoreToLeaderboard(endlessLeaderboard, &endlessCount, player_name, endless_score);
                saveLeaderboard(ENDLESS_SCORE_FILE, endlessLeaderboard, endlessCount);
                            prev_gamestate = gamestate;

                gamestate = GAMEOVER;
            }
            iChangeSpriteFrames(&spaceship, idle, 1);
            exp_idx = 0;
        }
    }
}
void moveSpaceship()
{
    if (game_paused || ship_state == EXP)
        return;

    // if (gamestate == 21)
    // {
    // Vertical Movement
    if (key_w)
    {
        move_ud += 25;
        if (move_ud >= 555)
            move_ud = 555;
    }
    if (key_s)
    {
        move_ud -= 25;
        if (move_ud < -48)
            move_ud = -48;
    }

    // Horizontal Movement with Boost Animation
    if (key_a)
    {
        move_lf -= 25;
        if (move_lf < 0)
            move_lf = 0;
        ship_state = BOOST;
        iChangeSpriteFrames(&spaceship, s_boost, 6);
    }
    if (key_d)
    {
        move_lf += 25;
        if (move_lf > 1010)
            move_lf = 1010;
        ship_state = BOOST;
        iChangeSpriteFrames(&spaceship, s_boost, 6);
    }

    // Return to Idle State
    if (!key_a && !key_d && ship_state == BOOST)
    {
        ship_state = IDLE;
        iChangeSpriteFrames(&spaceship, idle, 1);
    }

    // Update Sprite Position
    iSetSpritePosition(&spaceship, move_lf, move_ud);

    // Update Shield Position
    if (shield_active)
    {
        shieldx = move_lf - 5;
        shieldy = move_ud - 5;
        iSetSpritePosition(&shieldsprt, shieldx, shieldy);
    }
    // }
}

void moveBullets()
{
    if (game_paused)
        return;

    // Move Player Bullets
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullet_active[i])
        {
            double rad = bullet_angle[i] * 3.14159 / 180.0;
            bullet_x[i] += 30 * cos(rad);
            bullet_y[i] += 30 * sin(rad);
            iSetSpritePosition(&bullet_sprites[i], bullet_x[i], bullet_y[i]);
            if (bullet_x[i] > SCREEN_WIDTH || bullet_y[i] < -50 || bullet_y[i] > SCREEN_HEIGHT + 50)
            {
                bullet_active[i] = 0;
                bullet_x[i] = -100;
                bullet_y[i] = -100;
                iSetSpritePosition(&bullet_sprites[i], -100, -100);
            }
        }
    }

    // Move Enemy 1 Bullets
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (ebullet_active[i])
        {
            ebullet_x[i] -= 30;
            iSetSpritePosition(&ebulsprite[i], ebullet_x[i], ebullet_y[i]);
            if (ebullet_x[i] < 0)
            {
                ebullet_active[i] = 0;
                ebullet_x[i] = -100;
                ebullet_y[i] = -100;
                iSetSpritePosition(&ebulsprite[i], -100, -100);
            }
        }
    }

    // Move Enemy 2 Bullets
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (e2bullet_active[i])
        {
            e2bullet_x[i] -= 30;
            iSetSpritePosition(&e2bulsprite[i], e2bullet_x[i], e2bullet_y[i]);
            if (e2bullet_x[i] < 0)
            {
                e2bullet_active[i] = 0;
                e2bullet_x[i] = -100;
                e2bullet_y[i] = -100;
                iSetSpritePosition(&e2bulsprite[i], -100, -100);
            }
        }
    }

    // Move Enemy 3 Bullets
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (e3bullet_active[i])
        {
            e3bullet_x[i] -= 30;
            iSetSpritePosition(&e3bulsprite[i], e3bullet_x[i], e3bullet_y[i]);
            if (e3bullet_x[i] < 0)
            {
                e3bullet_active[i] = 0;
                e3bullet_x[i] = -100;
                e3bullet_y[i] = -100;
                iSetSpritePosition(&e3bulsprite[i], -100, -100);
            }
        }
    }

    // Move Enemy 4 Bullets
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (e4bullet_active[i])
        {
            e4bullet_x[i] -= 30;
            iSetSpritePosition(&e4bulsprite[i], e4bullet_x[i], e4bullet_y[i]);
            if (e4bullet_x[i] < 0)
            {
                e4bullet_active[i] = 0;
                e4bullet_x[i] = -100;
                e4bullet_y[i] = -100;
                iSetSpritePosition(&e4bulsprite[i], -100, -100);
            }
        }
    }

    // Move Enemy 5 Bullets
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (e5bullet_active[i])
        {
            e5bullet_x[i] -= 30;
            iSetSpritePosition(&e5bulsprite[i], e5bullet_x[i], e5bullet_y[i]);
            if (e5bullet_x[i] < 0)
            {
                e5bullet_active[i] = 0;
                e5bullet_x[i] = -100;
                e5bullet_y[i] = -100;
                iSetSpritePosition(&e5bulsprite[i], -100, -100);
            }
        }
    }

    // Move Enemy 6 Bullets
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (e6bullet_active[i])
        {
            e6bullet_x[i] += e6bullet_vx[i];
            e6bullet_y[i] += e6bullet_vy[i];
            iSetSpritePosition(&e6bulsprite[i], e6bullet_x[i], e6bullet_y[i]);
            if (e6bullet_x[i] < -50 || e6bullet_y[i] < -50 || e6bullet_y[i] > SCREEN_HEIGHT + 50)
            {
                e6bullet_active[i] = 0;
                e6bullet_x[i] = -100;
                e6bullet_y[i] = -100;
                e6bullet_vx[i] = 0.0;
                e6bullet_vy[i] = 0.0;
                iSetSpritePosition(&e6bulsprite[i], -100, -100);
            }
        }
    }
}

//=============================================================================
// METEOR UPDATE FUNCTION
//=============================================================================

void updatemeteor()
{
    if (game_paused)
        return;

    if (gamestate == ARCADE)
    {
        // Meteor Spawning
        meteor_spawn_timer += 50;
        if (!meteor && meteor_spawn_timer >= 5000)
        {
            meteor = true;
            metx = 100 + (rand() % (SCREEN_WIDTH - 50));
            mety = SCREEN_HEIGHT + 50;
            iSetSpritePosition(&met, metx, mety);
            meteor_spawn_timer = 0;
        }

        // Meteor Movement and Rotation
        if (meteor)
        {
            metx -= 15;
            mety -= 15;
            iSetSpritePosition(&met, metx, mety);

            double rotationCenterX = metx + 45.0;
            double rotationCenterY = mety + 39.3;
            meteorRotationAngle += 10.0;

            if (meteorRotationAngle >= 360.0)
            {
                meteorRotationAngle = 0.0;
            }

            iRotateSprite(&met, rotationCenterX, rotationCenterY, meteorRotationAngle);

            // Remove Meteor if Off Screen
            if (metx < -50 || mety < -70)
            {
                meteor = false;
                meteor_spawn_timer = 0;
            }
        }
    }
    if (gamestate == ENDLESS || gamestate == BOSS)
    {
        // Meteor Spawning
        meteor_spawn_timer += 50;
        if (!meteor && meteor_spawn_timer >= 5000)
        {
            meteor = true;
            metx = 100 + (rand() % (SCREEN_WIDTH - 50));
            mety = SCREEN_HEIGHT + 50;
            iSetSpritePosition(&met, metx, mety);
            meteor_spawn_timer = 0;
        }
        meteor2_spawn_timer += 50;
        if (!meteor2 && meteor2_spawn_timer >= 5000)
        {
            meteor2 = true;
            met2x = 100 + (rand() % (SCREEN_WIDTH - 50));
            met2y = -50;
            iSetSpritePosition(&met2, met2x, met2y);
            meteor2_spawn_timer = 0;
        }
        // Meteor Movement and Rotation
        if (meteor)
        {
            metx -= 15;
            mety -= 15;
            iSetSpritePosition(&met, metx, mety);

            double rotationCenterX = metx + 45.0;
            double rotationCenterY = mety + 39.3;
            meteorRotationAngle += 10.0;

            if (meteorRotationAngle >= 360.0)
            {
                meteorRotationAngle = 0.0;
            }

            iRotateSprite(&met, rotationCenterX, rotationCenterY, meteorRotationAngle);

            // Remove Meteor if Off Screen
            if (metx < -50 || mety < -70)
            {
                meteor = false;
                meteor_spawn_timer = 0;
            }
        }
        if (meteor2)
        {
            met2x -= 15;
            met2y += 15;
            iSetSpritePosition(&met2, met2x, met2y);

            double rotationCenter2X = met2x + 45.0;
            double rotationCenter2Y = met2y + 39.3;
            meteor2RotationAngle += 10.0;

            if (meteor2RotationAngle >= 360.0)
            {
                meteor2RotationAngle = 0.0;
            }

            iRotateSprite(&met2, rotationCenter2X, rotationCenter2Y, meteor2RotationAngle);

            // Remove Meteor if Off Screen
            if (met2x < -50 || met2y > 770)
            {
                meteor2 = false;
                meteor2_spawn_timer = 0;
            }
        }
    }
}

//=============================================================================
// POWER-UP UPDATE FUNCTION
//=============================================================================

void updateBonuses()
{
    if (game_paused)
        return;

    // if (gamestate == 21)
    //{
    // Health Bonus Management
    hpbonus_spawn_timer += 50;
    if (!bonushp && hpbonus_spawn_timer >= 5000)
    {
        bonushp = true;
        bonushpx = getNonOverlappingXPosition(bonusrocketx * bonusrocket, bonusshieldx * bonusshield, 100);
        bonushpy = SCREEN_HEIGHT + 50;
        iSetSpritePosition(&bo_hp, bonushpx, bonushpy);
        hpbonus_spawn_timer = 0;
    }

    if (bonushp)
    {
        bonushpy -= 10;
        iSetSpritePosition(&bo_hp, bonushpx, bonushpy);

        double hprotcenx = bonushpx + 42;
        double hprotceny = bonushpy + 42;
        hprotangle += 10;
        if (hprotangle >= 360)
        {
            hprotangle = 0.0;
        }
        iRotateSprite(&bo_hp, hprotcenx, hprotceny, hprotangle);

        if (bonushpy < -50)
        {
            bonushp = false;
            hpbonus_spawn_timer = 0;
        }
    }

    // Rocket Bonus Management
    rocket_spawn_timer += 50;
    if (!bonusrocket && rocket_spawn_timer >= 4000)
    {
        bonusrocket = true;
        bonusrocketx = getNonOverlappingXPosition(bonushpx * bonushp, bonusshieldx * bonusshield, 100);
        bonusrockety = SCREEN_HEIGHT + 50;
        iSetSpritePosition(&bo_roc, bonusrocketx, bonusrockety);
        rocket_spawn_timer = 0;
    }

    if (bonusrocket)
    {
        bonusrockety -= 10;
        iSetSpritePosition(&bo_roc, bonusrocketx, bonusrockety);

        double rocrotcenx = bonusrocketx + 42;
        double rocrotceny = bonusrockety + 42;
        rockrotangle += 10;
        if (rockrotangle >= 360)
        {
            rockrotangle = 0.0;
        }
        iRotateSprite(&bo_roc, rocrotcenx, rocrotceny, rockrotangle);

        if (bonusrockety < -50)
        {
            bonusrocket = false;
            rocket_spawn_timer = 0;
        }
    }

    // Shield Bonus Management
    shield_spawn_timer += 50;
    if (!bonusshield && shield_spawn_timer >= 9000)
    {
        bonusshield = true;
        bonusshieldx = getNonOverlappingXPosition(bonushpx * bonushp, bonusrocketx * bonusrocket, 100);
        bonusshieldy = SCREEN_HEIGHT + 50;
        iSetSpritePosition(&bo_shi, bonusshieldx, bonusshieldy);
        shield_spawn_timer = 0;
    }

    if (bonusshield)
    {
        bonusshieldy -= 10;
        iSetSpritePosition(&bo_shi, bonusshieldx, bonusshieldy);

        double shirotcenx = bonusshieldx + 42;
        double shirotceny = bonusshieldy + 42;
        shieldrotangle += 10;
        if (shieldrotangle >= 360)
        {
            shieldrotangle = 0.0;
        }
        iRotateSprite(&bo_shi, shirotcenx, shirotceny, shieldrotangle);

        if (bonusshieldy < -50)
        {
            bonusshield = false;
            shield_spawn_timer = 0;
        }
    }

    // Rocket Power-up Duration Check
    if (rocket_powerup_active && glutGet(GLUT_ELAPSED_TIME) > rocket_powerup_end_time)
    {
        rocket_powerup_active = false;
        rocket_powerup_count = 0; // Reset power-up count
        if (rocket_countidx != -1)
            {
                iStopSound(rocket_countidx); // Stop sound when countdown ends
                rocket_countidx = -1;
            }
    }
    //}
}

//=============================================================================
// ENEMY FUNCTIONS
//=============================================================================


void updateEnemy()
{
    if (game_paused)
        return;

    if (gamestate == ARCADE)
    {
        // Enemy 1 Management
        if (!enem1_active && !enem1_exploding)
        {
            if (!enem6_active || enem6hp <= 70)
            { // Only spawn if enem6 is inactive or its HP <= 25
                enem1_respawn_timer += 50;
                int respawn_interval = (enem6_active && enem6hp <= 70) ? 3000 : 1000; // Slower spawn when enem6 HP <= 25
                if (enem1_respawn_timer >= respawn_interval)
                {
                    enem1_active = true;
                    enem1_x = 1200;
                    enem1_y = rand() % SCREEN_HEIGHT;
                    if (enem1_y > SCREEN_HEIGHT - 100)
                        enem1_y = SCREEN_HEIGHT - 250;
                    else if (enem1_y < 50)
                        enem1_y = 50;
                    iSetSpritePosition(&enem1, enem1_x, enem1_y);
                    iChangeSpriteFrames(&enem1, e1idle, 1);
                    enem1_respawn_timer = 0;
                    enem1_fire_timer = 0;
                }
            }
        }

        if (enem1_active && !enem1_exploding)
        {
            enem1_x -= 10;
            iSetSpritePosition(&enem1, enem1_x, enem1_y);
            if (enem1_x < -100)
            {
                enem1_active = false;
            }

            enem1_fire_timer += 50;
            if (enem1_fire_timer >= 1000)
            {
                enem_shoot();
                enem1_fire_timer = 0;
            }
        }

        // Enemy 2 Management
        if (!enem2_active && !enem2_exploding)
        {
            if (!enem6_active || enem6hp <= 70)
            { // Only spawn if enem6 is inactive or its HP <= 25
                enem2_respawn_timer += 50;
                int respawn_interval = (enem6_active && enem6hp <= 70) ? 3000 : 1000; // Slower spawn when enem6 HP <= 25
                if (enem2_respawn_timer >= respawn_interval)
                {
                    enem2_active = true;
                    enem2hp = 4;
                    enem2_x = 1200;
                    if (enem1_y > 350)
                        enem2_y = enem1_y - 400;
                    else
                        enem2_y = enem1_y + 300;
                    if (enem2_y > SCREEN_HEIGHT - 100)
                        enem2_y = SCREEN_HEIGHT - 250;
                    iSetSpritePosition(&enem2, enem2_x, enem2_y);
                    iChangeSpriteFrames(&enem2, e2idle, 1);
                    enem2_respawn_timer = 0;
                    enem2_fire_timer = 0;
                }
            }
        }

        if (enem2_active && !enem2_exploding)
        {
            enem2_x -= 10;
            iSetSpritePosition(&enem2, enem2_x, enem2_y);
            if (enem2_x < -100)
            {
                enem2_active = false;
            }

            enem2_fire_timer += 50;
            if (enem2_fire_timer >= 1000)
            {
                enem_shoot();
                enem2_fire_timer = 0;
            }
        }

        // Enemy 6 Management
    }
    // In updateEnemy function, replace the Enemy 6 spawning block
    if (gamestate == ARCADE || gamestate == BOSS)
    {
        if (!enem6_active && !enem6_exploding && !enem6_spawned && gamestate == ARCADE && scorenumber >= 2000)
        {
            enem6_active = true;
            enem6_spawned = true; // Ensure it only spawns once
            enem6_x = SCREEN_WIDTH - 370;
            enem6_y = SCREEN_HEIGHT + 50;
            iSetSpritePosition(&enem6, enem6_x, enem6_y);
            iChangeSpriteFrames(&enem6, e6idle, 1);
            enem6_respawn_timer = 0;
            enem6_fire_timer = 0;
        }

        if (enem6_active && !enem6_exploding)
        {
            if (enem6_moving_down)
            {
                enem6_y -= 10; // Move down
                if (enem6_y <= -100)
                { // Lower boundary
                    enem6_moving_down = false;
                }
            }
            else
            {
                enem6_y += 10; // Move up
                if (enem6_y >= SCREEN_HEIGHT - 310)
                { // Upper boundary (adjust for sprite size)
                    enem6_moving_down = true;
                }
            }
            iSetSpritePosition(&enem6, enem6_x, enem6_y);

            enem6_fire_timer += 50;
            if (enem6_fire_timer >= 1000)
            {
                enem_shoot();
                enem6_fire_timer = 0;
            }
        }
    }
    if (gamestate == ENDLESS)
    {
        // Wave system
        wave_timer += 50; // Assuming 50ms per frame
        if (wave_timer >= 30000)
        {                                      // 30 seconds per wave
            enemy_wave = (enemy_wave + 1) % 3; // Cycle: 0 (1,2,3), 1 (4,5), 2 (1,2,3)
            wave_timer = 0;
            enem1_respawn_timer = enem2_respawn_timer = enem3_respawn_timer = 0;
            enem4_respawn_timer = enem5_respawn_timer = 0;
        }
        if (enem1_active && !enem1_exploding)
        {
            enem1_x -= 10;
            iSetSpritePosition(&enem1, enem1_x, enem1_y);
            if (enem1_x < -100)
            {
                enem1_active = false;
            }

            enem1_fire_timer += 50;
            if (enem1_fire_timer >= 1000)
            {
                enem_shoot();
                enem1_fire_timer = 0;
            }
        }
        if (enem2_active && !enem2_exploding)
        {
            enem2_x -= 10;
            iSetSpritePosition(&enem2, enem2_x, enem2_y);
            if (enem2_x < -100)
            {
                enem2_active = false;
            }

            enem2_fire_timer += 50;
            if (enem2_fire_timer >= 1000)
            {
                enem_shoot();
                enem2_fire_timer = 0;
            }
        }
        if (enem3_active && !enem3_exploding)
        {
            enem3_x -= 10;
            iSetSpritePosition(&enem3, enem3_x, enem3_y);
            if (enem3_x < -100)
            {
                enem3_active = false;
            }

            enem3_fire_timer += 50;
            if (enem3_fire_timer >= 1000)
            {
                enem_shoot();
                enem3_fire_timer = 0;
            }
        }
        if (enem4_active && !enem4_exploding)
        {
            enem4_x -= 10;
            iSetSpritePosition(&enem4, enem4_x, enem4_y);
            if (enem4_x < -100)
            {
                enem4_active = false;
            }

            enem4_fire_timer += 50;
            if (enem4_fire_timer >= 1000)
            {
                enem_shoot();
                enem4_fire_timer = 0;
            }
        }
        if (enem5_active && !enem5_exploding)
        {
            enem5_x -= 10;
            iSetSpritePosition(&enem5, enem5_x, enem5_y);
            if (enem5_x < -100)
            {
                enem5_active = false;
            }

            enem5_fire_timer += 50;
            if (enem5_fire_timer >= 1000)
            {
                enem_shoot();
                enem5_fire_timer = 0;
            }
        }
        // Wave 0 and 2: Enemies 1, 2, and 3
        if (enemy_wave == 0 || enemy_wave == 2)
        {
            // Enemy 1 Management
            if (!enem1_active && !enem1_exploding)
            {
                enem1_respawn_timer += 50;
                if (enem1_respawn_timer >= 1000)
                {
                    enem1_active = true;
                    enem1_x = 1200;
                    enem1_y = 60 + rand() % 100; // Band: 100 to 199
                    iSetSpritePosition(&enem1, enem1_x, enem1_y);
                    iChangeSpriteFrames(&enem1, e1idle, 1);
                    enem1_respawn_timer = 0;
                    enem1_fire_timer = 0;
                }
            }
            // Enemy 2 Management
            if (!enem2_active && !enem2_exploding)
            {
                enem2_respawn_timer += 50;
                if (enem2_respawn_timer >= 1000)
                {
                    enem2_active = true;
                    enem2hp = 4;
                    enem2_x = 1200;
                    enem2_y = 250 + rand() % 100; // Band: 300 to 399
                    iSetSpritePosition(&enem2, enem2_x, enem2_y);
                    iChangeSpriteFrames(&enem2, e2idle, 1);
                    enem2_respawn_timer = 0;
                    enem2_fire_timer = 0;
                }
            }
            // Enemy 3 Management
            if (!enem3_active && !enem3_exploding)
            {
                enem3_respawn_timer += 50;
                if (enem3_respawn_timer >= 1000)
                {
                    enem3_active = true;
                    enem3hp = 6;
                    enem3_x = 1200;
                    enem3_y = 460 + rand() % 100; // Band: 500 to 599
                    iSetSpritePosition(&enem3, enem3_x, enem3_y);
                    iChangeSpriteFrames(&enem3, e3idle, 1);
                    enem3_respawn_timer = 0;
                    enem3_fire_timer = 0;
                }
            }
        }

        // Wave 1: Enemies 4 and 5
        if (enemy_wave == 1)
        {
            // Enemy 4 Management
            if (!enem4_active && !enem4_exploding)
            {
                enem4_respawn_timer += 50;
                if (enem4_respawn_timer >= 1000)
                {
                    enem4_active = true;
                    enem4hp = 8;
                    enem4_x = 1200;
                    enem4_y = rand() % SCREEN_HEIGHT;
                    if (enem4_y > SCREEN_HEIGHT - 100)
                        enem4_y = SCREEN_HEIGHT - 250;
                    else if (enem4_y < 50)
                        enem4_y = 50;
                    iSetSpritePosition(&enem4, enem4_x, enem4_y);
                    iChangeSpriteFrames(&enem4, e4idle, 1);
                    enem4_respawn_timer = 0;
                    enem4_fire_timer = 0;
                }
            }

            // Enemy 5 Management
            if (!enem5_active && !enem5_exploding)
            {
                enem5_respawn_timer += 50;
                if (enem5_respawn_timer >= 1000)
                {
                    enem5_active = true;
                    enem5hp = 10;
                    enem5_x = 1200;
                    if (enem4_y > 350)
                        enem5_y = enem4_y - 200;
                    else
                        enem5_y = enem4_y + 150;
                    if (enem5_y > SCREEN_HEIGHT - 100)
                        enem5_y = SCREEN_HEIGHT - 250;
                    iSetSpritePosition(&enem5, enem5_x, enem5_y);
                    iChangeSpriteFrames(&enem5, e5idle, 1);
                    enem5_respawn_timer = 0;
                    enem5_fire_timer = 0;
                }
            }
        }
    }
}

void updateEnemyExplosion()
{
    if (game_paused)
        return;
    if (gamestate == ARCADE)
    {
        // Enemy 1 Explosion Animation
        if (enem1_exploding)
        {
            printf("Enemy1 explosion frame: %d\n", enem1_exp_idx);
            enem1_exp_idx++;
            if (enem1_exp_idx < 10)
            {
                iChangeSpriteFrames(&enem1, e1exp, 10);
                enem1.currentFrame = enem1_exp_idx;
                iSetSpritePosition(&enem1, enem1_x, enem1_y);
            }
            else
            {
                printf("Enemy1 explosion complete, deactivating\n");
                enem1_exploding = false;
                enem1_active = false;
                enem1_exp_idx = 0;
                iChangeSpriteFrames(&enem1, e1idle, 1);
                enem1_x = 1200;
                enem1_y = rand() % SCREEN_HEIGHT;
                if (enem1_y > SCREEN_HEIGHT - 100)
                    enem1_y = SCREEN_HEIGHT - 250;
                else if (enem1_y < 50)
                    enem1_y = 50;
                iSetSpritePosition(&enem1, enem1_x, enem1_y);
            }
        }

        // Enemy 2 Explosion Animation
        if (enem2_exploding)
        {
            printf("Enemy2 explosion frame: %d\n", enem2_exp_idx);
            enem2_exp_idx++;
            if (enem2_exp_idx < 12)
            {
                iChangeSpriteFrames(&enem2, e2exp, 12);
                enem2.currentFrame = enem2_exp_idx;
                iSetSpritePosition(&enem2, enem2_x, enem2_y);
            }
            else
            {
                printf("Enemy2 explosion complete, deactivating\n");
                enem2_exploding = false;
                enem2_active = false;
                enem2_exp_idx = 0;
                enem2hp = 4;
                iChangeSpriteFrames(&enem2, e2idle, 1);
                enem2_x = 1200;
                if (enem1_y > 350)
                    enem2_y = enem1_y - 400;
                else
                    enem2_y = enem1_y + 300;
                if (enem2_y > SCREEN_HEIGHT - 100)
                    enem2_y = SCREEN_HEIGHT - 250;
                iSetSpritePosition(&enem2, enem2_x, enem2_y);
            }
        }
    }
    if (gamestate == ENDLESS)
    {
        // Enemy 5 Explosion Animation
        if (enem5_exploding)
        {
            printf("Enemy2 explosion frame: %d\n", enem5_exp_idx);
            enem5_exp_idx++;
            if (enem5_exp_idx < 11)
            {
                iChangeSpriteFrames(&enem5, e5exp, 11);
                enem5.currentFrame = enem5_exp_idx;
                iSetSpritePosition(&enem5, enem5_x - 80, enem5_y - 80);
            }
            else
            {
                printf("Enemy2 explosion complete, deactivating\n");
                enem5_exploding = false;
                enem5_active = false;
                enem5_exp_idx = 0;
                enem5hp = 10;
                iChangeSpriteFrames(&enem5, e5idle, 1);
                enem5_x = 1200;
                if (enem4_y > 350)
                    enem5_y = enem4_y - 400;
                else
                    enem5_y = enem4_y + 300;
                if (enem5_y > SCREEN_HEIGHT - 100)
                    enem5_y = SCREEN_HEIGHT - 250;
                iSetSpritePosition(&enem5, enem5_x, enem5_y);
            }
        }
        // enemy 4
        if (enem4_exploding)
        {
            printf("Enemy1 explosion frame: %d\n", enem4_exp_idx);
            enem4_exp_idx++;
            if (enem4_exp_idx < 11)
            {
                iChangeSpriteFrames(&enem4, e4exp, 11);
                enem4.currentFrame = enem4_exp_idx;
                iSetSpritePosition(&enem4, enem4_x - 80, enem4_y - 80);
            }
            else
            {
                printf("Enemy1 explosion complete, deactivating\n");
                enem4_exploding = false;
                enem4_active = false;
                enem4_exp_idx = 0;
                iChangeSpriteFrames(&enem4, e4idle, 1);
                enem4hp = 8;
                enem4_x = 1200;
                enem4_y = rand() % SCREEN_HEIGHT;
                if (enem4_y > SCREEN_HEIGHT - 100)
                    enem4_y = SCREEN_HEIGHT - 250;
                else if (enem4_y < 50)
                    enem4_y = 50;
                iSetSpritePosition(&enem4, enem4_x, enem4_y);
            }
        }
        if (enem1_exploding)
        {
            printf("Enemy1 explosion frame: %d\n", enem1_exp_idx);
            enem1_exp_idx++;
            if (enem1_exp_idx < 10)
            {
                iChangeSpriteFrames(&enem1, e1exp, 10);
                enem1.currentFrame = enem1_exp_idx;
                iSetSpritePosition(&enem1, enem1_x, enem1_y);
            }
            else
            {
                printf("Enemy1 explosion complete, deactivating\n");
                enem1_exploding = false;
                enem1_active = false;
                enem1_exp_idx = 0;
                iChangeSpriteFrames(&enem1, e1idle, 1);
                enem1_x = 1200;
                enem1_y = 100 + rand() % 100;
                ;
                iSetSpritePosition(&enem1, enem1_x, enem1_y);
            }
        }
        if (enem2_exploding)
        {
            printf("Enemy2 explosion frame: %d\n", enem2_exp_idx);
            enem2_exp_idx++;
            if (enem2_exp_idx < 12)
            {
                iChangeSpriteFrames(&enem2, e2exp, 12);
                enem2.currentFrame = enem2_exp_idx;
                iSetSpritePosition(&enem2, enem2_x, enem2_y);
            }
            else
            {
                printf("Enemy2 explosion complete, deactivating\n");
                enem2_exploding = false;
                enem2_active = false;
                enem2_exp_idx = 0;
                enem2hp = 4;
                iChangeSpriteFrames(&enem2, e2idle, 1);
                enem2_x = 1200;
                enem2_y = 300 + rand() % 100;
                iSetSpritePosition(&enem2, enem2_x, enem2_y);
            }
        }
        if (enem3_exploding)
        {
            printf("Enemy2 explosion frame: %d\n", enem2_exp_idx);
            enem3_exp_idx++;
            if (enem3_exp_idx < 11)
            {
                iChangeSpriteFrames(&enem3, e3exp, 11);
                enem3.currentFrame = enem3_exp_idx;
                iSetSpritePosition(&enem3, enem3_x - 80, enem3_y - 80);
            }
            else
            {
                printf("Enemy2 explosion complete, deactivating\n");
                enem3_exploding = false;
                enem3_active = false;
                enem3_exp_idx = 0;
                enem3hp = 6;
                iChangeSpriteFrames(&enem3, e3idle, 1);
                enem3_x = 1200;
                enem3_y = 500 + rand() % 100;
                iSetSpritePosition(&enem3, enem3_x, enem3_y);
            }
        }
    }
}
void bossexplosion()
{
    if (game_paused)
        return;
    if (enem6_exploding)
    {
        printf("Enemy6 explosion frame: %d\n", enem6_exp_idx);
        enem6_exp_idx++;
        if (enem6_exp_idx < 11)
        {
            iChangeSpriteFrames(&enem6, e6exp, 11);
            enem6.currentFrame = enem6_exp_idx;
            iSetSpritePosition(&enem6, enem6_x - 50 , enem6_y - 50);
        }
        else
        {
            printf("Enemy6 explosion complete, transitioning to game over\n");
            enem6_exploding = false;
            enem6_active = false;
            enem6_exp_idx = 0;
            enem6hp = 300;
            bosshp = 600;

            iChangeSpriteFrames(&enem6, e6idle, 1);
            enem6_x = SCREEN_WIDTH - 370;
            enem6_y = SCREEN_HEIGHT + 50;
            iSetSpritePosition(&enem6, enem6_x, enem6_y);
            prev_gamestate = gamestate;
            

            gamestate = GAMEOVER; // Transition to game over
        }
    }
}
//=============================================================================
// COLLISION DETECTION FUNCTIONS
//=============================================================================

void checkshieldcollision()
{
    if (game_paused)
        return;

    if (shield_active)
    {
        // Shield vs Enemy Bullets
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (ebullet_active[i])
            {
                if (iCheckCollision(&shieldsprt, &ebulsprite[i]))
                {
                    ebullet_active[i] = 0;
                    shield_hp--;
                    if (shield_hp <= 0)
                    {   iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                        shield_active = false;
                        shield_hp = 3;
                        iSetSpritePosition(&shieldsprt, -100, -100);
                    }
                }
            }
        }

        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (e2bullet_active[i])
            {
                if (iCheckCollision(&shieldsprt, &e2bulsprite[i]))
                {
                    e2bullet_active[i] = 0;
                    shield_hp--;
                    if (shield_hp <= 0)
                    {iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                        shield_active = false;
                        shield_hp = 3;
                        iSetSpritePosition(&shieldsprt, -100, -100);
                    }
                }
            }
        }
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (e3bullet_active[i])
            {
                if (iCheckCollision(&shieldsprt, &e3bulsprite[i]))
                {
                    e3bullet_active[i] = 0;
                    shield_hp--;
                    if (shield_hp <= 0)
                    {iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                        shield_active = false;
                        shield_hp = 3;
                        iSetSpritePosition(&shieldsprt, -100, -100);
                    }
                }
            }
        }
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (e4bullet_active[i])
            {
                if (iCheckCollision(&shieldsprt, &e4bulsprite[i]))
                {
                    e4bullet_active[i] = 0;
                    shield_hp--;
                    if (shield_hp <= 0)
                    {iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                        shield_active = false;
                        shield_hp = 3;
                        iSetSpritePosition(&shieldsprt, -100, -100);
                    }
                }
            }
        }
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (e5bullet_active[i])
            {
                if (iCheckCollision(&shieldsprt, &e5bulsprite[i]))
                {
                    e5bullet_active[i] = 0;
                    shield_hp--;
                    if (shield_hp <= 0)
                    {iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                        shield_active = false;
                        shield_hp = 3;
                        iSetSpritePosition(&shieldsprt, -100, -100);
                    }
                }
            }
        }
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (e6bullet_active[i])
            {
                if (iCheckCollision(&shieldsprt, &e6bulsprite[i]))
                {
                    e6bullet_active[i] = 0;
                    shield_hp--;
                    if (shield_hp <= 0)
                    {iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                        shield_active = false;
                        shield_hp = 3;
                        iSetSpritePosition(&shieldsprt, -100, -100);
                    }
                }
            }
        }

        // Shield vs Meteor
        if (iCheckCollision(&shieldsprt, &met))
        {
            meteor = false;
            iPlaySound("assets/sounds/rock.wav", false, 100);
            iSetSpritePosition(&met, -100, -100);
            meteor_spawn_timer = 0;
            shield_hp -= 3;
            if (shield_hp <= 0)
            {iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                shield_active = false;
                shield_hp = 3;
                iSetSpritePosition(&shieldsprt, -100, -100);
            }
        }
        if (iCheckCollision(&shieldsprt, &met2))
        {
            meteor2 = false;
            iPlaySound("assets/sounds/rock.wav", false, 100);
            iSetSpritePosition(&met2, -100, -100);
            meteor2_spawn_timer = 0;
            shield_hp -= 3;
            if (shield_hp <= 0)
            {iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                shield_active = false;
                shield_hp = 3;
                iSetSpritePosition(&shieldsprt, -100, -100);
            }
        }
        // Shield vs Enemies
        if (iCheckCollision(&shieldsprt, &enem1))
        {
            enem1_active = false;
            enem1_exploding = true;
            enem1_exp_idx = 0;
            iChangeSpriteFrames(&enem1, e1exp, 10);
            shield_hp -= 3;
            if (shield_hp <= 0)
            {iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                shield_active = false;
                shield_hp = 3;
                iSetSpritePosition(&shieldsprt, -100, -100);
            }
        }

        if (iCheckCollision(&shieldsprt, &enem2))
        {
            enem2_active = false;
            enem2_exploding = true;
            enem2_exp_idx = 0;
            iChangeSpriteFrames(&enem2, e2exp, 12);
            shield_hp -= 3;
            if (shield_hp <= 0)
            {iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                shield_active = false;
                shield_hp = 3;
                iSetSpritePosition(&shieldsprt, -100, -100);
            }
        }
        if (iCheckCollision(&shieldsprt, &enem3))
        {
            enem3_active = false;
            enem3_exploding = true;
            enem3_exp_idx = 0;
            iChangeSpriteFrames(&enem3, e3exp, 11);
            shield_hp -= 3;
            if (shield_hp <= 0)
            {iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                shield_active = false;
                shield_hp = 3;
                iSetSpritePosition(&shieldsprt, -100, -100);
            }
        }

        if (iCheckCollision(&shieldsprt, &enem4))
        {
            enem4_active = false;
            enem4_exploding = true;
            enem4_exp_idx = 0;
            iChangeSpriteFrames(&enem4, e4exp, 11);
            shield_hp -= 3;
            if (shield_hp <= 0)
            {iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                shield_active = false;
                shield_hp = 3;
                iSetSpritePosition(&shieldsprt, -100, -100);
            }
        }
        if (iCheckCollision(&shieldsprt, &enem5))
        {
            enem5_active = false;
            enem5_exploding = true;
            enem5_exp_idx = 0;
            iChangeSpriteFrames(&enem5, e5exp, 11);
            shield_hp -= 3;
            if (shield_hp <= 0)
            {iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                shield_active = false;
                shield_hp = 3;
                iSetSpritePosition(&shieldsprt, -100, -100);
            }
        }
        if (iCheckCollision(&shieldsprt, &enem6))
        {
            enem6_active = false;
            enem6_exploding = true;
            enem6_exp_idx = 0;
            iChangeSpriteFrames(&enem6, e6exp, 11);
            shield_hp -= 3;
            if (shield_hp <= 0)
            {iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                shield_active = false;
                shield_hp = 3;
                iSetSpritePosition(&shieldsprt, -100, -100);
            }
        }
    }
}

void checkEnemSpaceCollision()
{
    if (just_reset)
        return;
    if (game_paused)
        return;

    // if (gamestate == 21)
    // {
    // Spaceship vs Enemy 1
    if (iCheckCollision(&spaceship, &enem1) && !shield_active && !shipexp)
    {
        printf("Collision with enemy1 at spaceship (%d, %d), enemy1 (%d, %d)\n", move_lf, move_ud, enem1_x, enem1_y);
        enem1_active = false;
        enem1_exploding = true;
        iPlaySound("assets/sounds/enemyexplosion.wav", false, 100);
        enem1_exp_idx = 0;
        iChangeSpriteFrames(&enem1, e1exp, 10);
        shipexp = true;
        iPlaySound("assets/sounds/shipexplosion.wav", false, 100);
        ship_state = EXP;
        exp_idx = 0;
        iChangeSpriteFrames(&spaceship, s_exp, 7);
        spaceship.currentFrame = 0;
    }

    // Spaceship vs Enemy 2
    if (iCheckCollision(&spaceship, &enem2) && !shield_active && !shipexp)
    {
        printf("Collision with enemy2 at spaceship (%d, %d), enemy2 (%d, %d)\n", move_lf, move_ud, enem2_x, enem2_y);
        enem2_active = false;
        enem2_exploding = true;
        iPlaySound("assets/sounds/enemyexplosion.wav", false, 100);
        enem2_exp_idx = 0;
        iChangeSpriteFrames(&enem2, e2exp, 12);
        shipexp = true;
        iPlaySound("assets/sounds/shipexplosion.wav", false, 100);
        ship_state = EXP;
        exp_idx = 0;
        iChangeSpriteFrames(&spaceship, s_exp, 7);
        spaceship.currentFrame = 0;
    }

    // Spaceship vs Enemy 6
    if (iCheckCollision(&spaceship, &enem6) && !shield_active && !shipexp)
    {
        printf("Collision with enemy6 at spaceship (%d, %d), enemy6 (%d, %d)\n", move_lf, move_ud, enem6_x, enem6_y);
        enem6_active = false;
        enem6_exploding = true;
        iPlaySound("assets/sounds/explosionboss.wav", false, 100);
        enem6_exp_idx = 0;
        iChangeSpriteFrames(&enem6, e6exp, 11);
        shipexp = true;
         iPlaySound("assets/sounds/shipexplosion.wav", false, 100);
        ship_state = EXP;
        exp_idx = 0;
        iChangeSpriteFrames(&spaceship, s_exp, 7);
        spaceship.currentFrame = 0;
        gamestate = GAMEOVER;
    }
    if (iCheckCollision(&spaceship, &enem3) && !shield_active && !shipexp)
    {
        printf("Collision with enemy3 at spaceship (%d, %d), enemy3 (%d, %d)\n", move_lf, move_ud, enem1_x, enem1_y);
        enem3_active = false;
        enem3_exploding = true;
        iPlaySound("assets/sounds/enemyexplosion.wav", false, 100);
        enem3_exp_idx = 0;
        iChangeSpriteFrames(&enem3, e3exp, 11);
        shipexp = true;
         iPlaySound("assets/sounds/shipexplosion.wav", false, 100);
        ship_state = EXP;
        exp_idx = 0;
        iChangeSpriteFrames(&spaceship, s_exp, 7);
        spaceship.currentFrame = 0;
    }

    // Spaceship vs Enemy 4
    if (iCheckCollision(&spaceship, &enem4) && !shield_active && !shipexp)
    {
        printf("Collision with enemy4 at spaceship (%d, %d), enemy4 (%d, %d)\n", move_lf, move_ud, enem2_x, enem2_y);
        enem4_active = false;
        enem4_exploding = true;
        iPlaySound("assets/sounds/enemyexplosion.wav", false, 100);
        enem4_exp_idx = 0;
        iChangeSpriteFrames(&enem4, e4exp, 11);
        shipexp = true;
         iPlaySound("assets/sounds/shipexplosion.wav", false, 100);
        ship_state = EXP;
        exp_idx = 0;
        iChangeSpriteFrames(&spaceship, s_exp, 7);
        spaceship.currentFrame = 0;
    }

    // Spaceship vs Enemy 5
    if (iCheckCollision(&spaceship, &enem5) && !shield_active && !shipexp)
    {
        printf("Collision with enemy5 at spaceship (%d, %d), enemy5 (%d, %d)\n", move_lf, move_ud, enem6_x, enem6_y);
        enem5_active = false;
        enem5_exploding = true;
        iPlaySound("assets/sounds/enemyexplosion.wav", false, 100);
        enem5_exp_idx = 0;
        iChangeSpriteFrames(&enem5, e5exp, 11);
        shipexp = true;
         iPlaySound("assets/sounds/shipexplosion.wav", false, 100);
        ship_state = EXP;
        exp_idx = 0;
        iChangeSpriteFrames(&spaceship, s_exp, 7);
        spaceship.currentFrame = 0;
    }

    // Spaceship vs Meteor
    if (meteor && iCheckCollision(&spaceship, &met) && !shield_active && !shipexp)
    {
        printf("Collision with meteor at spaceship (%d, %d), meteor (%d, %d)\n", move_lf, move_ud, metx, mety);
        iPlaySound("rock.wav", false, 100);
        meteor = false;
        meteor_spawn_timer = 0;
        metx = 550;
        mety = 1000;
        iSetSpritePosition(&met, metx, mety);
        shipexp = true;
         iPlaySound("assets/sounds/shipexplosion.wav", false, 100);
        ship_state = EXP;
        exp_idx = 0;
        iChangeSpriteFrames(&spaceship, s_exp, 7);
        spaceship.currentFrame = 0;
    }
    if (meteor2 && iCheckCollision(&spaceship, &met2) && !shield_active && !shipexp)
    {
        printf("Collision with meteor at spaceship (%d, %d), meteor (%d, %d)\n", move_lf, move_ud, metx, mety);
        iPlaySound("rock.wav", false, 100);
        meteor2 = false;
        meteor2_spawn_timer = 0;
        met2x = 550;
        met2y = -300;
        iSetSpritePosition(&met2, met2x, met2y);
        shipexp = true;
         iPlaySound("assets/sounds/shipexplosion.wav", false, 100);
        ship_state = EXP;
        exp_idx = 0;
        iChangeSpriteFrames(&spaceship, s_exp, 7);
        spaceship.currentFrame = 0;
    }
    // Spaceship vs Power-ups
    if (bonushp && iCheckCollision(&spaceship, &bo_hp))
    {   iPlaySound("assets/sounds/powerup.mp3", false, 100);
        if (health < 3)
        {
            health++;
        }
        bonushp = false;
        hpbonus_spawn_timer = 0;
    }

    if (bonusrocket && iCheckCollision(&spaceship, &bo_roc))
    {   iPlaySound("assets/sounds/powerup.mp3", false, 100);
        rocket_powerup_active = true;
        rocket_powerup_count++; // Increment power-up count
        rocket_powerup_start_time = glutGet(GLUT_ELAPSED_TIME);
        rocket_powerup_end_time = rocket_powerup_start_time + 10000;
        bonusrocket = false;
        rocket_spawn_timer = 0;
    }

    if (bonusshield && iCheckCollision(&spaceship, &bo_shi))
    {   iPlaySound("assets/sounds/powerup.mp3", false, 100);
        shield_active = true;
        shieldx = move_lf - 5;
        shieldy = move_ud - 5;
        iSetSpritePosition(&shieldsprt, shieldx, shieldy);
        bonusshield = false;
        shield_spawn_timer = 0;
    }
    //}
}

void checkBulletEnemyCollision()
{
    if (just_reset)
        return;
    if (game_paused)
        return;

    if (gamestate == ARCADE)
    {
        // Player Bullets vs Enemy 1
        if (enem1_active && !enem1_exploding)
        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullet_active[i])
                {
                    if (iCheckCollision(&bullet_sprites[i], &enem1))
                    {
                        bullet_active[i] = 0;
                        enem1_active = false;
                        enem1_exploding = true;
                        iPlaySound("assets/sounds/enemyexplosion.wav", false, 100);
                        scorenumber += 100;
                        sprintf(scoretext, "%d", scorenumber);
                        enem1_exp_idx = 0;
                        iChangeSpriteFrames(&enem1, e1exp, 10);
                        iSetSpritePosition(&enem1, enem1_x, enem1_y);
                    }
                }
            }
        }

        // Player Bullets vs Enemy 2
        if (enem2_active && !enem2_exploding)
        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullet_active[i])
                {
                    if (iCheckCollision(&bullet_sprites[i], &enem2))
                    {
                        bullet_active[i] = 0;
                        enem2hp--;
                        if (enem2hp <= 0)
                        {
                            enem2_active = false;
                            enem2_exploding = true;
                            iPlaySound("assets/sounds/enemyexplosion.wav", false, 100);
                            scorenumber += 100;
                            sprintf(scoretext, "%d", scorenumber);
                            enem2_exp_idx = 0;
                            iChangeSpriteFrames(&enem2, e2exp, 12);
                            iSetSpritePosition(&enem2, enem2_x, enem2_y);
                        }
                    }
                }
            }
        }
    }
    if (gamestate == ARCADE || gamestate == BOSS)
    { // Player Bullets vs Enemy 6
        if (enem6_active && !enem6_exploding)
        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullet_active[i])
                {
                    if (iCheckCollision(&bullet_sprites[i], &enem6))
                    {
                        bullet_active[i] = 0;
                        if (gamestate == ARCADE)
                            enem6hp--;
                        else if (gamestate == BOSS)
                            bosshp--;
                        if (gamestate == BOSS)
                        {
                            scorenumber += 5;
                            sprintf(scoretext, "%d", scorenumber);
                        }
                        if (enem6hp <= 0 || bosshp <= 0)
                        {
                            enem6_active = false;
                            enem6_exploding = true;
                            iPlaySound("assets/sounds/explosionboss.wav", false, 100);
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

        // Player Bullets vs Meteor
        if (meteor)
        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullet_active[i])
                {
                    if (iCheckCollision(&bullet_sprites[i], &met))
                    {
                        printf("Bullet hit meteor at bullet (%d, %d), meteor (%d, %d)\n", bullet_x[i], bullet_y[i], metx, mety);
                        bullet_active[i] = 0;
                        bullet_x[i] = -100;
                        bullet_y[i] = -100;
                        iSetSpritePosition(&bullet_sprites[i], -100, -100);
                        //meteor = false;
                        iPlaySound("rock.wav", false, 100);
                        meteor=false;
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
    else if (gamestate == ENDLESS)
    {
        // Player Bullets vs Enemy 1
        if (enem1_active && !enem1_exploding)
        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullet_active[i])
                {
                    if (iCheckCollision(&bullet_sprites[i], &enem1))
                    {
                        bullet_active[i] = 0;
                        enem1_active = false;
                        enem1_exploding = true;
                        iPlaySound("assets/sounds/enemyexplosion.wav", false, 100);
                        scorenumber += 100;
                        sprintf(scoretext, "%d", scorenumber);
                        enem1_exp_idx = 0;
                        iChangeSpriteFrames(&enem1, e1exp, 10);
                        iSetSpritePosition(&enem1, enem1_x, enem1_y);
                    }
                }
            }
        }

        // Player Bullets vs Enemy 2
        if (enem2_active && !enem2_exploding)
        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullet_active[i])
                {
                    if (iCheckCollision(&bullet_sprites[i], &enem2))
                    {
                        bullet_active[i] = 0;
                        enem2hp--;
                        if (enem2hp <= 0)
                        {
                            enem2_active = false;
                            enem2_exploding = true;
                            iPlaySound("assets/sounds/enemyexplosion.wav", false, 100);
                            scorenumber += 100;
                            sprintf(scoretext, "%d", scorenumber);
                            enem2_exp_idx = 0;
                            iChangeSpriteFrames(&enem2, e2exp, 12);
                            iSetSpritePosition(&enem2, enem2_x, enem2_y);
                        }
                    }
                }
            }
        }
        if (enem3_active && !enem3_exploding)
        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullet_active[i])
                {
                    if (iCheckCollision(&bullet_sprites[i], &enem3))
                    {
                        bullet_active[i] = 0;
                        enem3hp--;
                        if (enem3hp <= 0)
                        {
                            enem3_active = false;
                            enem3_exploding = true;
                            iPlaySound("assets/sounds/enemyexplosion.wav", false, 100);
                            scorenumber += 100;
                            sprintf(scoretext, "%d", scorenumber);
                            enem3_exp_idx = 0;
                            iChangeSpriteFrames(&enem3, e3exp, 11);
                            iSetSpritePosition(&enem3, enem3_x, enem3_y);
                        }
                    }
                }
            }
        }
        if (enem4_active && !enem4_exploding)
        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullet_active[i])
                {
                    if (iCheckCollision(&bullet_sprites[i], &enem4))
                    {
                        bullet_active[i] = 0;
                        enem4hp--;
                        if (enem4hp <= 0)
                        {
                            enem4_active = false;
                            enem4_exploding = true;
                            iPlaySound("assets/sounds/enemyexplosion.wav", false, 100);
                            scorenumber += 200;
                            sprintf(scoretext, "%d", scorenumber);
                            enem4_exp_idx = 0;
                            iChangeSpriteFrames(&enem4, e4exp, 11);
                            iSetSpritePosition(&enem4, enem4_x, enem4_y);
                        }
                    }
                }
            }
        }
        if (enem5_active && !enem5_exploding && !enem1_active && !enem1_exploding && !enem2_active && !enem2_exploding && !enem3_active && !enem3_exploding)
        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullet_active[i])
                {
                    if (iCheckCollision(&bullet_sprites[i], &enem5))
                    {
                        bullet_active[i] = 0;
                        enem5hp--;
                        if (enem5hp <= 0)
                        {
                            enem5_active = false;
                            enem5_exploding = true;
                            iPlaySound("assets/sounds/enemyexplosion.wav", false, 100);
                            scorenumber += 200;
                            sprintf(scoretext, "%d", scorenumber);
                            enem5_exp_idx = 0;
                            iChangeSpriteFrames(&enem5, e5exp, 11);
                            iSetSpritePosition(&enem5, enem5_x, enem5_y);
                        }
                    }
                }
            }
        }
        if (meteor)
        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullet_active[i])
                {
                    if (iCheckCollision(&bullet_sprites[i], &met))
                    {
                        // printf("Bullet hit meteor at bullet (%d, %d), meteor (%d, %d)\n", bullet_x[i], bullet_y[i], metx, mety);
                        bullet_active[i] = 0;
                        bullet_x[i] = -100;
                        bullet_y[i] = -100;
                        iSetSpritePosition(&bullet_sprites[i], -100, -100);
                        //meteor = false;
                        iPlaySound("rock.wav", false, 100);
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
        if (meteor2)
        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullet_active[i])
                {
                    if (iCheckCollision(&bullet_sprites[i], &met2))
                    {
                        printf("Bullet hit meteor at bullet (%d, %d), meteor (%d, %d)\n", bullet_x[i], bullet_y[i], metx, mety);
                        bullet_active[i] = 0;
                        bullet_x[i] = -100;
                        bullet_y[i] = -100;
                        iSetSpritePosition(&bullet_sprites[i], -100, -100);
                       // meteor2 = false;
                        iPlaySound("rock.wav", false, 100);
                        meteor2 = false;  
                        meteor2_spawn_timer = 0;
                        met2x = 550;
                        met2y = -300;
                        iSetSpritePosition(&met2, met2x, met2y);
                        scorenumber += 50;
                        sprintf(scoretext, "%d", scorenumber);
                    }
                }
            }
        }
        
    }
}

void enemBulletCollision()
{
    if (just_reset)
        return;
    if (game_paused)
        return;

    // if (gamestate == 21)
    // {
    int current_time = glutGet(GLUT_ELAPSED_TIME);
    if (current_time < invincibility_end_time)
    {
        return;
    }

    bool hit_this_frame = false;
    if(gamestate==ARCADE){
    // Enemy 1 Bullets vs Spaceship
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (ebullet_active[i] && iCheckCollision(&spaceship, &ebulsprite[i]) && !shipexp)
        {
            printf("Enemy1 bullet %d collision at spaceship (%d, %d), bullet (%d, %d)\n", i, move_lf, move_ud, ebullet_x[i], ebullet_y[i]);
            ebullet_active[i] = 0;
            ebullet_x[i] = -100;
            ebullet_y[i] = -100;
            iSetSpritePosition(&ebulsprite[i], ebullet_x[i], ebullet_y[i]);

            if (!shield_active)
            {
                health--;
            }
            else
            {
                shield_hp--;
                if (shield_hp <= 0)
                {   iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                    shield_active = false;
                    shield_hp = 3;
                    iSetSpritePosition(&shieldsprt, -100, -100);
                }
            }
            hit_this_frame = true;
            break;
        }
    }

    // Enemy 2 Bullets vs Spaceship
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (e2bullet_active[i] && iCheckCollision(&spaceship, &e2bulsprite[i]) && !shipexp)
        {
            printf("Enemy2 bullet %d collision at spaceship (%d, %d), bullet (%d, %d)\n", i, move_lf, move_ud, e2bullet_x[i], e2bullet_y[i]);
            e2bullet_active[i] = 0;
            e2bullet_x[i] = -100;
            e2bullet_y[i] = -100;
            iSetSpritePosition(&e2bulsprite[i], -100, -100);

            if (!shield_active)
            {
                health--;
            }
            else
            {
                shield_hp--;
                if (shield_hp <= 0)
                {   iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                    shield_active = false;
                    shield_hp = 3;
                    iSetSpritePosition(&shieldsprt, -100, -100);
                }
            }
            hit_this_frame = true;
            break;
        }
    }
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (e6bullet_active[i] && iCheckCollision(&spaceship, &e6bulsprite[i]) && !shipexp)
        {
            printf("Enemy6 bullet %d collision at spaceship (%d, %d), bullet (%d, %d)\n", i, move_lf, move_ud, e6bullet_x[i], e6bullet_y[i]);
            e6bullet_active[i] = 0;
            e6bullet_x[i] = -100;
            e6bullet_y[i] = -100;
            iSetSpritePosition(&e6bulsprite[i], -100, -100);

            if (!shield_active)
            {
                health--;
            }
            else
            {
                shield_hp--;
                if (shield_hp <= 0)
                {   iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                    shield_active = false;
                    shield_hp = 3;
                    iSetSpritePosition(&shieldsprt, -100, -100);
                }
            }
            hit_this_frame = true;
            break;
        }
    }
   }else if(gamestate==ENDLESS){
    // Enemy 1 Bullets vs Spaceship
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (ebullet_active[i] && iCheckCollision(&spaceship, &ebulsprite[i]) && !shipexp)
        {
            printf("Enemy1 bullet %d collision at spaceship (%d, %d), bullet (%d, %d)\n", i, move_lf, move_ud, ebullet_x[i], ebullet_y[i]);
            ebullet_active[i] = 0;
            ebullet_x[i] = -100;
            ebullet_y[i] = -100;
            iSetSpritePosition(&ebulsprite[i], -100, -100);

            if (!shield_active)
            {
                health--;
            }
            else
            {
                shield_hp--;
                if (shield_hp <= 0)
                {   iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                    shield_active = false;
                    shield_hp = 3;
                    iSetSpritePosition(&shieldsprt, -100, -100);
                }
            }
            hit_this_frame = true;
            break;
        }
    }

    // Enemy 2 Bullets vs Spaceship
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (e2bullet_active[i] && iCheckCollision(&spaceship, &e2bulsprite[i]) && !shipexp)
        {
            printf("Enemy2 bullet %d collision at spaceship (%d, %d), bullet (%d, %d)\n", i, move_lf, move_ud, e2bullet_x[i], e2bullet_y[i]);
            e2bullet_active[i] = 0;
            e2bullet_x[i] = -100;
            e2bullet_y[i] = -100;
            iSetSpritePosition(&e2bulsprite[i], -100, -100);

            if (!shield_active)
            {
                health--;
            }
            else
            {
                shield_hp--;
                if (shield_hp <= 0)
                {  iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                    shield_active = false;
                    shield_hp = 3;
                    iSetSpritePosition(&shieldsprt, -100, -100);
                }
            }
            hit_this_frame = true;
            break;
        }
    }
    // enemy3
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (e3bullet_active[i] && iCheckCollision(&spaceship, &e3bulsprite[i]) && !shipexp)
        {
            printf("Enemy3 bullet %d collision at spaceship (%d, %d), bullet (%d, %d)\n", i, move_lf, move_ud, e2bullet_x[i], e2bullet_y[i]);
            e3bullet_active[i] = 0;
            e3bullet_x[i] = -100;
            e3bullet_y[i] = -100;
            iSetSpritePosition(&e3bulsprite[i], -100, -100);

            if (!shield_active)
            {
                health--;
            }
            else
            {
                shield_hp--;
                if (shield_hp <= 0)
                {   iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                    shield_active = false;
                    shield_hp = 3;
                    iSetSpritePosition(&shieldsprt, -100, -100);
                }
            }
            hit_this_frame = true;
            break;
        }
    }
    // enemy 4
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (e4bullet_active[i] && iCheckCollision(&spaceship, &e4bulsprite[i]) && !shipexp)
        {
            printf("Enemy4 bullet %d collision at spaceship (%d, %d), bullet (%d, %d)\n", i, move_lf, move_ud, e2bullet_x[i], e2bullet_y[i]);
            e4bullet_active[i] = 0;
            e4bullet_x[i] = -100;
            e4bullet_y[i] = -100;
            iSetSpritePosition(&e4bulsprite[i], -100, -100);

            if (!shield_active)
            {
                health--;
            }
            else
            {
                shield_hp--;
                if (shield_hp <= 0)
                {   iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                    shield_active = false;
                    shield_hp = 3;
                    iSetSpritePosition(&shieldsprt, -100, -100);
                }
            }
            hit_this_frame = true;
            break;
        }
    }
    // enemy 5
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (e5bullet_active[i] && iCheckCollision(&spaceship, &ebulsprite[i]) && !shipexp)
        {
            printf("Enemy5 bullet %d collision at spaceship (%d, %d), bullet (%d, %d)\n", i, move_lf, move_ud, e2bullet_x[i], e2bullet_y[i]);
            e5bullet_active[i] = 0;
            e5bullet_x[i] = -100;
            e5bullet_y[i] = -100;
            iSetSpritePosition(&e5bulsprite[i], -100, -100);

            if (!shield_active)
            {
                health--;
            }
            else
            {
                shield_hp--;
                if (shield_hp <= 0)
                {   iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                    shield_active = false;
                    shield_hp = 3;
                    iSetSpritePosition(&shieldsprt, -100, -100);
                }
            }
            hit_this_frame = true;
            break;
        }
    }}else if(gamestate==BOSS){
    // Enemy 6 Bullets vs Spaceship
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (e6bullet_active[i] && iCheckCollision(&spaceship, &e6bulsprite[i]) && !shipexp)
        {
            printf("Enemy6 bullet %d collision at spaceship (%d, %d), bullet (%d, %d)\n", i, move_lf, move_ud, e6bullet_x[i], e6bullet_y[i]);
            e6bullet_active[i] = 0;
            e6bullet_x[i] = -100;
            e6bullet_y[i] = -100;
            iSetSpritePosition(&e6bulsprite[i], -100, -100);

            if (!shield_active)
            {
                health--;
            }
            else
            {
                shield_hp--;
                if (shield_hp <= 0)
                {   iPlaySound("assets/sounds/shieldbreak.wav", false, 100);
                    shield_active = false;
                    shield_hp = 3;
                    iSetSpritePosition(&shieldsprt, -100, -100);
                }
            }
            hit_this_frame = true;
            break;
        }
    }
}

    // Apply Invincibility and Check Health
    if (hit_this_frame)
    {
        invincibility_end_time = current_time + 500;
        if (health <= 0)
        {
            printf("Health <= 0, triggering explosion\n");
            shipexp = true;
            iPlaySound("assets/sounds/shipexplosion.wav", false, 100);
            ship_state = EXP;
            exp_idx = 0;
            iChangeSpriteFrames(&spaceship, s_exp, 7);
            spaceship.currentFrame = 0;
        }
    }
    // }
}
void enem_shoot()
{
    if (game_paused)
        return;

    if (gamestate == ARCADE)
    {
        // Enemy 1 Shooting
        if (enem1_active && !enem1_exploding)
        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (!ebullet_active[i])
                {
                    ebullet_x[i] = enem1_x - 10;
                    ebullet_y[i] = enem1_y + 40;
                    iSetSpritePosition(&ebulsprite[i], ebullet_x[i], ebullet_y[i]);
                    ebullet_active[i] = 1;
                    break;
                }
            }
        }

        // Enemy 2 Shooting
        if (enem2_active && !enem2_exploding)
        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (!e2bullet_active[i])
                {
                    e2bullet_x[i] = enem2_x - 10;
                    e2bullet_y[i] = enem2_y + 70;
                    iSetSpritePosition(&e2bulsprite[i], e2bullet_x[i], e2bullet_y[i]);
                    e2bullet_active[i] = 1;
                    break;
                }
            }
        }
    }
    // Enemy 6 Shooting
    if (gamestate == ARCADE || gamestate == BOSS)
    {
        if (enem6_active && !enem6_exploding)
        {
            if (gamestate == 21 && enem6hp <= 60) // 20% of 300
            {
                int num_bullets = 3;
                double angles[3] = {-0.261799, 0.0, 0.261799}; // -15°, 0°, +15° in radians
                double bullet_speed = 30.0;
                int fired = 0;
                for (int i = 0; i < MAX_BULLETS && fired < num_bullets; i++)
                {
                    if (!e6bullet_active[i])
                    {
                        e6bullet_x[i] = enem6_x - 10;
                        e6bullet_y[i] = enem6_y + 70;
                        e6bullet_vx[i] = -bullet_speed * cos(angles[fired]);
                        e6bullet_vy[i] = bullet_speed * sin(angles[fired]);
                        iSetSpritePosition(&e6bulsprite[i], e6bullet_x[i], e6bullet_y[i]);
                        e6bullet_active[i] = 1;
                        fired++;
                    }
                }
            }
            else if (gamestate == BOSS && bosshp <= 200) // 33.33% of 600
            {
                int num_bullets = 3;
                double angles[3] = {-0.261799, 0.0, 0.261799}; // -15°, 0°, +15° in radians
                double bullet_speed = 30.0;
                int fired = 0;
                for (int i = 0; i < MAX_BULLETS && fired < num_bullets; i++)
                {
                    if (!e6bullet_active[i])
                    {
                        e6bullet_x[i] = enem6_x - 10;
                        e6bullet_y[i] = enem6_y + 70;
                        e6bullet_vx[i] = -bullet_speed * cos(angles[fired]);
                        e6bullet_vy[i] = bullet_speed * sin(angles[fired]);
                        iSetSpritePosition(&e6bulsprite[i], e6bullet_x[i], e6bullet_y[i]);
                        e6bullet_active[i] = 1;
                        fired++;
                    }
                }
            }
            else if (gamestate == BOSS && bosshp <= 400) // 66.67% of 600
            {
                int num_bullets = 5;
                int y_offsets[5] = {70 + 100, 70 + 50, 70, 70 - 50, 70 - 100}; // Parallel bullets
                double bullet_speed = 30.0;
                int fired = 0;
                for (int i = 0; i < MAX_BULLETS && fired < num_bullets; i++)
                {
                    if (!e6bullet_active[i])
                    {
                        e6bullet_x[i] = enem6_x - 10;
                        e6bullet_y[i] = enem6_y + y_offsets[fired];
                        e6bullet_vx[i] = -bullet_speed;
                        e6bullet_vy[i] = 0.0;
                        iSetSpritePosition(&e6bulsprite[i], e6bullet_x[i], e6bullet_y[i]);
                        e6bullet_active[i] = 1;
                        fired++;
                    }
                }
            }
            else // Normal single bullet for both gamestates
            {
                for (int i = 0; i < MAX_BULLETS; i++)
                {
                    if (!e6bullet_active[i])
                    {
                        e6bullet_x[i] = enem6_x - 10;
                        e6bullet_y[i] = enem6_y + 70;
                        e6bullet_vx[i] = -30.0;
                        e6bullet_vy[i] = 0.0;
                        iSetSpritePosition(&e6bulsprite[i], e6bullet_x[i], e6bullet_y[i]);
                        e6bullet_active[i] = 1;
                        break;
                    }
                }
            }
        }
    }
    if (gamestate == ENDLESS)
    {
        // Enemy 1 Shooting
        if (enem1_active && !enem1_exploding)
        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (!ebullet_active[i])
                {
                    ebullet_x[i] = enem1_x - 10;
                    ebullet_y[i] = enem1_y + 40;
                    iSetSpritePosition(&ebulsprite[i], ebullet_x[i], ebullet_y[i]);
                    ebullet_active[i] = 1;
                    break;
                }
            }
        }

        // Enemy 2 Shooting
        if (enem2_active && !enem2_exploding)
        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (!e2bullet_active[i])
                {
                    e2bullet_x[i] = enem2_x - 10;
                    e2bullet_y[i] = enem2_y + 70;
                    iSetSpritePosition(&e2bulsprite[i], e2bullet_x[i], e2bullet_y[i]);
                    e2bullet_active[i] = 1;
                    break;
                }
            }
        }
        if (enem3_active && !enem3_exploding)
        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (!e3bullet_active[i])
                {
                    e3bullet_x[i] = enem3_x - 10;
                    e3bullet_y[i] = enem3_y + 40;
                    iSetSpritePosition(&e3bulsprite[i], e3bullet_x[i], e3bullet_y[i]);
                    e3bullet_active[i] = 1;
                    break;
                }
            }
        }
        // Enemy 4 Shooting
        if (enem4_active && !enem4_exploding)

        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (!e4bullet_active[i])
                {
                    e4bullet_x[i] = enem4_x - 10;
                    e4bullet_y[i] = enem4_y + 35;
                    iSetSpritePosition(&e4bulsprite[i], e4bullet_x[i], e4bullet_y[i]);
                    e4bullet_active[i] = 1;
                    break;
                }
            }
        }
        if (enem5_active && !enem5_exploding)
        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (!e5bullet_active[i])
                {
                    e5bullet_x[i] = enem5_x - 10;
                    e5bullet_y[i] = enem5_y - 10;
                    iSetSpritePosition(&e5bulsprite[i], e5bullet_x[i], e5bullet_y[i]);
                    e5bullet_active[i] = 1;
                    break;
                }
            }
        }
    }
}

//=============================================================================
// SCREEN DISPLAY FUNCTIONS
//=============================================================================

void homepage()
{
    iShowLoadedImage(0, 0, &home);
    scorenumber = 0;
    sprintf(scoretext, "%d", scorenumber);
}

void difficulty()
{
    iShowLoadedImage(0, 0, &mode);
}

void sound_manage()
{
    if (gamestate == HOME)
    {
        homeidx = iPlaySound("assets/sounds/menubg.wav", true, 70);
    }
    else if (gamestate == ARCADE || gamestate == ENDLESS || gamestate == BOSS)
    {
        iPauseSound(homeidx);
        mainidx = iPlaySound("assets/sounds/mainbg.wav", true, 70);
        //rocket_countidx= iPlaySound("assets/sounds/rocket_count.wav", true, 70);
    }
}

void mainpage1()
{
    // Background and Base UI
    if (gamestate == BOSS)
    {iShowLoadedImage(0, 0, &mainbg);
         // Shield Display
    if (shield_active)
    {
        iShowSprite(&shieldsprt);
    }
      // Spaceship Display
    iShowSprite(&spaceship);
    if(pur==3){
        iShowLoadedImage(400,200,&pur3);
        //iPlaySound("assets/sounds/321.wav", false, 70);
    }else if(pur==2){
        iShowLoadedImage(400,200,&pur2);
    }else if(pur==1){
        iShowLoadedImage(400,200,&pur1);
    }else if(pur==0) { 
        iShowLoadedImage(400,200,&purgo);
    }else if(pur<0){
    wrap = game_paused ? 0 : -2;
    iWrapImage(&mainbg, wrap);}}
    else if (gamestate == ARCADE)
    {iShowLoadedImage(0, 0, &abg);
         // Shield Display
    if (shield_active)
    {
        iShowSprite(&shieldsprt);
    }
        // Spaceship Display
    iShowSprite(&spaceship);
    if(blu==3){
        iShowLoadedImage(400,200,&blu3);
        //iPlaySound("assets/sounds/321.wav", false, 70);
    }else if(blu==2){
        iShowLoadedImage(400,200,&blu2);
    }else if(blu==1){
        iShowLoadedImage(400,200,&blu1);
    }else if(blu==0) { 
        iShowLoadedImage(400,200,&blugo);
    }else if(blu<0){
    wrap = game_paused ? 0 : -2;
    iWrapImage(&abg, wrap);}}
    if(blu<0 || pur<0){

    if (gamestate == BOSS)
        enem6_active = true;

    // Score Display
    iShowLoadedImage2(1060, 615, &score);

    // Enemy Display
    if (gamestate == ARCADE)
    {
        if (enem1_active || enem1_exploding)
            iShowSprite(&enem1);
        if (enem2_active || enem2_exploding)
            iShowSprite(&enem2);
    }
    if (gamestate == ARCADE || gamestate == BOSS)
    {
        if (enem6_active || enem6_exploding)
            iShowSprite(&enem6);
    }

    // Bullet Display
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullet_active[i])
        {
            iShowSprite(&bullet_sprites[i]);
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (ebullet_active[i])
        {
            iShowSprite(&ebulsprite[i]);
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (e2bullet_active[i])
        {
            iShowSprite(&e2bulsprite[i]);
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (e6bullet_active[i])
        {
            iShowSprite(&e6bulsprite[i]);
        }
    }

    // Score Text
    iText(1150, 650, scoretext, GLUT_BITMAP_TIMES_ROMAN_24);

    // Meteor Display
    if (meteor)
        iShowSprite(&met);
    if (meteor2)
        iShowSprite(&met2);

    // Power-up Display
    if (bonushp)
        iShowSprite(&bo_hp);

    // Collision Checks
    checkBulletEnemyCollision();
    checkEnemSpaceCollision();
    enemBulletCollision();
    checkshieldcollision();

    // Speed Display
    iShowSpeed(1110, 20);

    // Health Display
    if (health == 3)
    {
        iShowLoadedImage(-10, 550, &hp1);
    }
    else if (health == 2)
    {
        iShowLoadedImage(-10, 550, &hp2);
    }
    else if (health == 1)
    {
        iShowLoadedImage(-10, 550, &hp3);
    }

    // More Power-up Display
    if (bonusrocket)
        iShowSprite(&bo_roc);
    if (bonusshield)
        iShowSprite(&bo_shi);

    // Rocket Power-up Timer Display
    if (rocket_powerup_active)
    {
        int current_time = glutGet(GLUT_ELAPSED_TIME);
        int elapsed = current_time - rocket_powerup_start_time;
        int display_number = 9 - (elapsed / 1000);
        if (display_number >= 0 && display_number <= 9)
        {   if(rocket_countidx!=-1){
            iStopSound(rocket_countidx);
        }
            if (display_number == 9){
                iShowLoadedImage(180, 630, &num9);
             } else if (display_number == 8)
                iShowLoadedImage(180, 630, &num8);
            else if (display_number == 7)
                iShowLoadedImage(180, 630, &num7);
            else if (display_number == 6)
                iShowLoadedImage(180, 630, &num6);
            else if (display_number == 5)
                iShowLoadedImage(180, 630, &num5);
            else if (display_number == 4)
                iShowLoadedImage(180, 630, &num4);
            else if (display_number == 3)
                iShowLoadedImage(180, 630, &num3);
            else if (display_number == 2)
                iShowLoadedImage(180, 630, &num2);
            else if (display_number == 1)
                iShowLoadedImage(180, 630, &num1);
            else if (display_number == 0){
                iShowLoadedImage(180, 630, &num0);
        }else
        {
            rocket_powerup_active = false;
            if (rocket_countidx != -1)
            {
                iStopSound(rocket_countidx); // Stop sound when countdown ends
                rocket_countidx = -1;
            }
        }
        }
    }
    just_reset = false;
}
    
}

void mainpage2()
{
    // Background and Base UI
    iShowLoadedImage(0, 0, &mainbg2);
     // Shield Display
    if (shield_active)
    {
        iShowSprite(&shieldsprt);
    }
    // Spaceship Display
    iShowSprite(&spaceship);
    if(bla==3){
        iShowLoadedImage(400,200,&bla3);
        //iPlaySound("assets/sounds/321.wav", false, 70);
    }else if(bla==2){
        iShowLoadedImage(400,200,&bla2);
    }else if(bla==1){
        iShowLoadedImage(400,200,&bla1);
    }else if(bla==0) { 
        iShowLoadedImage(400,200,&blago);
    }else{
    wrap = game_paused ? 0 : -2;
    iWrapImage(&mainbg2, wrap);

   

    

    // Score Display
    iShowLoadedImage2(1060, 615, &score);

    // Enemy Display
    if (enem1_active || enem1_exploding)
        iShowSprite(&enem1);
    if (enem2_active || enem2_exploding)
        iShowSprite(&enem2);
    if ((enem4_active || enem4_exploding))
        iShowSprite(&enem4);
    if (enem3_active || enem3_exploding)
        iShowSprite(&enem3);
    if ((enem5_active || enem5_exploding))
        iShowSprite(&enem5);

    // Bullet Display
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullet_active[i])
        {
            iShowSprite(&bullet_sprites[i]);
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (ebullet_active[i])
        {
            iShowSprite(&ebulsprite[i]);
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (e2bullet_active[i])
        {
            iShowSprite(&e2bulsprite[i]);
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (e3bullet_active[i])
        {
            iShowSprite(&e3bulsprite[i]);
        }
    }
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (e4bullet_active[i])
        {
            iShowSprite(&e4bulsprite[i]);
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (e5bullet_active[i])
        {
            iShowSprite(&e5bulsprite[i]);
        }
    }

    // Score Text
    iText(1150, 650, scoretext, GLUT_BITMAP_TIMES_ROMAN_24);

    // Meteor Display
    if (meteor)
        iShowSprite(&met);
    if (meteor2)
        iShowSprite(&met2);
    // Power-up Display
    if (bonushp)
        iShowSprite(&bo_hp);

    // Collision Checks
    checkBulletEnemyCollision();
    checkEnemSpaceCollision();
    enemBulletCollision();
    checkshieldcollision();

    // Speed Display
    iShowSpeed(1110, 20);

    // Health Display
    if (health == 3)
    {
        iShowLoadedImage(-10, 550, &hp1);
    }
    else if (health == 2)
    {
        iShowLoadedImage(-10, 550, &hp2);
    }
    else if (health == 1)
    {
        iShowLoadedImage(-10, 550, &hp3);
    }

    // More Power-up Display
    if (bonusrocket)
        iShowSprite(&bo_roc);
    if (bonusshield)
        iShowSprite(&bo_shi);

    // Rocket Power-up Timer Display
    if (rocket_powerup_active)
    {
        int current_time = glutGet(GLUT_ELAPSED_TIME);
        int elapsed = current_time - rocket_powerup_start_time;
        int display_number = 9 - (elapsed / 1000);
        if (display_number >= 0 && display_number <= 9)
        {
             
            if (display_number == 9){
                iShowLoadedImage(180, 630, &num9);
             } else if (display_number == 8)
                iShowLoadedImage(180, 630, &num8);
            else if (display_number == 7)
                iShowLoadedImage(180, 630, &num7);
            else if (display_number == 6)
                iShowLoadedImage(180, 630, &num6);
            else if (display_number == 5)
                iShowLoadedImage(180, 630, &num5);
            else if (display_number == 4)
                iShowLoadedImage(180, 630, &num4);
            else if (display_number == 3)
                iShowLoadedImage(180, 630, &num3);
            else if (display_number == 2)
                iShowLoadedImage(180, 630, &num2);
            else if (display_number == 1)
                iShowLoadedImage(180, 630, &num1);
            else if (display_number == 0){
                iShowLoadedImage(180, 630, &num0);
            }else {
            // Countdown has ended (display_number < 0)
            rocket_powerup_active = false;
            if (rocket_countidx != -1) {
                iStopSound(rocket_countidx); // Stop the sound
                rocket_countidx = -1;        // Reset the sound index
            }
        }
        }
    }
}
    just_reset = false;
}

//=============================================================================
// iGRAPHICS CALLBACK FUNCTIONS
//=============================================================================

void iDraw() {
    iClear();
    switch (gamestate) {
    case HOME:
        ship_state = IDLE;
        homepage();
        break;
    case MODE:
        difficulty();
        break;
    case HELP:
        iShowLoadedImage(0, 0, &help);
        break;
    case CONTROLS:
        iShowLoadedImage(0, 0, &controls);
        break;
    case SOUND:
        iShowLoadedImage(0, 0, &sound);
        break;
    case QUIT:
        iShowLoadedImage(0, 0, &quit);
        break;
    case LEADERBOARD:
        difficulty();
        break;
    case NAME:
        iShowLoadedImage(0, 0, &name);
        iSetColor(255, 255, 255);
        iShowText(450, 253, player_name, "assets/fonts/Orbitron-Medium.ttf", 40);
        break;
    case ARCADE:
        mainpage1();
        if (game_paused) {
            iShowLoadedImage(420, 400, &paused);
            iSetColor(255, 255, 255);
            iShowText(520, 370, "RESUME", "assets/fonts/mokoto.ttf");
            iShowText(510, 270, "RESTART", "assets/fonts/mokoto.ttf");
            iShowText(475, 170, "MAIN MENU", "assets/fonts/mokoto.ttf");
        }
        break;
    case ENDLESS:
        mainpage2();
        if (game_paused) {
            iShowLoadedImage(420, 400, &paused);
            iSetColor(255, 255, 255);
            iShowText(520, 370, "RESUME", "assets/fonts/mokoto.ttf");
            iShowText(510, 270, "RESTART", "assets/fonts/mokoto.ttf");
            iShowText(475, 170, "MAIN MENU", "assets/fonts/mokoto.ttf");
        }
        break;
    case BOSS:
        mainpage1();
        if (game_paused) {
            iShowLoadedImage(420, 400, &paused);
            iSetColor(255, 255, 255);
            iShowText(520, 370, "RESUME", "assets/fonts/mokoto.ttf");
            iShowText(510, 270, "RESTART", "assets/fonts/mokoto.ttf");
            iShowText(475, 170, "MAIN MENU", "assets/fonts/mokoto.ttf");
        }
        break;
    case GAMEOVER:
        if (sound_check == 0 && mainidx != -1)
            iPauseSound(mainidx);
        iShowLoadedImage(0, 0, &gameoverscreen);
        if (gocount == 0)
            iShowLoadedImage(400, 300, &go1);
        else if (gocount == 1)
            iShowLoadedImage(400, 300, &go2);
        else if (gocount == 2)
            iShowLoadedImage(400, 300, &go3);
        else if (gocount == 3)
            iShowLoadedImage(400, 300, &go4);
        else if (gocount == 4)
            iShowLoadedImage(400, 300, &go5);
        else if (gocount == 5)
            iShowLoadedImage(400, 300, &go6);
        else if (gocount == 6)
            iShowLoadedImage(400, 300, &go7);
        else if (gocount == 7)
            iShowLoadedImage(400, 300, &go8);
        if (scorecolour == 0)
            iShowLoadedImage(400, 50, &score1);
        else if (scorecolour == 1)
            iShowLoadedImage(400, 50, &score2);
        else if (scorecolour == 2)
            iShowLoadedImage(400, 50, &score3);
        sprintf(scoretext, "%d", scorenumber);
        iShowText(640, 140, scoretext, "assets/fonts/Orbitron-Medium.ttf");
        break;
    case ARCADE_SCORE:
        iShowLoadedImage(0, 0, &ascore);
        iSetColor(255, 255, 255);
        for (int i = 0; i < arcadeCount && i < MAX_LEADERBOARD_ENTRIES; i++) {
            char scoreStr[20];
            sprintf(scoreStr, "%d", arcadeLeaderboard[i].score);
            iShowText(235, 525 - i * 80, arcadeLeaderboard[i].name, "assets/fonts/Orbitron-Medium.ttf", 35);
            iShowText(785, 525 - i * 80, scoreStr, "assets/fonts/Orbitron-Medium.ttf", 35);
        }
        break;
    case BOSS_SCORE:
        iShowLoadedImage(0, 0, &bscore);
        iSetColor(255, 255, 255);
        for (int i = 0; i < bossCount && i < MAX_LEADERBOARD_ENTRIES; i++) {
            char scoreStr[20];
            sprintf(scoreStr, "%d", bossLeaderboard[i].score);
            iShowText(250, 525 - i * 80, bossLeaderboard[i].name, "assets/fonts/Orbitron-Medium.ttf", 35);
            iShowText(785, 525 - i * 80, scoreStr, "assets/fonts/Orbitron-Medium.ttf", 35);
        }
        break;
    case ENDLESS_SCORE:
        iShowLoadedImage(0, 0, &lscore);
        iSetColor(255, 255, 255);
        for (int i = 0; i < endlessCount && i < MAX_LEADERBOARD_ENTRIES; i++) {
            char scoreStr[20];
            sprintf(scoreStr, "%d", endlessLeaderboard[i].score);
            iShowText(235, 525 - i * 80, endlessLeaderboard[i].name, "assets/fonts/Orbitron-Medium.ttf", 35);
            iShowText(785, 525 - i * 80, scoreStr, "assets/fonts/Orbitron-Medium.ttf", 35);
        }
        break;
    }
}
void iMouseMove(int mx, int my)
{
}

void iMouseDrag(int mx, int my) {}

void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        switch (gamestate)
        {
        case HOME:
            if ((424 <= mx && mx <= 773) && (354 <= my && my <= 416)){
                iPlaySound("assets/sounds/selection.wav", false, 100);
                gamestate = MODE;
            }else if ((424 <= mx && mx <= 773) && (245 <= my && my <= 310)){
                iPlaySound("assets/sounds/selection.wav", false, 100);
                gamestate = LEADERBOARD;
             } else if ((424 <= mx && mx <= 773) && (140 <= my && my <= 205)){
                iPlaySound("assets/sounds/selection.wav", false, 100);
                gamestate = HELP;
              } else if ((424 <= mx && mx <= 773) && (35 <= my && my <= 100)){
                iPlaySound("assets/sounds/selection.wav", false, 100);
                gamestate = ABOUT;
              }
            break;

        case MODE:

            if ((424 <= mx && mx <= 775) && (355 <= my && my <= 415))
            {   iPlaySound("assets/sounds/selection.wav", false, 100);
                prev_gamestate = ARCADE;
                gamestate = NAME;
                ;
            }
            else if ((424 <= mx && mx <= 775) && (225 <= my && my <= 285))
            {   iPlaySound("assets/sounds/selection.wav", false, 100);
                prev_gamestate = BOSS;
                gamestate = NAME;
            }
            else if ((424 <= mx && mx <= 775) && (95 <= my && my <= 155))
            {   iPlaySound("assets/sounds/selection.wav", false, 100);
                prev_gamestate = ENDLESS;
                gamestate = NAME;
            }

            break;
        case LEADERBOARD:

            if ((424 <= mx && mx <= 775) && (355 <= my && my <= 415))
            {   iPlaySound("assets/sounds/selection.wav", false, 100);
                gamestate = ARCADE_SCORE;
                
            }
            else if ((424 <= mx && mx <= 775) && (225 <= my && my <= 285))
            {   iPlaySound("assets/sounds/selection.wav", false, 100);
                gamestate = BOSS_SCORE;
            }
            else if ((424 <= mx && mx <= 775) && (95 <= my && my <= 155))
            {   iPlaySound("assets/sounds/selection.wav", false, 100);
                gamestate = ENDLESS_SCORE;
            }

            break;
        case HELP:
            cout << mx << " " << my << endl;
            if ((424 <= mx && mx <= 775) && (355 <= my && my <= 415)){
                iPlaySound("assets/sounds/selection.wav", false, 100);
                gamestate = CONTROLS;

             } else if ((424 <= mx && mx <= 775) && (220 <= my && my <= 290)){
                iPlaySound("assets/sounds/selection.wav", false, 100);
                gamestate = SOUND;
             }else if ((424 <= mx && mx <= 775) && (95 <= my && my <= 155)){
                iPlaySound("assets/sounds/selection.wav", false, 100);
                gamestate = QUIT;}

            break;
        case ARCADE:
            if (game_paused)
            {
                if ((522 <= mx && mx <= 704) && (372 <= my && my <= 403))
                {   iPlaySound("assets/sounds/ingameselection.wav", false, 100);
                    game_paused = false;
                    iResumeTimer(timer_id);
                    iResumeTimer(animation_timer_id);
                    if (sound_check == 0)
                        iResumeSound(mainidx);
                }
                else if ((511 <= mx && mx <= 719) && (273 <= my && my <= 303))
                {   iPlaySound("assets/sounds/ingameselection.wav", false, 100);
                    game_paused = false;
                    resetGame();
                    gamestate = ARCADE;
                    iPlaySound("assets/sounds/321.wav", false, 70);
                    if (sound_check == 0)
                    {
                        iResumeSound(mainidx);
                    }
                }
                else if ((476 <= mx && mx <= 746) && (172 <= my && my <= 203))
                {   iPlaySound("assets/sounds/ingameselection.wav", false, 100);
                    game_paused = false;
                    resetGame();
                    gamestate = HOME;
                    if (sound_check == 0)
                    {
                        iPauseSound(mainidx);
                        iResumeSound(homeidx);
                    }
                }
            }
            break;
        case ENDLESS:
            if (game_paused)
            {
                if ((522 <= mx && mx <= 704) && (372 <= my && my <= 403))
                {   iPlaySound("assets/sounds/ingameselection.wav", false, 100);
                    game_paused = false;
                    iResumeTimer(timer_id);
                    iResumeTimer(animation_timer_id);
                    if (sound_check == 0)
                        iResumeSound(mainidx);
                }
                else if ((511 <= mx && mx <= 719) && (273 <= my && my <= 303))
                {   iPlaySound("assets/sounds/ingameselection.wav", false, 100);
                    game_paused = false;
                    resetGame();
                    gamestate = ENDLESS;
                    iPlaySound("assets/sounds/321.wav", false, 70);
                    if (sound_check == 0)
                    {
                        iResumeSound(mainidx);
                    }
                }
                else if ((476 <= mx && mx <= 746) && (172 <= my && my <= 203))
                {   iPlaySound("assets/sounds/ingameselection.wav", false, 100);
                    game_paused = false;
                    resetGame();
                    gamestate = HOME;
                    if (sound_check == 0)
                    {
                        iPauseSound(mainidx);
                        iResumeSound(homeidx);
                    }
                }
            }
            break;
        case BOSS:
            if (game_paused)
            {
                if ((522 <= mx && mx <= 704) && (372 <= my && my <= 403))
                {   iPlaySound("assets/sounds/ingameselection.wav", false, 100);
                    game_paused = false;
                    iResumeTimer(timer_id);
                    iResumeTimer(animation_timer_id);
                    if (sound_check == 0)
                        iResumeSound(mainidx);
                }
                else if ((511 <= mx && mx <= 719) && (273 <= my && my <= 303))
                {   iPlaySound("assets/sounds/ingameselection.wav", false, 100);
                    game_paused = false;
                    resetGame();
                    gamestate = BOSS;
                    iPlaySound("assets/sounds/321.wav", false, 70);
                    if (sound_check == 0)
                    {
                        iResumeSound(mainidx);
                    }
                }
                else if ((476 <= mx && mx <= 746) && (172 <= my && my <= 203))
                {   iPlaySound("assets/sounds/ingameselection.wav", false, 100);
                    game_paused = false;
                    resetGame();
                    gamestate = HOME;
                    if (sound_check == 0)
                    {
                        iPauseSound(mainidx);
                        iResumeSound(homeidx);
                    }
                }
            }
            break;
                case GAMEOVER:
                if ((344 <= mx && mx <= 541) && (246 <= my && my <= 298))
            {   iPlaySound("assets/sounds/ingameselection.wav", false, 100);
                game_paused = false;
                resetGame();
                gamestate = prev_gamestate;
                iPlaySound("assets/sounds/321.wav", false, 70);
                if (sound_check == 0)
                {
                    iResumeSound(mainidx);
                }
            }
         
            if ((601 <= mx && mx <= 806) && (241 <= my && my <= 294))
            {   iPlaySound("assets/sounds/ingameselection.wav", false, 100);
                game_paused = false;
                resetGame();
                gamestate = HOME;
                if (sound_check == 0)
                {
                    iPauseSound(mainidx);
                    iResumeSound(homeidx);
                }
            }

            break;
        case ARCADE_SCORE:
                        cout << mx << " " << my << endl;
            break;
        }
    }
}

void iMouseWheel(int dir, int mx, int my) {}

// Keyboard Functions
void iKeyPress(unsigned char key)
{
    if (gamestate == NAME)
    {
        
        if (key == 8) // Backspace
        {
            if (name_length > 0)
            {
                player_name[--name_length] = '\0';
            }
        }
        else if (name_length < MAX_NAME_LENGTH && ((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z') || (key >= '0' && key <= '9')))
        {
            player_name[name_length++] = key;
            player_name[name_length] = '\0';
        }
        return;
    }

    switch (key)
    {
    case 'q':
        if (homeidx != -1)
            iStopSound(homeidx);
        if (mainidx != -1)
            iStopSound(mainidx);
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
        if (ship_state != EXP)
        {
            iPlaySound("assets/sounds/bulletsound.wav");
            ship_state = SHOOT;
            iChangeSpriteFrames(&spaceship, s_shoot, 4);
            iAnimateSprite(&spaceship);
            if (rocket_powerup_active)
            {
                if (rocket_powerup_count >= 2)
                {   
                    // Spread pattern for second power-up
                    int num_bullets = 5;
                    double angles[5] = {-15.0, -7.5, 0.0, 7.5, 15.0};
                    int fired = 0;
                    for (int i = 0; i < MAX_BULLETS && fired < num_bullets; i++)
                    {
                        if (!bullet_active[i])
                        {
                            bullet_x[i] = move_lf + 190;
                            bullet_y[i] = move_ud + 83;
                            bullet_angle[i] = angles[fired];
                            iSetSpritePosition(&bullet_sprites[i], bullet_x[i], bullet_y[i]);
                            bullet_active[i] = 1;
                            fired++;
                        }
                    }
                }
                else
                {
                    // Parallel lines for first power-up
                    int num_bullets = 5;
                    int y_offsets[5] = {83 + 30, 83 + 15, 83, 83 - 15, 83 - 30};
                    int fired = 0;
                    for (int i = 0; i < MAX_BULLETS && fired < num_bullets; i++)
                    {
                        if (!bullet_active[i])
                        {
                            bullet_x[i] = move_lf + 190;
                            bullet_y[i] = move_ud + y_offsets[fired];
                            bullet_angle[i] = 0.0; // No angle for parallel
                            iSetSpritePosition(&bullet_sprites[i], bullet_x[i], bullet_y[i]);
                            bullet_active[i] = 1;
                            fired++;
                        }
                    }
                }
            }
            else
            {
                int num_bullets = 2;
                int y_offsets[2] = {90, 77};
                int fired = 0;
                for (int i = 0; i < MAX_BULLETS && fired < num_bullets; i++)
                {
                    if (!bullet_active[i])
                    {
                        bullet_x[i] = move_lf + 190;
                        bullet_y[i] = move_ud + y_offsets[fired];
                        bullet_angle[i] = 0.0; // No angle for default
                        iSetSpritePosition(&bullet_sprites[i], bullet_x[i], bullet_y[i]);
                        bullet_active[i] = 1;
                        fired++;
                    }
                }
            }
        }
        break;
    case 'f':
        if (fullscreen == 0)
        {
            iEnterFullscreen();
            fullscreen++;
        }
        else
        {
            iLeaveFullscreen();
            fullscreen--;
        }
        break;
    }
}

void iKeyRelease(unsigned char key)
{
    if (gamestate == NAME)
    {
        if (key == 13 && name_length > 0) // Enter key, only proceed if name is not empty
        {
            gamestate = prev_gamestate;
            iPlaySound("assets/sounds/321.wav", false, 70);
            sound_manage();
        }
        return;
    }

    switch (key)
    {
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
        if (ship_state != EXP)
        {
            ship_state = IDLE;
            iChangeSpriteFrames(&spaceship, idle, 1);
        }
        break;
    case 'm':
        if (sound_check == 0)
        {
            if (gamestate == HOME || gamestate == MODE || gamestate == HELP || gamestate == ABOUT || gamestate == LEADERBOARD)
            
            {
                iPauseSound(homeidx);
                sound_check++;
            }
            if (gamestate == ARCADE || gamestate == ENDLESS || gamestate == BOSS)
            {
                iPauseSound(mainidx);
                sound_check++;
            }
        }
        else
        {
            if (gamestate == HOME || gamestate == MODE || gamestate == HELP || gamestate == ABOUT || gamestate == LEADERBOARD)
            
            {
                iResumeSound(homeidx);
                sound_check = 0;
            }
            if (gamestate == ARCADE || gamestate == ENDLESS || gamestate == BOSS)
            {
                iResumeSound(mainidx);
                sound_check = 0;
            }
        }
        break;
    case 27: // ESC key
        if (gamestate == ARCADE || gamestate == ENDLESS || gamestate == BOSS)
        { 
            game_paused = true;
            iPauseTimer(timer_id);
            iPauseTimer(animation_timer_id);
            if (sound_check == 0)
            {
                iPauseSound(mainidx);
            }
        }
        if (gamestate == MODE || gamestate == HELP || gamestate == ABOUT || gamestate == LEADERBOARD){
            iPlaySound("assets/sounds/selection.wav", false, 100);
            gamestate = HOME;}
        if (gamestate == CONTROLS || gamestate == SOUND || gamestate == QUIT){
            iPlaySound("assets/sounds/selection.wav", false, 100);
            gamestate = HELP;}
        if (gamestate == ARCADE_SCORE || gamestate == BOSS_SCORE || gamestate == ENDLESS_SCORE){
            iPlaySound("assets/sounds/selection.wav", false, 100);
            gamestate = LEADERBOARD;}
        
        break;
    }
}

void iSpecialKeyPress(unsigned char key)
{
    // Handle special keys (e.g., GLUT_KEY_LEFT, GLUT_KEY_RIGHT) if needed
    switch (key)
    {
    default:
        break;
    }
}

void iSpecialKeyRelease(unsigned char key)
{
    // Handle special key releases if needed
    switch (key)
    {
    default:
        break;
    }
}

//=============================================================================
// MAIN TIMER FUNCTION
//=============================================================================

void timer()
{
    if (game_paused)
        return;
        if(gamestate == ARCADE || gamestate == ENDLESS || gamestate == BOSS){
    countertimer+=50;}
    if(countertimer>=1000){
        if(gamestate == ARCADE){
            blu--;
            countertimer = 0;
        }else if(gamestate == BOSS){
            pur--;
            countertimer = 0;
        }else if(gamestate == ENDLESS){
            bla--;
            countertimer = 0;
        }
    }
    if(bla<0 || blu<0 || pur<0){
    
    // Update Game Components
    moveBullets();
    moveSpaceship();
    updateBonuses();
    updateEnemy();
    updateEnemyExplosion();
    updatemeteor();

    

    
    // Survival Score Timer (1 point per ~1.4 seconds)
    if (gamestate == ARCADE || gamestate == ENDLESS || gamestate == BOSS)
    {

        survival_score_timer += 50;
        if (survival_score_timer >= 70)
        {
            scorenumber += 1;
            sprintf(scoretext, "%d", scorenumber);
            survival_score_timer = 0;
        }
    }
}
    if (just_reset && just_reset_timer > 0)
    {
        just_reset_timer -= 50;
        if (just_reset_timer <= 0)
        {
            just_reset = false;
        }
    }
    // Game Over Screen Animation
    if (gamestate == GAMEOVER)
    {
        gocount = (gocount + 1) % 8;
        scorecolourtimer += 50;
        if (scorecolourtimer >= 1000)
        {
            scorecolour = (scorecolour + 1) % 3;
            scorecolourtimer = 0;
        }
    }

}

//=============================================================================
// MAIN FUNCTION
//=============================================================================

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    iInitializeSound();
    loadresources();
    sound_manage();
    iInitializeFont();
    initLeaderboards();
    //if (gamestate == ARCADE || gamestate == BOSS || gamestate == ENDLESS)
    //iPlaySound("assets/sounds/321.wav", false, 70);
    timer_id = iSetTimer(50, timer);
    animation_timer_id = iSetTimer(100, updateAnimation);
    iSetTimer(50, bossexplosion);
    iOpenWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Galaxy-Annihilator");
    return 0;
}