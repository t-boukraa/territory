#pragma once

#include "play.h"

/* Set 2 targets in the corner */
void set_targets(struct gameboard *self, struct list_position *possible, int x, int y, struct position *target1, struct position *target2);

/*  */
struct position *divide(struct gameboard *self, struct list_position *possible, int x, int y, struct position *target);

/* Return the closest position from a position */
struct position *distance_Min(const struct list_position *self, struct position *pos);

/* Return the distance between 2 positions */
int dMinPosition(struct position *p1, struct position *p2);

struct position *star(struct gameboard *self, struct list_position *possible, int x, int y);
