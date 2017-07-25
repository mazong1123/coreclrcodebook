# number

[number.h](https://github.com/dotnet/coreclr/blob/master/src/classlibnative/bcltype/number.h)

[number.cpp](https://github.com/dotnet/coreclr/blob/master/src/classlibnative/bcltype/number.cpp)

## Overview

`numer.h` and `number.cpp` define number formatting and converting functions. Such as `FormatDecimal`, `FormatDouble` and `NumberBufferToDouble` are all defined here.

## Class/Methods/Functions

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

Note: `FCall` functions obey certain rules. We do not bother the rules here. Instead we focus on the functionality of each function.

#### FCIMPL3_VII(Object*, COMNumber::FormatDouble, double value, StringObject* formatUNSAFE, NumberFormatInfo* numfmtUNSAFE)


This function trying to convert a given double number to a string with a given format. 

In managed code, it's called on [double.ToString()](https://github.com/dotnet/coreclr/blob/e6865018d91fd257f05a42fe4fe353beb32c641a/src/mscorlib/shared/System/Double.cs#L249)

Since it's a `FCall`, here's a corresponding [managed code interface](https://github.com/dotnet/coreclr/blob/cdff8b0babe5d82737058ccdae8b14d8ae90160d/src/mscorlib/src/System/Number.cs#L297-L298).

The most important part of this function is to choose the right precision and call `DoubleToNumber` to convert the input double value. It's all replying on the value of `formatUNSAFE`:

`R`:
In order to give numbers that are both friendly to display and round-trippable,
We parse the number using 15 digits and then determine if it round trips to the same value.  If it does, we convert that NUMBER to a string, otherwise we reparse using 17 digits and display that.
