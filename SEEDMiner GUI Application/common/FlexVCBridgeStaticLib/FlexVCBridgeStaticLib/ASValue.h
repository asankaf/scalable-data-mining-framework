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

#include <string>
#include <map>
#include <vector>

/* The various types that an ASObject can be of */
enum ASObjectType { ASOBJECT_NONE, ASOBJECT_STRING, ASOBJECT_INT, 
                    ASOBJECT_NUMBER, ASOBJECT_ARRAY, ASOBJECT_OBJECT, 
                    ASOBJECT_BOOLEAN, ASOBJECT_INSTANCE, ASOBJECT_LOCALINSTANCE, 
                    ASOBJECT_NULL };

class CASObject;

/* This never actually allocates or dellocs memory. It's just for
 * convenience. But it doesn't do an instance (local and AS) because
 * there is default support for that in ASObject. There's also support
 * for an ASOBJECT_OBJECT in ASObject.*/
class CASValue
{
  private:


  public:

    /* type of obj */
    ASObjectType m_Type;

    /* proper pointer refs to val. One of these will be non-null
     * according to m_Type */
    std::string* m_sVal;
    int* m_iVal;
    double* m_nVal;
    bool* m_bVal;
    std::vector<CASObject>* m_aVal;
    std::map<std::string, CASObject>* m_oVal;

    CASValue(void);
    ~CASValue(void);

    /* Don't use this. Instead use CASObject's GetValue() method
     * (which internally uses this) */
    void Set(ASObjectType type, void* pVal);

    
};
