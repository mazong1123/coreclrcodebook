# classlibnative

This folder contains the native implementation of base classes and functions in managed world. Something like string manipulation, floating point calculation, number conversion and etc.

These native functions are called by managed code via `FCall`.

For `FCall` please refer to [fcall](../../vm/README.md#fcall)

Note: `FCall` functions obey certain rules. We do not bother the rules in this module. Instead we focus on the functionality of each function.