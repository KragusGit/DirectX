#include "Player.h"

void player::move(int index)
{	
	float unit_x;
	float unit_y;
	float comparex =0;
	float comparey =0;
	
	switch (index)
	{
	
	case 1: comparex = q1.x;
		comparey = q1.y;
		
		break;
	case 2: comparex = q2.x;
		comparey = q2.y;
		break;
	case 3: comparex = q3.x;
		comparey = q3.y;
		break;
	case 4: comparex = q4.x;
		comparey = q4.y;
		break;

	
	}


	unit_x = comparex - plrPos.x;
	unit_y = comparey - plrPos.y;
	normalize_vec(unit_x, unit_y);

	if(move_player==1)
	{
		if (plrPos.x<comparex || plrPos.x>comparex)
		{
			plrPos.x += unit_x/2;
		}
		if (plrPos.y<comparey || plrPos.y>comparey)
		{
			plrPos.y += unit_y/2;
		}

		
		if ((plrPos.y <= comparey + 0.5 && plrPos.y >= comparey - 0.5) && (plrPos.x <= comparex + 0.5 && plrPos.x >= comparex - 0.5))
		{
			move_player = 0;
			peak++;
			quad = index;
			
		}
	}
}

player::player()
{
	
	//cunstructor

}

player::~player()
{
	//destructor
}

void player::normalize_vec(float& a, float& b)
{
	
	float modlen;

	modlen = sqrtf(((a*a) + (b*b)));

	a = a / modlen;
	b = b / modlen;


}
