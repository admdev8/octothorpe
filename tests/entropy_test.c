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

#include "oassert.h"
#include "datatypes.h"
#include "entropy.h"
#include "memutils.h"

int main()
{
	byte buf[256];
	bzero(buf, sizeof(buf));
	oassert(entropy (buf, sizeof(buf))==0);
	bytefill(buf, sizeof(buf), 0x11);
	oassert(entropy (buf, sizeof(buf))==0);
	for (int i=0; i<256; i++)
		buf[i]=i;
	oassert(entropy (buf, sizeof(buf))==8);
	return 0;
};

