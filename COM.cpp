#include "COM.hpp"

#include <cstdio>

#include <Windows.h>

COM::COM() {
	const HRESULT res =
		CoInitializeEx(nullptr, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE | COINIT_SPEED_OVER_MEMORY);
	switch (res) {
		case S_OK:
		case S_FALSE:
		case RPC_E_CHANGED_MODE:
			m_ok = true;
			break;
		default:
			printf("CoInitializeEx() failed with error %x!\n", res);
	}
}

COM::~COM() {
	if (m_ok) {
		CoUninitialize();
	}
}
