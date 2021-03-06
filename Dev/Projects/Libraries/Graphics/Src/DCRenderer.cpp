#include "GraphicsPrivate.h"
#include "ShaderMgr.h"
#include "DCRenderer.h"
#include "Terrain.h"
#include "DCModel.h"
#include "VertexDeclarationManager.h"
#include "PostFXRenderer.h"
#include "DCRenderSetter.h"

SINGLETON_DEFINE(DCRenderer);

namespace RendererConfig
{
    const u32 BackBufferSizeX = 800;
    const u32 BackBufferSizeY = 600;
}

namespace
{
	struct BatchNode
	{
		DCModelPtr model;
		XMFLOAT3   translation;
	};

	std::vector<BatchNode> BatchNodes;	

	BMTerrainPtr		   TerrainPtr;
}

DCRenderer::DCRenderer()
:   mBackBufferSizeX(RendererConfig::BackBufferSizeX)
,   mBackBufferSizeY(RendererConfig::BackBufferSizeY)
{
    msInstance = this;
}

DCRenderer::~DCRenderer()
{
	//SAFE_RELEASE(DEVICEPTR);
}

bool DCRenderer::ApplyTexture(u32 stage, const DCTexture* tex)
{
    DEVICEPTR->SetSamplerState(stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    DEVICEPTR->SetSamplerState(stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    DEVICEPTR->SetSamplerState(stage, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	return SUCCEEDED(DEVICEPTR->SetTexture(stage,(IDirect3DBaseTexture9*)(tex->GetSurfacePtr()->GetTexture().GetPointer())) );
}

void DCRenderer::Init(QWidget* pRenderWidget)
{
    DCRenderSetter::Init();

    BMPostFXRenderer::CreateInstance();
    BMPostFXRenderer::GetInstance()->Init();

    ShaderMgr::CreateInstance();  
    VertexDeclareManager::CreateInstance();

    D3D9Renderer::Init(pRenderWidget);         
}

void DCRenderer::Exit()
{
}

void DCRenderer::Update(f32 fDeltaTime)
{

}

void DCRenderer::Draw(f32 fDeltaTime)
{	
    // For our world matrix, we will just rotate the object about the y-axis.
    D3DXMATRIXA16 mxView, mxProj;

    D3DXVECTOR3 vEye(0.0f, 5.0f,-5.0f);
    D3DXVECTOR3 vAt(0.0f,0.0f,1.0f);
    D3DXVECTOR3 vUp(0.0f,1.0f,0.0f);

    float fAspectRatio = m_pRenderWidget->width() / (FLOAT)m_pRenderWidget->height();

    D3DXMatrixLookAtLH(&mxView, &vEye, &vAt, &vUp);
    D3DXMatrixPerspectiveFovLH(&mxProj, D3DX_PI/3, fAspectRatio, 0.001f, 100.0f);

 	//models
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 mxWorld, mxViewProj;

	D3DXMatrixIdentity(&mxViewProj);

	BM_AssertHr(DEVICEPTR->BeginScene());	
	{        
		BeginRender();
		DEVICEPTR->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_COLORVALUE(85.0f/255.0f,101.0f/255.0f,215.0f/255.0f,1.0f), 1.0f, 0 );
		BM_AssertHr(DEVICEPTR->EndScene());    

		//models		
		const std::vector<BatchNode>& nodes = BatchNodes;
		const u32 nodeNum = nodes.size();
		for(u32 nodeIdx = 0; nodeIdx<nodeNum; nodeIdx++)
		{
			BM_AssertHr(DEVICEPTR->BeginScene());
			const BatchNode& nodePtrRef = nodes[nodeIdx];
			const XMFLOAT3& translation = nodePtrRef.translation;
			D3DXMatrixTranslation(&mxWorld, translation.x, translation.y, translation.z);
			mxViewProj = mxView * mxProj;

			DEVICEPTR->SetVertexShaderConstantF(0,(float*)&(mxWorld),4);
			DEVICEPTR->SetVertexShaderConstantF(4,(float*)mViewProjMatrix,4);
			DEVICEPTR->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			DEVICEPTR->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

			nodePtrRef.model->Draw(0);
			BM_AssertHr(DEVICEPTR->EndScene()); 
		}

		BM_AssertHr(DEVICEPTR->BeginScene());

		//terrain		
		if(TerrainPtr)
		{
			TerrainPtr->Draw(&mxWorld, (D3DXMATRIXA16*)mViewMatrix, (D3DXMATRIXA16*)mProjMatrix);
		}

		BMPostFXRenderer::GetInstance()->Render();

		EndRender();
		BM_AssertHr(DEVICEPTR->EndScene()); 
	}

	BatchNodes.clear();
}

void DCRenderer::OnResetDevice(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc)
{
    InitResource(pd3dDevice);    
}

void DCRenderer::OnLostDevice()
{
    ReleaseResource();
}

void DCRenderer::OnDestroyDevice()
{
    ReleaseResource();
}

u32 aa,bb;
void DCRenderer::BeginRender()
{
    BMPostFXRenderer::GetInstance()->StoreBackBuffer();

    //set lighting rt
	IDirect3DSurface9* surface;
	HRESULT result = mLightingRT->GetSurfaceLevel(0, &surface);
	assert(SUCCEEDED(result));

	result = DEVICEPTR->SetRenderTarget(0, surface);
	assert(SUCCEEDED(result));

    DEVICEPTR->GetDepthStencilSurface(&surface);
    D3DSURFACE_DESC pDesc;
    surface->GetDesc(&pDesc);
    //std::string str("size");
    //str += (char)pDesc.Width;
    //str += (char)pDesc.Height;
    //LogToDebug(str.c_str());
    aa = pDesc.Width;
    bb = pDesc.Height;
}


//render lighting buffer into back to backbuffer
void DCRenderer::EndRender()
{
    BMPostFXRenderer::GetInstance()->RenderToBackBuffer(mLightingRT);
}

//can be called on onresetdevice
void DCRenderer::InitResource(IDirect3DDevice9* deivce)
{
    //DEVICEPTR = deivce;   
    //render target
    HRESULT result = DEVICEPTR->CreateTexture(
        mBackBufferSizeX,
        mBackBufferSizeY, 
        1, 
        D3DUSAGE_RENDERTARGET,
        D3DFMT_A8R8G8B8, 
        D3DPOOL_DEFAULT, 
        &mLightingRT, 
        NULL
        );

    assert(SUCCEEDED(result));
    
    ShaderMgr::GetInstance()->LoadShaders();

    BMPostFXRenderer::GetInstance()->InitResource();
    VertexDeclareManager::GetInstance()->CreateResource();
}

//release graphics driver related resource including shader/texture
void DCRenderer::ReleaseResource()
{
    mLightingRT = NULL;

    BMPostFXRenderer::GetInstance()->ReleaseResource();
    VertexDeclareManager::GetInstance()->ReleaseResource();
}


void DCRenderer::AddSceneNode(DCModel* modelPtr, const XMFLOAT3& translation)
{
	BatchNode node;
	node.model = modelPtr;
	node.translation = translation;
	BatchNodes.push_back(node);
}

void DCRenderer::AddSceneTerrain(BMTerrain* terrain)
{
	TerrainPtr = terrain;
}

void DCRenderer::SetViewProjMatrix(XMMATRIX* view, XMMATRIX* proj, XMMATRIX* viewProj)
{
	BM_Assert(view && proj && viewProj);
	mViewMatrix = view;
	mProjMatrix = proj;
	mViewProjMatrix = viewProj;
}