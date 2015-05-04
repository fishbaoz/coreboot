/* $NoKeywords:$ */
/**
 * @file
 *
 * SMU interface to BIOS
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-09 12:28:05 -0600 (Tue, 09 Dec 2014) $
 *
 */
/*
*****************************************************************************
*
* Copyright 2008 - 2015 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
*
* AMD is granting you permission to use this software (the Materials)
* pursuant to the terms and conditions of your Software License Agreement
* with AMD.  This header does *NOT* give you permission to use the Materials
* or any rights under AMD's intellectual property.  Your use of any portion
* of these Materials shall constitute your acceptance of those terms and
* conditions.  If you do not agree to the terms and conditions of the Software
* License Agreement, please do not use any portion of these Materials.
*
* CONFIDENTIALITY:  The Materials and all other information, identified as
* confidential and provided to you by AMD shall be kept confidential in
* accordance with the terms and conditions of the Software License Agreement.
*
* LIMITATION OF LIABILITY: THE MATERIALS AND ANY OTHER RELATED INFORMATION
* PROVIDED TO YOU BY AMD ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
* WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
* MERCHANTABILITY, NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE,
* OR WARRANTIES ARISING FROM CONDUCT, COURSE OF DEALING, OR USAGE OF TRADE.
* IN NO EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER
* (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS
* INTERRUPTION, OR LOSS OF INFORMATION) ARISING OUT OF AMD'S NEGLIGENCE,
* GROSS NEGLIGENCE, THE USE OF OR INABILITY TO USE THE MATERIALS OR ANY OTHER
* RELATED INFORMATION PROVIDED TO YOU BY AMD, EVEN IF AMD HAS BEEN ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE
* EXCLUSION OR LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES,
* THE ABOVE LIMITATION MAY NOT APPLY TO YOU.
*
* AMD does not assume any responsibility for any errors which may appear in
* the Materials or any other related information provided to you by AMD, or
* result from use of the Materials or any related information.
*
* You agree that you will not reverse engineer or decompile the Materials.
*
* NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
* further information, software, technical information, know-how, or show-how
* available to you.  Additionally, AMD retains the right to modify the
* Materials at any time, without notice, and is not obligated to provide such
* modified Materials to you.
*
* U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with
* "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
* subject to the restrictions as set forth in FAR 52.227-14 and
* DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
* Government constitutes acknowledgement of AMD's proprietary rights in them.
*
* EXPORT ASSURANCE:  You agree and certify that neither the Materials, nor any
* direct product thereof will be exported directly or indirectly, into any
* country prohibited by the United States Export Administration Act and the
* regulations thereunder, without the required authorization from the U.S.
* government nor will be used for any purpose prohibited by the same.
* ***************************************************************************
*
*/
#ifndef _GNBBIOSSMC_H_
#define _GNBBIOSSMS_H_

#define BIOSSMC_Result_OK             ((UINT8)0x1)      // The operations was successful.
#define BIOSSMC_Result_NoMore         ((UINT8)0x2)      // The operation cannot be done because a limit has been reached.
                                                          // E.g. trying to enable a HW state when all are enabled already.
                                                          // Depending on the circumstances this might not be an error.
#define BIOSSMC_Result_Failed         ((UINT8)0xFF)     // i.e. -1
#define BIOSSMC_Result_UnknownCmd     ((UINT8)0xFE)     // i.e. -2
#define BIOSSMC_Result_UnknownVT      ((UINT8)0xFD)     // i.e. -3

typedef UINT8 BIOSSMC_Msg;
typedef UINT8 BIOSSMC_Result;

#define BIOSSMC_isERROR(x)                      ((UINT8)0x80 & (x))          // The result code is an actual error.


//BIOSSMC_MSG_EnableAllSmuFeatures
struct BIOSSMC_MSG_EnableAllSmuFeatures_t
{
  UINT32 Mask;
};
typedef struct BIOSSMC_MSG_EnableAllSmuFeatures_t BIOSSMC_MSG_EnableAllSmuFeatures_t;

//BIOSSMC_MSG_DisableAllSmuFeatures
struct BIOSSMC_MSG_DisableAllSmuFeatures_t
{
  UINT32 Mask;
};
typedef struct BIOSSMC_MSG_DisableAllSmuFeatures_t BIOSSMC_MSG_DisableAllSmuFeatures_t;

