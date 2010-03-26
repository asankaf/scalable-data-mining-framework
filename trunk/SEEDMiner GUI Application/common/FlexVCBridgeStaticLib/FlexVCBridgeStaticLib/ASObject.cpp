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

#include "ASObject.h"
#include "Logger.h"
#include "FlexBridge.h"
#include "BridgeManager.h"
#include "BridgeException.h"

/* for variable args support */
#include <cstdarg>

void CASObject::Init()
{
    m_pValue = NULL;
    m_Type = ASOBJECT_NONE;
}

/* Constructors */

CASObject::CASObject(void)
{
    Init();
}

CASObject::CASObject(const CASObject& obj)
{
    Init();
    Set(obj.GetType(), obj.GetValuePtr());
}

CASObject::CASObject(const std::string& val)
{
    Init();
    SetString(val);
}


CASObject::CASObject(const char* val)
{
    Init();
    SetString(val);
}

CASObject::CASObject(const bool& val)
{
    Init();
    SetBoolean(val);
}

CASObject::CASObject(const int& val)
{
    Init();
    SetInt(val);
}

CASObject::CASObject(const double& val)
{
    Init();
    SetNumber(val);
}

CASObject::CASObject(const std::vector<CASObject>& val)
{
    Init();
    SetArray(val);
}

CASObject::CASObject(const std::map<std::string, CASObject>& val)
{
    Init();
    SetObject(val);
}

CASObject::CASObject(const CASInstance& val)
{
    Init();
    SetASInstance(val);
}


CASObject::CASObject(const CCPPInstance& val)
{
    Init();
    SetCPPInstance(val);
}

CASObject::CASObject(void (*pVal)(CASObject& , CFlexBridge*))
{
    Init();
    CCPPInstance oInst;
    oInst.m_pFn = (void (*)(void*, void*))pVal;
    SetCPPInstance(oInst);
}

/* Destructor */

CASObject::~CASObject(void)
{
    Cleanup();
}


void CASObject::Cleanup()
{
    std::string* ptrStr = NULL;
    int* ptrInt = NULL;
    double* ptrDouble = NULL;
    std::vector<CASObject>* ptrArr = NULL;
    std::map<std::string, CASObject>* ptrObj = NULL;
    bool* ptrBool = NULL;
    CASInstance* ptrInst = NULL;
    CCPPInstance* ptrCpp = NULL;
    
    if ( m_pValue )
    {
        /* typecast to correct type and delete */
        switch ( m_Type )
        {
            case ASOBJECT_STRING:
                
                ptrStr = (std::string*)m_pValue;
                
                if ( ptrStr != NULL )
                {
                    delete ptrStr;
                }
                
                break;
                
            case ASOBJECT_INT:
                
                ptrInt = (int*)m_pValue;
                
                if ( ptrInt != NULL )
                {
                    delete ptrInt;
                }
                
                break;
                
            case ASOBJECT_NUMBER:
                
                ptrDouble = (double*)m_pValue;
                
                if ( ptrDouble != NULL )
                {
                    delete ptrDouble;
                }
                
                break;
                
            case ASOBJECT_ARRAY:
                
                ptrArr = (std::vector<CASObject>*)m_pValue;
                
                if ( ptrArr != NULL )
                {
                    delete ptrArr;
                }
                
                break;
                
            case ASOBJECT_OBJECT:
                
                ptrObj = (std::map<std::string, CASObject>*)m_pValue;
                
                if ( ptrObj != NULL )
                {
                    delete ptrObj;
                }
                
                break;
                
            case ASOBJECT_BOOLEAN:
                
                ptrBool = (bool*)m_pValue;
                
                if ( ptrBool != NULL )
                {
                    delete ptrBool;
                }
                
                break;
                
            case ASOBJECT_INSTANCE:
                
                ptrInst = (CASInstance*)m_pValue;
                
                if ( ptrInst != NULL )
                {
                    delete ptrInst;
                }
                
                break;

            case ASOBJECT_LOCALINSTANCE:
                
                ptrCpp = (CCPPInstance*)m_pValue;
                
                if ( ptrCpp != NULL )
                {
                    delete ptrCpp;
                }
                
                break;
        }
        
    }
    
    Init();
    
}

