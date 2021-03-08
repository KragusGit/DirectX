// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <d3d9.h>
#include<d3dx9.h>
// define the screen resolution
#define SCREEN_WIDTH 1900
#define SCREEN_HEIGHT 1080

// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")

// global declarations
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class
LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;    // the pointer to the vertex buffer

// function prototypes
void gameInit(HWND hWnd);    // sets up and initializes Direct3D
void render(void);    // renders a single frame
void cleanD3D(void);    // closes Direct3D and releases memory
void primitive_init(void);    // 3D declarations

struct CUSTOMVERTEX {FLOAT X, Y, Z, rhw; DWORD COLOR;};
#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

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

    hWnd = CreateWindowEx(NULL,
                          L"WindowClass",
                          L"Our Direct3D Program",
                          WS_OVERLAPPEDWINDOW,
                          0, 0,
                          SCREEN_WIDTH, SCREEN_HEIGHT,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hWnd, nCmdShow);

    // set up and initialize Direct3D
    gameInit(hWnd);

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

        render();
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
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void gameInit(HWND hWnd)
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

    // create a device class using this information and the info from the d3dpp stuct
    d3d->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      NULL,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &d3ddev);

    primitive_init();    // call the function to initialize the triangle
}


// this is the function used to render a single frame
void render(void)
{
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET,D3DCOLOR_XRGB(100, 100, 255), 1.0f, 0);	

	// Begin the scene
	if(SUCCEEDED(d3ddev->BeginScene()))
	{

	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	d3ddev->SetFVF(CUSTOMFVF);
	d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	// End the scene
	d3ddev->EndScene();
	}
	else{
	MessageBox(NULL,
               L"BeginScene Fails!!!",
               L"MESSAGE BOX",
               MB_ICONEXCLAMATION | MB_OK);
	}
	// Present the back buffer contents to the display
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
void primitive_init(void)
{
    // create the vertices using the CUSTOMVERTEX struct
   CUSTOMVERTEX vertices[] =
    {
        { 40.0f, 6.5f, 0.5f, 1.0f, D3DCOLOR_XRGB(200, 0, 0), },
        { 65.0f, 50.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 200, 0), },
        { 15.0f, 50.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 0, 0), }
   };

    // create a vertex buffer interface called v_buffer
    d3ddev->CreateVertexBuffer(3*sizeof(CUSTOMVERTEX),
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