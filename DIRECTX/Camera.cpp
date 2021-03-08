
#include "Camera.h"


Camera::Camera(float w,float h)
{
width=w;
height=h;
}

Camera::~Camera(void)
{
}


bool Camera::create(LPDIRECT3DDEVICE9 device, float nearView, float farView)
{
	myDevice = device;
	nearClip = nearView;//nearest point at which the objects stop rendering 
	farClip = farView;//farthest point at which the objects stop rendering


	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 0.0f;


	target.x = 0.0f;
	target.y = 0.0f;
	target.z = 0.0f;

	float aspect = width/height;


	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DXToRadian(45), aspect, nearClip, farClip);

	myDevice->SetTransform(D3DTS_PROJECTION, &projectionMatrix);

	return true;
}

		
void Camera::setPosition(D3DXVECTOR3 newPosition)
{
	position = newPosition;
	resetView();
}

	
void Camera::setLookAt(D3DXVECTOR3 newTarget)
{
	target = newTarget;	
	resetView();
} 


//   PURPOSE: resets the camera view, called after the camera position or target is adjusted.

void Camera::resetView(){

	D3DXMatrixLookAtLH(&viewMatrix, 
						&position,		//camera's position
						&target,		//camera's target
						&D3DXVECTOR3(0.0f, 1.0f, 0.0f));// the up direction
	
	myDevice->SetTransform(D3DTS_VIEW, &viewMatrix);
}

void Camera::moveCamera()

{
	
	resetView();

}


void Camera::rotateCamera()

{
	
	resetView();

}
 



