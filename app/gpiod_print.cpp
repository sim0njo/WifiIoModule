
//----------------------------------------------------------------------------
// cgpiod_print.hpp : command printer
//
// Copyright (c) Jo Simons, 2015-2016, All Rights Reserved.
//----------------------------------------------------------------------------
#include <gpiod.h>

//----------------------------------------------------------------------------
// print obj name or obj-id in hex if not found
//----------------------------------------------------------------------------
tCChar* CGpiod::PrintObj2String(tChar* pOut, tUint32 dwObj) 
{
  tUint32    dwErr = XERROR_SUCCESS;
  tParseRsvd *pRsvd = g_gpiodParseObj;

  for ( ; pRsvd->dwMask0; pRsvd++)
    if ( (pRsvd->dwTVal & 0x0000FFFF) == (dwObj & 0x0000FFFF) ) { 
      gstrcpy(pOut, pRsvd->szTVal);
      return pOut; 
      } //

  gsprintf(pOut, "%08X", dwObj);
  return pOut; 
  } // PrintObj2String

//----------------------------------------------------------------------------
// convert object status to string
//----------------------------------------------------------------------------
tCChar* CGpiod::PrintObjSta2String(tChar* pOut, tUint32 dwObj, tUint32 dwSta) 
{
  tParseRsvd *pRsvd = g_gpiodParseObjSta;

  for ( ; pRsvd->dwMask0; pRsvd++)
    if ( (pRsvd->dwMask0 &   dwObj) && (pRsvd->dwTVal  ==  dwSta) ) { 
      gstrcpy(pOut, pRsvd->szTVal);
      return pOut; 
      } // if

  gsprintf(pOut, "%08X", dwSta);
  return pOut; 
  } // PrintObjSta2String

//----------------------------------------------------------------------------
// convert object event to string
//----------------------------------------------------------------------------
tCChar* CGpiod::PrintObjEvt2String(tChar* pOut, tUint32 dwObj, tUint32 dwEvt) 
{
  tParseRsvd *pRsvd = g_gpiodParseObjEvt;

  for ( ; pRsvd->dwMask0; pRsvd++)
    if ( (pRsvd->dwMask0 &   dwObj) && (pRsvd->dwTVal  ==  dwEvt) ) { 
      gstrcpy(pOut, pRsvd->szTVal);
      return pOut; 
      } // if

  gsprintf(pOut, "%u", dwEvt);
  return pOut; 
  } // PrintObjEvt2String

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
tCChar* CGpiod::PrintObjCmd2String(tChar* pOut, tUint32 dwObj, tUint32 dwCmd) 
{
  tUint32    dwErr  = XERROR_SUCCESS;
  tParseRsvd *pRsvd = (dwObj & CGPIOD_OBJ_CLS_INPUT)   ? g_gpiodParseCmdInput   :
                      (dwObj & CGPIOD_OBJ_CLS_OUTPUT)  ? g_gpiodParseCmdOutput  :
                      (dwObj & CGPIOD_OBJ_CLS_SHUTTER) ? g_gpiodParseCmdShutter : g_gpiodParseCmdInput;

  for ( ; pRsvd->dwMask0; pRsvd++)
    if ( (pRsvd->dwTVal  & CGPIOD_CMD_NUM_MASK) == (dwCmd & CGPIOD_CMD_NUM_MASK) ) {
      gstrcpy(pOut, pRsvd->szTVal);
      return pOut; 
      } // if

  gsprintf(pOut, "%08X", dwCmd);
  return pOut; 
  } // PrintObjCmd2String

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
tCChar* CGpiod::PrintVal2String(tChar* pOut, tUint32 dwVal) 
{
  gsprintf(pOut, "%u", dwVal);
  return pOut;
  } // PrintVal2String

