;*****************************************************************************
; AMD Generic Encapsulated Software Architecture
;
; $Workfile:: cpcar.inc
;
; Description: CPCAR.INC - AGESA cache-as-RAM setup Include File
;
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
    .LIST
    .mmx

BSP_STACK_BASE_ADDR     EQU 30000h      ; Base address for primary cores stack
BSP_STACK_SIZE_64K      EQU 10000h      ; 64KB for BSP core
BSP_STACK_SIZE_32K      EQU 8000h       ; 32KB for BSP core
CORE0_STACK_BASE_ADDR   EQU 80000h      ; Base address for primary cores stack
CORE0_STACK_SIZE        EQU 4000h       ; 16KB for primary cores
CORE1_STACK_BASE_ADDR   EQU 40000h      ; Base address for AP cores
CORE1_STACK_SIZE        EQU 1000h       ; 4KB for each AP cores

;============================================================================
;
; Define a  macro that allow the exclusion of processor families from
; the cache-as-ram code. This will reduce the size of the assembled file.
;
;============================================================================

;---------------------------------------------------
;
; AMD_ENABLE_STACK_FAMILY_HOOK Macro - Stackless
;
;   Set any family specific controls needed to enable the use of
;   cache as general storage before main memory is available.
;
; Inputs:
;       ESI - node#, core#, flags from GET_NODE_ID_CORE_ID
; Outputs:
;       none
; Destroyed:
;       eax, ebx, ecx, edx
;---------------------------------------------------
AMD_ENABLE_STACK_FAMILY_HOOK MACRO

    AMD_ENABLE_STACK_FAMILY_HOOK_F15

ENDM

;----------------------------------------------
;
; AMD_DISABLE_STACK_FAMILY_HOOK Macro - Stackless
;
;   Return any family specific controls to their 'standard'
;   settings for using cache with main memory.
;
; Inputs:
;       ESI - node#, core#, flags from GET_NODE_ID_CORE_ID
; Outputs:
;       none
; Destroyed:
;       eax, ebx, ecx, edx
;----------------------------------------------
AMD_DISABLE_STACK_FAMILY_HOOK MACRO ApComm

    AMD_DISABLE_STACK_FAMILY_HOOK_F15 ApComm

ENDM

;---------------------------------------------------
;
; GET_NODE_ID_CORE_ID Macro - Stackless
;
;   Read family specific values to determine the node and core
;   numbers for the core executing this code.
;
; Inputs:
;     none
; Outputs:
;     SI[7:0] = Core# (0..N, relative to node)
;     SI[15:8]= Node# (0..N)
;     SI[23:16]= reserved
;     SI[24]=   flag: 1=Family Unrecognized
;     SI[25]=   flag: 1=Interface re-entry call
;     SI[26]=   flag: 1=Core is primary of compute unit
;     SI[31:27]= reserved, =0
;
; Destroyed:
;       eax, ebx, ecx, edx, esi
;---------------------------------------------------
GET_NODE_ID_CORE_ID MACRO

    mov     si, -1
    GET_NODE_ID_CORE_ID_F15

    ;
    ; Check for unrecognized Family
    ;
    .if (si == -1)                      ; Has family (node/core) been discovered?
        mov     esi, ( (1 SHL FLAG_UNKNOWN_FAMILY)+(1 SHL FLAG_IS_PRIMARY) ) ; No, Set error code, Only let BSP continue
        mov     ecx, APIC_BASE_ADDRESS  ; MSR:0000_001B
        _RDMSR
        bt      eax, APIC_BSC           ;   Is this the BSC?
        .if (!carry?)
            ; No, this is an AP
            hlt                         ;       Kill APs
        .endif
    .endif
ENDM


