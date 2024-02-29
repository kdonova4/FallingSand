#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define GRID_WIDTH 800
#define GRID_HEIGHT 800
#define EMPTY_COLOR BLACK
#define WATER_COLOR BLUE
#define STONE_COLOR GRAY
#define SAND_COLOR GOLD
#define SPAWN_RADIUS 15

typedef enum {
	EMPTY,
	SAND,
	WATER,
	STONE,
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
void spawn_water(int mouseX, int mouseY);
void spawn_stone(int mouseX, int mouseY);
void setPixel(Cell *pixel, CellType type, bool alive);
bool isEmpty(int x, int y);
void moveTo(int fromX, int fromY, int toX, int toY);

int main()
{
	srand((unsigned int)time(NULL));
	InitWindow(GRID_WIDTH, GRID_HEIGHT, "Falling Sand Simulation");
	Initialize_Grid(GRID);
	SetTargetFPS(1000);
	
	

	while (!WindowShouldClose())
	{
		BeginDrawing();
		
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			spawn_sand(GetMouseX(), GetMouseY());
		}
		if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
		{
			spawn_water(GetMouseX(), GetMouseY());
		}
		if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
		{
			spawn_stone(GetMouseX(), GetMouseY());
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
		}
	}
}

void renderGrid() {


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
			
			if (GRID[x][y].changed || GRID[x][y].alive)
			{
				DrawPixel(x, y, GRID[x][y].color);
				
				
			}


			
		}
	}

	
}



void updateGrid() {

	for (int y = 0; y < GRID_HEIGHT; y++)
	{
		for (int x = 0; x < GRID_WIDTH; x++)
		{
			GRID[x][y].changed = false;
		}
	}


	// Iterate over each cell in the grid
	for (int y = GRID_HEIGHT - 1; y >= 0; y--) 
	{
		for (int x = 0; x < GRID_WIDTH; x++) 
		{
			
			if (GRID[x][y].changed)
			{
				continue;
			}
			
			if (GRID[x][y].cell == EMPTY || GRID[x][y].cell == STONE)
			{
				continue;
			}




			bool down = isEmpty(x, y + 1);
			bool left = isEmpty(x - 1, y);
			bool right = isEmpty(x + 1, y);
			bool dleft = isEmpty(x - 1, y + 1);
			bool dright = isEmpty(x + 1, y + 1);

			if (down)
			{
				moveTo(x, y, x, y + 1);
				setPixel(&GRID[x][y], EMPTY, false);
			}
			if (GRID[x][y].cell == SAND && y < GRID_HEIGHT - 1)
			{
				
				
			

				bool checkLeft = rand() % 2 == 0;
			
				if (checkLeft)
				{
					if (dleft)
					{
						setPixel(&GRID[x - 1][y + 1], SAND, true);
						
					}
					else if (dright)
					{
						setPixel(&GRID[x + 1][y + 1], SAND, true);
						
					}
				}
				else
				{
					if (dright)
					{
						
						setPixel(&GRID[x + 1][y + 1], SAND, true);

					}
					else if (dleft)
					{
						
						setPixel(&GRID[x - 1][y + 1], SAND, true);
					}
				}

				if (down || dleft || dright)
				{
					setPixel(&GRID[x][y], EMPTY, false);
				}
				
			}
			else if (GRID[x][y].cell == WATER && y < GRID_HEIGHT - 1 && GRID[x][y+1].cell == WATER)
			{
				

				// Generate a random integer between 0 and RAND_MAX
				int randomNumber = rand();

				// Calculate a threshold based on the range of possible values
				int threshold = RAND_MAX / 2;

				// Determine the boolean value based on the comparison with the threshold
				bool checkLeft = randomNumber < threshold;

				if (checkLeft)
				{
					if (right)
					{
						moveTo(x, y, x + 1, y);
						setPixel(&GRID[x][y], EMPTY, false);
					}
					else if (left)
					{
						moveTo(x, y, x - 1, y);
						setPixel(&GRID[x][y], EMPTY, false);
					}
				}
			}
			



				
		}
	}
}

void spawn_sand(int mouseX, int mouseY)
{
	// Define the range for random sand particle spawning
	int minSpawn = 5;
	int maxSpawn = 20;

	// Generate a random number of sand particles to spawn
	int numToSpawn = GetRandomValue(minSpawn, maxSpawn);

	// Spawn the specified number of sand particles
	for (int i = 0; i < numToSpawn; i++)
	{
		// Randomize the position within a certain radius around the mouse position
		int randX = mouseX + GetRandomValue(-SPAWN_RADIUS, SPAWN_RADIUS);
		int randY = mouseY + GetRandomValue(-SPAWN_RADIUS, SPAWN_RADIUS);

		// Ensure the randomized position is within the grid bounds
		if (randX >= 0 && randX < GRID_WIDTH && randY >= 0 && randY < GRID_HEIGHT && GRID[randX][randY].cell == EMPTY)
		{
			// Set the cell at the randomized position to SAND
			setPixel(&GRID[randX][randY], SAND, true);
		}
	}
	
	
}
void spawn_water(int mouseX, int mouseY)
{
	
	// Define the range for random sand particle spawning
	int minSpawn = 5;
	int maxSpawn = 20;

	// Generate a random number of sand particles to spawn
	int numToSpawn = GetRandomValue(minSpawn, maxSpawn);

	// Spawn the specified number of sand particles
	for (int i = 0; i < numToSpawn; i++)
	{
		// Randomize the position within a certain radius around the mouse position
		int randX = mouseX + GetRandomValue(-SPAWN_RADIUS, SPAWN_RADIUS);
		int randY = mouseY + GetRandomValue(-SPAWN_RADIUS, SPAWN_RADIUS);

		// Ensure the randomized position is within the grid bounds
		if (randX >= 0 && randX < GRID_WIDTH && randY >= 0 && randY < GRID_HEIGHT && GRID[randX][randY].cell == EMPTY)
		{
			// Set the cell at the randomized position to SAND
			setPixel(&GRID[randX][randY], WATER, true);
		}
	}

}

void spawn_stone(int mouseX, int mouseY)
{
	for (int x = mouseX - SPAWN_RADIUS; x <= mouseX + SPAWN_RADIUS; x++) {
		for (int y = mouseY - SPAWN_RADIUS; y <= mouseY + SPAWN_RADIUS; y++) {
			if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
				setPixel(&GRID[x][y], STONE, true);
			}
		}
	}
	

}


void setPixel(Cell *pixel, CellType type, bool alive)
{
	if (type == SAND)
	{
		pixel->cell = SAND;
		pixel->color = SAND_COLOR;
		pixel->alive = alive;
		pixel->changed = true;

	}
	else if (type == WATER)
	{
		pixel->cell = WATER;
		pixel->color = WATER_COLOR;
		pixel->alive = alive;
		pixel->changed = true;
		
	}
	else if (type == STONE)
	{
		pixel->cell = STONE;
		pixel->color = STONE_COLOR;
		pixel->alive = true;
		pixel->changed = true;
	}
	else
	{
		pixel->cell = EMPTY;
		pixel->color = EMPTY_COLOR;
		pixel->alive = alive;
		pixel->changed = true;
		
	}

}

bool isEmpty(int x, int y)
{
	if (x >= GRID_WIDTH || x < 0)
	{
		return false;
	}
	else if (y >= GRID_HEIGHT || y < 0)
	{
		return false;
	}

	return (GRID[x][y].cell == EMPTY);
}

void moveTo(int fromX, int fromY, int toX, int toY)
{
	GRID[toX][toY] = GRID[fromX][fromY];


}