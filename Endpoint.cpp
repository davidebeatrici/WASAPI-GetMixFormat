#include "Endpoint.hpp"

#include <cstring>
#include <utility>

#include <audioclient.h>
#include <functiondiscoverykeys.h>
#include <mmdeviceapi.h>

using Flow   = Endpoint::Flow;
using Format = Endpoint::Format;

static std::string utf16To8(const wchar_t *utf16) {
	std::string ret;

	const auto utf16Len = static_cast< int >(wcslen(utf16));
	const auto utf8Len  = WideCharToMultiByte(CP_UTF8, 0, utf16, utf16Len, nullptr, 0, nullptr, nullptr);

	ret.resize(utf8Len);

	if (WideCharToMultiByte(CP_UTF8, 0, utf16, utf16Len, &ret[0], utf8Len, nullptr, nullptr) <= 0) {
		return {};
	}

	return ret;
}

Endpoint::Endpoint(Endpoint &&endpoint) : m_handle(std::exchange(endpoint.m_handle, nullptr)) {
}

Endpoint::Endpoint(IMMDevice *handle) : m_handle(handle) {
}

Endpoint::~Endpoint() {
	if (m_handle) {
		m_handle->Release();
	}
}

std::string Endpoint::getID() const {
	wchar_t *deviceID;
	if (m_handle->GetId(&deviceID) != S_OK) {
		return {};
	}

	const auto ret = utf16To8(deviceID);

	CoTaskMemFree(deviceID);

	return ret;
}

std::string Endpoint::getName() const {
	IPropertyStore *store;
	if (m_handle->OpenPropertyStore(STGM_READ, &store) != S_OK) {
		return {};
	}

	std::string ret;

	PROPVARIANT varName{};
	if (store->GetValue(PKEY_Device_FriendlyName, &varName) == S_OK) {
		ret = utf16To8(varName.pwszVal);
		PropVariantClear(&varName);
	}

	store->Release();

	return ret;
}

Flow Endpoint::getFlow() const {
	IMMEndpoint *endpoint;
	if (m_handle->QueryInterface(__uuidof(IMMEndpoint), reinterpret_cast< void ** >(&endpoint)) != S_OK) {
		return None;
	}

	EDataFlow dataflow;
	if (endpoint->GetDataFlow(&dataflow) != S_OK) {
		dataflow = EDataFlow_enum_count;
	}

	endpoint->Release();

	switch (dataflow) {
		case eRender:
			return Output;
		case eCapture:
			return Input;
		case eAll:
			return Both;
	}

	return None;
}

Format Endpoint::getFormat() const {
	IAudioClient3 *client;
	HRESULT res =
		m_handle->Activate(__uuidof(IAudioClient3), CLSCTX_ALL, nullptr, reinterpret_cast< void ** >(&client));
	if (res != S_OK) {
		printf("IMMDevice::Activate() failed with error %x!\n", res);
		return {};
	}

	WAVEFORMATEX *fmt;
	res = client->GetMixFormat(&fmt);
	if (res != S_OK) {
		printf("IAudioClient::GetMixFormat() failed with error %x!\n", res);
		client->Release();
		return {};
	}

	Format ret = {};

	ret.tag            = fmt->wFormatTag;
	ret.channels       = fmt->nChannels;
	ret.samplesPerSec  = fmt->nSamplesPerSec;
	ret.avgBytesPerSec = fmt->nAvgBytesPerSec;
	ret.blockAlign     = fmt->nBlockAlign;
	ret.bitsPerSample  = fmt->wBitsPerSample;

	if (ret.tag == WAVE_FORMAT_EXTENSIBLE) {
		const auto fmtex       = reinterpret_cast< WAVEFORMATEXTENSIBLE       *>(fmt);
		ret.validBitsPerSample = ret.samplesPerBlock = fmtex->Samples.wReserved;
	} else {
		ret.validBitsPerSample = ret.bitsPerSample;
	}

	CoTaskMemFree(fmt);

	return ret;
}
