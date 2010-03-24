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

/* TODO: Documentation. (Poor you!) */

#include "FlexBridge.h"
#include "FlexCall.h"
#include <windows.h>
#include "FlexCallQueue.h"
#include "Logger.h"
#include "ctype.h"
#include "ASInstance.h"
#include <sstream>
#include "BridgeManager.h"
#include "BridgeException.h"

using std::string;
using std::map;
using std::stringstream;
using std::istringstream;

const short int CFlexBridge::TYPE_ASINSTANCE =  1;
const short int CFlexBridge::TYPE_ASFUNCTION =  2;

const short int CFlexBridge::TYPE_JSFUNCTION =  3;
const short int CFlexBridge::TYPE_ANONYMOUS =   4;

/* for register local function to be thread safe */
static HANDLE g_hLocalFnMapMutex = CreateMutex(NULL, FALSE, NULL);

CFlexBridge::CFlexBridge(const string& name, int iFlashID)
{
    m_sName = name;
    m_iFlashID = iFlashID;
}

CFlexBridge::~CFlexBridge(void)
{
	std::map<string,CASProxy*>::iterator ii1;
   for( ii1=m_RemoteTypeMap.begin(); ii1!=m_RemoteTypeMap.end(); ++ii1)
   {
       CASProxy* pCache;
       pCache = (*ii1).second;
       delete pCache;
   }
   std::map<string,CASProxy*>::iterator ii2;
   for( ii2=m_RemoteFunctionMap.begin(); ii2!=m_RemoteFunctionMap.end(); ++ii2)
   {
       CASProxy* pCache;
       pCache = (*ii2).second;
       delete pCache;
   }
   std::map<string,CASInstance*>::iterator ii3;
   for( ii3=m_RemoteInstanceMap.begin(); ii3!=m_RemoteInstanceMap.end(); ++ii3)
   {
       CASInstance* pCache;
       pCache = (*ii3).second;
       delete pCache;
   }
  /* for( map<CCPPInstance*,void (*)(CASObject& , CFlexBridge*),cmp_cppinstance>::iterator ii=m_LocalFunctionMap.begin(); ii!=m_LocalFunctionMap.end(); ++ii) */
  /*  { */
  /*      CCPPInstance* pCache; */
  /*      pCache = (*ii).first; */
  /*      delete pCache; */
  /*  } */

}

void CFlexBridge::Root(CASObject& oRespObj)
{
    CallASFunction(string("getRoot"), NULL, oRespObj);
}

void CFlexBridge::CallASFunction(const string& sFn, const std::vector<CASObject>* vArgs, CASObject& outObj)
{
    string res;
    if ( vArgs == NULL )
    {
        CFlexCall::BuildXML(sFn, res);
    }
    else
    {
        CFlexCall::BuildXML(sFn, *vArgs, res);
    }
    string* sendmsg = new string(res);
    CASResponse oASResp;
    oASResp.m_iFlashID = m_iFlashID;
    CFlexCallQueue::RegisterForResponse(oASResp);
    SendMessage((HWND)CFlexCall::m_pDlg, WM_FLEXCALLBACK_RETURN, (WPARAM)sendmsg, (LPARAM)&oASResp);
    LOGDEBUG("waiting for as response");
    CFlexCallQueue::WaitForResponse(oASResp);
    LOGDEBUG("got as response");

    string fnout;
    std::vector<CASObject> objs;
    CFlexCallQueue::ParseFlexXML(oASResp.m_sResponse, fnout, objs);

    if ( objs.size() > 1 )
        LOG("Warning: Unexpected return array size in FlexBridge: " << objs.size());
    
    if ( objs.size () == 1 )
    {
        //objs[0].Dump();
        Deserialize(objs[0], outObj);
    }
    
    //LOG("Response from function: " << oASResp.m_sResponse);
}

void CFlexBridge::CallAS(const string& sFn, CASObject& outObj)
{
    CallASFunction(sFn, NULL, outObj);
}

void CFlexBridge::CallAS(const string& sFn, const std::vector<CASObject>& vArgs, 
                         CASObject& outObj)
{
    CallASFunction(sFn, &vArgs, outObj);
}

