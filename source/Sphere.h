#pragma once
#include "IPrimitive.h"
class Sphere final : public IPrimitive  // NOLINT(hicpp-special-member-functions)
{
public:

	Sphere( const Elite::FPoint3& position, IMaterial* pMaterial, float radius );

	virtual bool Hit( const Ray& ray, HitRecord* hitRecord = nullptr, bool invertCulling = false ) const override;
	virtual ~Sphere( ) override = default;
private:
	Elite::FPoint3 m_Position;
	float m_Radius;
	float m_RadiusSquared;
	float m_InverseRadius;
};

