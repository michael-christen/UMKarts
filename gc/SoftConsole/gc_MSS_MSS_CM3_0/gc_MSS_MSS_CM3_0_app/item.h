#ifndef __ITEM__H__
#define __ITEM__H__
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "lcd.h"
#include "lasertag.h"

#define DRIVER MARIO

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

extern char *ITEM_NAMES [MAX_NUM_ITEMS];

extern double ITEM_PROB [MAX_NUM_ITEMS];

extern int ITEM_WEIGHT [MAX_NUM_ITEMS];

extern int TOTAL_WEIGHT;

extern void (*ITEM_USE_FUNCTIONS[MAX_NUM_ITEMS])(void);

extern void (*ITEM_HIT_FUNCTIONS[MAX_NUM_ITEMS])(void);

extern item CURRENT_ITEM;

#endif