/* Look at Ely's FABridge.js. This is a C++ port of that. */
void CFlexBridge::AddTypeDataToCache(CASObject& newTypes)
{
    /* it has to be an object of type array */
    CASValue newTypesValue;
    /* add new functions to cache */
    newTypes.GetValue(newTypesValue);

    if ( newTypesValue.m_aVal != NULL )
    {
        for ( unsigned int i = 0; i < newTypesValue.m_aVal->size(); i++ )
        {
            CASValue oTypeObj;
            (*newTypesValue.m_aVal)[i].GetValue(oTypeObj);

            CASProxy* oObjProxy = new CASProxy();
            string sTypeName;


			std::map<string, CASObject>::iterator cur  = oTypeObj.m_oVal->find(string("name"));
            if ( cur != oTypeObj.m_oVal->end() )
            {
                LOGDEBUG("This is the name");
                CASObject* methodObj;
                CASValue methodObjValue;

                methodObj = &(cur->second);
                methodObj->GetValue(methodObjValue);

                if ( methodObjValue.m_sVal )
                {
                    sTypeName = *methodObjValue.m_sVal;
                }
            }
            else
            {
                LOGDEBUG("No typename");
            }

			std::map<string, CASProxy*>::iterator dupIt  = m_RemoteTypeMap.find(sTypeName);
            if ( dupIt != m_RemoteTypeMap.end() )
            {
                delete oObjProxy;
                return;
            }
            else
                m_RemoteTypeMap.insert(std::make_pair(sTypeName, oObjProxy));

            cur  = oTypeObj.m_oVal->find(string("methods"));
            if ( cur != oTypeObj.m_oVal->end() )
            {
                LOGDEBUG("There are methods");
                CASObject* methodObj;
                CASValue methodObjValue;

                methodObj = &(cur->second);
                methodObj->GetValue(methodObjValue);

                if ( methodObjValue.m_aVal )
                {
                    LOGDEBUG("Got array of methods");
                    for ( unsigned int j = 0; j < methodObjValue.m_aVal->size(); j++ )
                    {
                        CASValue methodName;
                        (*methodObjValue.m_aVal)[j].GetValue(methodName);
                        if ( methodName.m_sVal )
                            oObjProxy->Add(*methodName.m_sVal, ASPROXY_METHOD);
                    }
                }
                //AddTypeDataToCache(*newTypes);
            }
            else
            {
                LOGDEBUG("No methods");
            }

            cur  = oTypeObj.m_oVal->find(string("accessors"));
            if ( cur != oTypeObj.m_oVal->end() )
            {
                LOGDEBUG("There are accessors");
                CASObject* methodObj;
                CASValue methodObjValue;

                methodObj = &(cur->second);
                methodObj->GetValue(methodObjValue);

                if ( methodObjValue.m_aVal )
                {
                    LOGDEBUG("Got array of accessors");
                    for ( unsigned int j = 0; j < methodObjValue.m_aVal->size(); j++ )
                    {
                        CASValue methodName;
                        (*methodObjValue.m_aVal)[j].GetValue(methodName);

                        if ( methodName.m_sVal )
                        {
                            string oPropStr = *methodName.m_sVal;
                            string oFinalPropStrGet = "get";
                            oFinalPropStrGet += oPropStr; 
                            string oFinalPropStrSet = "set";
                            oFinalPropStrSet += oPropStr; 
                            if ( oPropStr.length() > 1 )
                            {
                                /* if ( oPropStr[0] >= 'a' && oPropStr[0] <= 'z' ) */
                                /* { */
                                /*     oFinalPropStrGet = "get"; */
                                /*     oFinalPropStrGet += toupper(oPropStr[0]); */
                                /*     oFinalPropStrGet += oPropStr.substr(1); */

                                /*     oFinalPropStrSet = "set"; */
                                /*     oFinalPropStrSet += toupper(oPropStr[0]); */
                                /*     oFinalPropStrSet += oPropStr.substr(1); */

                                /* } */
                                /* Change to support Devin Garner's FABridge */
                                if ( ( oPropStr[0] >= 'a' && oPropStr[0] <= 'z' ) || ( oPropStr[0] >= 'A' && oPropStr[0] <= 'Z' ) )
                                {
                                    oFinalPropStrGet = "get";
                                    oFinalPropStrGet += oPropStr;
                                    oFinalPropStrSet = "set";
                                    oFinalPropStrSet += oPropStr;
                                }
                            }
                            
                            oObjProxy->Add(oFinalPropStrGet, ASPROXY_GETTER);
                            oObjProxy->Add(oFinalPropStrSet, ASPROXY_SETTER);
                        }
                    }
                }


                //AddTypeDataToCache(*newTypes);
            }
            else
            {
                LOGDEBUG("No accessors");
            }

            /* cur  = oTypeObj.m_oVal->find(string("name")); */
            /* if ( cur != oTypeObj.m_oVal->end() ) */
            /* { */
            /*     LOGDEBUG("This is the name"); */
            /*     CASObject* methodObj; */
            /*     CASValue methodObjValue; */

            /*     methodObj = &(cur->second); */
            /*     methodObj->GetValue(methodObjValue); */

            /*     if ( methodObjValue.m_sVal ) */
            /*     { */
            /*         sTypeName = *methodObjValue.m_sVal; */
            /*     } */
            /* } */
            /* else */
            /* { */
            /*     LOGDEBUG("No typename"); */
            /* } */

            /* map<string, CASProxy*>::iterator dupIt  = m_RemoteTypeMap.find(sTypeName); */
            /* if ( dupIt != m_RemoteTypeMap.end() ) */
            /* { */
            /*     delete oObjProxy; */
            /* } */
            /* else */
            /*     m_RemoteTypeMap.insert(std::make_pair(sTypeName, oObjProxy)); */
        }
    }
    else
    {
        LOGDEBUG("Newtypesvalue is not an array!");
    }
}

