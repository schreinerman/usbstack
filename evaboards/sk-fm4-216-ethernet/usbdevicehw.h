/************************************************************************/
/*               (C) Fujitsu Semiconductor Europe GmbH (FSEU)           */
/*                                                                      */
/* The following software deliverable is intended for and must only be  */
/* used for reference and in an evaluation laboratory environment.      */
/* It is provided on an as-is basis without charge and is subject to    */
/* alterations.                                                         */
/* It is the user's obligation to fully test the software in its        */
/* environment and to ensure proper functionality, qualification and    */
/* compliance with component specifications.                            */
/*                                                                      */
/* In the event the software deliverable includes the use of open       */
/* source components, the provisions of the governing open source       */
/* license agreement shall apply with respect to such software          */
/* deliverable.                                                         */
/* FSEU does not warrant that the deliverables do not infringe any      */
/* third party intellectual property right (IPR). In the event that     */
/* the deliverables infringe a third party IPR it is the sole           */
/* responsibility of the customer to obtain necessary licenses to       */
/* continue the usage of the deliverable.                               */
/*                                                                      */
/* To the maximum extent permitted by applicable law FSEU disclaims all */
/* warranties, whether express or implied, in particular, but not       */
/* limited to, warranties of merchantability and fitness for a          */
/* particular purpose for which the deliverable is not designated.      */
/*                                                                      */
/* To the maximum extent permitted by applicable law, FSEU's liability  */
/* is restricted to intentional misconduct and gross negligence.        */
/* FSEU is not liable for consequential damages.                        */
/*                                                                      */
/* (V1.5)                                                               */
/************************************************************************/
/** \file usbdevicehw.h
 **
 ** - See README.TXT for project description
 ** - pre release for a simple universal usb function library
 **
 ** History:
 **   - 2010-03-30    1.0  MSc  First version  (works with 16FX,FR80)
 **   - 2011-03-30    1.1  MSc  New HW description style
 *****************************************************************************/

#ifndef __USBDEVICEHW_H__
#define __USBDEVICEHW_H__

#define DEVICE0VBUS_DISABLEISR                            
#define DEVICE0VBUS_ENABLEISR                             
#define DEVICE0VBUS_ISRISSET                              0
#define DEVICE0VBUS_CLEARISRFLAG                          
#define DEVICE0VBUS_SETLOWDETECT                          
#define DEVICE0VBUS_SETHIGHDETECT                         
#define DEVICE0VBUS_HIGHDETECT                            ((FM4_GPIO->PDIR6 & (1 << 0)) != 0)
#define DEVICE0VBUS_INIT                                  FM4_GPIO->DDR6 &= ~(1 << 0)
#define DEVICE0VBUS_DEINIT                                
#define DEVICE0VBUS_ENABLED                               1

#define DEVICE0HCONX_INIT                                 
#define DEVICE0HCONX_SET                                  
#define DEVICE0HCONX_CLEAR                                
#define DEVICE0HCONX_ENABLED                              1

#define DEVICE1VBUS_DISABLEISR                            
#define DEVICE1VBUS_ENABLEISR                             
#define DEVICE1VBUS_ISRISSET                              0
#define DEVICE1VBUS_CLEARISRFLAG                          
#define DEVICE1VBUS_SETLOWDETECT                          
#define DEVICE1VBUS_SETHIGHDETECT                         
#define DEVICE1VBUS_HIGHDETECT                            (0)
#define DEVICE1VBUS_INIT                                  
#define DEVICE1VBUS_DEINIT                                
#define DEVICE1VBUS_ENABLED                               1

#define DEVICE1HCONX_INIT                                 
#define DEVICE1HCONX_SET                                  
#define DEVICE1HCONX_CLEAR                                
#define DEVICE1HCONX_ENABLED                              1


#endif
