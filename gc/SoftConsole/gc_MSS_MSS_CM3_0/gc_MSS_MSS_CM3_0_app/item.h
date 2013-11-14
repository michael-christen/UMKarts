#ifndef __ITEM__H__
#define __ITEM__H__
enum item {
    GREEN_SHELL,
    MUSHROOM,
    LIGHTNING,
    MAX_NUM_ITEMS 
};

double ITEM_PROB [MAX_NUM_ITEMS] = {
  0.50, //GREEN_SHELL
  0.40, //MUSHROOM
  0.10  //Lighting
};

int ITEM_WEIGHT [MAX_NUM_ITEMS];

void initItemWeights();
item getNewItem();
void handleItemGrab();
void useCurrentItem();

void use_green_shell();
void use_mushroom();
void use_lightning();

void (*ITEM_FUNCTIONS(void))[MAX_NUM_ITEMS] = {
    &use_green_shell,
    &use_mushroom,
    &use_lightning
};

int TOTAL_WEIGHT = 1000000;

item CURRENT_ITEM;

#endif
