#ifndef VYQUON_H
#define VYQUON_H

#define DEBUG true

/* Debug mode enables a few useful macros */
#ifdef DEBUG
/* DEBUG_PRINT - print a message, flush the buffer. */
#define DEBUG_PRINT(a) {printf(a); fflush(stdout);}
#endif

/* Define boolean data types */
#define true TRUE
#define false FALSE

/* Include standard library */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/* External libraries */
#include "gdsl.h"      
#include <glib.h>

/* Memory management and garbage collection */
#include "Mem.h"

/*  Data types */
#include "Obj.h"
#include "Cons.h"
#include "String.h"
#include "Symbol.h"
#include "Function.h"

/* Parser and lexer and input */
#include "Lexer.h"
#include "Parser.h"

/* Runtime */
#include "VM.h"
#include "Scope.h"

/* External interface */
#include "External.h"


#endif /* VYQUON_H */
