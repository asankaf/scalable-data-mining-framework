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

#include "FlexCallQueue.h"
#include "tinyxml.h"
#include "Logger.h"
#include "ASObject.h"

#include <sstream>

HANDLE CFlexCallQueue::m_hMutex = NULL;
HANDLE CFlexCallQueue::m_hSem = NULL;

std::queue<CASRequest> CFlexCallQueue::m_qFlexCalls;
bool CFlexCallQueue::m_bShutdown = false;
std::vector<HANDLE> CFlexCallQueue::m_vHandles;

/* to ensure exclusive access to queue while inserting and removing */
HANDLE CFlexCallQueue::m_hResponseMutex;

/* to denote the number of waiting queue jobs */
HANDLE CFlexCallQueue::m_hResponseSem;

/* queue for responses from AS */
std::list<CASResponse*> CFlexCallQueue::m_qFlexResponses;

unsigned int CFlexCallQueue::m_uiResponseCount = 0;

CFlexCallQueue::CFlexCallQueue(void)
{
}

CFlexCallQueue::~CFlexCallQueue(void)
{
}


void CFlexCallQueue::Init()
{
    m_bShutdown = false;
    m_hMutex = CreateMutex(NULL, FALSE, NULL);    
    m_hSem = CreateSemaphore(NULL, 0, 1000, NULL);

    m_hResponseMutex = CreateMutex(NULL, FALSE, NULL);    
    m_hResponseSem = CreateSemaphore(NULL, 0, 1000, NULL);

}

bool CFlexCallQueue::Lock()
{ 
    DWORD dwResult;
    
    while ( (dwResult = WaitForSingleObject(m_hMutex, 5000)) == WAIT_TIMEOUT )
    {
        if ( dwResult != WAIT_OBJECT_0 || m_bShutdown == true )
            return false;
    }
    return true;
}

bool CFlexCallQueue::Unlock()
{
    ReleaseMutex(m_hMutex);
    return true;
}


bool CFlexCallQueue::WaitForWork()
{
    DWORD dwResult;
    
    while ( (dwResult = WaitForSingleObject(m_hSem, 5000)) == WAIT_TIMEOUT )
    {
        if ( m_bShutdown == true )
            return false;
    }
    return true;
}

void CFlexCallQueue::Uninit()
{
    m_bShutdown = true;
    CloseHandle(m_hMutex);
    CloseHandle(m_hSem);

    CloseHandle(m_hResponseMutex);
    CloseHandle(m_hResponseSem);

}

bool CFlexCallQueue::AddCall(CASRequest& obj)
{
    Lock();
    m_qFlexCalls.push(obj);
    Unlock();
    
    ReleaseSemaphore(m_hSem, 1, NULL);
    
    return true;
}

bool CFlexCallQueue::BuildFlexCall(const std::string& sFn, const std::vector<CASObject>& vArgs, CFlexCall& callout)
{
    callout.m_sFnName = sFn;
    
    for ( unsigned int i=0; i < vArgs.size(); i++ )
        callout.m_vArgs.push_back(vArgs[i]);
    
    return true;
}

DWORD WINAPI CFlexCallQueue::ProcessQueue(LPVOID lpParam)
{
    while ( 1 )
    {
        if ( m_bShutdown == true )
            break;
        
        if ( WaitForWork() == false )
        {
            LOG("WaitForWork() says to quit");
            break;
        }
        
        if ( Lock() == false )
        {
            LOG("Lock() says to quit");
            break;
        }
        
        CASRequest oCallObj = m_qFlexCalls.front();
        //std::string oCallObj = m_qFlexCalls.front();
        m_qFlexCalls.pop();
        Unlock();
        
        std::string sFn1;
        std::vector<CASObject> vArgs;
        ParseFlexXML(oCallObj.m_sRequest, sFn1, vArgs);
        
        CFlexCall oCaller;
        
        BuildFlexCall(sFn1, vArgs, oCaller);
        
        oCaller.Process(oCallObj.m_iFlashID);
        
    }
    return 0;
}

