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

#include "ASValue.h"
#include "ASInstance.h"
#include "CPPInstance.h"

/* Ick. There goes the class design. */
class CFlexBridge;

/* This is our secret weapon. It's ammo is a ton of copy constructors
 * and operator overloads so that argument passing to Call() looks
 * pretty natural. Use it well. */
class CASObject
{
    
  private:
    
    /* can point to anything */
    void* m_pValue;
    
    ASObjectType m_Type;

    inline void Init();
        
    /* De-alloc according to type. You'll see crashes in this if you
     * don't know how to code. (It's actually because my code is
     * buggy) */
    void Cleanup();
    
  public:
    
    CASObject(void);

    /* copy constructor */
    CASObject(const CASObject& obj);

    CASObject(const char* val);
    CASObject(const std::string& val);
    CASObject(const int& val);
    CASObject(const double& val);
    CASObject(const bool& val);
    CASObject(const std::vector<CASObject>& val);
    CASObject(const std::map<std::string, CASObject>& val);
    CASObject(const CASInstance& val);
    CASObject(const CCPPInstance& val);
    CASObject(void (*pVal)(CASObject& , CFlexBridge*));

    ~CASObject(void);

    /* Operator overloads help us forget that we are dealing with an
     * ASObject rather than strings, integers, arrays or maps. */
    CASObject& operator=(const CASObject& obj);
    CASObject& operator=(const char* str);
    CASObject& operator=(const std::string& str);
    CASObject& operator=(const int& val);
    CASObject& operator=(const double& val);
    CASObject& operator=(const bool& val);
    CASObject& operator=(const std::map<std::string, CASObject>& val);
    CASObject& operator=(const std::vector<CASObject>& val);
    CASObject& operator=(const CASInstance& val);    
    CASObject& operator=(const CCPPInstance& val);    
    CASObject& operator=(void (*pVal)(CASObject& , CFlexBridge*));
    
    void SetString(const char* val);
    void SetString(const std::string& val);
    void SetInt(const int& val);
    void SetNumber(const double& val);
    void SetArray(const std::vector<CASObject>& val);
    void SetObject(const std::map<std::string, CASObject>& val);
    void SetBoolean(const bool& val);
    void SetASInstance(const CASInstance& val);
    void SetCPPInstance(const CCPPInstance& val);
    
    void Set(ASObjectType type, void* pVal);

    ASObjectType GetType() const;
    void GetValue(CASValue& oVal) const;
    void* GetValuePtr() const;

    /* prints object contents to log file */
    void Dump(const char* sPrefix=NULL) const;

    /* returns null if not an asinstance. otherwise returns ptr to
     * asinstance obj */
    CASInstance* GetInstance() const;

    /* returns null if not an CPPInstance. otherwise returns ptr to
     * CPPInstance obj */
    CCPPInstance* GetCPPInstance() const;

    /* returns null if not an asinstance. otherwise returns ptr to
     * asinstance obj */
    std::map<std::string, CASObject>& GetObjectMap();

    /* Re-routes the call to the bridge's call method. This works only
     * if the CASObject is of type ASInstance. */
    CASObject CallArg(const char* sFn, const std::vector<CASObject>* pArgs) const;

    CASObject Call(const char* sFn) const;

    CASObject Call(const char* sFn, CASObject oArg1) const;
    CASObject Call(const char* sFn,
                   CASObject oArg1,
                   CASObject oArg2) const;
    
    CASObject Call(const char* sFn,
                   CASObject oArg1,
                   CASObject oArg2,
                   CASObject oArg3) const;
    CASObject Call(const char* sFn,
                   CASObject oArg1,
                   CASObject oArg2,
                   CASObject oArg3,
                   CASObject oArg4) const;
    CASObject Call(const char* sFn,
                   CASObject oArg1,
                   CASObject oArg2,
                   CASObject oArg3,
                   CASObject oArg4,
                   CASObject oArg5) const;

    /* takes any number of CASObject* after sFn. The last argument
     * however, MUST be NULL. Otherwise the results will be
     * unpredicatable. Internally uses CFlexBridge's call method. */
    CASObject CallEx(const char* sFn, ...) const;

    /* Same as CallEx but does not return a copy of the
     * object. Instead sets outobj to the appropriate value. Used so
     * that the overloaded Call's do not unnecessarily duplicate
     * objects. */
    void CallExConservative(CASObject& oOutObj, const char* sFn,  ...) const;

};

