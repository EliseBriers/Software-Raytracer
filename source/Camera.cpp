#include "Camera.h"
#include <cmath>

Camera::Camera( const Elite::FVector3& position, float fovDeg )
	: m_Matrix{ Elite::FMatrix4::Identity( ) }
	, m_Fov{ tanf( Elite::ToRadians( fovDeg / 2.f ) ) }
{
	m_Matrix[3].xyz = position;
	SetForward( { 0.f, 0.f, 1.f } );
}

const Elite::FVector3& Camera::GetForward( ) const
{
	return m_Matrix[2].xyz;
}

Elite::FVector3 Camera::GetForwardNoPitch( ) const
{
	Elite::FVector3 forward{ GetForward( ) };
	forward.y = 0.f;
	return GetNormalized( forward );
}

const Elite::FVector3& Camera::GetRight( ) const
{
	return m_Matrix[0].xyz;
}

void Camera::SetForward( const Elite::FVector3& forward )
{
	m_Matrix[2].xyz = GetNormalized( forward );
	m_Matrix[0].xyz = GetNormalized( Cross( Elite::FVector3{ 0.f, 1.f, 0.f }, m_Matrix[2].xyz ) );
	m_Matrix[1].xyz = GetNormalized( Cross( m_Matrix[2].xyz, m_Matrix[0].xyz ) );
}

const Elite::FVector3& Camera::GetPosition( ) const
{
	return m_Matrix[3].xyz;
}

void Camera::SetPosition( const Elite::FPoint3& pos )
{
	m_Matrix[3].xyz = Elite::FVector3{ pos };
}

Elite::FPoint3 Camera::Translate( const Elite::FPoint3& point ) const
{
	Elite::FPoint4 point4{ point };
	return ( m_Matrix * point4 ).xyz;
}

void Camera::Move( const Elite::FVector3& movement )
{
	m_Matrix[3].xyz += movement;
}

void Camera::RotateY( float rotation )
{
	Elite::FVector3 forward{ GetForward( ) };
	Elite::FMatrix3 rotationMatrix{ Elite::MakeRotationY( rotation ) };
	Elite::FVector3 rotated{ rotationMatrix * forward };
	SetForward( rotated );
}

void Camera::RotateX( float rotation )
{
	const Elite::FVector3 forward{ GetForward( ) };
	const Elite::FMatrix3 rotationMatrix{ MakeRotation( rotation, GetRight( ) ) };
	const Elite::FVector3 rotated{ rotationMatrix * forward };
	SetForward( rotated );
}

float Camera::GetFov( ) const
{
	return m_Fov;
}
