#pragma once
#include <d3d9.h>
#include <d3dx9tex.h>

class Camera
{
public:
	Camera(float w,float h);
	~Camera(void);

	bool create(LPDIRECT3DDEVICE9 device, float nearView, float farView);
	void setLookAt(D3DXVECTOR3 newTarget);
	void setPosition(D3DXVECTOR3 newPosition);
	void resetView();

	void            moveCamera();   //move camera
    void            rotateCamera(); //rotate view

	D3DXVECTOR3 target;		// Camera Target
	D3DXVECTOR3 position;		// Position of camera

private:

	D3DXMATRIX viewMatrix; //the view matrix
	D3DXMATRIX projectionMatrix; // the projection matrix
	
	float nearClip;
	float farClip;
	float width;
	float height;
	LPDIRECT3DDEVICE9 myDevice;
};
