#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_


#include "raylib.h"


#define GRID_WIDTH 800
#define GRID_HEIGHT 800
#define EMPTY_COLOR BLACK
#define WATER_COLOR BLUE
#define STONE_COLOR GRAY
#define SAND_COLOR GOLD
#define SPAWN_RADIUS 15

//types of cells
typedef enum {
	EMPTY,
	SAND,
	WATER,
	STONE,
} CellType;

//the cell itself, never needed velocity or density, but would want ot add that functionality in the future
typedef struct {
	CellType cell;
	Color color;
	float velocity;
	float density;
	bool alive;
	bool changed;

} Cell;

extern Cell GRID[GRID_WIDTH][GRID_HEIGHT];
extern int pixeltype;
extern bool pixel_change;
extern Sound stoneSound;
extern Sound sandSound;
extern Sound waterSound;
extern Music music;


void Initialize_Grid(Cell GRID[GRID_WIDTH][GRID_HEIGHT]);
void renderGrid();
void updateGrid();
void spawn_sand(int mouseX, int mouseY);
void spawn_water(int mouseX, int mouseY);
void spawn_stone(int mouseX, int mouseY);
void setPixel(Cell* pixel, CellType type, bool alive);
bool isEmpty(int x, int y);
void moveTo(int fromX, int fromY, int toX, int toY);
void switch_pixel();
void pixel_type_text();
void play_snd();
void handle_input();


#endif // !FUNCTIONS_H_