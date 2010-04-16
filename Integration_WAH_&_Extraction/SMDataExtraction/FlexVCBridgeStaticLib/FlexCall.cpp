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

/* TODO: other functions passed to addcallback in FABridge.as */

#include <windows.h>
#include "FlexCall.h"
#include "Logger.h"
#include "tinyxml.h"
#include "FlexBridge.h"
#include "BridgeManager.h"
#include "BridgeException.h"
#include <sstream>

using std::string;
using std::map;
using std::stringstream;

map<string, void (*)(const std::vector<CASObject>& , int, string& )> CFlexCall::m_mFnMap;
void* CFlexCall::m_pDlg = NULL;


CFlexCall::CFlexCall(void)
{
}

CFlexCall::~CFlexCall(void)
{
}

void CFlexCall::Init(void* pDlg)
{
    // build fn map
    //CFlexCall::m_mFnMap["dummy"] = CFlexCall::Dummy;
    CFlexCall::m_mFnMap["FABridge__bridgeInitialized"]  = CFlexCall::FABridgeInitialized;
    CFlexCall::m_mFnMap["FABridge__invokeJSFunction"]  = CFlexCall::FABridgeInvokeLocalFunction;
    
    m_pDlg = pDlg;
}

void CFlexCall::Process(int iFlashID)
{
    LOGDEBUG("Processing fn " << m_sFnName << " with " << m_vArgs.size() << " args " );
    
    //map<string,void (*)(std::vector<string>&)>::iterator it;
    /* process call from flex */
    if ( m_mFnMap.find(m_sFnName) != m_mFnMap.end() )
    {
        string res = "";
        
        (m_mFnMap[m_sFnName])(m_vArgs, iFlashID, res);
        
        /* check if callback into flex is required */
        if ( res.length() > 0 )
        {
            /* send message to make call back into flex */
            string* sendmsg = new string(res);
            SendMessage((HWND)m_pDlg, WM_FLEXCALLBACK, (WPARAM)sendmsg, (LPARAM)iFlashID);
        }
        
    }
}

static void BuildXMLInternal(const CASObject& oObj,
                             TiXmlElement * arguments
                             );

static void BuildXMLObjectInstanceInternal(const CASObject& oObj,
                                           TiXmlElement * argelem)
{
    bool bAsInst = false;
    bool bCppInst = false;

    CASValue oValObj;
    oObj.GetValue(oValObj);

        
    /* check if this is an as instance or not */
    
    if ( oObj.GetInstance() )
    {
        bAsInst = true;
    }
    else if ( oObj.GetCPPInstance() )
    {
        bCppInst = true;
    }

    if ( bCppInst == true || bAsInst == true || oValObj.m_oVal )
    {
        TiXmlElement * arrayelem = NULL;
        TiXmlElement * objwrapelem = NULL;
        TiXmlElement * objwrapstrelem = NULL;
        TiXmlElement * objwraptypeelem = NULL;
        TiXmlText * objwraptypestr = NULL;
      
        /* Objects get packed as type,value pairs */
        
        objwrapelem = new TiXmlElement( "property" );
        objwrapelem->SetAttribute("id","type");
        objwrapstrelem = new TiXmlElement("number");
        
        stringstream s;
        if ( bAsInst == true )
        {
            s << CFlexBridge::TYPE_ASINSTANCE;
        }
        else if ( bCppInst == true )
        {
            s << CFlexBridge::TYPE_JSFUNCTION;
        }
        else
        {
            s << CFlexBridge::TYPE_ANONYMOUS;
        }
        
        objwraptypestr = new TiXmlText(s.str().c_str());    
        objwrapstrelem->LinkEndChild(objwraptypestr);
        objwrapelem->LinkEndChild(objwrapstrelem);
        argelem->LinkEndChild(objwrapelem);
        
        objwrapelem = new TiXmlElement( "property" );
        objwrapelem->SetAttribute("id","value");

        if ( bAsInst == true ||  bCppInst == true )
        {
            objwraptypeelem = new TiXmlElement( "number" );

            if ( bAsInst == true )
            {
                objwraptypestr = new TiXmlText(oObj.GetInstance()->m_sID.c_str());
            }
            else
            {
                if ( oObj.GetCPPInstance()->m_sID.length() > 0 )
                {
                    objwraptypestr = new TiXmlText(oObj.GetCPPInstance()->m_sID.c_str());
                }
                else
                {
                    LOG("Error: CPP Instance has not been assigned an ID, throwing exception");
                    throw CBridgeException("CPP Instance has no ID. If you declared CPP Instances, you have to register them with the bridge.");
                }
            }

            objwraptypeelem->LinkEndChild(objwraptypestr);
        }
        else
        {
            objwraptypeelem = new TiXmlElement( "object" );    
            
            for( std::map<std::string,CASObject>::iterator ii= oValObj.m_oVal->begin(); ii!=oValObj.m_oVal->end(); ++ii)
            {
                arrayelem = new TiXmlElement( "property" );
				arrayelem->SetAttribute("id",ii->first.c_str());
                BuildXMLInternal(ii->second, arrayelem);
                objwraptypeelem->LinkEndChild(arrayelem);
            } 
        }
        objwrapelem->LinkEndChild(objwraptypeelem);
        argelem->LinkEndChild(objwrapelem);               
    }
}

