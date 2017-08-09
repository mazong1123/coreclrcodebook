# vm

## Overview

vm may stand for "Virtual Machine".

## FCall
There's a [good document](https://github.com/dotnet/coreclr/blob/master/Documentation/botr/mscorlib.md#calling-from-managed-to-native-code) describing what is `FCall`, as well as `QCall`, `HCall` and `P/Invoke`.

Generally speaking, `FCall` allows you to call directly into the CLR code from managed code without leveraging `P/Invoke`.


## Type System

High level documents:

https://github.com/dotnet/coreclr/blob/master/Documentation/botr/type-system.md

https://github.com/dotnet/coreclr/blob/master/Documentation/botr/type-loader.md

https://github.com/dotnet/coreclr/blob/master/Documentation/botr/method-descriptor.md

### How to iterate fields of an object

Given an instance of Object type, to iterate its fields we need to do following as prerequisites:

- Get the method table of the object. This can be done via `object->GetMethodTable()`.

- Construct a `ApproxFieldDescIterator`: 
    ```cpp
    ApproxFieldDescIterator(methodTable, ApproxFieldDescIterator::INSTANCE_FIELDS);
    ```

    You can specify which kind of fields to iterate via the second paramter. Currently support field type:

    ```cpp
    enum IteratorType {
        INSTANCE_FIELDS = 0x1,
        STATIC_FIELDS   = 0x2,
        ALL_FIELDS      = (INSTANCE_FIELDS | STATIC_FIELDS)
    };
    ```

    For full definition of `ApproxFieldDescIterator`, please see [vm/class.h](classh.h). 

- Iterate the fields:

    ```cpp
    FieldDesc* pField = iter.Next();
    while (pField != NULL)
    {
        // Do anything you want...

        pField = iter.Next();
    }
    ```

- Full example:

    ```cpp
    MethodTable* mt = object->GetMethodTable();
    ApproxFieldDescIterator(mt, ApproxFieldDescIterator::INSTANCE_FIELDS);
    FieldDesc* pField = iter.Next();
    while (pField != NULL)
    {
        // Do anything you want...

        pField = iter.Next();
    }
    ```