;;***************************************************************************
;;                      Family 15h MACROS
;;***************************************************************************
;---------------------------------------------------
;
; AMD_ENABLE_STACK_FAMILY_HOOK_F15 Macro - Stackless
;
;   Set any family specific controls needed to enable the use of
;   cache as general storage before main memory is available.
;
; Inputs:
;       ESI - node#, core#, flags from GET_NODE_ID_CORE_ID
; Outputs:
;       none
; Destroyed:
;       eax, ebx, ecx, edx
;
; Family 15h CZ requirements:
;   * Paging must be disabled.
;   * MSRC001_0015[INVDWBINVD]=0
;   * MSRC001_101C[DisSS]=1
;   * MSRC001_1021[DisSpecTlb]=1
;   * MSRC001_101C[DisSpecTlb]=1
;   * MSRC001_101C[DisHwPf]=1
;   * No INVD or WBINVD, no exceptions, page faults or interrupts
;---------------------------------------------------
AMD_ENABLE_STACK_FAMILY_HOOK_F15 MACRO
    local   fam15_enable_stack_hook_exit

    AMD_CPUID   CPUID_MODEL
    mov     ebx, eax                    ; Save revision info to EBX
    shr     eax, 20                     ; AL = cpu extended family
    cmp     al, 06h                     ; Is this family 15h?
    jnz     fam15_enable_stack_hook_exit ; Br if no

    bt      esi, FLAG_STACK_REENTRY     ; Check if stack has already been set
    .if (!carry?)
        mov     ecx, HWCR               ; MSR C001_0015
        _RDMSR
        btr     eax, INVD_WBINVD        ; disable INVD -> WBINVD conversion
        _WRMSR
    .endif

    mov     ecx, LS_CFG3                ; MSR:C001_101C
    _RDMSR
    bts     eax, DIS_SS_F15CZ           ; Turn on Streaming store functionality disabled bit
    _WRMSR

    mov     ecx, IC_CFG                 ; MSR:C001_1021
    _RDMSR
    bts     eax, IC_DIS_SPEC_TLB_RLD    ; Turn on Disable speculative IC-TLB reloads bit
    _WRMSR

    mov     ecx, LS_CFG3                ; MSR:C001_101C
    _RDMSR
    bts     eax, DC_DIS_SPEC_TLB_RLD_F15CZ  ; Turn on Disable speculative DC-TLB reloads bit
    bts     eax, DC_DIS_HW_PF_F15CZ         ; Turn on Disable hardware prefetches bit
    _WRMSR

    ; Do CZ enable stack special
    mov     ecx, CU_CFG             ; MSR:C001_1023
    _RDMSR
    bt      eax, L2_WAY_LOCK_EN     ; Check if way 15 of the L2 needs to be reserved
    .if (!carry?)
        bts     eax, L2_WAY_LOCK_EN
        or      eax, L2_FIRST_LOCKED_WAY_OR_MASK
        _WRMSR
    .endif

    ; Do Standard Family 15 work
    mov     ecx, CU_CFG3                ; MSR:C001_102B
    _RDMSR
    btr     edx, (COMBINE_CR0_CD - 32)  ; Clear CombineCr0Cd bit
    _WRMSR

fam15_enable_stack_hook_exit:
ENDM


