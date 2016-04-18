
//----------------------------------------------------------------------------
// app_clslevels.h
//
// Copyright (c) Jo Simons, 2015-2016, All Rights Reserved.
//----------------------------------------------------------------------------
// loglevels                     class level
//
// app                           0     00
// httpc                         0     06
//
//----------------------------------------------------------------------------
#ifndef __app_clslevels_h__
#define __app_clslevels_h__

#include "xstdafx.h"

// definitions
#define CLSLVL_APP                    DEBUG_LVL_00 //
#define CLSLVL_MQTT                   DEBUG_LVL_01 //
#define CLSLVL_HTTP                   DEBUG_LVL_02 //
#define CLSLVL_NETWORK                DEBUG_LVL_03 //
#define CLSLVL_ATS                    DEBUG_LVL_04 //
                                               
#define CLSLVL_GPIOD                  DEBUG_LVL_08 // 0x0100
#define CLSLVL_GPIOD_INPUT            DEBUG_LVL_09 // 0x0200
#define CLSLVL_GPIOD_OUTPUT           DEBUG_LVL_0A // 0x0400
#define CLSLVL_GPIOD_SHUTTER          DEBUG_LVL_0B // 0x0800
#define CLSLVL_GPIOD_SYSTEM           DEBUG_LVL_0C // 0x1000
#define CLSLVL_GPIOD_PARSE            DEBUG_LVL_0D // 0x2000
                                                          
#endif // __app_clslevels_h__
