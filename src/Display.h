/*
 * Display.h
 *
 *  Created on: 02.09.2025
 *      Author: hocki
 */

#include <FastLED.h>


/***************************************************************************************************
  LED matrix configuration
 **************************************************************************************************/
#define MATRIX_WIDTH                16
#define MATRIX_HEIGHT               16
#define MATRIX_SIZE                 MATRIX_WIDTH * MATRIX_HEIGHT
//#define MATRIX_TYPE                 HORIZONTAL_ZIGZAG_MATRIX

/***************************************************************************************************
  FastLED configuration
 **************************************************************************************************/
/* The LED stripe type                      */
#define LED_TYPE                    WS2812
/* The color order or the LED stripe        */
#define LED_COLOR_ORDER             GRB
/* Digital LED stripe connection pin - D32  */
#define LED_DATA_PIN                32
/* Default LEDs brightness                  */
#define LED_DEFAULT_BRIGHTNESS      255
/* Numbers of LEDs                          */
#define LED_NUMBER                  MATRIX_SIZE

/***************************************************************************************************
  WordClock configuration
 **************************************************************************************************/

static const char* mDisplayLayout[MATRIX_HEIGHT]=
{
    /*         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 */
	  /* 00 */ " A  L  A  R  M  G  E  B  U  R  T  S  T  A  G  W  "    // Alarm Geburtstag W
    /* 01 */ " M  Ü  L  L  A  U  T  O  F  E  I  E  R  T  A  G  "    // Müll Auto Feiertag
    /* 02 */ " A  F  O  R  M  E  L  1  D  O  W  N  L  O  A  D  "    // A Formel1 Download
    /* 03 */ " W  L  A  N  U  P  D  A  T  E  R  A  U  S  E  S  "    // Wlan Update Raus Es
    /* 04 */ " B  R  I  N  G  E  N  I  S  T  G  E  L  B  E  R  "    // Bringen Ist Gelber
    /* 05 */ " S  A  C  K  Z  E  I  T  Z  W  A  N  Z  I  G  F  "    // Sack Zeit Zwanzig F
    /* 06 */ " H  A  L  B  G  U  R  L  A  U  B  G  E  N  A  U  "    // Halb G Urlaub Genau
    /* 07 */ " Z  E  H  N  W  E  R  K  S  T  A  T  T  Z  U  M  "    // Zehn Werkstatt Zum
    /* 08 */ " F  Ü  N  F  R  I  S  E  U  R  Z  O  C  K  E  N  "    // Fünf riseur Zocken
    /* 09 */ " W  O  R  D  C  L  O  C  K  V  I  E  R  T  E  L  "    // Wordclock Viertel
    /* 10 */ " V  O  R  N  E  U  S  T  A  R  T  E  R  M  I  N  "    // Vor Neustar Termin
    /* 11 */ " N  A  C  H  L  H  A  L  B  V  S  I  E  B  E  N  "    // Nach L Halb V Sieben
    /* 12 */ " S  E  C  H  S  N  E  U  N  Z  E  H  N  E  L  F  "    // Sechs Neun Zehn Elf
    /* 13 */ " E  I  N  S  D  R  E  I  V  I  E  R  Z  W  E  I  "    // Eins Drei Vier Zwei
    /* 14 */ " A  C  H  T  Z  W  Ö  L  F  Ü  N  F  U  U  H  R  "    // Acht Zwölf ünf U Uhr
    /* 15 */ " S  +  1  2  3  4  O  K  M  I  N  U  T  E  N  W  "    // S + 1 2 3 4 OK Minuten W
};

