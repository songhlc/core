/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */

#ifndef INCLUDED_COMPHELPER_SCOPEGUARD_HXX
#define INCLUDED_COMPHELPER_SCOPEGUARD_HXX

#include <comphelper/comphelperdllapi.h>
#include <sal/log.hxx>
#include <com/sun/star/uno/Exception.hpp>

namespace comphelper {

/** ScopeGuard to ease writing exception-safe code.
 */
template <class Func> class ScopeGuard
{
public:
    /** @param func function object to be executed in dtor
    */
    explicit ScopeGuard( Func && func ) : m_func( std::move(func) ) {}

    ~ScopeGuard()
    {
        if (m_bDismissed)
            return;
        try
        {
            m_func();
        }
        catch (css::uno::Exception& exc)
        {
            SAL_WARN("comphelper", "UNO exception occurred: " << exc);
        }
        catch (...)
        {
            SAL_WARN("comphelper", "unknown exception occurred!");
        }
    }

    /** Dismisses the scope guard, i.e. the function won't
        be executed.
    */
    void dismiss() { m_bDismissed = true; }

private:
    // noncopyable until we have good reasons...
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;

    Func m_func;
    bool m_bDismissed = false;
};

} // namespace comphelper

#endif // ! defined(INCLUDED_COMPHELPER_SCOPEGUARD_HXX)

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
