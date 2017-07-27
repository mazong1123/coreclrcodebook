# bcltype

## Overview

bcltype may stand for "Base Class Library Type". You'll find a set of low level functions here. Most of them have managed implementations. They are called by managed code via `FCall`.

For `FCall` please refer to [fcall](../../vm/README.md#fcall)

Note: `FCall` functions obey certain rules. We do not bother the rules in this module. Instead we focus on the functionality of each function.

### [number.h](number.h) [number.cpp](number.cpp) 
These two files defines all number converting and formatting functions. Each of the function has corresponding managed interface. So you can imaging that when you call `double.ToString()` in managed code, it ends up calling functions in these two files.

