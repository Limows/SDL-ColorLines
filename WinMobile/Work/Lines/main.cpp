/****************************************************************************

Color Lines SDL

****************************************************************************/

#define _CRT_SECURE_NO_WARNINGS  // For warning C4996: 'sprintf': This function or variable may be unsafe.

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxBlitFunc.h>

#include <SDL/SDL_rotozoom.h>

using namespace std;

void MenuStart();
void GameStart();
void AboutStart();

#define VERSION_MAJOR	1
#define VERSION_MINOR	5
#define VERSION_BUILD	3105

#define SCREEN_WIDTH	480	
#define SCREEN_HEIGHT   360

#define FONT_WIDTH		  8
#define FONT_HEIGHT		 16

#define POSX_BOARD_LEFT	 130
#define POSX_TILES_LEFT	 (POSX_BOARD_LEFT + 2)
#define POSY_BOARD_TOP	 25
#define POSY_TILES_TOP	 (POSY_BOARD_TOP + 2)
#define POSX_NEXT_LEFT	 (POSX_BOARD_LEFT + 110)

#define PATH_MAX	1024
#define MAX_INT		65535



enum GameMode
{
	GAMEMODE_MENU = 0,
	GAMEMODE_PLAY = 1,
	GAMEMODE_ABOUT = 2
};

enum BallEnum
{
	BALL_NONE	   = 0,
	BALL_RED		= 1,
	BALL_GREEN	  = 2,
	BALL_BLUE	   = 3,
	BALL_YELLOW	 = 4,
	BALL_CYAN	   = 5,
	BALL_MAGENTA	= 6,
	BALL_BROWN	  = 7,
	BALLCOLORCOUNT  = 7
};

SDL_Surface *g_realVideo = NULL;
SDL_Surface *g_pSurface = NULL;
SDL_Surface *g_pSprites = NULL;
SDL_Surface *g_pFont = NULL;
enum GameMode g_GameMode = GAMEMODE_MENU;
int g_okQuit = 0;
int g_Board[9][9];
int g_Score;
int g_KingScore;
int g_NextColors[3];
int g_TileCursorX, g_TileCursorY;
int g_okShowTileCursor = 0;
int g_okTileCursorPhase = 0;
int g_TileSelectX, g_TileSelectY;

double zoomX=0.1;
double zoomY=0.1;

SDL_Rect centerize;

bool perfectSize=false;

double g_ScoreMult = 0;
bool g_PlayWin = true;
bool music_paused = false;

enum SpriteEnum
{
	SPRITE_KING = 0,
	SPRITE_PRETENDER = 1,
	SPRITE_TILE = 2,
	SPRITE_TILE_RED = 3,
	SPRITE_TILE_GREEN = 4,
	SPRITE_TILE_BLUE = 5,
	SPRITE_TILE_YELLOW = 6,
	SPRITE_TILE_CYAN = 7,
	SPRITE_TILE_MAGENTA = 8,
	SPRITE_TILE_BROWN = 9,
	SPRITE_NEXT = 10,
	SPRITE_NEXT_RED = 11,
	SPRITE_NEXT_GREEN = 12,
	SPRITE_NEXT_BLUE = 13,
	SPRITE_NEXT_YELLOW = 14,
	SPRITE_NEXT_CYAN = 15,
	SPRITE_NEXT_MAGENTA = 16,
	SPRITE_NEXT_BROWN = 17,
	SPRITE_COLUMN = 18,
	SPRITE_KING_LOSE = 19,
	SPRITE_PRETENDER_WIN = 20,
	SPRITE_BUTTON = 21,
	SPRITECOUNT = 22
};

int SPRITECOORDS[SPRITECOUNT * 4] =
{
	  1,   1, 70, 94,  // SPRITE_KING
	 72,   1, 66, 94,
	  1,  96, 24, 24,  // SPRITE_TILE
	 26,  96, 24, 24,
	 51,  96, 24, 24,
	 76,  96, 24, 24,
	101,  96, 24, 24,
	126,  96, 24, 24,
	151,  96, 24, 24,
	176,  96, 24, 24,
	  1, 121, 11, 10,  // SPRITE_NEXT
	 13, 121, 11, 10,
	 25, 121, 11, 10,
	 37, 121, 11, 10,
	 49, 121, 11, 10,
	 61, 121, 11, 10,
	 73, 121, 11, 10,
	 85, 121, 11, 10,
	216,  96, 60, 98,
	139,   1, 70, 94,
	210,   1, 66, 94,
	145, 144, 70, 50,
};

