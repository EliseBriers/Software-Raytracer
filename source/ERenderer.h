/*=============================================================================*/
// Copyright 2017-2019 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// ERenderer.h: class that holds the surface to render to, does traverse the pixels 
// and traces the rays using a tracer
/*=============================================================================*/
#ifndef ELITE_RAYTRACING_RENDERER
#define	ELITE_RAYTRACING_RENDERER

#include <cstdint>
#include "ThreadPool.h"
#include "ERGBColor.h"
#include "IPrimitive.h"

struct HitRecord;
struct Ray;
class Camera;
struct SDL_Window;
struct SDL_Surface;

namespace Elite
{
	class Renderer final
	{
	public:
		enum class RenderMode
		{
			IrradianceOnly,
			// ReSharper disable once CppInconsistentNaming
			BRDFOnly,
			Combined
		};
		Renderer( SDL_Window* pWindow );
		~Renderer( ) = default;

		Renderer( const Renderer& ) = delete;
		Renderer( Renderer&& ) noexcept = delete;
		Renderer& operator=( const Renderer& ) = delete;
		Renderer& operator=( Renderer&& ) noexcept = delete;

		void Render( const Camera& camera );
		bool SaveBackbufferToImage( ) const;
		bool ShadowHitCheck( const Ray& ray ) const;
		void FillColor( uint32_t r, uint32_t c, const RGBColor& color ) const;
		RGBColor CalculateLighting( const HitRecord& hr, const Ray& ray ) const;
		static bool CastRay( HitRecord& hr, Ray& ray );
		void ToggleShadows( );
		void CycleRenderMode( );
	private:
		ThreadPool m_ThreadPool;
		RGBColor m_BackgroundColor;
		SDL_Window* m_pWindow;
		SDL_Surface* m_pFrontBuffer;
		SDL_Surface* m_pBackBuffer{};
		uint32_t* m_pBackBufferPixels{};
		uint32_t m_Width{};
		uint32_t m_Height{};
		float m_InverseWidth { };
		float m_InverseHeight { };
		float m_AspectRatio { };
		RenderMode m_RenderMode { RenderMode::Combined };
		bool m_ShadowsEnabled { true };
	};
}

#endif
