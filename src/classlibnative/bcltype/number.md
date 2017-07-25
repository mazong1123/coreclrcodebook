# number

[number.h](https://github.com/dotnet/coreclr/blob/master/src/classlibnative/bcltype/number.h)

[number.cpp](https://github.com/dotnet/coreclr/blob/master/src/classlibnative/bcltype/number.cpp)

## Overview

Defines number formatting and converting functions. Such as `FormatDecimal`, `FormatDouble` and `NumberBufferToDouble` are all defined here.

There're 2 class/struct defined here:

```cpp
struct NUMBER {
    int precision;
    int scale;
    int sign;
    wchar_t digits[NUMBER_MAXDIGITS + 1];
    wchar_t* allDigits;
    NUMBER() : precision(0), scale(0), sign(0), allDigits(NULL) {}
};
```
This is an abstract data structure to represent a number. Especially useful for representing `double`. To understand this data structure, you need to read [IEEE Double-precision floating-point format](https://en.wikipedia.org/wiki/Double-precision_floating-point_format)

```cpp
class COMNumber
{
public:
    static FCDECL3_VII(Object*, FormatDecimal, FC_DECIMAL value, StringObject* formatUNSAFE, NumberFormatInfo* numfmtUNSAFE);
    static FCDECL3_VII(Object*, FormatDouble,  double  value, StringObject* formatUNSAFE, NumberFormatInfo* numfmtUNSAFE);
    static FCDECL3_VII(Object*, FormatSingle,  float   value, StringObject* formatUNSAFE, NumberFormatInfo* numfmtUNSAFE);
    static FCDECL3(Object*, FormatInt32,   INT32      value, StringObject* formatUNSAFE, NumberFormatInfo* numfmtUNSAFE);
    static FCDECL3(Object*, FormatUInt32,  UINT32     value, StringObject* formatUNSAFE, NumberFormatInfo* numfmtUNSAFE);
    static FCDECL3_VII(Object*, FormatInt64,   INT64  value, StringObject* formatUNSAFE, NumberFormatInfo* numfmtUNSAFE);
    static FCDECL3_VII(Object*, FormatUInt64,  UINT64 value, StringObject* formatUNSAFE, NumberFormatInfo* numfmtUNSAFE);
    static FCDECL2(FC_BOOL_RET, NumberBufferToDecimal, BYTE* number, DECIMAL* value);
    static FCDECL2(FC_BOOL_RET, NumberBufferToDouble, BYTE* number, double* value);
    
    static wchar_t* Int32ToDecChars(__in wchar_t* p, unsigned int value, int digits);
};
```

`COMNumber` defines common formatting and converting functions. You'll find there're macros like `FCDECL3_VII`, `FCDECL2` and `FCDECL3`. They declare these functions are `FCall`s. `Fcall` allows you to call DIRECTLY into the CLR code from managed code.

More details about `FCall` can be found at [fcall.md](../../vm/fcall.md)