static void BuildXMLInternal(const CASObject& oObj,
                             TiXmlElement * arguments
                             )
{
    
    TiXmlElement * argelem = NULL;
    CASValue oVal;
    
    switch ( oObj.GetType() )
    {
        case ASOBJECT_STRING:
            argelem = new TiXmlElement( "string" );
            break;
            
        case ASOBJECT_NUMBER:
            argelem = new TiXmlElement( "number" );
            break;
            
        case ASOBJECT_BOOLEAN:
            
            oObj.GetValue(oVal);
            if ( *(oVal.m_bVal) == false )
                argelem = new TiXmlElement( "false");
            else                                                        
                argelem = new TiXmlElement( "true");
            break;
            
        case ASOBJECT_ARRAY:
            argelem = new TiXmlElement( "array" );
            break;

        case ASOBJECT_INSTANCE:
        case ASOBJECT_LOCALINSTANCE:
        case ASOBJECT_OBJECT:
            argelem = new TiXmlElement( "object" );
            break;


            
        case ASOBJECT_NULL:
            argelem = new TiXmlElement( "null");
            break;
            
        default:
            argelem = new TiXmlElement( "string" );
            break;
    }
    
    if ( oObj.GetType() == ASOBJECT_STRING || 
         oObj.GetType() == ASOBJECT_NUMBER || 
         oObj.GetType() == ASOBJECT_INT ||
         oObj.GetType() == ASOBJECT_ARRAY ||
         oObj.GetType() == ASOBJECT_OBJECT || 
         oObj.GetType() == ASOBJECT_INSTANCE ||
         oObj.GetType() == ASOBJECT_LOCALINSTANCE )
    {
        TiXmlText* text = NULL;
        CASValue oValObj;
        oObj.GetValue(oValObj);
        
        if ( oValObj.m_Type == ASOBJECT_STRING )
            text = new TiXmlText(oValObj.m_sVal->c_str());
        else if ( oValObj.m_Type == ASOBJECT_INT )
        {
            stringstream s;
            s << *oValObj.m_iVal;
            text = new TiXmlText(s.str().c_str());
        }
        else if ( oValObj.m_Type == ASOBJECT_NUMBER )
        {
            stringstream s;
            s << *oValObj.m_nVal;
            text = new TiXmlText(s.str().c_str());
        }
        else if ( oValObj.m_Type == ASOBJECT_ARRAY )
        {
            if ( oValObj.m_aVal )
            {
                TiXmlElement * arrayelem = NULL;
                for ( unsigned int i = 0; i < oValObj.m_aVal->size(); i++ )
                { 
                    stringstream s;
                    s << i;
                    
                    arrayelem = new TiXmlElement( "property" );
                    arrayelem->SetAttribute("id",s.str().c_str());
                    
                    BuildXMLInternal((*oValObj.m_aVal)[i], arrayelem);
                    argelem->LinkEndChild(arrayelem);
                }                
            }
        }
        else if ( oValObj.m_Type == ASOBJECT_OBJECT || oValObj.m_Type == ASOBJECT_INSTANCE || oValObj.m_Type == ASOBJECT_LOCALINSTANCE )
        {
            BuildXMLObjectInstanceInternal(oObj, argelem);
        }

        
        if ( text != NULL )
            argelem->LinkEndChild(text);
    }
    
    arguments->LinkEndChild(argelem);
    
}

