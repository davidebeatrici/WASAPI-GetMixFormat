#include "COM.hpp"
#include "Enumerator.hpp"

#include <cstdio>

using Format = Endpoint::Format;

static void printEndpointInfo(const Endpoint &endpoint) {
	printf("[%s] %s\n", endpoint.getID().data(), endpoint.getName().data());
	printf("Flow: %s\n", endpoint.flowText(endpoint.getFlow()));

	Format format = endpoint.getFormat();

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

int main() {
	COM com;
	if (!com) {
		return 1;
	}

	Enumerator enumerator;
	if (!enumerator) {
		return 2;
	}

	Endpoints endpoints = enumerator.getEndpoints();
	for (auto &endpoint : endpoints) {
		printf("\n");
		printEndpointInfo(endpoint);
	}

	return 0;
}
