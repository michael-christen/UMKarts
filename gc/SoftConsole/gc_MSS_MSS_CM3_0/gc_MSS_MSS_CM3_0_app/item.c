#include "item.h"
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

void useCurrentItem() 
    if(CURRENT_ITEM == MAX_NUM_ITEMS) {
	return;
    }
    (*ITEM_FUNCTIONS[CURRENT_ITEM])();
    CURRENT_ITEM = MAX_NUM_ITEMS;
}

void use_green_shell() {
}
void use_mushroom() {
}
void use_lightning() {
}