void breadRender() 
{
	//?????? ??????????????? ?????????????????? ????????????????????????, ?????????????????? ????????????????????? ??????????????????????????????
	
	
	SDL_Surface *buf = zoomSurface(g_pSurface,zoomX,zoomY,1);
	if(!perfectSize){
		SDL_FreeSurface(buf);
		zoomX=((double)g_realVideo->h) / ((double)g_pSurface->h);
		zoomY=((double)g_realVideo->h) / ((double)g_pSurface->h);
		buf = zoomSurface(g_pSurface,zoomX,zoomY,1);
		while(buf->h > g_realVideo->h||buf->w > g_realVideo->w) 
		{
			SDL_FreeSurface(buf);
			
			zoomX=zoomX-0.01;
			zoomY=zoomY-0.01;
			buf = zoomSurface(g_pSurface,zoomX,zoomY,1);
			
		}

		centerize.h=buf->h;
		centerize.w=buf->w;
		centerize.y=(g_realVideo->h-buf->h)/2;
		centerize.x=(g_realVideo->w-buf->w)/2;
		perfectSize=true;
	}


	SDL_UpperBlit(buf,NULL,g_realVideo,&centerize);
	SDL_FreeSurface(buf);
	
	SDL_Flip(g_realVideo);
}

void DrawSprite(int sprite, int x, int y)
{
	SDL_Rect src, dest;

	int spriteX = SPRITECOORDS[sprite * 4];
	int spriteY = SPRITECOORDS[sprite * 4 + 1];
	int spriteW = SPRITECOORDS[sprite * 4 + 2];
	int spriteH = SPRITECOORDS[sprite * 4 + 3];

	src.x = spriteX;  src.y = spriteY;
	src.w = spriteW; src.h = spriteH;
	dest.x = x;  dest.y = y;
	dest.w = spriteW; dest.h = spriteH;

	SDL_BlitSurface(g_pSprites, &src, g_pSurface, &dest);
	
}

void DrawText(int x, int y, char *str)
{
	int charline, charperline;
	char ch;
	SDL_Rect src, dest;

	if (str == NULL) return;
	if (y >= SCREEN_HEIGHT) return;

	charperline = g_pFont->w / FONT_WIDTH;

	while (*str != 0)
	{
		ch = *str;
		charline = ((int)ch) / charperline;

		src.x = ((int)ch) % charperline * FONT_WIDTH;
		src.y = charline * FONT_HEIGHT;
		src.w = FONT_WIDTH;  src.h = FONT_HEIGHT;

		dest.x = x;  dest.y = y;
		dest.w = FONT_WIDTH;  dest.h = FONT_HEIGHT;

		SDL_BlitSurface(g_pFont, &src, g_pSurface, &dest);
		

		x += FONT_WIDTH;
		if (x >= SCREEN_WIDTH) break;

		str++;
	}
}

void DrawTileCursor()
{
	int x, y;
	int cursorColorLum;
	Uint32 cursorColor;
	SDL_Rect rc;

	x = POSX_TILES_LEFT + g_TileCursorX * 24;
	y = POSY_TILES_TOP + g_TileCursorY * 24;

	cursorColorLum = g_okTileCursorPhase - 16;
	if (cursorColorLum < 0) cursorColorLum = -cursorColorLum;
	cursorColorLum *= 15;
	cursorColor = SDL_MapRGB(g_pSurface->format, cursorColorLum,cursorColorLum,cursorColorLum);

	rc.x = x;  rc.y = y;  rc.w = 24;  rc.h = 1;
	SDL_FillRect(g_pSurface, &rc, cursorColor);
	rc.x = x;  rc.y = y;  rc.w = 1;  rc.h = 24;
	SDL_FillRect(g_pSurface, &rc, cursorColor);
	rc.x = x + 24;  rc.y = y;  rc.w = 1;  rc.h = 24;
	SDL_FillRect(g_pSurface, &rc, cursorColor);
	rc.x = x;  rc.y = y + 24;  rc.w = 24;  rc.h = 1;
	SDL_FillRect(g_pSurface, &rc, cursorColor);

	g_okTileCursorPhase = (g_okTileCursorPhase + 1) % 32;
}

void DrawTileSelect()
{
	int x, y;
	Uint32 cursorColor;
	SDL_Rect rc;

	x = POSX_TILES_LEFT + g_TileSelectX * 24;
	y = POSY_TILES_TOP + g_TileSelectY * 24;

	cursorColor = SDL_MapRGB(g_pSurface->format, 255,255,0);

	rc.x = x;  rc.y = y;  rc.w = 24;  rc.h = 1;
	SDL_FillRect(g_pSurface, &rc, cursorColor);
	rc.x = x;  rc.y = y;  rc.w = 1;  rc.h = 24;
	SDL_FillRect(g_pSurface, &rc, cursorColor);
	rc.x = x + 24;  rc.y = y;  rc.w = 1;  rc.h = 24;
	SDL_FillRect(g_pSurface, &rc, cursorColor);
	rc.x = x;  rc.y = y + 24;  rc.w = 24;  rc.h = 1;
	SDL_FillRect(g_pSurface, &rc, cursorColor);
}

