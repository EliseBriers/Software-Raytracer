#include "SceneBuilder.h"
#include "Lights.h"
#include "IPrimitive.h"
#include "Materials.h"
#include "Sphere.h"
#include "SceneGraph.h"
#include "Plane.h"
#include "AnimationSystem.h"
#include "TriangleMesh.h"
#include "OBJParser.h"

void SceneBuilder::LoadScene1( )
{
	AnimationSystem* pAnimationSystem{ AnimationSystem::GetInstance( ) };
	pAnimationSystem->ClearAnimations( );
	Elite::RGBColor silver{ 0.972f, 0.96f, 0.915f };
	SceneGraph* pSceneGraph{ SceneGraph::GetInstance( ) };
	pSceneGraph->ClearScene( );
	pSceneGraph->AddPrimitive( new Plane{ { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }, new Material_Lambert{ 1.f, { 0.4f, 0.4f, 0.4f } } } );
	pSceneGraph->AddPrimitive( new Plane{ { 8.f, 0.f, 0.f }, { -1.f, 0.f, 0.f }, new Material_Lambert{ 1.f, { 0.4f, 0.4f, 0.4f } } } );
	pSceneGraph->AddPrimitive( new Plane{ { -8.f, 0.f, 0.f }, { 1.f, 0.f, 0.f }, new Material_Lambert{ 1.f, { 0.4f, 0.4f, 0.4f } } } );
	pSceneGraph->AddPrimitive( new Plane{ { 0.f, 0.f, -8.f }, { 0.f, 0.f, 1.f }, new Material_Lambert{ 1.f, { 0.4f, 0.4f, 0.4f } } } );
	// SceneGraph::GetInstance( )->AddPrimitive( new Sphere{ { -0.75f, 1.f, 0.f }, new Material_Lambert{ 1.f, { 0.85f, 0.4f, 0.35f } }, 1.f } );
	// SceneGraph::GetInstance( )->AddPrimitive( new Sphere{ { 0.75f, 1.f, 0.f }, new Material_LambertPhong{ 0.4f, { 0.5f, 0.78f, 0.6f }, 60.f }, 1.f } );

	pSceneGraph->AddPrimitive( new Sphere{ { -2.5f, 1.f, 0.f }, new Material_PBR{ silver, false, 1.f }, 1.f } );
	pSceneGraph->AddPrimitive( new Sphere{ { 0.f, 1.f, 0.f }, new Material_PBR{ silver, false, 0.6f }, 1.f } );
	pSceneGraph->AddPrimitive( new Sphere{ { 2.5f, 1.f, 0.f }, new Material_PBR{ silver, false, 0.1f }, 1.f } );

	pSceneGraph->AddPrimitive( new Sphere{ { -2.5f, 3.5f, 0.f }, new Material_PBR{ silver, true, 1.f }, 1.f } );
	pSceneGraph->AddPrimitive( new Sphere{ { 0.f, 3.5f, 0.f }, new Material_PBR{ silver, true, 0.6f }, 1.f } );
	pSceneGraph->AddPrimitive( new Sphere{ { 2.5f, 3.5f, 0.f }, new Material_PBR{ silver, true, 0.1f }, 1.f } );
	Triangle* pTriangle1{
		new Triangle{
			{ -2.5f, 5.f, 0.f }, { 0.5f, 0.f, 0.f }, { 0.f, 0.5f, 0.f }, { -0.5f, 0.f, 0.f }, new Material_PBR{ silver, true, 0.6f }
		}
	};
	pSceneGraph->AddPrimitive( pTriangle1 );

	Triangle* pTriangle2{
		new Triangle{
			{ 0.f, 5.f, 0.f }, { 0.5f, 0.f, 0.f }, { 0.f, 0.5f, 0.f }, { -0.5f, 0.f, 0.f }, new Material_PBR{ silver, true, 0.6f }, CullMode::frontFace
		}
	};
	pSceneGraph->AddPrimitive( pTriangle2 );
	Triangle* pTriangle3{
		new Triangle{
			{ 2.5f, 5.f, 0.f }, { 0.5f, 0.f, 0.f }, { 0.f, 0.5f, 0.f }, { -0.5f, 0.f, 0.f }, new Material_PBR{ silver, true, 0.6f }, CullMode::none
		}
	};
	pSceneGraph->AddPrimitive( pTriangle3 );

	float rotationSpeed{ 2.f };
	pAnimationSystem->AddAnimation( [pTriangle2, rotationSpeed]( float dt )
	{
		pTriangle2->RotateY( rotationSpeed * dt );
	} );
	pAnimationSystem->AddAnimation( [pTriangle1, rotationSpeed]( float dt )
	{
		pTriangle1->RotateY( rotationSpeed * dt );
	} );
	pAnimationSystem->AddAnimation( [pTriangle3, rotationSpeed]( float dt )
	{
		pTriangle3->RotateY( rotationSpeed * dt );
	} );

	PointLight* pLight1{ new PointLight{ 25.f, { 0.f, 2.5f, 5.f }, { 1.f, 1.f, 0.8f } } };
	pSceneGraph->AddLight( pLight1 );
	PointLight* pLight2{ new PointLight{ 25.f, { -2.f, 2.5f, 5.f } } };
	pSceneGraph->AddLight( pLight2 );

	PointLight* pLight3{ new PointLight{ 25.f, { 0.f, 5.f, -5.f } } };
	pSceneGraph->AddLight( pLight3 );

	pSceneGraph->AddLight( new DirectionalLight{ 0.5f, GetNormalized( Elite::FVector3{ 0.5f, -1.f, 0.f } ), } );
	pSceneGraph->AddLight( new DirectionalLight{ 0.5f, GetNormalized( Elite::FVector3{ -0.5f, -1.f, 1.f } ) } );

	pAnimationSystem->AddAnimation( Anim_SinMovement{ { 3.f, 0.f, 0.f }, 1.f, pLight1 } );
	pAnimationSystem->AddAnimation( Anim_SinMovement{ { 0.f, 2.f, 0.f }, 0.7f, pLight3 } );
}

