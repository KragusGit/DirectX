// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>

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
LPDIRECT3DINDEXBUFFER9 i_buffer = NULL;

// function prototypes
void initD3D(HWND hWnd);    // sets up and initializes Direct3D
void render_frame(void);    // renders a single frame
void cleanD3D(void);    // closes Direct3D and releases memory
void init_graphics(void);    // 3D declarations
void Draw();
void Camera_Setting();
void init_light(void);    // sets up the light and the material

struct CUSTOMVERTEX {FLOAT X, Y, Z; DWORD COLOR;};
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

    // create a device class using this information and the info from the d3dpp stuct
    d3d->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &d3ddev);

    init_graphics();    // call the function to initialize the triangle
	init_light();    // call the function to initialize the light and material

    d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);    // turn on the 3D lighting
    d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer
    d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 100));    // ambient light
}


// this is the function used to render a single frame
void render_frame(void)
{
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

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
        { -3.0f, 3.0f, -3.0f, D3DCOLOR_XRGB(100, 0, 0),  },
		{ 3.0f, 3.0f, -3.0f, D3DCOLOR_XRGB(0, 100, 0),  },
		{ -3.0f, -3.0f, -3.0f, D3DCOLOR_XRGB(0, 0, 100),  },
		{ 3.0f, -3.0f, -3.0f, D3DCOLOR_XRGB(50, 0, 0),  },

		{ -3.0f, 3.0f, 3.0f, D3DCOLOR_XRGB(0, 50, 0),  },
		{ 3.0f, 3.0f, 3.0f, D3DCOLOR_XRGB(100, 0, 100),  },
		{ -3.0f, -3.0f, 3.0f, D3DCOLOR_XRGB(0, 0, 100),  },
		{ 3.0f, -3.0f, 3.0f, D3DCOLOR_XRGB(0, 100, 0),  }
       
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

    // create the indices using an int array
	//Using Triangle Strip
   short indices[] = {
0, 1, 2, 3, // Front
1, 5, 3, 7, // Right
4, 5, 0, 1, // Top
5, 4, 7, 6, // Back
4, 0, 6, 2, // Left
2, 3, 6, 7 // Bottom
};

	//Using Triangle List
	/* 
	short indices[] =
{
    0, 1, 2,    // side 1
    2, 1, 3,
    4, 0, 6,    // side 2
    6, 0, 2,
    7, 5, 6,    // side 3
    6, 5, 4,
    3, 1, 7,    // side 4
    7, 1, 5,
    4, 5, 0,    // side 5
    0, 5, 1,
    3, 7, 2,    // side 6
    2, 7, 6,
};
*/
    // create an index buffer interface called i_buffer
    d3ddev->CreateIndexBuffer(24*sizeof(short),
                              0,
                              D3DFMT_INDEX16,
                              D3DPOOL_MANAGED,
                              &i_buffer,
                              NULL);

    // lock i_buffer and load the indices into it
    i_buffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, indices, sizeof(indices));
    i_buffer->Unlock();
}


// this is the function that sets up the lights and materials
void init_light(void)
{
    D3DLIGHT9 light;    // create the light struct
    D3DMATERIAL9 material;    // create the material struct

    ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
    light.Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
    light.Diffuse = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);    // set the light's color
    light.Direction = D3DXVECTOR3(5.0f, -5.0f, 0.0f);

    d3ddev->SetLight(0, &light);    // send the light struct properties to light #0
    d3ddev->LightEnable(0, TRUE);    // turn on light #0

    ZeroMemory(&material, sizeof(D3DMATERIAL9));    // clear out the struct for use
    material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set diffuse color to white
    material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set ambient color to white

    d3ddev->SetMaterial(&material);    // set the globably-used material to &material
}


void Draw(){
	Camera_Setting();
	UINT iTime = timeGetTime() % 1000;

	float rotation1 = 0.0f, rotation2 = 0.0f;
rotation1 +=	iTime * (2*D3DX_PI) / 2000.0f;
rotation2 +=	iTime * (2*D3DX_PI) / 1000.0f;

    d3ddev->SetFVF(CUSTOMFVF);

D3DXMATRIX baseMatrix, worldMatrix, rotationMatrix1, rotationMatrix2,translateMatrix,scaleMatrix;
d3ddev->GetTransform(D3DTS_WORLD, &baseMatrix);

// *D3DXMatrixRotationX(&rotationMatrix2, rotation2) * //
// *D3DXMatrixRotationY(&rotationMatrix1, rotation1) * //
	
// #1
	worldMatrix = baseMatrix * *D3DXMatrixRotationX(&rotationMatrix2, rotation1) *
		*D3DXMatrixScaling(&scaleMatrix, 2.0f, 2.0f, 2.0f) *
		*D3DXMatrixTranslation(&translateMatrix, 10.0f, 0.0f, 0.0f);
	d3ddev->SetTransform(D3DTS_WORLD, &worldMatrix);
   // select the vertex and index buffers to use
    d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
    d3ddev->SetIndices(i_buffer);

    // draw the cube
	//Using Triangle Strip
	for (int i = 0; i < 6; i++) {
    d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 8, i*4,2); 
	}

	//Using Triangle List
	//d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8,0,12); 


d3ddev->SetTransform(D3DTS_WORLD, &baseMatrix);
}

void Camera_Setting(){
   // set the projection
D3DXMATRIX matView;    // the view transform matrix

    D3DXMatrixLookAtLH(&matView,
                       &D3DXVECTOR3 (0.0f, 0.0f, -80.0f),    // the camera position
                       &D3DXVECTOR3 (0.0f, 0.0f, 0.0f),    // the look-at position
                       &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));    // the up direction

    d3ddev->SetTransform(D3DTS_VIEW, &matView);    // set the view transform to matView

    D3DXMATRIX matProjection;     // the projection transform matrix

    D3DXMatrixPerspectiveFovLH(&matProjection,
                               D3DXToRadian(45),    // the horizontal field of view
                               (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
                               0.0f,    // the near view-plane
                               100.0f);    // the far view-plane

    d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection); 
}





   