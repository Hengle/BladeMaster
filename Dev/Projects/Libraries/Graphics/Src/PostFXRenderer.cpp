#include "GraphicsPrivate.h"
#include "ShaderMgr.h"
#include "PostFXRenderer.h"
#include "QuadGeometry.h"
#include "DCRenderer.h"


SINGLETON_DEFINE(BMPostFXRenderer);

BMPostFXRenderer::BMPostFXRenderer()
{

}

BMPostFXRenderer::~BMPostFXRenderer()
{
    ReleaseResource();
    SAFE_DELETE(mQuadGeometry);
}

namespace
{
	const WCHAR* postfx_shader_path = L"PostFX.hlsl";
    const WCHAR* copy_shader_path = L"Copy.hlsl";
}

void BMPostFXRenderer::Init()
{
    //resource init
    mQuadGeometry = new QuadGeometry;    
}

void BMPostFXRenderer::InitResource()
{
    mQuadGeometry->Init();

    //init bloom
    InitBloom();    
}

//only release render driver resource, no app resource
void BMPostFXRenderer::ReleaseResource()
{
    for(u32 i=0; i<msBloomBufferCount; i++)
    {
        mBloomRenderTarget[i] = NULL;
        mBloomSurface[i] = NULL;
    }
}

void BMPostFXRenderer::InitBloom()
{
    /*
     *	shader id init
     */
    ShaderId postFxShaderType = ShaderMgr::GetInstance()->GetShaderTypeId("PostFX");
    mBloomFilter = postFxShaderType | ShaderMgr::GetInstance()->GetShaderMacroMask(postFxShaderType, "BLOOM_FILTER");
    mBloomBlurV = postFxShaderType | ShaderMgr::GetInstance()->GetShaderMacroMask(postFxShaderType, "BLOOM_BLURV");
    mBloomBlurH = postFxShaderType | ShaderMgr::GetInstance()->GetShaderMacroMask(postFxShaderType, "BLOOM_BLURH");
    mBloomMerge = postFxShaderType | ShaderMgr::GetInstance()->GetShaderMacroMask(postFxShaderType, "BLOOM_MERGE");
    mCopy = postFxShaderType | ShaderMgr::GetInstance()->GetShaderMacroMask(postFxShaderType, "COPY");

	//render targets
	u32 sizeX, sizeY;
	DCRenderer::GetInstance()->GetBackBufferSize(sizeX,sizeY);
	HRESULT result = DEVICEPTR->CreateTexture(
		sizeX,
		sizeY, 
		1, 
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8, 
		D3DPOOL_DEFAULT, 
		&mBloomRenderTarget[0], 
		NULL
		);

	assert(SUCCEEDED(result));

    result = mBloomRenderTarget[0]->GetSurfaceLevel(0, &mBloomSurface[0]);
    assert(SUCCEEDED(result));

	const u32 halfSizeX = sizeX/2;
	const u32 halfSizeY = sizeY/2;
	result = DEVICEPTR->CreateTexture(
		halfSizeX,
		halfSizeY, 
		1, 
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8, 
		D3DPOOL_DEFAULT, 
		&mBloomRenderTarget[1], 
		NULL
		);
	assert(SUCCEEDED(result));
    
    result = mBloomRenderTarget[1]->GetSurfaceLevel(0, &mBloomSurface[1]);
    assert(SUCCEEDED(result));

	result = DEVICEPTR->CreateTexture(
		halfSizeX,
		halfSizeY, 
		1, 
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8, 
		D3DPOOL_DEFAULT, 
		&mBloomRenderTarget[2], 
		NULL
		);
	assert(SUCCEEDED(result));

    result = mBloomRenderTarget[2]->GetSurfaceLevel(0, &mBloomSurface[2]);
    assert(SUCCEEDED(result));
}

/*
*   bloom only
*/
void BMPostFXRenderer::Render()
{
    //pix_event g( DXUT_PERFEVENTCOLOR, L"PostFX" );
    //filter pass
    DEVICEPTR->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    DEVICEPTR->SetRenderTarget(0, mBloomSurface[0]);    
    ShaderMgr::GetInstance()->ApplyShader(mBloomFilter);

    TexturePtr lightingRT = DCRenderer::GetInstance()->GetLightingRT();
    SurfacePtr lightingSurface;
    lightingRT->GetSurfaceLevel(0,&lightingSurface);
    DEVICEPTR->SetTexture(0, lightingRT);
    
    mQuadGeometry->Render();

    //blur v
    DEVICEPTR->SetRenderTarget(0, mBloomSurface[1]);    
    ShaderMgr::GetInstance()->ApplyShader(mBloomBlurV);
    DEVICEPTR->SetTexture(0, mBloomRenderTarget[0]);
    mQuadGeometry->Render();

    //blur h
    DEVICEPTR->SetRenderTarget(0, mBloomSurface[2]);    
    ShaderMgr::GetInstance()->ApplyShader(mBloomBlurH);
    DEVICEPTR->SetTexture(0, mBloomRenderTarget[1]);
    mQuadGeometry->Render();

    //debug code
    DEVICEPTR->SetRenderTarget(0, mBackBuffer);
    DEVICEPTR->SetTexture(0, lightingRT);
    DEVICEPTR->SetTexture(1, mBloomRenderTarget[2]);    
    ShaderMgr::GetInstance()->ApplyShader(mBloomMerge);
    mQuadGeometry->Render();
}

void BMPostFXRenderer::StoreBackBuffer()
{
    HRESULT result = DEVICEPTR->GetRenderTarget(0, &mBackBuffer);
    assert(SUCCEEDED(result));
}

void BMPostFXRenderer::RenderToBackBuffer(TexturePtr tex)
{    
/*    
    DEVICEPTR->SetTexture(0, tex);    
    DEVICEPTR->SetRenderTarget(0,mBackBuffer);

    DEVICEPTR->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    mQuadGeometry->Render(); */   
}