//----------------------------------------------------------------------------
// print all parameters to output buffer
//----------------------------------------------------------------------------
tCChar* CGpiod::PrintCmdParamVals(tChar* pOut, tUint32 cbOut, tGpiodCmd* pCmd) 
{
  tChar str[32];

  do {
    *pOut = '\0';

    if (pCmd->dwParms == 0)
      break;

    switch (pCmd->dwObj & CGPIOD_OBJ_CLS_MASK) {
      case CGPIOD_OBJ_CLS_INPUT:   
        if (pCmd->dwParms & CGPIOD_IN_PRM_DEBOUNCE) { // debounce
          gsprintf(str, ".%u", pCmd->parmsInput.dwDebounce);
          xstrcatn(pOut, cbOut, str, 0);
          } // if

        break;

      case CGPIOD_OBJ_CLS_OUTPUT:  
//    case CGPIOD_OBJ_CLS_TIMER:  
        if (pCmd->dwParms & CGPIOD_OUT_PRM_LOCK) { 
          gsprintf(str, ".%u", pCmd->parmsOutput.dwLock);
          xstrcatn(pOut, cbOut, str, 0);
          } // if

        if (pCmd->dwParms & CGPIOD_OUT_PRM_DELAY) { 
          gsprintf(str, ".%u", pCmd->parmsOutput.dwDelay);
          xstrcatn(pOut, cbOut, str, 0);
          } // if

        if (pCmd->dwParms & CGPIOD_OUT_PRM_RUN) { 
          gsprintf(str, ".%u", pCmd->parmsOutput.dwRun);
          xstrcatn(pOut, cbOut, str, 0);
          } // if

        if (pCmd->dwParms & CGPIOD_OUT_PRM_OLOCK) {
          gsprintf(str, ".%u", pCmd->parmsOutput.dwLock);
          xstrcatn(pOut, cbOut, str, 0);
          } // if

        if (pCmd->dwParms & CGPIOD_OUT_PRM_DEFRUN) {
          gsprintf(str, ".%u", pCmd->parmsOutput.dwRun);
          xstrcatn(pOut, cbOut, str, 0);
          } // if

        break;

      case CGPIOD_OBJ_CLS_SHUTTER: 
        if (pCmd->dwParms & CGPIOD_SHU_PRM_PRIOMASK) { // prio-mask 0-63
          gsprintf(str, ".%u", pCmd->parmsShutter.dwPrioMask);
          xstrcatn(pOut, cbOut, str, 0);
          } // if

        if (pCmd->dwParms & CGPIOD_SHU_PRM_PRIOLEVEL) { // prio-level 0-5
          gsprintf(str, ".%u", pCmd->parmsShutter.dwPrioLvl);
          xstrcatn(pOut, cbOut, str, 0);
          } // if

        if (pCmd->dwParms & CGPIOD_SHU_PRM_PRIOLOCK) { // lock 0-1
          gsprintf(str, ".%u", pCmd->parmsShutter.dwLock);
          xstrcatn(pOut, cbOut, str, 0);
          } // if

        if (pCmd->dwParms & CGPIOD_SHU_PRM_DELAY) { // delay 0-3600 seconds 
          gsprintf(str, ".%u", pCmd->parmsShutter.dwDelay);
          xstrcatn(pOut, cbOut, str, 0);
          } // if

        if (pCmd->dwParms & CGPIOD_SHU_PRM_RUN) { // runtime 1-3600 seconds 
          gsprintf(str, ".%u", pCmd->parmsShutter.dwRun);
          xstrcatn(pOut, cbOut, str, 0);
          } // if

        if (pCmd->dwParms & CGPIOD_SHU_PRM_TIP0) { // tiptime 0-3600 1/10th seconds
          gsprintf(str, ".%u", pCmd->parmsShutter.dwTip);
          xstrcatn(pOut, cbOut, str, 0);
          } // if

        if (pCmd->dwParms & CGPIOD_SHU_PRM_TIP1) { // tiptime 1-3600 1/10th seconds
          gsprintf(str, ".%u", pCmd->parmsShutter.dwTip);
          xstrcatn(pOut, cbOut, str, 0);
          } // if

        if (pCmd->dwParms & CGPIOD_SHU_PRM_DEFRUN) {
          gsprintf(str, ".%u", pCmd->parmsShutter.dwRun);
          xstrcatn(pOut, cbOut, str, 0);
          } // if

        break;

//    case CGPIOD_OBJ_CLS_DIMMER:  
//      if (pCmd->dwCmd & CGPIOD_DIM_PRM_LEVEL) { // level 0-255 for dimmer
//        gsprintf(str, ".%u", pCmd->dwLevel);
//        xstrcatn(pOut, cbOut, str, 0);
//        } // if

//      if (pCmd->dwCmd & CGPIOD_DIM_PRM_RUN) { // runtime 1-160 seconds
//        gsprintf(str, ".%u", pCmd->dwRun);
//        xstrcatn(pOut, cbOut, str, 0);
//        } // if

//      break;

      case CGPIOD_OBJ_CLS_SYSTEM:  
        if (pCmd->dwParms & CGPIOD_SYS_PRM_LOGLEVEL) { // loglevel
          gsprintf(str, ".0x%08X", pCmd->parmsSystem.dwParm);
          xstrcatn(pOut, cbOut, str, 0);
          } // if

        if (pCmd->dwParms & CGPIOD_SYS_PRM_EMUL) { // emul
          gsprintf(str, ".%u", pCmd->parmsSystem.dwParm);
          xstrcatn(pOut, cbOut, str, 0);
          } // if

        if (pCmd->dwParms & CGPIOD_SYS_PRM_MODE) { // mode
          gsprintf(str, ".%u", pCmd->parmsSystem.dwParm);
          xstrcatn(pOut, cbOut, str, 0);
          } // if

        if (pCmd->dwParms & CGPIOD_SYS_PRM_OFFON) { // 0|1
          gsprintf(str, ".%u", pCmd->parmsSystem.dwParm);
          xstrcatn(pOut, cbOut, str, 0);
          } // if

        if (pCmd->dwParms & CGPIOD_SYS_PRM_ACK) { // ack
          gsprintf(str, ".ack");
          xstrcatn(pOut, cbOut, str, 0);
          } // if

        break;

      } // switch

    } while (FALSE);

    return pOut;
    } // _printCmdParams

