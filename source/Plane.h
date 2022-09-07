#pragma once
#include "IPrimitive.h"
#include "Triangle.h"

class Plane final : public IPrimitive
{
public:
	Plane( const Elite::FPoint3& position , const Elite::FVector3& normal , IMaterial* pMaterial, CullMode cullMode = CullMode::backFace );

	~Plane( ) override = default;
	virtual bool Hit( const Ray& ray, HitRecord* hitRecord = nullptr, bool invertCulling = false ) const override;

	Plane( const Plane& other ) = delete;
	Plane( Plane&& other ) noexcept = default;
	Plane& operator=( const Plane& other ) = delete;
	Plane& operator=( Plane&& other ) noexcept = default;
	static bool Hit( const Ray& ray, float& distance, const Elite::FPoint3& position, const Elite::FVector3& normal, CullMode cullMode );
private:
	Elite::FPoint3 m_Position;
	Elite::FVector3 m_Normal;
};
