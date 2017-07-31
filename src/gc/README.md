# gc

## Overview

This folder contains the implementation of Garbage Collection. Before go through the code, you should read https://github.com/dotnet/coreclr/blob/master/Documentation/botr/garbage-collection.md . That document will give you a high level design of GC.

## Main Process

Let's start with a simpliest console app:

```csharp
using System;

namespace gc
{
    class Program
    {
        static void Main(string[] args)
        {
        }
    }
}
```

Following is the main process of the GC when running this program:

[int st = initializeCoreCLR(); <br/> coreruncommon.cpp#413](src/coreclr/hosts/unixcoreruncommon/coreruncommon.cpp#413)
> Of course we should initialize the CoreCLR first.

         |
         |
         |
        \|/

[hr = host->Start(); <br/>
unixinterface.cpp#216](src/dlls/mscoree/unixinterface.cpp#216)
> Start the host. Because I'm debugging on Linux so we enter unixinterface.cpp.

         |
         |
         |
        \|/

[hr = CorRuntimeHostBase::Start(); <br/>
corhost.cpp#189](src/vm/corhost.cpp#189)

         |
         |
         |
        \|/

[hr = InitializeEE(COINITEE_DEFAULT); <br/>
corhost.cpp:242](src/vm/corhost.cpp:242)

         |
         |
         |
        \|/

[EEStartup(flags); <br/>
ceemain.cpp:366](src/vm/ceemain.cpp:366)

         |
         |
         |
        \|/

[PAL_EXCEPT_FILTER (FilterStartupException);<br/>
ceemain.cpp:1219](src/vm/ceemain.cpp:1219)

         |
         |
         |
        \|/

[EEStartupHelper(*pfFlags);<br/>
ceemain.cpp:1217](src/vm/ceemain.cpp:1217)

         |
         |
         |
        \|/

[SystemDomain::System()->Init();<br/>
ceemain.cpp:1035](src/vm/ceemain.cpp:1035)

         |
         |
         |
        \|/

[LoadBaseSystemClasses();<br/>
appdomain.cpp:2453](src/vm/appdomain.cpp:2453)

         |
         |
         |
        \|/

[MscorlibBinder::GetModule()->AllocateRegularStaticHandles(DefaultDomain());<br/>
appdomain.cpp:2767](src/vm/appdomain.cpp:2767)

         |
         |
         |
        \|/

[pDomain->AllocateStaticFieldObjRefPtrs();<br/>
ceeload.cpp:3050](src/vm/ceeload.cpp:3050)

         |
         |
         |
        \|/

[OBJECTREF* result = m_pLargeHeapHandleTable->AllocateHandles(nRequested, bCrossAD);<br/>
appdomain.cpp:1282](src/vm/appdomain.cpp:1282)

         |
         |
         |
        \|/

[m_pHead = new LargeHeapHandleBucket(m_pHead, NewBucketSize, m_pDomain, bCrossAD);<br/>
appdomain.cpp:551](src/vm/appdomain.cpp:551)

         |
         |
         |
        \|/

[HandleArrayObj = (PTRARRAYREF)AllocateObjectArray(Size, g_pObjectClass, TRUE);<br/>
appdomain.cpp:220](src/vm/appdomain.cpp:220)

         |
         |
         |
        \|/

[return AllocateArrayEx();<br/>
gchelpers.cpp:997](src/vm/gchelpers.cpp:997)

         |
         |
         |
        \|/

[return AllocateArrayEx();<br/>
gchelpers.cpp:455](src/vm/gchelpers.cpp:455)

         |
         |
         |
        \|/

[orArray = (ArrayBase *) AllocLHeap(totalSize, FALSE, pArrayMT->ContainsPointers());<br/>
gchelpers.cpp:581](src/vm/gchelpers.cpp:581)

         |
         |
         |
        \|/

[retVal = GCHeapUtilities::GetGCHeap()->AllocLHeap(size, flags);<br/>
gchelpers.cpp:334](src/vm/gchelpers.cpp:334)

         |
         |
         |
        \|/

[newAlloc = (Object*) hp->allocate_large_object (size + ComputeMaxStructAlignPadLarge(requiredAlignment), acontext->alloc_bytes_loh);<br/>
gc.cpp:34311](src/gc/gc.cpp:34311)

         |
         |
         |
        \|/

[if (! allocate_more_space (&acontext, (size + pad), max_generation+1))<br/>
gc.cpp:30600](src/gc/gc.cpp:30600)

         |
         |
         |
        \|/

[status = try_allocate_more_space (acontext, size, alloc_generation_number);<br/>
gc.cpp:13465](src/gc/gc.cpp:13465)

         |
         |
         |
        \|/

[BOOL can_allocate = ((gen_number == 0) ?
        allocate_small (gen_number, size, acontext, align_const) :
        allocate_large (gen_number, size, acontext, align_const));;<br/>
gc.cpp:13178](src/gc/gc.cpp:13178)