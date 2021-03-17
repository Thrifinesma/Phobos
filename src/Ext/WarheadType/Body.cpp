#include "Body.h"

#include <WarheadTypeClass.h>
#include <AnimTypeClass.h>

template<> const DWORD Extension<WarheadTypeClass>::Canary = 0x22222222;
WarheadTypeExt::ExtContainer WarheadTypeExt::ExtMap;

void WarheadTypeExt::ExtData::Initialize() {

}
// =============================
// load / save

void WarheadTypeExt::ExtData::LoadFromINIFile(CCINIClass* const pINI) {
	auto pThis = this->OwnerObject();
	const char* pSection = pThis->ID;

	if (!pINI->GetSection(pSection)) {
		return;
	}

	INI_EX exINI(pINI);

	this->SpySat.Read(exINI, pSection, "SpySat");
	this->BigGap.Read(exINI, pSection, "BigGap");
	this->TransactMoney.Read(exINI, pSection, "TransactMoney");
	this->SplashList.Read(exINI, pSection, "SplashList");
	this->SplashList_PickRandom.Read(exINI, pSection, "SplashList.PickRandom");
	this->RemoveDisguise.Read(exINI, pSection, "RemoveDisguise");
	this->RemoveDisguise_AffectAllies.Read(exINI, pSection, "RemoveDisguise.AffectAllies");
	this->RemoveDisguise_ApplyCellSpread.Read(exINI, pSection, "RemoveDisguise.ApplyCellSpread");
	this->RemoveMindControl.Read(exINI, pSection, "RemoveMindControl");
	this->RemoveMindControl_AffectAllies.Read(exINI, pSection, "RemoveMindControl.AffectAllies");
	this->RemoveMindControl_ApplyCellSpread.Read(exINI, pSection, "RemoveMindControl.ApplyCellSpread");
}

template <typename T>
void WarheadTypeExt::ExtData::Serialize(T& Stm) {
	Stm
		.Process(this->SpySat)
		.Process(this->BigGap)
		.Process(this->TransactMoney)
		.Process(this->SplashList)
		.Process(this->SplashList_PickRandom)
		.Process(this->RemoveDisguise)
		.Process(this->RemoveDisguise_AffectAllies)
		.Process(this->RemoveDisguise_ApplyCellSpread)
		.Process(this->RemoveMindControl)
		.Process(this->RemoveMindControl_AffectAllies)
		.Process(this->RemoveMindControl_ApplyCellSpread)
		;
}

void WarheadTypeExt::ExtData::LoadFromStream(PhobosStreamReader& Stm) {
	Extension<WarheadTypeClass>::LoadFromStream(Stm);
	this->Serialize(Stm);
}

void WarheadTypeExt::ExtData::SaveToStream(PhobosStreamWriter& Stm) {
	Extension<WarheadTypeClass>::SaveToStream(Stm);
	this->Serialize(Stm);
}

bool WarheadTypeExt::LoadGlobals(PhobosStreamReader& Stm) {
	return Stm
		.Success();
}

bool WarheadTypeExt::SaveGlobals(PhobosStreamWriter& Stm) {
	return Stm
		.Success();
}


// =============================
// container

WarheadTypeExt::ExtContainer::ExtContainer() : Container("WarheadTypeClass") {
}

WarheadTypeExt::ExtContainer::~ExtContainer() = default;

void WarheadTypeExt::ExtContainer::InvalidatePointer(void* ptr, bool bRemoved) {
	
}

// =============================
// container hooks

DEFINE_HOOK(75D1A9, WarheadTypeClass_CTOR, 7)
{
	GET(WarheadTypeClass*, pItem, EBP);

	WarheadTypeExt::ExtMap.FindOrAllocate(pItem);
	return 0;
}

DEFINE_HOOK(75E5C8, WarheadTypeClass_SDDTOR, 6)
{
	GET(WarheadTypeClass*, pItem, ESI);

	WarheadTypeExt::ExtMap.Remove(pItem);
	return 0;
}

DEFINE_HOOK_AGAIN(75E2C0, WarheadTypeClass_SaveLoad_Prefix, 5)
DEFINE_HOOK(75E0C0, WarheadTypeClass_SaveLoad_Prefix, 8)
{
	GET_STACK(WarheadTypeClass*, pItem, 0x4);
	GET_STACK(IStream*, pStm, 0x8);

	WarheadTypeExt::ExtMap.PrepareStream(pItem, pStm);

	return 0;
}

DEFINE_HOOK(75E2AE, WarheadTypeClass_Load_Suffix, 7)
{
	WarheadTypeExt::ExtMap.LoadStatic();
	return 0;
}

DEFINE_HOOK(75E39C, WarheadTypeClass_Save_Suffix, 5)
{
	WarheadTypeExt::ExtMap.SaveStatic();
	return 0;
}

DEFINE_HOOK_AGAIN(75DEAF, WarheadTypeClass_LoadFromINI, 5)
DEFINE_HOOK(75DEA0, WarheadTypeClass_LoadFromINI, 5)
{
	GET(WarheadTypeClass*, pItem, ESI);
	GET_STACK(CCINIClass*, pINI, 0x150);

	WarheadTypeExt::ExtMap.LoadFromINI(pItem, pINI);
	return 0;
}
