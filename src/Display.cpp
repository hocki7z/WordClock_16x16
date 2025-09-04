/*
 * Display.h
 *
 *  Created on: 02.09.2025
 *      Author: hocki
 */

#include <Arduino.h>

#include "Display.h"

/* Delay in msec between colors */
constexpr uint32_t mcUpdateDelay = 1000;


/**
 * @brief Constructor
 */
Display::Display()
{
    // do nothing
}

/**
 * @brief Destructor
 */
Display::~Display()
{
    // do nothing
}

void Display::Init(void)
{
    /* Initialize FastLED */
    // Initialize LEDs
    FastLED.addLeds<LED_TYPE, LED_DATA_PIN, LED_COLOR_ORDER>(mLeds, LED_NUMBER);
    // Disable dithering mode
	FastLED.setDither(DISABLE_DITHER);
    // Color-corrected LED brightness
    FastLED.setCorrection(TypicalLEDStrip);
    // Set default LED brightness
    FastLED.setBrightness(LED_DEFAULT_BRIGHTNESS);
    // Switch OFF all LEDs
    FastLED.clear();
}

void Display::SetTime(const uint8_t aHour, const uint8_t aMinute, const uint8_t aSecond)
{

}

void Display::Update(void)
{
    /* Get current system tick */
    uint32_t wCurrMillis = millis();

    //TODO handle millis rollover
    //     e.g. https://arduino.stackexchange.com/questions/12587/how-can-i-handle-the-millis-rollover

    /* Check time ticks delta */
    if ((wCurrMillis - mPrevMillis) >= mcUpdateDelay)
    {
        /* Update previous time tick */
        mPrevMillis = wCurrMillis;

        Clear();

        // Test Wordclock: Es ist zehn nach zwölf Uhr + 4 Minuten
        PaintLine(  3, 14,  2, CRGB::Red);          // Word: es
        PaintLine(  4,  7,  3, CRGB::Red);          // Word: ist
        PaintLine(  8,  0,  4, CRGB::Green);        // Word: zehn
        PaintLine( 11,  0,  4, CRGB::Red);          // Word: nach
        PaintLine( 14,  4,  5, CRGB::Green);        // Word: zwölf
        PaintLine( 14, 13,  3, CRGB::Blue);         // Word: Uhr
        PaintLine( 15,  1,  1, CRGB::DarkOrange);   // Word: +
        PaintLine( 15,  5,  1, CRGB::DarkOrange);   // Word: 4
        PaintLine( 15,  8,  7, CRGB::DarkOrange);   // Word: Minuten

        Transform();

        FastLED.show();
    }
}

void Display::Clear(void)
{
    /* Clear mLeds array with black color */
    for (uint16_t wI = 0; wI < LED_NUMBER; wI++)
    {
        mLeds[wI] = CRGB::Black;
    }
}

void Display::Transform(void)
{
    for (uint8_t wRow = 0; wRow < MATRIX_HEIGHT; wRow++)
    {
        bool wReverseRow = false;

        if ((wRow % 2) == 0)
        {
            // it is even row
            wReverseRow = true;
        }
        else
        {
            // it is odd row
        }

        if (wReverseRow)
        {
            uint16_t wRowStart = (wRow * MATRIX_WIDTH); //  e.g. 0 / 16 / 32 / 48 ... 224 / 240

            /* Pointer to first element */
            CRGB* wLeftElem  = &mLeds[wRowStart];
            /* Pointer to last element */
            CRGB* wRightElem = &mLeds[wRowStart + MATRIX_WIDTH - 1] ;

            while (wLeftElem < wRightElem)
            {
                CRGB wTemp  = *wLeftElem;
                *wLeftElem  = *wRightElem;
                *wRightElem = wTemp;

                wLeftElem++;
                wRightElem--;
            }
        }
    }
}

void Display::SetColor(const uint16_t aLedIndex, const CRGB aColor)
{
    if (aLedIndex < LED_NUMBER)
    {
        mLeds[aLedIndex] = aColor;
    }
}

void Display::PaintPixel(const uint16_t aRow, const uint16_t aCol, const CRGB aColor)
{
    /* Check input parameters */
    if ((aRow < MATRIX_HEIGHT) &&
        (aCol < MATRIX_WIDTH))
    {
        /* Calculate LED index */
        uint16_t wLedIndex = (aRow * MATRIX_HEIGHT) + aCol;

        /* Set LED color */
        SetColor(wLedIndex, aColor);
    }
}

void Display::PaintLine(const uint16_t aRow, const uint16_t aCol, const uint16_t aLength, const CRGB aColor)
{
    /* Check input parameters */
    if ((aRow < MATRIX_HEIGHT) &&
        (aCol < MATRIX_WIDTH)  && ((aCol + aLength) <= MATRIX_WIDTH))
    {
        for (uint16_t wI = 0; wI < aLength; wI++)
        {
            PaintPixel(aRow, (aCol + wI), aColor);
        }
    }
}

void Display::PaintArea(const uint16_t aRow, const uint16_t aCol, const uint16_t aWidth, const uint16_t aHeight, const CRGB aColor)
{
    /* Check input parameters */
    if ((aRow < MATRIX_HEIGHT) && ((aRow + aHeight) <= MATRIX_HEIGHT) &&
        (aCol < MATRIX_WIDTH)  && ((aCol + aWidth)  <= MATRIX_WIDTH))
    {
        for (uint16_t wI = 0; wI < aHeight; wI++)
        {
            PaintLine((aRow + wI), aCol, aWidth, aColor);
        }
    }
}

void Display::PaintWord(const tWord aWord, const CRGB aColor)
{
    /* Check input parameters */
    if ((aWord > WORD_END_OF_WORDS) &&
        (aWord < WORD_MAX_NUMBER))
    {
        /* Get word data */
        tWordData wWordData = mcWordDataArray[aWord];

        /* Paint word as a line */
        PaintLine(wWordData.mRow, wWordData.mColumn, wWordData.mLength, aColor);
    }
}

void Display::PaintTime(const uint8_t aHour, const uint8_t aMinute, const uint8_t aSecond)
{
    tWord wWords[WORD_MAX_NUMBER];

    uint8_t wHour   = aHour;
    uint8_t wMinute = aMinute;
    bool    wIsPM   = false;

    /* We have only 12 hours */
    if (wHour >= 12)
    {
        wHour -= /* HOUR_COUNT */ 12;
        wIsPM = true;
    }

    //tbl_minute  = &tbl_minutes[minute_mode][wMinute / 5];
}