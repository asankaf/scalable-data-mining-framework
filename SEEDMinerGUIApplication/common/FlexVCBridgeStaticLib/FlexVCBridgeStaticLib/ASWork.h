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

/* Always practice safe Flex.
 * -- Doug McCune */

#pragma once

/* Inherit from this class to do your AS C++ coding. It has the
 * reference to the bridge member and the bridge's initialization
 * method automatically calls all CASWork object's Run() method once
 * it is initialized. Use bridge's addInitializationCallback to
 * register work objects  */

class CASWork
{ 

  protected:

	  
    void* m_pBridge;

    /* override this and perform your work here. Run checks if bridge
     * is present and calls worker. */
    virtual void Worker()=0;

  public:

    /* if this is not -1 this work will run only if the bridge's id
     * matches this one */
    int m_iForBridgeID;
    
    CASWork();
    CASWork(void* pBridge);
    ~CASWork(void);

    void SetBridge(void* pBridge);
    
    /* this returns NULL for ASWork, but all classes that inherit from
     * this must implement this and return a copy of itself so that
     * the bridgemanager can construct the appropriate work objects
     * for each bridge */
    virtual CASWork* Clone();

    void Run();
};