void DrawGameScreen()
{
	int i, j, tile;
	SDL_Rect rc;
	char buf[10];
	int kingPos;
	int pretenderPos;
	int kingSprite;
	int pretenderSprite;

	if (g_Score <= 0)
	{
		pretenderPos = POSY_BOARD_TOP + 220 - 115 - FONT_HEIGHT;
		kingPos = POSY_BOARD_TOP + 220 - 200 - FONT_HEIGHT;
	}
	else
	{
		if (g_Score < g_KingScore - 10 * g_ScoreMult)
		{
			kingPos = POSY_BOARD_TOP + 220 - 100 - int((g_KingScore - g_Score)/(g_ScoreMult)) - FONT_HEIGHT;
			pretenderPos = POSY_BOARD_TOP + 220 - 215 + int((g_KingScore - g_Score)/(g_ScoreMult)) - FONT_HEIGHT;
		}
		else
		{
			kingPos = POSY_BOARD_TOP + 220 - 115 - FONT_HEIGHT;
			pretenderPos = POSY_BOARD_TOP + 220 - 200 - FONT_HEIGHT;
		}
	}

	if (g_Score <= g_KingScore)
	{
		kingSprite = SPRITE_KING;
		pretenderSprite = SPRITE_PRETENDER;
	}
	else
	{
		kingSprite = SPRITE_KING_LOSE;
		pretenderSprite = SPRITE_PRETENDER_WIN;
	}

	// Clear screen
	rc.x = rc.y = 0;  rc.w = SCREEN_WIDTH; rc.h = SCREEN_HEIGHT;
	SDL_FillRect(g_pSurface, &rc, SDL_MapRGB(g_pSurface->format, 0,0,0));

	// Draw board and sprites
	DrawSprite(SPRITE_COLUMN, 19, POSY_BOARD_TOP + 220 - 108 - FONT_HEIGHT);
	DrawSprite(SPRITE_COLUMN, SCREEN_WIDTH - 82, POSY_BOARD_TOP + 220 - 108 - FONT_HEIGHT);
	DrawSprite(kingSprite, 20, kingPos);
	DrawSprite(pretenderSprite, SCREEN_WIDTH - 90, pretenderPos);
	rc.x = POSX_BOARD_LEFT;  rc.y = POSY_BOARD_TOP;
	rc.w = rc.h = 24 * 9 + 4;
	SDL_FillRect(g_pSurface, &rc, SDL_MapRGB(g_pSurface->format, 173,170,173));

	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			tile = SPRITE_TILE + g_Board[i][j];
			DrawSprite(tile, POSX_TILES_LEFT + j * 24, POSY_TILES_TOP + i * 24);

			//sprintf(buf, "%d", g_Board[i][j]);  // DEBUG
			//DrawText(POSX_TILES_LEFT + j * 24, POSY_TILES_TOP + i * 24, buf);  // DEBUG
		}
	}

	// Draw cursor
	if (g_TileSelectX >= 0 && g_TileSelectY >= 0)
	{
		DrawTileSelect();
	}
	if (g_okShowTileCursor)
	{
		DrawTileCursor();
	}

	// Draw buttons
	DrawSprite(SPRITE_BUTTON, 13, SCREEN_HEIGHT - 70);
	DrawText(27, SCREEN_HEIGHT - 40, "Sound");
	DrawSprite(SPRITE_BUTTON, 109, SCREEN_HEIGHT - 70);
	DrawText(125, SCREEN_HEIGHT - 40, "About");
	DrawSprite(SPRITE_BUTTON, 205, SCREEN_HEIGHT - 70);
	DrawText(210, SCREEN_HEIGHT - 40, "New Game");
	DrawSprite(SPRITE_BUTTON, 301, SCREEN_HEIGHT - 70);
	DrawText(320, SCREEN_HEIGHT - 40, "Back");
	DrawSprite(SPRITE_BUTTON, 397, SCREEN_HEIGHT - 70);
	DrawText(417, SCREEN_HEIGHT - 40, "Exit");

	// Draw score
	sprintf(buf, "%d", g_KingScore);
	DrawText(35, 2, buf);
	sprintf(buf, "% 6d", g_Score);
	DrawText(SCREEN_WIDTH - FONT_WIDTH * 6 - 35, 2, buf);

	// Draw names
	DrawText(35, POSY_BOARD_TOP + 220 - FONT_HEIGHT, "King");
	DrawText((SCREEN_WIDTH - FONT_WIDTH * 9 - 14), POSY_BOARD_TOP + 220 - FONT_HEIGHT, "Pretender");

	// Draw next colors
	DrawText(POSX_NEXT_LEFT - FONT_WIDTH * 6, 2, "Next:");
	DrawSprite(SPRITE_NEXT + g_NextColors[0], POSX_NEXT_LEFT, 4);
	DrawSprite(SPRITE_NEXT + g_NextColors[1],  POSX_NEXT_LEFT + 16, 4);
	DrawSprite(SPRITE_NEXT + g_NextColors[2], POSX_NEXT_LEFT + 32, 4);
}

