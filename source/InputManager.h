#pragma once
#include "EMath.h"
#include <SDL.h>
#include "ERenderer.h"
class Camera;

class InputManager
{
public:
	InputManager( Camera& camera, bool& takeScreenshot, Elite::Renderer& renderer );
	void Update( float dt );

	void SetCamera( Camera& camera );
	void HandleKeyPress( SDL_Scancode key );
private:
	Elite::FVector2 m_PreviousMousePos;
	std::reference_wrapper<Camera> m_Camera;
	std::reference_wrapper<bool> m_TakeScreenshot;
	std::reference_wrapper<Elite::Renderer> m_Renderer;
	float m_MouseSpeed;
	float m_KeyboardSpeed;
	float m_RotationSpeed;

	void HandleKeyboard( float dt ) const;
	void HandleMouse( float dt );
	static void ToggleDirectionalLights( );
	static void ToggleLight( size_t index );
};

