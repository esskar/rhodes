/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "RhoThread.h"
#include "IRhoClassFactory.h"
#include "RhoMutexLock.h"

namespace rho {
namespace common {

CRhoThread::CRhoThread()
{
    m_nState = TS_NONE;
    m_pImpl = rho_get_RhoClassFactory()->createThreadImpl();
}

void CRhoThread::start(EPriority ePriority)
{
    if ( !isAlive() )
    {
        m_nState = TS_RUNNING;
        m_pImpl->start(this, ePriority);
    }
}

}
}

# ifdef __cplusplus
extern "C" {
# endif

void* rho_mutex_create() {
    return (void*)(new rho::common::CMutex());
}

void rho_mutex_destroy(void* m) {
    delete m;
}

void rho_mutex_lock(void* m) {
    ((rho::common::CMutex*)m)->Lock();
}

void rho_mutex_release(void* m) {
    ((rho::common::CMutex*)m)->Unlock();
}

# ifdef __cplusplus
}
#endif //__cplusplus
