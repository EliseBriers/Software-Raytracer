#pragma once
#include <vector>
#include "IPrimitive.h"

class ILight;
class IPrimitive;

class SceneGraph
{
public:
	static SceneGraph* GetInstance( );
	static void Cleanup( );

	const std::vector<IPrimitive*>& GetPrimitives( ) const;
	const std::vector<ILight*>& GetLights( ) const;
	void AddPrimitive( IPrimitive* pPrimitive );
	void AddLight( ILight* pLight );
	void ClearScene( );
	
	SceneGraph( const SceneGraph& other ) = delete;
	SceneGraph( SceneGraph&& other ) noexcept = delete;
	SceneGraph& operator=( const SceneGraph& other ) = delete;
	SceneGraph& operator=( SceneGraph&& other ) noexcept = delete;
private:
	SceneGraph( ) = default;
	std::vector<IPrimitive*> m_pPrimitives;
	std::vector<ILight*> m_pLights;
	
	static SceneGraph* m_pInstance;

	~SceneGraph( );
};

