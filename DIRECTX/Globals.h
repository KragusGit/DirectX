#include <d3d9.h>
#include <d3dx9.h>
#include "Player.h"
#include <string>
#include <time.h> 
#include "Camera.h"
#include <windows.h>
#include <windowsx.h>
#include <string>
//GLOBALS


using namespace std;


#define SCREEN_WIDTH 1900
#define SCREEN_HEIGHT 1080
static RECT textbox;

void PrintText(char*, int, int, int, DWORD); //custom function to print text

// function prototypes
void initD3D(HWND hWnd);    // sets up and initializes Direct3D
void render_frame(void);    // renders a single frame
void cleanD3D(void);    // closes Direct3D and releases memory
void init_graphics(void);    // 3D declarations
void Draw();
void Camera_Setting();
void init_light(void);    // sets up the light and the material