/* expects nodes which has tag name as string, number. puts the args in to vector */
bool ParseArgs(const TiXmlNode* argnode, std::vector<CASObject>& vArgOut)
{
    
    if ( argnode != NULL )
    {
        const TiXmlElement* argelem = argnode->ToElement();
        
        /* if ( std::string(argelem->Value()) == "object" )
           {
           
           } */

        for ( argelem = argelem->FirstChildElement(); argelem; argelem = argelem->NextSiblingElement())
        {
            if ( strcmp(argelem->Value(), "array") == 0 )
            {
                /* loop through each array element */
                const TiXmlElement* propelem = argelem;
                CASObject oArrObj;
                std::vector<CASObject> oArrValObj;
                
                for ( propelem = propelem->FirstChildElement(); propelem; propelem = propelem->NextSiblingElement())
                {
                    /* recursive call */
                    ParseArgs(propelem, oArrValObj);
                    if ( propelem->FirstChild() != NULL && propelem->FirstChild()->Value() != NULL )
                    {
                        //vArgOut.push_back(propelem->FirstChild()->Value());
                        //LOG(propelem->FirstChild()->Value());
                    }       
                }
                oArrObj.SetArray(oArrValObj);
                vArgOut.push_back(oArrObj);
            }
            else if ( strcmp(argelem->Value(), "object") == 0 )
            {
                /* loop through each array element */
                const TiXmlElement* propelem = argelem;
                CASObject oArrObj;
                std::map<std::string, CASObject> oMapValObj;
                
                //propelem->
                for ( propelem = propelem->FirstChildElement(); propelem; propelem = propelem->NextSiblingElement())
                {
                    std::vector<CASObject> oArrValObj;
                    //oArrValObj.clear();
                    /* recursive call */
                    ParseArgs(propelem, oArrValObj);
                    if ( oArrValObj.size() > 0 && std::string(propelem->Value()) == "property" )
                    {
                        //LOG("ID is " << propelem->Attribute("id") );
                        //oMapValObj[std::string(propelem->Attribute("id"))] = CASObject(oArrValObj[0]);
                        oMapValObj.insert(std::make_pair(std::string(propelem->Attribute("id")),CASObject(oArrValObj[0])));
                    }
                    if ( propelem->FirstChild() != NULL && propelem->FirstChild()->Value() != NULL )
                    {
                        //vArgOut.push_back(propelem->FirstChild()->Value());
                        //LOG(propelem->FirstChild()->Value());
                    }       
                }
                
                oArrObj.SetObject(oMapValObj);
                //oArrObj.Dump();
                vArgOut.push_back(oArrObj);
            }
            else if ( argelem->FirstChild() != NULL && argelem->FirstChild()->Value() != NULL )
            {
                CASObject oObj;
                if ( std::string(argelem->Value()) == "string" )
                    oObj.SetString(argelem->FirstChild()->Value());
                else if ( std::string(argelem->Value()) == "number" )
                {
                    double d  = 0;
                    std::istringstream iss(std::string(argelem->FirstChild()->Value()));
                    iss >> d;
                    oObj.SetNumber(d);
                }
                else if ( std::string(argelem->Value()) == "int" )
                {
                    int d  = 0;
                    std::istringstream iss(std::string(argelem->FirstChild()->Value()));
                    iss >> d;
                    oObj.SetInt(d);
                }
                /* Bug reported by Supun - Boolean return values come
                 * back as ASOBJECT_NONE. These if statements will
                 * never get executed because FirstChild() will be
                 * null since the node name itself is the value we
                 * want here */
                else if ( std::string(argelem->Value()) == "false" )
                {
                    oObj.SetBoolean(false);
                }
                else if ( std::string(argelem->Value()) == "true" )
                {
                    oObj.SetBoolean(true);
                }
                vArgOut.push_back(oObj);
                //LOG(argelem->FirstChild()->Value());
            }
            /* fix for the boolean value parsing bug */
            else if ( argelem->FirstChild() == NULL && argelem->Value() != NULL )
            {
                CASObject bObj;
                if ( std::string(argelem->Value()) == "false" )
                {
                    bObj.SetBoolean(false);
                }
                else if ( std::string(argelem->Value()) == "true" )
                {
                    bObj.SetBoolean(true);
                }
                if ( bObj.GetType() == ASOBJECT_BOOLEAN )
                {
                    vArgOut.push_back(bObj);
                }
            }


        }
        return true;        
    }
    
    
    
    
    return false;
}


