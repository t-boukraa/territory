#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include "play.h"
#include "strategy.h"

#define BUFSIZE 32
#define DEBUG			//use when no referee


int main() {
	char buf[BUFSIZE];

	// get the width
	if (fgets(buf, BUFSIZE, stdin) == NULL){
		return EXIT_FAILURE;
	}
	int width = atoi(buf);

	// get the height 
	if (fgets(buf, BUFSIZE, stdin) == NULL){
		return EXIT_FAILURE;
	}
	int height = atoi(buf);
		

	// get the initial x 
	if (fgets(buf, BUFSIZE, stdin) == NULL){
		return EXIT_FAILURE;
	}
	int x = atoi(buf);

	// get the initial y 
	if (fgets(buf, BUFSIZE, stdin) == NULL){
		return EXIT_FAILURE;
	}
	int y = atoi(buf);

	// initialize the state of the game
	struct gameboard board;
	struct list_position *wall = NULL;
	wall = gameboard_create(&board, width, height, x, y, wall);
	struct list_position *res = NULL;
	res = potential_position_of_walls(&board, wall, res);
	display_list_position(res);
	size_t count = 1;				// count the round
	fprintf(stderr, "\nStart playing\n");


	for (;;) {
		// play
#if defined DEBUG
		display_gameboard(&board);
#endif

		// choose a new x and y
//___________________________________________play randomly_______________
//		int ra = rand() % list_size(res);
//		printf("rand: %d\n", ra);
//_______________________________________________________________________
		fprintf(stderr, "\n# %zu\n", count);
		struct position *p;
		struct position target1;
		struct position target2;
		set_targets(&board, res, x, y, &target1, &target2);
		if (count % 2 == 0){
			p = divide(&board, res, x, y, &target2);
				if ( p == NULL){
					p = divide(&board, res, x, y, &target1);
				}
		}else {
			p = divide(&board, res, x, y, &target1);
			if (p == NULL){
				p = divide(&board, res, x, y, &target2);
			}
		}
//		p = star(&board, res, x, y);

//		p = list_access(res, ra);
		if ((list_size(res) != 0 && p != NULL) && count < (board.width + board.height + board.width)){
			puts("WALL");
			fprintf(stdout, "%i %i\n", p->x, p->y);
			fflush(stdout);
			fprintf(stderr, "Sent %i %i\n", p->x, p->y);
		}
		else{
			puts("PASS");
			fflush(stdout);
			fprintf(stderr, "Sent PASS\n");
		}
		count++;
		fprintf(stderr, "\nWaiting for a response from the server...\n");

		// get the response: WALL, ENEMY, ROCK, FINISH or ERROR
		if (fgets(buf, BUFSIZE, stdin) == NULL){
			perror(strerror(errno));
			return EXIT_FAILURE;
		}
		
		// update the state of the game
		if (strcmp(buf, "WALL\n") == 0) {					// Answered "WALL"
			fprintf(stderr, "Received WALL\n");
			wall = gameboard_add_Wall(&board, p->x, p->y, wall);
			res = NULL;
			list_destroy(res);
			res = potential_position_of_walls(&board, wall, res);
		}
		else if (strcmp(buf, "ENEMY\n") == 0) {			// Answered "ENEMY"
			fprintf(stderr, "Received ENEMY\n");
			gameboard_add_Enemy(&board, p->x, p->y);
			res = NULL;
			list_destroy(res);
			res = potential_position_of_walls(&board, wall, res);
		}
		else if (strcmp(buf, "ROCK\n") == 0) {			// Answered "ROCK"
			fprintf(stderr, "Received ROCK\n");
			gameboard_add_Rock(&board, p->x, p->y);
			list_destroy(res);
			res = NULL;
			res = potential_position_of_walls(&board, wall, res);
		}
		else if (strcmp(buf, "PASS\n") == 0) {			// Answered PASS
			fprintf(stderr, "Received PASS\n");
		}
		else if (strcmp(buf, "FINISH\n") == 0) {			// Answered FINISH
			fprintf(stderr, "Received FINISH\n");
			break;
		}
		else {												// ERROR
			fprintf(stderr, "Error: An error occured\n");
			gameboard_destroy(&board);		// !! the gameboard is not destroyed
			list_destroy(wall);
			list_destroy(res);
			return EXIT_FAILURE;
		}
	}
	gameboard_destroy(&board);
	list_destroy(wall);
	list_destroy(res);

	return EXIT_SUCCESS;
}
