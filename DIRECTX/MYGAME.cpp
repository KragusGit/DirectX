// include the basic windows header files and the Direct3D header file

#include "GameMan.h"

gameManager gm;



// define the screen resolution



// include the Direct3D Library files
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

// global declarations
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
//LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class
LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;    // the pointer to the vertex buffer
LPDIRECT3DINDEXBUFFER9 i_buffer = NULL;
LPDIRECT3DVERTEXBUFFER9 v_buffer1 = NULL;
LPDIRECT3DINDEXBUFFER9 i_buffer1 = NULL;
LPDIRECT3DTEXTURE9 PlayerT = NULL;
LPDIRECT3DTEXTURE9 GreenT = NULL;
LPDIRECT3DTEXTURE9 REDT = NULL;
LPDIRECT3DTEXTURE9 WHTT = NULL;
LPDIRECT3DTEXTURE9 BACK = NULL;
LPDIRECT3DSURFACE9 z_buffer = NULL;





void camera_follow();    //to make the camera follow the player

void resetgame();


struct CUSTOMVERTEX { FLOAT X, Y, Z; D3DVECTOR NORMAL; FLOAT u, v; };
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_NORMAL| D3DFVF_TEX1)

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);





LPDIRECT3DTEXTURE9 gettexture(int);
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


	case WM_KEYDOWN:

		// switch on which key was pressed

		switch (wParam)

		{

			// handle the up arrow key

		case VK_UP:if (gm.stage == 0 || gm.stage == 4)
		{
			gm.selection = 1;

		}


				   break;

		case VK_DOWN:if (gm.stage == 0 || gm.stage == 4)
		{
			gm.selection = 2;

		}
					 break;


					 case VK_OEM_2:gm.stage+=0.5; break;


		case VK_SPACE:{

			if (gm.stage == 0.5f)
			{

				gm.stage = 1;
			}
			if (gm.stage == 0)
			{
				switch (gm.selection)
				{
				default:gm.stage = 0.5f; break;
				case 1:gm.stage = 0.5f; break;
				case 2:PostQuitMessage(0);
					return 0; break;
				}
			}
			if (gm.stage == 4)
			{
				switch (gm.selection)
				{
				default:resetgame(); break;
				case 1:resetgame();
					break;
				case 2:PostQuitMessage(0);
					return 0; break;
				}
			}
			break;

		}


		case 0x51:
			if (gm.plr1.move_player == 0 && gm.stage == 2 && gm.haltplr == 0)
			{
				gm.targetquad = 1;
				gm.plr1.move_player = 1;
			} break;
		case 0x57:
			if (gm.plr1.move_player == 0 && gm.stage == 2 && gm.haltplr == 0)
			{
				gm.targetquad = 2;
				gm.plr1.move_player = 1;
			} break;
		case 0x41:
			if (gm.plr1.move_player == 0 && gm.stage == 2 && gm.haltplr == 0)
			{
				gm.targetquad = 3;
				gm.plr1.move_player = 1;
			} break;
		case 0x53:
			if (gm.plr1.move_player == 0 && gm.stage == 2 && gm.haltplr == 0)
			{
				gm.targetquad = 4;
				gm.plr1.move_player = 1;
			} break;


		}break;
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
	d3dpp.EnableAutoDepthStencil = TRUE; // Automaticaly runs the z buffer
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16; //16-bit Pixel Format for the z buffer
	// create a device class using this information and the info from the d3dpp stuct


	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);


	gm.camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);
	gm.camera->create(d3ddev, 1.0f, 1000.f);
	gm.camera->setLookAt(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	gm.camera->setPosition(D3DXVECTOR3(0.0f, 0.0f, -2000.0f));

	init_graphics();    // call the function to initialize the triangle
	init_light();    // call the function to initialize the light and material

	d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);    // turn on the 3D lighting
	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer
	d3ddev->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(100, 100, 100));    // ambient light
	d3ddev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	d3ddev->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 8);					// anisotropic level
	d3ddev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);		// minification
	d3ddev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);			// magnification
	d3ddev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);			// mipmap
}


