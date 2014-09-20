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

#ifdef _WIN64

#define O_BITS64

#elif _WIN32

#define O_BITS32

#elif __APPLE__

	#ifdef __ppc__
	#define O_BITS32
	#elif __ppc64__
	#define O_BITS64
	#elif __i386__
	#define O_BITS32
	#elif __x86_64__
	#define O_BITS64
	#else
	#error "(Apple) taget CPU was not detected"
	#endif
	
#elif __linux__

	#if defined(__LP64__) || defined (_LP64)
	#define O_BITS64
	#else
	#define O_BITS32
	#endif

#else
#error "OS was not detected"
#endif

