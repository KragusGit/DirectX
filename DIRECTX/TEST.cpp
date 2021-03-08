// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Camera.h"

class Camera; 
Camera* camera;


// define the screen resolution
#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768

// include the Direct3D Library files
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

// global declarations
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class
LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;    // the pointer to the vertex buffer
LPDIRECT3DINDEXBUFFER9 i_buffer;

// function prototypes
void initD3D(HWND hWnd);    // sets up and initializes Direct3D
void render_frame(void);    // renders a single frame
void cleanD3D(void);    // closes Direct3D and releases memory
void init_graphics(void);    // 3D declarations
void Draw();
void Camera_Setting();
void init_light(void);

struct Movement
{
	float mx;
	float mz;
}mv;

struct CUSTOMVERTEX { FLOAT X, Y, Z; D3DVECTOR NORMAL; };
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_NORMAL  )

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

    while(TRUE)
    {
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if(msg.message == WM_QUIT)
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
    switch(message)
    {
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;


			case WM_KEYDOWN:

            // switch on which key was pressed

            switch( wParam )

            {

                // handle the up arrow key

                case VK_UP:
					camera->position.y += 1.0f;
				//	camera->target.y += 1.0f;
                    camera->moveCamera();


                break;

               case VK_DOWN:

                    camera->position.y -= 1.0f;
					//camera->target.y -= 1.0f;
					camera->moveCamera();

                break;

                case VK_LEFT:

                    camera->position.x += 1.0f;
					//camera->target.x += 1.0f;
					camera->moveCamera();

                break;

                case VK_RIGHT:

                    camera->position.x -= 1.0f;
					//camera->target.x -= 1.0f;
					camera->moveCamera();
                break;

				case VK_NUMPAD8:
					mv.mz++;
					break;

				case VK_NUMPAD5:
					mv.mz--;
					break;

				case VK_NUMPAD4:
					mv.mx--;
					break;
				case VK_NUMPAD6:
					mv.mx++;
					break;

				
            }break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
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
	d3dpp.EnableAutoDepthStencil = TRUE; // Automaticaly runs the z buffer
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16; //16-bit Pixel Format for the z buffer


    // create a device class using this information and the info from the d3dpp stuct
    d3d->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &d3ddev);

	//Create a new Camera
     camera = new Camera(SCREEN_WIDTH,SCREEN_HEIGHT);
     camera->create(d3ddev, 1.0f, 1000.f);
     camera->setLookAt(D3DXVECTOR3(0.0f,0.0f,0.0f));
     camera->setPosition(D3DXVECTOR3(0.0f,0.0f,-50.0f));;

    init_graphics();    // call the function to initialize the triangle
	init_light();
	d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);    // turn off the 3D lighting
    d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);    // both sides of the triangles
    d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer
	d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(255,255,255));
}


// this is the function used to render a single frame
void render_frame(void)
{
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0); //CLEAR Z BUFFER

    d3ddev->BeginScene();
	Draw();
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
    {
		{-6.0f,6.0f,-6.0f, }, //0
		{6.0f,6.0f,-6.0f, }, //1
		{-6.0f,-6.0f,-6.0f}, //2
		{ 6.0f, -6.0f, -6.0f },//3
		{ -6.0f, 6.0f, 6.0f },//4
		{ 6.0f, 6.0f, 6.0f },//5
		{ -6.0f, -6.0f, 6.0f },//6
		{ 6.0f, -6.0f, 6.0f }//7

	
    };

    // create a vertex buffer interface called v_buffer
    d3ddev->CreateVertexBuffer(8*sizeof(CUSTOMVERTEX),
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

	short indices[] =
	{
		0, 1, 2, 3,
		1, 5, 3, 7,
		5, 4, 7, 6,
		4, 0, 6, 2,
		4, 5, 0, 1,
		2, 3, 6, 7

	};

	d3ddev->CreateIndexBuffer(24 * sizeof(short),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&i_buffer,
		NULL);

	i_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(indices));
	i_buffer->Unlock();
}

