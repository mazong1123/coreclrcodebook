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

[int st = initializeCoreCLR(); <br/> coreruncommon.cpp#L413](../../src/coreclr/hosts/unixcoreruncommon/coreruncommon.cpp#L413)

Of course we should initialize the CoreCLR first.

         |
         |
         |
        \|/

[hr = host->Start(); <br/>
unixinterface.cpp#L216](../../src/dlls/mscoree/unixinterface.cpp#L216)

Start the host. Because I'm debugging on Linux so we enter unixinterface.cpp.

         |
         |
         |
        \|/

[hr = CorRuntimeHostBase::Start(); <br/>
corhost.cpp#L189](../../src/vm/corhost.cpp#L189)

         |
         |
         |
        \|/

[hr = InitializeEE(COINITEE_DEFAULT); <br/>
corhost.cpp#L242](../../src/vm/corhost.cpp#L242)

         |
         |
         |
        \|/

[EEStartup(flags); <br/>
ceemain.cpp#L366](../../src/vm/ceemain.cpp#L366)

         |
         |
         |
        \|/

[PAL_EXCEPT_FILTER (FilterStartupException);<br/>
ceemain.cpp#L1219](../../src/vm/ceemain.cpp#L1219)

         |
         |
         |
        \|/

[EEStartupHelper(*pfFlags);<br/>
ceemain.cpp#L1217](../../src/vm/ceemain.cpp#L1217)

         |
         |
         |
        \|/

[SystemDomain::System()->Init();<br/>
ceemain.cpp#L1035](../../src/vm/ceemain.cpp#L1035)

         |
         |
         |
        \|/

[LoadBaseSystemClasses();<br/>
appdomain.cpp#L2453](../../src/vm/appdomain.cpp#L2453)

         |
         |
         |
        \|/

[MscorlibBinder::GetModule()->AllocateRegularStaticHandles(DefaultDomain());<br/>
appdomain.cpp#L2767](../../src/vm/appdomain.cpp#L2767)

         |
         |
         |
        \|/

[pDomain->AllocateStaticFieldObjRefPtrs();<br/>
ceeload.cpp#L3050](../../src/vm/ceeload.cpp#L3050)

         |
         |
         |
        \|/

[OBJECTREF* result = m_pLargeHeapHandleTable->AllocateHandles(nRequested, bCrossAD);<br/>
appdomain.cpp#L1282](../../src/vm/appdomain.cpp#L1282)

         |
         |
         |
        \|/

[m_pHead = new LargeHeapHandleBucket(m_pHead, NewBucketSize, m_pDomain, bCrossAD);<br/>
appdomain.cpp#L551](../../src/vm/appdomain.cpp#L551)

         |
         |
         |
        \|/

[HandleArrayObj = (PTRARRAYREF)AllocateObjectArray(Size, g_pObjectClass, TRUE);<br/>
appdomain.cpp#L220](../../src/vm/appdomain.cpp#L220)

         |
         |
         |
        \|/

[return AllocateArrayEx();<br/>
gchelpers.cpp#L997](../../src/vm/gchelpers.cpp#L997)

         |
         |
         |
        \|/

[return AllocateArrayEx();<br/>
gchelpers.cpp#L455](../../src/vm/gchelpers.cpp#L455)

         |
         |
         |
        \|/

[orArray = (ArrayBase *) AllocLHeap(totalSize, FALSE, pArrayMT->ContainsPointers());<br/>
gchelpers.cpp#L581](../../src/vm/gchelpers.cpp#L581)

         |
         |
         |
        \|/

[retVal = GCHeapUtilities::GetGCHeap()->AllocLHeap(size, flags);<br/>
gchelpers.cpp#L334](../../src/vm/gchelpers.cpp#L334)

         |
         |
         |
        \|/

[newAlloc = (Object*) hp->allocate_large_object (size + ComputeMaxStructAlignPadLarge(requiredAlignment), acontext->alloc_bytes_loh);<br/>
gc.cpp#L34311](../../src/gc/gc.cpp#L34311)

         |
         |
         |
        \|/

[if (! allocate_more_space (&acontext, (size + pad), max_generation+1))<br/>
gc.cpp#L30600](../../src/gc/gc.cpp#L30600)

         |
         |
         |
        \|/

[status = try_allocate_more_space (acontext, size, alloc_generation_number);<br/>
gc.cpp#L13465](../../src/gc/gc.cpp#L13465)

         |
         |
         |
        \|/

[BOOL can_allocate = ((gen_number == 0) ?
        allocate_small (gen_number, size, acontext, align_const) :
        allocate_large (gen_number, size, acontext, align_const));;<br/>
gc.cpp#L13178](../../src/gc/gc.cpp#L13178)