// this is the function used to render a single frame
void render_frame(void)
{
	
	
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	d3ddev->BeginScene();
	
	switch (gm.difficulty)
	{
	case 1:gm.diffsize = 5; sprintf(gm.gui.level, "LEVEL : %d", gm.difficulty); sprintf(gm.gui.difficulty, "DIFFICLULTY : %.4s", "EASY"); break;
	case 2:gm.diffsize = 7; sprintf(gm.gui.level, "LEVEL : %d", gm.difficulty); sprintf(gm.gui.difficulty, "DIFFICLULTY : %.6s", "MEDIUM"); break;
	case 3:gm.diffsize = 10; sprintf(gm.gui.level, "LEVEL : %d", gm.difficulty); sprintf(gm.gui.difficulty, "DIFFICLULTY : %.4s", "HARD"); break;
	default:gm.diffsize = 12; sprintf(gm.gui.level, "LEVEL : %d", gm.difficulty); sprintf(gm.gui.difficulty, "DIFFICLULTY : %.7s", "EXTREME"); break;
	}

	if (gm.stage == 0)
	{
		gm.stage0();
		Draw();
	}

	if (gm.stage == 0.5)
	{

		gm.stage0_5();
		Draw();
	}
	if (gm.stage == 1.0f)
	{
		gm.stage1();
		Draw();
	}
	if (gm.stage == 1.5f)
	{
		gm.stage1_5();
		Draw();
	}
	if (gm.stage == 2)
	{
		gm.stage2();
		
		PrintText(gm.gui.level,20, SCREEN_WIDTH-200,SCREEN_HEIGHT-200, D3DCOLOR_XRGB(255,255 , 255));
		PrintText(gm.gui.difficulty, 20, SCREEN_WIDTH - 200, SCREEN_HEIGHT - 100, D3DCOLOR_XRGB(255, 255, 255));
		camera_follow();

		Draw();
	}
	if (gm.stage == 3)
	{
		gm.stage3();
		Draw();
	}
	if (gm.stage == 4)
	{
		gm.stage4();

		Draw();
	}
	d3ddev->EndScene();

	d3ddev->Present(NULL, NULL, NULL, NULL);
}


// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	v_buffer->Release();
	v_buffer1->Release();  // close and release the vertex buffer
	i_buffer->Release();
	i_buffer1->Release();
	PlayerT->Release();
	GreenT->Release();
	REDT->Release();
	WHTT->Release();
	BACK->Release();
	d3ddev->Release();    // close and release the 3D device
	d3d->Release();    // close and release Direct3D
}