;----------------------------------------------
;
; AMD_DISABLE_STACK_FAMILY_HOOK_F15 Macro - Stackless
;
;   Return any family specific controls to their 'standard'
;   settings for using cache with main memory.
;
; Inputs:
;       ESI - [31:24] flags; [15:8]= Node#; [7:0]= core#
; Outputs:
;       none
; Destroyed:
;       eax, ebx, ecx, edx
;
; Family 15h CZ requirements:
;   * INVD or WBINVD
;   * MSRC001_0015[INVD_WBINVD]=1
;   * MSRC001_101C[DisSS]=0
;   * MSRC001_1021[DisSpecTlb]=0
;   * MSRC001_101C[DisSpecTlb]=0
;   * MSRC001_101C[DisHwPf]=0
;---------------------------------------------------
AMD_DISABLE_STACK_FAMILY_HOOK_F15 MACRO ApComm
    local   fam15_disable_stack_hook_exit

    AMD_CPUID   CPUID_MODEL
    mov     ebx, eax                    ; Save revision info to EBX
    shr     eax, 20                     ; AL = cpu extended family
    cmp     al, 06h                     ; Is this family 15h?
    jnz     fam15_disable_stack_hook_exit ; Br if no

    mov     edi, ebx                    ; Save revision info to EDI
    AMD_CPUID   AMD_CPUID_APIC
    mov     al, cl                      ; AL = number of cores - 1
    shr     cx, APIC_ID_CORE_ID_SIZE    ; CL = ApicIdCoreIdSize
    mov     bx, 1
    shl     bl, cl                      ; BL = theoretical number of cores on socket
    dec     bx                          ; BL = core number on socket mask
    mov     ah, bl                      ; AH = core number on socket mask
    mov     ebx, edi                    ; Restore revision info to EBX
    mov     di, ax                      ; DI[15:8] = core number mask, DI[7:0] = number of cores - 1

    and     ebx, 0F00FFh
    mov     eax, ebx
    shr     eax, 8
    or      bx, ax                      ; Save Extended Model, Model and Stepping to BX
                                        ; [11:8] = Extended Model, [7:4] = Model, [3:0] = Stepping

    ;; A handshake is required to ensure that all cores on a node invalidate in sync.
    ;    Entry:
    ;      BX: [11:8] = Extended Model,   [7:4] = Model, [3:0] = Stepping
    ;      DI: [15:8] = core number mask, [7:0] = number of cores - 1
    IFNB <ApComm>
        IF (ApComm EQ AP_COMM_PCI)
            ; below MACRO would destroy BX. But it's fine since we won't need revision info again.
            HANDSHAKE_F15_PCI
        ELSE
            HANDSHAKE_F15
        ENDIF
    ELSE
        HANDSHAKE_F15
    ENDIF
    ;    Exit:
    ;     BX:
    ;       HANDSHAKE_F15      - [11:8] = Extended Model,   [7:4] = Model, [3:0] = Stepping
    ;       HANDSHAKE_F15_PCI  - [7:0]  = Core ID
    ;     EDI: CU flag
    ;       bit[31] - AMD_CU_NEED_TO_WAIT
    ;       bit[30] - AMD_CU_SEND_INVD_MSG
    ;       bit[29] - AMD_CU_RESTORE_ES

    ;; Handshaking complete.  Continue tearing down CAR.
    mov     ecx, LS_CFG3                ; MSR:C001_101C
    _RDMSR
    btr     eax, DIS_SS_F15CZ           ; Turn on Streaming store functionality
    _WRMSR

    mov     ecx, IC_CFG                 ; MSR:C001_1021
    _RDMSR
    btr     eax, IC_DIS_SPEC_TLB_RLD    ; Turn on speculative TLB reloads
    _WRMSR

    mov     ecx, LS_CFG3                ; MSR:C001_101C
    _RDMSR
    btr     eax, DC_DIS_SPEC_TLB_RLD_F15CZ  ; Turn on speculative TLB reloads
    btr     eax, DC_DIS_HW_PF_F15CZ         ; Turn on hardware prefetches
    _WRMSR

    mov     ecx, HWCR                   ; MSR:C001_0015h
    _RDMSR
    btr     eax, INVD_WBINVD            ; Disable INVD -> WBINVD conversion
    _WRMSR
    invd                                ; Clear the cache tag RAMs

    ; Do CZ disable stack special
    mov     ecx, CU_CFG             ; MSR:C001_1023
    _RDMSR
    shr      eax, L2_FIRST_LOCKED_WAY
    and      eax, 01Fh
    .if (eax == 01Fh)               ; Check if way 15 of the L2 needs to be reserved
        _RDMSR                      ; ECX = CU_CFG, MSR:C001_1023
        btr     eax, L2_WAY_LOCK_EN
        _WRMSR
    .endif

    ; Do Standard Family 15 work
    mov     ecx, HWCR                   ; MSR:C001_0015h
    _RDMSR
    bts     eax, INVD_WBINVD            ; Turn on INVD -> WBINVD conversion
    _WRMSR

    mov     ecx, CU_CFG3                ; MSR:C001_102B
    _RDMSR
    bts     edx, (COMBINE_CR0_CD - 32)  ; Set CombineCr0Cd bit
    _WRMSR

    bt      edi, AMD_CU_SEND_INVD_MSG
    .if (carry?)
        ;; Non core zero needs to signal to core 0 to proceed
        IFNB <ApComm>
            IF (ApComm EQ AP_COMM_PCI)
                SIGNAL_OTHER_CORE_F15_PCI
            ELSE
                SIGNAL_OTHER_CORE_F15
            ENDIF
        ELSE
            SIGNAL_OTHER_CORE_F15
        ENDIF
    .endif