/*
  Clock words:
    MM   0 :    Es ist Genau HH
    MM   5 :    Es ist Fünf nach HH
    MM  10 :    Es ist Zehn nach HH
    MM  15 :    Es ist Viertel nach HH
    MM  20 :    (WESSI Mode) Es ist Zwanzig nach HH   / (RHEIN-RUHR Mode) Es ist Zehn vor halb HH
    MM  25 :    Es ist Fünf vor halb HH
    MM  30 :    Es ist Halb HH
    MM  35 :    Es ist Fünf nach halb HH
    MM  40 :    (WESSI Mode) Es ist Zwanzig vor HH   / (RHEIN-RUHR Mode) Es ist Zehn nach halb HH
    MM  45 :    Es ist Viertel vor HH
    MM  50 :    Es ist Zehn vor HH
    MM  55 :    Es ist Fünf vor HH

    ( + 1 / 2 / 3 / 4 Minuten )

    HH: Ein / Zwei / Drei / Vier / Fünf / Sechs / Sieben / Acht / Neun / Zehn / Elf / Zwölf ( Uhr )
*/

typedef /* class*/ enum tWord /* : uint8_t */
{
    WORD_END_OF_WORDS,

    /* WordClock: minutes words */
    WORD_CLOCK_MIN_5,   // Fünf
    WORD_CLOCK_MIN_10,  // Zehn
    WORD_CLOCK_MIN_20,  // Zwanzig
    WORD_CLOCK_MIN_30,  // Halb
    /* WordClock: hours words */
    WORD_CLOCK_HOUR_1,  // Eins
    WORD_CLOCK_HOUR_2,  // Zwei
    WORD_CLOCK_HOUR_3,  // Drei
    WORD_CLOCK_HOUR_4,  // Vier
    WORD_CLOCK_HOUR_5,  // Fünf
    WORD_CLOCK_HOUR_6,  // Sechs
    WORD_CLOCK_HOUR_7,  // Sieben
    WORD_CLOCK_HOUR_8,  // Acht
    WORD_CLOCK_HOUR_9,  // Neun
    WORD_CLOCK_HOUR_10, // Zehn
    WORD_CLOCK_HOUR_11, // Elf
    WORD_CLOCK_HOUR_12, // Zwölf
    /* WordClock: special words */
    WORD_ES,
    WORD_IST,
    WORD_GENAU,
    WORD_VIERTEL,
    WORD_HALB,
    WORD_VOR,
    WORD_NACH,
    WORD_UHR,
    WORD_PLUS,
    WORD_NUM_1,
    WORD_NUM_2,
    WORD_NUM_3,
    WORD_NUM_4,
    WORD_MINUTE,
    WORD_MINUTEN,
    /* Additional words */
    WORD_ALARM,
    WORD_GEBURTSTAG,
    WORD_WLAN,
    WORD_MUELL,
    WORD_AUTO,
    WORD_FEIERTAG,
    WORD_FORMEL1,
    WORD_DOWNLOAD,
    WORD_UPDATE,
    WORD_RAUS,
    WORD_BRINGEN,
    WORD_GELBER,
    WORD_SACK,
    //
    WORD_MAX_NUMBER
} tWord;


class Display
{
public:
    Display();
    virtual ~Display();

    void Init(void);

    void SetTime(const uint8_t aHour, const uint8_t aMinute, const uint8_t aSecond);

    void Update(void);


private:

    typedef struct tWordData
    {
        /* Row index [0..MATRIX_HEIGHT-1] */
        uint8_t mRow;
        /* Column index [0..MATRIX_WIDTH-1] */
        uint8_t mColumn;
        /* Word length */
        uint8_t mLength;
    } tWordData;

