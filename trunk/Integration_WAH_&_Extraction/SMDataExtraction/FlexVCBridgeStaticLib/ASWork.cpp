/*
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Flex C++ Bridge.
 *
 * The Initial Developer of the Original Code is
 * Anirudh Sasikumar (http://anirudhs.chaosnet.org/).
 * Portions created by the Initial Developer are Copyright (C) 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
*/

#include "ASWork.h"
#include "FlexBridge.h"

CASWork::CASWork()
{
    m_pBridge  = NULL;
    m_iForBridgeID = -1;
}


CASWork::CASWork(void* pBridge)
{
    SetBridge(pBridge);
}

CASWork::~CASWork(void)
{

}

void CASWork::SetBridge(void* pBridge)
{
    m_pBridge = pBridge;
}

void CASWork::Run()
{
    if ( m_pBridge )
    {
        Worker();
    }
}

CASWork* CASWork::Clone()
{
    return NULL;
}

/* void CASWork::Worker() */
/* { */
/*     CFlexBridge* pBridge = (CFlexBridge*)m_pBridge; */
/* } */

