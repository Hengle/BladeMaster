#pragma once

//////////////////////////////////////////////////////////////////////////
// DLL Import / Export
#ifdef GRAPHICS_LIB
#   define GRAPHICS_API __declspec(dllexport)
#else
#   define GRAPHICS_API __declspec(dllimport)
#endif
//////////////////////////////////////////////////////////////////////////

// BM
#include "Core.h"
