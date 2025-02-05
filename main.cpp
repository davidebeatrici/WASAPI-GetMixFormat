#include "COM.hpp"
#include "Enumerator.hpp"

#include <cstdio>
#include <optional>

using category_t = Endpoint::category_t;

static void printUsage() {
	printf("This program prints the current format for all input and output devices on the system.\n\n");

	printf("Optionally, you can also specify the audio category by passing it as an argument.\n");
	printf("Officially known values since Windows 10 21H1 go from 0 to 14:\n\n");

	printf("\t0 : All other streams (default)\n");
	printf("\t1 : (deprecated for Win10) Music, Streaming audio\n");
	printf("\t2 : (deprecated for Win10) Video with audio\n");
	printf("\t3 : VOIP, chat, phone call\n");
	printf("\t4 : Alarm, Ring tones\n");
	printf("\t5 : Sound effects, clicks, dings\n");
	printf("\t6 : Game sound effects\n");
	printf("\t7 : Background audio for games\n");
	printf("\t8 : In game player chat\n");
	printf("\t9 : Speech recognition\n");
	printf("\t10: Video with audio\n");
	printf("\t11: Music, Streaming audio\n");
	printf("\t12: Capture of far field speech\n");
	printf("\t13: Uniform, device agnostic speech processing\n");
	printf("\t14: Dictation, typing by voice\n");
}

static void printEndpointInfo(const Endpoint &endpoint, const std::optional< category_t > category) {
	printf("[%s] %s\n", endpoint.getID().data(), endpoint.getName().data());
	printf("Flow: %s\n", endpoint.flowText(endpoint.getFlow()));

	const auto format = endpoint.getFormat(category);

	printf("Format:\n");
	printf("\tTag:                   %x\n", format.tag);
	printf("\tChannels:              %hu\n", format.channels);
	printf("\tSamples per sec:       %u\n", format.samplesPerSec);
	printf("\tAvg bytes per sec:     %u\n", format.avgBytesPerSec);
	printf("\tBlock align:           %hu\n", format.blockAlign);

	if (format.bitsPerSample) {
		printf("\tBits per sample:       %hu\n", format.bitsPerSample);
		printf("\tValid bits per sample: %hu\n", format.validBitsPerSample);
	} else {
		printf("\tSamples per block:     %hu\n", format.samplesPerBlock);
	}
}

int main(const int argc, const char *argv[]) {
	std::optional< Endpoint::category_t > category = {};

	switch (argc) {
		case 1:
			break;
		case 2: {
			uint8_t value;
			if (sscanf_s(argv[1], "%2hhu", &value) == 1) {
				category = value;
				break;
			}

			[[fallthrough]];
		}
		default:
			printUsage();
			return 1;
	}

	COM com;
	if (!com) {
		return 2;
	}

	Enumerator enumerator;
	if (!enumerator) {
		return 3;
	}

	Endpoints endpoints = enumerator.getEndpoints();
	for (auto &endpoint : endpoints) {
		printf("\n");
		printEndpointInfo(endpoint, category);
	}

	return 0;
}