  const tWordData mcWordDataArray[WORD_MAX_NUMBER] =
  {
      /* WORD_END_OF_WORDS  */   {  0,  0,  0 },    // !!! End of words marker !!!

      /* WORD_CLOCK_MIN_5   */   {  8,  0,  4 },    // Fünf
      /* WORD_CLOCK_MIN_10  */   {  7,  0,  4 },    // Zehn
      /* WORD_CLOCK_MIN_20  */   {  5,  0,  6 },    // Zwanzig
      /* WORD_CLOCK_MIN_30  */   {  6,  0,  4 },    // Halb
      /* WORD_CLOCK_HOUR_1  */   { 13,  0,  4 },    // Eins
      /* WORD_CLOCK_HOUR_2  */   { 13, 12,  4 },    // Zwei
      /* WORD_CLOCK_HOUR_3  */   { 13,  4,  4 },    // Drei
      /* WORD_CLOCK_HOUR_4  */   { 13,  8,  4 },    // Vier
      /* WORD_CLOCK_HOUR_5  */   { 14,  8,  4 },    // Fünf
      /* WORD_CLOCK_HOUR_6  */   { 12,  0,  5 },    // Sechs
      /* WORD_CLOCK_HOUR_7  */   { 11, 12,  6 },    // Sieben
      /* WORD_CLOCK_HOUR_8  */   { 14,  0,  4 },    // Acht
      /* WORD_CLOCK_HOUR_9  */   { 12,  5,  4 },    // Neun
      /* WORD_CLOCK_HOUR_10 */   { 12, 10,  4 },    // Zehn
      /* WORD_CLOCK_HOUR_11 */   { 12, 15,  3 },    // Elf
      /* WORD_CLOCK_HOUR_12 */   { 14,  4,  5 },    // Zwölf
      /* WORD_ES            */   {  0,  0,  2 },    // Es
      /* WORD_IST           */   {  1,  0,  3 },    // Ist
      /* WORD_GENAU         */   {  6, 10,  5 },    // Genau
      /* WORD_VIERTEL       */   {  9,  5,  7 },    // Viertel
      /* WORD_HALB          */   {  6,  0,  4 },    // Halb
      /* WORD_VOR           */   { 11,  0,  3 },    // Vor
      /* WORD_NACH          */   {  7, 11,  4 },    // Nach
      /* WORD_UHR           */   { 14, 13,  3 },    // Uhr
      /* WORD_PLUS          */   { 15,  1,  1 },    // +
      /* WORD_NUM_1         */   { 15,  3,  1 },    // 1
      /* WORD_NUM_2         */   { 15,  5,  1 },    // 2
      /* WORD_NUM_3         */   { 15,  7,  1 },    // 3
      /* WORD_NUM_4         */   { 15,  9,  1 },    // 4
      /* WORD_MINUTE        */   { 15, 11,  6 },    // Minute
      /* WORD_MINUTEN       */   { 15, 11,  7 },    // Minuten
      /* WORD_ALARM         */   {  0,  0,  5 },    // Alarm
      /* WORD_GEBURTSTAG    */   {  0,  5, 10 },    // Geburtstag
      /* WORD_WLAN          */   {  3,  0,  4 },    // Wlan
      /* WORD_MUELL         */   {  1,  0,  4 },    // Müll
      /* WORD_AUTO          */   {  1,  5,  4 },    // Auto
      /* WORD_FEIERTAG      */   {  1, 10,  8 },    // Feiertag
      /* WORD_FORMEL1       */   {  2,  1,  7 },    // Formel1
      /* WORD_DOWNLOAD      */   {  2,  8,  8 },    // Download
      /* WORD_UPDATE        */   {  3,  5,  6 },    // Update
      /* WORD_RAUS          */   {  3, 12,  4 },    // Raus
      /* WORD_BRINGEN       */   {  4,  0,  7 },    // Bringen
      /* WORD_GELBER        */   {  4, 11,  6 },    // Gelber
      /* WORD_SACK          */   {  5,  0,  4 }     // Sack
  };




    /* Leds */
    CRGB mLeds[LED_NUMBER];

    uint32_t mPrevMillis = 0;

    void Clear(void);
    void Transform(void);

    void SetColor(const uint16_t aLedIndex, const CRGB aColor);

    void PaintPixel(const uint16_t aRow, const uint16_t aCol, const CRGB aColor);
    void PaintLine(const uint16_t aRow, const uint16_t aCol, const uint16_t aLength, const CRGB aColor);
    void PaintArea(const uint16_t aRow, const uint16_t aCol, const uint16_t aWidth, const uint16_t aHeight, const CRGB aColor);

    void PaintWord(const tWord aWord, const CRGB aColor);
    void PaintTime(const uint8_t aHour, const uint8_t aMinute, const uint8_t aSecond);
};