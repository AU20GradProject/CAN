#ifndef COMPILER_H_
#define COMPILER_H_

#include "Compiler_Cfg.h"

#define AUTOMATIC	/* used as input for memory class (memclass) and  pointer class (ptrclass) when no 
input from Compiler_Cfg.h is needed */
#define TYPEDEF		/* used with typedef keyword in as input for memory class (of compiler)because with 
typedef we cannot use them */
#define	NULL_PTR	((void*) 0x0)
#define	INLINE 			inline
#define LOCAL_INLINE	static inline


/* 8.2.2 function definition use it to define functions */

#define FUNC( rettype, memclass )   rettype 									/* for the declaration 
and definition of functions */

#define FUNC_P2CONST(rettype, ptrclass, memclass)	const rettype *			/* declaration and definition 
of functions returning a pointer to a constant */

#define FUNC_P2VAR(rettype, ptrclass, memclass)		rettype *		/* for the declaration and definition 
of functions returning a pointer to a variable */


/* 8.2.3 Pointer definitions use it to define pointers */

#define P2VAR(ptrtype, memclass, ptrclass)	ptrtype * 		                /* for the declaration and 
definition of pointers in RAM, pointing to variables */

#define P2CONST(ptrtype, memclass, ptrclass) const ptrtype*				    /* for the declaration and 
definition of pointers in RAM pointing to constants */

#define CONSTP2VAR(ptrtype, memclass, ptrclass) ptrtype * const 		    /* for the declaration and 
definition of constant pointers accessing variables */

#define CONSTP2CONST(ptrtype, memclass, ptrclass)	const ptrtype * const	/* for the declaration and 
definition of constant pointers accessing constants */

#define P2FUNC(rettype, ptrclass, fctname)	rettype (* fctname)			    /* for the type definition of 
pointers to functions  (fctname is pointer to function name)*/

#define CONSTP2FUNC(rettype, ptrclass, fctname)	rettype (* const fctname)	/* the type definition of 
constant pointers to functions */ /* fctname is pointer to function name*/


/* 8.2.4 Constant definitions use it to define Constants */

#define CONST(type, memclass)	const type		/* for the declaration and definition of constants */


/* 8.2.4 Variable definitions use it to define Variables */

#define VAR( type, memclass )  type		/* for the declaration and definition of constants */

#endif