void CFlexBridge::Deserialize(const CASObject& obj, CASObject& oOutObj)
{
    if ( obj.GetType() == ASOBJECT_NUMBER || 
         obj.GetType() == ASOBJECT_INT || 
         obj.GetType() == ASOBJECT_STRING || 
         obj.GetType() == ASOBJECT_BOOLEAN )
    {
        if ( obj.GetType() == ASOBJECT_STRING )
        {
            CASValue objStr;
            obj.GetValue(objStr);
            
            unsigned int uiErrIndex = (*objStr.m_sVal).find("__FLASHERROR__") ;

            if ( uiErrIndex == 0 )
            {
                string sErr = (*objStr.m_sVal).substr(string("__FLASHERROR__||").length());
                LOG("Flash Error occurred: " << sErr);
                oOutObj = sErr;
                throw CBridgeException(sErr);
                return;
                
            }
        }
        oOutObj = obj;
    }
    else if ( obj.GetType() == ASOBJECT_ARRAY )
    {
        CASValue objValue;        
        obj.GetValue(objValue);

        if ( objValue.m_aVal )
        {
            LOGDEBUG("Deserialize: it is an array");
            std::vector<CASObject> vArrObjs;

            for ( unsigned int ia = 0; ia < objValue.m_aVal->size(); ia++ )
            {
                CASObject oArrElem;
                Deserialize((*objValue.m_aVal)[ia], oArrElem);
                vArrObjs.push_back(oArrElem);
            }

            oOutObj.SetArray(vArrObjs);
        }
    }
    else if ( obj.GetType() == ASOBJECT_OBJECT ) /* heh heh */
    {
        /* iterate and add new types to cache */
        LOGDEBUG("Deserialize:  it is an object");

        CASValue objValue;        

        CASObject* newTypes;
        obj.GetValue(objValue);

        if ( objValue.m_oVal != NULL )
        {
			std::map<string, CASObject>::iterator cur  = objValue.m_oVal->find(string("newTypes"));
            if ( cur != objValue.m_oVal->end() )
            {
                LOGDEBUG("There are newTypes");
                newTypes = &(cur->second);
                AddTypeDataToCache(*newTypes);
            }
            else
            {
                LOGDEBUG("No newTypes");
            }

            cur  = objValue.m_oVal->find(string("newRefs"));
            if ( cur != objValue.m_oVal->end() )
            {
                LOGDEBUG("There are newRefs");
                newTypes = &(cur->second);
                CreateProxies(*newTypes);
            }
            else
            {
                LOGDEBUG("No newRefs");
            }

            cur  = objValue.m_oVal->find(string("type"));
            if ( cur != objValue.m_oVal->end() )
            {
                newTypes = &(cur->second);
                CASValue oASType;
                CASObject* oTypeValObj = NULL;
                CASValue oASValType;
                newTypes->GetValue(oASType);

                cur  = objValue.m_oVal->find(string("value"));

                if ( cur != objValue.m_oVal->end() )
                {
                    oTypeValObj = &(cur->second);
                    oTypeValObj->GetValue(oASValType);

                    LOGDEBUG("Got val obj");
                    //oTypeValObj->Dump();
                }

                if ( oASType.m_nVal )
                {
                    LOGDEBUG(" AS Type is " << *oASType.m_nVal << " ");

                    switch ( (int)*oASType.m_nVal )
                    {
                        case TYPE_ASINSTANCE:

                            if ( oTypeValObj )
                            {
                                stringstream s;
                                s << *oASValType.m_nVal;
                                LOGDEBUG("Type value as string is " << s.str() );
                                /* look up in cache for this instance */
                                CASInstance* pAsinst = GetInstance(s.str());

                                if ( pAsinst )
                                {
                                    oOutObj.SetASInstance(*pAsinst);
                                }
                            }


                            break;

                        case TYPE_ASFUNCTION:

                            /* get function proxy */
                            LOG("Warning: AS Function not supported yet!");
                            

                            break;

                        case TYPE_ANONYMOUS:

                            if ( oTypeValObj )
                            {
                                oOutObj = *oTypeValObj;
                            }
                            break;

                        case TYPE_JSFUNCTION:

                            if ( oTypeValObj )
                            {
                                stringstream s;
                                s << *oASValType.m_nVal;

                                CCPPInstance oCPPinst;
                                oCPPinst.m_sID = s.str();
                                oCPPinst.m_uiBridgeID = CBridgeManager::GetIDBridgePtr((void*)this);
                                
                                oOutObj = oCPPinst;
                            }

                            break;
                    }
                }
            }
            else
            {
                LOG("No type");
            }

        }
        else
        {
            LOG("Object to be deserialized has map empty!");
        }
        
        
        
        
    }
}

