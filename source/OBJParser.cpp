#include "OBJParser.h"
#include "TriangleMesh.h"
#include <fstream>
#include <sstream>

TriangleMesh* OBJParser::ParseObjFile( const std::string& fileName, const Elite::FPoint3& pos, IMaterial* pMaterial )
{
	std::ifstream stream{ fileName };

	if( !stream.is_open( ) )
		return nullptr;

	std::vector<Elite::FPoint3> vertexBuffer{};
	std::vector<int> indexBuffer{};

	std::string str;
	while( std::getline( stream, str ) )
	{
		std::stringstream ss{ str };

		char c;
		ss >> c;
		switch( c )
		{
		case 'v':
			Elite::FPoint3 p{};
			ss >> p.x;
			ss >> p.y;
			ss >> p.z;
			vertexBuffer.push_back( p );
			break;
		case 'f':
			int index;
			ss >> index;
			indexBuffer.push_back( index - 1 );
			ss >> index;
			indexBuffer.push_back( index - 1 );
			ss >> index;
			indexBuffer.push_back( index - 1 );
			break;
		default:
			break;
		}
	}

	return new TriangleMesh{
		pos,
		std::move( vertexBuffer ),
		std::move( indexBuffer ),
		pMaterial,
		CullMode::backFace
	};
}
