#include "Globals.h"
static LPD3DXFONT  dxfont;
static LPDIRECT3DDEVICE9 d3ddev;
class Camera;

class gameManager{


	
public:
 
	  Camera *camera;
	  player plr1;
	

	  float stage;
	  int difficulty;
	  int sequence[15];
	  int diffsize;
	  int randcount;
	  int selectionheight;
	  int selection;
	  char nxtp[10];
	  char verdict[10];
	  char disp[100];
	  int win;
	  int haltplr;

	  int targetquad;

	  struct  GUI
	  {
		  char level[20];
		  char difficulty[20];
	  }gui;






	gameManager();
	~gameManager();
	int getsheight();
	UINT elapsed;
	UINT starttime;
	UINT CURTIME;
	void randar(int[], int);
	void int_to_string(int [], int , char []);
	void stage0();
	void stage0_5();
	void stage1();
	void stage1_5();
	void stage2();
	void stage3();
	void stage4();

};