/**************************************************************************
* Copyright (C)2011 Spansion LLC. All Rights Reserved . 
*
* This software is owned and published by: 
* Spansion LLC, 915 DeGuigne Dr. Sunnyvale, CA  94088-3453 ("Spansion").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND 
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software constitutes driver source code for use in programming Spansion's 
* Flash memory components. This software is licensed by Spansion to be adapted only 
* for use in systems utilizing Spansion's Flash memories. Spansion is not be 
* responsible for misuse or illegal use of this software for devices not 
* supported herein.  Spansion is providing this source code "AS IS" and will 
* not be responsible for issues arising from incorrect user implementation 
* of the source code herein.  
*
* SPANSION MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE, 
* REGARDING THE SOFTWARE, ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED 
* USE, INCLUDING, WITHOUT LIMITATION, NO IMPLIED WARRANTY OF MERCHANTABILITY, 
* FITNESS FOR A  PARTICULAR PURPOSE OR USE, OR NONINFRINGEMENT.  SPANSION WILL 
* HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT, NEGLIGENCE OR 
* OTHERWISE) FOR ANY DAMAGES ARISING FROM USE OR INABILITY TO USE THE SOFTWARE, 
* INCLUDING, WITHOUT LIMITATION, ANY DIRECT, INDIRECT, INCIDENTAL, 
* SPECIAL, OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA, SAVINGS OR PROFITS, 
* EVEN IF SPANSION HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.  
*
* This software may be replicated in part or whole for the licensed use, 
* with the restriction that this Copyright notice must be included with 
* this software, whether used in part or whole, at all times.  
******************************************************************************/
/** \file usbdevicehw.h
 **
 ** USB device hardware settings for sk-fm4-120pmc-tft evaluation board
 **
 ** History:
 **   - 2014-03-12  MSc First Version
 *****************************************************************************/

#ifndef __USBDEVICEHW_H__
#define __USBDEVICEHW_H__

#define DEVICE0VBUS_DISABLEISR 
#define DEVICE0VBUS_ENABLEISR 
#define DEVICE0VBUS_ISRISSET 0
#define DEVICE0VBUS_CLEARISRFLAG
#define DEVICE0VBUS_SETLOWDETECT 
#define DEVICE0VBUS_SETHIGHDETECT 
#define DEVICE0VBUS_HIGHDETECT (bFM4_GPIO_PDIR6_P60 == 1)
#define DEVICE0VBUS_INIT bFM4_GPIO_DDR6_P60 = 0
#define DEVICE0VBUS_DEINIT 
#define DEVICE0VBUS_ENABLED 1

#define DEVICE0HCONX_INIT 
#define DEVICE0HCONX_SET 
#define DEVICE0HCONX_CLEAR 
#define DEVICE0HCONX_ENABLED 1

#define DEVICE1VBUS_DISABLEISR 
#define DEVICE1VBUS_ENABLEISR 
#define DEVICE1VBUS_ISRISSET 0
#define DEVICE1VBUS_CLEARISRFLAG 
#define DEVICE1VBUS_SETLOWDETECT 
#define DEVICE1VBUS_SETHIGHDETECT 
#define DEVICE1VBUS_HIGHDETECT 0
#define DEVICE1VBUS_INIT 
#define DEVICE1VBUS_DEINIT
#define DEVICE1VBUS_ENABLED 1

#define DEVICE1HCONX_INIT 
#define DEVICE1HCONX_SET 
#define DEVICE1HCONX_CLEAR 
#define DEVICE1HCONX_ENABLED 1


#endif
