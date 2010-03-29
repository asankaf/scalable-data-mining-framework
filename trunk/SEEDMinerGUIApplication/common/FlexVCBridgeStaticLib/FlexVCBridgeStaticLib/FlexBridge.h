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

/* "Let every man praise the bridge that carries him over"
 * -- English Proverb
*/

/*  ____    ___                     */
/* /\  _`\ /\_ \                    */
/* \ \ \L\_\//\ \      __   __  _   */
/*  \ \  _\/ \ \ \   /'__`\/\ \/'\  */
/*   \ \ \/   \_\ \_/\  __/\/>  </  */
/*    \ \_\   /\____\ \____\/\_/\_\ */
/*     \/_/   \/____/\/____/\//\/_/ */

#pragma once


#include <vector>
#include "ASObject.h"
#include "ASInstance.h"
#include "ASWork.h"

/* main bridge class */
class CFlexBridge
{
    
  private:

    /* name of the bridge */
    std::string m_sName;


    void CallASFunction(const std::string& sFn, 
                        const std::vector<CASObject>* vArgs, 
                        CASObject& outObj);

    std::map <std::string, CASProxy*> m_RemoteTypeMap;
    std::map <std::string, CASInstance*> m_RemoteInstanceMap;
    std::map <std::string, CASProxy*> m_RemoteFunctionMap;
    std::map <std::string, void (*)(CASObject& , CFlexBridge*)> m_LocalFunctionMap;

    std::vector <CASWork*> m_vInitCallbacks;

    /* For deserialization */

    void Deserialize(const CASObject& obj, CASObject& oOutObj);
    void AddTypeDataToCache(CASObject& newTypes);
    void CreateProxy(std::string& sType, CASObject& newTypes);
    void CreateProxies(CASObject& newTypes);

    CASInstance* GetInstance(std::string iID);
    bool CallInternal(const CASObject& oObjIn, const char* sFn, const std::vector<CASObject>* oArgsIn, CASObject& oObjOut);
    void GetProxyName(const CASProxy* pProxy, std::string& sTypeNameOut);

  public:

    /* id of the flash player instance */
    int m_iFlashID;

    static const short int TYPE_ASINSTANCE;
    static const short int TYPE_ASFUNCTION;
    
    static const short int TYPE_JSFUNCTION;
    static const short int TYPE_ANONYMOUS;

    CFlexBridge(const std::string& name, int iFlashID);
    ~CFlexBridge(void);

    CCPPInstance RegisterLocalFunction(void (*pFunc)(CASObject& , CFlexBridge*));

    /* JS function equivalence */

    void Root(CASObject& oRespObj);

    /* to create an as object */
    CASObject Create(const char* sClassName);

    /* to get ref to a static object */
    CASObject ClassRef(const char* sClassName);

    bool Call(const CASObject& oObjIn, const char* sFn, CASObject& oObjOut);
    bool Call(const CASObject& oObjIn, const char* sFn, const std::vector<CASObject>& oArgsIn, CASObject& oObjOut);


    void CallAS(const std::string& sFn, CASObject& outObj);
    void CallAS(const std::string& sFn, 
                const std::vector<CASObject>& vArgs, 
                CASObject& outObj);

    void AddInitializationCallback(CASWork* pWork);

    void Initialized();

    void CallLocalFunction(const CASObject& oArgs);
};
