#pragma once
#include "ERGBColor.h"
#include "EMath.h"

class IMaterial;

struct HitRecord
{
	Elite::FPoint3 position{};
	Elite::FVector3 normal{};
	IMaterial* pMaterial{};
	float distance{};
};

struct Ray
{
	Elite::FPoint3 origin{};
	Elite::FVector3 direction{};
	float tMin{ 0.000001f };
	float tMax{ FLT_MAX };
};

enum class CullMode
{
	frontFace,
	backFace,
	none
};

class IPrimitive
{
public:
	IPrimitive( IMaterial* pMaterial = nullptr, CullMode cullMode = CullMode::backFace );
	virtual ~IPrimitive( );
	virtual bool Hit( const Ray& ray, HitRecord* hitRecord = nullptr, bool invertCulling = false ) const = 0;
	static Elite::FVector3 GetNormal( const Elite::FVector3& normal, const Elite::FVector3& v, CullMode cullMode );

	IPrimitive( const IPrimitive& other ) = delete;
	IPrimitive( IPrimitive&& other ) noexcept = default;
	IPrimitive& operator=( const IPrimitive& other ) = delete;
	IPrimitive& operator=( IPrimitive&& other ) noexcept = default;
protected:
	static bool VerifyCullingMode( float dn, CullMode cullMode );
	CullMode GetCullingMode( bool invert ) const;
	
	IMaterial* m_pMaterial;

private:
	CullMode m_CullMode;
};
