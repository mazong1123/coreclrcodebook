// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

/*=============================================================================
**
**
**
** Purpose: Exception class representing an AV that was deemed unsafe and may have corrupted the application.
**
**
=============================================================================*/

using System;
using System.Runtime.Serialization;

namespace System
{
    public class AccessViolationException : SystemException
    {
        public AccessViolationException()
            : base(SR.Arg_AccessViolationException)
        {
            HResult = __HResults.E_POINTER;
        }

        public AccessViolationException(String message)
            : base(message)
        {
            HResult = __HResults.E_POINTER;
        }

        public AccessViolationException(String message, Exception innerException)
            : base(message, innerException)
        {
            HResult = __HResults.E_POINTER;
        }

        protected AccessViolationException(SerializationInfo info, StreamingContext context) : base(info, context)
        {
            throw new PlatformNotSupportedException();
        }

#pragma warning disable 169  // Field is not used from managed.
        private IntPtr _ip;             // Address of faulting instruction.
        private IntPtr _target;         // Address that could not be accessed.
        private int _accessType;        // 0:read, 1:write
#pragma warning restore 169
    }
}
