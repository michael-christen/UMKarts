#ifndef __ITEM__H__
#define __ITEM__H__
#include <stdlib.h>
#include <time.h>

typedef enum {
    GREEN_SHELL,
    MUSHROOM,
    LIGHTNING,
    MAX_NUM_ITEMS 
} item;


item getNewItem();
void initItemWeights();
void handleItemGrab();
void useCurrentItem();

void use_green_shell();
void use_mushroom();
void use_lightning();

void hit_green_shell();
void hit_mushroom();
void hit_lightning();

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

int ITEM_WEIGHT [MAX_NUM_ITEMS];

int TOTAL_WEIGHT = 1000000;

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

item CURRENT_ITEM;

#endif
