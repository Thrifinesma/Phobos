#include "Body.h"

DEFINE_HOOK(547CF0, IsometricTileTypeClass_DrawTMP_SetPalette, 7)
{
	GET(IsometricTileTypeClass*, pThis, ECX);
	LEA_STACK(LightConvertClass**, ppLightConvert, 0x4);

	auto pData = IsometricTileTypeExt::ExtMap.Find(pThis);
	auto pOld = *ppLightConvert;

	if (pData->Palette) {
		if (!pData->LightConvert)
			pData->SetLightConvert();

		pData->LightConvert->UpdateColors(
			pOld->Color1.Red, pOld->Color1.Green, pOld->Color1.Blue, true);

		long id;
		SwizzleManagerClass::Instance.Fetch_Swizzle_ID(pOld, &id);
		SwizzleManagerClass::Instance.Here_I_Am(id, pData->LightConvert);

		*ppLightConvert = pData->LightConvert;
	}

	return 0;
}
