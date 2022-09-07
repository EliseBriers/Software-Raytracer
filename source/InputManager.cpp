#include "InputManager.h"
#include <SDL.h>
#include "EMath.h"
#include "Camera.h"
#include <iostream>
#include "SceneBuilder.h"
#include "Lights.h"
#include "SceneGraph.h"

InputManager::InputManager( Camera& camera, bool& takeScreenshot, Elite::Renderer& renderer )
	: m_PreviousMousePos{}
	, m_Camera{ camera }
	, m_TakeScreenshot{ takeScreenshot }
	, m_Renderer{ renderer }
	, m_MouseSpeed{ 3.f }
	, m_KeyboardSpeed{ 8.f }
	, m_RotationSpeed{ 0.3f }
{
}

void InputManager::Update( float dt )
{
	HandleKeyboard( dt );
	HandleMouse( dt );
}

void InputManager::SetCamera( Camera& camera )
{
	m_Camera = camera;
}

void InputManager::HandleKeyPress( SDL_Scancode key )
{
	switch( key )
	{
	case SDL_SCANCODE_X:
		m_TakeScreenshot.get( ) = true;
		break;
	case SDL_SCANCODE_J:
		ToggleDirectionalLights( );
		break;
	case SDL_SCANCODE_K:
		ToggleLight( 1 );
		break;
	case SDL_SCANCODE_L:
		ToggleLight( 0 );
		break;
	case SDL_SCANCODE_Z:
		m_Renderer.get( ).ToggleShadows( );
		break;
	case SDL_SCANCODE_T:
		m_Renderer.get( ).CycleRenderMode( );
		break;
	case SDL_SCANCODE_1:
	case SDL_SCANCODE_KP_1:
		SceneBuilder::LoadScene1( );
		break;
	case SDL_SCANCODE_2:
	case SDL_SCANCODE_KP_2:
		SceneBuilder::LoadScene2( );
		break;
	}
}

void InputManager::HandleKeyboard( float dt ) const
{
	const Uint8* pStates = SDL_GetKeyboardState( nullptr );
	if( pStates[SDL_SCANCODE_W] )
	{
		Elite::FVector3 forward{ m_Camera.get( ).GetForwardNoPitch( ) };
		m_Camera.get( ).Move( -forward * dt * m_KeyboardSpeed );
	}
	if( pStates[SDL_SCANCODE_S] )
	{
		Elite::FVector3 forward{ m_Camera.get( ).GetForwardNoPitch( ) };
		m_Camera.get( ).Move( forward * dt * m_KeyboardSpeed );
	}
	if( pStates[SDL_SCANCODE_Q] )
	{
		Elite::FVector3 up{ 0.f, 1.f, 0.f };
		m_Camera.get( ).Move( up * dt * m_KeyboardSpeed );
	}
	if( pStates[SDL_SCANCODE_E] )
	{
		Elite::FVector3 up{ 0.f, 1.f, 0.f };
		m_Camera.get( ).Move( -up * dt * m_KeyboardSpeed );
	}
	if( pStates[SDL_SCANCODE_D] )
	{
		Elite::FVector3 up{ m_Camera.get( ).GetRight( ) };
		m_Camera.get( ).Move( up * dt * m_KeyboardSpeed );
	}
	if( pStates[SDL_SCANCODE_A] )
	{
		Elite::FVector3 up{ m_Camera.get( ).GetRight( ) };
		m_Camera.get( ).Move( -up * dt * m_KeyboardSpeed );
	}
}

void InputManager::HandleMouse( float dt )
{
	int x{}, y{};
	Uint32 mouseState{ SDL_GetGlobalMouseState( &x, &y ) };
	Elite::FVector2 mousePos{ float( x ), float( y ) };
	Elite::FVector2 mouseMovement{ mousePos - m_PreviousMousePos };

	// Up, down and sideways movement
	if( mouseState & SDL_BUTTON_LMASK && mouseState & SDL_BUTTON_RMASK )
	{
		Elite::FVector3 down{ 0.f, -1.f, 0.f };
		m_Camera.get( ).Move( down * m_MouseSpeed * dt * mouseMovement.y );
		m_Camera.get( ).Move( m_Camera.get( ).GetRight( ) * m_MouseSpeed * dt * mouseMovement.x );
	} // Forward and backwards movement
	else if( mouseState & SDL_BUTTON_LMASK )
	{
		// Move
		Elite::FVector3 forwardNoPitch{ m_Camera.get( ).GetForwardNoPitch( ) };
		m_Camera.get( ).Move( forwardNoPitch * m_MouseSpeed * dt * mouseMovement.y );
		// Rotate
		m_Camera.get( ).RotateY( -m_RotationSpeed * mouseMovement.x * dt );
	} // Look around
	else if( mouseState & SDL_BUTTON_RMASK )
	{
		m_Camera.get( ).RotateY( -m_RotationSpeed * mouseMovement.x * dt );
		m_Camera.get( ).RotateX( -m_RotationSpeed * mouseMovement.y * dt );
	}

	m_PreviousMousePos = mousePos;
}

void InputManager::ToggleDirectionalLights( )
{
	const auto& pLights{ SceneGraph::GetInstance( )->GetLights( ) };
	for( ILight* pLight : pLights )
	{
		if( DirectionalLight* pDirectionalLight{ dynamic_cast<DirectionalLight*>(pLight) } )
		{
			pDirectionalLight->ToggleEnabled( );
		}
	}
}

void InputManager::ToggleLight( size_t index )
{
	const auto& pLights{ SceneGraph::GetInstance( )->GetLights( ) };
	if( pLights.size( ) <= index )
		return;

	pLights[index]->ToggleEnabled( );
}
