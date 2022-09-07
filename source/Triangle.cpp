#include "Triangle.h"
#include "Plane.h"

Triangle::Triangle( const Elite::FPoint3& pivot, const Elite::FPoint3& v0, const Elite::FPoint3& v1, const Elite::FPoint3& v2, IMaterial* pMaterial,
                    CullMode cullMode )
	: IPrimitive{ pMaterial, cullMode }
	, m_Matrix{ Elite::FMatrix4::Identity( ) }
	, m_Normal{ GetNormalized( Cross( v1 - v0, v2 - v0 ) ) }
	, m_V0{ v0 }
	, m_V1{ v1 }
	, m_V2{ v2 }
	, m_TranslatedNormal{}
	, m_TranslatedV0{}
	, m_TranslatedV1{}
	, m_TranslatedV2{}
{
	m_Matrix[3].xyz = Elite::FVector3{ pivot };
	UpdateTranslatedValues( );
}

bool Triangle::Hit( const Ray& ray, HitRecord* hitRecord, bool invertCulling ) const
{
	if( !Hit( m_TranslatedNormal, m_TranslatedV0, m_TranslatedV1, m_TranslatedV2, ray, hitRecord, GetCullingMode( invertCulling ) ) )
		return false;
	if( !hitRecord )
		return true;

	hitRecord->pMaterial = m_pMaterial;

	return true;
}

bool Triangle::Hit( const Elite::FVector3& normal, const Elite::FPoint3& v0, const Elite::FPoint3& v1, const Elite::FPoint3& v2, const Ray& ray,
                    HitRecord* hitRecord, CullMode cullMode )
{
	float t;
	if( !Plane::Hit( ray, t, v0, normal, cullMode ) )
		return false;

	Elite::FPoint3 intersectionPoint{ ray.origin + ray.direction * t };

	if( !CheckEdge( v0, v1, intersectionPoint, normal ) )
		return false;
	if( !CheckEdge( v1, v2, intersectionPoint, normal ) )
		return false;
	if( !CheckEdge( v2, v0, intersectionPoint, normal ) )
		return false;

	if( !hitRecord )
		return true;

	hitRecord->distance = t;
	hitRecord->normal = IPrimitive::GetNormal( normal, ray.direction, cullMode );
	hitRecord->position = intersectionPoint;
	return true;
}

void Triangle::RotateY( float angle )
{
	Elite::FMatrix4 rotationMatrix{ Elite::MakeRotationY( angle ) };
	m_Matrix *= rotationMatrix;
	UpdateTranslatedValues( );
}

Elite::FPoint3 Triangle::GetTranslatedPoint( const Elite::FPoint3& point ) const
{
	Elite::FPoint4 v{ point };
	return ( m_Matrix * v ).xyz;
}

Elite::FVector3 Triangle::GetNormal( ) const
{
	Elite::FVector4 vector{ m_Normal };
	return ( m_Matrix * vector ).xyz;
}

void Triangle::UpdateTranslatedValues( )
{
	m_TranslatedV0 = GetTranslatedPoint( m_V0 );
	m_TranslatedV1 = GetTranslatedPoint( m_V1 );
	m_TranslatedV2 = GetTranslatedPoint( m_V2 );
	m_TranslatedNormal = GetNormal( );
}

bool Triangle::CheckEdge( const Elite::FPoint3& v0, const Elite::FPoint3& v1, const Elite::FPoint3& p, const Elite::FVector3& normal )
{
	Elite::FVector3 a{ v1 - v0 };
	Elite::FVector3 c{ p - v0 };
	return Dot( normal, Cross( a, c ) ) > 0.f;
}
