#ifndef COMPILER_CFG_H

#define COMPILER_CFG_H


/*********************************************************************/


/* Published information */
#define Can_MODULE_ID 0x020u
#define Can_VENDOR_ID 0x002Bu   /* vendor id of NXP */
		
#define Can_AR_RELEASE_MAJOR_VERSION 0x01u
#define Can_AR_RELEASE_MINOR_VERSION 0x00u
#define Can_AR_RELEASE_REVISION_VERSION 0x00u
		
#define Can_SW_MAJOR_VERSION 0x04u
#define Can_SW_MINOR_VERSION 0x03u
#define Can_SW_PATCH_VERSION 0x01u


/*********************************************************************/

/* 7.1.2 */
/* this is compiler keywords (not C keywords) must by used with function, variables and, pointers to work 
in prober way */
/* for ARM C (GCC, ADS) compilers no need for those*/

#define Can_CODE                /* used for ordinary code */
#define Can_CODE_FAST			/* used for code that shall go into fast code memory segments */
#define Can_CODE_SLOW			/* used for code that shall go into slow code memory segments */
#define Can_CALIB				/* used for calibration constants */
#define Can_CONFIG_DATA			/* used for cused for module configuration constants */
#define Can_CONST				/* used for global or static constants */
#define Can_APPL_DATA			/* used for global or static pointer */
#define Can_APPL_CONST	 		/* used for global or static pointer to constants */
#define REGSPACE		 		/* used for staitc Pointer to Reg (ptr to volatile) carry adress of value 
that changed in any time by interrupts */

/* used for all global or static variables */
#define Can_VAR_NO_INIT					/* for variables that are never cleared and never initialized */
#define	Can_VAR_CLEARED					/* for variables that are cleared to zero after every reset */
#define	Can_VAR_POWER_ON_CLEARED		/* for variables that are cleared to zero only after power on 
reset */
#define	Can_VAR_INIT					/* for variables that are initialized with values after every 
reset */
#define	Can_VAR_ON_INIT					/* for variables that are initialized with values only after 
power on reset */

/* used for all global or static variables that have at least one of the following properties: accessed 
bitwise, frequently used, high number of accesses in source code */
#define Can_VAR_FAST_NO_INIT				/* for variables that are never cleared and never initialized 
*/
#define	Can_VAR_FAST_CLEARED				/* for variables that are cleared to zero after every reset */
#define	Can_VAR_FAST_POWER_ON_CLEARED		/* for variables that are cleared to zero only after power on 
reset */
#define	Can_VAR_FAST_INIT					/* for variables that are initialized with values after every 
reset */
#define	Can_VAR_FAST_ON_INIT				/* for variables that are initialized with values only after 
power on reset */

/* used for all infrequently accessed global or static variables */
#define Can_VAR_SLOW__NO_INIT				/* for variables that are never cleared and never initialized 
*/
#define	Can_VAR_SLOW__CLEARED				/* for variables that are cleared to zero after every reset */
#define	Can_VAR_SLOW__POWER_ON_CLEARED		/* for variables that are cleared to zero only after power on 
reset */
#define	Can_VAR_SLOW__INIT  				/* for variables that are initialized with values after every 
reset */
#define	Can_VAR_SLOW__ON_INIT				/* for variables that are initialized with values only after 
power on reset */

/* used for global or static variables which are accessible from a calibration tool */
#define Can_INTERNAL_VAR__NO_INIT				/* for variables that are never cleared and never 
initialized */
#define	Can_INTERNAL_VAR__CLEARED				/* for variables that are cleared to zero after every 
reset */
#define	Can_INTERNAL_VAR__POWER_ON_CLEARED		/* for variables that are cleared to zero only after 
power on reset */
#define	Can_INTERNAL_VAR__INIT  				/* for variables that are initialized with values after 
every reset */
#define	Can_INTERNAL_VAR__ON_INIT				/* for variables that are initialized with values only 
after power on reset */



#endif 	/* COMPILER_CFG_H */
