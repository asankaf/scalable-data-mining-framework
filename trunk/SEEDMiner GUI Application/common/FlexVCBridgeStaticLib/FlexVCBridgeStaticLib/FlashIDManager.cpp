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

#include "FlashIDManager.h"
#include <iostream>
#include "Logger.h"

std::vector<void*> CFlashIDManager::m_vFlashPtrs;

CFlashIDManager::CFlashIDManager(void)
{
}

CFlashIDManager::~CFlashIDManager(void)
{
}

int CFlashIDManager::AddFlashPtr(void* ptr)
{
    int iIndex = m_vFlashPtrs.size();

    m_vFlashPtrs.push_back(ptr);
    
    return iIndex;
}

int CFlashIDManager::RemoveFlashPtr(void* ptr)
{
    int i = 0;
    for ( i = m_vFlashPtrs.size() - 1; i >= 0 ; i-- )
    {
        if ( m_vFlashPtrs[i] == ptr )
        {
            m_vFlashPtrs[i] = NULL;
            return i;
        }
    }

    return -1;
}

void* CFlashIDManager::GetFlashPtr(unsigned int iID)
{
    if ( iID < m_vFlashPtrs.size() )
        return m_vFlashPtrs[iID];

    return NULL;
}

int CFlashIDManager::GetIDFlashPtr(void* ptr)
{
    unsigned int i = 0;
    //LOG("flash objs size is " << m_vFlashPtrs.size());
    for ( i = 0; i < m_vFlashPtrs.size(); i++ )
    {
        if ( (void*)m_vFlashPtrs[i] == ptr )
        {
            return i;
        }
    }
    
    return -1;
}
