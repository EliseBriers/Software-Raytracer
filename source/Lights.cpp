#include "Lights.h"
#include "IPrimitive.h"
#include "tuple"

ILight::ILight( float intensity, const Elite::RGBColor& color )
	: m_Intensity{ intensity }
	, m_Color{ color }
	, m_IsEnabled{ true }
{
}

bool ILight::IsEnabled( ) const
{
	return m_IsEnabled;
}

void ILight::ToggleEnabled( )
{
	m_IsEnabled = !m_IsEnabled;
}

PointLight::PointLight( float intensity, const Elite::FPoint3& position, const Elite::RGBColor& color )
	: ILight{ intensity, color }
	, m_Position{ position }
{
}

Elite::RGBColor PointLight::GetIrradiance( const Elite::FPoint3& position ) const
{
	return m_Color * ( m_Intensity / SqrDistance( m_Position, position ) );
}

std::tuple<Elite::FVector3, float> PointLight::GetLightDirection( const Elite::FPoint3& position ) const
{
	Elite::FVector3 value{ m_Position - position };
	float distance{ Normalize( value ) };
	return { value, distance };
}

bool PointLight::VerifyLambertCosine( const Elite::FPoint3& position, const Elite::FVector3& normal ) const
{
	return Dot( normal, m_Position - position ) > 0.f;
}

const Elite::FPoint3& PointLight::GetPosition( ) const
{
	return m_Position;
}

void PointLight::SetPosition( const Elite::FPoint3& pos )
{
	m_Position = pos;
}

DirectionalLight::DirectionalLight( float intensity, const Elite::FVector3& direction, const Elite::RGBColor& color )
	: ILight{ intensity, color }
	, m_Direction{ direction }
{
}

Elite::RGBColor DirectionalLight::GetIrradiance( const Elite::FPoint3& position ) const
{
	return m_Color * m_Intensity;
}

std::tuple<Elite::FVector3, float> DirectionalLight::GetLightDirection( const Elite::FPoint3& position ) const
{
	return { -m_Direction, FLT_MAX };
}

bool DirectionalLight::VerifyLambertCosine( const Elite::FPoint3& position, const Elite::FVector3& normal ) const
{
	return Dot( normal, -m_Direction ) > 0.f;
}
