/*=================================================================|
|* File:				Flags.h									  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef FLAGS_H
#define FLAGS_H

#include <PowerConsole/Declarations.h>

//=================================================================|
// DEFINITIONS													   |
//=================================================================/

/* Returns true if the flag is applied to the attributes. */
#define getFlag(attributes, flag) (((attributes) & (flag)) == (flag))
/* Returns true if the any flag is applied to the attributes. */
#define getAnyFlag(attributes, flag, noneFlag) (((attributes) & (flag)) != (noneFlag))
/* Sets the specified flag in the attributes. */
#define setFlag(attributes, flag, enabled) ((enabled) ? ((attributes) |= (flag)) : ((attributes) &= ~(flag)))

/* Returns true if the flag is applied to the attributes. */
#define getTypeFlag(type, attributes, flag)	((((type)(attributes)) & ((type)(flag))) == ((type)(flag)))
/* Sets the specified flag in the attributes. */
#define setTypeFlag(type, attributes, flag, enabled) ((enabled) ? (((type)(attributes)) |= ((type)(flag))) : (((type)(attributes)) &= ~((type)(flag))))

/* Returns true if the flag is applied to the attributes. */
#define getByteFlag(attributes, flag)	getTypeFlag(unsigned char, attributes, flag)
/* Sets the specified flag in the attributes. */
#define setByteFlag(attributes, flag, enabled) setTypeFlag(unsigned char, attributes, flag, enabled)

/* Returns true if the flag is applied to the attributes. */
#define getUShortFlag(attributes, flag)	getTypeFlag(unsigned short, attributes, flag)
/* Sets the specified flag in the attributes. */
#define setUShortFlag(attributes, flag, enabled) setTypeFlag(unsigned short, attributes, flag, enabled)

/* Returns true if the flag is applied to the attributes. */
#define getUIntFlag(attributes, flag)	getTypeFlag(unsigned int, attributes, flag)
/* Sets the specified flag in the attributes. */
#define setUIntFlag(attributes, flag, enabled) setTypeFlag(unsigned int, attributes, flag, enabled)

/* Returns true if the flag is applied to the attributes. */
#define getULongFlag(attributes, flag)	getTypeFlag(unsigned long, attributes, flag)
/* Sets the specified flag in the attributes. */
#define setULongFlag(attributes, flag, enabled) setTypeFlag(unsigned long, attributes, flag, enabled)

/* Returns true if the flag is applied to the attributes. */
#define getULLongFlag(attributes, flag)	getTypeFlag(unsigned long long, attributes, flag)
/* Sets the specified flag in the attributes. */
#define setULLongFlag(attributes, flag, enabled) setTypeFlag(unsigned long long, attributes, flag, enabled)

//=================================================================|
#endif /* End include guard */
//=================================================================|