// this is the function that puts the 3D models into video RAM
void init_graphics(void)
{
	CUSTOMVERTEX vertices1[] =
	{
		{ -3.0f, 3.0f, -6.0f, 0.0f, 0.0f, -1.0f, 0, 0 },    // side 1
		{ 3.0f, 3.0f, -6.0f, 0.0f, 0.0f, -1.0f, 1, 0 },
		{ -3.0f, -3.0f, -6.0f, 0.0f, 0.0f, -1.0f, 0, 1 },
		{ 3.0f, -3.0f, -6.0f, 0.0f, 0.0f, -1.0f, 1, 1 }


	};
	d3ddev->CreateVertexBuffer(6 * sizeof(CUSTOMVERTEX),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&v_buffer1,
		NULL);

	VOID* pVoid1;    // a void pointer

	// lock v_buffer and load the vertices into it
	v_buffer1->Lock(0, 0, (void**)&pVoid1, 0);
	memcpy(pVoid1, vertices1, sizeof(vertices1));
	v_buffer1->Unlock();




	short indices1[] = {

		0, 1, 2,
		2, 1, 3
	};

	d3ddev->CreateIndexBuffer(6 * sizeof(short),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&i_buffer1,
		NULL);

	// lock i_buffer and load the indices into it
	i_buffer1->Lock(0, 0, (void**)&pVoid1, 0);
	memcpy(pVoid1, indices1, sizeof(indices1));
	i_buffer1->Unlock();

	D3DXCreateTextureFromFile(d3ddev, L"Player.jpg", &PlayerT);
	D3DXCreateTextureFromFile(d3ddev, L"WHITETILE.jpg", &WHTT);
	D3DXCreateTextureFromFile(d3ddev, L"REDTILE.png", &REDT);
	D3DXCreateTextureFromFile(d3ddev, L"GREENTILE.jpg", &GreenT);
	D3DXCreateTextureFromFile(d3ddev, L"abc.png", &BACK);
	// create the vertices using the CUSTOMVERTEX struct
	CUSTOMVERTEX vertices[] =
	{
		{ -3.0f, -3.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0, 0 },    // side 1
		{ 3.0f, -3.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1, 0 },
		{ -3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0, 1 },
		{ 3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1, 1 },

		{ -3.0f, -3.0f, -3.0f, 0.0f, 0.0f, -1.0f, 0, 0 },    // side 2
		{ -3.0f, 3.0f, -3.0f, 0.0f, 0.0f, -1.0f, 0, 1 },
		{ 3.0f, -3.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1, 0 },
		{ 3.0f, 3.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1, 1 },

		{ -3.0f, 3.0f, -3.0f, 0.0f, 1.0f, 0.0f, 0, 0 },    // side 3
		{ -3.0f, 3.0f, 3.0f, 0.0f, 1.0f, 0.0f, 0, 1 },
		{ 3.0f, 3.0f, -3.0f, 0.0f, 1.0f, 0.0f, 1, 0 },
		{ 3.0f, 3.0f, 3.0f, 0.0f, 1.0f, 0.0f, 1, 1 },

		{ -3.0f, -3.0f, -3.0f, 0.0f, -1.0f, 0.0f, 0, 0, },    // side 4
		{ 3.0f, -3.0f, -3.0f, 0.0f, -1.0f, 0.0f, 0, 1 },
		{ -3.0f, -3.0f, 3.0f, 0.0f, -1.0f, 0.0f, 1, 0 },
		{ 3.0f, -3.0f, 3.0f, 0.0f, -1.0f, 0.0f, 1, 1 },

		{ 3.0f, -3.0f, -3.0f, 1.0f, 0.0f, 0.0f, 0, 0 },    // side 5
		{ 3.0f, 3.0f, -3.0f, 1.0f, 0.0f, 0.0f, 0, 1 },
		{ 3.0f, -3.0f, 3.0f, 1.0f, 0.0f, 0.0f, 1, 0 },
		{ 3.0f, 3.0f, 3.0f, 1.0f, 0.0f, 0.0f, 1, 1 },

		{ -3.0f, -3.0f, -3.0f, -1.0f, 0.0f, 0.0f, 0, 0 },    // side 6
		{ -3.0f, -3.0f, 3.0f, -1.0f, 0.0f, 0.0f, 0, 1 },
		{ -3.0f, 3.0f, -3.0f, -1.0f, 0.0f, 0.0f, 1, 0 },
		{ -3.0f, 3.0f, 3.0f, -1.0f, 0.0f, 0.0f, 1, 1 },
	};

	// create a vertex buffer interface called v_buffer
	d3ddev->CreateVertexBuffer(24 * sizeof(CUSTOMVERTEX),
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

	short indices[] =
	{
		0, 1, 2,    // side 1
		2, 1, 3,
		4, 5, 6,    // side 2
		6, 5, 7,
		8, 9, 10,    // side 3
		10, 9, 11,
		12, 13, 14,    // side 4
		14, 13, 15,
		16, 17, 18,    // side 5
		18, 17, 19,
		20, 21, 22,    // side 6
		22, 21, 23,
	};

	// create an index buffer interface called i_buffer
	d3ddev->CreateIndexBuffer(36 * sizeof(short),
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
	light.Diffuse = D3DXCOLOR(10.0f, 10.0f, 10.0f, 10.0f);    // set the light's color
	light.Direction = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	d3ddev->SetLight(0, &light);    // send the light struct properties to light #0
	d3ddev->LightEnable(0, TRUE);    // turn on light #0

	ZeroMemory(&material, sizeof(D3DMATERIAL9));    // clear out the struct for use
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set diffuse color to white
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 2.0f);    // set ambient color to white

	d3ddev->SetMaterial(&material);    // set the globably-used material to &material
}


void Draw(){

	UINT iTime = timeGetTime() % 20000;

	float rotation1 = 0.0f, rotation2 = 0.0f;
	rotation1 += iTime * (2 * D3DX_PI) / 10000.0f;
	rotation2 += iTime * (2 * D3DX_PI) / 5000.0f;

	d3ddev->SetFVF(CUSTOMFVF);

	D3DXMATRIX baseMatrix, worldMatrix, translateMatrix, scaleMatrix, trm2,roty,rotx;
	d3ddev->GetTransform(D3DTS_WORLD, &baseMatrix);


	worldMatrix = baseMatrix *
		*D3DXMatrixScaling(&scaleMatrix, 2.0f, 2.0f, 2.0f) *
		*D3DXMatrixTranslation(&translateMatrix, gm.plr1.plrPos.x, gm.plr1.plrPos.y, gm.plr1.plrPos.z);
	d3ddev->SetTransform(D3DTS_WORLD, &worldMatrix);
	// select the vertex and index buffers to use
	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));

	d3ddev->SetIndices(i_buffer);



	d3ddev->SetTexture(0, PlayerT);
	d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);
	if (gm.stage != 2)
	
	{	
		worldMatrix = baseMatrix *  *D3DXMatrixRotationX(&roty, rotation1) * *D3DXMatrixRotationY(&rotx, rotation1) *
			*D3DXMatrixScaling(&scaleMatrix, 3.0f, 3.0f, 3.0f) *
			*D3DXMatrixTranslation(&translateMatrix, 25,-25 , -1900);
		d3ddev->SetTransform(D3DTS_WORLD, &worldMatrix);
		// select the vertex and index buffers to use
		d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));

		d3ddev->SetIndices(i_buffer);



		d3ddev->SetTexture(0, PlayerT);
		d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);

		worldMatrix = baseMatrix *  *D3DXMatrixRotationX(&roty, rotation2) * *D3DXMatrixRotationY(&rotx, rotation2) *
			*D3DXMatrixScaling(&scaleMatrix, 2.0f, 2.0f, 2.0f) *
			*D3DXMatrixTranslation(&translateMatrix, -70, 0, -1500);
		d3ddev->SetTransform(D3DTS_WORLD, &worldMatrix);
		// select the vertex and index buffers to use
		d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));

		d3ddev->SetIndices(i_buffer);



		d3ddev->SetTexture(0, PlayerT);
		d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);

	}
	worldMatrix = baseMatrix *
		*D3DXMatrixScaling(&scaleMatrix, -50.0f, -50.0f, -150.0f) *
		*D3DXMatrixTranslation(&translateMatrix, 0, 0, -100);
	d3ddev->SetTransform(D3DTS_WORLD, &worldMatrix);
	// select the vertex and index buffers to use
	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));

	d3ddev->SetIndices(i_buffer);



	d3ddev->SetTexture(0, BACK);
	d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);

	worldMatrix = baseMatrix *
		*D3DXMatrixScaling(&scaleMatrix, 5.0f, 5.0f, 5.0f)*
		*D3DXMatrixTranslation(&trm2, -15.5f, -15.5f, 50.0f);;
	d3ddev->SetTransform(D3DTS_WORLD, &worldMatrix);
	// select the vertex and index buffers to use
	d3ddev->SetStreamSource(0, v_buffer1, 0, sizeof(CUSTOMVERTEX));
	d3ddev->SetIndices(i_buffer1);

	d3ddev->SetTexture(0, gettexture(3));
	d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);


	worldMatrix = baseMatrix *
		*D3DXMatrixScaling(&scaleMatrix, 5.0f, 5.0f, 5.0f)*
		*D3DXMatrixTranslation(&trm2, 15.5f, 15.5f, 50.0f);;
	d3ddev->SetTransform(D3DTS_WORLD, &worldMatrix);
	// select the vertex and index buffers to use
	d3ddev->SetStreamSource(0, v_buffer1, 0, sizeof(CUSTOMVERTEX));
	d3ddev->SetIndices(i_buffer1);

	d3ddev->SetTexture(0, gettexture(2));
	d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);

	worldMatrix = baseMatrix *
		*D3DXMatrixScaling(&scaleMatrix, 5.0f, 5.0f, 5.0f)*
		*D3DXMatrixTranslation(&trm2, -15.5f, 15.5f, 50.0f);;
	d3ddev->SetTransform(D3DTS_WORLD, &worldMatrix);
	// select the vertex and index buffers to use
	d3ddev->SetStreamSource(0, v_buffer1, 0, sizeof(CUSTOMVERTEX));
	d3ddev->SetIndices(i_buffer1);

	d3ddev->SetTexture(0, gettexture(1));
	d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);

	worldMatrix = baseMatrix *
		*D3DXMatrixScaling(&scaleMatrix, 5.0f, 5.0f, 5.0f)*
		*D3DXMatrixTranslation(&trm2, 15.5f, -15.5f, 50.0f);;
	d3ddev->SetTransform(D3DTS_WORLD, &worldMatrix);
	// select the vertex and index buffers to use
	d3ddev->SetStreamSource(0, v_buffer1, 0, sizeof(CUSTOMVERTEX));
	d3ddev->SetIndices(i_buffer1);

	d3ddev->SetTexture(0, gettexture(4));
	d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);
	d3ddev->SetTransform(D3DTS_WORLD, &baseMatrix);



}


