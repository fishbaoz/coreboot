;/**
; * @file
; *
; * AMD Family_15 Stoney boot time calibration code
; *
; * @xrefitem bom "File Content Label" "Release Content"
; * @e project:      AGESA
; * @e sub-project:  CPU/Family/0x15/ST
; * @e \$Revision: 312756 $   @e \$Date: 2015-02-11 11:00:50 +0800 (Wed, 11 Feb 2015) $
; */
;*****************************************************************************
;
; Copyright 2008 - 2015 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
;
; AMD is granting you permission to use this software (the Materials)
; pursuant to the terms and conditions of your Software License Agreement
; with AMD.  This header does *NOT* give you permission to use the Materials
; or any rights under AMD's intellectual property.  Your use of any portion
; of these Materials shall constitute your acceptance of those terms and
; conditions.  If you do not agree to the terms and conditions of the Software
; License Agreement, please do not use any portion of these Materials.
;
; CONFIDENTIALITY:  The Materials and all other information, identified as
; confidential and provided to you by AMD shall be kept confidential in
; accordance with the terms and conditions of the Software License Agreement.
;
; LIMITATION OF LIABILITY: THE MATERIALS AND ANY OTHER RELATED INFORMATION
; PROVIDED TO YOU BY AMD ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
; WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
; MERCHANTABILITY, NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE,
; OR WARRANTIES ARISING FROM CONDUCT, COURSE OF DEALING, OR USAGE OF TRADE.
; IN NO EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER
; (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS
; INTERRUPTION, OR LOSS OF INFORMATION) ARISING OUT OF AMD'S NEGLIGENCE,
; GROSS NEGLIGENCE, THE USE OF OR INABILITY TO USE THE MATERIALS OR ANY OTHER
; RELATED INFORMATION PROVIDED TO YOU BY AMD, EVEN IF AMD HAS BEEN ADVISED OF
; THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE
; EXCLUSION OR LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES,
; THE ABOVE LIMITATION MAY NOT APPLY TO YOU.
;
; AMD does not assume any responsibility for any errors which may appear in
; the Materials or any other related information provided to you by AMD, or
; result from use of the Materials or any related information.
;
; You agree that you will not reverse engineer or decompile the Materials.
;
; NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
; further information, software, technical information, know-how, or show-how
; available to you.  Additionally, AMD retains the right to modify the
; Materials at any time, without notice, and is not obligated to provide such
; modified Materials to you.
;
; U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with
; "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
; subject to the restrictions as set forth in FAR 52.227-14 and
; DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
; Government constitutes acknowledgement of AMD's proprietary rights in them.
;
; EXPORT ASSURANCE:  You agree and certify that neither the Materials, nor any
; direct product thereof will be exported directly or indirectly, into any
; country prohibited by the United States Export Administration Act and the
; regulations thereunder, without the required authorization from the U.S.
; government nor will be used for any purpose prohibited by the same.
;*****************************************************************************
 text  SEGMENT

XSET_BV MACRO
        db 00fh, 001h, 0d1h
ENDM

XGET_BV MACRO
        db 00fh, 001h, 0d0h
ENDM

;======================================================================
; ExecuteF15StBtc:  Wrapper code for boot time calibration workloads
;
;   In:
;       rdx - buffer address for workloads
;       rcx - test to run
;
;   Out:
;       None
;
;   Destroyed:
;       None
;
;======================================================================
ExecuteF15StBtc PROC PUBLIC
  push  rax
  push  rbx
  push  rcx
  push  rdx
  push  rdi
  push  rsi
  push  rbp
  push  r8
  push  r9
  push  r10
  push  r11
  push  r12
  push  r13
  push  r14
  push  r15
  pushfq

  add   rdx, 63
  and   rdx, 0FFFFFFFFFFFFFFC0h
  push  rcx
  push  rdx
  mov   rax, 1
  cpuid
  bt    rcx, 26
  jc    SetupAVX
  pop   rdx
  pop   rcx
  jmp   NoAVX

SetupAVX:
  mov   r10, cr4
  mov   r11, r10
  bts   r10, 18
  mov   cr4, r10
  xor   rcx, rcx
  XGET_BV
  mov   r12, rax
  mov   r13, rdx
  mov   rax, 0dh
  cpuid
  xor   rdx, rdx
  xor   rcx, rcx
  XSET_BV
  pop   rdx
  pop   rcx
  push  r11
  push  r12
  push  r13
  call  cpuF15StBtc
  pop   rdx
  pop   rax
  xor   rcx, rcx
  XSET_BV
  pop   r11
  mov   cr4, r11

NoAVX:
  popfq
  pop   r15
  pop   r14
  pop   r13
  pop   r12
  pop   r11
  pop   r10
  pop   r9
  pop   r8
  pop   rbp
  pop   rsi
  pop   rdi
  pop   rdx
  pop   rcx
  pop   rbx
  pop   rax
  ret
ExecuteF15StBtc ENDP

;======================================================================
; cpuF15StBtc: The workloads.
;
;   In:
;       rdx - buffer address for workloads
;       rcx - test to run
;
;   Out:
;       None
;
;   Destroyed:
;       Many
;
;======================================================================
cpuF15StBtc PROC PUBLIC
include cpuF15StBtc64.inc
cpuF15StBtc ENDP

END

