#pragma once

#include "Endpoint.hpp"

#include <vector>

using Endpoints = std::vector< Endpoint >;

struct IMMDeviceEnumerator;

class Enumerator {
public:
	Enumerator();
	~Enumerator();

	constexpr explicit operator bool() { return m_handle != nullptr; }

	Endpoints getEndpoints();

private:
	IMMDeviceEnumerator *m_handle = nullptr;
};
