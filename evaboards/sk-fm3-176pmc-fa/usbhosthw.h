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
/** \file usbhosthw.h
 **
 ** USB host hardware settings for sk-fm3-176pmc-fa evaluation board
 **
 ** History:
 **   - 2014-03-12  MSc First Version
 *****************************************************************************/

#ifndef __USBHOSTHW_H__
#define __USBHOSTHW_H__

#define HOST0VBUS_DEINIT 
#define HOST0VBUS_INIT FM3_GPIO->DDR6 |= 0x0004
#define HOST0VBUS_SET FM3_GPIO->PDOR6 &= ~0x0004
#define HOST0VBUS_CLEAR FM3_GPIO->PDOR6 |= 0x0004
#define HOST0VBUS_GET ((FM3_GPIO->PDOR6 & 0x0004) > 0)
#define HOST0VBUS_ENABLED 1

#define HOST0OTGPULLDOWN_DEINIT 
#define HOST0OTGPULLDOWN_INIT FM3_GPIO->DDR5 |= 0x0010
#define HOST0OTGPULLDOWN_SET FM3_GPIO->PDOR5 |= 0x0010
#define HOST0OTGPULLDOWN_CLEAR FM3_GPIO->PDOR5 &= ~0x0010
#define HOST0OTGPULLDOWN_GET ((FM3_GPIO->PDOR5 & 0x0010) > 0)
#define HOST0OTGPULLDOWN_ENABLED 1

#define HOST0OVERCURRENT_DISABLEISR bFM3_EXTI_ENIR_EN2 = 0
#define HOST0OVERCURRENT_ENABLEISR bFM3_EXTI_ENIR_EN2 = 1
#define HOST0OVERCURRENT_CLEARISRFLAG bFM3_EXTI_EICL_ECL2 = 0
#define HOST0OVERCURRENT_ISRISSET bFM3_EXTI_EIRR_ER2 == 1
#define HOST0OVERCURRENT_SETLOWDETECT bFM3_EXTI_ELVR_LA2 = 0
#define HOST0OVERCURRENT_SETHIGHDETECT bFM3_EXTI_ELVR_LA2 = 1
#define HOST0OVERCURRENT_HIGHDETECT (FM3_GPIO->PDIR7 & 0x2000) > 0
#define HOST0OVERCURRENT_INIT FM3_GPIO->DDR7 &= ~0x2000
#define HOST0OVERCURRENT_DEINIT 
#define HOST0OVERCURRENT_ENABLED 1


#define HOST1VBUS_DEINIT
#define HOST1VBUS_INIT
#define HOST1VBUS_SET
#define HOST1VBUS_CLEAR
#define HOST1VBUS_GET 1
#define HOST1VBUS_ENABLED 1

#define HOST1OTGPULLDOWN_DEINIT 
#define HOST1OTGPULLDOWN_INIT 
#define HOST1OTGPULLDOWN_SET 
#define HOST1OTGPULLDOWN_CLEAR 
#define HOST1OTGPULLDOWN_GET 1
#define HOST1OTGPULLDOWN_ENABLED 1

#define HOST1OVERCURRENT_DISABLEISR 
#define HOST1OVERCURRENT_ENABLEISR 
#define HOST1OVERCURRENT_CLEARISRFLAG 
#define HOST1OVERCURRENT_ISRISSET 0
#define HOST1OVERCURRENT_SETLOWDETECT 
#define HOST1OVERCURRENT_SETHIGHDETECT 
#define HOST1OVERCURRENT_HIGHDETECT 0
#define HOST1OVERCURRENT_INIT 
#define HOST1OVERCURRENT_DEINIT
#define HOST0OVERCURRENT_ENABLED 1


#endif
