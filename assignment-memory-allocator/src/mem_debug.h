#ifndef _MEM_DEBUG_H_
#define _MEM_DEBUG_H_

#include <stdarg.h>
#include <stdio.h>

#include "mem.h"
#include "mem_internals.h"

void debug_block(struct block_header* b, const char* fmt, ... );

void debug(const char* fmt, ... );

#endif