fam15_disable_stack_hook_exit:
ENDM

;---------------------------------------------------
;
; HANDSHAKE_F15 Macro - Stackless
;
;   A handshake is required to ensure that all cores
;   on a node invalidate in sync.
;
; Inputs:
;     BX:  [11:8] = Extended Model,   [7:4] = Model, [3:0] = Stepping
;     DI:  [15:8] = core number mask, [7:0] = number of cores - 1
; Outputs:
;     BX:  [11:8] = Extended Model,   [7:4] = Model, [3:0] = Stepping
;     EDI: CU flag
;       bit[31] - AMD_CU_NEED_TO_WAIT
;       bit[30] - AMD_CU_SEND_INVD_MSG
;       bit[29] - AMD_CU_RESTORE_ES
; Destroyed:
;       eax, ebx, ecx, edx
;---------------------------------------------------
HANDSHAKE_F15 MACRO
    local   fam15_disable_stack_remote_read_exit

    mov     ecx, APIC_BASE_ADDRESS
    _RDMSR
    mov     dx, bx                      ; Save Extended Model, Model and Stepping to DX
    shl     edx, 16                     ; EDX[31:16] = Extended Model, Model and Stepping
    mov     ebx, eax                    ; EBX = LAPIC base
    xor     ecx, ecx                    ; Zero out CU flags
    bts     ecx, AMD_CU_NEED_TO_WAIT    ; Default to waiting
    bts     ecx, AMD_CU_SEND_INVD_MSG   ; Default to signaling
    mov     eax, CR0
    bt      ax, CR0_PE                  ; Are we in protected mode?
    .if (!carry?)
        bts     ecx, AMD_CU_RESTORE_ES  ; Indicate ES restore is required
        mov     cx, es                  ; Save ES segment register to CX
        xor     ax, ax
        mov     es, ax                  ; Set ES to big real mode selector for 4GB access
    .endif

    and     bx, 0F000h                  ; EBX = LAPIC base, offset 0
    or      bl, APIC_ID_REG
    mov     eax, es:[ebx]               ; EAX[31:24] = APIC ID
    shr     eax, APIC20_APICID          ; AL = APIC ID
    mov     ah, al                      ; AH = APIC ID
    mov     dx, di                      ; DH = core mask
    and     ah, dh                      ; AH = core number
    .if (zero?)
        ;; Core 0 of a socket
        btr     ecx, AMD_CU_SEND_INVD_MSG ; No need to signal after INVD
        .if (dl != 0)
            ;; This socket has multiple cores
            and     bx, 0F000h          ; EBX = LAPIC base, offset 0
            or      bx, APIC_MSG_REG
            mov     edi, APIC_MSG
            mov     es:[ebx], edi       ; Signal for non core 0s to complete CAR breakdown
        .else
            btr     ecx, AMD_CU_NEED_TO_WAIT ; No need to wait on a single core CPU
        .endif
    .endif

    bt     ecx, AMD_CU_NEED_TO_WAIT
    .if (carry?)
        .if (ah == dl)
            ;; This is the highest numbered core on this socket -- wait on core 0
            not     dh                  ; Flip the mask to determine local core 0's APIC ID
            and     al, dh              ; AL = target APIC ID
        .else
            ;; All other cores (including core 0) wait on the next highest core.
            ;; In this way, cores will halt in a cascading fashion down to 0.
            inc     al
        .endif

        shl     eax, APIC20_APICID
        and     bx, 0F000h
        or      bx, APIC_CMD_HI_REG
        mov     es:[ebx], eax           ; Set target APIC ID

        ;; Use bits 23:16 as a timeout for unresponsive cores
        ror     ecx, 8
        mov     ch, 0FFh
        stc
        .while (carry?)
            and     bx, 0F000h          ; EBX = LAPIC base, offset 0
            or      bx, APIC_CMD_LO_REG
            mov     eax, CMD_REG_TO_READ_DATA
            mov     es:[ebx], eax       ; Fire remote read IPI
            inc     ch                  ; Pre increment the timeout
            stc
            .while (carry?)
                dec     ch              ; Check the timeout
                jz      fam15_disable_stack_remote_read_exit   ; Branch if there is an unresponsive core
                mov     eax, es:[ebx]
                bt      eax, DELIVERY_STS_BIT
            .endw
            stc
            .while (carry?)
                mov     eax, es:[ebx]
                and     eax, REMOTE_READ_STS
                .if (eax == REMOTE_DELIVERY_PEND)
                    dec     ch          ; Check the timeout
                    jz      fam15_disable_stack_remote_read_exit   ; Branch if there is an unresponsive core
                    stc
                .else
                    clc
                .endif
            .endw
            .if (eax == REMOTE_DELIVERY_DONE)
                and     bx, 0F000h      ; EBX = LAPIC base, offset 0
                or      bl, APIC_REMOTE_READ_REG
                mov     eax, es:[ebx]
                .if (eax == APIC_MSG)
                    clc
                .else
                    stc
                .endif
            .else
                dec     ch              ; Check the timeout
                jz      fam15_disable_stack_remote_read_exit   ; Branch if there is an unresponsive core
                stc
            .endif
        .endw

