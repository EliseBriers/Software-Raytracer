#include "Plane.h"

Plane::Plane( const Elite::FPoint3& position, const Elite::FVector3& normal, IMaterial* pMaterial, CullMode cullMode )
	: IPrimitive{ pMaterial, cullMode }
	, m_Position{ position }
	, m_Normal{ normal }
{
}

bool Plane::Hit( const Ray& ray, HitRecord* hitRecord, bool invertCulling ) const
{
	float t;
	CullMode culling { GetCullingMode( invertCulling ) };
	if( !Hit( ray, t, m_Position, m_Normal, culling ) )
		return false;

	if( !hitRecord )
		return true;

	hitRecord->distance = t;
	hitRecord->pMaterial = m_pMaterial;
	hitRecord->normal = GetNormal( m_Normal, ray.direction, culling );
	hitRecord->position = ray.origin + t * ray.direction;
	return true;
}

bool Plane::Hit( const Ray& ray, float& distance, const Elite::FPoint3& position, const Elite::FVector3& normal, CullMode cullMode )
{
	float dn{ Dot( ray.direction, normal ) };
	if( !VerifyCullingMode( dn, cullMode ) ) return false;

	Elite::FVector3 rayToPlane{ position - ray.origin };
	distance = Dot( rayToPlane, normal ) / dn;

	return !( distance < ray.tMin || distance > ray.tMax );
}
