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

#include <vector>

/* Maps int ids to the correct shockwave objects ptr. */
class CFlashIDManager
{

  private:

    static std::vector<void*> m_vFlashPtrs;

  public:

    CFlashIDManager(void);
    ~CFlashIDManager(void);

    /* Adds a flash ptr and returns it's id */
    static int AddFlashPtr(void* ptr);

    /* Sets ptr to null in vector and returns it's index. returns -1
     * on error. */
    static int RemoveFlashPtr(void* ptr);

    /* Gets the ptr to the flash instance. Returns NULL if not found
     * or if marked as deleted */
    static void* GetFlashPtr(unsigned int iID);

    /* Returns the id of the ptr. */
    static int GetIDFlashPtr(void* ptr);

};
