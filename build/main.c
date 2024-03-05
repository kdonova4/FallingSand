#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "functions.h"
#include <time.h>



Sound stoneSound;
Sound sandSound;
Sound waterSound;

bool pixel_change = false;
int pixeltype = 0;

Music music;
Cell GRID[GRID_WIDTH][GRID_HEIGHT] = { 0 };

int main()
{
	// seed random number generator
	srand((unsigned int)time(NULL));

	//initialize
	InitAudioDevice();
	InitWindow(GRID_WIDTH, GRID_HEIGHT, "Falling Sand Simulation");
	Initialize_Grid(GRID);
	SetTargetFPS(1000);
	
	//load change sounds
	stoneSound = LoadSound("resources/stone.wav");
	sandSound = LoadSound("resources/sand.wav");
	waterSound = LoadSound("resources/water.wav");
	
	//load music
	music = LoadMusicStream("resources/Zelda.wav");
	

	
	//play music
	PlayMusicStream(music);
	SetMusicVolume(music, 0.1f);
	
	//game loop
	while (!WindowShouldClose())
	{
		BeginDrawing();
		
		switch_pixel();
		
		play_snd(pixeltype);

		handle_input();
		
		UpdateMusicStream(music);

		ClearBackground(EMPTY_COLOR);

		updateGrid();

		renderGrid();

		pixel_type_text();

		DrawFPS(10, 10);

		EndDrawing();

		
	}

	//unload memory
	UnloadSound(stoneSound);
	UnloadSound(waterSound);
	UnloadSound(sandSound);
	UnloadMusicStream(music);
	CloseAudioDevice();
	CloseWindow();
	return 0;
}