/* Data setters */

void CASObject::SetString(const std::string& val)
{
    Cleanup();
    std::string* ptrStr = new std::string(val);
    m_pValue = (void*)ptrStr;
    m_Type = ASOBJECT_STRING;
}

void CASObject::SetString(const char* val)
{
    Cleanup();
    std::string* ptrStr = new std::string(val);
    m_pValue = (void*)ptrStr;
    m_Type = ASOBJECT_STRING;
}

void CASObject::SetInt(const int& val)
{
    Cleanup();
    int* ptrStr = new int(val);
    m_pValue = (void*)ptrStr;
    m_Type = ASOBJECT_INT;
    
}

void CASObject::SetNumber(const double& val)
{
    Cleanup();
    double* ptrStr = new double(val);
    m_pValue = (void*)ptrStr;
    m_Type = ASOBJECT_NUMBER;
    
}

void CASObject::SetBoolean(const bool& val)
{
    Cleanup();
    bool* ptrBool = new bool(val);
    m_pValue = (void*)ptrBool;
    m_Type = ASOBJECT_BOOLEAN;   
}

void CASObject::SetArray(const std::vector<CASObject>& val)
{
    Cleanup();
    std::vector<CASObject>* ptrStr = new std::vector<CASObject>(val);
    m_pValue = (void*)ptrStr;
    m_Type = ASOBJECT_ARRAY;
    
}

void CASObject::SetObject(const std::map<std::string, CASObject>& val)
{
    Cleanup();
    std::map<std::string, CASObject>* ptrStr = new std::map<std::string, CASObject>(val);
    m_pValue = (void*)ptrStr;
    m_Type = ASOBJECT_OBJECT;
}

void CASObject::SetASInstance(const CASInstance& val)
{
    Cleanup();
    CASInstance* ptrInst = new CASInstance(val);
    m_pValue = (void*)ptrInst;
    m_Type = ASOBJECT_INSTANCE;
    
}

void CASObject::SetCPPInstance(const CCPPInstance& val)
{
    Cleanup();
    CCPPInstance* ptrInst = new CCPPInstance(val);
    m_pValue = (void*)ptrInst;
    m_Type = ASOBJECT_LOCALINSTANCE;    
}

void CASObject::Set(ASObjectType type, void* pVal)
{    
    Cleanup();
    m_Type = type;
    
    std::string* ptrStr = NULL;
    int* ptrInt = NULL;
    double* ptrNum = NULL;
    std::vector<CASObject>* ptrArr = NULL;
    std::map<std::string, CASObject>* ptrObj = NULL;
    bool* ptrBool = NULL;
    CASInstance* ptrInst = NULL;
    CCPPInstance* ptrCppInst = NULL;
    
    switch ( type )
    {
        case ASOBJECT_STRING:
            
            ptrStr = new std::string(*((std::string*)pVal));
            m_pValue = (void*)ptrStr;
            break;
            
        case ASOBJECT_INT:
            
            ptrInt = new int(*((int*)pVal));
            m_pValue = (void*)ptrInt;
            break;
            
        case ASOBJECT_NUMBER:
            
            ptrNum = new double(*((double*)pVal));
            m_pValue = (void*)ptrNum;
            break;
            
        case ASOBJECT_BOOLEAN:
            
            ptrBool = new bool(*((bool*)pVal));
            m_pValue = (void*)ptrBool;
            break;
            
        case ASOBJECT_ARRAY:
            
            ptrArr = new std::vector<CASObject>(*((std::vector<CASObject>*)pVal));
            m_pValue = (void*)ptrArr;
            break;
            
        case ASOBJECT_OBJECT:
            
            ptrObj = new std::map<std::string, CASObject>(*((std::map<std::string, CASObject>*)pVal));
            m_pValue = (void*)ptrObj;
            break;
            
        case ASOBJECT_INSTANCE:
            
            ptrInst = new CASInstance(*((CASInstance*)pVal));
            m_pValue = (void*)ptrInst;
            break;

        case ASOBJECT_LOCALINSTANCE:
            
            ptrCppInst = new CCPPInstance(*((CCPPInstance*)pVal));
            m_pValue = (void*)ptrCppInst;
            break;
            
            
        case ASOBJECT_NONE:
            m_pValue = NULL;
            break;
    }
}