void CFlexBridge::CreateProxy(string& sType, CASObject& newTypes)
{
    /* TODO: add support of creation of local C++ objects (look at if
     * (ut.isUserProto){var instFactory = window[ut.simpleType];var
     * instance = new instFactory(this.name,
     * objID);instance.fb_instance_id = objID; }
     */

    CASInstance* oASInst = new CASInstance();

	std::map<string, CASInstance*>::iterator cur  = m_RemoteInstanceMap.find(sType);
    if ( cur != m_RemoteInstanceMap.end() )
    {
        delete oASInst;
        return;
    }
    else
        m_RemoteInstanceMap.insert(std::make_pair(sType, oASInst));

    /* check if the type is there in our cache */
    CASValue newTypesValue;
    /* add new functions to cache */
    newTypes.GetValue(newTypesValue);

    oASInst->m_uiBridgeID = CBridgeManager::GetIDBridgePtr((void*)this);
    oASInst->m_sID = sType;

    if ( newTypesValue.m_sVal )
    {
		std::map<string, CASProxy*>::iterator cur  = m_RemoteTypeMap.find(*newTypesValue.m_sVal);
        if ( cur != m_RemoteTypeMap.end() )
        {
            /* found type */
            LOGDEBUG("Type " << *newTypesValue.m_sVal <<  " is a remote type");
            oASInst->m_oTypeProxy = cur->second;
        }
    }

    /* map<string, CASInstance*>::iterator cur  = m_RemoteInstanceMap.find(sType); */
    /* if ( cur != m_RemoteInstanceMap.end() ) */
    /* { */
    /*     delete oASInst; */
    /* } */
    /* else */
    /*     m_RemoteInstanceMap.insert(std::make_pair(sType, oASInst)); */
}