void CFlexCall::BuildXML(const std::string& sFn, const std::vector<CASObject>& vArgs, std::string& out)
{
    TiXmlDocument doc;
    //TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    TiXmlElement * element = new TiXmlElement( "invoke" );
    element->SetAttribute("name",sFn.c_str());
    element->SetAttribute("returntype","xml");
    TiXmlElement * arguments = new TiXmlElement( "arguments" );
    //TiXmlText * text = new TiXmlText( "World" );
    element->LinkEndChild( arguments );
    
    for ( unsigned int i = 0; i < vArgs.size(); i++ )
    {
        BuildXMLInternal(vArgs[i], arguments);
    }
    //doc.LinkEndChild( decl );
    doc.LinkEndChild( element );
    
    TiXmlPrinter printer;
    printer.SetStreamPrinting();
    doc.Accept( &printer );
    
    out = printer.Str();
    //delete element;
}

void CFlexCall::BuildXML(const std::string& sFn, std::string& out)
{
    TiXmlDocument doc;
    //TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    TiXmlElement * element = new TiXmlElement( "invoke" );
    element->SetAttribute("name",sFn.c_str());
    element->SetAttribute("returntype","xml");
    TiXmlElement * arguments = new TiXmlElement( "arguments" );
    //TiXmlText * text = new TiXmlText( "World" );
    element->LinkEndChild( arguments );
    
    //doc.LinkEndChild( decl );
    doc.LinkEndChild( element );
    
    TiXmlPrinter printer;
    printer.SetStreamPrinting();
    doc.Accept( &printer );
    
    out = printer.Str();
    //delete element;
}

/* HANDLER FUNCTIONS */

/* void CFlexCall::Dummy(const std::vector<CASObject>& vArgs,  */
/*                       int iFlashID, */
/*                       string& res) */
/* { */
/*     LOGDEBUG("Dummy fn called"); */
/*     if ( vArgs.size() > 0 ) */
/*     { */
/*         std::vector<CASObject> vFlexArgs; */
        
/*         vFlexArgs.push_back(CASObject(std::string("asd"))); */
/*         vFlexArgs.push_back(CASObject(12)); */
        
        
/*         BuildXML(string("resfn"), vFlexArgs, res); */
/*         //res = "<invoke name=\""; */
/*         //res += vArgs[0]; */
/*         //res += "\"><arguments/></invoke>"; */
/*     } */
/*     else */
/*     { */
/*         LOGDEBUG("There's no args."); */
/*     } */
/* } */

void CFlexCall::FABridgeInitialized(const std::vector<CASObject>& vArgs, 
                                    int iFlashID,
                                    string& res)
{
    
    if ( vArgs.size() > 0 )
    {
        CASValue valobj, valobj1;
        vArgs[0].GetValue(valobj);
        std::vector<CASObject>* arr = valobj.m_aVal;
        if ( arr->size() > 0 )
        {
            (*arr)[0].GetValue(valobj1);
            string* bridgeName = valobj1.m_sVal;

            /* if you want to test if there is a problem when multiple
             * flash players are embedded, uncomment the line below to
             * activate bridge only on the first one. */
            //if ( iFlashID == 0 )
            //{
                CFlexBridge* oBridge = new CFlexBridge(*bridgeName, iFlashID);
                CBridgeManager::AddBridgePtr(oBridge);
                
                LOGDEBUG("Bridge Initialized " << *bridgeName << " with ID " << iFlashID);
                //}
        }                
    }
    else
    {
        LOG("There's no args.");
    }
}

void CFlexCall::FABridgeInvokeLocalFunction(const std::vector<CASObject>& vArgs, 
                                            int iFlashID,
                                            string& res)
{
    
    if ( vArgs.size() > 0 )
    {
        CASValue valobj, valobj1;
        vArgs[0].GetValue(valobj);
        std::vector<CASObject>* arr = valobj.m_aVal;
        CFlexBridge* oBridge = (CFlexBridge*)CBridgeManager::GetBridgePtrFlashID(iFlashID);

        if ( oBridge )
        {
            if ( vArgs.size() > 0 ) 
                oBridge->CallLocalFunction(vArgs[0]);
            
            LOGDEBUG("Local function call  with ID " << iFlashID);
        }
        
    }
    else
    {
        LOG("There's no args.");
    }
}
