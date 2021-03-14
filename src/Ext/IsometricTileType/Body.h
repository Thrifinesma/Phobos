#pragma once
#include <IsometricTileTypeClass.h>

#include <Helpers/Macro.h>
#include "../_Container.hpp"
#include "../../Utilities/TemplateDef.h"

class IsometricTileTypeExt
{
public:
	using base_type = IsometricTileTypeClass;

	class ExtData final : public Extension<IsometricTileTypeClass>
	{
	public:
		int TileSetNumber;
		char PaletteBuffer[32];
		BytePalette* Palette;
		LightConvertClass* LightConvert;

		ExtData(IsometricTileTypeClass* OwnerObject) : Extension<IsometricTileTypeClass>(OwnerObject),
			TileSetNumber(-1),
			PaletteBuffer(""),
			Palette(nullptr),
			LightConvert(nullptr)
		{ }

		virtual void LoadFromINIFile(CCINIClass* pINI) override;
		virtual ~ExtData() = default;

		virtual void InvalidatePointer(void* ptr, bool bRemoved) override {}

		virtual void LoadFromStream(IStream* Stm);
		virtual void SaveToStream(IStream* Stm);

		const char* GetSectionName();
	};

	class ExtContainer final : public Container<IsometricTileTypeExt> {
	public:
		ExtContainer();
		~ExtContainer();
	};

	static ExtContainer ExtMap;
};
