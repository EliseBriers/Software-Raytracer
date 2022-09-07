#include "AnimationSystem.h"
#include "Lights.h"

AnimationSystem* AnimationSystem::m_pInstance{ nullptr };

Anim_SinMovement::Anim_SinMovement( const Elite::FVector3& offset, float speed, PointLight* pLight )
	: m_Offset{ offset }
	, m_StartPos{ pLight->GetPosition( ) }
	, m_pLight{ pLight }
	, m_Speed{ speed }
	, m_SinInput{}
{
}

void Anim_SinMovement::operator()( float dt )
{
	m_SinInput += dt * m_Speed;
	Elite::FVector3 offset{ m_Offset * ( sinf( m_SinInput ) + 1.f ) / 2.f };
	m_pLight->SetPosition( m_StartPos + offset );
}

void AnimationSystem::Update( float dt )
{
	for( std::function<void( float )>& function : m_Animations )
	{
		m_ThreadPool.AddTask( [&function, dt]( )
		{
			function( dt );
		} );
	}
	m_ThreadPool.WaitForCompletion( );
}

void AnimationSystem::AddAnimation( const std::function<void( float )>& animation )
{
	m_Animations.push_back( animation );
}

void AnimationSystem::ClearAnimations( )
{
	m_Animations.clear( );
}

AnimationSystem* AnimationSystem::GetInstance( )
{
	if( !m_pInstance )
		m_pInstance = new AnimationSystem( );
	return m_pInstance;
}

void AnimationSystem::Cleanup( )
{
	delete m_pInstance;
	m_pInstance = nullptr;
}

AnimationSystem::AnimationSystem( )
	: m_ThreadPool{ std::thread::hardware_concurrency( ) }
{
}
