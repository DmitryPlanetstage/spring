#include "NewUtils.h"
#include "Map/ReadMap.h"
#include "Map/MapDamage.h"
#include "System/Log/ILog.h"
#include <string>
//!temp
#include <fstream>

namespace {
	std::string HeightMapFilePath;

	bool LoadHeightBitmap(CBitmap& bitmap, const char* filePath) {
		if (!bitmap.LoadGrayscale(std::string(filePath), true)) {
			LOG_L(L_ERROR, "[%s()]: Couldn't load \"%s\" bitmap!", __func__, filePath);
			return false;
		}
		//!clean - mapxp1/mapyp1 may be apparently not available
		//if (bitmap.xsize != mapDims.mapxp1 || bitmap.ysize != mapDims.mapyp1) {
		if (bitmap.xsize != mapDims.mapx+1 || bitmap.ysize != mapDims.mapy+1) {
			LOG_L(L_ERROR, "[%s()]: Incorrect \"%s\" bitmap dimensions!", __func__, filePath);
			return false;
		}
		return true;
	}
}

void SetGameMapRequisites_(const char* heightMapFilePath) {
	HeightMapFilePath = heightMapFilePath;
}

// A bitmap must be 16-bit grayscale
void SetHeightMapByBitmap(const CBitmap& bitmap, int fromX, int fromZ, int toX, int toZ) {
	/*if (mapDamage->Disabled()) return;

	const uint16_t* data = reinterpret_cast<const uint16_t*>(bitmap.GetRawMem());
	bool heightMapChanged = false;
	for (int z = fromZ; z <= toZ; ++z) {
		for (int x = fromX; x <= toX; ++x) {
			const int index = z*mapDims.mapxp1 +x;
			const float height = heightBase +float(data[index])/65535.0f*heightScale;
			const float oldHeight = readMap->GetCornerHeightMapSynced()[index];
			readMap->SetHeight(index, height);
			heightMapChanged = heightMapChanged || (height != oldHeight);
		}
	}

	if (heightMapChanged) {
		mapDamage->RecalcArea(fromX, toX, fromZ, toZ);
	}*/
}

void SetHeightMapByFile(const char* filePath, int fromX, int fromZ, int toX, int toZ) {
	if (mapDamage->Disabled()) return;

	//!temp
	char* fileData;
	{
		std::ifstream file(HeightMapFilePath, std::ios::in | std::ios::binary | std::ios::ate);
		if (!file.is_open()) return;

		size_t size = file.tellg();
		file.seekg(0, std::ios::beg);

		fileData = new char[size];
		file.read(fileData, size);
	}

	float* heights = reinterpret_cast<float*>(fileData);
	bool heightMapChanged = false;
	for (int z = fromZ; z <= toZ; ++z) {
		for (int x = fromX; x <= toX; ++x) {
			const int index = z*mapDims.mapxp1 +x;
			const float height = heights[index];
			const float oldHeight = readMap->GetCornerHeightMapSynced()[index];
			readMap->SetHeight(index, height);
			heightMapChanged = heightMapChanged || (height != oldHeight);
		}
	}

	delete[] fileData;

	if (heightMapChanged) {
		mapDamage->RecalcArea(fromX, toX, fromZ, toZ);
	}

	//!clean
	HeightMapFilePath = filePath;
}

void GetHeightDataFromCurFile(float* destHeightData) {
	//!temp
	std::ifstream file(HeightMapFilePath, std::ios::in | std::ios::binary | std::ios::ate);
	if (!file.is_open()) return;

	size_t size = file.tellg();
	file.seekg(0, std::ios::beg);

	if (size <= (mapDims.mapx+1)*(mapDims.mapy+1)*sizeof(float)) {
		file.read(reinterpret_cast<char*>(destHeightData), size);
	}
}