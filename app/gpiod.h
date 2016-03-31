
//----------------------------------------------------------------------------
// cgpiod.hpp
//
// Copyright (c) Jo Simons, 2016-2016, All Rights Reserved.
//----------------------------------------------------------------------------
#ifndef __cgpiod_hpp__
#define __cgpiod_hpp__

#include <user_config.h>
#include <SmingCore/SmingCore.h>
//#include <controller.h>
#include <mqtt.h>
#include <stdafx.h>
#include <xerror.h>
#include <cparse.hpp>


extern tParseRsvd    g_gpiodParseObj[];
extern tParseRsvd    g_gpiodParseCmdOutput[];
extern tParseRsvd    g_gpiodParseCmdShutter[];
extern tParseRsvd    g_gpiodParseCmdSystem[];
extern tParseRsvd    g_gpiodParseCmdCounter[];


#define CGPIOD_VERSION                   "4.0.1.0" //                                   

#define CGPIOD_CMD_PFX                       "cmd" //
#define CGPIOD_EVT_PFX                       "evt" //

#define CGPIOD_MODE_OUTPUT                       0 // configured as out0-3
#define CGPIOD_MODE_SHUTTER                      1 // configured as udm0-1

//                                      0x0000CCNN //
#define CGPIOD_OBJ_NUM_MASK             0x000000FF //
#define CGPIOD_OBJ_CLS_MASK             0x0000FF00 //
#define CGPIOD_OBJ_CLS_INPUT            0x00000100 // input
#define CGPIOD_OBJ_CLS_OUTPUT           0x00000200 // output
#define CGPIOD_OBJ_CLS_SHUTTER          0x00000400 // shutter
#define CGPIOD_OBJ_CLS_SYSTEM           0x00000800 // system

#define CGPIOD_OBJ_NONE                 0x00000000 //
#define CGPIOD_OBJ_INPUT0               0x00000100 //
#define CGPIOD_OBJ_INPUT1               0x00000101 //
#define CGPIOD_OBJ_OUTPUT0              0x00000200 //
#define CGPIOD_OBJ_OUTPUT1              0x00000201 //
#define CGPIOD_OBJ_SHUTTER0             0x00000400 //
#define CGPIOD_OBJ_SYSTEM               0x00000800 //

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
#define CGPIOD_SYS_CMD_PING                      0 // 
#define CGPIOD_SYS_CMD_MODE                      1 // 
#define CGPIOD_SYS_CMD_DATE                      2 // 
#define CGPIOD_SYS_CMD_TIME                      3 // 
#define CGPIOD_SYS_CMD_VERSION                   4 // 

//----------------------------------------------------------------------------
// counter definitions
//----------------------------------------------------------------------------
#define CGPIOD_CNT_COUNT                         1 //

#define CGPIOD_CNT_CMD_SET                       1 // 
#define CGPIOD_CNT_CMD_ADD                       2 // 
#define CGPIOD_CNT_CMD_ABORT                     3 // 

#define CGPIOD_CNT_EVT_TICK                      2 // 

#define CGPIOD_CNT_FLG_NONE             0x00000000 // 
#define CGPIOD_CNT_FLG_CNTR             0x00000001 // counter timer

//----------------------------------------------------------------------------
// heartbeat definitions
//----------------------------------------------------------------------------
#define CGPIOD_HB_COUNT                          3 //
#define CGPIOD_HB0                               0 // 0.5s heartbeat
#define CGPIOD_HB0_PERIOD                      500 // 
#define CGPIOD_HB1                               1 // 1.0s heartbeat
#define CGPIOD_HB1_PERIOD                     1000 // 
#define CGPIOD_HB2                               2 // 2.0s heartbeat
#define CGPIOD_HB2_PERIOD                     2000 // 

typedef struct {
  tUint32            dwFlags;                      //
  tUint32            msStart;                      // start time in ms
  tUint32            msPeriod;                     // period in ms
  tUint32            dwRepeat;                     // repeat times, 0=forever
  tUint32            dwCntr;                       // counter value
  tCChar*            szName;                       //
  } tGpiodCounter;

