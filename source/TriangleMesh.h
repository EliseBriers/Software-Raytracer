#pragma once
#include "IPrimitive.h"
#include <vector>

class TriangleMesh final : public IPrimitive
{
public:
	TriangleMesh( const Elite::FPoint3& position, const std::vector<Elite::FPoint3>& vertexBuffer, const std::vector<int>& indexBuffer, IMaterial* pMaterial = nullptr, CullMode cullMode = CullMode::backFace );
	TriangleMesh( const Elite::FPoint3& position, std::vector<Elite::FPoint3>&& vertexBuffer, std::vector<int>&& indexBuffer, IMaterial* pMaterial = nullptr, CullMode cullMode = CullMode::backFace );
	virtual ~TriangleMesh( ) override = default;
	virtual bool Hit( const Ray& ray, HitRecord* hitRecord, bool invertCulling ) const override;
	void RotateY( float angle );
private:
	using TriangleTuple = std::tuple<const Elite::FPoint3&, const Elite::FPoint3&, const Elite::FPoint3&, const Elite::FVector3&>;
	void Init( const Elite::FPoint3& position );
	void UpdateTranslatedValues( );
	
	const Elite::FPoint3& GetPoint ( int index ) const;
	TriangleTuple GetTriangle( int index ) const;
	void RemoveDuplicateVertices( );
	bool AddVertex( const Elite::FPoint3& vertex, int nextIndex );
	
	Elite::FMatrix4 m_Matrix;
	std::vector<Elite::FPoint3> m_VertexBuffer;
	std::vector<Elite::FPoint3> m_TranslatedVertexBuffer;
	std::vector<Elite::FVector3> m_TranslatedNormalBuffer;
	std::vector<int> m_IndexBuffer;
};

