#include "item.h"
#include "player_drive.h"
#include "sound.h"
#include "sound_samples.h"
#include "drivers/mss_rtc/mss_rtc.h"

int ITEM_WEIGHT [MAX_NUM_ITEMS];
item CURRENT_ITEM = MAX_NUM_ITEMS;

void (*ITEM_USE_FUNCTIONS[MAX_NUM_ITEMS])(void) = {
    &use_green_shell,
    &use_mushroom,
    &use_lightning,
    &use_star
};

void (*ITEM_HIT_FUNCTIONS[MAX_NUM_ITEMS])(void) = {
    &hit_green_shell,
    &hit_mushroom,
    &hit_lightning,
    &hit_star
};

char *ITEM_NAMES [MAX_NUM_ITEMS] = {
    "GREEN_SHELL",
    "MUSHROOM",
    "LIGHTNING",
    "STAR"
};

uint64_t ITEM_DURATIONS [] = {3, 4, 0, 10};
/*ITEM_DURATIONS[GREEN_SHELL] = 0;
ITEM_DURATIONS[MUSHROOM] = 400;
ITEM_DURATIONS[LIGHTNING] = 0;
ITEM_DURATIONS[STAR] = 10000;*/


double ITEM_PROB [MAX_NUM_ITEMS] = {
  0.40, //GREEN_SHELL
  0.30, //MUSHROOM
  0.10,  //Lighting
  0.20	//STAR
};

int TOTAL_WEIGHT = 1000000;

int FAST_SOUND_BEGIN[] = {
	MARIOFAST_BEGIN,
	LUIGIFAST_BEGIN,
	WARIOFAST_BEGIN,
	PEACHFAST_BEGIN
};

int FAST_SOUND_END[] = {
	MARIOFAST_END,
	LUIGIFAST_END,
	WARIOFAST_END,
	PEACHFAST_END
};

int OW_SOUND_BEGIN[] = {
	MARIOOW_BEGIN,
	LUIGIOW_BEGIN,
	WARIOOW_BEGIN,
	PEACHOW_BEGIN
};

int OW_SOUND_END[] = {
	MARIOOW_END,
	LUIGIOW_END,
	WARIOOW_END,
	PEACHOW_END
};

int SUCCESS_SOUND_BEGIN[] = {
	MARIOSUCCESS_BEGIN,
	LUIGISUCCESS_BEGIN,
	WARIOSUCCESS_BEGIN,
	PEACHSUCCESS_BEGIN
};

int SUCCESS_SOUND_END[] = {
	MARIOSUCCESS_END,
	LUIGISUCCESS_END,
	WARIOSUCCESS_END,
	PEACHSUCCESS_END
};

//Requires all probs to sum to 1
void initItemWeights() {
    /*
    int i, weight_bottom = 1, weight_top = 100000;
    double epsilon = 0.0001;
    //Find smallest total_weight that
    //accurately represents percentages
    for(i=0; i < MAX_NUM_ITEMS; ++i) {
    }
    */
    int i;
    for(i=0; i < MAX_NUM_ITEMS; ++i) {
    	ITEM_WEIGHT[i] = ITEM_PROB[i] * TOTAL_WEIGHT;
    }
    srand((uint32_t) MSS_RTC_get_rtc_count());
}

item getNewItem() {
    int rChoice = rand() % TOTAL_WEIGHT;
    int i;
    for(i=0; i < MAX_NUM_ITEMS; ++i) {
	if(rChoice < ITEM_WEIGHT[i]) {
	    return (item) i;
	}
	rChoice -= ITEM_WEIGHT[i];
    }
    //Error output
    return MAX_NUM_ITEMS;
}

void handleItemGrab() {
    //Can't get item if already have one
    if(CURRENT_ITEM != MAX_NUM_ITEMS) {
    	return;
    }
    CURRENT_ITEM = getNewItem();
    LCD_printf("Picked up %s", ITEM_NAMES[CURRENT_ITEM]);
    printf("player1, picked up, %s",ITEM_NAMES[CURRENT_ITEM]);
    sound_play(ITEMPICKUP_BEGIN, ITEMPICKUP_END);
}

void useCurrentItem() {
    if(CURRENT_ITEM == MAX_NUM_ITEMS) {
	return;
    }
    (*ITEM_USE_FUNCTIONS[CURRENT_ITEM])();

    printf("player1, used, %s",ITEM_NAMES[CURRENT_ITEM]);
    CURRENT_ITEM = MAX_NUM_ITEMS;
}

void hitByItem(item i) {
	if (PLAYER_DRIVE_is_invincible()) {
		return;
	}
    (*ITEM_HIT_FUNCTIONS[i])();
}

void use_green_shell() {
	sound_play(SHOOT_BEGIN, SHOOT_END);
	LASER_TAG_shoot();
}
void use_mushroom() {
	sound_play(FAST_SOUND_BEGIN[DRIVER], FAST_SOUND_END[DRIVER]);
	PLAYER_DRIVE_set_modification(mod_speed_boost, ITEM_DURATIONS[MUSHROOM]);
}
void use_lightning() {
	sound_play(LIGHTNING_BEGIN, LIGHTNING_END);
	PLAYER_DRIVE_set_modification(mod_speed_slow, ITEM_DURATIONS[LIGHTNING]);
}
void use_star() {
	sound_play_repeat(STARPOWER_BEGIN, STARPOWER_END);
	PLAYER_DRIVE_set_modification(mod_star, ITEM_DURATIONS[STAR]);
}

void hit_green_shell() {
	uint8_t opId = LASER_TAG_hit();
	// You can't shoot yourself
	if (opId == DRIVER) {
		return;
	}
	printf("He shot me, %d\r\n", opId);
	PLAYER_DRIVE_set_modification(mod_hit_by_shell, ITEM_DURATIONS[GREEN_SHELL]);
	sound_play(OW_SOUND_BEGIN[DRIVER], OW_SOUND_END[DRIVER]);
}
void hit_mushroom() {
}
void hit_lightning() {
}
void hit_star() {
}