void CFlexBridge::CreateProxies(CASObject& newTypes)
{
    /* it has to be an object of type array */
    CASValue newTypesValue;
    /* add new functions to cache */
    newTypes.GetValue(newTypesValue);

    if ( newTypesValue.m_oVal != NULL )
    {
		std::map<string, CASObject>::iterator ii;
        for(ii= newTypesValue.m_oVal->begin(); ii!= newTypesValue.m_oVal->end(); ++ii)
        {
            string sTypeName;
            sTypeName = (*ii).first;
            
            LOGDEBUG("Got proxy " << sTypeName );
            CreateProxy(sTypeName, (*ii).second);
        }
    }
    else
    {
        LOG("CreateProxies did not get an object!");
    }

}

CASInstance* CFlexBridge::GetInstance(string iID)
{
	std::map<string, CASInstance*>::iterator cur  = m_RemoteInstanceMap.find(iID);

    if ( cur != m_RemoteInstanceMap.end() )
    {
        return cur->second;
    }

    return NULL;
}

bool CFlexBridge::Call(const CASObject& oObjIn, const char* sFn, CASObject& oObjOut)
{	
    return CallInternal(oObjIn, sFn, NULL, oObjOut);
}

bool CFlexBridge::Call(const CASObject& oObjIn, const char* sFn, const std::vector<CASObject>& oArgsIn, CASObject& oObjOut)
{
    return CallInternal(oObjIn, sFn,  &oArgsIn, oObjOut);
}

bool CFlexBridge::CallInternal(const CASObject& oObjIn, const char* sFn, const std::vector<CASObject>* oArgsIn, CASObject& oObjOut)
{
    /* You don't see this */
    CASInstance* pInst = oObjIn.GetInstance();
    string sFnStr = sFn;
    if ( pInst )
    {
        
        std::vector<CASObject> vProps;
        CASObject objRef;
        CASObject propName;
        double d  = 0;
        std::istringstream iss(pInst->m_sID);
        iss >> d;
                
        objRef.SetNumber(d);
        string propStr = sFnStr;
        ASProxyType oPType = pInst->m_oTypeProxy->GetType(sFnStr);

        if ( oPType == ASPROXY_NONE )
        {
            /* not in our type cache */
            string errType;
            GetProxyName(pInst->m_oTypeProxy, errType);
            string errMsg = "Method ";
            errMsg += sFnStr;
            errMsg += " not found in type cache for type ";
            if ( errType.length() == 0 )
                errMsg += pInst->m_sID;
            else
                errMsg += errType;

            throw CBridgeException(errMsg);
        }
        if ( oPType == ASPROXY_GETTER || oPType == ASPROXY_SETTER )
        {
            /* Change to support Devin Garner's FABridge */
            /* if (propStr[0] == 'G' || propStr[0] == 'S' ) */
            /* { */
                propStr = propStr.substr(3);
            /* } */
            /* else */
            /* { */
            /*     propStr = propStr.substr(3); */
            /*     char c = tolower(propStr[0]); */
            /*     propStr[0] = c; */
            /* } */
        }
        propName.SetString(propStr);
        
        vProps.push_back(objRef);
        vProps.push_back(propName);
        if ( oPType == ASPROXY_GETTER )
        {
            if ( oArgsIn )
            {
                LOG("Warning: Silently ignoring args for getter method");
            }
            CallASFunction(string("getPropFromAS"), &vProps, oObjOut);
        }
        else if ( oPType == ASPROXY_SETTER )
        {
            if ( oArgsIn )
            {
                if ( oArgsIn->size() > 1 )
                {
                    throw CBridgeException("Setter method passed more than 1 argument");
                }
                for ( unsigned int i = 0; i < oArgsIn->size(); i++ )
                {
                    vProps.push_back((*oArgsIn)[i]);
                }
                CallASFunction(string("setPropInAS"), &vProps, oObjOut);
            }
            else
                throw CBridgeException("Setter method called without an argument");
        }
        else if ( oPType == ASPROXY_METHOD )
        {
            if ( oArgsIn )
            {
                CASObject oArgArrObj;

                for ( unsigned int i=0; i < oArgsIn->size(); i++ )
                {
                    if ( (*oArgsIn)[i].GetType() == ASOBJECT_LOCALINSTANCE  )
                    {
                        if ( (*oArgsIn)[i].GetCPPInstance()->m_sID.length() == 0 )
                        {
                            CCPPInstance oInst = RegisterLocalFunction((void (*)(CASObject& , CFlexBridge*))((*oArgsIn)[i].GetCPPInstance()->m_pFn));
                            (*oArgsIn)[i].GetCPPInstance()->m_sID = oInst.m_sID;
                            (*oArgsIn)[i].GetCPPInstance()->m_uiBridgeID = oInst.m_uiBridgeID;
                        }
                    }
                }

                oArgArrObj.SetArray(*oArgsIn);
                vProps.push_back(oArgArrObj);
                CallASFunction(string("invokeASMethod"), &vProps, oObjOut);
            }
            else
            {
				CASObject oArgArrObj;
				std::vector<CASObject> vEmptyArgs;
				vProps.push_back(vEmptyArgs);
                CallASFunction(string("invokeASMethod"), &vProps, oObjOut);
            }
        }
    }
    else
    {
        throw CBridgeException("ASObject is not an AS instance!");
    }
    
    return false;
}

