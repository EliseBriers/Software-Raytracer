#include "BRDF.h"

Elite::RGBColor BRDF::Lambert( float diffuseReflectance, const Elite::RGBColor& diffuseColor )
{
	return diffuseColor * diffuseReflectance / float( E_PI );
}

Elite::RGBColor BRDF::Lambert( const Elite::RGBColor& diffuseReflectance, const Elite::RGBColor& diffuseColor )
{
	return diffuseColor * diffuseReflectance / float( E_PI );
}

Elite::RGBColor BRDF::Phong( float specularReflectance, float phongExponent, const Elite::FVector3& l, const Elite::FVector3& v, const Elite::FVector3& n )
{
	Elite::FVector3 reflect{ l - 2 * ( Dot( n, l ) ) * n };
	float cosA{ Dot( reflect, v ) };
	float reflection{ specularReflectance * powf( cosA, phongExponent ) };
	return Elite::RGBColor{ reflection, reflection, reflection };
}

Elite::RGBColor BRDF::Cook_Torrance( float d, const Elite::RGBColor& f, float g, const Elite::FVector3& v, const Elite::FVector3& n, const Elite::FVector3& l )
{
	return ( f * d * g ) / ( 4.f * Dot( v, n ) * Dot( l, n ) );
}

float BRDF::ThrowBridge_Reitz( const Elite::FVector3& n, const Elite::FVector3& h, float a )
{
	float a2{ a * a };
	return a2 / ( float( E_PI ) * Elite::Square( Elite::Square( Dot( n, h ) ) * ( a2 - 1 ) + 1 ) );
}

Elite::RGBColor BRDF::Schlick( const Elite::FVector3& h, const Elite::FVector3& v, const Elite::RGBColor& f0 )
{
	return f0 + ( Elite::RGBColor{ 1.f, 1.f, 1.f } - f0 ) * powf( 1.f - Dot( h, v ), 5 );
}

float BRDF::Schlick_GGX( const Elite::FVector3& n, const Elite::FVector3& v, float k )
{
	float nv{ Dot( n, v ) };
	return nv / ( nv * ( 1 - k ) + k );
}

float BRDF::Smith( const Elite::FVector3& n, const Elite::FVector3& v, const Elite::FVector3& l, float k )
{
	return Schlick_GGX( n, v, k ) * Schlick_GGX( n, l, k );
}
