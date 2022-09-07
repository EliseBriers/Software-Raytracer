#pragma once
#include <string>
#include "EMath.h"

class IMaterial;
class TriangleMesh;

namespace OBJParser
{
	TriangleMesh* ParseObjFile( const std::string& fileName, const Elite::FPoint3& pos, IMaterial* pMaterial = nullptr );
};

