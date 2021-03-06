#include "TheaterExt.h"

void TheaterExt::Init()
{
	Debug::Log("Init custom theater %s\n", ID);
	if (!strlen(ID)) {
		strcpy_s(ID, sizeof(ID), TheaterProto::Array[0].name);
	}

	if (_stricmp(ID, TheaterExt::nameLast) == 0) {
		return;
	}

	if (protectFlags == 0) {
		VirtualProtect(Theater::Array, sizeof(Theater) * 6, PAGE_EXECUTE_READWRITE, &protectFlags);
	}

	TheaterExt::LoadFromINI();
	ScenarioClass::Instance->Theater = static_cast<TheaterType>(TheaterExt::slot);

	auto slotPrt = &Theater::Array[slot];
	strcpy_s(nameLast, ID);
	strcpy_s(slotPrt->Extension, suffix);
	strcpy_s(slotPrt->Letter, letter);
	slotPrt->RadarTerrainBrightness = radarBrightness;

	TheaterExt::LoadSpecificMixs();
}

void TheaterExt::LoadFromINI()
{
	specificMixNames.Clear();
	specificMixNames_buff[0] = 0;

	slot = TheaterProto::FindIndexOrDefault(ID);

	CCINIClass* pINI = TheaterExt::OpenConfig(ID);
	if (pINI) {
		const char* section = "THEATER";

		slot = pINI->ReadInteger(section, "Slot", slot);
		auto proto = TheaterProto::GetOrDefault(slot);

		pINI->ReadString(section, "Suffix", proto->suffix, suffix);
		pINI->ReadString(section, "Letter", proto->letter, letter);
		pINI->ReadString(section, "ControlFile", proto->controlFileName, controlFileName);
		pINI->ReadString(section, "Palette.Overlay", proto->palOverlay, palOverlay);
		pINI->ReadString(section, "Palette.Unit", proto->palUnit, palUnit);
		pINI->ReadString(section, "Palette.ISO", proto->palISO, palISO);
		pINI->ReadString(section, "MixArray", proto->specificMix, specificMixNames_buff);
		radarBrightness = (float)pINI->ReadDouble(section, "RadarBrightness", proto->radarBrightness);

		Phobos::CloseConfig(pINI);
	}
	else {
		auto proto = TheaterProto::GetOrDefault(slot);

		strcpy_s(suffix, proto->suffix);
		strcpy_s(letter, proto->letter);
		strcpy_s(controlFileName, proto->controlFileName);
		strcpy_s(palOverlay, proto->palOverlay);
		strcpy_s(palUnit, proto->palUnit);
		strcpy_s(palISO, proto->palISO);
		strcpy_s(specificMixNames_buff, proto->specificMix);
		radarBrightness = (float)proto->radarBrightness;
	}

	// Parse MixArray
	if (specificMixNames_buff[0]) {
		char* context = nullptr;
		char* cur = strtok_s(specificMixNames_buff, Phobos::readDelims, &context);
		while (cur) {
			specificMixNames.AddItem(Trim::FullTrim(cur));
			cur = strtok_s(nullptr, Phobos::readDelims, &context);
		}
	}
}

void TheaterExt::LoadSpecificMixs()
{
	TheaterExt::UnloadSpecificMixs();
	Debug::Log("Load specific Theater Mixfiles:\n");
	for (int i = 0; i < specificMixNames.Count; i++)
	{
		char* pName = specificMixNames.GetItem(i);
		Debug::Log("\t%s\n", pName);
		specificMixFiles.AddItem(
			GameCreate<MixFileClass>(pName)
		);
	}
	Debug::Log("\n");
}

void TheaterExt::UnloadSpecificMixs()
{
	Debug::Log("Unload specific Theater Mixfiles:\n");
	for (int i = 0; i < specificMixFiles.Count; i++)
	{
		MixFileClass* pMix = specificMixFiles.GetItem(i);
		Debug::Log("\t%s\n", pMix->FileName);
		GameDelete(pMix);
	}
	specificMixFiles.Clear();
	Debug::Log("\n");
}

// =============================
// hooks

 // CCINIClass::ReadTheater
