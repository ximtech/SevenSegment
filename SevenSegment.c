#include "SevenSegment.h"

#define SEGMENT_COUNT 8

static const unsigned char ASCII_TABLE[] = {
        0x00, 0x86, 0x22, 0x7E, 0x7B, 0x63, 0x7E, 0x02,  /* !"#$%&'*/
        0x39, 0x0F, 0x63, 0x70, 0x80, 0x40, 0x80, 0x52,  /*()*+,-./*/
        0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,  /*01234567*/
        0x7F, 0x6F, 0x00, 0x00, 0x58, 0x48, 0x4C, 0xA7,  /*89:;<=>?*/
        0x5C, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x3D,  /*@ABCDEFG*/
        0x76, 0x06, 0x1E, 0x72, 0x38, 0x55, 0x54, 0x3F,  /*HIJKLMNO*/
        0x73, 0x67, 0x50, 0x6D, 0x78, 0x3E, 0x1C, 0x1D,  /*PQRSTUVW*/
        0x49, 0x6E, 0x5b, 0x39, 0x64, 0x0F, 0x23, 0x08   /*XYZ[\]^_*/
};

static SegmentType segmentType = COMMON_CATHODE;
static DigitCount digitCount = ONE_DIGIT;
static uint8_t segmentPinArray[SEGMENT_COUNT] = {[0 ... (SEGMENT_COUNT - 1)] = 0};
static uint8_t digitPinArray[EIGHT_DIGITS] = {[0 ... (EIGHT_DIGITS - 1)] = 0};
static uint8_t allDigitPins = 0;
static uint8_t allSegmentPins = 0;
static volatile DigitCount ledDigitNumber = ONE_DIGIT;
bool isLeadingZeroOn = false;

static void writeToPort(uint8_t portPin, uint8_t value);
static void writeValueToSegments(uint8_t value);

