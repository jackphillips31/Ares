#pragma once
#include <Ares.h>
#include <windows.h>
#include <imgui.h>

void* LoadFromResource(const wchar_t* resourceName, DWORD& fontSize)
{
	HINSTANCE hInstance = GetModuleHandle(nullptr);

	// Locate the resource
	HRSRC hRes = FindResource(hInstance, resourceName, RT_RCDATA);
	if (!hRes)
	{
		AR_ASSERT(false, "Resource not found!");
		return nullptr;
	}

	// Load and lock the resource
	HGLOBAL hResData = LoadResource(hInstance, hRes);
	if (!hResData) return nullptr;

	void* pFontData = LockResource(hResData);
	fontSize = SizeofResource(hInstance, hRes);
	return pFontData;
}