bool CFlexCallQueue::ParseFlexXML(const std::string& xml, std::string& sFnOut, std::vector<CASObject>& vArgOut)
{
    TiXmlDocument doc( "xmlreq.xml" );
    
    sFnOut = "";
    vArgOut.clear();
    
    //doc.Parse(ws2s(sRequest).c_str());
    doc.Parse(xml.c_str());
    TiXmlNode* thenode = doc.FirstChild("invoke");
    if ( thenode )
    {
        const TiXmlElement* theelem = thenode->ToElement();
        if ( theelem )
        {
            sFnOut  = theelem->Attribute("name");
            
            const TiXmlNode* argnode = thenode->FirstChild("arguments");
            ParseArgs(argnode, vArgOut);
            
            return true;
        }
        
    }
    return false;
}

void CFlexCallQueue::Start(int n)
{
    Init();
    DWORD tid;
    for ( int i=0; i < n; i++ )
        m_vHandles.push_back(CreateThread(NULL, 0, ProcessQueue, NULL, 0, &tid));   
    
}

void CFlexCallQueue::Stop()
{
    m_bShutdown = true;
    for ( unsigned int i = 0; i < m_vHandles.size(); i++ )
    {
        WaitForSingleObject(m_vHandles[i], INFINITE);
        CloseHandle(m_vHandles[i]);
    }
    Uninit();
}

bool CFlexCallQueue::LockResponse()
{ 
    DWORD dwResult;
    
    while ( (dwResult = WaitForSingleObject(m_hResponseMutex, 5000)) == WAIT_TIMEOUT )
    {
        if ( dwResult != WAIT_OBJECT_0 || m_bShutdown == true )
            return false;
    }
    return true;
}

bool CFlexCallQueue::UnlockResponse()
{
    ReleaseMutex(m_hResponseMutex);
    return true;
}

void CFlexCallQueue::RegisterForResponse(CASResponse& oResp)
{
    LockResponse();
    oResp.m_uiID = m_uiResponseCount;
    m_uiResponseCount++;
    m_qFlexResponses.push_front(&oResp);
    UnlockResponse();
}

bool CFlexCallQueue::AddResponse(unsigned int uID, std::string& sVal)
{
    LockResponse();
    
    std::list<CASResponse*>::iterator i;
    
    for(i=m_qFlexResponses.begin(); i != m_qFlexResponses.end(); ++i)
    {
        if ( (*i)->m_uiID == uID )
        {
            (*i)->m_sResponse = sVal;
            (*i)->m_bResponded = true;
            ReleaseSemaphore(m_hResponseSem, 1, NULL);
            UnlockResponse();
            return true;
        }
    }
    
    UnlockResponse();
    LOG("Ignoring AS Response");
    return false;
}

/* waits for any entry to come to queue by listening to the semaphore */
bool CFlexCallQueue::WaitForResponse(CASResponse& oResp)
{
    DWORD dwResult;
    CASResponse* pTheResp = NULL;

    while ( m_bShutdown == false )
    {

        while ( (dwResult = WaitForSingleObject(m_hResponseSem, 5000)) == WAIT_TIMEOUT )
        {
            if ( m_bShutdown == true )
                return false;
        }
        
        LockResponse();

        if ( pTheResp == NULL )
        {
            std::list<CASResponse*>::iterator i;
            
            for(i=m_qFlexResponses.begin(); i != m_qFlexResponses.end(); ++i)
            {
                if ( (*i)->m_uiID == oResp.m_uiID )
                {
                    pTheResp = *i;
                    
                    if ( pTheResp->m_bResponded == true )
                    {
                        m_qFlexResponses.remove(pTheResp);
                        UnlockResponse();
                        return true;
                    }
                    else
                        break;
                }
            }
        }
        else
        {
            if ( pTheResp->m_bResponded == true )
            {
                UnlockResponse();
                return true;
            }
        }
        
        UnlockResponse();

    }
    
    return false;
}