fam15_disable_stack_remote_read_exit:
        rol     ecx, 8                  ; Restore ECX

    .endif
    bt      ecx, AMD_CU_RESTORE_ES
    .if (carry?)
        mov     es, cx
    .endif
    mov     edi, ecx                    ; EDI = CU flags
    shr     edx, 16
    mov     bx,  dx
ENDM

;---------------------------------------------------
;
; HANDSHAKE_F15_PCI Macro - Stackless
;
;   The same as HANDSHAKE_F15, but this macro
;   use PCI registers to do AP communication
;
; Inputs:
;     BX:  [11:8] = Extended Model,   [7:4] = Model, [3:0] = Stepping
;     DI:  [15:8] = core number mask, [7:0] = number of cores - 1
; Outputs:
;     BL:  Core ID
;     EDI: CU flag
;       bit[31] - AMD_CU_NEED_TO_WAIT
;       bit[30] - AMD_CU_SEND_INVD_MSG
;       bit[29] - AMD_CU_RESTORE_ES
; Destroyed:
;       eax, ebx, ecx, edx
;---------------------------------------------------
HANDSHAKE_F15_PCI MACRO
    ; Below CPUID instruction would destroy BX which contains revision info.
    ; It's fine since we don't need it anymore
    ; But if we want to keep revision info, please refer to HANDSHAKE_F15 to
    ; get core ID
    AMD_CPUID   CPUID_MODEL
    shr     ebx, LOCAL_APIC_ID          ; BL = core ID

    mov     ecx, MMIO_CFG_BASE
    _RDMSR
    and     eax, 0FFF00000h             ; We assume MMIO base is below 4G
    mov     edx, eax                    ; EDX = MMIO base address

    xor     ecx, ecx                    ; Zero out CU flags
    bts     ecx, AMD_CU_NEED_TO_WAIT    ; Default to waiting
    bts     ecx, AMD_CU_SEND_INVD_MSG   ; Default to signaling
    mov     cl,  bl                     ; Save BL to CL, CL = core ID

    mov     ax, di                      ; AH = core mask
    and     cl, ah                      ; CL = core ID
    .if (zero?)
        ;; Core 0 of a socket
        btr     ecx, AMD_CU_SEND_INVD_MSG ; No need to signal after INVD
        .if (al != 0)
            ;; This socket has multiple cores
            AP_COMM_CORE0_DATA_ADDR     ; EAX = Core0 data address
            add     eax, edx
            mov     ebx, APIC_MSG
            mov     es:[eax], ebx       ; Signal for non core 0s to complete CAR breakdown
        .else
            btr     ecx, AMD_CU_NEED_TO_WAIT ; No need to wait on a single core CPU
        .endif
    .endif

    mov     ax, di                      ; AL = number of cores
    bt      ecx, AMD_CU_NEED_TO_WAIT
    .if (carry?)
        .if (cl == al)
            ;; This is the highest numbered core on this socket -- wait on core 0
            mov     al, 0
        .else
            ;; All other cores (including core 0) wait on the next highest core.
            ;; In this way, cores will halt in a cascading fashion down to 0.
            mov     al, cl
            inc     al
        .endif
        ; AL - Target core ID

        mov     bl, 4
        mul     bl
        mov     bx, ax                  ; Get offset of data address of target core
        AP_COMM_CORE0_DATA_ADDR         ; EAX = Core0 data address
        add     ax, bx
        add     eax, edx                ; EAX = Target core's data address in MMIO configuration space

        stc
        .while (carry?)
            mov     edx, es:[eax]
            .if (edx == APIC_MSG)
                clc
            .else
                stc
            .endif
        .endw
    .endif
    mov     bl, cl                      ; Restore BL
    xor     cl, cl
    mov     edi, ecx                    ; EDI = CU flags