//----------------------------------------------------------------------------
// input definitions
//----------------------------------------------------------------------------
#define CGPIOD_IN_COUNT                          4 //

#define CGPIOD_IN0_PIN                           0 // D3 
#define CGPIOD_IN1_PIN                           2 // D4 

#define CGPIOD_IN_TMR_DEBOUNCE                 150 // in ms
#define CGPIOD_IN_TMR_INGT1                   1000 //
#define CGPIOD_IN_TMR_INGT2                   2000 //

#define CGPIOD_IN_VAL_IN                         1 // input closed
#define CGPIOD_IN_VAL_OUT                        0 // input opened

#define CGPIOD_IN_POL_NORMAL                     0 //
#define CGPIOD_IN_POL_INVERT                     1 //

#define CGPIOD_IN_STATE_OUT                      0 // 
#define CGPIOD_IN_STATE_INGT0                    1 // 
#define CGPIOD_IN_STATE_INGT1                    2 // 
#define CGPIOD_IN_STATE_INGT2                    3 // 

#define CGPIOD_IN_EVT_INGT0                      2 // 
#define CGPIOD_IN_EVT_OUTLT1                     3 // 
#define CGPIOD_IN_EVT_INGT1                      4 // 
#define CGPIOD_IN_EVT_OUTGT1                     5 // 
#define CGPIOD_IN_EVT_INGT2                      6 // 
#define CGPIOD_IN_EVT_OUT                        7 // 

#define CGPIOD_IN_FLG_NONE              0x00000000 // 
#define CGPIOD_IN_FLG_MQTT_INGT0        0x00000004 // send event to MQTT broker
#define CGPIOD_IN_FLG_MQTT_OUTLT1       0x00000008 // send event to MQTT broker
#define CGPIOD_IN_FLG_MQTT_INGT1        0x00000010 // send event to MQTT broker
#define CGPIOD_IN_FLG_MQTT_OUTGT1       0x00000020 // send event to MQTT broker
#define CGPIOD_IN_FLG_MQTT_INGT2        0x00000040 // send event to MQTT broker
#define CGPIOD_IN_FLG_MQTT_OUT          0x00000080 // send event to MQTT broker
#define CGPIOD_IN_FLG_MQTT_ALL          0x000000FC // send all events to MQTT broker

typedef struct {
  tUint32            dwFlags;                      // evt routing flags
  tUint32            dwState;                      // input state
  tUint32            dwPin;                        // GPIO pin number
  tUint32            dwPol;                        // polarity 0=normal, 1=invert
  tUint32            dwVal;                        // stable pin value
  tUint32            tmrDebounce;                  // debounce timer
  tUint32            msDebounce;                   // debounce ref time
  tUint32            msState;                      // state ref time
  tCChar*            szName;                       // obj name
  } tGpiodInput;

//----------------------------------------------------------------------------
// output definitions
//----------------------------------------------------------------------------
#define CGPIOD_OUT_COUNT                         2 // 

#define CGPIOD_OUT0_PIN                          5 // D1
#define CGPIOD_OUT1_PIN                          4 // D2

#define CGPIOD_OUT_POL_NORMAL                    0 // 
#define CGPIOD_OUT_POL_INVERT                    1 // 

#define CGPIOD_OUT_STATE_OFF                     0 // 
#define CGPIOD_OUT_STATE_ON                      1 // 

#define CGPIOD_OUT_CMD_MASK             0x0000FFFF //
#define CGPIOD_OUT_CMD_PARMS            0xFFFF0000 //
#define CGPIOD_OUT_CMD_NONE                      0 //
#define CGPIOD_OUT_CMD_STATUS                    1 //
#define CGPIOD_OUT_CMD_ON                        2 //
#define CGPIOD_OUT_CMD_OFF                       3 //
#define CGPIOD_OUT_CMD_ONLOCKED                  4 //
#define CGPIOD_OUT_CMD_OFFLOCKED                 5 //
#define CGPIOD_OUT_CMD_TOGGLE                    6 //
#define CGPIOD_OUT_CMD_UNLOCK                    7 //
#define CGPIOD_OUT_CMD_ONDELAYED                 8 //
#define CGPIOD_OUT_CMD_OFFDELAYED                9 //
#define CGPIOD_OUT_CMD_ONTIMED                  10 //
#define CGPIOD_OUT_CMD_OFFTIMED                 11 //
#define CGPIOD_OUT_CMD_TOGGLEDELAYED            12 //
#define CGPIOD_OUT_CMD_TOGGLETIMED              13 //
#define CGPIOD_OUT_CMD_LOCK                     14 //
#define CGPIOD_OUT_CMD_LOCKTIMED                15 //
#define CGPIOD_OUT_CMD_BLINK                    16 //

