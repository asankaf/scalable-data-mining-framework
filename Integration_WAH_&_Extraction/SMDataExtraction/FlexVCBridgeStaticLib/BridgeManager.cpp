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

#include "BridgeManager.h"
#include <iostream>
#include "Logger.h"
#include "FlexBridge.h"
#include "windows.h"

static HANDLE g_hBridgeMutex = NULL;

std::vector<void*> CBridgeManager::m_vBridgePtrs;
std::vector<CASWork*> CBridgeManager::m_vDefaultCallbacks;

CBridgeManager::CBridgeManager(void)
{
}

CBridgeManager::~CBridgeManager(void)
{
}

int CBridgeManager::AddBridgePtr(void* ptr)
{
    DWORD dwResult;

    if ( g_hBridgeMutex == NULL )
    {
        g_hBridgeMutex = CreateMutex(NULL, FALSE, NULL);
    }
    
    dwResult = WaitForSingleObject(g_hBridgeMutex, INFINITE);

    if ( dwResult != WAIT_OBJECT_0 )
        return -1;

    //int iIndex = m_vBridgePtrs.size();

    m_vBridgePtrs.push_back(ptr);

    CFlexBridge* pBridge = (CFlexBridge*)ptr;
	int iIndex = pBridge->m_iFlashID;
    for ( unsigned int j = 0 ; j < m_vDefaultCallbacks.size(); j++ )
    {
        if ( m_vDefaultCallbacks[j]->m_iForBridgeID == -1 || m_vDefaultCallbacks[j]->m_iForBridgeID == iIndex )
        {
            /* we have to clone the work objects as the bridge frees
             * them once they are done with it. The work object
             * registered with BridgeManager gets freed on
             * BridgeManager's Cleanup */
            CASWork* pWork = m_vDefaultCallbacks[j]->Clone();
            pWork->SetBridge(ptr);
            pBridge->AddInitializationCallback(pWork);
        }
    }

    ReleaseMutex(g_hBridgeMutex);

    pBridge->Initialized();

    return iIndex;
}

int CBridgeManager::RemoveBridgePtr(void* ptr)
{
    int i = 0;
    for ( i = m_vBridgePtrs.size() - 1; i >= 0 ; i-- )
    {
        if ( m_vBridgePtrs[i] == ptr )
        {
            m_vBridgePtrs[i] = NULL;
            return i;
        }
    }

    return -1;
}

void* CBridgeManager::GetBridgePtr(unsigned int iID)
{
    if ( iID < m_vBridgePtrs.size() )
        return m_vBridgePtrs[iID];

    return NULL;
}

int CBridgeManager::GetIDBridgePtr(void* ptr)
{
    unsigned int i = 0;
    //LOG("bridge size is " << m_vBridgePtrs.size());
    for ( i = 0; i < m_vBridgePtrs.size(); i++ )
    {
        if ( (void*)m_vBridgePtrs[i] == ptr )
        {
            return i;
        }
    }
    
    return -1;
}

void CBridgeManager::RegisterDefaultCallbacks(CASWork* pWork)
{
    RegisterDefaultCallbacks(pWork, -1);
}

void CBridgeManager::RegisterDefaultCallbacks(CASWork* pWork, int iForBridgeID)
{
  
	if ( g_hBridgeMutex == NULL )
    {
        g_hBridgeMutex = CreateMutex(NULL, FALSE, NULL);
    }

    if ( iForBridgeID != -1 )
        pWork->m_iForBridgeID = iForBridgeID;

	m_vDefaultCallbacks.push_back(pWork);
	
	
}

void* CBridgeManager::GetBridgePtrFlashID(int iID)
{
    unsigned int i = 0;

    for ( i = 0; i < m_vBridgePtrs.size(); i++ )
    {
        if ( ((CFlexBridge*)m_vBridgePtrs[i])->m_iFlashID == iID )
        {
            return m_vBridgePtrs[i];
        }
    }

    return NULL;
}

void CBridgeManager::Cleanup()
{
    DWORD dwResult;
    
    dwResult = WaitForSingleObject(g_hBridgeMutex, INFINITE);

    if ( dwResult != WAIT_OBJECT_0 )
        return;

    for ( unsigned int j = 0 ; j < m_vBridgePtrs.size(); j++ )
    {
        CFlexBridge* pBridge = (CFlexBridge*)m_vBridgePtrs[j];
        delete pBridge;
    }

	for ( unsigned int jj = 0 ; jj < m_vDefaultCallbacks.size(); jj++ )
    {
        delete m_vDefaultCallbacks[jj];
    }
    m_vBridgePtrs.clear();

   ReleaseMutex(g_hBridgeMutex);
}
