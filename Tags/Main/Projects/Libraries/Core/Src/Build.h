// Included in CorePublic.h

//////////////////////////////////////////////////////////////////////////
// Configurations
#if defined(DEBUG) || defined(_DEBUG)
#   define BM_DEBUG         1
#elif defined(NDEBUG)
#   define BM_RELEASE       1
#else
#   error "Invalid Configuration"
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// DirectX
#define BM_USE_D3D9  1
//////////////////////////////////////////////////////////////////////////