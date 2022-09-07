#pragma once
#include "ERGBColor.h"
#include "EMath.h"

struct HitRecord;
struct Ray;

class ILight
{
public:
	ILight( float intensity, const Elite::RGBColor& color = { 1.f, 1.f, 1.f } );
	virtual Elite::RGBColor GetIrradiance(const Elite::FPoint3& position ) const = 0;
	virtual ~ILight( ) = default;
	virtual std::tuple<Elite::FVector3, float> GetLightDirection( const Elite::FPoint3& position ) const = 0;
	virtual bool VerifyLambertCosine( const Elite::FPoint3& position, const Elite::FVector3& normal ) const = 0;
	bool IsEnabled( ) const;
	void ToggleEnabled( );

	ILight( const ILight& other ) = default;
	ILight( ILight&& other ) noexcept = default;
	ILight& operator=( const ILight& other ) = default;
	ILight& operator=( ILight&& other ) noexcept = default;
protected:
	float m_Intensity{};
	Elite::RGBColor m_Color{};
private:
	bool m_IsEnabled;
};

class PointLight final : public ILight
{
public:
	PointLight( float intensity, const Elite::FPoint3& position, const Elite::RGBColor& color = { 1.f, 1.f, 1.f } );
	Elite::RGBColor GetIrradiance( const Elite::FPoint3& position ) const override;
	virtual std::tuple<Elite::FVector3, float> GetLightDirection( const Elite::FPoint3& position ) const override;

	virtual bool VerifyLambertCosine( const Elite::FPoint3& position, const Elite::FVector3& normal ) const override;
	~PointLight( ) override = default;
	const Elite::FPoint3& GetPosition( ) const;
	void SetPosition( const Elite::FPoint3& pos );

	PointLight( const PointLight& other ) = default;
	PointLight( PointLight&& other ) noexcept = default;
	PointLight& operator=( const PointLight& other ) = default;
	PointLight& operator=( PointLight&& other ) noexcept = default;
private:
	Elite::FPoint3 m_Position;
};

class DirectionalLight final: public ILight
{
public:
	DirectionalLight( float intensity, const Elite::FVector3& direction, const Elite::RGBColor& color = { 1.f, 1.f, 1.f } );
	virtual Elite::RGBColor GetIrradiance( const Elite::FPoint3& position ) const override;
	virtual std::tuple<Elite::FVector3, float> GetLightDirection( const Elite::FPoint3& position ) const override;

	virtual bool VerifyLambertCosine( const Elite::FPoint3& position, const Elite::FVector3& normal ) const override;
	virtual ~DirectionalLight( ) override = default;

	DirectionalLight( const DirectionalLight& other ) = default;
	DirectionalLight( DirectionalLight&& other ) noexcept = default;
	DirectionalLight& operator=( const DirectionalLight& other ) = default;
	DirectionalLight& operator=( DirectionalLight&& other ) noexcept = default;
private:
	Elite::FVector3 m_Direction;
};