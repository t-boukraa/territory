#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "play.h"
#define DEBUG			//use when no referee

struct list_position *gameboard_create(struct gameboard *self, int width, int height, int x, int y, struct list_position *wall) {

#if defined DEBUG
	int fd = creat("player.log", 0640);
	if (fd == -1){
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDERR_FILENO);
	close(fd);
#endif

	fprintf(stderr, "Date: %s %s\n", __DATE__, __TIME__);
	fprintf(stderr, "\nIniatializing the gameboard...\n");
	fprintf(stderr, "Width:\t%d\n", width);
	fprintf(stderr, "Height:\t%d\n", height);
	fprintf(stderr, "Initial position: (%d, %d)\n", x, y);
	
	self->height = height;
	self->width = width;
	self->size = self->width*self->height;
	self->ch = calloc(self->size, sizeof(enum type));
	if (self->ch == NULL) {
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}
	for (size_t i = 0; i < self->size; i++) {
		self->ch[i] = VOID;
	}
	wall = gameboard_add_Wall(self, x, y, wall);
	return wall;
}

void gameboard_destroy(struct gameboard *self) {
	free(self->ch);
	self->ch = NULL;
	fprintf(stderr, "Gameboard destroyed\n");
}

void list_destroy(struct list_position *self) {
	struct list_position *prev;
	while (self != NULL) {
		prev = self;
		self = self->next;
		fprintf(stderr, "Released position (%d %d)\n", prev->pos->x, prev->pos->y);
		free(prev);
		prev = NULL;
	}
	free(self);
	self = NULL;
}

struct list_position *gameboard_add_Wall(struct gameboard *self, int x, int y, struct list_position *wall) {
	fprintf(stderr, "\n\tOperation: Adding a wall to the struct gameboard\n");
	if (is_valid(self, x, y)) {
		if (self->ch[x + y*self->width] == VOID) {
			self->ch[x + y*self->width] = WALL;
			struct position p;
			p.x = x;
			p.y = y;
			wall = list_position_add(wall, &p);
			fprintf(stderr, "Information: Added a WALL in the position (%d;%d)\n", x, y);
			fprintf(stderr, "\tEnd operation: Adding a wall to the struct gameboard\n");
		}
		else {
			fprintf(stderr, "Information: Cannot add a wall because the position was not clear\n");
		}
	}
	else {
		fprintf(stderr, "Error: invalid position (%d, %d)\n", x, y);
		exit(EXIT_FAILURE);
	}
	return wall;
}

void gameboard_add_Rock(struct gameboard *self, int x, int y) {
	self->ch[x + y*self->width] = ROCK;
}

void gameboard_add_Enemy(struct gameboard *self, int x, int y) {
	self->ch[x + y*self->width] = ENEMY;
}

struct list_position *potential_position_of_walls(struct gameboard *self, const struct list_position *wall, struct list_position *res) {
	fprintf(stderr, "\n\tOperation : search for potential positions of walls\n");
	while (wall != NULL) {
		struct position pa;		// above position
		pa.x = wall->pos->x;
		pa.y = wall->pos->y - 1;
		struct position pb;		// bottom position
		pb.x = wall->pos->x;
		pb.y = wall->pos->y + 1;
		struct position pl;		// left position
		pl.x = wall->pos->x - 1;
		pl.y = wall->pos->y;
		struct position pr;		// right position
		pr.x = wall->pos->x + 1;
		pr.y = wall->pos->y;
		if (is_valid(self, pl.x, pl.y) && self->ch[pl.x + pl.y*self->width] == VOID && !is_present(res, &pl)) {
			res = list_position_add(res, &pl);
		}
		if (is_valid(self, pr.x, pr.y) && self->ch[pr.x + pr.y*self->width] == VOID && !is_present(res, &pr)) {
			res = list_position_add(res, &pr);
		}
		if (is_valid(self, pa.x, pa.y) && self->ch[pa.x + pa.y*self->width] == VOID && !is_present(res, &pa)) {
			res = list_position_add(res, &pa);
		}
		if (is_valid(self, pb.x, pb.y) && self->ch[pb.x + pb.y*self->width] == VOID && !is_present(res, &pb)) {
			res = list_position_add(res, &pb);
		}
		wall = wall->next;
	}
	fprintf(stderr, "\tEnd operation : search for potential positions of walls\n");
	return res;
}

size_t list_size(const struct list_position *self) {
	if (self == NULL) {
		return 0;
	}
	return 1 + list_size(self->next);
}

struct position *list_access(const struct list_position *self, size_t j) {
	if (self == NULL) {
		return NULL;
	}
	if (j == 0) {
		return self->pos;
	}
	return list_access(self->next, j - 1);
}

bool is_present(const struct list_position *self, struct position *p) {
	if (self == NULL) {
		return false;
	}
	if (self->pos->x != p->x || self->pos->y != p->y) {
		return is_present(self->next, p);
	}
	return true;
}

bool is_valid(struct gameboard *self, int x, int y) {
	return (x >= 0 && x < self->width && y >= 0 && y < self->height);
}

struct list_position *list_position_add(struct list_position *self, struct position *pos) {
	struct list_position *other = calloc(1, sizeof(struct list_position));
	if (other == NULL) {
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}
	struct position *p = calloc(1, sizeof(struct position));
	if (p == NULL) {
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}
	p->x = pos->x;
	p->y = pos->y;
	other->pos = p;
	other->next = self;
	fprintf(stderr, "Added position (%d, %d)\n", pos->x, pos->y);
	return other;
}

void display_list_position(struct list_position *list_pos) {
	fprintf(stderr, "\n\tOperation: Display a list\n");
	struct list_position *cure = list_pos;
	while (cure != NULL) {
		fprintf(stderr, "(%d;%d) > ", cure->pos->x, cure->pos->y);
		cure = cure->next;
	}
	fprintf(stderr, "NULL\n");
	fprintf(stderr, "\tEnd operation: Display a list\n");
}

void display_gameboard(struct gameboard *self) {
	for (size_t x = 0; x < self->width; x++) {
		if (x < 10) {
			printf(" %zu  ", x);
		}
		else if (x < 100) {
			printf(" %zu ", x);
		}
	}
	for (size_t i = 0; i < self->size; i++) {
		if (i % self->width == 0) {
			printf("\n");
		}
		printf(" %c |", self->ch[i]);
	}
	printf("\n");
}


