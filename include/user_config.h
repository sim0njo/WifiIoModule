#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

	// UART config
	#define SERIAL_BAUD_RATE 115200

  #define INPUT0_PIN       5 // D1
  #define INPUT1_PIN       4 // D2
  #define OUTPUT0_PIN      0 // D3
  #define OUTPUT1_PIN      2 // D4

  // for Logic state analyser measurements
  #define SCOPE_PIN 16 // GPIO

	// ESP SDK config
	#define LWIP_OPEN_SRC
	#define USE_US_TIMER

	// Default types
	#define __CORRECT_ISO_CPP_STDLIB_H_PROTO
	#include <limits.h>
	#include <stdint.h>

	// Override c_types.h include and remove buggy espconn
	#define _C_TYPES_H_
	#define _NO_ESPCON_

	// Updated, compatible version of c_types.h
	// Just removed types declared in <stdint.h>
	#include <espinc/c_types_compatible.h>

	// System API declarations
	#include <esp_systemapi.h>

	// C++ Support
	#include <esp_cplusplus.h>
	// Extended string conversion for compatibility
	#include <stringconversion.h>
	// Network base API
	#include <espinc/lwip_includes.h>

	// Beta boards
	#define BOARD_ESP01

#ifdef __cplusplus
}
#endif

#endif
