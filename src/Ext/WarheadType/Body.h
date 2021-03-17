#pragma once
#include <WarheadTypeClass.h>

#include <Helpers/Macro.h>
#include "../_Container.hpp"
#include "../../Utilities/TemplateDef.h"

class WarheadTypeExt
{
public:
	using base_type = WarheadTypeClass;

	class ExtData final : public Extension<WarheadTypeClass>
	{
	public:
		Valueable<bool> SpySat;
		Valueable<bool> BigGap;
		Valueable<int> TransactMoney;
		ValueableVector<AnimTypeClass*> SplashList;
		Valueable<bool> SplashList_PickRandom;
		Valueable<bool> RemoveDisguise;
		Valueable<bool> RemoveDisguise_AffectAllies;
		Valueable<bool> RemoveDisguise_ApplyCellSpread;
		Valueable<bool> RemoveMindControl;
		Valueable<bool> RemoveMindControl_AffectAllies;
		Valueable<bool> RemoveMindControl_ApplyCellSpread;

		ExtData(WarheadTypeClass* OwnerObject) : Extension<WarheadTypeClass>(OwnerObject),
			SpySat(false),
			BigGap(false),
			TransactMoney(0),
			SplashList(),
			SplashList_PickRandom(false),
			RemoveDisguise(false),
			RemoveDisguise_AffectAllies(false),
			RemoveDisguise_ApplyCellSpread(true),
			RemoveMindControl(false),
			RemoveMindControl_AffectAllies(false),
			RemoveMindControl_ApplyCellSpread(true)
		{ }

		virtual ~ExtData() = default;

		virtual void Initialize() override;

		virtual void LoadFromINIFile(CCINIClass* pINI) override;

		virtual void InvalidatePointer(void* ptr, bool bRemoved) override {
		}

		virtual void LoadFromStream(PhobosStreamReader& Stm) override;

		virtual void SaveToStream(PhobosStreamWriter& Stm) override;
	
	private:
		template <typename T>
		void Serialize(T& Stm);
	};

	class ExtContainer final : public Container<WarheadTypeExt> {
	public:
		ExtContainer();
		~ExtContainer();

		virtual void InvalidatePointer(void* ptr, bool bRemoved) override;
	};

	static ExtContainer ExtMap;
	static bool LoadGlobals(PhobosStreamReader& Stm);
	static bool SaveGlobals(PhobosStreamWriter& Stm);
};