void Draw(){
	Camera_Setting();
	UINT iTime = timeGetTime() % 1000;

	float rotation1 = 0.0f, rotation2 = 0.0f;
rotation1 +=	iTime * (2*D3DX_PI) / 2000.0f;
rotation2 +=	iTime * (2*D3DX_PI) / 1000.0f;

    d3ddev->SetFVF(CUSTOMFVF);

D3DXMATRIX baseMatrix, worldMatrix, rotationMatrix1, rotationMatrix2,translateMatrix;
d3ddev->GetTransform(D3DTS_WORLD, &baseMatrix);

// *D3DXMatrixRotationX(&rotationMatrix2, rotation2) * //
// *D3DXMatrixRotationY(&rotationMatrix1, rotation1) * //



/* OLD INSTANTIONS 
// #1
	worldMatrix = baseMatrix * *D3DXMatrixRotationX(&rotationMatrix2, rotation2) *

		*D3DXMatrixTranslation(&translateMatrix, -28.0f, 24.0f, 0.0f);
	d3ddev->SetTransform(D3DTS_WORLD, &worldMatrix);
    d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);


// #2
	worldMatrix = baseMatrix * 
		*D3DXMatrixRotationY(&rotationMatrix1, rotation1) *
		*D3DXMatrixTranslation(&translateMatrix, 0.0f, 24.0f, 0.0f);
	d3ddev->SetTransform(D3DTS_WORLD, &worldMatrix);
    d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
    d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);


// #3 
	worldMatrix = baseMatrix * *D3DXMatrixRotationX(&rotationMatrix2, rotation2) *
		
		*D3DXMatrixTranslation(&translateMatrix, 28.0f, 24.0f, 0.0f);
	 d3ddev->SetTransform(D3DTS_WORLD, &worldMatrix);
	 d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	 d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

// #4
	worldMatrix = baseMatrix * 
		*D3DXMatrixRotationY(&rotationMatrix1, rotation1) *
		*D3DXMatrixTranslation(&translateMatrix, -28.0f, 0.0f, 0.0f);
	 d3ddev->SetTransform(D3DTS_WORLD, &worldMatrix);
	 d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	 d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

// #5
	worldMatrix = baseMatrix * *D3DXMatrixRotationX(&rotationMatrix2, rotation2) *
		
		*D3DXMatrixTranslation(&translateMatrix, 0.0f, 0.0f, 0.0f);
	 d3ddev->SetTransform(D3DTS_WORLD, &worldMatrix);
	 d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	 d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);


// #6 
	worldMatrix = baseMatrix * 
		 *D3DXMatrixRotationY(&rotationMatrix1, rotation1) * 
		*D3DXMatrixTranslation(&translateMatrix, 28.0f, 0.0f, 0.0f);
	 d3ddev->SetTransform(D3DTS_WORLD, &worldMatrix);
	 d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	 d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);


// #7 
	worldMatrix = baseMatrix * *D3DXMatrixRotationX(&rotationMatrix2, rotation2) *
		
		*D3DXMatrixTranslation(&translateMatrix, -28.0f, -24.0f, 0.0f);
	 d3ddev->SetTransform(D3DTS_WORLD, &worldMatrix);
	 d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	 d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);


// #8 
	worldMatrix = baseMatrix * *D3DXMatrixRotationX(&rotationMatrix2, rotation2) *
		*D3DXMatrixRotationY(&rotationMatrix1, rotation1) *
		*D3DXMatrixTranslation(&translateMatrix, 0.0f, -24.0f, 0.0f);
	 d3ddev->SetTransform(D3DTS_WORLD, &worldMatrix);
	 d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	 d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

// #9 
	worldMatrix = baseMatrix * *D3DXMatrixRotationX(&rotationMatrix2, rotation2) *
		
		*D3DXMatrixTranslation(&translateMatrix,28.0f, -24.0f, 0.0f);
	 d3ddev->SetTransform(D3DTS_WORLD, &worldMatrix);
	 d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	 d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);



d3ddev->SetTransform(D3DTS_WORLD, &baseMatrix);

*/

worldMatrix = baseMatrix * *D3DXMatrixTranslation(&translateMatrix, 0+ mv.mx, 0, 0 + mv.mz);
d3ddev->SetTransform(D3DTS_WORLD, &worldMatrix);
d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
d3ddev->SetIndices(i_buffer);
d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 8, 0, 12);

worldMatrix = baseMatrix * *D3DXMatrixTranslation(&translateMatrix, 10, 0, 0 );
d3ddev->SetTransform(D3DTS_WORLD, &worldMatrix);
d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
d3ddev->SetIndices(i_buffer);
d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0,0,8,0, 6);
d3ddev->SetTransform(D3DTS_WORLD, &baseMatrix);

}

void Camera_Setting(){

	


	/*
   // set the projection
D3DXMATRIX matView;    // the view transform matrix

    D3DXMatrixLookAtLH(&matView,
                       &D3DXVECTOR3 (0.0f, 0.0f, -100.0f),    // the camera position
                       &D3DXVECTOR3 (0.0f, 0.0f, 0.0f),    // the look-at position
                       &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));    // the up direction

    d3ddev->SetTransform(D3DTS_VIEW, &matView);    // set the view transform to matView

    D3DXMATRIX matProjection;     // the projection transform matrix

    D3DXMatrixPerspectiveFovLH(&matProjection,
                               D3DXToRadian(45),    // the horizontal field of view
                               (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
                               1.0f,    // the near view-plane
                               100.0f);    // the far view-plane

    d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection); 

	*/
}


void init_light(void)
{
	D3DLIGHT9 light;
	D3DMATERIAL9 material;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.1f);
	light.Direction = D3DXVECTOR3(-1.0f, -0.3f, -1.0f);
	d3ddev->SetLight(0, &light);
	d3ddev->LightEnable(0, TRUE);


	ZeroMemory(&material, sizeof(&material));
	material.Diffuse = D3DXCOLOR(1.0, 1.0f, 1.0f, 1.0f);
	material.Ambient = (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	d3ddev->SetMaterial(&material);
}




   