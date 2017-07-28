// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
//
// File: Number.h
//

//

// Define number formatting and converting functions.

#ifndef _NUMBER_H_
#define _NUMBER_H_

#include <pshpack1.h>

#define NUMBER_MAXDIGITS 50

// This is an abstract data structure to represent a number.
// Especially useful for representing `double`. To understand this data structure,
// you need to read https://en.wikipedia.org/wiki/Double-precision_floating-point_format
struct NUMBER {
    int precision;
    int scale;
    int sign;
    wchar_t digits[NUMBER_MAXDIGITS + 1];
    wchar_t* allDigits;
    NUMBER() : precision(0), scale(0), sign(0), allDigits(NULL) {}
};

// Defines common formatting and converting functions. You'll find there're macros like FCDECL3_VII, FCDECL2 and FCDECL3.
// They declare these functions are "FCall"s. "Fcall" allows you to call DIRECTLY into the CLR code from managed code.
//
// More details about `FCall` can be found at https://github.com/mazong1123/coreclrcodebook/tree/master/src/vm/README.md#fcall
//
// Note: "FCall" functions obey certain rules. We do not bother the rules here. Instead we focus on the functionality of each function.
class COMNumber
{
public:
    static FCDECL3_VII(Object*, FormatDecimal, FC_DECIMAL value, StringObject* formatUNSAFE, NumberFormatInfo* numfmtUNSAFE);
    
    // This function trying to convert a given double number to a string with a given format.
    // In managed code, it's called on [double.ToString()](https://github.com/mazong1123/coreclrcodebook/blob/master/src/mscorlib/shared/System/Double.cs#L250)
    // Since it's a `FCall`, here's a corresponding [managed code interface](https://github.com/mazong1123/coreclrcodebook/blob/master/src/mscorlib/src/System/Number.cs#L297-L298).
    // The most important part of this function is to choose the right precision and call "DoubleToNumber"" to convert the input double value. It's all replying on the value of "formatUNSAFE"
    static FCDECL3_VII(Object*, FormatDouble,  double  value, StringObject* formatUNSAFE, NumberFormatInfo* numfmtUNSAFE);
    static FCDECL3_VII(Object*, FormatSingle,  float   value, StringObject* formatUNSAFE, NumberFormatInfo* numfmtUNSAFE);
    static FCDECL3(Object*, FormatInt32,   INT32      value, StringObject* formatUNSAFE, NumberFormatInfo* numfmtUNSAFE);
    static FCDECL3(Object*, FormatUInt32,  UINT32     value, StringObject* formatUNSAFE, NumberFormatInfo* numfmtUNSAFE);
    static FCDECL3_VII(Object*, FormatInt64,   INT64  value, StringObject* formatUNSAFE, NumberFormatInfo* numfmtUNSAFE);
    static FCDECL3_VII(Object*, FormatUInt64,  UINT64 value, StringObject* formatUNSAFE, NumberFormatInfo* numfmtUNSAFE);
    static FCDECL2(FC_BOOL_RET, NumberBufferToDecimal, BYTE* number, DECIMAL* value);
    
    // This function trying to convert a number (BYTE*) back to double.
    // In managed code, it's called on [Number.ParseDouble()](https://github.com/mazong1123/coreclrcodebook/blob/master/src/mscorlib/src/System/Number.cs#L716)
    // Note that in managed code, the input string was converted to `NumberBuffer` and then converted to byte* via `NumberBuffer.PackForNative()`.
    // Since it's a `FCall`, here's a corresponding [managed code interface](https://github.com/mazong1123/coreclrcodebook/blob/master/src/mscorlib/src/System/Number.cs#L312-L313).
    static FCDECL2(FC_BOOL_RET, NumberBufferToDouble, BYTE* number, double* value);

    static wchar_t* Int32ToDecChars(__in wchar_t* p, unsigned int value, int digits);
};

#include <poppack.h>

#endif // _NUMBER_H_
