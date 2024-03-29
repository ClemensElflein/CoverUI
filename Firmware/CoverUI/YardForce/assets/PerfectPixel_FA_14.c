/*******************************************************************************
 * Size: 14 px
 * Bpp: 1
 * Opts: 
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef PERFECTPIXEL_FA_14
#define PERFECTPIXEL_FA_14 1
#endif

#if PERFECTPIXEL_FA_14

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xfc, 0x3c,

    /* U+0022 "\"" */
    0xde, 0xf6,

    /* U+0023 "#" */
    0x31, 0x86, 0x33, 0xff, 0xff, 0xf3, 0x18, 0x63,
    0xc, 0x67, 0xff, 0xff, 0xe6, 0x30, 0xc6, 0x0,

    /* U+0025 "%" */
    0x63, 0x63, 0x7, 0xf, 0x1e, 0x3c, 0x38, 0x70,
    0xe0, 0xc6, 0xc6,

    /* U+0026 "&" */
    0x7c, 0x7f, 0x31, 0x98, 0xf, 0x7f, 0xbf, 0xd,
    0x86, 0xc3, 0x7f, 0x9f, 0x80,

    /* U+0027 "'" */
    0xfc,

    /* U+0028 "(" */
    0x7f, 0xcc, 0xcc, 0xcc, 0xcc, 0xcf, 0x70,

    /* U+0029 ")" */
    0xef, 0x33, 0x33, 0x33, 0x33, 0x3f, 0xe0,

    /* U+002A "*" */
    0xff, 0xb0,

    /* U+002B "+" */
    0x18, 0x18, 0xff, 0xff, 0x18, 0x18, 0x18,

    /* U+002C "," */
    0x6f, 0x0,

    /* U+002D "-" */
    0xff, 0xf0,

    /* U+002E "." */
    0xf0,

    /* U+002F "/" */
    0x3, 0x3, 0x7, 0xe, 0x1c, 0x38, 0x70, 0x60,
    0xc0, 0xc0, 0xc0,

    /* U+0030 "0" */
    0x7e, 0xff, 0xc7, 0xcf, 0xdf, 0xdb, 0xf3, 0xf3,
    0xe3, 0xff, 0x7e,

    /* U+0031 "1" */
    0xff, 0xff, 0xfc,

    /* U+0032 "2" */
    0xfe, 0xff, 0x3, 0x3, 0x7f, 0xfe, 0xc0, 0xc0,
    0xc0, 0xff, 0xff,

    /* U+0033 "3" */
    0xfe, 0xff, 0x3, 0x3, 0x7e, 0x7f, 0x3, 0x3,
    0x3, 0xff, 0xfe,

    /* U+0034 "4" */
    0x7, 0xf, 0x1f, 0x3b, 0x73, 0xe3, 0xff, 0xff,
    0x3, 0x3, 0x3,

    /* U+0035 "5" */
    0xfe, 0xfe, 0xc0, 0xc0, 0xfe, 0xff, 0x3, 0x3,
    0x3, 0xff, 0xfe,

    /* U+0036 "6" */
    0x7e, 0xfe, 0xc0, 0xc0, 0xfe, 0xff, 0xc3, 0xc3,
    0xc3, 0xff, 0x7e,

    /* U+0037 "7" */
    0xff, 0xff, 0xc1, 0xc1, 0xc3, 0xc1, 0xc0, 0xc0,
    0x60, 0x30, 0x18, 0xc, 0x0,

    /* U+0038 "8" */
    0x7e, 0xff, 0xc3, 0xc3, 0xff, 0xff, 0xc3, 0xc3,
    0xc3, 0xff, 0x7e,

    /* U+0039 "9" */
    0x7e, 0xff, 0xc3, 0xc3, 0xff, 0x7f, 0x3, 0x3,
    0x3, 0x7f, 0x7e,

    /* U+003A ":" */
    0xf0, 0xf,

    /* U+003B ";" */
    0x6c, 0x0, 0x1b, 0xc0,

    /* U+003C "<" */
    0xf, 0x1f, 0x38, 0x70, 0xe0, 0xc0, 0xe0, 0x70,
    0x38, 0x1f, 0xf,

    /* U+003D "=" */
    0xff, 0xc0, 0x0, 0x7f, 0xe0,

    /* U+003E ">" */
    0xf0, 0xf8, 0x3c, 0x1e, 0xf, 0x7, 0xf, 0x1e,
    0x3c, 0xf8, 0xf0,

    /* U+003F "?" */
    0x3f, 0x7f, 0xf8, 0x60, 0x30, 0xf8, 0xf8, 0x60,
    0x30, 0x0, 0xc, 0x6, 0x0,

    /* U+0040 "@" */
    0x7f, 0xbf, 0xfd, 0xff, 0xff, 0xf3, 0xfc, 0xff,
    0xff, 0x7f, 0xc3, 0xbf, 0xf7, 0xfc,

    /* U+0041 "A" */
    0x3f, 0x1f, 0xef, 0x3f, 0x87, 0xc0, 0xf0, 0x3f,
    0xff, 0xff, 0xc0, 0xf0, 0x3c, 0xc,

    /* U+0042 "B" */
    0xff, 0xbf, 0xfc, 0xf, 0x3, 0xff, 0xff, 0xec,
    0xf, 0x3, 0xc0, 0xff, 0xff, 0xf8,

    /* U+0043 "C" */
    0x7f, 0xff, 0xf0, 0x18, 0xc, 0x6, 0x3, 0x1,
    0x80, 0xc0, 0x7f, 0xdf, 0xe0,

    /* U+0044 "D" */
    0xff, 0x7f, 0xf0, 0x78, 0x3c, 0x1e, 0xf, 0x7,
    0x83, 0xc1, 0xff, 0xff, 0xc0,

    /* U+0045 "E" */
    0xff, 0xff, 0xf0, 0x18, 0xf, 0xff, 0xff, 0x1,
    0x80, 0xc0, 0x7f, 0xff, 0xe0,

    /* U+0046 "F" */
    0xff, 0xff, 0xf0, 0x18, 0xf, 0xff, 0xff, 0x1,
    0x80, 0xc0, 0x60, 0x30, 0x0,

    /* U+0047 "G" */
    0x7f, 0xff, 0xfc, 0x3, 0x0, 0xc7, 0xf1, 0xfc,
    0xf, 0x3, 0xe1, 0xff, 0xf7, 0xf8,

    /* U+0048 "H" */
    0xc1, 0xe0, 0xf0, 0x78, 0x3f, 0xff, 0xff, 0x7,
    0x83, 0xc1, 0xe0, 0xf0, 0x60,

    /* U+0049 "I" */
    0xff, 0xff, 0xfc,

    /* U+004A "J" */
    0x18, 0xc6, 0x31, 0x8c, 0x63, 0x1f, 0xfc,

    /* U+004B "K" */
    0xc7, 0x67, 0x37, 0x1f, 0xf, 0x87, 0xe3, 0xf9,
    0x8e, 0xc3, 0xe0, 0xf0, 0x60,

    /* U+004C "L" */
    0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xc0, 0xff, 0xff,

    /* U+004D "M" */
    0xc0, 0x3f, 0xf, 0xf9, 0xff, 0xff, 0xdf, 0xbc,
    0xf3, 0xc6, 0x3c, 0x3, 0xc0, 0x3c, 0x3, 0xc0,
    0x30,

    /* U+004E "N" */
    0xc0, 0x7c, 0xf, 0xc1, 0xfc, 0x3d, 0xc7, 0x9c,
    0xf1, 0xde, 0x1f, 0xc1, 0xf8, 0x1f, 0x1, 0x80,

    /* U+004F "O" */
    0x7f, 0xdf, 0xff, 0x3, 0xe0, 0x3c, 0x7, 0x80,
    0xf0, 0x1e, 0x3, 0xc0, 0x7f, 0xfd, 0xff, 0x0,

    /* U+0050 "P" */
    0xff, 0x7f, 0xf0, 0x78, 0x3c, 0x1f, 0xff, 0xfd,
    0x80, 0xc0, 0x60, 0x30, 0x0,

    /* U+0051 "Q" */
    0x7f, 0xbf, 0xfc, 0x1f, 0x3, 0xc0, 0xf0, 0x3c,
    0xf, 0x33, 0xed, 0xff, 0xf7, 0xf8, 0x30,

    /* U+0052 "R" */
    0xff, 0x7f, 0xf0, 0x78, 0x3c, 0x1f, 0xff, 0xff,
    0x83, 0xc1, 0xe0, 0xf0, 0x60,

    /* U+0053 "S" */
    0x7f, 0x7f, 0xb0, 0x18, 0xf, 0xf3, 0xfc, 0x6,
    0x3, 0x1, 0xff, 0xff, 0xc0,

    /* U+0054 "T" */
    0xff, 0xff, 0xf0, 0xc0, 0x30, 0xc, 0x3, 0x0,
    0xc0, 0x30, 0xc, 0x3, 0x0, 0xc0,

    /* U+0055 "U" */
    0xc1, 0xe0, 0xf0, 0x78, 0x3c, 0x1e, 0xf, 0x7,
    0x83, 0xc1, 0xff, 0xdf, 0xc0,

    /* U+0056 "V" */
    0xc1, 0xe0, 0xf0, 0x78, 0x3c, 0x1e, 0xf, 0x7,
    0xc7, 0x77, 0x1f, 0x7, 0x0,

    /* U+0057 "W" */
    0xc0, 0x78, 0xf, 0x1, 0xe0, 0x3c, 0x47, 0x9c,
    0xf7, 0xdf, 0xdf, 0xf1, 0xfc, 0x1f, 0x1, 0x80,

    /* U+0058 "X" */
    0xc0, 0x7c, 0x1d, 0xc7, 0x1d, 0xc1, 0xf0, 0x1c,
    0x7, 0xc1, 0xdc, 0x71, 0xdc, 0x1f, 0x1, 0x80,

    /* U+0059 "Y" */
    0xc0, 0x7c, 0x1d, 0xc7, 0x1f, 0xc1, 0xf0, 0x3c,
    0x3, 0x0, 0x60, 0xc, 0x1, 0x80, 0x30, 0x0,

    /* U+005A "Z" */
    0xff, 0xff, 0xc0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0,
    0xe0, 0xe0, 0x7f, 0xff, 0xe0,

    /* U+005B "[" */
    0xff, 0xf1, 0x8c, 0x63, 0x18, 0xc6, 0x31, 0x8f,
    0xfc,

    /* U+005C "\\" */
    0xc0, 0xc0, 0xe0, 0x70, 0x38, 0x3c, 0x1e, 0xf,
    0x7, 0x3, 0x3,

    /* U+005D "]" */
    0xff, 0xc6, 0x31, 0x8c, 0x63, 0x18, 0xc6, 0x3f,
    0xfc,

    /* U+005E "^" */
    0x6, 0x0, 0xf0, 0x1f, 0x83, 0x9c, 0x70, 0xe0,

    /* U+005F "_" */
    0xff, 0xfc,

    /* U+0060 "`" */
    0xcc, 0x60,

    /* U+0061 "a" */
    0x3e, 0xfe, 0xc6, 0xc6, 0xc6, 0xc6, 0x7f, 0x3f,

    /* U+0062 "b" */
    0xc1, 0x83, 0x7, 0xef, 0xf8, 0xf1, 0xe3, 0xc7,
    0xff, 0xf0,

    /* U+0063 "c" */
    0x7f, 0xff, 0x6, 0xc, 0x18, 0x3f, 0xbf,

    /* U+0064 "d" */
    0x6, 0xc, 0x1b, 0xff, 0xf8, 0xf1, 0xe3, 0xc7,
    0xfd, 0xf8,

    /* U+0065 "e" */
    0x7d, 0xff, 0x1f, 0xff, 0xf8, 0x3f, 0x3e,

    /* U+0066 "f" */
    0x3d, 0xf6, 0x3e, 0xf9, 0x86, 0x18, 0x61, 0x86,
    0x0,

    /* U+0067 "g" */
    0x7f, 0xff, 0x1e, 0x3c, 0x78, 0xff, 0xbf, 0x6,
    0xd, 0xfb, 0xe0,

    /* U+0068 "h" */
    0xc1, 0x83, 0x7, 0xef, 0xf8, 0xf1, 0xe3, 0xc7,
    0x8f, 0x18,

    /* U+0069 "i" */
    0xf3, 0xff, 0xfc,

    /* U+006A "j" */
    0x33, 0x3, 0x33, 0x33, 0x33, 0x33, 0xfe,

    /* U+006B "k" */
    0xc1, 0x83, 0x6, 0x7d, 0xdf, 0x3c, 0x78, 0xf9,
    0xbb, 0x38,

    /* U+006C "l" */
    0xff, 0xff, 0xfc,

    /* U+006D "m" */
    0x33, 0x1f, 0xec, 0xcf, 0x33, 0xcc, 0xf3, 0x3c,
    0xcf, 0x33,

    /* U+006E "n" */
    0x7d, 0xff, 0x1e, 0x3c, 0x78, 0xf1, 0xe3,

    /* U+006F "o" */
    0x7e, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0x7e,

    /* U+0070 "p" */
    0xfd, 0xff, 0x1e, 0x3c, 0x78, 0xff, 0xfe, 0xc1,
    0x83, 0x6, 0x0,

    /* U+0071 "q" */
    0x7f, 0xff, 0x1e, 0x3c, 0x78, 0xff, 0xbf, 0x6,
    0xc, 0x18, 0x30,

    /* U+0072 "r" */
    0x7f, 0xf1, 0x8c, 0x63, 0x18,

    /* U+0073 "s" */
    0x7f, 0xff, 0x7, 0xe7, 0xe0, 0xff, 0xfe,

    /* U+0074 "t" */
    0x61, 0x8f, 0xbe, 0x61, 0x86, 0x18, 0x7c, 0xf0,

    /* U+0075 "u" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x78, 0xdf, 0x1c,

    /* U+0076 "v" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x7d, 0xdf, 0x1c,

    /* U+0077 "w" */
    0xcc, 0xf3, 0x3c, 0xcf, 0x33, 0xcc, 0xf3, 0x37,
    0xf8, 0xcc,

    /* U+0078 "x" */
    0xc3, 0xe7, 0x7e, 0x3c, 0x3c, 0x7e, 0xe7, 0xc3,

    /* U+0079 "y" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x78, 0xff, 0xbf, 0x6,
    0xd, 0xfb, 0xe0,

    /* U+007A "z" */
    0xff, 0xfc, 0x30, 0xc3, 0x1c, 0x3f, 0xff,

    /* U+007B "{" */
    0x19, 0xd8, 0xc6, 0x31, 0x8c, 0x63, 0x18, 0xc3,
    0x8c,

    /* U+007C "|" */
    0xff, 0xff, 0xff,

    /* U+007D "}" */
    0xe3, 0xc1, 0x86, 0x18, 0x61, 0x86, 0x18, 0x61,
    0x8e, 0xf3, 0x80,

    /* U+007E "~" */
    0x20, 0xfb, 0xf4, 0xc0, 0x0,

    /* U+2022 "•" */
    0x6f, 0xf6,

    /* U+F06A "" */
    0xf, 0xc0, 0x7f, 0x83, 0xff, 0x1f, 0x3e, 0xfc,
    0xff, 0xf3, 0xff, 0xcf, 0xff, 0xff, 0xfc, 0xff,
    0xf3, 0xf7, 0xcf, 0x8f, 0xfc, 0x1f, 0xe0, 0x3f,
    0x0,

    /* U+F071 "" */
    0x1, 0x80, 0x3, 0xc0, 0x3, 0xc0, 0x7, 0xe0,
    0x7, 0xe0, 0xe, 0x70, 0x1e, 0x70, 0x1e, 0x78,
    0x3e, 0x7c, 0x3f, 0xfc, 0x7e, 0x7e, 0xfe, 0x7e,
    0xff, 0xff, 0xff, 0xfe,

    /* U+F0E7 "" */
    0x7c, 0x3e, 0x1f, 0x1f, 0xf, 0xf7, 0xff, 0xfd,
    0xfc, 0xe, 0x6, 0x7, 0x3, 0x1, 0x80, 0x80,

    /* U+F0E9 "" */
    0x1, 0x80, 0x3, 0xc0, 0xf, 0xf0, 0x3f, 0xfc,
    0x7f, 0xfe, 0x7f, 0xfe, 0xff, 0xff, 0x8c, 0x31,
    0x5, 0xa0, 0x1, 0x80, 0x1, 0x80, 0x5, 0x80,
    0x7, 0x80, 0x7, 0x0,

    /* U+F124 "" */
    0x0, 0x1c, 0x1, 0xf0, 0x3f, 0x83, 0xfe, 0x3f,
    0xfb, 0xff, 0xcf, 0xff, 0x1f, 0xf8, 0x3, 0xe0,
    0xf, 0x0, 0x3c, 0x0, 0xe0, 0x3, 0x80, 0x4,
    0x0,

    /* U+F13D "" */
    0x3, 0xc0, 0x7, 0xe0, 0x6, 0x60, 0x7, 0xe0,
    0x3, 0xc0, 0x7, 0xe0, 0x7, 0xe0, 0x41, 0x82,
    0xe1, 0x87, 0xf1, 0x8f, 0x61, 0x86, 0x39, 0x9c,
    0x1f, 0xf8, 0x7, 0xe0,

    /* U+F1E6 "" */
    0x31, 0x86, 0x30, 0xc6, 0x18, 0xcf, 0xff, 0xff,
    0xdf, 0xf3, 0xfe, 0x7f, 0xc7, 0xf0, 0x7c, 0x2,
    0x0, 0x40, 0x8, 0x0,

    /* U+F21E "" */
    0x3c, 0xf1, 0xff, 0xef, 0xff, 0xff, 0xff, 0xf3,
    0x7d, 0xec, 0xe0, 0xe8, 0xf, 0x3c, 0x1f, 0xe0,
    0x3f, 0x0, 0x78, 0x0, 0xc0,

    /* U+F240 "" */
    0xff, 0xff, 0xbf, 0xff, 0xec, 0x0, 0x1f, 0x7f,
    0xf7, 0xdf, 0xfc, 0xf7, 0xff, 0x7c, 0x0, 0x1f,
    0xff, 0xfe, 0xff, 0xff, 0x80,

    /* U+F241 "" */
    0xff, 0xff, 0xbf, 0xff, 0xec, 0x0, 0x1f, 0x7f,
    0xc7, 0xdf, 0xf0, 0xf7, 0xfc, 0x7c, 0x0, 0x1f,
    0xff, 0xfe, 0xff, 0xff, 0x80,

    /* U+F242 "" */
    0xff, 0xff, 0xbf, 0xff, 0xec, 0x0, 0x1f, 0x7e,
    0x7, 0xdf, 0x80, 0xf7, 0xe0, 0x7c, 0x0, 0x1f,
    0xff, 0xfe, 0xff, 0xff, 0x80,

    /* U+F243 "" */
    0xff, 0xff, 0xbf, 0xff, 0xec, 0x0, 0x1f, 0x70,
    0x7, 0xdc, 0x0, 0xf7, 0x0, 0x7c, 0x0, 0x1f,
    0xff, 0xfe, 0xff, 0xff, 0x80,

    /* U+F244 "" */
    0xff, 0xff, 0xbf, 0xff, 0xec, 0x0, 0x1f, 0x0,
    0x7, 0xc0, 0x0, 0xf0, 0x0, 0x7c, 0x0, 0x1f,
    0xff, 0xfe, 0xff, 0xff, 0x80,

    /* U+F5DC "" */
    0xc, 0x60, 0x3d, 0xe0, 0xfb, 0xe1, 0xf7, 0xc7,
    0xef, 0xcf, 0xdf, 0x9f, 0xbf, 0x7f, 0x7f, 0xfe,
    0xff, 0xfd, 0xfd, 0xfb, 0xf3, 0xf7, 0xe3, 0xef,
    0x81, 0x8c, 0x0,

    /* U+F5DF "" */
    0x38, 0x70, 0xe1, 0xc3, 0x87, 0x3f, 0xff, 0xff,
    0xdf, 0x1e, 0x3c, 0x78, 0xff, 0xf7, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xc0,

    /* U+F5E1 "" */
    0x10, 0x0, 0x6, 0x80, 0x1, 0xef, 0x1, 0xff,
    0xf8, 0x3d, 0x8f, 0x1d, 0xc0, 0x4f, 0x5e, 0x18,
    0xd3, 0xfe, 0x2f, 0xff, 0x83, 0xff, 0x30, 0xff,
    0xc8, 0x18, 0x7e, 0x0, 0x3, 0x0, 0x1, 0xc0,

    /* U+F5E7 "" */
    0x7f, 0x80, 0x7f, 0x86, 0x63, 0x86, 0x63, 0x8f,
    0x61, 0x8f, 0x63, 0x8f, 0x7b, 0x86, 0x77, 0xe6,
    0x7f, 0xb6, 0x7f, 0xb6, 0x7f, 0x94, 0x0, 0x1c,
    0xff, 0xc0, 0xff, 0xc0,

    /* U+F7C0 "" */
    0x7, 0x0, 0x1f, 0x0, 0xe, 0x1, 0xcc, 0x7,
    0x98, 0x7, 0x6c, 0xe, 0xf9, 0x9b, 0xf6, 0x6f,
    0xe0, 0xf, 0xc0, 0x1f, 0x80, 0x7f, 0x0, 0x7c,
    0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 114, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 48, .box_w = 2, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 96, .box_w = 5, .box_h = 3, .ofs_x = 1, .ofs_y = 8},
    {.bitmap_index = 6, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 22, .adv_w = 141, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 33, .adv_w = 163, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 46, .adv_w = 45, .box_w = 2, .box_h = 3, .ofs_x = 1, .ofs_y = 8},
    {.bitmap_index = 47, .adv_w = 76, .box_w = 4, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 54, .adv_w = 73, .box_w = 4, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 61, .adv_w = 74, .box_w = 3, .box_h = 4, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 63, .adv_w = 134, .box_w = 8, .box_h = 7, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 70, .adv_w = 66, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 72, .adv_w = 117, .box_w = 6, .box_h = 2, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 74, .adv_w = 48, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 75, .adv_w = 141, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 86, .adv_w = 152, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 97, .adv_w = 55, .box_w = 2, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 100, .adv_w = 148, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 111, .adv_w = 146, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 122, .adv_w = 146, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 133, .adv_w = 148, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 144, .adv_w = 148, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 155, .adv_w = 145, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 168, .adv_w = 148, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 179, .adv_w = 149, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 190, .adv_w = 42, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 192, .adv_w = 57, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 196, .adv_w = 128, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 207, .adv_w = 90, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 212, .adv_w = 129, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 223, .adv_w = 136, .box_w = 9, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 236, .adv_w = 178, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 250, .adv_w = 174, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 264, .adv_w = 172, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 278, .adv_w = 158, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 291, .adv_w = 170, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 304, .adv_w = 163, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 317, .adv_w = 156, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 330, .adv_w = 176, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 344, .adv_w = 173, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 357, .adv_w = 57, .box_w = 2, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 360, .adv_w = 92, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 367, .adv_w = 162, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 380, .adv_w = 144, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 391, .adv_w = 207, .box_w = 12, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 408, .adv_w = 195, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 424, .adv_w = 185, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 440, .adv_w = 159, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 453, .adv_w = 190, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 468, .adv_w = 159, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 481, .adv_w = 163, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 494, .adv_w = 170, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 508, .adv_w = 166, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 521, .adv_w = 161, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 534, .adv_w = 206, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 550, .adv_w = 186, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 566, .adv_w = 182, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 582, .adv_w = 160, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 595, .adv_w = 91, .box_w = 5, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 604, .adv_w = 141, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 615, .adv_w = 91, .box_w = 5, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 624, .adv_w = 189, .box_w = 12, .box_h = 5, .ofs_x = 0, .ofs_y = 7},
    {.bitmap_index = 632, .adv_w = 117, .box_w = 7, .box_h = 2, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 634, .adv_w = 67, .box_w = 4, .box_h = 3, .ofs_x = 1, .ofs_y = 8},
    {.bitmap_index = 636, .adv_w = 138, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 644, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 654, .adv_w = 123, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 661, .adv_w = 132, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 671, .adv_w = 122, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 678, .adv_w = 99, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 687, .adv_w = 127, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 698, .adv_w = 130, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 708, .adv_w = 50, .box_w = 2, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 711, .adv_w = 50, .box_w = 4, .box_h = 14, .ofs_x = -1, .ofs_y = -3},
    {.bitmap_index = 718, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 728, .adv_w = 51, .box_w = 2, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 731, .adv_w = 185, .box_w = 10, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 741, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 748, .adv_w = 142, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 756, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 767, .adv_w = 130, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 778, .adv_w = 89, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 783, .adv_w = 125, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 790, .adv_w = 104, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 798, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 805, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 812, .adv_w = 185, .box_w = 10, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 822, .adv_w = 133, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 830, .adv_w = 129, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 841, .adv_w = 123, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 848, .adv_w = 89, .box_w = 5, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 857, .adv_w = 57, .box_w = 2, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 860, .adv_w = 89, .box_w = 6, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 871, .adv_w = 126, .box_w = 7, .box_h = 5, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 876, .adv_w = 76, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 878, .adv_w = 224, .box_w = 14, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 903, .adv_w = 252, .box_w = 16, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 931, .adv_w = 140, .box_w = 9, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 947, .adv_w = 252, .box_w = 16, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 975, .adv_w = 224, .box_w = 14, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1000, .adv_w = 252, .box_w = 16, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1028, .adv_w = 168, .box_w = 11, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1048, .adv_w = 224, .box_w = 14, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1069, .adv_w = 280, .box_w = 18, .box_h = 9, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1090, .adv_w = 280, .box_w = 18, .box_h = 9, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1111, .adv_w = 280, .box_w = 18, .box_h = 9, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1132, .adv_w = 280, .box_w = 18, .box_h = 9, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1153, .adv_w = 280, .box_w = 18, .box_h = 9, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1174, .adv_w = 252, .box_w = 15, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1201, .adv_w = 224, .box_w = 14, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1221, .adv_w = 280, .box_w = 18, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1253, .adv_w = 252, .box_w = 16, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1281, .adv_w = 224, .box_w = 14, .box_h = 14, .ofs_x = 0, .ofs_y = -2}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_2[] = {
    0x0, 0xd048, 0xd04f, 0xd0c5, 0xd0c7, 0xd102, 0xd11b, 0xd1c4,
    0xd1fc, 0xd21e, 0xd21f, 0xd220, 0xd221, 0xd222, 0xd5ba, 0xd5bd,
    0xd5bf, 0xd5c5, 0xd79e
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 4, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 37, .range_length = 90, .glyph_id_start = 5,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 8226, .range_length = 55199, .glyph_id_start = 95,
        .unicode_list = unicode_list_2, .glyph_id_ofs_list = NULL, .list_length = 19, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 3,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t PerfectPixel_FA_14 = {
#else
lv_font_t PerfectPixel_FA_14 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 17,          /*The maximum line height required by the font*/
    .base_line = 4,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if PERFECTPIXEL_FA_14*/