void initSevenSegment(SegmentType segType, DigitCount digCount, ...) {
    segmentType = segType;
    digitCount = digCount;

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    LL_AHB1_GRP1_EnableClock(SEGMENT_PERIPHERAL);
    LL_AHB1_GRP1_EnableClock(DIGIT_PERIPHERAL);

    LL_GPIO_ResetOutputPin(SEGMENT_PORT, SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G | SEGMENT_DP);
    GPIO_InitStruct.Pin = SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G | SEGMENT_DP;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    segmentPinArray[0] = SEGMENT_A;
    segmentPinArray[1] = SEGMENT_B;
    segmentPinArray[2] = SEGMENT_C;
    segmentPinArray[3] = SEGMENT_D;
    segmentPinArray[4] = SEGMENT_E;
    segmentPinArray[5] = SEGMENT_F;
    segmentPinArray[6] = SEGMENT_G;
    segmentPinArray[7] = SEGMENT_DP;

    for (int i = 0; i < SEGMENT_COUNT; ++i) {
        allSegmentPins |= segmentPinArray[i];
    }

    va_list valist;
    va_start(valist, digCount);    // initialize valist for digit PIN number of arguments

    for (uint8_t i = 0; i < digitCount; i++) {
        digitPinArray[i] = va_arg(valist, unsigned int);
        allDigitPins |= digitPinArray[i];
    }

    LL_GPIO_ResetOutputPin(DIGIT_PORT, allDigitPins);
    GPIO_InitStruct.Pin = allDigitPins;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;

    if (segType == COMMON_CATHODE) {
        GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    } else {
        GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
    }
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void setLeadingZeros(bool isZerosOn) {
    isLeadingZeroOn = isZerosOn;
}

void printNumberSevenSegment(uint32_t number) {
    selectLedDigitSevenSegment(ledDigitNumber);
    switch (ledDigitNumber) {
        case ONE_DIGIT:
            printDigitSevenSegment(number % 10);
            break;
        case TWO_DIGITS:
            printDigitSevenSegment(number % 100 / 10);
            if (number < 10 && !isLeadingZeroOn) powerOffAllSegments();
            break;
        case THREE_DIGITS:
            printDigitSevenSegment(number % 1000 / 100);
            if (number < 100 && !isLeadingZeroOn) powerOffAllSegments();
            break;
        case FOUR_DIGITS:
            printDigitSevenSegment(number % 10000 / 1000);
            if (number < 1000 && !isLeadingZeroOn) powerOffAllSegments();
            break;
        case FIVE_DIGITS:
            printDigitSevenSegment(number % 100000 / 10000);
            if (number < 10000 && !isLeadingZeroOn) powerOffAllSegments();
            break;
        case SIX_DIGITS:
            printDigitSevenSegment(number % 1000000 / 100000);
            if (number < 100000 && !isLeadingZeroOn) powerOffAllSegments();
            break;
        case SEVEN_DIGITS:
            printDigitSevenSegment(number % 10000000 / 1000000);
            if (number < 10000000 && !isLeadingZeroOn) powerOffAllSegments();
            break;
        case EIGHT_DIGITS:
            printDigitSevenSegment(number % 100000000 / 10000000);
            if (number < 100000000 && !isLeadingZeroOn) powerOffAllSegments();
            break;
        default:
            break;
    }

    ledDigitNumber++;
    if (ledDigitNumber > digitCount) {
        ledDigitNumber = ONE_DIGIT;
    }
}

void printSevenSegment(const char *string) {
    selectLedDigitSevenSegment(ledDigitNumber);

    uint8_t length = strlen(string);
    if (length >= ledDigitNumber) {
        char charToDisplay = string[length - ledDigitNumber];
        printCharSevenSegment(charToDisplay);
    } else {
        powerOffAllSegments();
    }

    ledDigitNumber++;
    if (ledDigitNumber > digitCount) {
        ledDigitNumber = ONE_DIGIT;
    }
}

void printSevenSegmentWithDP(const char *string, uint8_t position) {
    selectLedDigitSevenSegment(ledDigitNumber);

    uint8_t length = strlen(string);
    if (length >= ledDigitNumber) {

        char charToDisplay = string[length - ledDigitNumber];
        if (position == ledDigitNumber) {
            printCharSevenSegmentWithDP(charToDisplay);
        } else {
            printCharSevenSegment(charToDisplay);
        }
    } else {
        powerOffAllSegments();
    }

    ledDigitNumber++;
    if (ledDigitNumber > digitCount) {
        ledDigitNumber = ONE_DIGIT;
    }
}

void printfSevenSegment(char *format, ...) {
    char formatBuffer[digitCount + 1];
    va_list args;

    va_start(args, format);
    vsnprintf(formatBuffer, digitCount + 1, format, args);
    va_end(args);

    printSevenSegment(formatBuffer);
}

void printfSevenSegmentWithDP(uint8_t position, char *format, ...) {
    char formatBuffer[digitCount + 1];
    va_list args;

    va_start(args, format);
    vsnprintf(formatBuffer, digitCount + 1, format, args);
    va_end(args);

    printSevenSegmentWithDP(formatBuffer, position);
}

void selectLedDigitSevenSegment(uint8_t digit) {
    if (digit <= digitCount) {
        uint8_t currentDigitPin = digitPinArray[digit - 1];
        uint8_t allDigitPinsExcludeCurrent = (allDigitPins ^ currentDigitPin);

        if (segmentType == COMMON_CATHODE) {
            LL_GPIO_ResetOutputPin(DIGIT_PORT, currentDigitPin);// turn on current digit
            LL_GPIO_SetOutputPin(DIGIT_PORT, allDigitPinsExcludeCurrent);// turn off other digits
        } else {
            LL_GPIO_SetOutputPin(DIGIT_PORT, currentDigitPin);
            LL_GPIO_ResetOutputPin(DIGIT_PORT, allDigitPinsExcludeCurrent);
        }
    }
}

void printDigitSevenSegment(uint8_t digitToDisplay) {
    if (digitToDisplay >= 0 && digitToDisplay <= 9) {
        printCharSevenSegment(digitToDisplay + '0');
    }
}

void printDigitSevenSegmentWithDP(uint8_t digitToDisplay) {
    if (digitToDisplay >= 0 && digitToDisplay <= 9) {
        printCharSevenSegmentWithDP(digitToDisplay + '0');
    }
}

void printCharSevenSegmentWithDP(unsigned char symbol) {
    unsigned char value = ASCII_TABLE[symbol - ' '] | ASCII_TABLE['.' - ' '];
    writeValueToSegments(value);
}

void printCharSevenSegment(unsigned char symbol) {
    unsigned char value = ASCII_TABLE[symbol - ' '];
    writeValueToSegments(value);
}

void powerOffAllSegments() {
    if (segmentType == COMMON_CATHODE) {
        LL_GPIO_ResetOutputPin(SEGMENT_PORT, allSegmentPins);
    } else {
        LL_GPIO_SetOutputPin(SEGMENT_PORT, allSegmentPins);
    }
}

static void writeValueToSegments(uint8_t value) {
    for (uint8_t i = 0; i < SEGMENT_COUNT; i++) {
        if (segmentType == COMMON_CATHODE) {
            writeToPort(segmentPinArray[i], (value >> i) & 0x01);
        } else {
            writeToPort(segmentPinArray[i], ~(value >> i) & 0x01);
        }
    }
}

static void writeToPort(uint8_t portPin, uint8_t value) {
    if (value) {
        LL_GPIO_SetOutputPin(SEGMENT_PORT, portPin);
    } else {
        LL_GPIO_ResetOutputPin(SEGMENT_PORT, portPin);
    }
}