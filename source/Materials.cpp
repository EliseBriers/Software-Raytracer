#include "Materials.h"
#include "BRDF.h"
#include "IPrimitive.h"

Material_Lambert::Material_Lambert( float diffuseReflectance, const Elite::RGBColor& diffuseColor )
	: m_DiffuseColor{ diffuseColor }
	, m_DiffuseReflectance{ diffuseReflectance }
{
}

Elite::RGBColor Material_Lambert::Shade( const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v ) const
{
	return BRDF::Lambert( m_DiffuseReflectance, m_DiffuseColor );
}

Material_LambertPhong::Material_LambertPhong( float diffuseReflectance, const Elite::RGBColor& diffuseColor, float phongExponent )
	: m_DiffuseColor{ diffuseColor }
	, m_DiffuseReflectance{ diffuseReflectance }
	, m_SpecularReflectance{ 1.f - diffuseReflectance }
	, m_PhongExponent{ phongExponent }
{
}

Elite::RGBColor Material_LambertPhong::Shade( const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v ) const
{
	return BRDF::Lambert( m_DiffuseReflectance, m_DiffuseColor )
		+ BRDF::Phong( m_SpecularReflectance, m_PhongExponent, l, v, hitRecord.normal );
}

Material_PBR::Material_PBR( const Elite::RGBColor& albedo, bool isMetal, float roughness )
	: m_Albedo{ albedo }
	, m_F0{ isMetal ? m_Albedo : Elite::RGBColor{ 0.04, 0.04, 0.04 } }
	, m_A{ Elite::Square( roughness ) }
	, m_K{ Elite::Square( m_A + 1 ) / 8.f }
	, m_IsMetal{ isMetal }
{
}

Elite::RGBColor Material_PBR::Shade( const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v ) const
{
	Elite::FVector3 h{ GetNormalized( v + l ) };
	const Elite::FVector3& n{ hitRecord.normal };

	float d{ BRDF::ThrowBridge_Reitz( n, h, m_A ) };
	Elite::RGBColor f{ BRDF::Schlick( h, v, m_F0 ) };
	float g{ BRDF::Smith( n, v, l, m_K ) };

	Elite::RGBColor specular{ BRDF::Cook_Torrance( d, f, g, v, n, l ) };
	if( m_IsMetal )
		return specular;
	Elite::RGBColor kd{ Elite::RGBColor{ 1.f, 1.f, 1.f } - f };
	Elite::RGBColor diffuse{ BRDF::Lambert( kd, m_Albedo ) };
	return specular + diffuse;
}
