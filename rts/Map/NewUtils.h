/* This file is part of the Recoil engine (GPL v2 or later), see LICENSE.html */

#pragma once

#include "Rendering/Textures/Bitmap.h"
#include "Map/ReadMap.h"

//!temp name
void SetGameMapRequisites_(const char* heightMapFilePath);

void SetHeightMapByBitmap(const CBitmap& bitmap, int fromX, int fromZ, int toX, int toZ);
inline void SetHeightMapByBitmap(const CBitmap& bitmap) { SetHeightMapByBitmap(bitmap, 0,0, mapDims.mapx,mapDims.mapy); };

void SetHeightMapByFile(const char* filePath, int fromX, int fromZ, int toX, int toZ);
inline void SetHeightMapByFile(const char* filePath) { SetHeightMapByFile(filePath, 0,0, mapDims.mapx,mapDims.mapy); };

void GetHeightDataFromCurFile(float* destHeightData);