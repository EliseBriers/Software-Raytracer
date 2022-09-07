#pragma once
#include "EMath.h"
#include "ERGBColor.h"

struct HitRecord;


class IMaterial  // NOLINT(hicpp-special-member-functions)
{
public:
	virtual Elite::RGBColor Shade( const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v ) const = 0;
	virtual ~IMaterial( ) = default;
};

class Material_Lambert final: public IMaterial
{
public:
	Material_Lambert( float diffuseReflectance, const Elite::RGBColor& diffuseColor );
	virtual Elite::RGBColor Shade( const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v ) const override;
private:
	Elite::RGBColor m_DiffuseColor;
	float m_DiffuseReflectance;
};

class Material_LambertPhong final: public IMaterial
{
public:
	Material_LambertPhong( float diffuseReflectance, const Elite::RGBColor& diffuseColor, float phongExponent );
	virtual Elite::RGBColor Shade( const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v ) const override;
private:
	Elite::RGBColor m_DiffuseColor;
	float m_DiffuseReflectance;
	float m_SpecularReflectance;
	float m_PhongExponent;
};

class Material_PBR final: public IMaterial
{
public:
	Material_PBR( const Elite::RGBColor& albedo, bool isMetal, float roughness );
	virtual Elite::RGBColor Shade( const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v ) const override;
private:
	Elite::RGBColor m_Albedo;
	Elite::RGBColor m_F0;
	float m_A;
	float m_K;
	bool m_IsMetal;
};