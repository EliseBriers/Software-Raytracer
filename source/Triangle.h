#pragma once
#include "IPrimitive.h"

class Triangle final : public IPrimitive
{
public:

	Triangle( const Elite::FPoint3& pivot, const Elite::FPoint3& v0, const Elite::FPoint3& v1, const Elite::FPoint3& v2, IMaterial* pMaterial, CullMode cullMode = CullMode::backFace );

	virtual ~Triangle( ) override = default;
	virtual bool Hit( const Ray& ray, HitRecord* hitRecord, bool invertCulling = false ) const override;
	static bool Hit(const Elite::FVector3& normal, const Elite::FPoint3& v0, const Elite::FPoint3& v1, const Elite::FPoint3& v2, const Ray& ray, HitRecord* hitRecord, CullMode cullMode = CullMode::backFace );
	void RotateY( float angle );
private:
	Elite::FPoint3 GetTranslatedPoint( const Elite::FPoint3& point ) const;
	Elite::FVector3 GetNormal( ) const;
	void UpdateTranslatedValues( );
	static bool CheckEdge( const Elite::FPoint3& v0, const Elite::FPoint3& v1, const Elite::FPoint3& p, const Elite::FVector3& normal );


	Elite::FMatrix4 m_Matrix;
	Elite::FVector3 m_Normal;
	Elite::FPoint3 m_V0;
	Elite::FPoint3 m_V1;
	Elite::FPoint3 m_V2;
	Elite::FVector3 m_TranslatedNormal;
	Elite::FPoint3 m_TranslatedV0;
	Elite::FPoint3 m_TranslatedV1;
	Elite::FPoint3 m_TranslatedV2;
};