DEFINE_HOOK(47589F, CCINIClass_ReadTheater_Slot, 5)
{
	Debug::Log("Init custom theater\n");
	GET(const char*, ID, ECX);
	strcpy_s(TheaterExt::ID, ID);

	auto result = TheaterProto::FindIndexOrDefault(ID);

	CCINIClass* pINI = TheaterExt::OpenConfig(ID);
	if (pINI) {
		const char* section = "THEATER";
		result = pINI->ReadInteger(section, "Slot", result);
		Phobos::CloseConfig(pINI);
	}

	R->EAX(result);
	return 0x4758A4;
}

DEFINE_HOOK(5349CF, Theaters_Init, 0)
{
	Debug::Log("Init custom theater = Theaters_Init\n");
	Game::SetProgress(8);
	//GET(int, slot, ECX);

	TheaterExt::Init();

	R->EAX(FileSystem::LoadFile(TheaterExt::palOverlay));
	R->EDI(0);
	R->EDX(0);
	return 0x534C09;
}

DEFINE_HOOK(534CA9, Theaters_palUnit, 0)
{
	Debug::Log("Init custom theater = Theaters_palUnit\n");
	R->ESI(FileSystem::LoadFile(TheaterExt::palUnit));
	Game::SetProgress(12);
	return 0x534CCA;
}

DEFINE_HOOK(54547F, Theaters_palISO, 0)
{
	Debug::Log("Init custom theater = Theaters_palISO\n");
	R->ECX(TheaterExt::palISO);
	return 0x5454A2;
}

DEFINE_HOOK(5454F0, Theaters_controlFileName, 0)
{
	Debug::Log("Init custom theater = Theaters_controlFileName\n");
	R->ECX(TheaterExt::controlFileName);
	return 0x545513;
}

// DisplayClass::Read_INI
DEFINE_HOOK(4ACFD8, Theater_Skip_ReReadINI, 0)
{
	Debug::Log("Init custom theater = Theater_Skip_ReReadINI\n");
	return 0x4ACFF6;
}

// AnimTypeClass_load_image
DEFINE_HOOK(427A9A, Theaters_LastTheater_1, 0)
{
	Debug::Log("Init custom theater = Theaters_LastTheater_1\n");
	return TheaterExt::SameTheater() ? 0x427AA6 : 0x427B40;
}

// DisplayClass::Read_INI
DEFINE_HOOK(4AD03C, Theaters_LastTheater_2, 0)
{
	Debug::Log("Init custom theater = Theaters_LastTheater_2\n");
	R->ECX(TheaterExt::slot);
	return TheaterExt::SameTheater() ? 0x4AD04F : 0x4AD046;
}

// MapGeneratorClass_initrandommap_599650
DEFINE_HOOK(59A1DE, Theaters_LastTheater_3, 0)
{
	Debug::Log("Init custom theater = Theaters_LastTheater_3\n");
	return TheaterExt::SameTheater() ? 0x59A1F0 : 0x59A1E7;
}

DEFINE_HOOK(59A066, Theaters_LastTheater_33, 6)
{
	
	return 0;
}

// MouseClass::Decode_Pointers
DEFINE_HOOK(5BE63C, Theaters_LastTheater_4, 0)
{
	Debug::Log("Init custom theater = Theaters_LastTheater_4\n");
	return TheaterExt::SameTheater() ? 0x5BE64E : 0x5BE645;
}

DEFINE_HOOK(689384, Theaters_Save, 6)
{
	GET(IStream*, pStm, ESI);
	pStm->Write(&TheaterExt::ID, sizeof(TheaterExt::ID), 0);
	return 0;
}

DEFINE_HOOK(6894BA, Theaters_Load, 8)
{
	GET(IStream*, pStm, ESI);
	pStm->Read(&TheaterExt::ID, sizeof(TheaterExt::ID), 0);
	return 0;
}

//#include <StringTable.h>
//DEFINE_HOOK(5970EA, RMG_EnableDesert, 9)
//{
//	GET(HWND, hWnd, EDI);
//
//	//List the item
//	LRESULT result =
//		SendMessageA(
//			hWnd,
//			WW_CB_ADDITEM,		//CUSTOM BY WESTWOOD
//			0,
//			reinterpret_cast<LPARAM>(StringTable::LoadString("Name:Desert1"))); // oh pd
//
//	//Set the item data
//	SendMessageA(
//		hWnd,
//		CB_SETITEMDATA,
//		result,
//		5);			//Make it actually be desert
//
//	return 0;
//}
