#ifndef O4S_HEADER
#define O4S_HEADER

#include <cstdint>

typedef uint16_t word;
typedef uint8_t byte;

struct __attribute__ ((packed, scalar_storage_order("little-endian"))) O4SHeader {
    byte magicNumber[3] = {79,52,83};
    byte version = 1;
    byte sidMode;
    byte reservedByte = 0;
    word patternLength;
    word sequenceLength;
    word reservedWord = 0;
};

struct __attribute__ ((packed, scalar_storage_order("little-endian"))) O4SInstrument {
    word voice1Frequency;
    word voice1PulseWidth;
    byte voice1Control;
    byte voice1AttackDecay;
    byte voice1SustainRelease;

    word voice2Frequency;
    word voice2PulseWidth;
    byte voice2Control;
    byte voice2AttackDecay;
    byte voice2SustainRelease;

    word voice3Frequency;
    word voice3PulseWidth;
    byte voice3Control;
    byte voice3AttackDecay;
    byte voice3SustainRelease;

    word filterCutoffFrequency;
    byte filterControl1;
    byte filterControl2;
};

struct __attribute__ ((packed, scalar_storage_order("little-endian"))) O4SPattern {
    word patternLength;

    byte *patternData;
};

struct __attribute__ ((packed, scalar_storage_order("little-endian"))) O4SSequence {
    word sequenceLength;

    byte *sequenceData;
};

#endif