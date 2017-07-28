// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
//
// File: typehandle.inl
//


//

//
// ============================================================================


#ifndef _TYPEHANDLE_INL_
#define _TYPEHANDLE_INL_

#include "typehandle.h"

inline mdTypeDef TypeHandle::GetCl() const
{ 
    LIMITED_METHOD_DAC_CONTRACT;

    PREFIX_ASSUME(GetMethodTable() != NULL);
    return GetMethodTable()->GetCl(); 
}

inline PTR_MethodTable TypeHandle::GetMethodTable() const
{
    LIMITED_METHOD_DAC_CONTRACT;

    if (IsTypeDesc())
        return(AsTypeDesc()->GetMethodTable());
    else
        return AsMethodTable();
}

inline void TypeHandle::SetIsFullyLoaded()
{
    LIMITED_METHOD_CONTRACT;

    if (IsTypeDesc())
        return AsTypeDesc()->SetIsFullyLoaded();
    else
        return AsMethodTable()->SetIsFullyLoaded();
}

inline MethodTable* TypeHandle::GetMethodTableOfElementType() const
{
    LIMITED_METHOD_CONTRACT;

    if (IsTypeDesc())
    {
        TypeHandle elementType = AsTypeDesc()->GetTypeParam();
        return elementType.GetMethodTableOfElementType();
    }
    else
    {
        return AsMethodTable();
    }
}

inline MethodTable * TypeHandle::GetPossiblySharedArrayMethodTable() const
{
    LIMITED_METHOD_CONTRACT;

    _ASSERTE(IsArrayType());

    if (IsArray())
        return AsArray()->GetTemplateMethodTable();
    else
        return AsMethodTable();
}

inline TypeHandle TypeHandle::GetElementType() const
{
    LIMITED_METHOD_CONTRACT;

    if (IsTypeDesc())
    {
        if (IsGenericVariable())
            return *this;

        return AsTypeDesc()->GetTypeParam().GetElementType();
    }
    else
    {
        return *this;
    }
}

inline BOOL TypeHandle::IsZapped() const
{
    LIMITED_METHOD_DAC_CONTRACT;

    return (GetZapModule() != NULL);
}

inline PTR_ArrayTypeDesc TypeHandle::AsArray() const
{
    LIMITED_METHOD_DAC_CONTRACT;

    _ASSERTE(IsArray());

    PTR_ArrayTypeDesc result = PTR_ArrayTypeDesc(m_asTAddr - 2);
    PREFIX_ASSUME(result != NULL);
    return result;
}

// Methods to allow you get get a the two possible representations
inline PTR_MethodTable TypeHandle::AsMethodTable() const
{
    LIMITED_METHOD_DAC_CONTRACT;

    _ASSERTE(!IsTypeDesc());

    return PTR_MethodTable(m_asTAddr);
}

inline PTR_TypeDesc TypeHandle::AsTypeDesc() const 
{
    LIMITED_METHOD_DAC_CONTRACT;

    _ASSERTE(IsTypeDesc());

    PTR_TypeDesc result = PTR_TypeDesc(m_asTAddr - 2);
    PREFIX_ASSUME(result != NULL);
    return result;
}

inline FnPtrTypeDesc* TypeHandle::AsFnPtrType() const
{
    LIMITED_METHOD_DAC_CONTRACT;

    _ASSERTE(IsFnPtrType());

    FnPtrTypeDesc* result = PTR_FnPtrTypeDesc(m_asTAddr - 2);
    PREFIX_ASSUME(result != NULL);
    return result;
}

inline TypeVarTypeDesc* TypeHandle::AsGenericVariable() const
{ 
    LIMITED_METHOD_DAC_CONTRACT;

    _ASSERTE(IsGenericVariable());

    TypeVarTypeDesc* result = PTR_TypeVarTypeDesc(m_asTAddr - 2);
    PREFIX_ASSUME(result != NULL);
    return result;
}

inline BOOL TypeHandle::IsNativeValueType() const
{
    LIMITED_METHOD_DAC_CONTRACT;
    return (IsTypeDesc() && AsTypeDesc()->IsNativeValueType());
}

