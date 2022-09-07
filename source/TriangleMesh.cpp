#include "TriangleMesh.h"
#include "Triangle.h"
#include <unordered_set>
#include <unordered_map>
#include <map>

TriangleMesh::TriangleMesh( const Elite::FPoint3& position, const std::vector<Elite::FPoint3>& vertexBuffer, const std::vector<int>& indexBuffer,
                            IMaterial* pMaterial,
                            CullMode cullMode )
	: IPrimitive{ pMaterial, cullMode }
	, m_Matrix{ Elite::FMatrix4::Identity( ) }
	, m_VertexBuffer{ vertexBuffer }
	, m_IndexBuffer{ indexBuffer }
{
	Init( position );
}

TriangleMesh::TriangleMesh( const Elite::FPoint3& position, std::vector<Elite::FPoint3>&& vertexBuffer, std::vector<int>&& indexBuffer, IMaterial* pMaterial,
                            CullMode cullMode )
	: IPrimitive{ pMaterial, cullMode }
	, m_Matrix{ Elite::FMatrix4::Identity( ) }
	, m_VertexBuffer{ std::move( vertexBuffer ) }
	, m_IndexBuffer{ std::move( indexBuffer ) }
{
	Init( position );
}

bool TriangleMesh::Hit( const Ray& ray, HitRecord* hitRecord, bool invertCulling ) const
{
	Ray rayCopy{ ray };
	HitRecord temp{};
	bool hit{ false };
	CullMode cullMode{ GetCullingMode( invertCulling ) };
	for( size_t i{}; i < m_IndexBuffer.size( ); i += 3 )
	{
		auto [v0, v1, v2, normal]{ GetTriangle( int( i ) ) };

		if( Triangle::Hit( normal, v0, v1, v2, rayCopy, &temp, cullMode ) )
		{
			hit = true;
			rayCopy.tMax = temp.distance;
			if( hitRecord )
				*hitRecord = temp;
		}
	}
	if( hitRecord )
		hitRecord->pMaterial = m_pMaterial;
	return hit;
}

void TriangleMesh::RotateY( float angle )
{
	Elite::FMatrix4 rotationMatrix{ Elite::MakeRotationY( angle ) };
	m_Matrix *= rotationMatrix;
	UpdateTranslatedValues( );
}

void TriangleMesh::Init( const Elite::FPoint3& position )
{
	RemoveDuplicateVertices( );
	m_Matrix[3].xyz = Elite::FVector3{ position };
	m_TranslatedVertexBuffer.reserve( m_VertexBuffer.size( ) );
	m_TranslatedNormalBuffer.reserve( m_IndexBuffer.size( ) / 3 );
	UpdateTranslatedValues( );
	if( m_IndexBuffer.size( ) % 3 != 0 )
		throw std::exception{ "Invalid index buffer" };
}

void TriangleMesh::UpdateTranslatedValues( )
{
	m_TranslatedVertexBuffer.clear( );
	for( const auto& v : m_VertexBuffer )
	{
		m_TranslatedVertexBuffer.push_back( ( m_Matrix * Elite::FPoint4( v ) ).xyz );
	}
	for( size_t i{}; i < m_IndexBuffer.size( ); i += 3 )
	{
		Elite::FPoint3 v0{ GetPoint( m_IndexBuffer[i] ) };
		Elite::FPoint3 v1{ GetPoint( m_IndexBuffer[i + 1] ) };
		Elite::FPoint3 v2{ GetPoint( m_IndexBuffer[i + 2] ) };

		Elite::FVector3 normal{ GetNormalized( Cross( v1 - v0, v2 - v0 ) ) };
		m_TranslatedNormalBuffer.push_back( normal );
	}
}

const Elite::FPoint3& TriangleMesh::GetPoint( int index ) const
{
	return m_TranslatedVertexBuffer[index];
}

TriangleMesh::TriangleTuple TriangleMesh::GetTriangle( int index ) const
{
	return {
		GetPoint( m_IndexBuffer[index] ), GetPoint( m_IndexBuffer[size_t( index ) + 1] ), GetPoint( m_IndexBuffer[size_t( index ) + 2] ),
		m_TranslatedNormalBuffer[index / 3]
	};
}

void TriangleMesh::RemoveDuplicateVertices( )
{
	// Build vertexBuffer with duplicate vertices
	std::vector<Elite::FPoint3> vertices{};
	vertices.reserve( m_IndexBuffer.size( ) );
	for( int index : m_IndexBuffer )
		vertices.push_back( m_VertexBuffer[index] );

	// Reconstruct vertexBuffer and indexBuffer from scratch
	m_VertexBuffer.clear( );
	m_IndexBuffer.clear( );
	int nextIndex{};
	for( const Elite::FPoint3& vertex : vertices )
	{
		if( AddVertex( vertex, nextIndex ) )
			++nextIndex;
	}
}

bool TriangleMesh::AddVertex( const Elite::FPoint3& vertex, int nextIndex )
{
	float maxSqrDistance{ 0.00001f };
	for( int i{}; i < int( m_IndexBuffer.size( ) ); ++i )
	{
		int currentIndex{ m_IndexBuffer[i] };
		Elite::FPoint3 currentVertex{ m_VertexBuffer[currentIndex] };
		if( SqrDistance( vertex, currentVertex ) < maxSqrDistance )
		{
			m_IndexBuffer.push_back( currentIndex );
			return false;
		}
	}

	m_IndexBuffer.push_back( nextIndex );
	m_VertexBuffer.push_back( vertex );
	return true;
}
