
#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <math.h>


class player
{
public:
	int quad;
	int peak=0;
	int move_player;
	struct plrPos
	{
		
		float x;
		float y;
		float z;

	}plrPos;

	struct q1
	{
		float x = -15.0f;
		float y = 15.0f;
	}q1;


	struct q2
	{
		float x=15.0f;
		float y=15.0f;

	}q2;

	struct q3
	{
		float x=-15.0f;
		float y=-15.0f;
	}q3;

	struct q4
	{
		float x=15.0f;
		float y=-15.0f;
	}q4;
	
	void move(int);

	void normalize_vec( float&,float& );

	player();				// Constructor necessary for random generation
	
	~player();
	
	
	
};