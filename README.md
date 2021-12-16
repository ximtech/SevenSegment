# SevenSegment

***STM32*** Low Layer(LL) library. A seven-segment display is a form of electronic display device for displaying decimal
numerals that is an alternative to the more complex dot matrix displays.

### Features

- From 1 to 8 digits handling
- printf function support
- Decimal point set function

<img src="https://github.com/ximtech/SevenSegment/blob/main/example/seven_segment.PNG" alt="image" width="300"/>

### Add as CPM project dependency

How to add CPM to the project, check the [link](https://github.com/cpm-cmake/CPM.cmake)

```cmake
CPMAddPackage(
        NAME SevenSegment
        GITHUB_REPOSITORY ximtech/SevenSegment
        GIT_TAG origin/main)
```

### Project configuration

1. Start project with STM32CubeMX:
2. Select: Project Manager -> Advanced Settings -> GPIO -> LL
3. Generate Code
4. Add sources to project:

```cmake
include_directories(${includes}
        ${SEVEN_SEGMENT_DIRECTORY})   # source directories

file(GLOB_RECURSE SOURCES ${sources}
        ${SEVEN_SEGMENT_SOURCES})    # source files
```

3. Then Build -> Clean -> Rebuild Project

## Wiring

- <img src="https://github.com/ximtech/SevenSegment/blob/main/example/wiring.png" alt="image" width="300"/>

## Usage

The `SevenSegment.h` has default configuration defines. Override them in `main.h` if needed

```c
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
```

- Usage example: [link](https://github.com/ximtech/SevenSegment/blob/main/example/example.c)
