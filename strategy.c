#include <stdio.h>
#include <math.h>

#include "play.h"
#include "strategy.h"


struct position *star(struct gameboard *self, struct list_position *possible, int x, int y){
	fprintf(stderr, "\tOperation: Star\n");
	struct position *res = NULL;
	
	while (possible != NULL){
		if (possible->pos->x == x || possible->pos->y == y) {
			return possible->pos;
		}
		possible = possible->next;
	}
	fprintf(stderr, "returned NULL position\n");
	return res;
}


/* 	x: initial x
	y: initial y */
void set_targets(struct gameboard *self, struct list_position *possible, int x, int y, struct position *target1, struct position *target2) {
	fprintf(stderr, "\tOperation: set targets\n");
	if (x < (int)self->width / 2) {
		target1->x = self->width-1;
		target2->x = 0;
	}
	else {
		target1->x = 0;
		target2->x = self->width-1;
	}
	if (y < (int)self->height / 2) {
		target1->y = self->height-1;
		target2->y = 0;
	}
	else {
		target1->y = 0;
		target2->y = self->height-1;
	}
	fprintf(stderr, "Target1 position: %d, %d\n", target1->x, target1->y);
	fprintf(stderr, "Target2 position: %d, %d\n", target2->x, target2->y);
	fprintf(stderr, "\tEnd Operation: set targets\n");
}

struct position *divide(struct gameboard *self, struct list_position *possible, int x, int y, struct position *target) {
	fprintf(stderr, "\tOperation: divide\n");
	struct position *res = NULL;

	struct position *pMin = distance_Min(possible, target);

	if (dMinPosition(target, pMin) != 0 ){
		fprintf(stderr, "\tEnd Operation: divide\n");
		return pMin;
	}
	else {	//Target reached
		fprintf(stderr, "Target reached\n");
	}
	fprintf(stderr, "returned NULL position\n");
	return res;
}

struct position *distance_Min(const struct list_position *self, struct position *pos) {
	fprintf(stderr, "\t\tOperation: Distance min\n");
	double dMin = dMinPosition(self->pos, pos);
	struct position *p = self->pos;
	while (self != NULL) {
		fprintf(stderr, "\tComparing dMin= %f with, dMinPosition= %d\n", dMin, dMinPosition(self->pos, pos));
		if (dMin > dMinPosition(self->pos, pos)) {
			dMin = dMinPosition(self->pos, pos);
			p = self->pos;
		}
		self = self->next;
	}
	fprintf(stderr, "\t\tEnd Operation: Distance min\n");
	return p;
}

int dMinPosition(struct position *p1, struct position *p2) {
	return (int) sqrt((p2->x - p1->x) * (p2->x - p1->x) + (p2->y - p1->y) * (p2->y - p1->y));
}
