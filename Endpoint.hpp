#pragma once

#include <optional>
#include <string>

struct IMMDevice;

class Endpoint {
public:
	enum Flow : std::uint8_t { None, Input, Output, Both };

	using category_t = std::uint8_t;

	struct Format {
		std::uint16_t tag;
		std::uint16_t channels;
		std::uint32_t samplesPerSec;
		std::uint32_t avgBytesPerSec;
		std::uint16_t blockAlign;
		std::uint16_t bitsPerSample;
		union {
			std::uint16_t validBitsPerSample; // Bits of precision
			std::uint16_t samplesPerBlock;    // If bitsPerSample == 0
		};
	};

	Endpoint(Endpoint &&endpoint);
	Endpoint(IMMDevice *handle);
	~Endpoint();

	constexpr explicit operator bool() { return m_handle != nullptr; }

	static constexpr const char *flowText(const Flow flow) {
		switch (flow) {
			case Input:
				return "Input";
			case Output:
				return "Output";
			case Both:
				return "Both";
		}

		return "None";
	}

	std::string getID() const;
	std::string getName() const;
	Flow getFlow() const;
	std::optional< Format > getFormat(std::optional< category_t > category) const;

private:
	IMMDevice *m_handle = nullptr;
};
