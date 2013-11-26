#include "item.h"
#include "sound.h"
#include "sound_samples.h"
#include "drivers/mss_rtc/mss_rtc.h"

int ITEM_WEIGHT [MAX_NUM_ITEMS];
item CURRENT_ITEM = MAX_NUM_ITEMS;

void (*ITEM_USE_FUNCTIONS[MAX_NUM_ITEMS])(void) = {
    &use_green_shell,
    &use_mushroom,
    &use_lightning
};

void (*ITEM_HIT_FUNCTIONS[MAX_NUM_ITEMS])(void) = {
    &hit_green_shell,
    &hit_mushroom,
    &hit_lightning
};

char *ITEM_NAMES [MAX_NUM_ITEMS] = {
    "GREEN_SHELL",
    "MUSHROOM",
    "LIGHTNING"
};

double ITEM_PROB [MAX_NUM_ITEMS] = {
  0.50, //GREEN_SHELL
  0.40, //MUSHROOM
  0.10  //Lighting
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
	MSS_RTC_init();
	MSS_RTC_start();

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
}

item getNewItem() {
	srand((uint32_t) MSS_RTC_get_rtc_count());
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
    printf("player1, picked up, %S",ITEM_NAMES[CURRENT_ITEM]);
    sound_play(ITEMPICKUP_BEGIN, ITEMPICKUP_END);
}

void useCurrentItem() {
    if(CURRENT_ITEM == MAX_NUM_ITEMS) {
	return;
    }
    (*ITEM_USE_FUNCTIONS[CURRENT_ITEM])();
    CURRENT_ITEM = MAX_NUM_ITEMS;
    printf("player1, used, %S",ITEM_NAMES[CURRENT_ITEM]);
}

void hitByItem(item i) {
    (*ITEM_HIT_FUNCTIONS[i])();
}

void use_green_shell() {
	sound_play(SHOOT_BEGIN, SHOOT_END);
	LASER_TAG_shoot();
}
void use_mushroom() {
	sound_play(FAST_SOUND_BEGIN[DRIVER], FAST_SOUND_END[DRIVER]);
}
void use_lightning() {
	sound_play(LIGHTNING_BEGIN, LIGHTNING_END);
}

void hit_green_shell() {
	uint8_t opId = LASER_TAG_hit();
	printf("He shot me, %d\r\n", opId);
	sound_play(OW_SOUND_BEGIN[DRIVER], OW_SOUND_END[DRIVER]);
}
void hit_mushroom() {
}
void hit_lightning() {
}
