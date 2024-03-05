#include "functions.h"
#include <stdlib.h>




//initialize every cell to empty
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


//rander the grid, skip cells that aren't alive
void renderGrid() {


	// Iterate over each cell in the grid
	for (int y = GRID_HEIGHT - 1; y >= 0; y--)
	{
		for (int x = 0; x < GRID_WIDTH; x++)
		{
			if (GRID[x][y].alive)
			{
				DrawPixel(x, y, GRID[x][y].color);
			}
		}
	}


}


//goes over each pixel in the array and updates its status before being rendered
void updateGrid() {

	// Iterate over each cell in the grid
	for (int y = GRID_HEIGHT - 1; y >= 0; y--)
	{
		for (int x = 0; x < GRID_WIDTH; x++)
		{


			//if static blocks then just skip
			if (GRID[x][y].cell == EMPTY || GRID[x][y].cell == STONE)
			{
				continue;
			}



			//storing whether the current moves for each type are open
			bool down = isEmpty(x, y + 1);
			bool left = isEmpty(x - 1, y);
			bool right = isEmpty(x + 1, y);
			bool dleft = isEmpty(x - 1, y + 1);
			bool dright = isEmpty(x + 1, y + 1);

			//every pixel moves down
			if (down)
			{
				moveTo(x, y, x, y + 1);

			}
			// if sand
			if (GRID[x][y].cell == SAND && y < GRID_HEIGHT - 1)
			{



				//randomize going left or right
				bool checkLeft = rand() % 2 == 0;

				if (checkLeft)
				{
					if (dleft)
					{
						moveTo(x, y, x - 1, y + 1);
					}
					else if (dright)
					{
						moveTo(x, y, x + 1, y + 1);
					}
				}
				else
				{
					if (dright)
					{
						moveTo(x, y, x + 1, y + 1);
					}
					else if (dleft)
					{
						moveTo(x, y, x - 1, y + 1);
					}
				}



			}
			else if (GRID[x][y].cell == WATER && y < GRID_HEIGHT - 1 && GRID[x][y + 1].cell == WATER)
			{


				// randomize going left or right !! this kinda works, water still prefers going right because of the way iteration is
				bool checkLeft = rand() % 2 == 0;

				if (checkLeft)
				{
					if (right)
					{
						moveTo(x, y, x + 1, y);
					}
					else if (left)
					{
						moveTo(x, y, x - 1, y);
					}
				}
			}





		}
	}
}


//spawns random amount of sand in a square
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


//spawns random amount of water in a square
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


//spawns a block of stone
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


//sets pixel attributes for each type, and whether its alive or not
void setPixel(Cell* pixel, CellType type, bool alive)
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


//check if pixel you want to go to is of Cell type EMPTY
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


// moves pixel and sets the original pixel to empty and dead (not alive)
void moveTo(int fromX, int fromY, int toX, int toY)
{
	GRID[toX][toY] = GRID[fromX][fromY];
	setPixel(&GRID[fromX][fromY], EMPTY, false);
}


// switches your pixel when you move the mouse wheel up or down
void switch_pixel()
{
	//0 - sand
	//1 - water
	//2 - stone
	int scroll = GetMouseWheelMove();

	if (scroll > 0)
	{

		pixeltype++;
		if (pixeltype == 3)
		{
			pixeltype = 0;
		}
		pixel_change = true;
	}
	else if (scroll < 0)
	{
		pixeltype--;
		if (pixeltype < 0)
		{
			pixeltype = 2;

		}

		pixel_change = true;
	}


}


// displays your selected pixel as text on screen
void pixel_type_text()
{
	if (pixeltype == 0)
	{
		DrawText("SAND", 10, 50, 20, SAND_COLOR);
	}
	else if (pixeltype == 1)
	{
		DrawText("WATER", 10, 50, 20, BLUE);
	}
	else if (pixeltype == 2)
	{
		DrawText("STONE", 10, 50, 20, STONE_COLOR);
	}
}


//plays pixel change sound when you change your pixel
void play_snd()
{
	if (pixeltype == 0 && pixel_change == true)
	{
		SetSoundVolume(sandSound, 5);
		PlaySound(sandSound);
		pixel_change = false;
	}
	else if (pixeltype == 1 && pixel_change == true)
	{
		PlaySound(waterSound);
		pixel_change = false;
	}
	else if (pixeltype == 2 && pixel_change == true)
	{
		PlaySound(stoneSound);
		pixel_change = false;
	}
}


// spawns the right pixel based on the int pixeltype
void handle_input()
{
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		if (pixeltype == 0)
		{
			spawn_sand(GetMouseX(), GetMouseY());
		}
		else if (pixeltype == 1)
		{
			spawn_water(GetMouseX(), GetMouseY());
		}
		else if (pixeltype == 2)
		{
			spawn_stone(GetMouseX(), GetMouseY());
		}

	}
}