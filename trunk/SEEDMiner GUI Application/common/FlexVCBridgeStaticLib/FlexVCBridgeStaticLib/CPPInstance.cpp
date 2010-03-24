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

#include "CPPInstance.h"
#include <sstream>

unsigned int CCPPInstance::m_uiCounter = 0;

CCPPInstance::CCPPInstance(void)
{
    /* std::ostringstream s; */
    /* s << m_uiCounter++; */
    /* m_sID = s.str(); */

    m_uiBridgeID = 0;
    m_Type = CPPINSTANCE_FUNCTION;
    m_pFn = NULL;
}

CCPPInstance::CCPPInstance(const CCPPInstance& obj)
{
    m_sID = obj.m_sID;
    m_uiBridgeID = obj.m_uiBridgeID;
    m_Type = obj.m_Type;
    m_pFn = obj.m_pFn;
}

CCPPInstance::~CCPPInstance(void)
{
}
