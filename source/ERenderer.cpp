//External includes
#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include "ERenderer.h"
#include "ERGBColor.h"
#include "IPrimitive.h"
#include "SceneGraph.h"
#include "Camera.h"
#include "Lights.h"
#include "Materials.h"

Elite::Renderer::Renderer( SDL_Window* pWindow )
	: m_ThreadPool{ std::thread::hardware_concurrency( ) }
	, m_BackgroundColor{ 0.085f, 0.04f, 0.035f }
	, m_pWindow{ pWindow }
	, m_pFrontBuffer{ SDL_GetWindowSurface( pWindow ) }
{
	//Initialize
	int width, height;
	SDL_GetWindowSize( pWindow, &width, &height );
	m_Width = uint32_t( width );
	m_Height = uint32_t( height );
	m_InverseHeight = 1.f / float( height );
	m_InverseWidth = 1.f / float( width );
	m_AspectRatio = float( width ) * m_InverseHeight;
	m_pBackBuffer = SDL_CreateRGBSurface( 0, m_Width, m_Height, 32, 0, 0, 0, 0 );
	m_pBackBufferPixels = static_cast<uint32_t*>(m_pBackBuffer->pixels);
}

void Elite::Renderer::Render( const Camera& camera )
{
	SDL_LockSurface( m_pBackBuffer );
	const float fov{ camera.GetFov( ) };


	for( uint32_t r{}; r < m_Height; ++r )
	{
		const float y{ ( 1 - 2 * ( ( float( r ) + 0.5f ) * m_InverseHeight ) ) * fov };

		m_ThreadPool.AddTask( [=]( )
		{
			for( uint32_t c{}; c < m_Width; ++c )
			{
				const float x{ ( 2 * ( ( float( c ) + 0.5f ) * m_InverseWidth ) - 1 ) * m_AspectRatio * fov };
				const FPoint3 position{ camera.Translate( { x, y, -1 } ) };
				const FVector3 direction{ position - camera.GetPosition( ) };
				Ray ray{ FPoint3{ camera.GetPosition( ) }, GetNormalized( direction ) };

				HitRecord hr;
				bool hasHit{ CastRay( hr, ray ) };

				FillColor( r, c, hasHit ? CalculateLighting( hr, ray ) : m_BackgroundColor );
			}
		} );
	}
	m_ThreadPool.WaitForCompletion( );

	SDL_UnlockSurface( m_pBackBuffer );
	SDL_BlitSurface( m_pBackBuffer, nullptr, m_pFrontBuffer, nullptr );
	SDL_UpdateWindowSurface( m_pWindow );
}

bool Elite::Renderer::SaveBackbufferToImage( ) const
{
	return SDL_SaveBMP( m_pBackBuffer, "BackbufferRender.bmp" );
}

bool Elite::Renderer::ShadowHitCheck( const Ray& ray ) const
{
	if( !m_ShadowsEnabled )
		return false;
	const std::vector<IPrimitive*>& pPrimitives{ SceneGraph::GetInstance( )->GetPrimitives( ) };
	for( IPrimitive const* pPrimitive : pPrimitives )
	{
		if( pPrimitive->Hit( ray, nullptr, true ) )
			return true;
	}
	return false;
}

void Elite::Renderer::FillColor( uint32_t r, uint32_t c, const RGBColor& color ) const
{
	m_pBackBufferPixels[c + ( r * m_Width )] = SDL_MapRGB( m_pBackBuffer->format,
	                                                       uint8_t( color.r * 255.f ),
	                                                       uint8_t( color.g * 255.f ),
	                                                       uint8_t( color.b * 255.f ) );
}

Elite::RGBColor Elite::Renderer::CalculateLighting( const HitRecord& hr, const Ray& ray ) const
{
	const std::vector<ILight*>& pLights{ SceneGraph::GetInstance( )->GetLights( ) };
	RGBColor finalColor{};
	for( ILight* light : pLights )
	{
		if( !light->IsEnabled( ) )
			continue;
		// Check if dot product is correct, without normalizing the l vector, this is faster
		if( !light->VerifyLambertCosine( hr.position, hr.normal ) )
			continue;

		// Check if point is in shadow
		const auto [ lightDirection, lightDistance ]{ light->GetLightDirection( hr.position ) };
		const Ray shadowRay{ hr.position, lightDirection, 0.005f, lightDistance };
		if( ShadowHitCheck( shadowRay ) )
			continue;

		// Calculate resulting color
		const float lambertCosine{ Dot( hr.normal, lightDirection ) };

		switch( m_RenderMode )
		{
		case RenderMode::IrradianceOnly:
			finalColor += light->GetIrradiance( hr.position ) * lambertCosine;
			break;
		case RenderMode::BRDFOnly:
			if( hr.pMaterial )
				finalColor += hr.pMaterial->Shade( hr, lightDirection, -ray.direction ) * lambertCosine;
			else
				finalColor += light->GetIrradiance( hr.position ) * lambertCosine;
			break;
		case RenderMode::Combined:
			if( hr.pMaterial )
				finalColor += light->GetIrradiance( hr.position ) * lambertCosine * hr.pMaterial->Shade( hr, lightDirection, -ray.direction );
			else
				finalColor += light->GetIrradiance( hr.position ) * lambertCosine;
			break;
		default: ;
		}
	}
	finalColor.MaxToOne( );
	return finalColor;
}

bool Elite::Renderer::CastRay( HitRecord& hr, Ray& ray )
{
	const std::vector<IPrimitive*>& pPrimitives{ SceneGraph::GetInstance( )->GetPrimitives( ) };
	bool hasHit{ false };
	hr = { {}, {}, {}, FLT_MAX };
	for( const IPrimitive* primitive : pPrimitives )
	{
		HitRecord temp{};
		if( primitive->Hit( ray, &temp ) )
		{
			// I do the distance check by setting my ray tMax to the hit distance,
			// I do this so I can avoid an unnecessary square root in my sphere.
			hasHit = true;
			ray.tMax = temp.distance;
			hr = temp;
		}
	}
	return hasHit;
}

void Elite::Renderer::ToggleShadows( )
{
	m_ShadowsEnabled = !m_ShadowsEnabled;
}

void Elite::Renderer::CycleRenderMode( )
{
	switch( m_RenderMode )
	{
	case RenderMode::IrradianceOnly:
		m_RenderMode = RenderMode::BRDFOnly;
		break;
	case RenderMode::BRDFOnly:
		m_RenderMode = RenderMode::Combined;
		break;
	case RenderMode::Combined:
		m_RenderMode = RenderMode::IrradianceOnly;
		break;
	default: ;
	}
}
