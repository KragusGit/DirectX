#include "GameMan.h"


gameManager::gameManager()
{

	stage = 0;
	difficulty = 1;
	randcount = 0;	
	selectionheight = 350;
	starttime = 0;
	win = 0;
	haltplr = 0;
}
gameManager::~gameManager()
{

}
void gameManager::stage0()
{
	PrintText("COLOUR QUAD!", 60, (SCREEN_WIDTH / 2) / 2-300 , 100, D3DCOLOR_XRGB(255, 255, 255));
	PrintText("WHAT WOULD U LIKE TO DO?", 40, (SCREEN_WIDTH / 2) / 2 - 300, 300, D3DCOLOR_XRGB(255, 255, 255));
	PrintText("1.PLAY", 30, (SCREEN_WIDTH / 2) / 2 - 300, 350, D3DCOLOR_XRGB(255, 255, 255));
	PrintText("2.QUIT", 30, (SCREEN_WIDTH / 2) / 2 - 300, 400, D3DCOLOR_XRGB(255, 255, 255));
	PrintText("->", 30, (SCREEN_WIDTH / 2) / 2 - 450, getsheight(), D3DCOLOR_XRGB(255, 255, 255));
}

void gameManager::stage0_5()
{
	PrintText("-:INSTRUCTIONS:-", 50, 0, 100, D3DCOLOR_XRGB(255, 255, 255));
	PrintText("A RANDOMIZED SEQUENCE OF QUADRANTS WILL BE PROVIDED IN THE BEGINING", 20, 0, 200, D3DCOLOR_XRGB(255, 255, 255));
	PrintText("REMEMBER THAT AND MOVE THE CUBE ACCORDING TO THAT SEQUENCE", 20, 0, 250, D3DCOLOR_XRGB(255, 255, 255));
	PrintText("BUTTONS FOR RESPECTIVE QUADRANTS ARE AS FOLLOWS: ", 20, 0, 300, D3DCOLOR_XRGB(255, 255, 255));
	PrintText("BUTTON Q - Q1 ", 20, 0, 350, D3DCOLOR_XRGB(255, 255, 255));
	PrintText("BUTTON W - Q2 ", 20, 0, 400, D3DCOLOR_XRGB(255, 255, 255));
	PrintText("BUTTON A - Q3 ", 20, 0, 450, D3DCOLOR_XRGB(255, 255, 255));
	PrintText("BUTTON S - Q4 ", 20, 0, 500, D3DCOLOR_XRGB(255, 255, 255));

	PrintText("WHEN READY, PRESS SPACE! ", 40, 0, 600, D3DCOLOR_XRGB(255, 255, 255));
}

void gameManager::stage1()
{
	if (starttime == 0)
	{
		starttime = timeGetTime() / 1000;
	}
CURTIME = timeGetTime()/1000;
elapsed = CURTIME - starttime;
char cd[20];
int countdown;
countdown = 10 - elapsed;
sprintf(cd, "STARTING IN %d", countdown);
if (elapsed < 5)
{
	randar(sequence, diffsize);
}
	int_to_string(sequence, diffsize - 1, disp);


	PrintText(disp, 50, 0, 100, D3DCOLOR_XRGB(255, 255, 255));

	PrintText(cd, 25, -(SCREEN_WIDTH)+200, SCREEN_HEIGHT-100, D3DCOLOR_XRGB(255, 255, 255));
	
	//randcount++;
	if (elapsed>=4)
	{	
		srand(time(NULL));
	}
	if (elapsed>5)
	{
		PrintText("MAKE SURE YOU REMEMBER THIS SEQUENCE!", 50, 0, 150, D3DCOLOR_XRGB(255, 255, 255));
		randcount = 0;
		if (elapsed >10)
		{
			stage = 1.5f;
			elapsed = 0;
			starttime = 0;
		}
	}
	
}

void gameManager::stage1_5(){
	
	if (camera->position.z < -100)
	{
		camera->position.z += 10;
		camera->moveCamera();
	}
	else
	{
		stage = 2;
	}
}
void gameManager::stage2(){
	if (plr1.move_player == 1)
	{
		plr1.move(targetquad);
	}
	if (plr1.peak > 0 && plr1.move_player == 0)
	{

		if (plr1.quad == sequence[plr1.peak - 1])
		{
			if (plr1.peak == (diffsize)){
				
				if (starttime == 0)
				{
					starttime = timeGetTime() / 1000;
				}
				CURTIME = timeGetTime() / 1000;
				elapsed = CURTIME - starttime;

				win = 1;
				haltplr = 1;
				
				if (elapsed == 2)
				{
					difficulty++;
					stage++;
					haltplr = 0; 
					elapsed = 0;
					starttime = 0;
					
				}
		
				
			}
			
		}
		else
		{
			if (starttime == 0)
			{
				starttime = timeGetTime() / 1000;
			}
			CURTIME = timeGetTime() / 1000;
			elapsed = CURTIME - starttime;

			win = 0;
			haltplr=1;
			if (elapsed == 2)
			{
				stage++;
				haltplr = 0;
				elapsed = 0;
				starttime = 0;
			}
		}
	}
}
void gameManager::stage3(){


	if (camera->position.z < 50)
	{
		camera->position.z += 2;
		camera->moveCamera();
	}
	else
	{
		stage++;
	}

}
void gameManager::stage4(){
	switch (win)
	{
	case 1:{
		sprintf(verdict, "%.6s", "WINNER");
		sprintf(nxtp, "%.10s", "NEXT LEVEL");

		
	}break;


	case 0:{

		sprintf(verdict, "%.9s", "GAME OVER");
		sprintf(nxtp, "%.6s", "RETRY");

	}break;
	}

	PrintText(verdict, 60, (SCREEN_WIDTH / 2) / 2 - 300, 100, D3DCOLOR_XRGB(255, 255, 255));
	PrintText("WHAT WOULD U LIKE TO DO?", 30, (SCREEN_WIDTH / 2) / 2 - 300, 300, D3DCOLOR_XRGB(255, 255, 255));
	PrintText(nxtp, 30, (SCREEN_WIDTH / 2) / 2 - 300, 350, D3DCOLOR_XRGB(255, 255, 255));
	PrintText("2.QUIT", 30, (SCREEN_WIDTH / 2) / 2 - 300, 400, D3DCOLOR_XRGB(255, 255, 255));
	PrintText("->", 30, (SCREEN_WIDTH / 2) / 2 - 500, getsheight(), D3DCOLOR_XRGB(255, 255, 255));

}
int gameManager::getsheight()
{
	switch (selection)
	{
	case 1:selectionheight = 350;
		break;
	case 2: selectionheight = 400;
		break;
	}
	return selectionheight;
}



void  gameManager::randar(int ar[], int sz)
{

	int cur = 0;
	int prev = 0;
	for (int i = 0; i <sz;)
	{

		cur = rand() % 4 + 1;
		if (cur != prev)
		{
			ar[i] = cur;
			prev = cur;
			i++;
		}

	}
}


void gameManager::int_to_string(int intar[], int size_of_ar, char temp[])
{
	char *pos = temp;

	for (int i = 0; i <= size_of_ar; i++) {
		if (i == 0)
		{
			pos += sprintf(pos, "Q");
		}
		if (i) {
			pos += sprintf(pos, ",Q");
		}
		pos += sprintf(pos, "%d", intar[i]);
	}


}