/* Operator overloads using the above setter functions */

CASObject& CASObject::operator=(const CASObject& obj)
{
    this->Set(obj.GetType(), obj.GetValuePtr());
    return *this;
}

CASObject& CASObject::operator=(const char* str)
{
    this->SetString(std::string(str));
    return *this;
}

CASObject& CASObject::operator=(const std::string& str)
{
    this->SetString(str);
    return *this;
}


CASObject& CASObject::operator=(const int& val)
{
    this->SetInt(val);
    return *this;
}

CASObject& CASObject::operator=(const double& val)
{
    this->SetNumber(val);
    return *this;
}


CASObject& CASObject::operator=(const bool& val)
{
    this->SetBoolean(val);
    return *this;
}


CASObject& CASObject::operator=(const std::vector<CASObject>& val)
{
    this->SetArray(val);
    return *this;
}


CASObject& CASObject::operator=(const std::map<std::string, CASObject>& val)
{
    this->SetObject(val);
    return *this;
}


CASObject& CASObject::operator=(const CASInstance& val)
{
    this->SetASInstance(val);
    return *this;
}

CASObject& CASObject::operator=(const CCPPInstance& val)
{
    this->SetCPPInstance(val);
    return *this;
}

CASObject& CASObject::operator=(void (*pVal)(CASObject& , CFlexBridge*))
{
    CCPPInstance oInst;
    oInst.m_pFn = (void (*)(void*,void*))pVal;
    this->SetCPPInstance(oInst);
    return *this;
}

/* Getters */

ASObjectType CASObject::GetType() const
{
    return m_Type;
}


void CASObject::GetValue(CASValue& oVal) const
{
    oVal.Set(m_Type, m_pValue);
}


void* CASObject::GetValuePtr() const
{
    return m_pValue;
}

CASInstance* CASObject::GetInstance() const
{
    if ( m_Type == ASOBJECT_INSTANCE )
    {
        return (CASInstance*)m_pValue;
    }
    return NULL;
}

CCPPInstance* CASObject::GetCPPInstance() const
{
    if ( m_Type == ASOBJECT_LOCALINSTANCE )
    {
        return (CCPPInstance*)m_pValue;
    }
    return NULL;
}


std::map<std::string, CASObject>& CASObject::GetObjectMap()
{
    if ( m_Type == ASOBJECT_OBJECT )
    {
        return *((std::map<std::string, CASObject>*)m_pValue);
    }
    else
    {
        throw CBridgeException("ASObject has no object map!");
    }
}

/* To call into AS */

CASObject CASObject::CallArg(const char* sFn, const std::vector<CASObject>* pArgs) const
{
    CASObject oResObj;
    CASInstance* pInst = GetInstance();
    
    if ( pInst )
    {
        CFlexBridge* pBridge = NULL;
        pBridge = (CFlexBridge*)CBridgeManager::GetBridgePtr(pInst->m_uiBridgeID);
        if ( pBridge )
        {
            if ( pArgs )
                pBridge->Call(*this, sFn, *pArgs, oResObj);
            else
                pBridge->Call(*this, sFn, oResObj);
        }
    }
    else
    {
        throw CBridgeException("ASObject not an AS instance to call method");
    }

    return oResObj;
}

