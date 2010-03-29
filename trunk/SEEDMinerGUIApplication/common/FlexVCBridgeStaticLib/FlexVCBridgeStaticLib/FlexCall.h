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

/* TODO: Optimize AS object passing. Much like buck passing, we don't
 * care about the cost associated with object passing as of now. */

#pragma once

#include <string>
#include <vector>
#include <map>
#include "ASObject.h"

/* for our custom flex message */
#define WM_FLEXCALLBACK (WM_USER + 100)
#define WM_FLEXCALLBACK_RETURN (WM_USER + 102)

/* Handles an incoming function call from AS */
class CFlexCall
{
  private:

    /* maps strings to functions of the same class */
    static std::map<std::string, void (*)(const std::vector<CASObject>& , 
                                          int,
                                          std::string& )> m_mFnMap;
    
  public:

    /* pointer to parent dialog so that we can send the message */
    static void* m_pDlg;

    /* the function name */
    std::string m_sFnName;

    /* it's arguments */
    std::vector<CASObject> m_vArgs;
    
    CFlexCall(void);
    ~CFlexCall(void);

    /* initializes the fn call map and saves ptr to parent dialog */
    static void Init(void* pDlg);

    /* Builds reply XML in AS's invoke format */
    static void BuildXML(const std::string& sFn, 
                         const std::vector<CASObject>& vArgs,
                         std::string& out);

    /* Builds reply XML in AS's invoke format */
    static void BuildXML(const std::string& sFn, 
                         std::string& out);


  
    /* processes the current call by looking at the map */
    void Process(int iFlashID);

    /* Static functions that actually do whatever work is necessary */

    static void Dummy(const std::vector<CASObject>& vArgs, 
                      int iFlashID,
                      std::string& res);

    static void FABridgeInitialized(const std::vector<CASObject>& vArgs, 
                                    int iFlashID,
                                    std::string& res);

    static void FABridgeInvokeLocalFunction(const std::vector<CASObject>& vArgs, 
                                            int iFlashID,
                                            std::string& res);

};