void SceneBuilder::LoadScene2( )
{
	AnimationSystem* pAnimationSystem{ AnimationSystem::GetInstance( ) };
	pAnimationSystem->ClearAnimations( );
	SceneGraph* pSceneGraph{ SceneGraph::GetInstance( ) };
	pSceneGraph->ClearScene( );


	TriangleMesh* pMesh{
		OBJParser::ParseObjFile( "lowpoly_bunny.obj", { 0.f, 0.f, -3.f }, new Material_Lambert{ 1.f, { 0.9f, 0.9f, 0.9f } } )
	};

	if( pMesh )
		pSceneGraph->AddPrimitive( pMesh );

	// Add planes
	pSceneGraph->AddPrimitive( new Plane { { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }, new Material_Lambert{ 1.f, { 0.4f, 0.4f, 0.4f } } } );
	pSceneGraph->AddPrimitive( new Plane { { 8.f, 0.f, 0.f }, { -1.f, 0.f, 0.f }, new Material_Lambert{ 1.f, { 0.4f, 0.4f, 0.4f } } } );
	pSceneGraph->AddPrimitive( new Plane { { -8.f, 0.f, 0.f }, { 1.f, 0.f, 0.f }, new Material_Lambert{ 1.f, { 0.4f, 0.4f, 0.4f } } } );
	pSceneGraph->AddPrimitive( new Plane { { 0.f, 0.f, -8.f }, { 0.f, 0.f, 1.f }, new Material_Lambert{ 1.f, { 0.4f, 0.4f, 0.4f } } } );

	// Add Lights
	PointLight* pLight1 { new PointLight{ 25.f, { 0.f, 2.5f, 2.f }, { 1.f, 1.f, 0.8f } } };
	pSceneGraph->AddLight( pLight1 );
	PointLight* pLight2 { new PointLight{ 25.f, { -2.f, 2.5f, 2.f } } };
	pSceneGraph->AddLight( pLight2 );

	PointLight* pLight3 { new PointLight{ 25.f, { 0.f, 5.f, -5.f } } };
	pSceneGraph->AddLight( pLight3 );

	pSceneGraph->AddLight( new DirectionalLight { 0.5f, GetNormalized( Elite::FVector3{ 0.5f, -1.f, 0.f } ), } );
	pSceneGraph->AddLight( new DirectionalLight { 0.5f, GetNormalized( Elite::FVector3{ -0.5f, -1.f, 1.f } ) } );
}
