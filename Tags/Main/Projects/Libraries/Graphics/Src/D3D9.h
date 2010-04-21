#pragma once

// DirectX Includes
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <dxdiag.h>

// DirectX Libraries
#pragma comment( lib, "d3d9.lib" )
#if defined(DEBUG) || defined(_DEBUG)
#   pragma comment( lib, "d3dx9d.lib" )
#else
#   pragma comment( lib, "d3dx9.lib" )
#endif
#pragma comment( lib, "dxerr.lib" )
#pragma comment( lib, "dxguid.lib" )