void GamePutRandomBall(int freecount)
{
	int i, place, ball;
	int *pBoard;

	pBoard = (int*)g_Board;

	if (freecount == 1)
		place = 0;
	else
		place = rand() % freecount;

	freecount = 0;

	for (i = 0; i < 9 * 9; i++)
	{
		if (*(pBoard + i) == BALL_NONE)
		{
			if (freecount == place)
			{
				// Get ball color from "next colors"
				ball = g_NextColors[0];
				g_NextColors[0] = g_NextColors[1];
				g_NextColors[1] = g_NextColors[2];
				g_NextColors[2] = rand() % BALLCOLORCOUNT + 1;

				// Place the ball
				*(pBoard + i) = ball;

				//g_Score++;  //DEBUG
				return;
			}

			freecount++;
		}
	}
}

bool GamePutThreeRandomBalls()
{
	int freecount = 0, n = 3;
	int *pBoard;

	pBoard = (int*)g_Board;

	for (int i = 0; i < 9 * 9; i++)
	{
		if (*(pBoard + i) == BALL_NONE)
			freecount++;
	}

	if (freecount < 3) n = freecount;

	for (int i = 0; i < n; i++)
	{
		GamePutRandomBall(freecount--);
		DrawGameScreen();
		breadRender();
		SDL_Delay(100);
	}

	if (freecount <= 0) return false; // No place to put a ball
	else return true;
}

void SaveScore(int score)
{

	if (g_Score > g_KingScore)
	{
   		//if (getcwd(cwd, sizeof(cwd)) != NULL) 
		//i know this is unsafe
    		ofstream save;
			
  			save.open("\\score.file");
			if (save.rdstate()!=0)
			{
				save.close();
				return;
			}
  			save << score << endl;
  			save.close();
		
	}
	else return;
}

int LoadScore()
{
	int score = 100;

   	//if (getcwd(cwd, sizeof(cwd)) != NULL) 
	//i know this is unsafe
		ifstream save;

  		save.open("\\score.file");
		if (save.rdstate()!=0)
		{
			save.close();
			return 100;
		}
		if(save.is_open())
		{
  			save >> score;
  			save.close();
		}
		else return 100;

		if (score > 100) return score;
		else return 100;
}

bool SaveGame()
{

   //if (getcwd(cwd, sizeof(cwd)) != NULL) 
	//i know this is unsafe

    	ofstream save;

  		save.open("\\save.file");
		if (save.rdstate()!=0)
		{
			save.close();
			return false;
		}
		save << g_Score << endl;
		save.close();
	
	

	return true;
}

bool LoadGame()
{
	int score = 0;

   	//if (getcwd(cwd, sizeof(cwd)) != NULL) 
	//i know this is unsafe
	if(true)
	{	
		ifstream save;

  		save.open("\\save.file");
		if (save.rdstate()!=0)
		{
			save.close();
			return false;
		}
		if(save.is_open())
		{
  			save >> score;
  			save.close();

			g_Score = score;
			GameStart();
			return true;
		}
		else return false;
   	} 
	else return false;
}

