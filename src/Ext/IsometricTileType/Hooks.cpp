#include "Body.h"

DEFINE_HOOK(547CF0, IsometricTileTypeClass_DrawTMP_SetPalette, 7)
{
	GET(IsometricTileTypeClass*, pThis, ECX);
	LEA_STACK(LightConvertClass**, ppLightConvert, 0x4);

	auto pData = IsometricTileTypeExt::ExtMap.Find(pThis);

	if (pData->Palette) {
		if (!pData->LightConvert)
			pData->SetLightConvert();

		*ppLightConvert = pData->LightConvert;
	}

	return 0;
}
