#pragma once
#include "ThreadPool.h"
#include "EMath.h"

class PointLight;

class Anim_SinMovement
{
public:
	Anim_SinMovement( const Elite::FVector3& offset, float speed, PointLight* pLight );
	void operator()( float dt );

private:
	Elite::FVector3 m_Offset;
	Elite::FPoint3 m_StartPos;
	PointLight* m_pLight;
	float m_Speed;
	float m_SinInput;
};

// Very hacky animation system, 0/10 wouldn't use in production code
class AnimationSystem
{
public:
	void Update( float dt );
	void AddAnimation( const std::function<void( float )>& animation );
	void ClearAnimations( );

	static AnimationSystem* GetInstance( );
	static void Cleanup( );
private:
	static AnimationSystem* m_pInstance;

	AnimationSystem( );

	ThreadPool m_ThreadPool;
	std::vector<std::function<void( float )>> m_Animations;
};
