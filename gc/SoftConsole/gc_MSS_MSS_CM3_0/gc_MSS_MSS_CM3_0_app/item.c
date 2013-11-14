#include "item.h"

int ITEM_WEIGHT [MAX_NUM_ITEMS];
item CURRENT_ITEM;

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
    "Green Shell",
    "Mushroom",
    "Lightning"
};

double ITEM_PROB [MAX_NUM_ITEMS] = {
  0.50, //GREEN_SHELL
  0.40, //MUSHROOM
  0.10  //Lighting
};

int TOTAL_WEIGHT = 1000000;

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
}

item getNewItem() {
	srand(time(NULL));
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
}

void useCurrentItem() {
    if(CURRENT_ITEM == MAX_NUM_ITEMS) {
	return;
    }
    (*ITEM_USE_FUNCTIONS[CURRENT_ITEM])();
    CURRENT_ITEM = MAX_NUM_ITEMS;
}

void hitByItem(item i) {
    (*ITEM_HIT_FUNCTIONS[i])();
}

void use_green_shell() {
}
void use_mushroom() {
}
void use_lightning() {
}

void hit_green_shell() {
}
void hit_mushroom() {
}
void hit_lightning() {
}
