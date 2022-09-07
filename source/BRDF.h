#pragma once
#include "ERGBColor.h"
#include "EMath.h"

namespace BRDF
{
	Elite::RGBColor Lambert( float diffuseReflectance, const Elite::RGBColor& diffuseColor );
	Elite::RGBColor Lambert( const Elite::RGBColor& diffuseReflectance, const Elite::RGBColor& diffuseColor );
	Elite::RGBColor Phong( float specularReflectance, float phongExponent, const Elite::FVector3& l, const Elite::FVector3& v, const Elite::FVector3& n );

	Elite::RGBColor Cook_Torrance( float d, const Elite::RGBColor& f, float g, const Elite::FVector3& v, const Elite::FVector3& n, const Elite::FVector3& l );

	float ThrowBridge_Reitz( const Elite::FVector3& n, const Elite::FVector3& h, float a );
	Elite::RGBColor Schlick( const Elite::FVector3& h, const Elite::FVector3& v, const Elite::RGBColor& f0 );
	float Schlick_GGX( const Elite::FVector3& n, const Elite::FVector3& v, float k );
	float Smith( const Elite::FVector3& n, const Elite::FVector3& v, const Elite::FVector3& l, float k );
}
