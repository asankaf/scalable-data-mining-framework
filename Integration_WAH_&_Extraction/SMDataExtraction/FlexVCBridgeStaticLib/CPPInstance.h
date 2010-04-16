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

enum CPPInstanceType {CPPINSTANCE_FUNCTION, CPPINSTANCE_OBJECT};

/* Represents a function at the CPP end. In future this should
 * represent objects as well. */
class CCPPInstance
{
  public:

    static unsigned int m_uiCounter;

    std::string m_sID;
    unsigned int m_uiBridgeID;
    CPPInstanceType m_Type;

    void (*m_pFn)(void*,void*);

    CCPPInstance();
    CCPPInstance(const CCPPInstance& obj);
    ~CCPPInstance(void);
};
