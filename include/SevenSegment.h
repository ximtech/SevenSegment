#pragma once

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "main.h"

#if !defined(SEGMENT_PORT) || !defined(DIGIT_PORT)
#define SEGMENT_PERIPHERAL  LL_AHB1_GRP1_PERIPH_GPIOA
#define SEGMENT_PORT GPIOA
#define SEGMENT_A  LL_GPIO_PIN_0
#define SEGMENT_B  LL_GPIO_PIN_1
#define SEGMENT_C  LL_GPIO_PIN_2
#define SEGMENT_D  LL_GPIO_PIN_3
#define SEGMENT_E  LL_GPIO_PIN_4
#define SEGMENT_F  LL_GPIO_PIN_5
#define SEGMENT_G  LL_GPIO_PIN_6
#define SEGMENT_DP LL_GPIO_PIN_7

#define DIGIT_PERIPHERAL  LL_AHB1_GRP1_PERIPH_GPIOB
#define DIGIT_PORT GPIOB
#endif

typedef enum SegmentType {
    COMMON_CATHODE, COMMON_ANODE
} SegmentType;

typedef enum DigitCount {
    ONE_DIGIT    =  1,
    TWO_DIGITS   =  2,
    THREE_DIGITS =  3,
    FOUR_DIGITS  =  4,
    FIVE_DIGITS  =  5,
    SIX_DIGITS   =  6,
    SEVEN_DIGITS =  7,
    EIGHT_DIGITS =  8
} DigitCount;


void initSevenSegment(SegmentType segType, DigitCount digCount, ...);	// Example: initSevenSegment(COMMON_CATHODE, FOUR_DIGITS, DIGIT_4_Pin, DIGIT_3_Pin, DIGIT_2_Pin, DIGIT_1_Pin);
void setLeadingZeros(bool isLeadingZeroOn); // disabled by default

void printNumberSevenSegment(uint32_t number);  // call this functions continuously from 1-2 ms period, see attached example
void printSevenSegment(const char *string);
void printSevenSegmentWithDP(const char *string, uint8_t position);
void printfSevenSegment(char *format, ...);
void printfSevenSegmentWithDP(uint8_t position, char *format, ...);

void selectLedDigitSevenSegment(uint8_t digit);
void printDigitSevenSegment(uint8_t digitToDisplay);
void printDigitSevenSegmentWithDP(uint8_t digitToDisplay);

void printCharSevenSegment(unsigned char symbol);
void printCharSevenSegmentWithDP(unsigned char symbol);

void powerOffAllSegments();