inline MethodTable *TypeHandle::AsNativeValueType() const
{
    LIMITED_METHOD_CONTRACT;

    _ASSERTE(IsNativeValueType());
    return AsTypeDesc()->GetMethodTable();
}

inline BOOL TypeHandle::IsTypicalTypeDefinition() const
{
    LIMITED_METHOD_CONTRACT;

    return !HasInstantiation() || IsGenericTypeDefinition();
}

inline BOOL TypeHandle::SupportsGenericInterop(InteropKind interopKind) const
{
    LIMITED_METHOD_CONTRACT;
    return (!IsTypeDesc() && AsMethodTable()->SupportsGenericInterop(interopKind));
}

inline BOOL TypeHandle::HasTypeEquivalence() const
{
    LIMITED_METHOD_CONTRACT;

    if (IsTypeDesc()) 
        return AsTypeDesc()->HasTypeEquivalence();
    else
        return AsMethodTable()->HasTypeEquivalence();
}


//--------------------------------------------------------------------------------------
// IsEquivalentTo is based on Guid and TypeIdentifier attributes to support the "no-PIA"
// feature. The idea is that compilers pull types from the PIA into different assemblies
// and these types - represented by separate MTs/TypeHandles - are considered equivalent
// for certain operations.


#ifndef DACCESS_COMPILE
inline BOOL TypeHandle::IsEquivalentTo(TypeHandle type COMMA_INDEBUG(TypeHandlePairList *pVisited /*= NULL*/)) const
{
    CONTRACTL
    {
        THROWS;
        GC_TRIGGERS;
        MODE_ANY;
        SO_TOLERANT;
    }
    CONTRACTL_END;

    if (*this == type)
        return TRUE;

#ifdef FEATURE_TYPEEQUIVALENCE
    // bail early for normal types
    if (!HasTypeEquivalence() || !type.HasTypeEquivalence())
        return FALSE;

    if (IsTypeDesc())
        return AsTypeDesc()->IsEquivalentTo(type COMMA_INDEBUG(pVisited));

    if (type.IsTypeDesc())
        return FALSE;

    return AsMethodTable()->IsEquivalentTo_Worker(type.AsMethodTable() COMMA_INDEBUG(pVisited));
#else
    return FALSE;
#endif
}
#endif

// Execute the callback functor for each MethodTable that makes up the given type handle.  This method
// does not invoke the functor for generic variables
template<class T>
inline void TypeHandle::ForEachComponentMethodTable(T &callback) const
{
    CONTRACTL
    {
        THROWS;
        GC_TRIGGERS;
        MODE_ANY;
    }
    CONTRACTL_END;

    if (IsTypeDesc() && AsTypeDesc()->HasTypeParam())
    {
        // If we have a type parameter, then we just need to invoke ourselves on that parameter
        AsTypeDesc()->GetTypeParam().ForEachComponentMethodTable(callback);
    }
    else if (IsFnPtrType())
    {
        // If we are a function pointer, then we need to invoke the callback method on the function
        // pointer's return type as well as each of its argument types
        FnPtrTypeDesc *pFnPtr = AsFnPtrType();
        for (DWORD iArg = 0; iArg < pFnPtr->GetNumArgs() + 1; ++iArg)
        {
            pFnPtr->GetRetAndArgTypesPointer()[iArg].ForEachComponentMethodTable(callback);
        }
    }
    else if (HasInstantiation())
    {
        // If we have a generic instantiation, we need to invoke the callback on each of the generic
        // parameters as well as the root method table.
        callback(GetMethodTable());

        Instantiation instantiation = GetInstantiation();
        for (DWORD iGenericArg = 0; iGenericArg < instantiation.GetNumArgs(); ++iGenericArg)
        {
            instantiation[iGenericArg].ForEachComponentMethodTable(callback);
        }
    }
    else if (IsGenericVariable())
    {
        // We don't invoke the callback on generic variables since they don't have method tables
        return;
    }
    else
    {
        // Otherwise, we must be a simple type, so just do the callback directly on the method table
        callback(GetMethodTable());
    }
}


#endif  // _TYPEHANDLE_INL_

