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
void setPixel(Cell *pixel, CellType type, bool alive);
bool isEmpty(int x, int y);

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
			setPixel(&GRID[x][y], EMPTY, false);
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
			
			if (GRID[x][y].alive)
			{
				DrawPixel(x, y, GRID[x][y].color);
				
			}


			
		}
	}

	
}

void updateGrid() {

	srand(time(NULL));
	// Iterate over each cell in the grid
	for (int y = GRID_HEIGHT - 1; y >= 0; y--) 
	{
		for (int x = 0; x < GRID_WIDTH; x++) 
		{
			
				bool down = isEmpty(x,       y + 1);
				bool dleft = isEmpty(x - 1,  y + 1);
				bool dright = isEmpty(x + 1, y + 1);
				if (GRID[x][y].cell == SAND && y < GRID_HEIGHT - 1)
				{

					if (dleft && dright)
					{
						int random = rand() % 100 + 1;

						if (random > 50)
						{
							dleft = false;
							dright = true;
						}
						else
						{
							dleft = true;
							dright = false;
						}
					}


					if (down)
					{
							setPixel(&GRID[x][y + 1], SAND, true);
					}
					else if (dleft)
					{
							setPixel(&GRID[x - 1][y], SAND, true);
					}
					else if (dright)
					{
							setPixel(&GRID[x + 1][y], SAND, true);
					}
					
					if (down || dleft || dright)
					{
						setPixel(&GRID[x][y], EMPTY, false);
					}
				}
		}
	}
}

void spawn_sand(int mouseX, int mouseY)
{
	
	if (mouseX >= 0 && mouseX < GRID_WIDTH && mouseY >= 0 && mouseY < GRID_HEIGHT)
	{
		setPixel(&GRID[mouseX][mouseY], SAND, true);

	}
	
}

void setPixel(Cell *pixel, CellType type, bool alive)
{
	if (type == SAND)
	{
		pixel->cell = SAND;
		pixel->color = SAND_COLOR;
		pixel->alive = alive;

	}
	else if (type == WATER)
	{
		pixel->cell = WATER;
		pixel->color = WATER_COLOR;
		pixel->alive = alive;
	}
	else
	{
		pixel->cell = EMPTY;
		pixel->color = EMPTY_COLOR;
		pixel->alive = alive;
		
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