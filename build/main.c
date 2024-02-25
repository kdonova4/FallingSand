#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define GRID_WIDTH 800
#define GRID_HEIGHT 800
#define EMPTY_COLOR BLACK
#define WATER_COLOR BLUE
#define SAND_COLOR GOLD
#define SPAWN_RADIUS 15

typedef enum {
	EMPTY,
	SAND,
	WATER,
} CellType;

typedef struct {
	CellType cell;
	Color color;
	float velocity;
	float density;
	bool alive;
	bool changed;
} Cell;

Cell GRID[GRID_WIDTH][GRID_HEIGHT] = { 0 };
Cell BUFFER[GRID_WIDTH][GRID_HEIGHT] = { 0 };

void Initialize_Grid(Cell GRID[GRID_WIDTH][GRID_HEIGHT]);
void renderGrid();
void updateGrid();
void copyGRID(Cell GRID[GRID_WIDTH][GRID_HEIGHT], Cell BUFFER[GRID_WIDTH][GRID_HEIGHT]);
void spawn_sand(int mouseX, int mouseY);

int main()
{

	InitWindow(GRID_WIDTH, GRID_HEIGHT, "Falling Sand Simulation");
	Initialize_Grid(GRID);
	SetTargetFPS(60);
	
	

	while (!WindowShouldClose())
	{
		BeginDrawing();
		
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			spawn_sand(GetMouseX(), GetMouseY());
		}
		ClearBackground(EMPTY_COLOR);
		updateGrid();
		renderGrid(GRID, BUFFER);
		DrawFPS(10, 10);
		EndDrawing();

		
	}
	return 0;
}


void Initialize_Grid(Cell GRID[GRID_WIDTH][GRID_HEIGHT])
{
	for (int x = 0; x < GRID_WIDTH; x++)
	{
		for (int y = 0; y < GRID_HEIGHT; y++)
		{
			GRID[x][y].cell = EMPTY;
			GRID[x][y].color = EMPTY_COLOR;
			GRID[x][y].alive = false;
			GRID[x][y].changed = false;
		}
	}
}

void renderGrid(Cell GRID[GRID_WIDTH][GRID_HEIGHT], Cell BUFFER[GRID_WIDTH][GRID_HEIGHT]) {


	// Iterate over each cell in the grid
	for (int y = GRID_HEIGHT - 1; y >= 0; y--) 
	{
		for (int x = 0; x < GRID_WIDTH; x++)
		{
			/*
			
			if (GRID[x][y].alive == true)
			{
					// Determine screen coordinates for current cell
					int screenX = x;
					int screenY = y;

					// Determine color for current cell
					Color cellColor = GRID[x][y].color;

					// Draw pixel representing cell on screen
					DrawPixel(screenX, screenY, cellColor);
			}  
			else if(GRID[x][y].cell != BUFFER[x][y].cell)
			{
				// Determine color for current cell
				Color cellColor = GRID[x][y].color;
				
				// Draw pixel representing cell on screen
				DrawPixel(x, y, cellColor);

				GRID[x][y].changed = false;
			}
			*/
			
			if (GRID[x][y].alive)
			{
				DrawPixel(x, y, GRID[x][y].color);
				
			}


			
		}
	}

	
}

void updateGrid() {

	
	// Iterate over each cell in the grid
	for (int y = GRID_HEIGHT - 1; y >= 0; y--) 
	{
		for (int x = 0; x < GRID_WIDTH; x++) 
		{
			if (GRID[x][y].cell == SAND && y < GRID_HEIGHT - 1 && GRID[x][y + 1].cell == EMPTY)
			{
				GRID[x][y].cell = EMPTY;
				GRID[x][y].color = EMPTY_COLOR;
				GRID[x][y].alive = false;
				
				
				GRID[x][y + 1].cell = SAND;
				GRID[x][y + 1].color = SAND_COLOR;
				GRID[x][y + 1].alive = true;
				
				

			}
		}
	}
}

void spawn_sand(int mouseX, int mouseY)
{
	// Define the range for random sand particle spawning
	int minSpawn = 5;
	int maxSpawn = 50;

	// Generate a random number of sand particles to spawn
	int numToSpawn = GetRandomValue(minSpawn, maxSpawn);

	// Spawn the specified number of sand particles
	for (int i = 0; i < numToSpawn; i++)
	{
		// Randomize the position within a certain radius around the mouse position
		int randX = mouseX + GetRandomValue(-SPAWN_RADIUS, SPAWN_RADIUS);
		int randY = mouseY + GetRandomValue(-SPAWN_RADIUS, SPAWN_RADIUS);

		// Ensure the randomized position is within the grid bounds
		if (randX >= 0 && randX < GRID_WIDTH && randY >= 0 && randY < GRID_HEIGHT)
		{
			// Set the cell at the randomized position to SAND
			GRID[randX][randY].cell = SAND;
			GRID[randX][randY].alive = true;
			GRID[randX][randY].color = SAND_COLOR;
		}
	}
}

