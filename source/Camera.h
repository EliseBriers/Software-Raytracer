#pragma once
#include "EMath.h"

class Camera
{
public:
	Camera( const Elite::FVector3& position = {}, float fovDeg = 45.f );
	const Elite::FVector3& GetForward( ) const;
	Elite::FVector3 GetForwardNoPitch( ) const;
	const Elite::FVector3& GetRight( ) const;
	void SetForward( const Elite::FVector3& forward );
	const Elite::FVector3& GetPosition( ) const;
	void SetPosition( const Elite::FPoint3& pos );

	Elite::FPoint3 Translate( const Elite::FPoint3& point ) const;

	void Move( const Elite::FVector3& movement );

	void RotateY( float rotation );
	void RotateX( float rotation );

	float GetFov( ) const;
private:
	Elite::FMatrix4 m_Matrix;
	float m_Fov;
};