CASObject CASObject::Call(const char* sFn) const
{
    CASObject oResObj;
    CASInstance* pInst = GetInstance();
    
    if ( pInst )
    {
        CFlexBridge* pBridge = NULL;
        pBridge = (CFlexBridge*)CBridgeManager::GetBridgePtr(pInst->m_uiBridgeID);
        pBridge->Call(*this, sFn, oResObj);
    }
    else
    {
        throw CBridgeException("ASObject not an AS instance to call method");
    }

    return oResObj;
}

CASObject CASObject::CallEx(const char* sFn, ...) const
{
    
    CASObject oResObj;
    
    CASInstance* pInst = GetInstance();
    
    if ( pInst )
    {
        CFlexBridge* pBridge = NULL;
        pBridge = (CFlexBridge*)CBridgeManager::GetBridgePtr(pInst->m_uiBridgeID);
        if ( pBridge )
        {
            va_list vl;
            
            va_start( vl, sFn );
            
            CASObject* pArg = va_arg( vl, CASObject* );
            std::vector<CASObject> vArgs;
            
            for ( ; pArg ; pArg = va_arg( vl, CASObject* ) )
            {
                vArgs.push_back(*pArg);
            }
            
            va_end(vl);
            
            if ( vArgs.size() > 0 )
                pBridge->Call(*this, sFn, vArgs, oResObj);
            else
                pBridge->Call(*this, sFn, oResObj);
        }
    }
    else
    {
        throw CBridgeException("ASObject not an AS instance to call method");
    }

    
    return oResObj;    
}

/* AS method call helpers */

CASObject CASObject::Call(const char* sFn, CASObject oArg1) const
{
    CASObject oResObj;
    CallExConservative(oResObj, sFn, &oArg1, NULL);
    return oResObj;
}

CASObject CASObject::Call(const char* sFn,
                          CASObject oArg1,
                          CASObject oArg2) const
{
    CASObject oResObj;
    CallExConservative(oResObj, sFn, &oArg1, &oArg2, NULL);
    return oResObj;
}


CASObject CASObject::Call(const char* sFn,
                          CASObject oArg1,
                          CASObject oArg2,
                          CASObject oArg3) const
{
    CASObject oResObj;
    CallExConservative(oResObj, sFn, &oArg1, &oArg2, &oArg3, NULL);
    return oResObj;
}

CASObject CASObject::Call(const char* sFn,
                          CASObject oArg1,
                          CASObject oArg2,
                          CASObject oArg3,
                          CASObject oArg4) const
{
    CASObject oResObj;
    CallExConservative(oResObj, sFn, &oArg1, &oArg2, &oArg3, &oArg4, NULL);
    return oResObj;
}

CASObject CASObject::Call(const char* sFn,
                          CASObject oArg1,
                          CASObject oArg2,
                          CASObject oArg3,
                          CASObject oArg4,
                          CASObject oArg5) const
{
    CASObject oResObj;
    CallExConservative(oResObj, sFn, &oArg1, &oArg2, &oArg3, &oArg4, &oArg5, NULL);
    return oResObj;
}


void CASObject::CallExConservative(CASObject& oOutObj, const char* sFn, ...) const
{
    
    CASInstance* pInst = GetInstance();
    
    if ( pInst )
    {
        CFlexBridge* pBridge = NULL;
        pBridge = (CFlexBridge*)CBridgeManager::GetBridgePtr(pInst->m_uiBridgeID);
        if ( pBridge )
        {
            va_list vl;
            
            va_start( vl, sFn );
            
            CASObject* pArg = va_arg( vl, CASObject* );
            std::vector<CASObject> vArgs;
            
            for ( ; pArg ; pArg = va_arg( vl, CASObject* ) )
            {
                vArgs.push_back(*pArg);
            }
            
            va_end(vl);
            
            if ( vArgs.size() > 0 )
                pBridge->Call(*this, sFn, vArgs, oOutObj);
            else
                pBridge->Call(*this, sFn, oOutObj);
        }
    }
    else
    {
        throw CBridgeException("ASObject not an AS instance to call method");
    }

}

