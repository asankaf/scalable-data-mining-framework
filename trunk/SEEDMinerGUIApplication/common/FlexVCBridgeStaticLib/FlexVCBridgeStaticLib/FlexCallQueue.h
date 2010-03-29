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

#pragma once

#include "FlexCall.h"
#include "ASRequest.h"
#include "ASResponse.h"
#include "ASObject.h"
#include <queue>
#include <list>
#include <windows.h>

/* Queue that receives all AS requests */
class CFlexCallQueue
{
  private:

    /* boolean that becomes true when the app is going to quit */
    static bool m_bShutdown;

    /* the handles to the processing thread */
    static std::vector<HANDLE> m_vHandles;

    /* to ensure exclusive access to queue while inserting and removing */
    static HANDLE m_hMutex;

    /* to denote the number of waiting queue jobs */
    static HANDLE m_hSem;

    /* the actual queue */
    static std::queue<CASRequest> m_qFlexCalls;

    /* uses mutex for lock/unlock. Function is used instead of actual
     * mutex so that a timeout is possible */
    static bool Lock();
    static bool Unlock();

    /* waits for any entry to come to queue by listening to the semaphore */
    static bool WaitForWork();

    /* to ensure exclusive access to queue while inserting and removing */
    static HANDLE m_hResponseMutex;

    /* to denote the number of waiting queue jobs */
    static HANDLE m_hResponseSem;

    /* queue for responses from AS */
    static std::list<CASResponse*> m_qFlexResponses;

    /* uses mutex for lock/unlock. Function is used instead of actual
     * mutex so that a timeout is possible */
    static bool LockResponse();
    static bool UnlockResponse();

    static unsigned int m_uiResponseCount;

  public:

    /* create mutex, semaphore */
    static void Init();

    /* release resources */
    static void Uninit();

    /* add a as call to the q */
    static bool AddCall(CASRequest& obj);

    
    /* builds a CFlexCall object from a fn name and params */
    static bool BuildFlexCall(const std::string& sFn, const std::vector<CASObject>& vArgs, 
                              CFlexCall& callout);

    /* entry point for worker threads */
    static DWORD WINAPI ProcessQueue(LPVOID lpParam);


    /* break up incoming XML from AS */
    static bool ParseFlexXML(const std::string& xml, std::string& sFnOut, 
                             std::vector<CASObject>& vArgOut);

    CFlexCallQueue(void);
    ~CFlexCallQueue(void);

    /* start n worker threads */
    static void Start(int n);

    /* stop all workers */
    static void Stop();

    static void RegisterForResponse(CASResponse& oResp);

    /* waits for any entry to come to queue by listening to the semaphore */
    static bool WaitForResponse(CASResponse& oResp);


    static bool AddResponse(unsigned int uID, std::string& sVal);

};