#define CGPIOD_OUT_EVT_ON                        2 //
#define CGPIOD_OUT_EVT_OFF                       3 //
#define CGPIOD_OUT_EVT_TIMER                     4 //

#define CGPIOD_OUT_FLG_NONE             0x00000000 //
#define CGPIOD_OUT_FLG_MQTT_ON          0x00000004 //
#define CGPIOD_OUT_FLG_MQTT_OFF         0x00000008 //
#define CGPIOD_OUT_FLG_MQTT_TIMER       0x00000010 //
#define CGPIOD_OUT_FLG_MQTT_ALL         0x0000001C //
#define CGPIOD_OUT_FLG_LOCKED           0x40000000 //
#define CGPIOD_OUT_FLG_STANDALONE       0x80000000 //

typedef struct {
  tUint32            dwFlags;                      //
  tUint32            dwPin;                        // GPIO pin number
  tUint32            dwPol;                        // polarity 0=normal, 1=invert
  tUint32            dwState;                      //
  tUint32            dwCmd;                        // 
  tUint32            dwRun;                        // time 2
  tCChar*            szName;                       //
  } tGpiodOutput;

//----------------------------------------------------------------------------
// shutter definitions
//----------------------------------------------------------------------------
#define CGPIOD_UDM_COUNT                         1 //

#define CGPIOD_UDM0_PIN_UP                       5 // D1
#define CGPIOD_UDM0_PIN_DOWN                     4 // D2

#define CGPIOD_UDM_POL_NORMAL                    0 // 
#define CGPIOD_UDM_POL_INVERT                    1 // 

#define CGPIOD_UDM_RUN_DEF                   15000 // 15s

#define CGPIOD_UDM_STATE_STOP                    0 // 
#define CGPIOD_UDM_STATE_UP                      1 // 
#define CGPIOD_UDM_STATE_DOWN                    2 // 

#define CGPIOD_UDM_CMD_MASK             0x0000FFFF //
#define CGPIOD_UDM_CMD_PARMS            0xFFFF0000 //
#define CGPIOD_UDM_CMD_NONE                      0 //
#define CGPIOD_UDM_CMD_STATUS                    1 //
#define CGPIOD_UDM_CMD_STOP                      2 //
#define CGPIOD_UDM_CMD_TOGGLEUP                  3 //
#define CGPIOD_UDM_CMD_TOGGLEDOWN                4 //
#define CGPIOD_UDM_CMD_UP                        5 //
#define CGPIOD_UDM_CMD_DOWN                      6 //
#define CGPIOD_UDM_CMD_TIPUP                     7 //
#define CGPIOD_UDM_CMD_TIPDOWN                   8 //
#define CGPIOD_UDM_CMD_PRIOLOCK                  9 //
#define CGPIOD_UDM_CMD_PRIOUNLOCK               10 //
#define CGPIOD_UDM_CMD_LEARNON                  11 //
#define CGPIOD_UDM_CMD_LEARNOFF                 12 //
#define CGPIOD_UDM_CMD_PRIOSET                  13 //
#define CGPIOD_UDM_CMD_PRIORESET                14 //
#define CGPIOD_UDM_CMD_SENSROLUP                15 //
#define CGPIOD_UDM_CMD_SENSJALUP                16 //
#define CGPIOD_UDM_CMD_SENSROLDOWN              17 //
#define CGPIOD_UDM_CMD_SENSJALDOWN              18 //

