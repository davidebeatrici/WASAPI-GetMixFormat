#include "Enumerator.hpp"

#include <cstdio>

#include <mmdeviceapi.h>

Enumerator::Enumerator() {
	const HRESULT res = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
										 __uuidof(IMMDeviceEnumerator), reinterpret_cast< void ** >(&m_handle));
	if (res != S_OK) {
		printf("CoCreateInstance() failed with error %x!\n", res);
	}
}

Enumerator::~Enumerator() {
	if (m_handle) {
		m_handle->Release();
	}
}

Endpoints Enumerator::getEndpoints() {
	IMMDeviceCollection *collection;
	if (m_handle->EnumAudioEndpoints(eAll, DEVICE_STATE_ACTIVE, &collection) != S_OK) {
		return {};
	}

	UINT count = 0;
	collection->GetCount(&count);

	Endpoints ret;

	for (decltype(count) i = 0; i < count; ++i) {
		IMMDevice *device;
		if (collection->Item(i, &device) == S_OK) {
			ret.push_back(device);
		}
	}

	collection->Release();

	return ret;
}