void Init()
{	
	// Clear board
	memset(g_Board, 0, sizeof(g_Board));

	// Clear score
	g_Score = 0;
	g_KingScore = LoadScore();
	g_ScoreMult = g_KingScore/100.0;

	//Start music
	//Mix_PlayMusic(g_Music, MAX_INT);
	PlaySound(NULL, 0, 0);
	if(!music_paused){
		PlaySound(L"\\Program Files\\LinesCE\\music.wav",NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
	
}

void GameStart()
{
    if (g_GameMode != GAMEMODE_ABOUT)
    {
		g_TileCursorX = g_TileCursorY = 4;  // Cursor to center of the board
		g_okShowTileCursor = 0;  // Hide cursor
		g_TileSelectX = g_TileSelectY = -1;  // No selection

		g_NextColors[0] = rand() % BALLCOLORCOUNT + 1;
		g_NextColors[1] = rand() % BALLCOLORCOUNT + 1;
		g_NextColors[2] = rand() % BALLCOLORCOUNT + 1;

		DrawGameScreen();
		breadRender();
		SDL_Delay(100);

		// Put first three random balls
		GamePutThreeRandomBalls();

		g_okShowTileCursor = 1;  // Show cursor

		g_GameMode = GAMEMODE_PLAY;
    }
    else
    {
        DrawGameScreen();
        breadRender();
        SDL_Delay(100);
        g_GameMode = GAMEMODE_PLAY;
    }
}

int GameCheckPathExists(int x1, int y1, int x2, int y2)
{
	int i, j;
	int waveboard[9][9];

	memcpy(waveboard, g_Board, sizeof(waveboard));

	waveboard[y1][x1] = -1;  // Wave seed

	for (;;)
	{
		// Wave step
		int okProcess = 0;
		for (i = 0; i < 9; i++)
		{
			for (j = 0; j < 9; j++)
			{
				if (waveboard[i][j] != 0) continue;  // Skip balls and wave

				if (j < 8 && waveboard[i][j + 1] < 0)  // Right
					waveboard[i][j] = waveboard[i][j + 1] - 1;
				if (j > 0 && waveboard[i][j - 1] < waveboard[i][j])  // Left
					waveboard[i][j] = waveboard[i][j - 1] - 1;
				if (i < 8 && waveboard[i + 1][j] < waveboard[i][j])  // Down
					waveboard[i][j] = waveboard[i + 1][j] - 1;
				if (i > 0 && waveboard[i - 1][j] < waveboard[i][j])  // Down
					waveboard[i][j] = waveboard[i - 1][j] - 1;

				if (waveboard[i][j] < 0) okProcess = 1;  // Wave still moving
			}
		}

		if (waveboard[y2][x2] < 0)
			return 1;
		if (!okProcess) break;  // Wave stopped
	}

	return 0;
}

int GameMoveSelectedBall()
{
	int ball;

	// Check move rules; return 0 if the move isn't valid
	if (!GameCheckPathExists(g_TileSelectX, g_TileSelectY, g_TileCursorX, g_TileCursorY))
		return 0;

	g_okShowTileCursor = 0;  // Hide cursor

	ball = g_Board[g_TileSelectY][g_TileSelectX];
	g_Board[g_TileSelectY][g_TileSelectX] = BALL_NONE;
	g_TileSelectX = g_TileSelectY = -1;  // Clear selection

	DrawGameScreen();
	breadRender();
	SDL_Delay(100);

	g_Board[g_TileCursorY][g_TileCursorX] = ball;

	DrawGameScreen();
	breadRender();
	SDL_Delay(100);

	g_okShowTileCursor = 1;  // Show cursor

	return 1;
}

int GameCheckToRemove()
{
	int i, j, c, dx, dy;
	int ball, count;
	int bestball, bestcount, bestx = 0, besty = 0, bestdx = 0, bestdy = 0;

	bestball = BALL_NONE;  bestcount = 0;

	// Check horizontal lines
	for (i = 0; i < 9; i++)
	{
		ball = BALL_NONE;  count = 0;
		for (j = 0; j < 9; j++)
		{
			if (ball != BALL_NONE && g_Board[i][j] == ball)
				count++;
			else
			{
				if (count >= 5 && count > bestcount)
				{
					bestball = ball;  bestcount = count;
					bestx = j - count;  besty = i;  bestdx = 1;  bestdy = 0;
				}
				ball = g_Board[i][j];
				count = 1;
			}
		}
		if (count >= 5 && count > bestcount)
		{
			bestball = ball;  bestcount = count;
			bestx = j - count;  besty = i;  bestdx = 1;  bestdy = 0;
		}
	}

	// Check vertical lines
	for (j = 0; j < 9; j++)
	{
		ball = BALL_NONE;  count = 0;
		for (i = 0; i < 9; i++)
		{
			if (ball != BALL_NONE && g_Board[i][j] == ball)
				count++;
			else
			{
				if (count >= 5 && count > bestcount)
				{
					bestball = ball;  bestcount = count;
					bestx = j;  besty = i - count;  bestdx = 0;  bestdy = 1;
				}
				ball = g_Board[i][j];
				count = 1;
			}
		}
		if (count >= 5 && count > bestcount)
		{
			bestball = ball;  bestcount = count;
			bestx = j;  besty = i - count;  bestdx = 0;  bestdy = 1;
		}
	}

	// Check diagonal lines: from SW to NE
	dx = 1;  dy = -1;
	for (c = 0; c < 9; c++)
	{
		i = (c <= 4) ? c + 4 : 8;
		j = (c <= 4) ? 0 : c - 4;

		ball = BALL_NONE;  count = 0;
		for (;;)
		{
			if (ball != BALL_NONE && g_Board[i][j] == ball)
				count++;
			else
			{
				if (count >= 5 && count > bestcount)
				{
					bestball = ball;  bestcount = count;
					bestx = j - count;  besty = i + count;  bestdx = dx;  bestdy = dy;
				}
				ball = g_Board[i][j];
				count = 1;
			}

			i += dy;  j += dx;
			if (i >= 9 || j >= 9)
			{
				if (count >= 5 && count > bestcount)
				{
					bestball = ball;  bestcount = count;
					bestx = j - count;  besty = i + count;  bestdx = dx;  bestdy = dy;
				}
				break;
			}
		}
	}

	// Check diagonal lines: from NW to SE
	dx = 1;  dy = 1;
	for (c = 0; c < 9; c++)
	{
		i = (c <= 4) ? 4 - c : 0;
		j = (c <= 4) ? 0 : c - 4;

		ball = BALL_NONE;  count = 0;
		for (;;)
		{
			if (ball != BALL_NONE && g_Board[i][j] == ball)
				count++;
			else
			{
				if (count >= 5 && count > bestcount)
				{
					bestball = ball;  bestcount = count;
					bestx = j - count;  besty = i - count;  bestdx = dx;  bestdy = dy;
				}
				ball = g_Board[i][j];
				count = 1;
			}

			i += dy;  j += dx;
			if (i >= 9 || j >= 9)
			{
				if (count >= 5 && count > bestcount)
				{
					bestball = ball;  bestcount = count;
					bestx = j - count;  besty = i - count;  bestdx = dx;  bestdy = dy;
				}
				break;
			}
		}
	}

	if (bestball == BALL_NONE)
		return 0;  // Balls to remove was not found

	// Remove the balls
	i = besty;  j = bestx;
	for (c = 0; c < bestcount; c++)
	{
		g_Board[i][j] = BALL_NONE;
		g_Score += 2;

		DrawGameScreen();
		breadRender();
		SDL_Delay(25);

		i += bestdy;  j += bestdx;
	}

	return 1;
}

bool PointOver(int cx, int cy, int x, int y, int w, int h) 
{	
	if((cx<x) || (cx>(x+w)) || (cy<y) || (cy>(y+h))) return false; 
	else return true; 
}

bool ClickOnTile(int mx, int my, int x, int y) 
{ 
	return PointOver(mx, my, POSX_TILES_LEFT+x*24, POSY_TILES_TOP+y*24, 24, 24); 
}

bool ClickSoundButton(int mx, int my)
{
        return PointOver(mx, my, 33, 290, 25, 25);
}

bool ClickAboutButton(int mx, int my)
{
        return PointOver(mx, my, 130, 290, 25, 25);
}

bool ClickNewGameButton(int mx, int my)
{
        return PointOver(mx, my, 225, 290, 25, 25);
}

bool ClickBackButton(int mx, int my)
{
        return PointOver(mx, my, 320, 290, 25, 25);
}

bool ClickExitButton(int mx, int my)
{
        return PointOver(mx, my, 417, 290, 25, 25);
}

void TakeAction(int i, int j) 
{
	g_TileCursorX=i; g_TileCursorY=j;

	if (g_Board[g_TileCursorY][g_TileCursorX] == BALL_NONE) {
		if (g_TileSelectX >= 0 && g_TileSelectY >= 0) { // We have selection and user selects the destination
			if (GameMoveSelectedBall()) { // Check rules and Move the ball
				if (!GameCheckToRemove()) {
					if(!GamePutThreeRandomBalls())
					{	
						GameCheckToRemove();
						SaveScore(g_Score);
						//Mix_PauseMusic();
						PlaySound(NULL, 0, 0);
						MenuStart();
					}
					else
					{
						GameCheckToRemove();
					}
				}
			}
		}
	}
	else 
	{ // If we have a ball under the cursor then select it
		g_TileSelectX = g_TileCursorX;
		g_TileSelectY = g_TileCursorY;
	}
}

bool ClickAt(double x, double y) 
{
	int i, j;

	for(i=0; i<9;i++) 
		for(j=0; j<9;j++)
			if(ClickOnTile(x, y, i, j)) 
			{ 
				TakeAction(i, j); return true; 
			}

	if(ClickSoundButton(x, y))
    {
		if (music_paused) {
			PlaySound(L"\\Program Files\\LinesCE\\music.wav",NULL,(SND_FILENAME | SND_ASYNC) | SND_LOOP);
			music_paused=false;
		}
		else
		{
			PlaySound(NULL, 0, 0);
			music_paused=true;
		}
		
        return true;
    }

    if(ClickAboutButton(x, y))
    {
        AboutStart();
        return true;
    }

    if(ClickBackButton(x, y))
    {
        //Mix_PauseMusic();
		PlaySound(NULL, 0, 0);
		if (!music_paused) {
			PlaySound(L"\\Program Files\\LinesCE\\intro.wav",NULL,(SND_FILENAME | SND_ASYNC) | SND_LOOP);
		}
        SaveScore(g_Score);
        MenuStart();
        return true;
    }

	if(ClickExitButton(x, y))
    {
        SaveScore(g_Score);
		g_okQuit = 1;
        return true;
    }

    if(ClickNewGameButton(x, y))
    {
        Init();
        GameStart();
        return true;
    }

	return false;
}

void GameProcessEvent(SDL_Event evt)
{	
	if (evt.type == SDL_KEYDOWN)
	{
		switch (evt.key.keysym.sym)
		{
		case SDLK_F6:	// F6
			Init();
			GameStart();
			break;
		case SDLK_ESCAPE:  // ESC
			//Mix_PauseMusic();
			PlaySound(NULL, 0, 0);
			MenuStart();
			SaveScore(g_Score);
			break;
		case SDLK_LEFT:	// LEFT
			g_TileCursorX--;
			if (g_TileCursorX < 0) g_TileCursorX = 8;
			break;
		case SDLK_RIGHT:   // RIGHT
			g_TileCursorX++;
			if (g_TileCursorX >= 9) g_TileCursorX = 0;
			break;
		case SDLK_UP:	  // UP
			g_TileCursorY--;
			if (g_TileCursorY < 0) g_TileCursorY = 8;
			break;
		case SDLK_DOWN:	// DOWN
			g_TileCursorY++;
			if (g_TileCursorY >= 9) g_TileCursorY = 0;
			break;
		case SDLK_F4: // F4
			AboutStart();
			break;
		case SDLK_F2: // F2
			if (music_paused) {
				PlaySound(L"\\Program Files\\LinesCE\\music.wav",NULL,(SND_FILENAME | SND_ASYNC )| SND_LOOP);
				music_paused=false;
			}
			else {

				PlaySound(NULL, 0, 0);
				music_paused=true;
			}
			break;
		case SDLK_RETURN:	// ENTER
			if (g_Board[g_TileCursorY][g_TileCursorX] == BALL_NONE)
			{
				if (g_TileSelectX >= 0 && g_TileSelectY >= 0)  // We have selection and user selects the destination
				{
					// Check rules and Move the ball
					if (GameMoveSelectedBall())
					{
						if (!GameCheckToRemove())
						{
							if(!GamePutThreeRandomBalls())
							{	
								GameCheckToRemove();
								SaveScore(g_Score);
								MenuStart();
							}
							else
							{
								GameCheckToRemove();
							}
						}
					}
				}
			}
			else  // If we have a ball under the cursor then select it
			{
				g_TileSelectX = g_TileCursorX;
				g_TileSelectY = g_TileCursorY;
			}
			break;
		default:  // Do nothing
			break;
		}
	} else
	if (evt.type == SDL_MOUSEBUTTONDOWN)
	{
		
		if(evt.button.button==SDL_BUTTON_LEFT) ClickAt((evt.button.x-centerize.x)/zoomX, (evt.button.y-centerize.y)/zoomY);
		
	}
}

void MenuStart()
{
	SDL_Surface *tempSurface;
	SDL_Surface *pSurfaceTitle;
	SDL_Rect src, dest;
	int vAlign = (SCREEN_HEIGHT - 240)/2;
	int hAlign = (SCREEN_WIDTH - 320)/2;
	int vTextAlign = vAlign + 34;

	//Clear screen
	//SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, 0);
	SDL_FillRect(g_pSurface, NULL, 0x000000);

	// Show title screen
	tempSurface = SDL_LoadBMP("\\Program Files\\LinesCE\\title.bmp");

	if (tempSurface != NULL)
	{
		pSurfaceTitle = SDL_DisplayFormat(tempSurface);
		SDL_FreeSurface(tempSurface);

		src.x = 0;  src.y = 0;
		src.w = SCREEN_WIDTH; src.h = SCREEN_HEIGHT;
		dest.x = hAlign;  dest.y = vAlign;
		dest.w = 320; dest.h = 240;
		SDL_BlitSurface(pSurfaceTitle, &src, g_pSurface, &dest);
		

		SDL_FreeSurface(pSurfaceTitle);
	}

	DrawText((SCREEN_WIDTH - FONT_WIDTH * 20)/2, (SCREEN_HEIGHT - FONT_HEIGHT * 2), "Press Enter to start");

	breadRender();

	g_GameMode = GAMEMODE_MENU;
}

void AboutStart()
{
	SDL_Surface *tempSurface;
	SDL_Surface *pSurfaceTitle;
	SDL_Rect src, dest;
	char buffer[20];
	int vAlign = (SCREEN_HEIGHT - 240)/2;
	int hAlign = (SCREEN_WIDTH - 320)/2;
	int vTextAlign = vAlign + 34;

	// Show about screen
	tempSurface = SDL_LoadBMP("\\Program Files\\LinesCE\\about.bmp");

	if (tempSurface != NULL)
	{
		pSurfaceTitle = SDL_DisplayFormat(tempSurface);
		SDL_FreeSurface(tempSurface);

		src.x = 0;  src.y = 0;
		src.w = SCREEN_WIDTH; src.h = SCREEN_HEIGHT;
		dest.x = hAlign;  dest.y = vAlign;
		dest.w = 320; dest.h = 240;
		SDL_BlitSurface(pSurfaceTitle, &src, g_pSurface, &dest);
		
		SDL_FreeSurface(pSurfaceTitle);
	}

	// Draw info
	sprintf(buffer, "Version: %d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);
	DrawText((SCREEN_WIDTH - FONT_WIDTH * 17)/2, vTextAlign + FONT_HEIGHT, buffer);
	DrawText((SCREEN_WIDTH - FONT_WIDTH * 32)/2, vTextAlign + FONT_HEIGHT * 3, "This game was originally written");
	DrawText((SCREEN_WIDTH - FONT_WIDTH * 33)/2, vTextAlign + FONT_HEIGHT * 4 + 2, "for DOS in 1992. The goal of game");
	DrawText((SCREEN_WIDTH - FONT_WIDTH * 29)/2, vTextAlign + FONT_HEIGHT * 5 + 2, "is to score as many points as");
	DrawText((SCREEN_WIDTH - FONT_WIDTH * 30)/2, vTextAlign + FONT_HEIGHT * 6 + 2, "possible by matching the balls");
	DrawText((SCREEN_WIDTH - FONT_WIDTH * 23)/2, vTextAlign + FONT_HEIGHT * 7 + 2, "in lines of 5 or more.");
	DrawText((SCREEN_WIDTH - FONT_WIDTH * 32)/2, vTextAlign + FONT_HEIGHT * 9 + 2," Windows Mobile port by Darkine");

	breadRender();

	g_GameMode = GAMEMODE_ABOUT;
}

void MenuProcessEvent(SDL_Event evt)
{
	if (evt.type == SDL_KEYDOWN)
	{
		switch (evt.key.keysym.sym)
		{
		case SDLK_ESCAPE:  // ESC
			g_okQuit = 1;
			break;
		case SDLK_RETURN:	// ENTER
			Init();
			GameStart();
			break;
		default:  // Do nothing
			break;
		}
	} else
	if (evt.type == SDL_MOUSEBUTTONDOWN) // TOUCH
	{
		if(evt.button.button==SDL_BUTTON_LEFT)
		{	
			Init();
			GameStart();
		}
	}
}

void AboutProcessEvent(SDL_Event evt)
{
	if (evt.type == SDL_KEYDOWN)
	{
		GameStart();
	} else
	if (evt.type == SDL_MOUSEBUTTONDOWN) // TOUCH
	{
		if(evt.button.button==SDL_BUTTON_LEFT)
		{	
			GameStart();
		}
	}
	

}

int main(int argc, char * argv[])
{
	int flags = 0;
	SDL_Event evt;
	SDL_Surface *tempSurface;

	// Init randomizer
	srand(SDL_GetTicks());

	// Init SDL video
	if (SDL_Init(SDL_INIT_VIDEO) < 0) return 255;  // Unable to initialize SDL video
	SDL_WM_SetCaption("Color Lines", "Color Lines");
	SDL_WM_SetIcon(IMG_Load("ismall.png"), NULL);
	
	// Init SDL audio
	if( SDL_Init(SDL_INIT_AUDIO) < 0) return 254; // Unable to initialize SDL audio

	// Prepare screen surface
	
	g_pSurface = SDL_CreateRGBSurface(0,SCREEN_WIDTH, SCREEN_HEIGHT, 16,0,0,0,0);
	int realX = GetSystemMetrics(SM_CXSCREEN);
	int realY = GetSystemMetrics(SM_CYSCREEN);
	//swap X and Y for landscape
	g_realVideo = SDL_SetVideoMode(realY, realX, 16, flags);
	breadRender();
	
	if (g_pSurface == NULL) return 253;  // Unable to set video mode
	SDL_ShowCursor(SDL_ENABLE);

	// Load font
	tempSurface = SDL_LoadBMP("\\Program Files\\LinesCE\\font.bmp");
	if (tempSurface == NULL) return 252;  // Unable to load bitmap
	g_pFont = SDL_DisplayFormat(tempSurface);
	SDL_FreeSurface(tempSurface);

	// Load sprites
	tempSurface = SDL_LoadBMP("\\Program Files\\LinesCE\\sprites.bmp");
	if (tempSurface == NULL) return 252;  // Unable to load bitmap
	g_pSprites = SDL_DisplayFormat(tempSurface);
	SDL_FreeSurface(tempSurface);
	
	MenuStart();

	if (!music_paused) {
		PlaySound(L"\\Program Files\\LinesCE\\intro.wav",NULL,(SND_FILENAME | SND_ASYNC) | SND_LOOP);
	}

	while (!g_okQuit)
	{	
		if (g_GameMode != GAMEMODE_MENU)
		{
			if (g_KingScore < g_Score && g_PlayWin)
			{	
				if (!music_paused) {
					PlaySound(L"\\Program Files\\LinesCE\\win.wav",NULL,SND_FILENAME | SND_ASYNC);
				}
				
				g_PlayWin = false;
			}
		}

		while (SDL_PollEvent(&evt))
		{
			if (evt.type == SDL_QUIT)
			{
				g_okQuit = 1;
				break;
			}
			else
			{	
				switch (g_GameMode)
				{
					case GAMEMODE_MENU:
						MenuProcessEvent(evt);
						break;
					case GAMEMODE_PLAY:
						GameProcessEvent(evt);
						break;
					case GAMEMODE_ABOUT:
						AboutProcessEvent(evt);
						break;
					default:
						break;
				}
			}
		}

		if (g_GameMode == GAMEMODE_PLAY)
		{
			DrawGameScreen();
			breadRender();
		}

		SDL_Delay(20);
	}

	SDL_FreeSurface(g_pSprites);
	SDL_FreeSurface(g_pFont);
	SDL_FreeSurface(g_pSurface);
	PlaySound(NULL, 0, 0);

	SDL_Quit();

	return 0;
}