#define CGPIOD_UDM_PRIO_LVL_0                    0 //
#define CGPIOD_UDM_PRIO_LVL_1                    1 //
#define CGPIOD_UDM_PRIO_LVL_2                    2 //
#define CGPIOD_UDM_PRIO_LVL_3                    3 //
#define CGPIOD_UDM_PRIO_LVL_4                    4 //
#define CGPIOD_UDM_PRIO_LVL_5                    5 //

#define CGPIOD_UDM_PRIO_MASK_NONE       0x00000000 //
#define CGPIOD_UDM_PRIO_MASK_0          0x00000001 //
#define CGPIOD_UDM_PRIO_MASK_1          0x00000002 //
#define CGPIOD_UDM_PRIO_MASK_2          0x00000004 //
#define CGPIOD_UDM_PRIO_MASK_3          0x00000008 //
#define CGPIOD_UDM_PRIO_MASK_4          0x00000010 //
#define CGPIOD_UDM_PRIO_MASK_5          0x00000020 //

#define CGPIOD_UDM_EVT_STOP                      2 //
#define CGPIOD_UDM_EVT_UPON                      3 //
#define CGPIOD_UDM_EVT_UPOFF                     4 //
#define CGPIOD_UDM_EVT_DOWNON                    5 //
#define CGPIOD_UDM_EVT_DOWNOFF                   6 //

#define CGPIOD_UDM_FLG_NONE             0x00000000 //
#define CGPIOD_UDM_FLG_MQTT_STOP        0x00000004 //
#define CGPIOD_UDM_FLG_MQTT_UPON        0x00000008 //
#define CGPIOD_UDM_FLG_MQTT_UPOFF       0x00000010 //
#define CGPIOD_UDM_FLG_MQTT_DOWNON      0x00000020 //
#define CGPIOD_UDM_FLG_MQTT_DOWNOFF     0x00000040 //
#define CGPIOD_UDM_FLG_MQTT_ALL         0x0000007C //
#define CGPIOD_UDM_FLG_LOCKED           0x40000000 //
#define CGPIOD_UDM_FLG_STANDALONE       0x80000000 //

typedef struct {
  tUint32            dwFlags;                      //
  tUint32            dwPinUp;                      // GPIO pin number
  tUint32            dwPinDown;                    // GPIO pin number
  tUint32            dwPol;                        // polarity 0=normal, 1=invert
  tUint32            dwRunDef;                     // default run time for standalone oper
  tUint32            dwState;                      // CGPIOD_UDM_STATE_
  tUint32            dwPrioLvl;                    // 0..5
  tUint32            dwPrioMask;                   // 0..63
  tUint32            dwCmd;                        // 
  tUint32            dwDelay;                      // delay time
  tUint32            dwRun;                        // run time
  tUint32            dwTip;                        // tip time
  tCChar*            szName;                       //
  } tGpiodShutter;

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
typedef struct {
  tUint32            msNow;                        // 
  tUint32            dwObj;                        // 
  tUint32            dwCmd;                        // 
  tUint32            dwLock;                       //
  tUint32            dwPrioLvl;                    //
  tUint32            dwPrioMask;                   //
  tUint32            dwDelay;                      //
  tUint32            dwRun;                        //
  tUint32            dwTip;                        //
  } tGpiodCmd;

typedef struct {
  tUint32            msNow;                        // 
  tUint32            dwObj;                        // 
  tUint32            dwEvt;                        // 
  tCChar*            szEvt;                        //
  } tGpiodEvt;

