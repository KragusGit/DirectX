// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Camera.h"
class Camera;
Camera *cam;

// define the screen resolution
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// include the Direct3D Library files
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

// global declarations
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class
LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;    // the pointer to the vertex buffer

// function prototypes
void initD3D(HWND hWnd);    // sets up and initializes Direct3D
void render_frame(void);    // renders a single frame
void cleanD3D(void);    // closes Direct3D and releases memory
void init_graphics(void);    // 3D declarations
void draw_prem(void); //drawing part
struct CUSTOMVERTEX { FLOAT X, Y, Z; DWORD COLOR; };
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL, L"WindowClass", L"Our Direct3D Program",
		WS_OVERLAPPEDWINDOW, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	initD3D(hWnd);

	// enter the main loop:

	MSG msg;

	while (TRUE)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			break;

		render_frame();
	}

	// clean up DirectX and COM
	cleanD3D();

	return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	

	}

	
	
	
	



	return DefWindowProc(hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	// create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);
	cam = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);
	cam->create(d3ddev, 1.0f, 1000.f);
	cam->setLookAt(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	cam->setPosition(D3DXVECTOR3(0.0f, 0.0f, -100.0f));;


	init_graphics();    // call the function to initialize the triangle

	d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);    // turn off the 3D lighting
	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);    // both sides of the triangles
	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer

}


// this is the function used to render a single frame
void render_frame(void)
{
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	d3ddev->BeginScene();
	draw_prem();
	d3ddev->EndScene();

	d3ddev->Present(NULL, NULL, NULL, NULL);
}


// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	v_buffer->Release();    // close and release the vertex buffer
	d3ddev->Release();    // close and release the 3D device
	d3d->Release();    // close and release Direct3D
}


