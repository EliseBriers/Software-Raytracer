//External includes
// ReSharper disable once CppUnusedIncludeDirective
#include "SDL.h"
#undef main

//Standard includes
#include <iostream>

//Project includes
#include "ETimer.h"
#include "ERenderer.h"
#include "SceneGraph.h"
#include "Camera.h"
#include "InputManager.h"
#include "SceneBuilder.h"
#include "AnimationSystem.h"

void ShutDown( SDL_Window* pWindow )
{
	SDL_DestroyWindow( pWindow );
	SDL_Quit( );
}

void DisplayControls( )
{
	std::cout << "Controls:\n";
	std::cout << "  Move/Look: Default unreal bindings\n";
	std::cout << "  Toggle Directional light: J\n";
	std::cout << "  Toggle Left light: K\n";
	std::cout << "  Toggle Right light: L\n";
	std::cout << "  Toggle Shadows: Z\n";
	std::cout << "  Toggle Irradiance, BRDF, Combined: T\n";
	std::cout << "  Switch Scene: 1 and 2 keys (1 = spheres and triangles, 2= bunny)\n";
	std::cout <<
		"Note: camera does not reset on scene change, this is so you can reposition in scene 1 with a good framerate before switching back to scene 2\n";
}

int main( int argc, char* args[] )
{
	DisplayControls( );
	
	//Unreferenced parameters
	(void)argc;
	(void)args;

	// Fill SceneGraph
	SceneBuilder::LoadScene1( );

	Camera camera{ { 0.f, 1.f, 5.f } };

	bool takeScreenshot = false;
	//Create window + surfaces
	SDL_Init( SDL_INIT_VIDEO );

	const uint32_t width = 640;
	const uint32_t height = 480;
	SDL_Window* pWindow = SDL_CreateWindow(
		"RayTracer - Elise Briers",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height, 0 );

	if( !pWindow )
		return 1;

	//Initialize "framework"
	Elite::Timer* pTimer = new Elite::Timer( );
	Elite::Renderer* pRenderer = new Elite::Renderer( pWindow );

	InputManager inputManager{ camera, takeScreenshot, *pRenderer };
	//Start loop
	pTimer->Start( );
	float printTimer = 0.f;
	bool isLooping = true;

	while( isLooping )
	{
		//--------- Get input events ---------
		SDL_Event e;
		while( SDL_PollEvent( &e ) )
		{
			// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
			switch( e.type )
			{
			case SDL_QUIT:
				isLooping = false;
				break;
			case SDL_KEYUP:
				inputManager.HandleKeyPress( e.key.keysym.scancode );
				break;
			}
		}

		//--------- Render ---------
		pRenderer->Render( camera );

		//--------- Timer ---------
		pTimer->Update( );
		printTimer += pTimer->GetElapsed( );
		if( printTimer >= 1.f )
		{
			printTimer = 0.f;
			std::cout << "FPS: " << pTimer->GetFPS( ) << std::endl;
		}

		inputManager.Update( pTimer->GetElapsed( ) );
		AnimationSystem::GetInstance( )->Update( pTimer->GetElapsed( ) );
		//Save screenshot after full render
		if( takeScreenshot )
		{
			if( !pRenderer->SaveBackbufferToImage( ) )
				std::cout << "Screenshot saved!" << std::endl;
			else
				std::cout << "Something went wrong. Screenshot not saved!" << std::endl;
			takeScreenshot = false;
		}
	}
	pTimer->Stop( );

	//Shutdown "framework"
	delete pRenderer;
	delete pTimer;
	SceneGraph::Cleanup( );
	AnimationSystem::Cleanup( );

	ShutDown( pWindow );
	return 0;
}