//BIOSSMC_MSG_VddNbRequest
struct BIOSSMC_MSG_VddNbRequest_t
{
  UINT32 Voltage; //mV with 2 fractional bits
};
typedef struct BIOSSMC_MSG_VddNbRequest_t BIOSSMC_MSG_VddNbRequest_t;

//BIOSSMC_MSG_VddGfxRequest
struct BIOSSMC_MSG_VddGfxRequest_t
{
  UINT32 Voltage; //mV with 2 fractional bits
};
typedef struct BIOSSMC_MSG_VddGfxRequest_t BIOSSMC_MSG_VddGfxRequest_t;

//BIOSSMC_MSG_ReadScs
struct BIOSSMC_MSG_ReadScs_t
{
  UINT32 TablePointer;
};
typedef struct BIOSSMC_MSG_ReadScs_t BIOSSMC_MSG_ReadScs_t;

//BIOSSMC_MSG_LoadPort80MonitorParameters
struct BIOSSMC_MSG_LoadPort80MonitorParameters_t
{
  UINT32 MmioAddress;
  UINT32 MemoryBaseHi;
  UINT32 MemoryBaseLo;
  UINT32 MemoryBufferSize;
  UINT16 PollingInterval; // ms
  UINT8  EnableCsrShadow;
  UINT8  EnableDramShadow;
};
typedef struct BIOSSMC_MSG_LoadPort80MonitorParameters_t GNB_SMU_PORT80_PARAMS;

//BIOSSMC_MSG_LoadTdcLimitParameters
struct BIOSSMC_MSG_LoadTdcLimitParameters_t
{
  UINT32 VddLimit;
  UINT32 VddNbLimit;
  UINT32 VddGfxLimit;
};
typedef struct BIOSSMC_MSG_LoadTdcLimitParameters_t BIOSSMC_MSG_LoadTdcLimitParameters_t;

//BIOSSMC_MSG_LoadEdcLimitParameters
struct BIOSSMC_MSG_LoadEdcLimitParameters_t
{
  UINT32 VddLimit;
  UINT32 VddNbLimit;
  UINT32 VddGfxLimit;
};
typedef struct BIOSSMC_MSG_LoadEdcLimitParameters_t BIOSSMC_MSG_LoadEdcLimitParameters_t;

//BIOSSMC_MSG_LoadCTdpParameters
struct BIOSSMC_MSG_LoadCTdpParameters_t
{
  UINT32 Tdp;
};
typedef struct BIOSSMC_MSG_LoadCTdpParameters_t BIOSSMC_MSG_LoadCTdpParameters_t;

//BIOSSMC_MSG_LoadStapmParameters
struct BIOSSMC_MSG_LoadStapmParameters_t
{
  UINT32    Tsp; //Thermally sustainable power
  UINT32 CoolingTableID;
};
typedef struct BIOSSMC_MSG_LoadStapmParameters_t BIOSSMC_MSG_LoadStapmParameters_t;

//BIOSSMC_MSG_LoadBbbParameters
struct BIOSSMC_MSG_LoadBbbParameters_t
{
  UINT32 PowerLimit;
};
typedef struct BIOSSMC_MSG_LoadBbbParameters_t BIOSSMC_MSG_LoadBbbParameters_t;

//BIOSSMC_MSG_LoadMemoryParameters
struct BIOSSMC_MSG_LoadMemoryParameters_t
{
  UINT32 Vddio;
  UINT32 Vddr;
  UINT32 MemorySpeedHigh; //Mhz
  UINT32 MemorySpeedLow;  //MHz
  UINT32 Dimms;
};
typedef struct BIOSSMC_MSG_LoadMemoryParameters_t BIOSSMC_MSG_LoadMemoryParameters_t;

//BIOSSMC_MSG_LoadPkgPowerParameters
struct BIOSSMC_MSG_LoadPkgPowerParameters_t
{
  UINT32 PowerLimitAC;
  UINT32 PowerLimitDC;
};
typedef struct BIOSSMC_MSG_LoadPkgPowerParameters_t BIOSSMC_MSG_LoadPkgPowerParameters_t;

//BIOSSMC_MSG_LoadSystemConfigParameters
struct BIOSSMC_MSG_LoadSystemConfigParameters_t
{
  UINT32 SystemConfig;
};
typedef struct BIOSSMC_MSG_LoadSystemConfigParameters_t BIOSSMC_MSG_LoadSystemConfigParameters_t;

#endif

