#include "SceneGraph.h"
#include "Lights.h"
#include "IPrimitive.h"

SceneGraph* SceneGraph::m_pInstance { };

SceneGraph* SceneGraph::GetInstance( )
{
	if( m_pInstance )
		return m_pInstance;
	m_pInstance = new SceneGraph { };
	return m_pInstance;
}

void SceneGraph::Cleanup( )
{
	delete m_pInstance;
	m_pInstance = nullptr;
}

const std::vector<IPrimitive*>& SceneGraph::GetPrimitives( ) const
{
	return m_pPrimitives;
}

const std::vector<ILight*>& SceneGraph::GetLights( ) const
{
	return m_pLights;
}

void SceneGraph::AddPrimitive( IPrimitive* pPrimitive )
{
	m_pPrimitives.push_back( pPrimitive );
}

void SceneGraph::AddLight( ILight* pLight )
{
	m_pLights.push_back( pLight );
}

void SceneGraph::ClearScene( )
{
	for( IPrimitive* pPrimitive : m_pPrimitives )
	{
		delete pPrimitive;
	}
	for( ILight* pLight : m_pLights )
	{
		delete pLight;
	}
	m_pPrimitives.clear( );
	m_pLights.clear( );
}

SceneGraph::~SceneGraph( )
{
	ClearScene( );
}