// this is the function that puts the 3D models into video RAM
void init_graphics(void)
{
	// create the vertices using the CUSTOMVERTEX struct
	CUSTOMVERTEX vertices[] =
	{//F
		{ 0.0f, 0.0f, 10.0f, D3DCOLOR_XRGB(255, 0 ,0), },
		{ 6.0f, 0.0f, 10.0f, D3DCOLOR_XRGB(255, 0 ,0) },
		{ 0.0f, -6.0f, 10.0f, D3DCOLOR_XRGB(255, 0 ,0), },
		//F	
		{0.0f, -6.0f, 10.0f, D3DCOLOR_XRGB(255, 0 ,0) },
		{ 6.0f, 0.0f, 10.0f, D3DCOLOR_XRGB(255, 0 ,0), },
		{ 6.0f, -6.0f, 10.0f, D3DCOLOR_XRGB(255, 0 ,0), },
		//RS	
		{ 6.0f, 0.0f, 10.0f, D3DCOLOR_XRGB(255, 0 ,0) },
		{ 6.0f, 0.0f, 16.0f, D3DCOLOR_XRGB(255, 0 ,0), },
		{ 6.0f, -6.0f, 10.0f, D3DCOLOR_XRGB(255, 0 ,0), },
		//RS
		{ 6.0f, -6.0f, 10.0f, D3DCOLOR_XRGB(255, 0 ,0) },
		{ 6.0f, 0.0f, 16.0f, D3DCOLOR_XRGB(255, 0 ,0) },
		{ 6.0f, -6.0f, 16.0f, D3DCOLOR_XRGB(255, 0 ,0), },
		//B
		{ 6.0f, 0.0f, 16.0f, D3DCOLOR_XRGB(255, 0 ,0), },
		{ 0.0f, 0.0f, 16.0f, D3DCOLOR_XRGB(255, 0 ,0) },
		{ 6.0f, -6.0f, 16.0f, D3DCOLOR_XRGB(255, 0 ,0), },
		//B
		{ 6.0f, -6.0f, 16.0f, D3DCOLOR_XRGB(255, 0 ,0) },
		{ 0.0f, 0.0f, 16.0f, D3DCOLOR_XRGB(255, 0 ,0), },
		{ 0.0f, -6.0f, 16.0f, D3DCOLOR_XRGB(255, 0 ,0), },
		//LS	
		{ 0.0f, 0.0f, 10.0f, D3DCOLOR_XRGB(255, 0 ,0) },
		{ 0.0f, -6.0f, 16.0f, D3DCOLOR_XRGB(255, 0 ,0), },
		{ 0.0f, -6.0f, 10.0f, D3DCOLOR_XRGB(255, 0 ,0), },
		//LS
		{ 0.0f, 0.0f, 10.0f, D3DCOLOR_XRGB(255, 0 ,0) },
		{ 0.0f, 0.0f, 16.0f, D3DCOLOR_XRGB(255, 0 ,0) },
		{ 0.0f, -6.0f, 16.0f, D3DCOLOR_XRGB(255, 0 ,0), },

		//T
		{ 0.0f, 0.0f, 10.0f, D3DCOLOR_XRGB(255, 0 ,0) },
		{ 0.0f, 0.0f, 16.0f, D3DCOLOR_XRGB(255, 0 ,0) },
		{ 6.0f, 0.0f, 10.0f, D3DCOLOR_XRGB(255, 0 ,0) },
		//T
		{ 6.0f, 0.0f, 10.0f, D3DCOLOR_XRGB(255, 0 ,0) },
		{ 0.0f, 0.0f, 16.0f, D3DCOLOR_XRGB(255, 0 ,0) },
		{ 6.0f, 0.0f, 16.0f, D3DCOLOR_XRGB(255, 0 ,0) },
		//BOT
		{ 0.0f, -6.0f, 10.0f, D3DCOLOR_XRGB(255, 0 ,0) },
		{ 0.0f, -6.0f, 16.0f, D3DCOLOR_XRGB(255, 0 ,0) },
		{ 6.0f, -6.0f, 10.0f, D3DCOLOR_XRGB(255, 0 ,0) },
		//BOT
		{ 6.0f, -6.0f, 10.0f, D3DCOLOR_XRGB(255, 0 ,0) },
		{ 0.0f, -6.0f, 16.0f, D3DCOLOR_XRGB(255, 0 ,0) },
		{ 6.0f, -6.0f, 10.0f, D3DCOLOR_XRGB(255, 0 ,0) },






	};

	// create a vertex buffer interface called v_buffer
	d3ddev->CreateVertexBuffer(36 * sizeof(CUSTOMVERTEX),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&v_buffer,
		NULL);

	VOID* pVoid;    // a void pointer

	// lock v_buffer and load the vertices into it
	v_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	v_buffer->Unlock();
}
void camera_stuff()
{
	/*

D3DXMATRIX matView;    // the view transform matrix

	D3DXMatrixLookAtLH(&matView,
		&D3DXVECTOR3(3.0f, 0.0f, -80.0f),    // the camera position
		&D3DXVECTOR3(3.0f, 0.0f, 0.0f),    // the look-at position
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));    // the up direction

	d3ddev->SetTransform(D3DTS_VIEW, &matView);    // set the view transform to matView

	D3DXMATRIX matProjection;     // the projection transform matrix

	D3DXMatrixPerspectiveFovLH(&matProjection,
		D3DXToRadian(45),    // the horizontal field of view
		(FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
		1.0f,    // the near view-plane
		100.0f);    // the far view-plane

	d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);    // set the projection
	*/
}
void draw_prem(void)
{
	camera_stuff();
	UINT iTime = timeGetTime() % 2000;

	float rot = 0.0f;
	rot += iTime * (2 * D3DX_PI) / 2000.0f;
	// select which vertex format we are using
	d3ddev->SetFVF(CUSTOMFVF);


	// SET UP THE PIPELINE

	D3DXMATRIX worldmat, basemat, translationmatrix ;
	//D3DXMatrixIdentity(&g_Transform)
	d3ddev->GetTransform(D3DTS_WORLD, &basemat);


	worldmat = basemat *   *D3DXMatrixTranslation(&translationmatrix, -30, 30, 0) ;
	d3ddev->SetTransform(D3DTS_WORLD, &worldmat);
	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 12);

	worldmat = basemat *  *D3DXMatrixTranslation(&translationmatrix, 30, -30, 0);
	d3ddev->SetTransform(D3DTS_WORLD, &worldmat);
	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 12);


	worldmat = basemat *   *D3DXMatrixTranslation(&translationmatrix, -30, -30, 0);
	d3ddev->SetTransform(D3DTS_WORLD, &worldmat);
	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 12);

	worldmat = basemat *  *D3DXMatrixTranslation(&translationmatrix, 30, 30, 0);
	d3ddev->SetTransform(D3DTS_WORLD, &worldmat);
	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 12);


	d3ddev->SetTransform(D3DTS_WORLD, &basemat);

}