# fcall

[fcall.h](https://github.com/dotnet/coreclr/blob/master/src/vm/fcall.h)

[fcall.cpp](https://github.com/dotnet/coreclr/blob/master/src/vm/fcall.cpp)

## Overview

There's a [good document](https://github.com/dotnet/coreclr/blob/master/Documentation/botr/mscorlib.md#calling-from-managed-to-native-code) describing what is `FCall`, as well as `QCall`, `HCall` and `P/Invoke`.

Generally speaking, `FCall` allows you to call directly into the CLR code from managed code without leveraging `P/Invoke`.