ENDM

;---------------------------------------------------
;
; SIGNAL_OTHER_CORE_F15 Macro - Stackless
;
;   Non core zero needs to signal to other cores to proceed
;
; Inputs:
;     BX:  Core ID
;     DI:  [15:8] = core number mask, [7:0] = number of cores - 1
; Outputs:

; Destroyed:
;       eax, ebx, ecx, edx
;---------------------------------------------------
SIGNAL_OTHER_CORE_F15 MACRO
    mov     ecx, APIC_BASE_ADDRESS
    _RDMSR
    mov     ebx, eax                ; EBX = LAPIC base
    and     bx, 0F000h              ; EBX = LAPIC base, offset 0
    or      bx, APIC_MSG_REG
    mov     eax, APIC_MSG
    mov     es:[ebx], eax           ; Signal for core 0 to complete CAR breakdown
ENDM

;---------------------------------------------------
;
; SIGNAL_OTHER_CORE_F15_PCI Macro - Stackless
;
;   The same as SIGNAL_OTHER_CORE_F15, but this macro
;   use PCI registers to do AP communication
;
; Inputs:
;     BL:  [11:8] = Extended Model,   [7:4] = Model, [3:0] = Stepping
;     DI:  [15:8] = core number mask, [7:0] = number of cores - 1
; Outputs:

; Destroyed:
;       eax, ebx, ecx, edx
;---------------------------------------------------
SIGNAL_OTHER_CORE_F15_PCI MACRO
    mov     ecx, MMIO_CFG_BASE
    _RDMSR
    and     eax, 0FFF00000h     ; We assume MMIO base is below 4G
    mov     edx, eax            ; EDX = MMIO base address

    mov     al, bl
    mov     bl, 4
    mul     bl
    mov     bx, ax              ; Get offset of data address of target core
    AP_COMM_CORE0_DATA_ADDR     ; EAX = Core0 data address
    add     ax, bx
    add     eax, edx            ; EAX = Target core's data address in MMIO configuration space

    mov     edx, APIC_MSG
    mov     es:[eax], edx
ENDM

