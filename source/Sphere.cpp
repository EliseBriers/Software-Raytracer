#include "Sphere.h"

Sphere::Sphere( const Elite::FPoint3& position, IMaterial* pMaterial, float radius )
	: IPrimitive{ pMaterial }
	, m_Position{ position }
	, m_Radius{ radius }
	, m_RadiusSquared{ Elite::Square( radius ) }
	, m_InverseRadius{ 1.f / radius }
{
}

bool Sphere::Hit( const Ray& ray, HitRecord* hitRecord, bool invertCulling ) const
{
	Elite::FVector3 l{ m_Position - ray.origin };
	float tca{ Dot( l, ray.direction ) };
	float od_squared{ SqrMagnitude( Reject( l, ray.direction ) ) };
	float thc_squared{ m_RadiusSquared - od_squared };


	if( thc_squared < 0.f ) return false;

	if( tca < ray.tMin ) return false;

	if( thc_squared > Elite::Square( tca - ray.tMin ) )
		return false;

	if( tca > ray.tMax && thc_squared < Elite::Square( tca - ray.tMax ) )
		return false;

	if( !hitRecord )
		return true;

	float thc{ sqrtf( thc_squared ) };
	float t0{ tca - thc };
	// float t1 { tca + thc };

	hitRecord->distance = t0;
	hitRecord->pMaterial = m_pMaterial;
	hitRecord->position = Elite::FPoint3{ ray.origin + t0 * ray.direction };
	hitRecord->normal = ( hitRecord->position - m_Position ) * m_InverseRadius;
	return true;
}
