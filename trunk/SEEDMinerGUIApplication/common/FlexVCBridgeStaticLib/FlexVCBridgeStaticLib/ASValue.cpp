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

#include "ASValue.h"

CASValue::CASValue(void)
{
    m_sVal = NULL;
    m_iVal = NULL;
    m_nVal = NULL;
    m_aVal = NULL;
    m_oVal = NULL;
}

CASValue::~CASValue(void)
{
}

void CASValue::Set(ASObjectType type, void* pVal)
{
    m_Type = type;
    
    switch ( type )
    {
        case ASOBJECT_STRING:
            
            m_sVal = ((std::string*)pVal);
            break;
            
        case ASOBJECT_INT:
            
            m_iVal = ((int*)pVal);
            break;
            
        case ASOBJECT_NUMBER:
            
            m_nVal = ((double*)pVal);
            break;
            
        case ASOBJECT_ARRAY:
            
            m_aVal = ((std::vector<CASObject>*)pVal);
            break;
            
        case ASOBJECT_OBJECT:
            
            m_oVal = ((std::map<std::string, CASObject>*)pVal);
            break;

      case ASOBJECT_BOOLEAN:
            
            m_bVal = ((bool*)pVal);
            break;


      case ASOBJECT_NULL:
            

            break;


            
    }
}
