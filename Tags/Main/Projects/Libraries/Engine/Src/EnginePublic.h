#pragma once

//////////////////////////////////////////////////////////////////////////
// DLL Import / Export
#ifdef ENGINE_LIB
#   define ENGINE_API __declspec(dllexport)
#else
#   define ENGINE_API __declspec(dllimport)
#endif
//////////////////////////////////////////////////////////////////////////

// BM
#include "Core.h"
#include "Graphics.h"