;---------------------------------------------------
;
; GET_NODE_ID_CORE_ID_F15 Macro - Stackless
;
;   Read family specific values to determine the node and core
;   numbers for the core executing this code.
;
; Inputs:
;     none
; Outputs:
;     ESI = core#, node# & flags (see GET_NODE_ID_CORE_ID macro above)
;     MM5 = 32b pointer to family info structure
; Destroyed:
;       eax, ebx, ecx, edx, esi, mm5
;---------------------------------------------------
GET_NODE_ID_CORE_ID_F15 MACRO

    local   node_core_f15_exit
    local   end_of_f15h_data

    IFNDEF FAM15H_INFO_STRUCT
        jmp     end_of_f15h_data
        ; Family 15h Info Structure:          L2Size,  #SharedCores, AllocMem, AllocExe, SzAddrBus, pad
        FAM15H_INFO_STRUCT  CPU_FAMILY_INFO {  512,         2,          0,     0,         48,         0    }
end_of_f15h_data:
    ENDIF

    cmp     si, -1                      ; Has node/core already been discovered?
    jnz     node_core_f15_exit          ; Br if yes

    AMD_CPUID   CPUID_MODEL
    shr     eax, 12                     ; AH = cpu extended family
    cmp     ah, 06h                     ; Is this family 15h?
    jnz     node_core_f15_exit          ; Br if no
    shr     al, 4                       ; AL = cpu extended model
    shr     ebx, 16                     ; BH = LocalApicId
    mov     bl, al                      ; BL = cpu extended model

    LoadTableAddress(FAM15H_INFO_STRUCT)
    movd    mm5, eax                    ; load pointer to Family Info Struc

    xor     esi, esi                    ; Assume BSC, clear local flags
    mov     ecx, APIC_BASE_ADDRESS      ; MSR:0000_001B
    _RDMSR
    bt      eax, APIC_BSC               ; Is this the BSC?
    .if (carry?)
        ; This is the BSP.
        ; Enable routing tables on BSP (just in case the topology init code has not yet enabled them)
        mov     eax, 8000C06Ch          ;   PCI address for D18F0x6C Link Initialization Control Register
        mov     dx, 0CF8h
        out     dx, eax
        add     dx, 4
        in      eax, dx
        btr     eax, 0                  ;   Set LinkInitializationControl[RouteTblDis] = 0
        out     dx, eax
    .else                               ;
        mov     al, bl                  ;   AL = cpu extended model
        shr     bx, 8                   ;   BL = CPUID Fn0000_0001_EBX[LocalApicId]
        mov si, bx                      ;   SI = [15:8]= Node# = 0; [7:0]= core#
    .endif                              ; end

    ;
    ;   determine if this core shares MTRRs
    ;
    mov     eax, 8000C580h              ; Compute Unit Status
    mov     bx, si                      ; load node#(bh), core#(bl)
    shl     bh, 3                       ; Move node# to PCI Dev# field
    add     ah, bh                      ; Adjust PCI address for node number
    mov     dx, 0CF8h
    out     dx, eax
    add     dx, 4
    in      eax, dx                     ; [3:0]=Enabled; [19:16]=DualCore

                                        ; BL is MyCore#  , BH is primary flag
    mov     cx, 08h                     ; Use CH as 'first of pair' core#
    .while (cl > 0)
        bt      eax, 0                  ;   Is pair enabled?
        .if (carry?)                    ;
            mov     bh, 01h             ;   flag core as primary
            bt      eax, 16             ;   Is there a 2nd in the pair?
            .if (carry?)                ;
                .break .if (ch == bl)   ;     Does 1st match MyCore#?
                inc     ch
                xor     bh, bh          ;     flag core as NOT primary
                .break .if (ch == bl)   ;     Does 2nd match MyCore#?
            .else                       ;   No 2nd core
                .break .if (ch == bl)   ;     Does 1st match MyCore#?
            .endif
            inc     ch
        .endif
        shr     eax, 1
        dec     cl
    .endw
    .if (cl == 0)
        ;Error - core# didn't match Compute Unit Status content
        bts     esi, FLAG_CORE_NOT_IDENTIFIED
        bts     esi, FLAG_IS_PRIMARY    ;   Set Is_Primary for unknowns
    .endif
    .if (bh != 0)                       ; Check state of primary for the matched core
        bts     esi, FLAG_IS_PRIMARY    ;   Set shared flag into return value
    .endif
node_core_f15_exit:
ENDM


