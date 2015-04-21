; ****************************************************************************
; * @file
; *
; * IdsInternal.inc
; *
; * Contains AMD AGESA Internal Integrated Debug Macros
; *
; * @xrefitem bom "File Content Label" "Release Content"
; * @e project:      AGESA
; * @e sub-project: (Proc/Ids/Internal)
; * @e \$Revision  $   @e \$Date  $
; */
; ****************************************************************************
; *
; * Copyright 2008 - 2015 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
; *
; * AMD is granting you permission to use this software (the Materials)
; * pursuant to the terms and conditions of your Software License Agreement
; * with AMD.  This header does *NOT* give you permission to use the Materials
; * or any rights under AMD's intellectual property.  Your use of any portion
; * of these Materials shall constitute your acceptance of those terms and
; * conditions.  If you do not agree to the terms and conditions of the Software
; * License Agreement, please do not use any portion of these Materials.
; *
; * CONFIDENTIALITY:  The Materials and all other information, identified as
; * confidential and provided to you by AMD shall be kept confidential in
; * accordance with the terms and conditions of the Software License Agreement.
; *
; * LIMITATION OF LIABILITY: THE MATERIALS AND ANY OTHER RELATED INFORMATION
; * PROVIDED TO YOU BY AMD ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
; * WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
; * MERCHANTABILITY, NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE,
; * OR WARRANTIES ARISING FROM CONDUCT, COURSE OF DEALING, OR USAGE OF TRADE.
; * IN NO EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER
; * (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS
; * INTERRUPTION, OR LOSS OF INFORMATION) ARISING OUT OF AMD'S NEGLIGENCE,
; * GROSS NEGLIGENCE, THE USE OF OR INABILITY TO USE THE MATERIALS OR ANY OTHER
; * RELATED INFORMATION PROVIDED TO YOU BY AMD, EVEN IF AMD HAS BEEN ADVISED OF
; * THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE
; * EXCLUSION OR LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES,
; * THE ABOVE LIMITATION MAY NOT APPLY TO YOU.
; *
; * AMD does not assume any responsibility for any errors which may appear in
; * the Materials or any other related information provided to you by AMD, or
; * result from use of the Materials or any related information.
; *
; * You agree that you will not reverse engineer or decompile the Materials.
; *
; * NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
; * further information, software, technical information, know-how, or show-how
; * available to you.  Additionally, AMD retains the right to modify the
; * Materials at any time, without notice, and is not obligated to provide such
; * modified Materials to you.
; *
; * U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with
; * "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
; * subject to the restrictions as set forth in FAR 52.227-14 and
; * DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
; * Government constitutes acknowledgement of AMD's proprietary rights in them.
; *
; * EXPORT ASSURANCE:  You agree and certify that neither the Materials, nor any
; * direct product thereof will be exported directly or indirectly, into any
; * country prohibited by the United States Export Administration Act and the
; * regulations thereunder, without the required authorization from the U.S.
; * government nor will be used for any purpose prohibited by the same.
; *
; **************************************************************************


  AGESA_IDS_INT_ID_START                            EQU    0024h; <                                                            specify the start of internal NV id

  AGESA_IDS_NV_ACPI_SRAT                                      EQU    0025h; <                                                         Enable or disable the ACPI SRAT tables
  AGESA_IDS_NV_CORE_LEVEL                                     EQU    0026h; <                                                             Set the number of Cores to be used

  AGESA_IDS_NV_CACHEFLUSHHLT                                  EQU    0027h; <                                  Set the core clock divisor after the caches have been flushed
  AGESA_IDS_NV_CACHEFLUSHHLTTMR                               EQU    0028h; <                                                Set the time of each core needs to stay in halt
  AGESA_IDS_NV_CACHEFLUSHHLTDIV                               EQU    0029h; <                                                              Cache Flush on Halt Clock divisor
  AGESA_IDS_NV_CACHEFLUSHTMR                                  EQU    002Ah; <                                                                              Cache Flush Timer
  AGESA_IDS_NV_CFSUCCESSMONCTL                                EQU    002Bh; <                                                            Cache Flush Success Monitor Control
  AGESA_IDS_NV_CACHEFLUSHSUCMONTHRESHOLD                      EQU    002Ch; <                                                          Cache flush success monitor threshold

  AGESA_IDS_NV_DCACHE                                         EQU    002Dh; <                                                                      Enable or disable D-Cache

  AGESA_IDS_NV_ACPI_PSS                                       EQU    002Eh; <                                                             Enable or disable ACPI PSS objects
  AGESA_IDS_NV_ACPI_CST                                       EQU    002Fh; <                                                                       Disable ACPI CST objects
  AGESA_IDS_NV_ACPI_WHEA                                      EQU    0030h; <                                                            Enable or disable ACPI WHEA objects
  AGESA_IDS_NV_ACPI_SLIT                                      EQU    0031h; <                                                            Enable or disable ACPI SLIT objects
  AGESA_IDS_NV_DMIEN                                          EQU    0032h; <                                                          Controls activation of CPU DMI Table!

  AGESA_IDS_NV_CUSTOM_PSTATE                                  EQU    0033h; <                                                              whether to configure the P-States
  AGESA_IDS_NV_NUMCUSTOMPSTATES                               EQU    0034h; <                                                                 Defines the number of P-states
  AGESA_IDS_NV_PSTATENBFID                                    EQU    0035h; <                                                                                 Set the NB FID

  AGESA_IDS_NV_PSTATE0COREFID                                 EQU    0036h; <                                                               Set the FID for specified Pstate
  AGESA_IDS_NV_PSTATE0COREVID                                 EQU    0037h; <                                                               Set the VID for specified Pstate
  AGESA_IDS_NV_PSTATE0COREDID                                 EQU    0038h; <                                                               Set the DID for specified Pstate
  AGESA_IDS_NV_PSTATE0COREDIDLSD                              EQU    0039h; <                                                           Set the DID LSD for specified Pstate
  AGESA_IDS_NV_PSTATE0NBVID                                   EQU    003Ah; <                                                             Set the NBVID for specified Pstate
  AGESA_IDS_NV_PSTATE0NBDID                                   EQU    003Bh; <                                                             Set the NBDID for specified Pstate
  AGESA_IDS_NV_PSTATE0NBFID                                   EQU    003Ch; <                                                             Set the NBFID for specified Pstate

  AGESA_IDS_NV_PSTATE1COREFID                                 EQU    003Dh; <                                                               Set the FID for specified Pstate
  AGESA_IDS_NV_PSTATE1COREVID                                 EQU    003Eh; <                                                               Set the VID for specified Pstate
  AGESA_IDS_NV_PSTATE1COREDID                                 EQU    003Fh; <                                                               Set the DID for specified Pstate
  AGESA_IDS_NV_PSTATE1COREDIDLSD                              EQU    0040h; <                                                           Set the DID LSD for specified Pstate
  AGESA_IDS_NV_PSTATE1NBVID                                   EQU    0041h; <                                                             Set the NBVID for specified Pstate
  AGESA_IDS_NV_PSTATE1NBDID                                   EQU    0042h; <                                                             Set the NBDID for specified Pstate
  AGESA_IDS_NV_PSTATE1NBFID                                   EQU    0043h; <                                                             Set the NBFID for specified Pstate

  AGESA_IDS_NV_PSTATE2COREFID                                 EQU    0044h; <                                                               Set the FID for specified Pstate
  AGESA_IDS_NV_PSTATE2COREVID                                 EQU    0045h; <                                                               Set the VID for specified Pstate
  AGESA_IDS_NV_PSTATE2COREDID                                 EQU    0046h; <                                                               Set the DID for specified Pstate
  AGESA_IDS_NV_PSTATE2COREDIDLSD                              EQU    0047h; <                                                           Set the DID LSD for specified Pstate
  AGESA_IDS_NV_PSTATE2NBVID                                   EQU    0048h; <                                                             Set the NBVID for specified Pstate
  AGESA_IDS_NV_PSTATE2NBDID                                   EQU    0049h; <                                                             Set the NBDID for specified Pstate
  AGESA_IDS_NV_PSTATE2NBFID                                   EQU    004Ah; <                                                             Set the NBFID for specified Pstate

  AGESA_IDS_NV_PSTATE3COREFID                                 EQU    004Bh; <                                                               Set the FID for specified Pstate
  AGESA_IDS_NV_PSTATE3COREVID                                 EQU    004Ch; <                                                               Set the VID for specified Pstate
  AGESA_IDS_NV_PSTATE3COREDID                                 EQU    004Dh; <                                                               Set the DID for specified Pstate
  AGESA_IDS_NV_PSTATE3COREDIDLSD                              EQU    004Eh; <                                                           Set the DID LSD for specified Pstate
  AGESA_IDS_NV_PSTATE3NBVID                                   EQU    004Fh; <                                                             Set the NBVID for specified Pstate
  AGESA_IDS_NV_PSTATE3NBDID                                   EQU    0050h; <                                                             Set the NBDID for specified Pstate
  AGESA_IDS_NV_PSTATE3NBFID                                   EQU    0051h; <                                                             Set the NBFID for specified Pstate

  AGESA_IDS_NV_PSTATE4COREFID                                 EQU    0052h; <                                                               Set the FID for specified Pstate
  AGESA_IDS_NV_PSTATE4COREVID                                 EQU    0053h; <                                                               Set the VID for specified Pstate
  AGESA_IDS_NV_PSTATE4COREDID                                 EQU    0054h; <                                                               Set the DID for specified Pstate
  AGESA_IDS_NV_PSTATE4COREDIDLSD                              EQU    0055h; <                                                           Set the DID LSD for specified Pstate
  AGESA_IDS_NV_PSTATE4NBVID                                   EQU    0056h; <                                                             Set the NBVID for specified Pstate
  AGESA_IDS_NV_PSTATE4NBDID                                   EQU    0057h; <                                                             Set the NBDID for specified Pstate
  AGESA_IDS_NV_PSTATE4NBFID                                   EQU    0058h; <                                                             Set the NBFID for specified Pstate

  AGESA_IDS_NV_PSTATE5COREFID                                 EQU    0059h; <                                                               Set the FID for specified Pstate
  AGESA_IDS_NV_PSTATE5COREVID                                 EQU    005Ah; <                                                               Set the VID for specified Pstate
  AGESA_IDS_NV_PSTATE5COREDID                                 EQU    005Bh; <                                                               Set the DID for specified Pstate
  AGESA_IDS_NV_PSTATE5COREDIDLSD                              EQU    005Ch; <                                                           Set the DID LSD for specified Pstate
  AGESA_IDS_NV_PSTATE5NBVID                                   EQU    005Dh; <                                                             Set the NBVID for specified Pstate
  AGESA_IDS_NV_PSTATE5NBDID                                   EQU    005Eh; <                                                             Set the NBDID for specified Pstate
  AGESA_IDS_NV_PSTATE5NBFID                                   EQU    005Fh; <                                                             Set the NBFID for specified Pstate

  AGESA_IDS_NV_PSTATE6COREFID                                 EQU    0060h; <                                                               Set the FID for specified Pstate
  AGESA_IDS_NV_PSTATE6COREVID                                 EQU    0061h; <                                                               Set the VID for specified Pstate
  AGESA_IDS_NV_PSTATE6COREDID                                 EQU    0062h; <                                                               Set the DID for specified Pstate
  AGESA_IDS_NV_PSTATE6COREDIDLSD                              EQU    0063h; <                                                           Set the DID LSD for specified Pstate
  AGESA_IDS_NV_PSTATE6NBVID                                   EQU    0064h; <                                                             Set the NBVID for specified Pstate
  AGESA_IDS_NV_PSTATE6NBDID                                   EQU    0065h; <                                                             Set the NBDID for specified Pstate
  AGESA_IDS_NV_PSTATE6NBFID                                   EQU    0066h; <                                                             Set the NBFID for specified Pstate

  AGESA_IDS_NV_PSTATE7COREFID                                 EQU    0067h; <                                                               Set the FID for specified Pstate
  AGESA_IDS_NV_PSTATE7COREVID                                 EQU    0068h; <                                                               Set the VID for specified Pstate
  AGESA_IDS_NV_PSTATE7COREDID                                 EQU    0069h; <                                                               Set the DID for specified Pstate
  AGESA_IDS_NV_PSTATE7COREDIDLSD                              EQU    006Ah; <                                                           Set the DID LSD for specified Pstate
  AGESA_IDS_NV_PSTATE7NBVID                                   EQU    006Bh; <                                                             Set the NBVID for specified Pstate
  AGESA_IDS_NV_PSTATE7NBDID                                   EQU    006Ch; <                                                             Set the NBDID for specified Pstate
  AGESA_IDS_NV_PSTATE7NBFID                                   EQU    006Dh; <                                                             Set the NBFID for specified Pstate

  AGESA_IDS_NV_PSIEN                                          EQU    006Eh; <                                                                          Enable or disable PSI


  AGESA_IDS_NV_MEMORY_PARITY                                  EQU    006Fh; <                                                              Enable or disable the DRAM Parity

  AGESA_IDS_NV_CLKHZALTVIDC3EN                                EQU    0070h; <                                                      Enable or disable the AltVidC3MemClkTriEn

  AGESA_IDS_NV_SYSMEMSIZEOVERLAYEN                            EQU    0071h; <                                                   Enable or disable System Memory Size Overlay
  AGESA_IDS_NV_SYSMEMSIZE                                     EQU    0072h; <                                                                     System Memory Size Overlay

  AGESA_IDS_NV_INTSWAPEN                                      EQU    0073h; <                                                                  Interleave region swap enable
  AGESA_IDS_NV_SWAPBASE                                       EQU    0074h; <                                                Interleave swap region base address bits[33:27]
  AGESA_IDS_NV_SWAPLIMIT                                      EQU    0075h; <                                               Interleave swap region limit address bits[33:27]
  AGESA_IDS_NV_SWAPSIZE                                       EQU    0076h; <                                                        Interleave swap region size bits[33:27]

  AGESA_IDS_NV_POR_MEM_FREQ                                   EQU    0077h; <                                                  Enable of diable MemClk frequency enforcement
  AGESA_IDS_NV_DRAMTIME                                       EQU    0078h; <                                                              Set the DRAM Timing Configuration
  AGESA_IDS_NV_MEMCLKSPEED                                    EQU    0079h; <                                                                     Set the memory clock speed
  AGESA_IDS_NV_MEMSPEED                                       EQU    007Ah; <                                                                 Set the memory clock frequency
  AGESA_IDS_NV_RDPTRDDR3                                      EQU    007Bh; <                                                                                          RdPtr
  AGESA_IDS_NV_TCL                                            EQU    007Ch; <                                                                               Set the tCL time
  AGESA_IDS_NV_TRCD                                           EQU    007Dh; <                                              This Set the RAS# Active to CAS# read/write delay
  AGESA_IDS_NV_TRP                                            EQU    007Eh; <                                                                 Specify the row precharge time
  AGESA_IDS_NV_TRTP                                           EQU    007Fh; <                                                      Specifies the read CAS# to precharge time
  AGESA_IDS_NV_TRAS                                           EQU    0080h; <                                                               Specify the min RAS# active time
  AGESA_IDS_NV_TRRD                                           EQU    0081h; <                                                             Specify the active-to-active delay
  AGESA_IDS_NV_TRC                                            EQU    0082h; <                                                    Specify the RAS# active to RAS# active time
  AGESA_IDS_NV_TRWTTO                                         EQU    0083h; <                                                                       This Set the tRWTTO time
  AGESA_IDS_NV_TWR                                            EQU    0084h; <                                          Specify when the last write is registered by the DRAM
  AGESA_IDS_NV_TWRRD                                          EQU    0085h; <                                          Specify when the last write is registered by the DRAM
  AGESA_IDS_NV_TWTR                                           EQU    0086h; <                                   Specify the write to read delay when accessing the same DIMM
  AGESA_IDS_NV_TWRWR                                          EQU    0087h; <                                                                         Specify the Twrwr time
  AGESA_IDS_NV_TWRWRSDSC                                      EQU    0088h; <                                                                                      tWRWRSdSc
  AGESA_IDS_NV_TWRWRSDDC                                      EQU    0089h; <                                                                                      tWRWRSdDc
  AGESA_IDS_NV_TWRWRDD                                        EQU    008Ah; <                                                                                        tWRWRDd
  AGESA_IDS_NV_TRDRD                                          EQU    008Bh; <                                                                         Specify the Trdrd time
  AGESA_IDS_NV_TRDRDSDSC                                      EQU    008Ch; <                                                                                      tRDRDSdSc
  AGESA_IDS_NV_TRDRDSDDC                                      EQU    008Dh; <                                                                                      tRDRDSdDc
  AGESA_IDS_NV_TRDRDDD                                        EQU    008Eh; <                                                                                        tRDRDDd
  AGESA_IDS_NV_TREF                                           EQU    008Fh; <                                                                                           tREF
  AGESA_IDS_NV_TRWTWB                                         EQU    0090h; <                                                                        Specify the Trwtwb time
  AGESA_IDS_NV_TRFCx                                          EQU    0091h; <                                                                         Specify the Trfcx time
  AGESA_IDS_NV_TRFC0                                          EQU    0092h; <                                                                         Specify the Trfc0 time
  AGESA_IDS_NV_TRFC1                                          EQU    0093h; <                                                                         Specify the Trfc1 time
  AGESA_IDS_NV_TRFC2                                          EQU    0094h; <                                                                         Specify the Trfc2 time
  AGESA_IDS_NV_TRFC3                                          EQU    0095h; <                                                                         Specify the Trfc3 time

  AGESA_IDS_NV_DRAMCONTRSEL                                   EQU    0096h; <                                                                     Select the DRAM Controller
  AGESA_IDS_NV_PCHGPDMODESEL                                  EQU    0097h; <                                                               Precharge power down mode select
  AGESA_IDS_NV_DIMMODT                                        EQU    0098h; <                                                                     Set the On Die Termination
  AGESA_IDS_NV_DRAMDRVWEAK                                    EQU    0099h; <                                                                    Set the DRAM drive strength
  AGESA_IDS_NV_DRAMSELFREFRATEEN                              EQU    009Ah; <                                                   Enable or disable the DRAM Self Refresh Rate

  AGESA_IDS_NV_CMDMOTIME                                      EQU    009Bh; <                                                      Select between 1T and 2T mode on ADDR/CMD
  AGESA_IDS_NV_BANK_SWIZZLE                                   EQU    009Ch; <                                                            Enable or disable bank swizzle mode
  AGESA_IDS_NV_THROTTLEDCT                                    EQU    009Dh; <           When MEMHOT_L pin is asserted throttle both DCT channels to the  average utilization
  AGESA_IDS_NV_TREFRATEDCT                                    EQU    009Eh; <                                When MEMHOT_L pin is asserted Tref forced to 3.9us auto refresh
  AGESA_IDS_NV_LOAD_CARD_MODE                                 EQU    009Fh; <                                                   Set where the load card *will* be swapped in
  AGESA_IDS_NV_LOAD_CARD_ACTIVE                               EQU    00A0h; <                                              Specify if the load card is swapped in the system

  AGESA_IDS_NV_MemClr                                         EQU    00A1h; <                                                                       Enable or disable MemClr
  AGESA_IDS_NV_PhyDLLStandby                                  EQU    00A2h; <                                                                Enable or disable PhyDLLStandby
  AGESA_IDS_NV_RxDLLWakeupTime                                EQU    00A3h; <                                                                        Set the RxDLLWakeupTime
  AGESA_IDS_NV_RxCPUpdPeriod                                  EQU    00A4h; <                                                                          Set the RxCPUpdPeriod
  AGESA_IDS_NV_RxMaxDurDllNoLock                              EQU    00A5h; <                                                                      Set the RxMaxDurDllNoLock
  AGESA_IDS_NV_TxDLLWakeupTime                                EQU    00A6h; <                                                                        Set the TxDLLWakeupTime
  AGESA_IDS_NV_TxCPUpdPeriod                                  EQU    00A7h; <                                                                          Set the TxCPUpdPeriod
  AGESA_IDS_NV_TxMaxDurDllNoLock                              EQU    00A8h; <                                                                      Set the TxMaxDurDllNoLock
  AGESA_IDS_NV_MEM_ERROR_RECOVERY                             EQU    00A9h; <                                                          Specify if error recovery is enabled.
  AGESA_IDS_NV_MEM_RETRAIN_TIMES                              EQU    00AAh; <                         Specify how many times training should be executed should error occurs
  AGESA_IDS_NV_DLLRegDis                                      EQU    00ABh; <                                                        Enable or disable DLL Regulator Disable

  AGESA_IDS_NV_DRSTRCON                                       EQU    00ACh; <                                                 Set the mode for Drive Strength initialization
  AGESA_IDS_NV_CKEDRSTR                                       EQU    00ADh; <                                                                     Set the CKE drive strength
  AGESA_IDS_NV_CODRSTR                                        EQU    00AEh; <                                                                  Set the CS/ODT drive strength
  AGESA_IDS_NV_ACDRSTR                                        EQU    00AFh; <                                                                Set the Addr/Cmd drive strength
  AGESA_IDS_NV_CLKDRSTR                                       EQU    00B0h; <                                                                   Set the Clock drive strength
  AGESA_IDS_NV_DTDRSTR                                        EQU    00B1h; <                                                                    Set the data drive strength
  AGESA_IDS_NV_DQSDRSTR                                       EQU    00B2h; <                                                                     Set the DQS drive strength
  AGESA_IDS_NV_DQSPROCODT                                     EQU    00B3h; <                                                                             Set the ProcOdtDis

  AGESA_IDS_NV_DQSDELAYS                                      EQU    00B4h; <                                                    Set the mode for DQS training configuration
  AGESA_IDS_NV_WRBYTEDL0                                      EQU    00B5h; <                                                           Set the write data fine delay byte 0
  AGESA_IDS_NV_WRBYTEDLG0                                     EQU    00B6h; <                                                          Set the write data gross delay byte 0
  AGESA_IDS_NV_WRBYTEDL1                                      EQU    00B7h; <                                                           Set the write data fine delay byte 1
  AGESA_IDS_NV_WRBYTEDLG1                                     EQU    00B8h; <                                                          Set the write data gross delay byte 1
  AGESA_IDS_NV_WRBYTEDL2                                      EQU    00B9h; <                                                           Set the write data fine delay byte 2
  AGESA_IDS_NV_WRBYTEDLG2                                     EQU    00BAh; <                                                          Set the write data gross delay byte 2
  AGESA_IDS_NV_WRBYTEDL3                                      EQU    00BBh; <                                                           Set the write data fine delay byte 3
  AGESA_IDS_NV_WRBYTEDLG3                                     EQU    00BCh; <                                                          Set the write data gross delay byte 3
  AGESA_IDS_NV_WRBYTEDL4                                      EQU    00BDh; <                                                           Set the write data fine delay byte 4
  AGESA_IDS_NV_WRBYTEDLG4                                     EQU    00BEh; <                                                          Set the write data gross delay byte 4
  AGESA_IDS_NV_WRBYTEDL5                                      EQU    00BFh; <                                                           Set the write data fine delay byte 5
  AGESA_IDS_NV_WRBYTEDLG5                                     EQU    00C0h; <                                                          Set the write data gross delay byte 5
  AGESA_IDS_NV_WRBYTEDL6                                      EQU    00C1h; <                                                           Set the write data fine delay byte 6
  AGESA_IDS_NV_WRBYTEDLG6                                     EQU    00C2h; <                                                          Set the write data gross delay byte 6
  AGESA_IDS_NV_WRBYTEDL7                                      EQU    00C3h; <                                                           Set the write data fine delay byte 7
  AGESA_IDS_NV_WRBYTEDLG7                                     EQU    00C4h; <                                                          Set the write data gross delay byte 7
  AGESA_IDS_NV_WRDTECCDL                                      EQU    00C5h; <                                                              Set the write data ECC fine delay
  AGESA_IDS_NV_WRDTECCDLG                                     EQU    00C6h; <                                                             Set the write data ECC gross delay
  AGESA_IDS_NV_RDDQSBTDL0                                     EQU    00C7h; <                                                                  Set the Read DQS Byte 0 Delay
  AGESA_IDS_NV_RDDQSBTDL1                                     EQU    00C8h; <                                                                  Set the Read DQS Byte 1 Delay
  AGESA_IDS_NV_RDDQSBTDL2                                     EQU    00C9h; <                                                                  Set the Read DQS Byte 2 Delay
  AGESA_IDS_NV_RDDQSBTDL3                                     EQU    00CAh; <                                                                  Set the Read DQS Byte 3 Delay
  AGESA_IDS_NV_RDDQSBTDL4                                     EQU    00CBh; <                                                                  Set the Read DQS Byte 4 Delay
  AGESA_IDS_NV_RDDQSBTDL5                                     EQU    00CCh; <                                                                  Set the Read DQS Byte 5 Delay
  AGESA_IDS_NV_RDDQSBTDL6                                     EQU    00CDh; <                                                                  Set the Read DQS Byte 6 Delay
  AGESA_IDS_NV_RDDQSBTDL7                                     EQU    00CEh; <                                                                  Set the Read DQS Byte 7 Delay
  AGESA_IDS_NV_RDDQSECCDL                                     EQU    00CFh; <                                                                     Set the Read DQS ECC Delay
  AGESA_IDS_NV_CKEFNDL                                        EQU    00D0h; <                                                                         Set the CKE Fine Delay
  AGESA_IDS_NV_CKESETUPTIME                                   EQU    00D1h; <                                                                         Set the CKE Setup Time
  AGESA_IDS_NV_COFNDL                                         EQU    00D2h; <                                                                      Set the CS/ODT Fine Delay
  AGESA_IDS_NV_COSETUPTIME                                    EQU    00D3h; <                                                                      Set the CS/ODT Setup Time
  AGESA_IDS_NV_ADDRCMDDNDL                                    EQU    00D4h; <                                                                    Set the Addr/Cmd Fine Delay
  AGESA_IDS_NV_ADDRCMDSETUPTIME                               EQU    00D5h; <                                                                    Set the Addr/Cmd Setup Time

  AGESA_IDS_NV_MEMDQSWRBYTE0CTL                               EQU    00D6h; <                                                                      Write Data Byte 0 Control
  AGESA_IDS_NV_MEMDQSWRBYTE1CTL                               EQU    00D7h; <                                                                      Write Data Byte 1 Control
  AGESA_IDS_NV_MEMDQSWRBYTE2CTL                               EQU    00D8h; <                                                                      Write Data Byte 2 Control
  AGESA_IDS_NV_MEMDQSWRBYTE3CTL                               EQU    00D9h; <                                                                      Write Data Byte 3 Control
  AGESA_IDS_NV_MEMDQSWRBYTE4CTL                               EQU    00DAh; <                                                                      Write Data Byte 4 Control
  AGESA_IDS_NV_MEMDQSWRBYTE5CTL                               EQU    00DBh; <                                                                      Write Data Byte 5 Control
  AGESA_IDS_NV_MEMDQSWRBYTE6CTL                               EQU    00DCh; <                                                                      Write Data Byte 6 Control
  AGESA_IDS_NV_MEMDQSWRBYTE7CTL                               EQU    00DDh; <                                                                      Write Data Byte 7 Control
  AGESA_IDS_NV_MEMDQSRDDQSBT0CTL                              EQU    00DEh; <                                                                        Read DQS Byte 0 Control
  AGESA_IDS_NV_MEMDQSRDDQSBT1CTL                              EQU    00DFh; <                                                                        Read DQS Byte 1 Control
  AGESA_IDS_NV_MEMDQSRDDQSBT2CTL                              EQU    00E0h; <                                                                        Read DQS Byte 2 Control
  AGESA_IDS_NV_MEMDQSRDDQSBT3CTL                              EQU    00E1h; <                                                                        Read DQS Byte 3 Control
  AGESA_IDS_NV_MEMDQSRDDQSBT4CTL                              EQU    00E2h; <                                                                        Read DQS Byte 4 Control
  AGESA_IDS_NV_MEMDQSRDDQSBT5CTL                              EQU    00E3h; <                                                                        Read DQS Byte 5 Control
  AGESA_IDS_NV_MEMDQSRDDQSBT6CTL                              EQU    00E4h; <                                                                        Read DQS Byte 6 Control
  AGESA_IDS_NV_MEMDQSRDDQSBT7CTL                              EQU    00E5h; <                                                                        Read DQS Byte 7 Control
  AGESA_IDS_NV_MEMDQSCKEFINEDELAYCTL                          EQU    00E6h; <                                                                         CKE Fine Delay Control
  AGESA_IDS_NV_MEMDQSCSODTFINEDELAYCTL                        EQU    00E7h; <                                                                      CS/ODT Fine Delay Control
  AGESA_IDS_NV_MEMDQSADDRCMDFINEDELAYCTL                      EQU    00E8h; <                                                                    Addr/Cmd Fine Delay Control


  AGESA_IDS_NV_ALWAYS_USE_ACTIVE_DQS_TRAINING                 EQU    00E9h; <                                      Enable or disable Read and Write DQS Training data output

  AGESA_IDS_NV_CPUDEEMPHASIS                                  EQU    00EAh; <                                                                                  DcvDeemphasis
  AGESA_IDS_NV_NBDEEMPHASIS                                   EQU    00EBh; <                                                                             ReceiverDeemphasis

  AGESA_IDS_NV_REGISTER1D4L                                   EQU    00ECh; <                                                                                   F3x1D4[15:0]
  AGESA_IDS_NV_REGISTER1D4H                                   EQU    00EDh; <                                                                                  F3x1D4[31:16]
  AGESA_IDS_NV_REGISTER1D8L                                   EQU    00EEh; <                                                                                   F3x1D8[15:0]
  AGESA_IDS_NV_REGISTER1D8H                                   EQU    00EFh; <                                                                                  F3x1D8[23:16]

  AGESA_IDS_NV_GENREG1E                                       EQU    00F0h; <                                                            Enable or disable register access A
  AGESA_IDS_NV_GENREG1T                                       EQU    00F1h; <                                                             Select the type of register to set
  AGESA_IDS_NV_GENREG1W                                       EQU    00F2h; <                                                             When to modify the value specified
  AGESA_IDS_NV_GENREG1PCIANODES                               EQU    00F3h; <                                The PCI setting should be applied to all nodes or a single node
  AGESA_IDS_NV_GENREG1MSRACORES                               EQU    00F4h; <                                                 The MSR setting should be applied to all cores
  AGESA_IDS_NV_GENREG1MSRNODE                                 EQU    00F5h; <                                           The MSR setting should be applied to a selector node
  AGESA_IDS_NV_GENREG1MSRCORE                                 EQU    00F6h; <                                          The MSR setting should be applied to a selected cores
  AGESA_IDS_NV_GENREG1BUS                                     EQU    00F7h; <                                                        Set the Bus number when the type is PCI
  AGESA_IDS_NV_GENREG1DEVICE                                  EQU    00F8h; <                                                     Set the Device number when the type is PCI
  AGESA_IDS_NV_GENREG1FUNC                                    EQU    00F9h; <                                                   Set the Function number when the type is PCI
  AGESA_IDS_NV_GENREG1OFFSET                                  EQU    00FAh; <                                                            Set the offset when the type is PCI
  AGESA_IDS_NV_MSRHIGH1                                       EQU    00FBh; <                                                      Set the high address when the type is MSR
  AGESA_IDS_NV_MSRLOW1                                        EQU    00FCh; <                                                       Set the low address when the type is MSR
  AGESA_IDS_NV_GENREG1ORHIGHEST                               EQU    00FDh; <                                                             The highest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG1ORHIGHER                                EQU    00FEh; <                                                              Bits 32 through 47 of the OR mask
  AGESA_IDS_NV_GENREG1ORLOWER                                 EQU    00FFh; <                                                              Bits 16 through 31 of the OR mask
  AGESA_IDS_NV_GENREG1ORLOWEST                                EQU    0100h; <                                                              The lowest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG1ANDHIGHEST                              EQU    0101h; <                                                            The highest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG1ANDHIGHER                               EQU    0102h; <                                                             Bits 32 through 47 of the AND mask
  AGESA_IDS_NV_GENREG1ANDLOWER                                EQU    0103h; <                                                             Bits 16 through 31 of the AND mask
  AGESA_IDS_NV_GENREG1ANDLOWEST                               EQU    0104h; <                                                             The lowest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG1MMIOBASEHIGH                            EQU    0105h; <                                                                                 MMIO base High
  AGESA_IDS_NV_GENREG1MMIOBASELOW                             EQU    0106h; <                                                                                  MMIO base low
  AGESA_IDS_NV_GENREG1PCI_INDIRECT_OFF_HIGH                   EQU    0107h; <                                                                       PCI indirect Offset High
  AGESA_IDS_NV_GENREG1PCI_INDIRECT_OFF_LOW                    EQU    0108h; <                                                                        PCI indirect Offset Low
  AGESA_IDS_NV_GENREG1PCI_INDIRECT_WEBIT                      EQU    0109h; <                                                                  PCI indirect Write Enable bit
  AGESA_IDS_NV_GENREG1GMMX_OFF_HIGH                           EQU    010Ah; <                                                                               GMMX offset High
  AGESA_IDS_NV_GENREG1GMMX_OFF_LOW                            EQU    010Bh; <                                                                                GMMX offset low

  AGESA_IDS_NV_GENREG2E                                       EQU    010Ch; <                                                            Enable or disable register access B
  AGESA_IDS_NV_GENREG2T                                       EQU    010Dh; <                                                             Select the type of register to set
  AGESA_IDS_NV_GENREG2W                                       EQU    010Eh; <                                                             When to modify the value specified
  AGESA_IDS_NV_GENREG2PCIANODES                               EQU    010Fh; <                                The PCI setting should be applied to all nodes or a single node
  AGESA_IDS_NV_GENREG2MSRACORES                               EQU    0110h; <                                                 The MSR setting should be applied to all cores
  AGESA_IDS_NV_GENREG2MSRNODE                                 EQU    0111h; <                                           The MSR setting should be applied to a selector node
  AGESA_IDS_NV_GENREG2MSRCORE                                 EQU    0112h; <                                          The MSR setting should be applied to a selected cores
  AGESA_IDS_NV_GENREG2BUS                                     EQU    0113h; <                                                        Set the Bus number when the type is PCI
  AGESA_IDS_NV_GENREG2DEVICE                                  EQU    0114h; <                                                     Set the Device number when the type is PCI
  AGESA_IDS_NV_GENREG2FUNC                                    EQU    0115h; <                                                   Set the Function number when the type is PCI
  AGESA_IDS_NV_GENREG2OFFSET                                  EQU    0116h; <                                                            Set the offset when the type is PCI
  AGESA_IDS_NV_MSRHIGH2                                       EQU    0117h; <                                                      Set the high address when the type is MSR
  AGESA_IDS_NV_MSRLOW2                                        EQU    0118h; <                                                       Set the low address when the type is MSR
  AGESA_IDS_NV_GENREG2ORHIGHEST                               EQU    0119h; <                                                             The highest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG2ORHIGHER                                EQU    011Ah; <                                                              Bits 32 through 47 of the OR mask
  AGESA_IDS_NV_GENREG2ORLOWER                                 EQU    011Bh; <                                                              Bits 16 through 31 of the OR mask
  AGESA_IDS_NV_GENREG2ORLOWEST                                EQU    011Ch; <                                                              The lowest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG2ANDHIGHEST                              EQU    011Dh; <                                                            The highest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG2ANDHIGHER                               EQU    011Eh; <                                                             Bits 32 through 47 of the AND mask
  AGESA_IDS_NV_GENREG2ANDLOWER                                EQU    011Fh; <                                                             Bits 16 through 31 of the AND mask
  AGESA_IDS_NV_GENREG2ANDLOWEST                               EQU    0120h; <                                                             The lowest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG2MMIOBASEHIGH                            EQU    0121h; <                                                                                 MMIO base High
  AGESA_IDS_NV_GENREG2MMIOBASELOW                             EQU    0122h; <                                                                                  MMIO base low
  AGESA_IDS_NV_GENREG2PCI_INDIRECT_OFF_HIGH                   EQU    0123h; <                                                                       PCI indirect Offset High
  AGESA_IDS_NV_GENREG2PCI_INDIRECT_OFF_LOW                    EQU    0124h; <                                                                        PCI indirect Offset Low
  AGESA_IDS_NV_GENREG2PCI_INDIRECT_WEBIT                      EQU    0125h; <                                                                  PCI indirect Write Enable bit
  AGESA_IDS_NV_GENREG2GMMX_OFF_HIGH                           EQU    0126h; <                                                                               GMMX offset High
  AGESA_IDS_NV_GENREG2GMMX_OFF_LOW                            EQU    0127h; <                                                                                GMMX offset low

  AGESA_IDS_NV_GENREG3E                                       EQU    0128h; <                                                            Enable or disable register access C
  AGESA_IDS_NV_GENREG3T                                       EQU    0129h; <                                                             Select the type of register to set
  AGESA_IDS_NV_GENREG3W                                       EQU    012Ah; <                                                             When to modify the value specified
  AGESA_IDS_NV_GENREG3PCIANODES                               EQU    012Bh; <                                The PCI setting should be applied to all nodes or a single node
  AGESA_IDS_NV_GENREG3MSRACORES                               EQU    012Ch; <                                                 The MSR setting should be applied to all cores
  AGESA_IDS_NV_GENREG3MSRNODE                                 EQU    012Dh; <                                           The MSR setting should be applied to a selector node
  AGESA_IDS_NV_GENREG3MSRCORE                                 EQU    012Eh; <                                          The MSR setting should be applied to a selected cores
  AGESA_IDS_NV_GENREG3BUS                                     EQU    012Fh; <                                                        Set the Bus number when the type is PCI
  AGESA_IDS_NV_GENREG3DEVICE                                  EQU    0130h; <                                                     Set the Device number when the type is PCI
  AGESA_IDS_NV_GENREG3FUNC                                    EQU    0131h; <                                                   Set the Function number when the type is PCI
  AGESA_IDS_NV_GENREG3OFFSET                                  EQU    0132h; <                                                            Set the offset when the type is PCI
  AGESA_IDS_NV_MSRHIGH3                                       EQU    0133h; <                                                      Set the high address when the type is MSR
  AGESA_IDS_NV_MSRLOW3                                        EQU    0134h; <                                                       Set the low address when the type is MSR
  AGESA_IDS_NV_GENREG3ORHIGHEST                               EQU    0135h; <                                                             The highest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG3ORHIGHER                                EQU    0136h; <                                                              Bits 32 through 47 of the OR mask
  AGESA_IDS_NV_GENREG3ORLOWER                                 EQU    0137h; <                                                              Bits 16 through 31 of the OR mask
  AGESA_IDS_NV_GENREG3ORLOWEST                                EQU    0138h; <                                                              The lowest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG3ANDHIGHEST                              EQU    0139h; <                                                            The highest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG3ANDHIGHER                               EQU    013Ah; <                                                             Bits 32 through 47 of the AND mask
  AGESA_IDS_NV_GENREG3ANDLOWER                                EQU    013Bh; <                                                             Bits 16 through 31 of the AND mask
  AGESA_IDS_NV_GENREG3ANDLOWEST                               EQU    013Ch; <                                                             The lowest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG3MMIOBASEHIGH                            EQU    013Dh; <                                                                                 MMIO base High
  AGESA_IDS_NV_GENREG3MMIOBASELOW                             EQU    013Eh; <                                                                                  MMIO base low
  AGESA_IDS_NV_GENREG3PCI_INDIRECT_OFF_HIGH                   EQU    013Fh; <                                                                       PCI indirect Offset High
  AGESA_IDS_NV_GENREG3PCI_INDIRECT_OFF_LOW                    EQU    0140h; <                                                                        PCI indirect Offset Low
  AGESA_IDS_NV_GENREG3PCI_INDIRECT_WEBIT                      EQU    0141h; <                                                                  PCI indirect Write Enable bit
  AGESA_IDS_NV_GENREG3GMMX_OFF_HIGH                           EQU    0142h; <                                                                               GMMX offset High
  AGESA_IDS_NV_GENREG3GMMX_OFF_LOW                            EQU    0143h; <                                                                                GMMX offset low

  AGESA_IDS_NV_GENREG4E                                       EQU    0144h; <                                                            Enable or disable register access D
  AGESA_IDS_NV_GENREG4T                                       EQU    0145h; <                                                             Select the type of register to set
  AGESA_IDS_NV_GENREG4W                                       EQU    0146h; <                                                             When to modify the value specified
  AGESA_IDS_NV_GENREG4PCIANODES                               EQU    0147h; <                                The PCI setting should be applied to all nodes or a single node
  AGESA_IDS_NV_GENREG4MSRACORES                               EQU    0148h; <                                                 The MSR setting should be applied to all cores
  AGESA_IDS_NV_GENREG4MSRNODE                                 EQU    0149h; <                                           The MSR setting should be applied to a selector node
  AGESA_IDS_NV_GENREG4MSRCORE                                 EQU    014Ah; <                                          The MSR setting should be applied to a selected cores
  AGESA_IDS_NV_GENREG4BUS                                     EQU    014Bh; <                                                        Set the Bus number when the type is PCI
  AGESA_IDS_NV_GENREG4DEVICE                                  EQU    014Ch; <                                                     Set the Device number when the type is PCI
  AGESA_IDS_NV_GENREG4FUNC                                    EQU    014Dh; <                                                   Set the Function number when the type is PCI
  AGESA_IDS_NV_GENREG4OFFSET                                  EQU    014Eh; <                                                            Set the offset when the type is PCI
  AGESA_IDS_NV_MSRHIGH4                                       EQU    014Fh; <                                                      Set the high address when the type is MSR
  AGESA_IDS_NV_MSRLOW4                                        EQU    0150h; <                                                       Set the low address when the type is MSR
  AGESA_IDS_NV_GENREG4ORHIGHEST                               EQU    0151h; <                                                             The highest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG4ORHIGHER                                EQU    0152h; <                                                              Bits 32 through 47 of the OR mask
  AGESA_IDS_NV_GENREG4ORLOWER                                 EQU    0153h; <                                                              Bits 16 through 31 of the OR mask
  AGESA_IDS_NV_GENREG4ORLOWEST                                EQU    0154h; <                                                              The lowest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG4ANDHIGHEST                              EQU    0155h; <                                                            The highest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG4ANDHIGHER                               EQU    0156h; <                                                             Bits 32 through 47 of the AND mask
  AGESA_IDS_NV_GENREG4ANDLOWER                                EQU    0157h; <                                                             Bits 16 through 31 of the AND mask
  AGESA_IDS_NV_GENREG4ANDLOWEST                               EQU    0158h; <                                                             The lowest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG4MMIOBASEHIGH                            EQU    0159h; <                                                                                 MMIO base High
  AGESA_IDS_NV_GENREG4MMIOBASELOW                             EQU    015Ah; <                                                                                  MMIO base low
  AGESA_IDS_NV_GENREG4PCI_INDIRECT_OFF_HIGH                   EQU    015Bh; <                                                                       PCI indirect Offset High
  AGESA_IDS_NV_GENREG4PCI_INDIRECT_OFF_LOW                    EQU    015Ch; <                                                                        PCI indirect Offset Low
  AGESA_IDS_NV_GENREG4PCI_INDIRECT_WEBIT                      EQU    015Dh; <                                                                  PCI indirect Write Enable bit
  AGESA_IDS_NV_GENREG4GMMX_OFF_HIGH                           EQU    015Eh; <                                                                               GMMX offset High
  AGESA_IDS_NV_GENREG4GMMX_OFF_LOW                            EQU    015Fh; <                                                                                GMMX offset low

  AGESA_IDS_NV_GENREG5E                                       EQU    0160h; <                                                            Enable or disable register access E
  AGESA_IDS_NV_GENREG5T                                       EQU    0161h; <                                                             Select the type of register to set
  AGESA_IDS_NV_GENREG5W                                       EQU    0162h; <                                                             When to modify the value specified
  AGESA_IDS_NV_GENREG5PCIANODES                               EQU    0163h; <                                The PCI setting should be applied to all nodes or a single node
  AGESA_IDS_NV_GENREG5MSRACORES                               EQU    0164h; <                                                 The MSR setting should be applied to all cores
  AGESA_IDS_NV_GENREG5MSRNODE                                 EQU    0165h; <                                           The MSR setting should be applied to a selector node
  AGESA_IDS_NV_GENREG5MSRCORE                                 EQU    0166h; <                                          The MSR setting should be applied to a selected cores
  AGESA_IDS_NV_GENREG5BUS                                     EQU    0167h; <                                                        Set the Bus number when the type is PCI
  AGESA_IDS_NV_GENREG5DEVICE                                  EQU    0168h; <                                                     Set the Device number when the type is PCI
  AGESA_IDS_NV_GENREG5FUNC                                    EQU    0169h; <                                                   Set the Function number when the type is PCI
  AGESA_IDS_NV_GENREG5OFFSET                                  EQU    016Ah; <                                                            Set the offset when the type is PCI
  AGESA_IDS_NV_MSRHIGH5                                       EQU    016Bh; <                                                      Set the high address when the type is MSR
  AGESA_IDS_NV_MSRLOW5                                        EQU    016Ch; <                                                       Set the low address when the type is MSR
  AGESA_IDS_NV_GENREG5ORHIGHEST                               EQU    016Dh; <                                                             The highest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG5ORHIGHER                                EQU    016Eh; <                                                              Bits 32 through 47 of the OR mask
  AGESA_IDS_NV_GENREG5ORLOWER                                 EQU    016Fh; <                                                              Bits 16 through 31 of the OR mask
  AGESA_IDS_NV_GENREG5ORLOWEST                                EQU    0170h; <                                                              The lowest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG5ANDHIGHEST                              EQU    0171h; <                                                            The highest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG5ANDHIGHER                               EQU    0172h; <                                                             Bits 32 through 47 of the AND mask
  AGESA_IDS_NV_GENREG5ANDLOWER                                EQU    0173h; <                                                             Bits 16 through 31 of the AND mask
  AGESA_IDS_NV_GENREG5ANDLOWEST                               EQU    0174h; <                                                             The lowest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG5MMIOBASEHIGH                            EQU    0175h; <                                                                                 MMIO base High
  AGESA_IDS_NV_GENREG5MMIOBASELOW                             EQU    0176h; <                                                                                  MMIO base low
  AGESA_IDS_NV_GENREG5PCI_INDIRECT_OFF_HIGH                   EQU    0177h; <                                                                       PCI indirect Offset High
  AGESA_IDS_NV_GENREG5PCI_INDIRECT_OFF_LOW                    EQU    0178h; <                                                                        PCI indirect Offset Low
  AGESA_IDS_NV_GENREG5PCI_INDIRECT_WEBIT                      EQU    0179h; <                                                                  PCI indirect Write Enable bit
  AGESA_IDS_NV_GENREG5GMMX_OFF_HIGH                           EQU    017Ah; <                                                                               GMMX offset High
  AGESA_IDS_NV_GENREG5GMMX_OFF_LOW                            EQU    017Bh; <                                                                                GMMX offset low

  AGESA_IDS_NV_GENREG6E                                       EQU    017Ch; <                                                            Enable or disable register access F
  AGESA_IDS_NV_GENREG6T                                       EQU    017Dh; <                                                             Select the type of register to set
  AGESA_IDS_NV_GENREG6W                                       EQU    017Eh; <                                                             When to modify the value specified
  AGESA_IDS_NV_GENREG6PCIANODES                               EQU    017Fh; <                                The PCI setting should be applied to all nodes or a single node
  AGESA_IDS_NV_GENREG6MSRACORES                               EQU    0180h; <                                                 The MSR setting should be applied to all cores
  AGESA_IDS_NV_GENREG6MSRNODE                                 EQU    0181h; <                                           The MSR setting should be applied to a selector node
  AGESA_IDS_NV_GENREG6MSRCORE                                 EQU    0182h; <                                          The MSR setting should be applied to a selected cores
  AGESA_IDS_NV_GENREG6BUS                                     EQU    0183h; <                                                        Set the Bus number when the type is PCI
  AGESA_IDS_NV_GENREG6DEVICE                                  EQU    0184h; <                                                     Set the Device number when the type is PCI
  AGESA_IDS_NV_GENREG6FUNC                                    EQU    0185h; <                                                   Set the Function number when the type is PCI
  AGESA_IDS_NV_GENREG6OFFSET                                  EQU    0186h; <                                                            Set the offset when the type is PCI
  AGESA_IDS_NV_MSRHIGH6                                       EQU    0187h; <                                                      Set the high address when the type is MSR
  AGESA_IDS_NV_MSRLOW6                                        EQU    0188h; <                                                       Set the low address when the type is MSR
  AGESA_IDS_NV_GENREG6ORHIGHEST                               EQU    0189h; <                                                             The highest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG6ORHIGHER                                EQU    018Ah; <                                                              Bits 32 through 47 of the OR mask
  AGESA_IDS_NV_GENREG6ORLOWER                                 EQU    018Bh; <                                                              Bits 16 through 31 of the OR mask
  AGESA_IDS_NV_GENREG6ORLOWEST                                EQU    018Ch; <                                                              The lowest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG6ANDHIGHEST                              EQU    018Dh; <                                                            The highest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG6ANDHIGHER                               EQU    018Eh; <                                                             Bits 32 through 47 of the AND mask
  AGESA_IDS_NV_GENREG6ANDLOWER                                EQU    018Fh; <                                                             Bits 16 through 31 of the AND mask
  AGESA_IDS_NV_GENREG6ANDLOWEST                               EQU    0190h; <                                                             The lowest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG6MMIOBASEHIGH                            EQU    0191h; <                                                                                 MMIO base High
  AGESA_IDS_NV_GENREG6MMIOBASELOW                             EQU    0192h; <                                                                                  MMIO base low
  AGESA_IDS_NV_GENREG6PCI_INDIRECT_OFF_HIGH                   EQU    0193h; <                                                                       PCI indirect Offset High
  AGESA_IDS_NV_GENREG6PCI_INDIRECT_OFF_LOW                    EQU    0194h; <                                                                        PCI indirect Offset Low
  AGESA_IDS_NV_GENREG6PCI_INDIRECT_WEBIT                      EQU    0195h; <                                                                  PCI indirect Write Enable bit
  AGESA_IDS_NV_GENREG6GMMX_OFF_HIGH                           EQU    0196h; <                                                                               GMMX offset High
  AGESA_IDS_NV_GENREG6GMMX_OFF_LOW                            EQU    0197h; <                                                                                GMMX offset low

  AGESA_IDS_NV_GENREG7E                                       EQU    0198h; <                                                            Enable or disable register access G
  AGESA_IDS_NV_GENREG7T                                       EQU    0199h; <                                                             Select the type of register to set
  AGESA_IDS_NV_GENREG7W                                       EQU    019Ah; <                                                             When to modify the value specified
  AGESA_IDS_NV_GENREG7PCIANODES                               EQU    019Bh; <                                The PCI setting should be applied to all nodes or a single node
  AGESA_IDS_NV_GENREG7MSRACORES                               EQU    019Ch; <                                                 The MSR setting should be applied to all cores
  AGESA_IDS_NV_GENREG7MSRNODE                                 EQU    019Dh; <                                           The MSR setting should be applied to a selector node
  AGESA_IDS_NV_GENREG7MSRCORE                                 EQU    019Eh; <                                          The MSR setting should be applied to a selected cores
  AGESA_IDS_NV_GENREG7BUS                                     EQU    019Fh; <                                                        Set the Bus number when the type is PCI
  AGESA_IDS_NV_GENREG7DEVICE                                  EQU    01A0h; <                                                     Set the Device number when the type is PCI
  AGESA_IDS_NV_GENREG7FUNC                                    EQU    01A1h; <                                                   Set the Function number when the type is PCI
  AGESA_IDS_NV_GENREG7OFFSET                                  EQU    01A2h; <                                                            Set the offset when the type is PCI
  AGESA_IDS_NV_MSRHIGH7                                       EQU    01A3h; <                                                      Set the high address when the type is MSR
  AGESA_IDS_NV_MSRLOW7                                        EQU    01A4h; <                                                       Set the low address when the type is MSR
  AGESA_IDS_NV_GENREG7ORHIGHEST                               EQU    01A5h; <                                                             The highest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG7ORHIGHER                                EQU    01A6h; <                                                              Bits 32 through 47 of the OR mask
  AGESA_IDS_NV_GENREG7ORLOWER                                 EQU    01A7h; <                                                              Bits 16 through 31 of the OR mask
  AGESA_IDS_NV_GENREG7ORLOWEST                                EQU    01A8h; <                                                              The lowest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG7ANDHIGHEST                              EQU    01A9h; <                                                            The highest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG7ANDHIGHER                               EQU    01AAh; <                                                             Bits 32 through 47 of the AND mask
  AGESA_IDS_NV_GENREG7ANDLOWER                                EQU    01ABh; <                                                             Bits 16 through 31 of the AND mask
  AGESA_IDS_NV_GENREG7ANDLOWEST                               EQU    01ACh; <                                                             The lowest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG7MMIOBASEHIGH                            EQU    01ADh; <                                                                                 MMIO base High
  AGESA_IDS_NV_GENREG7MMIOBASELOW                             EQU    01AEh; <                                                                                  MMIO base low
  AGESA_IDS_NV_GENREG7PCI_INDIRECT_OFF_HIGH                   EQU    01AFh; <                                                                       PCI indirect Offset High
  AGESA_IDS_NV_GENREG7PCI_INDIRECT_OFF_LOW                    EQU    01B0h; <                                                                        PCI indirect Offset Low
  AGESA_IDS_NV_GENREG7PCI_INDIRECT_WEBIT                      EQU    01B1h; <                                                                  PCI indirect Write Enable bit
  AGESA_IDS_NV_GENREG7GMMX_OFF_HIGH                           EQU    01B2h; <                                                                               GMMX offset High
  AGESA_IDS_NV_GENREG7GMMX_OFF_LOW                            EQU    01B3h; <                                                                                GMMX offset low

  AGESA_IDS_NV_GENREG8E                                       EQU    01B4h; <                                                            Enable or disable register access H
  AGESA_IDS_NV_GENREG8T                                       EQU    01B5h; <                                                             Select the type of register to set
  AGESA_IDS_NV_GENREG8W                                       EQU    01B6h; <                                                             When to modify the value specified
  AGESA_IDS_NV_GENREG8PCIANODES                               EQU    01B7h; <                                The PCI setting should be applied to all nodes or a single node
  AGESA_IDS_NV_GENREG8MSRACORES                               EQU    01B8h; <                                                 The MSR setting should be applied to all cores
  AGESA_IDS_NV_GENREG8MSRNODE                                 EQU    01B9h; <                                           The MSR setting should be applied to a selector node
  AGESA_IDS_NV_GENREG8MSRCORE                                 EQU    01BAh; <                                          The MSR setting should be applied to a selected cores
  AGESA_IDS_NV_GENREG8BUS                                     EQU    01BBh; <                                                        Set the Bus number when the type is PCI
  AGESA_IDS_NV_GENREG8DEVICE                                  EQU    01BCh; <                                                     Set the Device number when the type is PCI
  AGESA_IDS_NV_GENREG8FUNC                                    EQU    01BDh; <                                                   Set the Function number when the type is PCI
  AGESA_IDS_NV_GENREG8OFFSET                                  EQU    01BEh; <                                                            Set the offset when the type is PCI
  AGESA_IDS_NV_MSRHIGH8                                       EQU    01BFh; <                                                      Set the high address when the type is MSR
  AGESA_IDS_NV_MSRLOW8                                        EQU    01C0h; <                                                       Set the low address when the type is MSR
  AGESA_IDS_NV_GENREG8ORHIGHEST                               EQU    01C1h; <                                                             The highest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG8ORHIGHER                                EQU    01C2h; <                                                              Bits 32 through 47 of the OR mask
  AGESA_IDS_NV_GENREG8ORLOWER                                 EQU    01C3h; <                                                              Bits 16 through 31 of the OR mask
  AGESA_IDS_NV_GENREG8ORLOWEST                                EQU    01C4h; <                                                              The lowest 16 bits of the OR mask
  AGESA_IDS_NV_GENREG8ANDHIGHEST                              EQU    01C5h; <                                                            The highest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG8ANDHIGHER                               EQU    01C6h; <                                                             Bits 32 through 47 of the AND mask
  AGESA_IDS_NV_GENREG8ANDLOWER                                EQU    01C7h; <                                                             Bits 16 through 31 of the AND mask
  AGESA_IDS_NV_GENREG8ANDLOWEST                               EQU    01C8h; <                                                             The lowest 16 bits of the AND mask
  AGESA_IDS_NV_GENREG8MMIOBASEHIGH                            EQU    01C9h; <                                                                                 MMIO base High
  AGESA_IDS_NV_GENREG8MMIOBASELOW                             EQU    01CAh; <                                                                                  MMIO base low
  AGESA_IDS_NV_GENREG8PCI_INDIRECT_OFF_HIGH                   EQU    01CBh; <                                                                       PCI indirect Offset High
  AGESA_IDS_NV_GENREG8PCI_INDIRECT_OFF_LOW                    EQU    01CCh; <                                                                        PCI indirect Offset Low
  AGESA_IDS_NV_GENREG8PCI_INDIRECT_WEBIT                      EQU    01CDh; <                                                                  PCI indirect Write Enable bit
  AGESA_IDS_NV_GENREG8GMMX_OFF_HIGH                           EQU    01CEh; <                                                                               GMMX offset High
  AGESA_IDS_NV_GENREG8GMMX_OFF_LOW                            EQU    01CFh; <                                                                                GMMX offset low

  AGESA_IDS_NV_BVM_IMAGE_BASE_HIGHEST                         EQU    01D0h; <                                                    Bits 48 to 63 of the BVM image base address
  AGESA_IDS_NV_BVM_IMAGE_BASE_HIGHER                          EQU    01D1h; <                                                    Bits 32 to 47 of the BVM image base address
  AGESA_IDS_NV_BVM_IMAGE_BASE_LOWER                           EQU    01D2h; <                                                    Bits 16 to 31 of the BVM image base address
  AGESA_IDS_NV_BVM_IMAGE_BASE_LOWEST                          EQU    01D3h; <                                                    Bits  0 to 15 of the BVM image base address
  AGESA_IDS_NV_IGNORE_MEM_ERR                                 EQU    01D4h; <                                                    Whether to ignore memory error and continue

  AGESA_IDS_NV_GNB_ORB_DYNAMIC_WAKE_EN                        EQU    01D5h; <                                                                               ORB Dynamic Wake
  AGESA_IDS_NV_GNB_ORB_DYNAMIC_WAKE_HYSTERESIS                EQU    01D6h; <                                                                            ORB Wake Hysteresis
  AGESA_IDS_NV_GNB_ORB_DYNAMIC_WAKE_HYSTERESIS_VAL            EQU    01D7h; <                                                                      ORB Wake Hysteresis Value

  AGESA_IDS_NV_HDTBREAKPOINT_BFDRAMINIT                       EQU    01D8h; <                                                              hdtout breakpoint before Draminit
  AGESA_IDS_NV_HDTBREAKPOINT_BFMEMCLR                         EQU    01D9h; <                                                        hdtout breakpoint before set Memclr bit
  AGESA_IDS_NV_GNB_ABM_SUPPORT                                EQU    01DAh; <                                                                                    ABM support
  AGESA_IDS_NV_GNB_LCD_BACKLIGHT_CONTROL                      EQU    01DBh; <                                                     The PWM frequency to LCD backlight control
  AGESA_IDS_NV_GNB_LCD_BACKLIGHT                              EQU    01DCh; <                                                             The PWM frequency to LCD backlight
  AGESA_IDS_NV_GNB_DRR_SUPPORT                                EQU    01DDh; <                                                                                    DRR support
  AGESA_IDS_NV_GNB_PLATFORM_TYPE                              EQU    01DEh; <                                                                                  platform type
  AGESA_IDS_NV_GNB_UMA_STEERING                               EQU    01DFh; <                                                                                   UMA steering
  AGESA_IDS_NV_GNB_PRIMARY_GRAPHICS_ADAPTOR                   EQU    01E0h; <                                                                       Primary Graphics Adapter

  AGESA_IDS_NV_GNB_LCLK_DEEPSLEEP                             EQU    01E1h; <                                                                                LCLK deep sleep

  AGESA_IDS_NV_GNB_LCLK_DPM                                   EQU    01E2h; <                                                                                       LCLK DPM

  AGESA_IDS_NV_GNB_SMU_LCLK_GATING                            EQU    01E3h; <                                                                                SMU LCLK gating
  AGESA_IDS_NV_GNB_SMU_SCLK_GATING                            EQU    01E4h; <                                                                                SMU SCLK gating
  AGESA_IDS_NV_GNB_ORB_LCLK_GATING                            EQU    01E5h; <                                                                                ORB LCLK gating
  AGESA_IDS_NV_GNB_ORB_SCLK_GATING                            EQU    01E6h; <                                                                                ORB SCLK gating
  AGESA_IDS_NV_GNB_IOC_LCLK_GATING                            EQU    01E7h; <                                                                                IOC LCLK gating
  AGESA_IDS_NV_GNB_IOC_SCLK_GATING                            EQU    01E8h; <                                                                                IOC SCLK gating
  AGESA_IDS_NV_GNB_BIF_SCLK_GATING                            EQU    01E9h; <                                                                                BIF SCLK gating
  AGESA_IDS_NV_GNB_GMC_SCLK_GATING_CTRL                       EQU    01EAh; <                                                                        Control GMC SCLK gating
  AGESA_IDS_NV_GNB_GMC_SCLK_GATING                            EQU    01EBh; <                                                                                GMC SCLK gating
  AGESA_IDS_NV_GNB_DCE_DISPCLK_GATING                         EQU    01ECh; <                                                                       Dce display clock gating
  AGESA_IDS_NV_GNB_DCE_SCLK_GATING                            EQU    01EDh; <                                                                              Dce Sclock gating
  AGESA_IDS_NV_GNB_ORB_CLK_GATING                             EQU    01EEh; <                                                                                 ORB CLK gating
  AGESA_IDS_NV_GNB_NB_IOMMU_L1_LCLKGATING                     EQU    01EFh; <                                                                           IOMMU L1 LCLK gating
  AGESA_IDS_NV_GNB_NB_IOMMU_L2_LCLKGATING                     EQU    01F0h; <                                                                           IOMMU L2 LCLK gating
  AGESA_IDS_NV_GNB_PCIE_CORE_POWER_GATING                     EQU    01F1h; <                                                                     GNB PCIe Core Power Gating
  AGESA_IDS_NV_GNB_PCIE_CORE_POWER_GATING_DATA                EQU    01F2h; <                                                                GNB PCIe Core Power Gating Data
  AGESA_IDS_NV_GNB_PCIE_PHY_POWER_GATING                      EQU    01F3h; <                                                                 GNB PCIe Phy Power Gating Data

  AGESA_IDS_NV_GNB_FORCE_CABLESAFE                            EQU    01F4h; <                                                                               Force Cable Safe
  AGESA_IDS_NV_GNB_ALTVDDNB                                   EQU    01F5h; <                                                                                       AltVddNb
  AGESA_IDS_NV_GNB_PMM_BAPMCTL                                EQU    01F6h; <                                                                                   BAPM Control
  AGESA_IDS_NV_GNB_PMM_BAPMSMUCTL                             EQU    01F7h; <                                                                               BAPM SMU Control
  AGESA_IDS_NV_GNB_PMM_TDCTRACKING                            EQU    01F8h; <                                                                                   TDC Tracking
  AGESA_IDS_NV_GNB_PMM_PACKAGEPOWERTRACKING                   EQU    01F9h; <                                                                         Package Power Tracking
  AGESA_IDS_NV_GNB_PMM_NBDPMCTL                               EQU    01FAh; <                                                                                  NBDPM Control
  AGESA_IDS_NV_GNB_PMM_GNBSWTJOFFSETCTL                       EQU    01FBh; <                                                                          GNBSWTJOFFSET Control
  AGESA_IDS_NV_GNB_PMM_GNBSWTJOFFSET                          EQU    01FCh; <                                                                                  GNBSWTJOFFSET
  AGESA_IDS_NV_GNB_PMM_LOADLINEOPTIMIZATION                   EQU    01FDh; <                                                                  Loadline Optimization Control
  AGESA_IDS_NV_GNB_PMM_SMUSCLKCLOCKGATING                     EQU    01FEh; <                                                                          SMU SCLK Clock Gating
  AGESA_IDS_NV_GNB_PMM_GNBPMMLHTC                             EQU    01FFh; <                                                                                           LHTC
  AGESA_IDS_NV_GNB_PMM_HYBRIDBOOSTCTL                         EQU    0200h; <                                                                           Hybrid Boost Control
  AGESA_IDS_NV_GNB_PMM_HYBRIDBOOSTTHRESHOLDHYSTCTL            EQU    0201h; <                                                             Hybrid Boost Threshold hystControl
  AGESA_IDS_NV_GNB_PMM_HYBRIDBOOSTTHRESHOLDHYST               EQU    0202h; <                                                                    Hybrid Boost Threshold hyst
  AGESA_IDS_NV_GNB_PMM_HYBRIDBOOSTTHRESHOLDOFFSETCTL          EQU    0203h; <                                                          Hybrid Boost Threshold offset Control
  AGESA_IDS_NV_GNB_PMM_HYBRIDBOOSTTHRESHOLDOFFSET             EQU    0204h; <                                                                  Hybrid Boost Threshold offset
  AGESA_IDS_NV_GNB_PMM_HYBRIDBOOSTSCALARCTL                   EQU    0205h; <                                                                    Hybrid Boost Scalar Control
  AGESA_IDS_NV_GNB_PMM_HYBRIDBOOSTSCALAR                      EQU    0206h; <                                                                            Hybrid Boost Scalar
  AGESA_IDS_NV_GNB_PMM_PCIEPHYISOLATION                       EQU    0207h; <                                                                             Pcie Phy Isolation
  AGESA_IDS_NV_GNB_PMM_NATIVEGEN1PLL                          EQU    0208h; <                                                                                Native Gen1 PLL

  AGESA_IDS_NV_GNB_NB_SWDEVICEIDREMAPPING                     EQU    0209h; <                                                                         SW Device ID Remapping
  AGESA_IDS_NV_GNB_NB_PBR0DEVFNMAP                            EQU    020Ah; <                                                                                  PBr0 DevFnMap
  AGESA_IDS_NV_GNB_NB_PBR1DEVFNMAP                            EQU    020Bh; <                                                                                  PBr1 DevFnMap
  AGESA_IDS_NV_GNB_NB_PBR2DEVFNMAP                            EQU    020Ch; <                                                                                  PBr2 DevFnMap
  AGESA_IDS_NV_GNB_NB_PBR3DEVFNMAP                            EQU    020Dh; <                                                                                  PBr3 DevFnMap
  AGESA_IDS_NV_GNB_NB_PBR4DEVFNMAP                            EQU    020Eh; <                                                                                  PBr4 DevFnMap
  AGESA_IDS_NV_GNB_NB_PBR5DEVFNMAP                            EQU    020Fh; <                                                                                  PBr5 DevFnMap
  AGESA_IDS_NV_GNB_NB_PBR6DEVFNMAP                            EQU    0210h; <                                                                                  PBr6 DevFnMap
  AGESA_IDS_NV_GNB_NB_PBR7DEVFNMAP                            EQU    0211h; <                                                                                  PBr7 DevFnMap
  AGESA_IDS_NV_GNB_NB_PBR8DEVFNMAP                            EQU    0212h; <                                                                                  PBr8 DevFnMap
  AGESA_IDS_NV_GNB_NB_PBR9DEVFNMAP                            EQU    0213h; <                                                                                  PBr9 DevFnMap
  AGESA_IDS_NV_GNB_NB_PBR10DEVFNMAP                           EQU    0214h; <                                                                                 PBr10 DevFnMap
  AGESA_IDS_NV_GNB_NB_PBR11DEVFNMAP                           EQU    0215h; <                                                                                 PBr11 DevFnMap

  AGESA_IDS_NV_MAIN_PLL_CON                                   EQU    0216h; <                                                                         00 - Auto, 01 - Manual
  AGESA_IDS_NV_MAIN_PLL_FID_EN                                EQU    0217h; <                                                                           MainPllFreqId Enable
  AGESA_IDS_NV_MAIN_PLL_FID                                   EQU    0218h; <                                                                                  MainPllFreqId

  AGESA_IDS_NV_CUSTOM_NB_PSTATE                               EQU    0219h; <                                                                         00 - Auto, 01 - Manual
  AGESA_IDS_NV_NUMOFNBPSTATES                                 EQU    021Ah; <                                                                           Number of NB PStates
  AGESA_IDS_NV_CUSTOM_NB_P0_DIV_CTRL                          EQU    021Bh; <                                                                       00 - Manual, 01 - MCLK/2
  AGESA_IDS_NV_CUSTOM_NB_P1_DIV_CTRL                          EQU    021Ch; <                                                                       00 - Manual, 01 - MCLK/2
  AGESA_IDS_NV_FORCE_NB_PSTATE                                EQU    021Dh; <                                                                00 - NBP0  01 - NBP1  02 - Auto

  AGESA_IDS_NV_CPB_GNB_EN                                     EQU    021Eh; <                                                              Graphics northbridge boost enable
  AGESA_IDS_NV_CPB_THROTTLE_CORE                              EQU    021Fh; <                                                                              Throttle One Core
  AGESA_IDS_NV_CPB_THROTTLE_CORE_TIMER                        EQU    0220h; <                                                                            Throttle Core Timer
  AGESA_IDS_NV_CPB_EN_ALLCORES                                EQU    0221h; <                                                                         Boost enable all cores
  AGESA_IDS_NV_CPB_PLL_RANGE_MODE                             EQU    0222h; <                                                                                 Pll Range Mode
  AGESA_IDS_NV_CPB_BOOST_SOURCE                               EQU    0223h; <                                                                                   Boost source
  AGESA_IDS_NV_CPB_NUM_BOOST_CTRL                             EQU    0224h; <                                                               Number of boosted states Control
  AGESA_IDS_NV_CPB_NUM_BOOST                                  EQU    0225h; <                                                                       Number of boosted states
  AGESA_IDS_NV_CPB_BOOST_DIS_CORE0                            EQU    0226h; <                                                        Core Performance Boost disable on core0
  AGESA_IDS_NV_CPB_BOOST_DIS_CORE1                            EQU    0227h; <                                                        Core Performance Boost disable on core1
  AGESA_IDS_NV_CPB_BOOST_DIS_CORE2                            EQU    0228h; <                                                        Core Performance Boost disable on core2
  AGESA_IDS_NV_CPB_BOOST_DIS_CORE3                            EQU    0229h; <                                                        Core Performance Boost disable on core3

  AGESA_IDS_NV_CPBMASTEREN                                    EQU    022Ah; <                                                           Core performance boost master enable
  AGESA_IDS_NV_C_STATE_BOOSTCTL                               EQU    022Bh; <                                                                          C-state Boost Control
  AGESA_IDS_NV_CSTATECNT                                      EQU    022Ch; <                                                                                  C-state count
  AGESA_IDS_NV_CSTATEBOOST                                    EQU    022Dh; <                                                                                    CstateBoost
  AGESA_IDS_NV_APMBOOSTSTATES                                 EQU    022Eh; <                                                                           APM TDP Boost States
  AGESA_IDS_NV_TDPCTL                                         EQU    022Fh; <                                                                                APM TDP Control
  AGESA_IDS_NV_NODETDPLIMITEN                                 EQU    0230h; <                                                                            NodeTdpLimit Enable
  AGESA_IDS_NV_APMLSWTDPLIMITEN                               EQU    0231h; <                                                                          ApmlSwTdpLimit Enable

  AGESA_IDS_NV_CC6CTL                                         EQU    0232h; <                                                                                    CC6 Control

  AGESA_IDS_NV_FASTSLAMTIMEDOWN                               EQU    0233h; <                                                                            Fast Slam Time Down

  AGESA_IDS_NV_NB_PSTATE_CTRL_DIS                             EQU    0234h; <                                                                     NB P-state control disable

  AGESA_IDS_NV_GNB_GMC_POWG_GATING_CTRL                       EQU    0235h; <                                                                             Control GMC gating
  AGESA_IDS_NV_GNB_GMC_POWG_GATING                            EQU    0236h; <                                                                               GMC power gating

  AGESA_IDS_NV_DRAMTERMDYN                                    EQU    0237h; <                                                                                    DramTermDyn
  AGESA_IDS_NV_DRAMTERM                                       EQU    0238h; <                                                                                       DramTerm
  AGESA_IDS_NV_DATATXFIFOWRDLY                                EQU    0239h; <                                                                 Data transmit FIFO write delay
  AGESA_IDS_NV_FASTSELFREFENTRYDIS                            EQU    023Ah; <                                                                Fast self refresh entry disable

  AGESA_IDS_NV_LOWPOWERDRVSTRENGTHEN                          EQU    023Bh; <                                                                Low power drive strength enable
  AGESA_IDS_NV_CC6DSMWORKAROUND                               EQU    023Ch; <                                                                               CC6DSMworkaround
  AGESA_IDS_NV_ODTSEN                                         EQU    023Dh; <                                                              On DIMM temperature sensor Enable

  AGESA_IDS_NV_HTT0TIMECTL                                    EQU    023Eh; <                                                                              HT T0Time Control
  AGESA_IDS_NV_HTT0TIME                                       EQU    023Fh; <                                                                 This specifies training 0 time
  AGESA_IDS_NV_ATMMODEEN                                      EQU    0240h; <                                  This specifies accelerated transition to modified mode enable
  AGESA_IDS_NV_VICTIMDISTRIBUTIONMODE                         EQU    0241h; <                             This specifies cHT VicBlk and VicBlkClean packet distribution mode
  AGESA_IDS_NV_HTFULLT0TIMECTL                                EQU    0242h; <                                                                          HT FullT0Time Control
  AGESA_IDS_NV_HTFULLT0TIME                                   EQU    0243h; <                                                                 This specifies training 0 time
  AGESA_IDS_NV_HTPOWERSAVINGSMODE                             EQU    0244h; <                                                           This specifies HT Power Savings Mode
  AGESA_IDS_NV_TXLS23CLKGATEEN                                EQU    0245h; <                                                     This specifies LS2/LS3 clock gating enable
  AGESA_IDS_NV_LINKPHYDLLREGCTL                               EQU    0246h; <                                                                 Link Phy DLL registers Control
  AGESA_IDS_NV_LINKPHYDLLREGHI                                EQU    0247h; <                                                                    Link Phy DLL registers High
  AGESA_IDS_NV_LINKPHYDLLREGLO                                EQU    0248h; <                                                                     Link Phy DLL registers Low
  AGESA_IDS_NV_TXSSBUSPWRSAVEEN                               EQU    0249h; <                                              Transmit source synchronous bus power save Enable

  AGESA_IDS_NV_INTLINKPHYOVERRIDE                             EQU    024Ah; <                                                                     Internal Link Phy Override
  AGESA_IDS_NV_DFEINT                                         EQU    024Bh; <                                                             Set Decision feedback equalization
  AGESA_IDS_NV_DFEVOLTAGEINT                                  EQU    024Ch; <                                                                       DFE offset voltage level
  AGESA_IDS_NV_DCVINT                                         EQU    024Dh; <                                                Specifies the peak singleended DC voltage level
  AGESA_IDS_NV_DEEMPHASISINT1                                 EQU    024Eh; <                                                 Link Phy Tx Deemphasis and Margin Test Control
  AGESA_IDS_NV_DEEMPHASISINT2                                 EQU    024Fh; <                                                 Link Phy Tx Deemphasis and Margin Test Control
  AGESA_IDS_NV_EXTLINKPHYOVERRIDE                             EQU    0250h; <                                                                     External Link Phy Override
  AGESA_IDS_NV_DFEEXT                                         EQU    0251h; <                                                             Set Decision feedback equalization
  AGESA_IDS_NV_DFEVOLTAGEEXT                                  EQU    0252h; <                                                                       DFE offset voltage level
  AGESA_IDS_NV_DCVEXT                                         EQU    0253h; <                                                Specifies the peak singleended DC voltage level
  AGESA_IDS_NV_DEEMPHASISEXT1                                 EQU    0254h; <                                                 Link Phy Tx Deemphasis and Margin Test Control
  AGESA_IDS_NV_DEEMPHASISEXT2                                 EQU    0255h; <                                                 Link Phy Tx Deemphasis and Margin Test Control
  AGESA_IDS_NV_LINKFIFOP0                                     EQU    0256h; <                                                    Set the Link FIFO Read Pointer for P0 state
  AGESA_IDS_NV_P0XMTRDPTR                                     EQU    0257h; <                                   This specifies transmit FIFO read pointer for P0 power state
  AGESA_IDS_NV_P0RCVRDPTR                                     EQU    0258h; <                                    This specifies receive FIFO read pointer for P0 power state
  AGESA_IDS_NV_LINKFIFOP1                                     EQU    0259h; <                                                    Set the Link FIFO Read Pointer for P1 state
  AGESA_IDS_NV_P1XMTRDPTR                                     EQU    025Ah; <                                   This specifies transmit FIFO read pointer for P1 power state
  AGESA_IDS_NV_P1RCVRDPTR                                     EQU    025Bh; <                                    This specifies receive FIFO read pointer for P1 power state

  AGESA_IDS_NV_ONIONDATABUFCOUNT                              EQU    025Ch; <                                                                Onion Data Buffer Count Control
  AGESA_IDS_NV_UPHIRESPCBCD                                   EQU    025Dh; <                                              Upstream high priority response data buffer count
  AGESA_IDS_NV_UPHINPREQCBCD                                  EQU    025Eh; <                                            Upstream high priority non-posted data buffer count
  AGESA_IDS_NV_UPHIPREQCBCD                                   EQU    025Fh; <                                                Upstream high priority posted data buffer count
  AGESA_IDS_NV_UPLORESPCBCD                                   EQU    0260h; <                                               Upstream low priority response data buffer count
  AGESA_IDS_NV_UPLONPREQCBCD                                  EQU    0261h; <                                             Upstream low priority non-posted data buffer count
  AGESA_IDS_NV_UPLOPREQCBCD                                   EQU    0262h; <                                                 Upstream low priority posted data buffer count

  AGESA_IDS_NV_ONIONCOMBUFCOUNT                               EQU    0263h; <                                    Controls the setting of Upstream ONION Command Buffer Count
  AGESA_IDS_NV_UPHIRESPCBCC                                   EQU    0264h; <                                           Upstream high priority response command buffer count
  AGESA_IDS_NV_UPHINPREQCBCC                                  EQU    0265h; <                                         Upstream high priority non-posted command buffer count
  AGESA_IDS_NV_UPHIPREQCBCC                                   EQU    0266h; <                                             Upstream high priority posted command buffer count
  AGESA_IDS_NV_UPLORESPCBCC                                   EQU    0267h; <                                            Upstream low priority response command buffer count
  AGESA_IDS_NV_UPLONPREQCBCC                                  EQU    0268h; <                                          Upstream low priority non-posted command buffer count
  AGESA_IDS_NV_UPLOPREQCBCC                                   EQU    0269h; <                                              Upstream low priority posted command buffer count

  AGESA_IDS_NV_IFQBUFALLOCATIONCON                            EQU    026Ah; <                             Controls the setting of In-Flight Queue Buffer Allocation Register
  AGESA_IDS_NV_FREEPOOLBC                                     EQU    026Bh; <                                                                         Free pool buffer count
  AGESA_IDS_NV_LOWPRINPBC                                     EQU    026Ch; <                                                   Low priority channel non-posted buffer count
  AGESA_IDS_NV_LOWPRIPBC                                      EQU    026Dh; <                                                       Low priority channel posted buffer count
  AGESA_IDS_NV_CPUBC                                          EQU    026Eh; <                                                                               CPU buffer count

  AGESA_IDS_NV_IFQEXTBUFALLOCATIONCON                         EQU    026Fh; <                    Controls the setting of In-Flight Queue Extended Buffer Allocation Register
  AGESA_IDS_NV_HIPRINPBC                                      EQU    0270h; <                                                  High priority channel non-posted buffer count
  AGESA_IDS_NV_HIPRIPBC                                       EQU    0271h; <                                                      High priority channel posted buffer count

  AGESA_IDS_NV_LSDSMWORDAROUND                                EQU    0272h; <                                                                 Control Erratum 463 workaround

  AGESA_IDS_NV_PDPHYPSDIS                                     EQU    0273h; <                                                              Power down phy power save Disable
  AGESA_IDS_NV_REFCHCMDMGTDIS                                 EQU    0274h; <                                                     Refresh channel command management disable
  AGESA_IDS_NV_OPPWRENHDIS                                    EQU    0275h; <                                                                Opportunistic write enh disable
  AGESA_IDS_NV_PRTLCHPDENHEN                                  EQU    0276h; <                                                          Partial channel power down enh enable
  AGESA_IDS_NV_AGGRPDEN                                       EQU    0277h; <                                                                   Aggressive power down enable
  AGESA_IDS_NV_ADDRCMDTRIEN                                   EQU    0278h; <                                                                Address command tristate enable
  AGESA_IDS_NV_PRTLCHPDDYNDLYCTL                              EQU    0279h; <                                               Partial channel power down dynamic delay Control
  AGESA_IDS_NV_PRTLCHPDDYNDLY                                 EQU    027Ah; <                                                       Partial channel power down dynamic delay
  AGESA_IDS_NV_AGGRPDDELAYCTL                                 EQU    027Bh; <                                                            Aggressive power down delay Control
  AGESA_IDS_NV_AGGRPDDELAY                                    EQU    027Ch; <                                                                    Aggressive power down delay

  AGESA_IDS_NV_PROCODTADV                                     EQU    027Dh; <                                                                                ProcOdt advance
  AGESA_IDS_NV_EXTRAPROCODTADV                                EQU    027Eh; <                                                                            ExtraProcOdtAdvance

  AGESA_IDS_NV_WRODTONDURATIONCTL                             EQU    027Fh; <                                                                  Write ODT on duration Control
  AGESA_IDS_NV_WRODTONDURATION                                EQU    0280h; <                                                                          Write ODT on duration
  AGESA_IDS_NV_WRODTTRNONDLYCTL                               EQU    0281h; <                                                                Write ODT Turn On Delay Control
  AGESA_IDS_NV_WRODTTRNONDLY                                  EQU    0282h; <                                                                        Write ODT Turn On Delay
  AGESA_IDS_NV_RDODTONDURATIONCTL                             EQU    0283h; <                                                                   Read ODT On Duration Control
  AGESA_IDS_NV_RDODTONDURATION                                EQU    0284h; <                                                                           Read ODT On Duration
  AGESA_IDS_NV_RDODTTRNONDLYCTL                               EQU    0285h; <                                                                 Read ODT Turn On Delay Control
  AGESA_IDS_NV_RDODTTRNONDLY                                  EQU    0286h; <                                                                         Read ODT Turn On Delay

  AGESA_IDS_NV_CSTATECTL                                      EQU    0287h; <                                                                                C-State Control
  AGESA_IDS_NV_NUMCSTATES                                     EQU    0288h; <                                                                  Select the Number of C-States
  AGESA_IDS_NV_CSTACT1                                        EQU    0289h; <                                                                     Set C-State Action Field 1
  AGESA_IDS_NV_CSTACT2                                        EQU    028Ah; <                                                                     Set C-State Action Field 2
  AGESA_IDS_NV_CSTACT3                                        EQU    028Bh; <                                                                     Set C-State Action Field 3
  AGESA_IDS_NV_ENFORCE_VDDIO                                  EQU    028Ch; <                                                                                   Enfore VDDIO

  AGESA_IDS_NV_DEADLOCK_WIDGET_DIS                            EQU    028Dh; <                                                                              DeadLockWidgetDis

  AGESA_IDS_NV_PCHGPDENDELAYCTL                               EQU    028Eh; <                                                       Precharge Power Down Entry Delay Control
  AGESA_IDS_NV_PCHGPDENDELAY                                  EQU    028Fh; <                                                               Precharge Power Down Entry Delay
  AGESA_IDS_NV_REDIRECT_SHUTDOWN_TO_HDT                       EQU    0290h; <                                                            Switch of Redirect Powerdown to HDT
  AGESA_IDS_NV_DEBUGCTRL_LBR                                  EQU    0291h; <                                                                    Switch of Debug Control Lbr
  AGESA_IDS_NV_NUMOFHIGHPERFORMANCENBPSTATE                   EQU    0292h; <                                                                    Switch of Debug Control Lbr

  AGESA_IDS_NV_FCH_NBSB_GEN2                                  EQU    0293h; <                                                                Bus generation between NB & FCH
  AGESA_IDS_NV_FCH_SPI_BURST_WRITE                            EQU    0294h; <                                                                            FCH SPI brust write
  AGESA_IDS_NV_HTC_LOCK                                       EQU    0295h; <                                                                              Override HTC lock
  AGESA_IDS_NV_MEMPSTATEDIS                                   EQU    0296h; <                                                                           Config  mempstatedis
  AGESA_IDS_NV_DLL_DAC_MONITOR                                EQU    0297h; <                                                                                Dll DAC Monitor
  AGESA_IDS_NV_FORCE_ASSERT                                   EQU    0298h; <                                                                                   Force Assert

  AGESA_IDS_NV_REDUCELOOPDELAY                                EQU    0299h; <                                                                                REDUCELOOPDELAY
  AGESA_IDS_NV_DLLCSRBIASTRIM                                 EQU    029Ah; <                                                                                 DLLCSRBIASTRIM
  AGESA_IDS_NV_D3DBYTE                                        EQU    029Bh; <                                                                                        D3DBYTE
  AGESA_IDS_NV_D3CLK                                          EQU    029Ch; <                                                                                          D3CLK
  AGESA_IDS_NV_D3CSODT                                        EQU    029Dh; <                                                                                        D3CSODT
  AGESA_IDS_NV_D3ABYTE2                                       EQU    029Eh; <                                                                                       D3ABYTE2
  AGESA_IDS_NV_CPUNBPS0MEMPSTATE                              EQU    029Fh; <                                                                         State 0 Memory P-state
  AGESA_IDS_NV_CPUNBPS1MEMPSTATE                              EQU    02A0h; <                                                                         State 1 Memory P-state
  AGESA_IDS_NV_CPUNBPS2MEMPSTATE                              EQU    02A1h; <                                                                         State 2 Memory P-state
  AGESA_IDS_NV_CPUNBPS3MEMPSTATE                              EQU    02A2h; <                                                                         State 3 Memory P-state
  AGESA_IDS_NV_MEMCTRLLEREFFARBDIS                            EQU    02A3h; <                                                                  Efficient arbitration disable
  AGESA_IDS_NV_MEMCTRLLERDISHALFNCLKPWRGATE                   EQU    02A4h; <                                                                    Disable HalfNCLK power gate
  AGESA_IDS_NV_CPUPSTSWNBPSTATELODIS                          EQU    02A5h; <                                                                Software NB P-state low disable
  AGESA_IDS_NV_TRACEMODE                                      EQU    02A6h; <                                                                       Enable Trace buffer mode
  AGESA_IDS_NV_NCEXTLINKPHYOVERRIDE                           EQU    02A7h; <                                                                     External Link Phy Override
  AGESA_IDS_NV_NCDFEEXT                                       EQU    02A8h; <                                                             Set Decision feedback equalization
  AGESA_IDS_NV_NCDFEVOLTAGEEXT                                EQU    02A9h; <                                                                       DFE offset voltage level
  AGESA_IDS_NV_NCDCVEXT                                       EQU    02AAh; <                                                Specifies the peak singleended DC voltage level
  AGESA_IDS_NV_NCDEEMPHASISEXT1                               EQU    02ABh; <                                                 Link Phy Tx Deemphasis and Margin Test Control
  AGESA_IDS_NV_NCDEEMPHASISEXT2                               EQU    02ACh; <                                                 Link Phy Tx Deemphasis and Margin Test Control
  AGESA_IDS_NV_MEMORY_POWER_POLICY                            EQU    02ADh; <                                                                            Memory power policy
  AGESA_IDS_NV_MEM_LOCKDRAM_CONFIG                            EQU    02AEh; <                                                                            Mem LockDram Config
  AGESA_IDS_NV_CPUCURRENTLIMITCTL                             EQU    02AFh; <                                                                      CPU Current Limit Control
  AGESA_IDS_NV_CPUCURRENTLIMIT                                EQU    02B0h; <                                                                              CPU Current Limit
  AGESA_IDS_NV_CPUMAXCURRENTLIMIT                             EQU    02B1h; <                                                                          CPU Max Current Limit
  AGESA_IDS_NV_NBCURRENTLIMITCTL                              EQU    02B2h; <                                                                       NB Current Limit Control
  AGESA_IDS_NV_NBCURRENTLIMIT                                 EQU    02B3h; <                                                                               NB Current Limit
  AGESA_IDS_NV_NBMAXCURRENTLIMIT                              EQU    02B4h; <                                                                           NB Max Current Limit
  AGESA_IDS_NV_BEFORE_GEN2_INIT                               EQU    02B5h; <                                                                Hdt breakpoint Before Gen2 Init
  AGESA_IDS_NV_BEFORE_GPP_TRAINING                            EQU    02B6h; <                                                             Hdt breakpoint Before Gpp training
  AGESA_IDS_NV_BEFORE_RECONFIGURATION                         EQU    02B7h; <                                                          Hdt breakpoint Before Reconfiguration
  AGESA_IDS_NV_BEFORE_GEN3_INIT                               EQU    02B8h; <                                                                Hdt breakpoint Before Gen3 Init
  AGESA_IDS_NV_BEFORE_POWER_GATING                            EQU    02B9h; <                                                             Hdt breakpoint Before Power Gating
  AGESA_IDS_NV_NBCORE_SYNC_POINTER_ADJ_CTL                    EQU    02BAh; <                                                       NB/core sync FIFO pointer adjust Control
  AGESA_IDS_NV_NBCORE_SYNC_POINTER_ADJ                        EQU    02BBh; <                                                               NB/core sync FIFO pointer adjust
  AGESA_IDS_NV_CPUCACHEFLUSHSUCMONTMRSEL                      EQU    02BCh; <                                                       Cache Flush Success Monitor Timer Select
  AGESA_IDS_NV_CPUCACHEFLUSHSUCMONMISPREDICTACT               EQU    02BDh; <                                                  Cache Flush Success Monitor Mispredict Action
  AGESA_IDS_NV_CPUCSTCACHEFLUSHIMMONALLHLT                    EQU    02BEh; <                                                              Cache Flush Immediate on All Halt
  AGESA_IDS_NV_CPUHLTCSTINDEX                                 EQU    02BFh; <                                                                              Halt CState Index
  AGESA_IDS_NV_CPUCSTMSGDIS                                   EQU    02C0h; <                                                                       CState Messaging Disable
  AGESA_IDS_NV_CPUCSTCC6PWRDWNREGEN                           EQU    02C1h; <                                                                CC6 Power Down Regulator Enable
  AGESA_IDS_NV_CPUCSTMISCTIMERTHRESH                          EQU    02C2h; <                                                                           Misc Timer Threshold
  AGESA_IDS_NV_CPUCSTXC6CTL                                   EQU    02C3h; <                                                                                    XC6 Control
  AGESA_IDS_NV_CPUCSTXC6CACHEFLUSHMODE                        EQU    02C4h; <                                                                           XC6 Cache Flush Mode
  AGESA_IDS_NV_CPUCSTXC6MONITORTHRESH                         EQU    02C5h; <                                                                          XC6 Monitor Threshold
  AGESA_IDS_NV_CPUCSTXC6MONITORTIMERSEL                       EQU    02C6h; <                                                                       XC6 Monitor Timer Select
  AGESA_IDS_NV_CPUCSTXC6MONMISPREDICTACT                      EQU    02C7h; <                                                                  XC6 Monitor Mispredict Action
  AGESA_IDS_NV_CPUCSTCACHERINSERCTL                           EQU    02C8h; <                                                                           Cache Rinser Control
  AGESA_IDS_NV_CPUCSTDCRINSERINTERVAL                         EQU    02C9h; <                                                                             DC Rinser Interval
  AGESA_IDS_NV_CPUCSTDCRINSERDIS                              EQU    02CAh; <                                                                              DC Rinser Disable
  AGESA_IDS_NV_CPUCSTL2RINSERDIS                              EQU    02CBh; <                                                                              L2 Rinser Disable
  AGESA_IDS_NV_CPUCSTL2RINSERINTERVAL                         EQU    02CCh; <                                                                             L2 Rinser Interval
  AGESA_IDS_NV_CPUCSTL2SECTORDIRTYDIS                         EQU    02CDh; <                                                                        L2 Sector Dirty Disable
  AGESA_IDS_NV_APIC330_MASK_CONFIG                            EQU    02CEh; <                                                                             APIC330[16] Config
  AGESA_IDS_NV_tCKSRXCTL                                      EQU    02CFh; <                                                                                 tCKSRX Control
  AGESA_IDS_NV_tCKSRX                                         EQU    02D0h; <                                                                                         tCKSRX
  AGESA_IDS_NV_tCKSRECTL                                      EQU    02D1h; <                                                                                 tCKSRE Control
  AGESA_IDS_NV_tCKSRE                                         EQU    02D2h; <                                                                                         tCKSRE
  AGESA_IDS_NV_tCKESRCTL                                      EQU    02D3h; <                                                                                 tCKSRE Control
  AGESA_IDS_NV_tCKESR                                         EQU    02D4h; <                                                                                         tCKSRE
  AGESA_IDS_NV_tPDCTL                                         EQU    02D5h; <                                                                                    tPD Control
  AGESA_IDS_NV_tPD                                            EQU    02D6h; <                                                                                            tPD
  AGESA_IDS_NV_ECCEN                                          EQU    02D7h; <                                                                                     ECC Enable
  AGESA_IDS_NV_ECCSYMOBOLSIZE                                 EQU    02D8h; <                                                                                ECC Symbol Size
  AGESA_IDS_NV_2D_READ_TRAINNING                              EQU    02D9h; <                                                                              2D Read Trainning
  AGESA_IDS_NV_ALWAYSENDLLCLKS                                EQU    02DAh; <                                                                       Always enable DLL clocks
  AGESA_IDS_NV_FASTMSTATECHGDIS                               EQU    02DBh; <                                                                    Fast M-state change disable
  AGESA_IDS_NV_RXSSBMNTCLKEN                                  EQU    02DCh; <                                                                                  RxSsbMntClkEn
  AGESA_IDS_NV_RXCHMNTCLKEN                                   EQU    02DDh; <                                                              Receive channel maitenance clocks
  AGESA_IDS_NV_DLLNOLOCK                                      EQU    02DEh; <                                                                                    Dll no lock
  AGESA_IDS_NV_ENRXPADSTANDBY                                 EQU    02DFh; <                                                                    Enable receiver pad standby
  AGESA_IDS_NV_DISAUTOREFRESH                                 EQU    02E0h; <                                                                      Disable automatic refresh
  AGESA_IDS_NV_CMDTHROTTLEMODECTL                             EQU    02E1h; <                                                                        CmdThrottleMode Control
  AGESA_IDS_NV_CMDTHROTTLEMODE                                EQU    02E2h; <                                                                                CmdThrottleMode
  AGESA_IDS_NV_DATABYTETXSLEWRATECTL                          EQU    02E3h; <                                                             Data Byte Tx Cal Slew Rate Control
  AGESA_IDS_NV_DATABYTETXPREDRIVERCALTXPREP                   EQU    02E4h; <                                                                  DataByteTxPreDriverCal TxPreP
  AGESA_IDS_NV_DATABYTETXPREDRIVERCALTXPREN                   EQU    02E5h; <                                                                  DataByteTxPreDriverCal TxPreN
  AGESA_IDS_NV_DATABYTETXPREDRIVERCAL2PAD1TXPREP              EQU    02E6h; <                                                             DataByteTxPreDriverCal2Pad1 TxPreP
  AGESA_IDS_NV_DATABYTETXPREDRIVERCAL2PAD1TXPREN              EQU    02E7h; <                                                             DataByteTxPreDriverCal2Pad1 TxPreN
  AGESA_IDS_NV_DATABYTETXPREDRIVERCAL2PAD2TXPREP              EQU    02E8h; <                                                             DataByteTxPreDriverCal2Pad2 TxPreP
  AGESA_IDS_NV_DATABYTETXPREDRIVERCAL2PAD2TXPREN              EQU    02E9h; <                                                             DataByteTxPreDriverCal2Pad2 TxPreN
  AGESA_IDS_NV_CMDADDR0TXCALSLEWRATECTL                       EQU    02EAh; <                                                      CmdAddr0 Tx Calibration Slew Rate Control
  AGESA_IDS_NV_CMDADDR0TXPREDRIVERCALPAD0TXPREP               EQU    02EBh; <                                                              CmdAddr0TxPreDriverCalPad0 TxPreP
  AGESA_IDS_NV_CMDADDR0TXPREDRIVERCALPAD0TXPREN               EQU    02ECh; <                                                              CmdAddr0TxPreDriverCalPad0 TxPreN
  AGESA_IDS_NV_CMDADDR0TXPREDRIVERCAL2PAD1TXPREP              EQU    02EDh; <                                                             CmdAddr0TxPreDriverCal2Pad1 TxPreP
  AGESA_IDS_NV_CMDADDR0TXPREDRIVERCAL2PAD1TXPREN              EQU    02EEh; <                                                             CmdAddr0TxPreDriverCal2Pad1 TxPreN
  AGESA_IDS_NV_CMDADDR0TXPREDRIVERCAL2PAD2TXPREP              EQU    02EFh; <                                                             CmdAddr0TxPreDriverCal2Pad1 TxPreP
  AGESA_IDS_NV_CMDADDR0TXPREDRIVERCAL2PAD2TXPREN              EQU    02F0h; <                                                             CmdAddr0TxPreDriverCal2Pad1 TxPreN
  AGESA_IDS_NV_CMDADDR1TXCALSLEWRATECTL                       EQU    02F1h; <                                                      CmdAddr1 Tx Calibration Slew Rate Control
  AGESA_IDS_NV_CMDADDR1TXPREDRIVERCALPAD0TXPREP               EQU    02F2h; <                                                              CmdAddr1TxPreDriverCalPad0 TxPreP
  AGESA_IDS_NV_CMDADDR1TXPREDRIVERCALPAD0TXPREN               EQU    02F3h; <                                                              CmdAddr1TxPreDriverCalPad0 TxPreN
  AGESA_IDS_NV_CMDADDR1TXPREDRIVERCAL2PAD1TXPREP              EQU    02F4h; <                                                             CmdAddr1TxPreDriverCal2Pad1 TxPreP
  AGESA_IDS_NV_CMDADDR1TXPREDRIVERCAL2PAD1TXPREN              EQU    02F5h; <                                                             CmdAddr1TxPreDriverCal2Pad1 TxPreN
  AGESA_IDS_NV_CMDADDR1TXPREDRIVERCAL2PAD2TXPREP              EQU    02F6h; <                                                             CmdAddr1TxPreDriverCal2Pad2 TxPreP
  AGESA_IDS_NV_CMDADDR1TXPREDRIVERCAL2PAD2TXPREN              EQU    02F7h; <                                                             CmdAddr1TxPreDriverCal2Pad2 TxPreN
  AGESA_IDS_NV_ADDRTXCALSLEWRATECTL                           EQU    02F8h; <                                                          Addr Tx Calibration Slew Rate Control
  AGESA_IDS_NV_ADDRTXPREDRIVERCALPAD0TXPREP                   EQU    02F9h; <                                                                  AddrTxPreDriverCalPad0 TxPreP
  AGESA_IDS_NV_ADDRTXPREDRIVERCALPAD0TXPREN                   EQU    02FAh; <                                                                  AddrTxPreDriverCalPad0 TxPreN
  AGESA_IDS_NV_ADDRTXPREDRIVERCAL2PAD1TXPREP                  EQU    02FBh; <                                                                 AddrTxPreDriverCal2Pad1 TxPreP
  AGESA_IDS_NV_ADDRTXPREDRIVERCAL2PAD1TXPREN                  EQU    02FCh; <                                                                 AddrTxPreDriverCal2Pad1 TxPreN
  AGESA_IDS_NV_ADDRTXPREDRIVERCAL2PAD2TXPREP                  EQU    02FDh; <                                                                 AddrTxPreDriverCal2Pad2 TxPreP
  AGESA_IDS_NV_ADDRTXPREDRIVERCAL2PAD2TXPREN                  EQU    02FEh; <                                                                 AddrTxPreDriverCal2Pad2 TxPreN
  AGESA_IDS_NV_ADDRTXPREDRIVERCAL2PAD3TXPREP                  EQU    02FFh; <                                                                 AddrTxPreDriverCal2Pad3 TxPreP
  AGESA_IDS_NV_ADDRTXPREDRIVERCAL2PAD3TXPREN                  EQU    0300h; <                                                                 AddrTxPreDriverCal2Pad3 TxPreN
  AGESA_IDS_NV_ADDRTXPREDRIVERCAL2PAD4TXPREP                  EQU    0301h; <                                                                 AddrTxPreDriverCal2Pad4 TxPreP
  AGESA_IDS_NV_ADDRTXPREDRIVERCAL2PAD4TXPREN                  EQU    0302h; <                                                                 AddrTxPreDriverCal2Pad4 TxPreN
  AGESA_IDS_NV_CLK0TXSLEWRATECTL                              EQU    0303h; <                                                        Clock0 Tx Calibration Slew Rate Control
  AGESA_IDS_NV_CLOCK0TXPREDRIVERCALPAD0TXPREP                 EQU    0304h; <                                                                Clock0TxPreDriverCalPad0 TxPreP
  AGESA_IDS_NV_CLOCK0TXPREDRIVERCALPAD0TXPREN                 EQU    0305h; <                                                                Clock0TxPreDriverCalPad0 TxPreN
  AGESA_IDS_NV_CLOCK1TXCALSLEWRATECTL                         EQU    0306h; <                                                        Clock1 Tx Calibration Slew Rate Control
  AGESA_IDS_NV_CLOCK1TXPREDRIVERCALPAD0TXPREP                 EQU    0307h; <                                                                Clock1TxPreDriverCalPad0 TxPreP
  AGESA_IDS_NV_CLOCK1TXPREDRIVERCALPAD0TXPREN                 EQU    0308h; <                                                                Clock1TxPreDriverCalPad0 TxPreN
  AGESA_IDS_NV_CLOCK2TXCALSLEWRATECTL                         EQU    0309h; <                                                        Clock2 Tx Calibration Slew Rate Control
  AGESA_IDS_NV_CLOCK2TXPREDRIVERCALPAD0TXPREP                 EQU    030Ah; <                                                                Clock2TxPreDriverCalPad0 TxPreP
  AGESA_IDS_NV_CLOCK2TXPREDRIVERCALPAD0TXPREN                 EQU    030Bh; <                                                                Clock2TxPreDriverCalPad0 TxPreN
  AGESA_IDS_SMU_PORT80_LOGGING                                EQU    030Ch; <                                                                     SMU port80 monitor logging
  AGESA_IDS_MSR_ACCESS                                        EQU    030Dh; <                                                                                     MSR Access
  AGESA_IDS_CTDP_NPST                                         EQU    030Eh; <                                                                  cTDP NbPstate Selection Table
  AGESA_IDS_NV_PSTATES_SUPPORT                                EQU    030Fh; <                                                                            NB P-states Support
  AGESA_IDS_NV_GNB_LOAD_SAMU_PATCH                            EQU    0310h; <                                                                                Load SAMU patch
  AGESA_IDS_NV_DLLSTAGGERDLY                                  EQU    0311h; <                                                                              Dll Stagger Delay
  AGESA_IDS_NV_CSAMPLE_TIMER_CTRL                             EQU    0312h; <                                                                                CSampleTimerCtl
  AGESA_IDS_NV_CSAMPLE_TIMER                                  EQU    0313h; <                                                                                   CSampleTimer
  AGESA_IDS_NV_PIPE0_THROTTLE_PACKED                          EQU    0314h; <                                                                            Pipe0ThrottlePacked
  AGESA_IDS_NV_PIPE1_THROTTLE_PACKED                          EQU    0315h; <                                                                            Pipe1ThrottlePacked
  AGESA_IDS_NV_PIPE0_THROTTLE_SELECT                          EQU    0316h; <                                                                            Pipe0ThrottleSelect
  AGESA_IDS_NV_PIPE1_THROTTLE_SELECT                          EQU    0317h; <                                                                            Pipe1ThrottleSelect
  AGESA_IDS_NV_NB_TRACING_BUFFER                              EQU    0318h; <                                                                              NB Tracing bugger
  AGESA_IDS_NV_DISSIMRDWR                                     EQU    0319h; <                                                            Disable simultaneous read and write
  AGESA_IDS_NV_DCTPWRGATEEN_CTRL                              EQU    031Ah; <                                                                Enable DCT power gating Control
  AGESA_IDS_NV_DCTPWRGATEEN                                   EQU    031Bh; <                                                                        Enable DCT power gating
  AGESA_IDS_NV_DCTCLKGATEEN_CTRL                              EQU    031Ch; <                                                                Enable DCT clock gating Control
  AGESA_IDS_NV_DCTCLKGATEEN                                   EQU    031Dh; <                                                                        Enable DCT clock gating
  AGESA_IDS_NV_ACP_ENGINE                                     EQU    031Eh; <                                                        Reserve system memory for KV ACP engine
  AGESA_IDS_NV_GNB_GPP_PCIEMASTERPLLSELECTION                 EQU    031Fh; <                                                                      PCIe Master PLL Selection
  AGESA_IDS_NV_WATCHDOG_TIMER                                 EQU    0320h; <                                                                         watchdog timer disable
  AGESA_IDS_NV_REDIRECT_INT1_INT3_TO_HDT                      EQU    0321h; <                                               redirect debug exceptions and breakpoints to HDT
  AGESA_IDS_NV_RDRAND                                         EQU    0322h; <                                                                                         RdRand
  AGESA_IDS_NV_EXCEPTION_BREAKPOINT_CTRL                      EQU    0323h; <                                                                   Exception breakpoint control
  AGESA_IDS_NV_EXCEPTION_BREAKPOINT_VECTOR                    EQU    0324h; <                                                                    Exception breakpoint vector
  AGESA_IDS_NV_ADAPT_FOM_BER_EN                               EQU    0325h; <                                                                   PHYLANE GEN3 FOM BER Control
  AGESA_IDS_NV_ADAPT_FOM_BER_VAL                              EQU    0326h; <                                                                     PHYLANE GEN3 FOM BER Value
  AGESA_IDS_NV_ADAPT_OFF_CAL_TIME_EN                          EQU    0327h; <                                                   PHYLANE GEN3 Offset Calculation Time Control
  AGESA_IDS_NV_ADAPT_OFF_CAL_TIME                             EQU    0328h; <                                                     PHYLANE GEN3 Offset Calculation Time Value
  AGESA_IDS_NV_ADAPT_CDR_TIME_EN                              EQU    0329h; <                                                                  PHYLANE GEN3 CDR Time Control
  AGESA_IDS_NV_ADAPT_CDR_TIME                                 EQU    032Ah; <                                                                    PHYLANE GEN3 CDR Time Value
  AGESA_IDS_NV_ADAPT_LEQ_TIME_EN                              EQU    032Bh; <                                                                  PHYLANE GEN3 LEQ Time Control
  AGESA_IDS_NV_ADAPT_LEQ_TIME                                 EQU    032Ch; <                                                                    PHYLANE GEN3 LEQ Time Value
  AGESA_IDS_NV_ADAPT_DFE_TIME_EN                              EQU    032Dh; <                                                                  PHYLANE GEN3 DFE Time Control
  AGESA_IDS_NV_ADAPT_DFE_TIME                                 EQU    032Eh; <                                                                    PHYLANE GEN3 DFE Time Value
  AGESA_IDS_NV_ADAPT_FOM_TIME_EN                              EQU    032Fh; <                                                                  PHYLANE GEN3 FOM Time Control
  AGESA_IDS_NV_ADAPT_FOM_TIME                                 EQU    0330h; <                                                                    PHYLANE GEN3 FOM Time Value
  AGESA_IDS_NV_ADAPT_PI_OFFSET_BYP_VAL_EN                     EQU    0331h; <                                                          PHYLANE GEN3 PI Offset Bypass Control
  AGESA_IDS_NV_ADAPT_PI_OFFSET_BYP_VAL                        EQU    0332h; <                                                            PHYLANE GEN3 PI Offset Bypass Value
  AGESA_IDS_NV_ADAPT_PI_OFFSET_BYP_EN                         EQU    0333h; <                                                           PHYLANE GEN3 PI Offset Bypass Enable
  AGESA_IDS_NV_ADAPT_TH_LOOPGAIN_EN                           EQU    0334h; <                                                              PHYLANE GEN3 TH Loop Gain Control
  AGESA_IDS_NV_ADAPT_TH_LOOPGAIN_VAL                          EQU    0335h; <                                                                PHYLANE GEN3 TH Loop Gain Value
  AGESA_IDS_NV_ADAPT_LEQ_LOOPGAIN_EN                          EQU    0336h; <                                                             PHYLANE GEN3 LEQ Loop Gain Control
  AGESA_IDS_NV_ADAPT_LEQ_LOOPGAIN                             EQU    0337h; <                                                               PHYLANE GEN3 LEQ Loop Gain Value
  AGESA_IDS_NV_RX_ADAPT_TRACK_EN                              EQU    0338h; <                                                         PHYLANE GEN3 Adaptive Tracking Control
  AGESA_IDS_NV_RX_ADAPT_TRACK                                 EQU    0339h; <                                                           PHYLANE GEN3 Adaptive Tracking Value
  AGESA_IDS_NV_TX_DIFF_EN                                     EQU    033Ah; <                                                     PHYLANE GEN3 Transmit Differential Control
  AGESA_IDS_NV_TX_DIFF                                        EQU    033Bh; <                                                       PHYLANE GEN3 Transmit Differential Value
  AGESA_IDS_NV_DFR_TIMER_CTL_EN                               EQU    033Ch; <                                                         PHYLANE GEN3 DFR Timer Control Control
  AGESA_IDS_NV_DFR_TIMER_CTL                                  EQU    033Dh; <                                                           PHYLANE GEN3 DFR Timer Control Value
  AGESA_IDS_NV_GEN12_PI_OFFSET_BYP_VAL_EN                     EQU    033Eh; <                                                    PHYLANE GEN3 Gen12 PI Offset Bypass Control
  AGESA_IDS_NV_GEN12_PI_OFFSET_BYP_VAL                        EQU    033Fh; <                                                      PHYLANE GEN3 Gen12 PI Offset Bypass Value
  AGESA_IDS_NV_GEN12_PI_OFFSET_BYP_EN                         EQU    0340h; <                                                     PHYLANE GEN3 Gen12 PI Offset Bypass Enable
  AGESA_IDS_NV_ANALOG_WAIT_TIME_EN                            EQU    0341h; <                                                          PHYLANE GEN3 Analog Wait Time Control
  AGESA_IDS_NV_ANALOG_WAIT_TIME                               EQU    0342h; <                                                            PHYLANE GEN3 Analog Wait Time Value
  AGESA_IDS_NV_PHASE_GAIN_GEN12_SEL_EN                        EQU    0343h; <                                                          PHYLANE GEN3 Gen12 Phase Gain Control
  AGESA_IDS_NV_PHASE_GAIN_GEN12_SEL                           EQU    0344h; <                                                            PHYLANE GEN3 Gen12 Phase Gain Value
  AGESA_IDS_NV_PHASE_GAIN_GEN3_SEL_EN                         EQU    0345h; <                                                                PHYLANE GEN3 Phase Gain Control
  AGESA_IDS_NV_PHASE_GAIN_GEN3_SEL                            EQU    0346h; <                                                                  PHYLANE GEN3 Phase Gain Value
  AGESA_IDS_NV_RECEIVER_POWER_MODE                            EQU    0347h; <                                                                            Reciever power mode
  AGESA_IDS_NV_GNB_ACP_POWER_GATING                           EQU    0348h; <                                                                               ACP Power Gating
  AGESA_IDS_NV_GNB_ACP_CLOCK_GATING                           EQU    0349h; <                                                                               ACP Clock Gating
  AGESA_IDS_NV_GNB_CHUB_CLOCK_GATING                          EQU    034Ah; <                                                                              CHUB CLOCK Gating
  AGESA_IDS_NV_DSM_LP_SEL_EN                                  EQU    034Bh; <                                                                                         Dsm Lp
  AGESA_IDS_NV_EQUALIZATION_SEARCH_MODE0                      EQU    034Ch; <                                                      Equalization Search Mode (PBr0/Gfx Port0)
  AGESA_IDS_NV_EQUALIZATION_SEARCH_MODE1                      EQU    034Dh; <                                                      Equalization Search Mode (PBr1/Gfx Port1)
  AGESA_IDS_NV_EQUALIZATION_PRESET0_PBR0                      EQU    034Eh; <                                                          Equalization Presets (PBr0/Gfx Port0)
  AGESA_IDS_NV_EQUALIZATION_PRESET0_PBR1                      EQU    034Fh; <                                                          Equalization Presets (PBr1/Gfx Port1)
  AGESA_IDS_NV_GEN1_LOOPBACK_MODE                             EQU    0350h; <                                                                           Gen 1 Loop back mode
  AGESA_IDS_CPU_FREQUENCY_LIMIT_EN                            EQU    0351h; <                                                                   CPU frequency limit  Control
  AGESA_IDS_CPU_FREQUENCY_LIMIT                               EQU    0352h; <                                                                            CPU frequency limit
  AGESA_IDS_NV_PMU_TRAIN_MODE                                 EQU    0353h; <                                                                                 PMU train mode
  AGESA_IDS_NV_GNB_PMM_EAPM                                   EQU    0354h; <                                                                            Global EAPM Control
  AGESA_IDS_NV_GNB_TARGET_TDP_CONTROL                         EQU    0355h; <                                                                             Target TDP Control
  AGESA_IDS_NV_GNB_TARGET_TDP_VALUE                           EQU    0356h; <                                                                               Target TDP Value
  AGESA_IDS_NV_SYNC_FLOOD_MCA                                 EQU    0357h; <                                                                                 Sync Flood MCA
  AGESA_IDS_NV_GNB_NB_IOMMU_L2_DEBUG_3_31                     EQU    0358h; <                                                                              L2_DEBUG_3 bit 31
  AGESA_IDS_NV_GNB_PMM_DCTDP                                  EQU    0359h; <                                                                                  DcTdp Control
  AGESA_IDS_NV_GNB_PMM_BBB_EN                                 EQU    035Ah; <                                                                                    BBB Control
  AGESA_IDS_NV_GNB_PMM_CHTC                                   EQU    035Bh; <                                                                                   CHTC Control
  AGESA_IDS_NV_GNB_PMM_LEAPM                                  EQU    035Ch; <                                                                             Local EAPM Control
  AGESA_IDS_NV_GNB_PMM_STAPM                                  EQU    035Dh; <                                                                                  STAPM Control
  AGESA_IDS_NV_GNB_PMM_ECT                                    EQU    035Eh; <                                                                                    ECT Control
  AGESA_IDS_NV_GNB_PMM_ECT_LIMIT_OVERRIDE_EN                  EQU    035Fh; <                                                                     ECT Limit Override Control
  AGESA_IDS_NV_GNB_PMM_ECT_LIMIT_OVERRIDE0                    EQU    0360h; <                                                                           ECT Limit Override 0
  AGESA_IDS_NV_GNB_PMM_ECT_LIMIT_OVERRIDE1                    EQU    0361h; <                                                                           ECT Limit Override 1
  AGESA_IDS_NV_EQUALIZATION_PRESET1_PBR0                      EQU    0362h; <                                                          Equalization Presets (PBr0/Gfx Port0)
  AGESA_IDS_NV_EQUALIZATION_PRESET1_PBR1                      EQU    0363h; <                                                          Equalization Presets (PBr1/Gfx Port1)
  AGESA_IDS_NV_EQUALIZATION_PRESET2_PBR0                      EQU    0364h; <                                                          Equalization Presets (PBr0/Gfx Port0)
  AGESA_IDS_NV_EQUALIZATION_PRESET2_PBR1                      EQU    0365h; <                                                          Equalization Presets (PBr1/Gfx Port1)
  AGESA_IDS_NV_EQUALIZATION_PRESET3_PBR0                      EQU    0366h; <                                                          Equalization Presets (PBr0/Gfx Port0)
  AGESA_IDS_NV_EQUALIZATION_PRESET3_PBR1                      EQU    0367h; <                                                          Equalization Presets (PBr1/Gfx Port1)
  AGESA_IDS_NV_LC_USC_EQ_NOT_REQD_PBR0                        EQU    0368h; <                                                          Equalization Presets (PBr0/Gfx Port0)
  AGESA_IDS_NV_LC_USC_EQ_NOT_REQD_PBR1                        EQU    0369h; <                                                          Equalization Presets (PBr1/Gfx Port1)
  AGESA_IDS_NV_GNB_DFSBYPASS                                  EQU    036Ah; <                                                                             DFS Bypass Control
  AGESA_IDS_NV_GNB_PMM_MULTIVID                               EQU    036Bh; <                                                                               GNB PMM MULTIVID
  AGESA_IDS_NV_GNB_PCIE_HW_INIT_PWR_GATING                    EQU    036Ch; <                                                             GNB PCIe Hardware Init PowerGating
  AGESA_IDS_NV_GNB_SPG_CLOCK_GATING                           EQU    036Dh; <                                                                           GNB SPG Clock gating
  AGESA_IDS_NV_GNB_ACG_AZ_CLOCK_GATING                        EQU    036Eh; <                                                                        GNB ACG_AZ Clock Gating
  AGESA_IDS_NV_GNB_IOMMU_L1_MEM_POWER_GATING                  EQU    036Fh; <                                                               GNB IOMMU L1 Memory Power Gating
  AGESA_IDS_NV_GNB_IOMMU_L2_MEM_POWER_GATING                  EQU    0370h; <                                                               GNB IOMMU L2 Memory Power Gating
  AGESA_IDS_NV_TREFCTL                                        EQU    0371h; <                                                                                   TREF Control
  AGESA_IDS_NV_TRFC0CTL                                       EQU    0372h; <                                                                                  TRFC0 Control
  AGESA_IDS_NV_TRFC1CTL                                       EQU    0373h; <                                                                                  TRFC1 Control
  AGESA_IDS_NV_DATASCRAMBLE                                   EQU    0374h; <                                                                                  Data Scramble
  AGESA_IDS_NV_RDPTR_NBP0_CTL                                 EQU    0375h; <                                                                      RdPtrInitVal NBP0 Control
  AGESA_IDS_NV_RDPTR_NBP0                                     EQU    0376h; <                                                                              RdPtrInitVal NBP0
  AGESA_IDS_NV_RDPTR_NBP1_CTL                                 EQU    0377h; <                                                                      RdPtrInitVal NBP1 Control
  AGESA_IDS_NV_RDPTR_NBP1                                     EQU    0378h; <                                                                              RdPtrInitVal NBP1
  AGESA_IDS_NV_RDPTR_NBP2_CTL                                 EQU    0379h; <                                                                      RdPtrInitVal NBP2 Control
  AGESA_IDS_NV_RDPTR_NBP2                                     EQU    037Ah; <                                                                              RdPtrInitVal NBP2
  AGESA_IDS_NV_RDPTR_NBP3_CTL                                 EQU    037Bh; <                                                                      RdPtrInitVal NBP3 Control
  AGESA_IDS_NV_RDPTR_NBP3                                     EQU    037Ch; <                                                                              RdPtrInitVal NBP3
  AGESA_IDS_NV_GNB_ORB_TX_MEM_POWER_GATING                    EQU    037Dh; <                                                                 GNB ORB TX Memory Power Gating
  AGESA_IDS_NV_GNB_ORB_RX_MEM_POWER_GATING                    EQU    037Eh; <                                                                 GNB ORB RX Memory Power Gating
  AGESA_IDS_NV_GNB_ACG_AZ_PRESENT                             EQU    037Fh; <                                                                             GNB ACG_AZ present
  AGESA_IDS_NV_GNB_SSTUNL_CLOCK_GATING                        EQU    0380h; <                                                                        GNB SSTUNL Clock Gating
  AGESA_IDS_NV_GNB_SMU_FEAT_BIT_MAP_CTRL                      EQU    0381h; <                                                                    GNB SMU Feature bit control
  AGESA_IDS_NV_GNB_SMU_FEAT_BIT_MAP_SET                       EQU    0382h; <                                                                        GNB SMU Feature bit Map
  AGESA_IDS_NV_GFXCURRENTLIMITCTL                             EQU    0383h; <                                                                      GFX Current Limit Control
  AGESA_IDS_NV_GFXCURRENTLIMIT                                EQU    0384h; <                                                                              GFX Current Limit
  AGESA_IDS_NV_GFXMAXCURRENTLIMIT                             EQU    0385h; <                                                                          GFX Max Current Limit
  AGESA_IDS_NV_PHYPOWERSAVING                                 EQU    0386h; <                                                                         DRAM Phy power savings
  AGESA_IDS_NV_MEMCLK_ABOVE_NCLK                              EQU    0387h; <                                                                        Enforce MClk above NClk
  AGESA_IDS_NV_GNB_SMU_PORT80_MEMLO                           EQU    0388h; <                                                                      GNB SMU PORT80 Memory low
  AGESA_IDS_NV_GNB_SMU_PORT80_MEMHI                           EQU    0389h; <                                                                     GNB SMU PORT80 Memory High
  AGESA_IDS_NV_GNB_SMU_PORT80_MEMSIZE                         EQU    038Ah; <                                                                 GNB SMU PORT80 Memory Position
  AGESA_IDS_NV_GNB_SMU_PORT80_MEMPOSN                         EQU    038Bh; <                                                                      GNB SMU PORT80 Memory low
  AGESA_IDS_NV_GNB_SMU_PORT80_POLL                            EQU    038Ch; <                                                                            GNB SMU PORT80 poll
  AGESA_IDS_NV_GNB_SMU_PORT80_CSR                             EQU    038Dh; <                                                                             GNB SMU PORT80 CSR
  AGESA_IDS_NV_GNB_SMU_PORT80_DRAM                            EQU    038Eh; <                                                                            GNB SMU PORT80 DRAM
  AGESA_IDS_NV_GNB_SMU_PORT80_EN                              EQU    038Fh; <                                                                          GNB SMU PORT80 Enable
  AGESA_IDS_NV_BANKGROUPSWAP                                  EQU    0390h; <                                                                                  BankGroupSwap
  AGESA_IDS_NV_WRCRCEN                                        EQU    0391h; <                                                                               Write CRC Enable
  AGESA_IDS_NV_STAPM_CONFIG_CONTROL                           EQU    0392h; <                                                                           STAPM Config Control
  AGESA_IDS_NV_STAPM_SCALAR                                   EQU    0393h; <                                                                                   STAPM Scalar
  AGESA_IDS_NV_STAPM_BOOST                                    EQU    0394h; <                                                                             STAPM Boost Enable
  AGESA_IDS_NV_STAPM_TIME_CONSTANT                            EQU    0395h; <                                                                            STAPM Time Constant
  AGESA_IDS_NV_MEM_MRL_RETRAIN_TIMES                          EQU    0396h; <                     Specify how many times MRL training should be executed should error occurs
  AGESA_IDS_INT_ID_END                                        EQU    0397h; <                                                              specify the end of internal NV ID
