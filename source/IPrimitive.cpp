#include "IPrimitive.h"
#include "Materials.h"

IPrimitive::IPrimitive( IMaterial* pMaterial, CullMode cullMode )
	: m_pMaterial{ pMaterial }
	, m_CullMode{ cullMode }
{
}

IPrimitive::~IPrimitive( )
{
	delete m_pMaterial;
}

Elite::FVector3 IPrimitive::GetNormal( const Elite::FVector3& normal, const Elite::FVector3& v, CullMode cullMode )
{
	switch( cullMode )
	{
	case CullMode::frontFace:
		return -normal;
	case CullMode::backFace:
		return normal;
	default:
		return ( Dot( v, normal ) < 0.f ) ? normal : -normal;
	}
}

bool IPrimitive::VerifyCullingMode( float dn, CullMode cullMode )
{
	switch( cullMode )
	{
	case CullMode::frontFace:
		return dn > 0.f;
	case CullMode::backFace:
		return dn < 0.f;
	case CullMode::none:
		return true;
	default:
		return false;
	}
}

CullMode IPrimitive::GetCullingMode( bool invert ) const
{
	if( !invert )
		return m_CullMode;
	switch( m_CullMode )
	{
	case CullMode::frontFace:
		return CullMode::backFace;
	case CullMode::backFace:
		return CullMode::frontFace;
	default:
		return CullMode::none;
	}
}
