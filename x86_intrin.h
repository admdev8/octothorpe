/*
 *             _        _   _                           
 *            | |      | | | |                          
 *   ___   ___| |_ ___ | |_| |__   ___  _ __ _ __   ___ 
 *  / _ \ / __| __/ _ \| __| '_ \ / _ \| '__| '_ \ / _ \
 * | (_) | (__| || (_) | |_| | | | (_) | |  | |_) |  __/
 *  \___/ \___|\__\___/ \__|_| |_|\___/|_|  | .__/ \___|
 *                                          | |         
 *                                          |_|
 *
 * Written by Dennis Yurichev <dennis(a)yurichev.com>, 2013
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/3.0/.
 *
 */

#pragma once

// Rationale: I use in x86 emulator testing.

#include "stuff.h"
#include "datatypes.h"

#ifdef _WIN64
#else

void intrin_SHL (IN tetrabyte value, IN uint8_t shift_value, OUT tetrabyte* result, IN OUT tetrabyte* flags);
void intrin_SHR (IN tetrabyte value, IN uint8_t shift_value, OUT tetrabyte* result, IN OUT tetrabyte* flags);
void intrin_SAR (IN tetrabyte value, IN uint8_t shift_value, OUT tetrabyte* result, IN OUT tetrabyte* flags);
void intrin_ADD (IN tetrabyte op1, IN tetrabyte op2, OUT tetrabyte* result, IN OUT tetrabyte* flags);
void intrin_ADC (IN tetrabyte op1, IN tetrabyte op2, OUT tetrabyte* result, IN OUT tetrabyte* flags);
void intrin_SUB (IN tetrabyte op1, IN tetrabyte op2, OUT tetrabyte* result, IN OUT tetrabyte* flags);
void intrin_SBB (IN tetrabyte op1, IN tetrabyte op2, OUT tetrabyte* result, IN OUT tetrabyte* flags);
void intrin_XOR (IN tetrabyte op1, IN tetrabyte op2, OUT tetrabyte* result, IN OUT tetrabyte* flags);
void intrin_XOR_addr (IN tetrabyte *address_of_op1, IN tetrabyte op2, OUT tetrabyte* result, IN OUT tetrabyte* flags);
void intrin_OR (IN tetrabyte op1, IN tetrabyte op2, OUT tetrabyte* result, IN OUT tetrabyte* flags);
void intrin_AND (IN tetrabyte op1, IN tetrabyte op2, OUT tetrabyte* result, IN OUT tetrabyte* flags);

#endif