/* For Logging */

void CASObject::Dump(const char* sPrefix) const
{
    char* sPrintPrefix = NULL;
    
    if ( sPrefix == NULL )
        sPrintPrefix = "- ";
    else
        sPrintPrefix = (char*)sPrefix;
    
    
    std::string* ptrStr = NULL;
    int* ptrInt = NULL;
    double* ptrDouble = NULL;
    std::vector<CASObject>* ptrArr = NULL;
    std::map<std::string, CASObject>* ptrObj = NULL;
    bool* ptrBool = NULL;
    CASInstance* ptrInst = NULL;
    CCPPInstance* ptrCpp = NULL;
    
    LOG(sPrintPrefix << "ASObject " << this <<  " Info:" );
    
    switch ( m_Type )
    {
        case ASOBJECT_STRING:
            
            ptrStr = (std::string*)m_pValue;
            
            if ( ptrStr != NULL )
            {
                LOG( sPrintPrefix << "String content: " << *ptrStr);
            }
            
            break;
            
        case ASOBJECT_INT:
            
            ptrInt = (int*)m_pValue;
            
            if ( ptrInt != NULL )
            {
                LOG( sPrintPrefix << "Int content: " << *ptrInt);
            }
            
            break;
            
        case ASOBJECT_NUMBER:
            
            ptrDouble = (double*)m_pValue;
            
            if ( ptrDouble != NULL )
            {
                LOG( sPrintPrefix << "Number content: " << *ptrDouble);
            }
            
            break;
            
        case ASOBJECT_ARRAY:
            
            ptrArr = (std::vector<CASObject>*)m_pValue;
            
            if ( ptrArr != NULL )
            {
                LOG( sPrintPrefix << "Array size: " << ptrArr->size());
                LOG( sPrintPrefix << "Array contents: ");
                std::string sPrompt = sPrintPrefix;
                sPrompt += "-> ";
                for ( unsigned int i = 0; i < ptrArr->size(); i++ )
                {
                    (*ptrArr)[i].Dump(sPrompt.c_str());
                }
            }
            
            break;
            
        case ASOBJECT_OBJECT:
            
            ptrObj = (std::map<std::string, CASObject>*)m_pValue;
            
            if ( ptrObj != NULL )
            {
                LOG( sPrintPrefix << "Map size: " << ptrObj->size());
                std::string sPrompt = sPrintPrefix;
                sPrompt += "-> ";
                
                for( std::map<std::string, CASObject>::iterator ii=ptrObj->begin(); ii!=ptrObj->end(); ++ii)
                {
                    LOG( sPrintPrefix << "Property name: " << (*ii).first << " ,Val: " );
                    (*ii).second.Dump(sPrompt.c_str());
                }
            }
            
            break;
            
        case ASOBJECT_BOOLEAN:
            
            ptrBool = (bool*)m_pValue;
            
            if ( ptrBool != NULL )
            {
                LOG( sPrintPrefix << "Bool val: " << *ptrBool);
            }
            
            break;
            
        case ASOBJECT_INSTANCE:
            
            ptrInst = (CASInstance*)m_pValue;
            
            if ( ptrInst != NULL )
            {
                LOG( sPrintPrefix << "AS Instance val: " << ptrInst->m_sID);
            }
            
            break;

        case ASOBJECT_LOCALINSTANCE:
            
            ptrCpp = (CCPPInstance*)m_pValue;
            
            if ( ptrCpp != NULL )
            {
                LOG( sPrintPrefix << "Local Instance val: " << ptrCpp->m_sID);
            }
            
            break;
            
        case ASOBJECT_NONE:
            
            LOG( sPrintPrefix << "Null object");
            
            break;
            
    }
    
}
