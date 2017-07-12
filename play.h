#pragma once
#include <stdbool.h>

enum type { VOID = ' ', WALL = '0', ENEMY = '1', ROCK = '^'};

struct gameboard {
	int width;
	int height;
	size_t size;
	enum type *ch;
	int territory;	//Territory under controle
};

struct position {
	int x;
	int y;
};

/* A list of position */
struct list_position {
	struct position *pos;
	struct list_position *next;
};

/* Create an empty game board and return a list_position *wall containing the initial position (x,y) */
struct list_position *gameboard_create(struct gameboard *self, int width, int height, int x, int y, struct list_position *wall);

/* Destroy a gameboard */
void gameboard_destroy(struct gameboard *self);

/* Destroy a list_position */
void list_destroy(struct list_position *self);

/* create a list_position*/
void list_position_create(struct list_position *self);

/* Add a wall to the gameboard */
struct list_position *gameboard_add_Wall(struct gameboard *self, int x, int y, struct list_position *wall);

/* Add a position to a sruct list_position and return a pointer of the begining of the list */
struct list_position *list_position_add(struct list_position *self, struct position *pos);

/* Add a rock to the gameboard */
void gameboard_add_Rock(struct gameboard *self, int x, int y);

/* Add an enemy position to the gameboard */
void gameboard_add_Enemy(struct gameboard *self, int x, int y);

/* Display a list position */
void display_list_position(struct list_position *list_pos);

/* Display the game board in stdout */
void display_gameboard(struct gameboard *self);

/* Convert the string response into an int*/
int server_response(const char *str);

/* Return a list of position containing the potential position of walls that can be played */
struct list_position *potential_position_of_walls(struct gameboard *self, const struct list_position *wall, struct list_position *res);

/* return a position of th jth element of a list_position  */
struct position *list_access(const struct list_position *self, size_t j);

/* return the size of a list_position*/
size_t list_size(const struct list_position *self);

/* tell if a position is present in a struct list_position*/
bool is_present(const struct list_position *self, struct position *p);

/* check if a position is valid */
bool is_valid(struct gameboard *self, int x, int y);