void CFlexBridge::AddInitializationCallback(CASWork* pWork)
{
    m_vInitCallbacks.push_back(pWork);
}

void CFlexBridge::Initialized()
{
    for ( unsigned int i = 0; i < m_vInitCallbacks.size(); i++ )
    {
        m_vInitCallbacks[i]->Run();
        delete m_vInitCallbacks[i];
    }

    m_vInitCallbacks.clear();
}

CASObject CFlexBridge::Create(const char* sClassName)
{
    CASObject oObjOut;
    std::vector<CASObject> vArgs;
    vArgs.push_back(CASObject(sClassName));
    CallASFunction(string("create"), &vArgs, oObjOut);
    return oObjOut;
}

CASObject CFlexBridge::ClassRef(const char* sClassName)
{
    CASObject oObjOut;
    std::vector<CASObject> vArgs;
    vArgs.push_back(CASObject(sClassName));
    CallASFunction(string("classRef"), &vArgs, oObjOut);
    return oObjOut;
}

CCPPInstance CFlexBridge::RegisterLocalFunction(void (*pFunc)(CASObject& , CFlexBridge*))
{
    
    DWORD dwResult;
    
    dwResult = WaitForSingleObject(g_hLocalFnMapMutex, INFINITE);
    CCPPInstance pInst;

    if ( dwResult != WAIT_OBJECT_0 )
    {
        LOG("ERROR: Mutex lock failed!");
        return pInst;
    }

    
    pInst.m_uiBridgeID = CBridgeManager::GetIDBridgePtr(this);

    std::ostringstream s;
    s << CCPPInstance::m_uiCounter++;
    pInst.m_sID = s.str();

    m_LocalFunctionMap.insert(std::make_pair(pInst.m_sID, pFunc));

    ReleaseMutex(g_hLocalFnMapMutex);

    return pInst;
}

void CFlexBridge::CallLocalFunction(const CASObject& oArgs)
{
    CASValue oVal;
    oArgs.GetValue(oVal);
    
    if ( oVal.m_aVal && oVal.m_aVal->size() > 0 )
    {
        CASObject oID = (*oVal.m_aVal)[0];
        CASValue oIDVal;
        string sID;

        oID.GetValue(oIDVal);
        std::ostringstream oss;
        oss << (int)(*oIDVal.m_nVal);
        sID = oss.str();
        LOGDEBUG("No Val is " << *oIDVal.m_nVal);
        std::map <string, void (*)(CASObject& , CFlexBridge*)>::iterator it = m_LocalFunctionMap.find(sID);
        
        if ( it != m_LocalFunctionMap.end() )
        {
            if ( oVal.m_aVal->size() > 1 )
            {
                CASObject oCallArg;
                Deserialize((*oVal.m_aVal)[1], oCallArg);
                (it->second)(oCallArg, this);
                return;
            }
        }
    }

    throw CBridgeException("Cannot find registered CPP function");
}

void CFlexBridge::GetProxyName(const CASProxy* pProxy, string& sTypeNameOut)
{
	std::map<string,CASProxy*>::iterator ii;
   for(ii=m_RemoteTypeMap.begin(); ii!=m_RemoteTypeMap.end(); ++ii)
   {
       CASProxy* pCache;
       pCache = (*ii).second;
       if ( pProxy == pCache )
       {
           sTypeNameOut = ii->first;
           return;
       }
   }
    
}
