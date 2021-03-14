#include "Body.h"

#include <CellClass.h>

/*
CellClass* currentCell = nullptr;

inline void ReplacePalette(BytePalette** ppDest)
{
	if (currentCell) {
		auto pData = IsometricTileTypeExt::ExtMap.Find(currentCell->GetIsoTileType());

		if (pData->Palette)
			*ppDest = pData->Palette;
	}
}

DEFINE_HOOK_AGAIN(484050, CellClass_DoCellLighting_Context_Set, 5)
DEFINE_HOOK(483E30, CellClass_SetupLightingValues_Context_Set, 7)
{
	currentCell = R->ECX<CellClass*>();

	return 0;
}

DEFINE_HOOK_AGAIN(484150, CellClass_DoCellLighting_Context_Unset, 5)
DEFINE_HOOK(48403B, CellClass_SetupLightingValues_Context_Unset, 6)
{
	currentCell = nullptr;

	return 0;
}

//DEFINE_HOOK_AGAIN(54508F, IsometricTileTypeClass_light_545000_CustomPalette, 5)
DEFINE_HOOK(544F74, IsometricTileTypeClass_SetupLightConvert_CustomPalette, 5)
{
	LEA_STACK(BytePalette**, ppPalette, 0x0);

	ReplacePalette(ppPalette);
	
	return 0;
}
*/

DEFINE_HOOK(546E81, Load_Tiles_Into_Memory, 6)
{
	GET(CellClass*, pCell, EDI);
	const int TileType = pCell->IsoTileTypeIndex;

	if (TileType != 0xFFFF && IsometricTileTypeClass::Array->ValidIndex(TileType))
	{
		auto pTile = IsometricTileTypeClass::Array->GetItem(TileType);

		char fileName[sizeof(pTile->FileName) + 1];
		strcpy_s(fileName, pTile->FileName);
		fileName[14] = 0;

		auto pTileExt = IsometricTileTypeExt::ExtMap.FindOrAllocate(pTile);

		Debug::Log("Load_Tiles_Into_Memory\n");
		Debug::Log("\t%s: %s\n", "FileName", fileName);
		Debug::Log("\t%s: %d\n", "TileSetNumber", pTileExt->TileSetNumber);
		Debug::Log("\n");
	}
	return 0;
}