void camera_follow()
{
	if (gm.camera->position.x < gm.plr1.plrPos.x)
	{
		gm.camera->position.x += 0.8f;
		gm.camera->moveCamera();

	}
	if (gm.camera->position.x > gm.plr1.plrPos.x)
	{
		gm.camera->position.x -= 0.8f;
		gm.camera->moveCamera();

	}
	if (gm.camera->position.x < gm.plr1.plrPos.y)
	{
		gm.camera->position.y += 0.8f;
		gm.camera->moveCamera();

	}
	if (gm.camera->position.y > gm.plr1.plrPos.y)
	{
		gm.camera->position.y -= 0.8f;
		gm.camera->moveCamera();

	}

}





LPDIRECT3DTEXTURE9 gettexture(int quad)
{
	if (gm.plr1.move_player == 1)
	{
		return WHTT;
	}
	else
	{
		if (quad == gm.plr1.quad)
		{
			if (gm.plr1.quad == gm.sequence[gm.plr1.peak - 1])
				return GreenT;
			else
			{
				return REDT;
			}
		}
		else
		{
			return WHTT;
		}
	}
}


void resetgame(){
	gm.plr1.move_player = 0;
	gm.stage = 1;
	gm.camera->position.z = -2000;
	gm.camera->moveCamera();
	gm.plr1.quad = NULL;
	gm.plr1.plrPos.x = 0;
	gm.plr1.plrPos.y = 0;
	gm.plr1.peak = 0;
	gm.targetquad = NULL;
}

void PrintText(char* str, int size, int x, int y, DWORD color)
{
	
	SetRect(&textbox, x, y, SCREEN_WIDTH, SCREEN_HEIGHT);
	D3DXCreateFont(d3ddev,    // the D3D Device
		size,    // font height
		0,    // default font width
		FW_BOLD,    // font weight
		1,    // not using MipLevels
		false,    // italic font
		DEFAULT_CHARSET,    // default character set
		OUT_DEFAULT_PRECIS,    // default OutputPrecision,
		DEFAULT_QUALITY,    // default Quality
		DEFAULT_PITCH | FF_DONTCARE,    // default pitch and family
		L"MV Boli",   // use Facename Arial
		&dxfont);    // the font object
	dxfont->DrawTextA(NULL,
		str,
		strlen(str),
		&textbox,
		DT_CENTER,
		color);
	dxfont->Release();
}