//----------------------------------------------------------------------------
// callbacks
//----------------------------------------------------------------------------
void                 gpiodOnHttpConfig(HttpRequest &request, HttpResponse &response);
void                 gpiodOnPublish(String topic, String message);

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class CGpiod {
 private:
  tUint32            m_dwError;                    //

  tUint32            m_dwMode;                     //
  tCChar*            m_szCmdPfx;                   //
  tCChar*            m_szEvtPfx;                   //

  tGpiodCounter      m_hb[CGPIOD_HB_COUNT];        // heartbeat counters
  tGpiodInput        m_input[CGPIOD_IN_COUNT];     //
  tGpiodOutput       m_output[CGPIOD_OUT_COUNT];   //
  tGpiodShutter      m_shutter[CGPIOD_UDM_COUNT];  // 
  CParse             m_parse;
  Timer              checkTimer;


 public:
  //--------------------------------------------------------------------------
  void               begin();
//  void               notifyChange(String object, String value);
//  void               registerHttpHandlers(HttpServer &server);
//  void               registerCommandHandlers();

  //--------------------------------------------------------------------------
  //
  //--------------------------------------------------------------------------
  tUint32            _GetFlags(tUint32* pFlags, tUint32 dwFlags) { return *pFlags &   dwFlags; }
  tUint32            _SetFlags(tUint32* pFlags, tUint32 dwFlags) { return *pFlags |=  dwFlags; }
  tUint32            _RstFlags(tUint32* pFlags, tUint32 dwFlags) { return *pFlags &= ~dwFlags; }

  //--------------------------------------------------------------------------
  // gpiod.cpp
  //--------------------------------------------------------------------------
  tUint32            GetMode()                     { return m_dwMode; }

  tUint32            OnConfig();
  tUint32            OnInit();
  void               OnRun();
  tUint32            OnExit();

  tUint32            DoCmd(tGpiodCmd* pCmd);
  tUint32            DoEvt(tGpiodEvt* pEvt);
  void               _DoPublish(tUint32 fDup, tUint32 fQoS, tUint32 fRetain, tCChar* szObj, tCChar* szMsg);

  //--------------------------------------------------------------------------
  // gpiod_parse.cpp
  //--------------------------------------------------------------------------
  tUint32            ParseCmd(tGpiodCmd* pOut, tChar* pObj, tChar* pCmd, tUint32 dwMask1);
  tUint32            _parseCmdSystem(tGpiodCmd* pOut);
  tUint32            _parseCmdOutput(tGpiodCmd* pOut);
  tUint32            _parseCmdShutter(tGpiodCmd* pOut);
  tUint32            _parseCmdParams(tGpiodCmd* pOut);

 private:
  void               checkConnection();

  tUint32            DumpCmd(tGpiodCmd* pCmd);
  
  //--------------------------------------------------------------------------
  // gpiod_input.cpp
  //--------------------------------------------------------------------------
  tCChar*            _inputEvt2String(tUint32 dwEvt);
  tUint32            _inputOnConfig();
  tUint32            _inputOnInit();
  tUint32            _inputOnRun(tUint32 msNow);
  tUint32            _inputOnExit();
  tUint32            _inputGetPinVal(tGpiodInput* pObj, tUint32 msNow);

  //--------------------------------------------------------------------------
  // gpiod_output.cpp
  //--------------------------------------------------------------------------
  tCChar*            _outputEvt2String(tUint32 dwEvt);
  tUint32            _outputOnConfig();
  tUint32            _outputOnInit();
  tUint32            _outputOnRun(tUint32 msNow);
  tUint32            _outputOnHbTick(tUint32 dwHb, tUint32 dwCntr);
  tUint32            _outputOnExit();
  tUint32            _outputDoEvt(tGpiodEvt* pEvt);
  tUint32            _outputDoCmd(tGpiodCmd* pCmd);
  void               _outputSetState(tGpiodOutput* pObj, tUint32 dwState, tGpiodEvt* pEvt);

  //--------------------------------------------------------------------------
  // gpiod_shutter.cpp
  //--------------------------------------------------------------------------
  tCChar*            _shutterEvt2String(tUint32 dwEvt);
  tUint32            _shutterOnConfig();
  tUint32            _shutterOnInit();
  tUint32            _shutterOnRun(tUint32 msNow);
  tUint32            _shutterOnExit();
  tUint32            _shutterDoEvt(tGpiodEvt* pEvt);
  tUint32            _shutterCheckPrio(tGpiodShutter* pObj, tGpiodCmd* pCmd);
  tUint32            _shutterDoCmd(tGpiodCmd* pCmd);
  void               _shutterSetState(tGpiodShutter* pObj, tUint32 dwState, tGpiodEvt* pEvt);

  }; // CGpiod

extern CGpiod g_gpiod;


#endif // __cgpiod_hpp__