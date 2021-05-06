#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define CELL_SIZE 10

typedef enum {ALIVE, DEAD} State;
typedef struct{
    int x;
    int y;
    State state;
} Cell;

// SDL related functions
SDL_Window *createWindow(char *title);
SDL_Renderer *createRenderer(SDL_Window *window);
void drawGrid(SDL_Renderer *r, int len, int wid);
void drawCells(SDL_Renderer *r, int a[][1000], int x, int y);

// Game of Life functions
void updateCells(int a[][1000], int len, int wid); // takes cells array as input
int countLivingNeighbours(int a[][1000], int x, int y, int len, int wid);
int conti(int *x, int *y, int a[][1000])
{
	FILE *fp;
	fp = fopen("State.txt", "r");
	fscanf(fp, "%d %d", x, y);
	for(int cy = 0; cy < *x; cy++)
	{
		for(int cx = 0; cx < *y; cx++)
		{
			if(fgetc(fp) == 0)
			a[cy][cx] = DEAD;
			if(fgetc(fp) == 1)
			a[cy][cx] = ALIVE;
		}
	}
}
int main(int argc, char *argv[])
{
	int choose;
    // Initialise SDL
    SDL_Init(SDL_INIT_VIDEO);
	// Create window and renderer
	SDL_Window *window = createWindow("Game of Life");
    SDL_Renderer *renderer = createRenderer(window);
	int cells[1000][1000];
	int grid_length, grid_width, *l = &grid_length, *w = &grid_width;
	printf("Which choose do you want to aplly\n1.Start a new one\n2.Continue. ");
	scanf("%d", &choose);
	if(choose == 1)//case 1: choose chongqi
	{
		printf("Please enter the height and width");
		scanf("%d %d", &grid_length, &grid_width);
		// Set all cells to initial state of dead
    	for(int cy = 0; cy < grid_length; cy++)
		{
        	for(int cx = 0; cx < grid_width; cx++)
			{
            	cells[cy][cx] = DEAD;
        	}
    	}
	}
	else
	{
		conti(l, w, cells);
	}
    // Setup event handling + mouse co-ordinate handling
    SDL_Event event;
    int mouseX, mouseY;
    bool mouse_left_down = false;
    bool mouse_right_down = false;
	int choice;
    // MAIN LOOP //
    printf("Do you want to decide steps or let it go\n1.Steps\n2.Let it go");
    scanf("%d", &choice);
    if(choice == 2)
	{
   	 	while(1)
			{
        	// Handle events/input
        	while(SDL_PollEvent(&event) != 0)
			{
            	switch(event.type)
				{
            	    case SDL_QUIT: // Check if user has quit
            	        return 1;

            	    // Check if user is HOLDING left or right mouse button
           	 	    case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:
             	       switch(event.button.button)
						{
                 	       case SDL_BUTTON_LEFT: mouse_left_down = !mouse_left_down; break;
                 	       case SDL_BUTTON_RIGHT: mouse_right_down = !mouse_right_down; break;
                 	   	}

             	   // If user presses space, simulate a single change
                	case SDL_KEYDOWN:
                    	if(event.key.keysym.sym == SDLK_SPACE)
                        	updateCells(cells, grid_length, grid_width);
            	}
        	}

        // Get user mouse button input - left click gives life to cell at current co-ords, right click kills
        SDL_GetMouseState(&mouseX, &mouseY);
        if(mouse_left_down == true)
            cells[mouseY / CELL_SIZE][mouseX / CELL_SIZE] = ALIVE;
        else if(mouse_right_down == true)
            cells[mouseY / CELL_SIZE][mouseX / CELL_SIZE] = DEAD;

        // Set screen colour to white
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        // Render white to screen (clear screen)
        SDL_RenderClear(renderer);

        // Draw the grid and living cells
        drawGrid(renderer, grid_length, grid_width);
        drawCells(renderer, cells, grid_length, grid_width);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Exit SDL and SDL_image
    SDL_Quit();
    return 0;
	}

	else if(choice == 1)
	{
		int step, delay;
		printf("Please enter your step and evolution delay");
		scanf("%d %d",&step,&delay);
		while(1){
     	   // Handle events/input
     	   	while(SDL_PollEvent(&event) != 0)
			{
         	switch(event.type)
				{
          	    	case SDL_QUIT: // Check if user has quit
          	        return 1;
	
                // Check if user is HOLDING left or right mouse button
                	case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:
                    	switch(event.button.button)
						{
                        	case SDL_BUTTON_LEFT: mouse_left_down = !mouse_left_down; break;
                        	case SDL_BUTTON_RIGHT: mouse_right_down = !mouse_right_down; break;
                   		}

                // If user presses space, simulate a single change
            	  	  case SDL_KEYDOWN:
                    if(event.key.keysym.sym == SDLK_SPACE)
					{
                    	for(int i=0; i< step;i++)
                    	{
                    		updateCells(cells, grid_length, grid_width);
                    		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        					// Render white to screen (clear screen)
        					SDL_RenderClear(renderer);

        					// Draw the grid and living cells
        					drawGrid(renderer, grid_length, grid_width);
        					drawCells(renderer, cells, grid_length, grid_width);

        					// Update screen
       						SDL_RenderPresent(renderer);
       						SDL_Delay(delay);
						}
					}

            }
        }

        // Get user mouse button input - left click gives life to cell at current co-ords, right click kills
        SDL_GetMouseState(&mouseX, &mouseY);
        if(mouse_left_down == true)
            cells[mouseY / CELL_SIZE][mouseX / CELL_SIZE] = ALIVE;
        else if(mouse_right_down == true)
            cells[mouseY / CELL_SIZE][mouseX / CELL_SIZE] = DEAD;

        // Set screen colour to white
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        // Render white to screen (clear screen)
        SDL_RenderClear(renderer);

        // Draw the grid and living cells
        drawGrid(renderer, grid_length, grid_width);
        drawCells(renderer, cells, grid_length, grid_width);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Exit SDL and SDL_image
    SDL_Quit();
    return 0;
	}
}
/*
1. Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
2. Any live cell with two or three live neighbours lives on to the next generation.
3. Any live cell with more than three live neighbours dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/
void updateCells(int a[][1000], int len, int wid){
    int new[len][wid];
    int cy, cx; // vertical count, horizontal count

    for(cy = 0; cy < len; cy++){
        for(cx = 0; cx < wid; cx++){
            // Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
            if(a[cy][cx] == ALIVE && countLivingNeighbours(a, cx, cy, len, wid) < 2)
                new[cy][cx] = DEAD;

            // Any live cell with two or three live neighbours lives on to the next generation.
            else if(a[cy][cx] == ALIVE && (countLivingNeighbours(a, cx, cy, len, wid) == 2 || countLivingNeighbours(a, cx, cy, len, wid) == 3))
                new[cy][cx] = ALIVE;

            // Any live cell with more than three live neighbours dies, as if by overpopulation.
            else if(a[cy][cx] == ALIVE && countLivingNeighbours(a, cx, cy, len, wid) > 3)
                new[cy][cx] = DEAD;

            // Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
            else if(a[cy][cx] == DEAD && countLivingNeighbours(a, cx, cy, len, wid) == 3)
                new[cy][cx] = ALIVE;

            else
                new[cy][cx] = DEAD;
        }
    }

    // Update all cells into new states
    for(cy = 0; cy < len; cy++){
        for(cx = 0; cx < wid; cx++){
            a[cy][cx] = new[cy][cx];
        }
    }
}

// THERE'S NO ERROR CHECKING HERE WHICH IS BAD
// Should ideally check if a cell even exists before checking its state
int countLivingNeighbours(int a[][1000], int x, int y, int len, int wid){
    int count = 0, cx, cy;

    for(cy = y - 1; cy <= y + 1; cy++){
        for(cx = x - 1; cx <= x + 1; cx++){

            // Ensure neighbouring cell is not out of bounds
            if(!(cy < 0 || cx < 0 || cy > len || cx > wid)){
                // If there is a living neighbouring cell, add to count
                if(a[cy][cx] == ALIVE)
                    count++;

                // disregard current cell as it is not a neighbour
                if(a[cy][cx] == ALIVE && cx == x && cy == y)
                    count--;
            }


        }
    }

    return count;
}

void drawGrid(SDL_Renderer *r,int len, int wid){
    // Draw vertical grid lines
    for(int v = CELL_SIZE; v < len*CELL_SIZE; v += CELL_SIZE){
        // Set draw colour to grey
        SDL_SetRenderDrawColor(r, 110, 110, 110, 110);

        // Draw vertical line
        SDL_RenderDrawLine(r, v, 0, v, len*CELL_SIZE);
    }

    // Draw horizontal grid lines
    for(int h = CELL_SIZE; h < wid*CELL_SIZE; h += CELL_SIZE){
        // Set draw colour to grey
        SDL_SetRenderDrawColor(r, 110, 110, 110, 110);

        // Draw horizontal line
        SDL_RenderDrawLine(r, 0, h, wid*CELL_SIZE, h);
    }
}

void drawCells(SDL_Renderer *r, int a[][1000], int x, int y){
    // Define cell width/height
    SDL_Rect cellRect;
    cellRect.w = CELL_SIZE + 1; // Same size as one cell +1 so it covers the grid line fully
    cellRect.h = CELL_SIZE + 1; // Same size as one cell +1 so it covers the grid line fully

    // Draw living cells
    int cx, cy;
    for(cy = 0; cy < x; cy++){
        for(cx = 0; cx < y; cx++){
            if(a[cy][cx] == ALIVE){
                // Set cell x/y pos
                cellRect.x = cx * CELL_SIZE;
                cellRect.y = cy * CELL_SIZE;

                SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
                SDL_RenderFillRect(r, &cellRect);
            }
        }
    }
}
void write(int a[][1000], int len, int wid)
{
	FILE *fp = fopen("State.txt", "w");
	fprintf(fp, "%d %d\n", len, wid);
	for(int i = 0; i < len; i++)
	{
		for(int j = 0; j< wid; j++)
		{
			fprintf(fp, "%d", a[len][wid]);
		}
	}
}
SDL_Window *createWindow(char *title){
    SDL_Window *window = SDL_CreateWindow(
        title,                  // Title
        SDL_WINDOWPOS_CENTERED, // Initial window x position
        SDL_WINDOWPOS_CENTERED, // Initial window y position
    	1000,           // Window Width
        1000,          // Window Height
        0                       // Flags
    );

    if(window == NULL){
        printf("Failed to create window. %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    return window;
}

SDL_Renderer *createRenderer(SDL_Window *window){
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,                     // Window
        -1,                         // Monitor index (-1 for first available)
        SDL_RENDERER_ACCELERATED    // Flags
    );

    if(renderer == NULL){
        printf("Failed to create renderer. %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    return renderer;
}
