#ifndef _MACHINE_H_
/****************************************************************/
/*                                                              */
        /*  Copyright 1995, David B. Wortman */
/*      machine.h                                               */
/*      Interface to the CSC488S machine interpreter            */
/*                                                              */
/****************************************************************/
#define _MACHINE_H_

static char machineInterfaceVersion[] = 
   "$Id: machine.h,v 1.7 1996/01/29 17:36:31 dw Exp $" ;

        /* Initialize the machine       */
void  machineInitialize();

        /*  Execute compiled program    */
void  machineExecute() ;

#endif

