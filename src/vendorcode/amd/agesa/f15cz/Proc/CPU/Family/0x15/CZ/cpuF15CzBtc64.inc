;*****************************************************************************
; AMD Generic Encapsulated Software Architecture
;
;  cpuFxxyyBtc64.inc (xx denotes family, yy denotes program)
;
; Description: Compiled 64 bit boot time calibration workloads.
;              Compile source : CZ\combo1
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

  db 048h, 089h, 015h, 02Bh, 000h, 000h, 000h, 048h, 089h, 00Dh, 01Ch, 000h, 000h, 000h, 048h, 0C7h
  db 005h, 021h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 048h, 083h, 0F9h, 001h, 072h, 02Bh, 00Fh
  db 084h, 002h, 008h, 000h, 000h, 0E9h, 0C2h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
  db 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
  db 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 090h, 090h, 090h, 090h, 090h, 090h
  db 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h
  db 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h
  db 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h
  db 041h, 0B9h, 001h, 000h, 000h, 000h, 048h, 031h, 0DBh, 0B8h, 001h, 000h, 000h, 000h, 00Fh, 0A2h
  db 048h, 0C1h, 0EBh, 018h, 041h, 0BCh, 032h, 000h, 000h, 000h, 048h, 083h, 0E3h, 001h, 00Fh, 085h
  db 0D6h, 002h, 000h, 000h, 041h, 0B9h, 000h, 000h, 000h, 000h, 048h, 08Bh, 035h, 081h, 0FFh, 0FFh
  db 0FFh, 048h, 081h, 0C6h, 030h, 002h, 000h, 000h, 041h, 0BCh, 032h, 000h, 000h, 000h, 0B8h, 004h
  db 000h, 000h, 000h, 048h, 089h, 006h, 0B8h, 020h, 000h, 000h, 000h, 048h, 089h, 046h, 008h, 0B8h
  db 0F9h, 003h, 000h, 000h, 048h, 089h, 046h, 010h, 0B8h, 010h, 000h, 000h, 000h, 048h, 089h, 046h
  db 018h, 0B8h, 0B8h, 00Bh, 000h, 000h, 048h, 089h, 046h, 020h, 048h, 08Bh, 035h, 041h, 0FFh, 0FFh
  db 0FFh, 048h, 081h, 0C6h, 058h, 002h, 000h, 000h, 048h, 0C7h, 0C0h, 0FFh, 0FFh, 0FFh, 0FFh, 048h
  db 089h, 006h, 048h, 089h, 046h, 008h, 048h, 089h, 046h, 010h, 048h, 089h, 046h, 018h, 0B8h, 000h
  db 000h, 000h, 000h, 048h, 089h, 046h, 020h, 048h, 089h, 046h, 028h, 048h, 089h, 046h, 030h, 048h
  db 089h, 046h, 038h, 048h, 08Bh, 035h, 008h, 0FFh, 0FFh, 0FFh, 048h, 081h, 0C6h, 0A0h, 002h, 000h
  db 000h, 048h, 0B8h, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 048h, 089h, 006h, 048h, 089h
  db 046h, 008h, 048h, 0B8h, 055h, 055h, 055h, 055h, 055h, 055h, 055h, 055h, 048h, 089h, 046h, 010h
  db 048h, 089h, 046h, 018h, 048h, 08Bh, 035h, 0D7h, 0FEh, 0FFh, 0FFh, 048h, 081h, 0C6h, 0C0h, 002h
  db 000h, 000h, 048h, 0B8h, 035h, 01Dh, 0C2h, 05Fh, 056h, 014h, 046h, 05Bh, 048h, 089h, 006h, 048h
  db 089h, 046h, 008h, 048h, 0B8h, 05Fh, 09Fh, 060h, 0D7h, 00Dh, 0CBh, 006h, 03Bh, 048h, 089h, 046h
  db 010h, 048h, 089h, 046h, 018h, 048h, 0B8h, 03Fh, 0B0h, 06Ah, 046h, 0E9h, 044h, 08Ch, 034h, 048h
  db 089h, 046h, 020h, 048h, 089h, 046h, 028h, 048h, 0B8h, 05Ah, 014h, 086h, 036h, 0A7h, 034h, 0BCh
  db 037h, 048h, 089h, 046h, 030h, 048h, 089h, 046h, 038h, 048h, 0B8h, 00Bh, 0A0h, 0CBh, 09Ah, 0B0h
  db 0C6h, 097h, 0CCh, 048h, 089h, 046h, 040h, 048h, 089h, 046h, 048h, 048h, 0B8h, 066h, 073h, 025h
  db 0BAh, 0DBh, 0D8h, 088h, 0AFh, 048h, 089h, 046h, 050h, 048h, 089h, 046h, 058h, 048h, 08Bh, 035h
  db 05Eh, 0FEh, 0FFh, 0FFh, 048h, 081h, 0C6h, 020h, 003h, 000h, 000h, 048h, 0B8h, 05Ah, 014h, 086h
  db 036h, 0A7h, 034h, 0BCh, 037h, 048h, 089h, 006h, 048h, 089h, 046h, 008h, 048h, 0B8h, 03Fh, 0B0h
  db 06Ah, 046h, 0E9h, 044h, 08Ch, 034h, 048h, 089h, 046h, 010h, 048h, 089h, 046h, 018h, 048h, 0B8h
  db 05Fh, 09Fh, 060h, 0D7h, 00Dh, 0CBh, 006h, 03Bh, 048h, 089h, 046h, 020h, 048h, 089h, 046h, 028h
  db 048h, 0B8h, 035h, 01Dh, 0C2h, 05Fh, 056h, 014h, 046h, 05Bh, 048h, 089h, 046h, 030h, 048h, 089h
  db 046h, 038h, 048h, 0B8h, 066h, 073h, 025h, 0BAh, 0DBh, 0D8h, 088h, 0AFh, 048h, 089h, 046h, 040h
  db 048h, 089h, 046h, 048h, 048h, 0B8h, 00Bh, 0A0h, 0CBh, 09Ah, 0B0h, 0C6h, 097h, 0CCh, 048h, 089h
  db 046h, 050h, 048h, 089h, 046h, 058h, 048h, 08Bh, 035h, 0E5h, 0FDh, 0FFh, 0FFh, 048h, 081h, 0C6h
  db 080h, 003h, 000h, 000h, 048h, 0B8h, 0FDh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 06Fh, 048h, 089h
  db 006h, 048h, 089h, 046h, 008h, 0BBh, 000h, 000h, 000h, 000h, 048h, 089h, 05Eh, 010h, 048h, 089h
  db 05Eh, 018h, 048h, 0B8h, 080h, 0A6h, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 048h, 089h, 046h, 020h
  db 048h, 089h, 046h, 028h, 048h, 089h, 05Eh, 030h, 048h, 089h, 05Eh, 038h, 048h, 0B8h, 040h, 053h
  db 055h, 055h, 055h, 055h, 055h, 055h, 048h, 089h, 046h, 040h, 048h, 089h, 046h, 048h, 048h, 089h
  db 05Eh, 050h, 048h, 089h, 05Eh, 058h, 048h, 08Bh, 035h, 085h, 0FDh, 0FFh, 0FFh, 048h, 081h, 0C6h
  db 0E0h, 003h, 000h, 000h, 048h, 0B8h, 040h, 053h, 055h, 055h, 055h, 055h, 055h, 055h, 048h, 089h
  db 006h, 048h, 089h, 046h, 008h, 0BBh, 000h, 000h, 000h, 000h, 048h, 089h, 05Eh, 010h, 048h, 089h
  db 05Eh, 018h, 048h, 0B8h, 080h, 0A6h, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 048h, 089h, 046h, 020h
  db 048h, 089h, 046h, 028h, 048h, 089h, 05Eh, 030h, 048h, 089h, 05Eh, 038h, 048h, 0B8h, 000h, 052h
  db 055h, 055h, 055h, 055h, 055h, 055h, 048h, 089h, 046h, 040h, 048h, 089h, 046h, 048h, 048h, 089h
  db 05Eh, 050h, 048h, 089h, 05Eh, 058h, 048h, 08Bh, 035h, 025h, 0FDh, 0FFh, 0FFh, 048h, 081h, 0C6h
  db 040h, 004h, 000h, 000h, 048h, 0B8h, 000h, 0A9h, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 048h, 089h
  db 006h, 048h, 089h, 046h, 008h, 0BBh, 000h, 000h, 000h, 000h, 048h, 089h, 05Eh, 010h, 048h, 089h
  db 05Eh, 018h, 048h, 089h, 046h, 020h, 048h, 089h, 046h, 028h, 048h, 089h, 05Eh, 030h, 048h, 089h
  db 05Eh, 038h, 048h, 0B8h, 000h, 052h, 055h, 055h, 055h, 055h, 055h, 055h, 048h, 089h, 046h, 040h
  db 048h, 089h, 046h, 048h, 048h, 089h, 05Eh, 050h, 048h, 089h, 05Eh, 058h, 0B9h, 015h, 000h, 001h
  db 0C0h, 00Fh, 032h, 025h, 0FFh, 07Fh, 0FFh, 0FFh, 00Fh, 030h, 00Fh, 020h, 0E0h, 025h, 0FFh, 0FDh
  db 0FFh, 0FFh, 00Dh, 000h, 002h, 000h, 000h, 00Fh, 022h, 0E0h, 0E8h, 02Ah, 014h, 000h, 000h, 031h
  db 0D2h, 031h, 0C9h, 048h, 031h, 0DBh, 0B8h, 001h, 000h, 000h, 000h, 00Fh, 0A2h, 048h, 0C1h, 0EBh
  db 018h, 048h, 083h, 0E3h, 001h, 049h, 089h, 0D9h, 0BAh, 064h, 000h, 000h, 000h, 048h, 08Bh, 035h
  db 08Eh, 0FCh, 0FFh, 0FFh, 048h, 081h, 0C6h, 008h, 010h, 000h, 000h, 048h, 0C1h, 0E3h, 00Ch, 048h
  db 001h, 0DEh, 048h, 0B8h, 000h, 000h, 000h, 000h, 0A0h, 0AAh, 0AAh, 02Ah, 048h, 0BBh, 0FFh, 0FFh
  db 0FFh, 05Fh, 050h, 055h, 055h, 055h, 048h, 089h, 006h, 048h, 089h, 05Eh, 008h, 048h, 083h, 0C6h
  db 010h, 0FFh, 0CAh, 075h, 0DDh, 0E8h, 0CFh, 013h, 000h, 000h, 031h, 0D2h, 031h, 0C9h, 049h, 083h
  db 0F9h, 001h, 00Fh, 084h, 067h, 002h, 000h, 000h, 049h, 089h, 0E5h, 048h, 083h, 0E4h, 0C0h, 0B8h
  db 000h, 000h, 000h, 000h, 050h, 00Fh, 0AEh, 01Ch, 024h, 00Fh, 0AEh, 054h, 024h, 004h, 050h, 0B8h
  db 008h, 052h, 000h, 000h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 048h, 0FFh, 0C8h, 075h
  db 0F3h, 058h, 048h, 08Bh, 035h, 019h, 0FCh, 0FFh, 0FFh, 048h, 081h, 0C6h, 0C0h, 002h, 000h, 000h
  db 0C5h, 07Eh, 06Fh, 00Eh, 0C5h, 07Eh, 06Fh, 056h, 020h, 0C5h, 07Eh, 06Fh, 05Eh, 040h, 048h, 08Bh
  db 035h, 0FDh, 0FBh, 0FFh, 0FFh, 048h, 081h, 0C6h, 020h, 003h, 000h, 000h, 0C5h, 07Eh, 06Fh, 026h
  db 0C5h, 07Eh, 06Fh, 06Eh, 020h, 0C5h, 07Eh, 06Fh, 076h, 040h, 048h, 08Bh, 035h, 0E1h, 0FBh, 0FFh
  db 0FFh, 048h, 081h, 0C6h, 078h, 002h, 000h, 000h, 0C5h, 0FEh, 06Fh, 006h, 0C5h, 0FEh, 06Fh, 00Eh
  db 048h, 08Bh, 035h, 0CBh, 0FBh, 0FFh, 0FFh, 048h, 081h, 0C6h, 0A0h, 002h, 000h, 000h, 048h, 08Bh
  db 03Dh, 0BDh, 0FBh, 0FFh, 0FFh, 048h, 081h, 0C7h, 0B0h, 002h, 000h, 000h, 0C5h, 0FAh, 06Fh, 006h
  db 0C5h, 0FAh, 06Fh, 017h, 0C5h, 0FAh, 06Fh, 026h, 0C5h, 0FAh, 06Fh, 037h, 0B8h, 000h, 000h, 000h
  db 000h, 048h, 08Bh, 015h, 09Ah, 0FBh, 0FFh, 0FFh, 048h, 081h, 0C2h, 030h, 002h, 000h, 000h, 048h
  db 08Bh, 01Ah, 048h, 08Bh, 015h, 089h, 0FBh, 0FFh, 0FFh, 048h, 081h, 0C2h, 038h, 002h, 000h, 000h
  db 048h, 08Bh, 00Ah, 0EBh, 00Bh, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h
  db 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h
  db 0C2h, 0B0h, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 0C4h, 043h
  db 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h
  db 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h
  db 0C2h, 0B0h, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 0C4h, 043h
  db 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 066h, 00Fh, 0E3h, 006h, 0C5h, 0F9h
  db 028h, 00Fh, 066h, 00Fh, 0E3h, 017h, 0C5h, 0F9h, 028h, 01Eh, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h
  db 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 066h, 00Fh, 0E3h, 026h, 0C5h, 0F9h, 028h, 02Fh, 066h, 00Fh
  db 0E3h, 037h, 0C5h, 0F9h, 028h, 03Eh, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h
  db 0C2h, 0B0h, 066h, 00Fh, 0E3h, 006h, 0C5h, 0F9h, 028h, 00Fh, 066h, 00Fh, 0E3h, 017h, 0C5h, 0F9h
  db 028h, 01Eh, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 066h, 00Fh
  db 0E3h, 026h, 0C5h, 0F9h, 028h, 02Fh, 066h, 00Fh, 0E3h, 037h, 0C5h, 0F9h, 028h, 03Eh, 0C4h, 043h
  db 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 066h, 00Fh, 0E3h, 006h, 0C5h, 0F9h
  db 028h, 00Fh, 066h, 00Fh, 0E3h, 017h, 0C5h, 0F9h, 028h, 01Eh, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h
  db 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 066h, 00Fh, 0E3h, 026h, 0C5h, 0F9h, 028h, 02Fh, 066h, 00Fh
  db 0E3h, 037h, 0C5h, 0F9h, 028h, 03Eh, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h
  db 0C2h, 0B0h, 066h, 00Fh, 0E3h, 006h, 0C5h, 0F9h, 028h, 00Fh, 0C4h, 043h, 035h, 069h, 0C2h, 0B0h
  db 090h, 090h, 0EBh, 015h, 050h, 0B8h, 010h, 000h, 000h, 000h, 090h, 090h, 090h, 090h, 090h, 090h
  db 048h, 0FFh, 0C8h, 075h, 0F5h, 058h, 048h, 0FFh, 0C9h, 048h, 083h, 0F9h, 030h, 074h, 0E5h, 048h
  db 083h, 0F9h, 060h, 074h, 0DFh, 048h, 081h, 0F9h, 090h, 000h, 000h, 000h, 074h, 0D6h, 048h, 0FFh
  db 0C9h, 00Fh, 085h, 00Dh, 0FFh, 0FFh, 0FFh, 048h, 0FFh, 0CBh, 074h, 037h, 048h, 08Bh, 035h, 01Fh
  db 0FAh, 0FFh, 0FFh, 048h, 081h, 0C6h, 0A0h, 002h, 000h, 000h, 048h, 08Bh, 03Dh, 011h, 0FAh, 0FFh
  db 0FFh, 048h, 081h, 0C7h, 0B0h, 002h, 000h, 000h, 0B8h, 000h, 000h, 000h, 000h, 048h, 08Bh, 015h
  db 0FEh, 0F9h, 0FFh, 0FFh, 048h, 081h, 0C2h, 040h, 002h, 000h, 000h, 048h, 08Bh, 00Ah, 0E9h, 07Dh
  db 0FEh, 0FFh, 0FFh, 00Fh, 0AEh, 014h, 024h, 04Ch, 089h, 0ECh, 0E9h, 0C5h, 001h, 000h, 000h, 041h
  db 054h, 048h, 08Bh, 015h, 0DAh, 0F9h, 0FFh, 0FFh, 048h, 081h, 0C2h, 050h, 002h, 000h, 000h, 048h
  db 08Bh, 00Ah, 048h, 08Bh, 02Dh, 0C9h, 0F9h, 0FFh, 0FFh, 048h, 081h, 0C5h, 008h, 010h, 000h, 000h
  db 048h, 0C7h, 0C0h, 090h, 0AFh, 0FAh, 0FFh, 048h, 001h, 0C5h, 048h, 08Bh, 03Dh, 0B1h, 0F9h, 0FFh
  db 0FFh, 048h, 081h, 0C7h, 008h, 020h, 000h, 000h, 048h, 0C7h, 0C0h, 090h, 0B0h, 0FAh, 0FFh, 048h
  db 001h, 0C7h, 0B8h, 03Bh, 08Bh, 009h, 062h, 0BBh, 0AAh, 0AAh, 000h, 000h, 0BAh, 032h, 076h, 0DEh
  db 002h, 041h, 0B8h, 0B7h, 037h, 01Eh, 0A4h, 049h, 0B9h, 000h, 000h, 000h, 000h, 054h, 055h, 055h
  db 001h, 041h, 0BAh, 028h, 0FFh, 073h, 038h, 041h, 0BBh, 000h, 028h, 002h, 000h, 041h, 0BCh, 020h
  db 009h, 040h, 000h, 041h, 0BDh, 0AAh, 0AAh, 000h, 000h, 049h, 0BEh, 000h, 000h, 000h, 000h, 055h
  db 055h, 055h, 000h, 041h, 0BFh, 078h, 049h, 0F6h, 0A8h, 050h, 0B8h, 008h, 052h, 000h, 000h, 090h
  db 090h, 090h, 090h, 090h, 090h, 090h, 090h, 048h, 0FFh, 0C8h, 075h, 0F3h, 058h, 0EBh, 011h, 090h
  db 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h
  db 04Dh, 001h, 0F9h, 04Dh, 001h, 0F0h, 04Eh, 08Bh, 054h, 0EDh, 020h, 04Eh, 08Bh, 05Ch, 0EDh, 028h
  db 048h, 08Bh, 044h, 0DFh, 020h, 048h, 08Bh, 054h, 0DFh, 028h, 0F7h, 0E3h, 04Dh, 001h, 0F0h, 04Dh
  db 001h, 0F9h, 04Eh, 08Bh, 054h, 0EDh, 030h, 04Eh, 08Bh, 05Ch, 0EDh, 038h, 048h, 08Bh, 044h, 0DFh
  db 030h, 048h, 08Bh, 054h, 0DFh, 038h, 04Dh, 001h, 0F9h, 04Dh, 001h, 0F0h, 04Dh, 001h, 0F0h, 04Eh
  db 08Bh, 054h, 0EDh, 040h, 04Eh, 08Bh, 05Ch, 0EDh, 048h, 048h, 08Bh, 044h, 0DFh, 040h, 048h, 08Bh
  db 054h, 0DFh, 048h, 0F7h, 0E3h, 04Dh, 001h, 0F0h, 04Dh, 001h, 0F9h, 04Eh, 08Bh, 054h, 0EDh, 050h
  db 04Eh, 08Bh, 05Ch, 0EDh, 058h, 048h, 08Bh, 044h, 0DFh, 050h, 048h, 08Bh, 054h, 0DFh, 058h, 04Dh
  db 001h, 0F9h, 04Dh, 001h, 0F0h, 04Dh, 001h, 0F0h, 04Eh, 08Bh, 054h, 0EDh, 020h, 04Eh, 08Bh, 05Ch
  db 0EDh, 028h, 048h, 08Bh, 044h, 0DFh, 020h, 048h, 08Bh, 054h, 0DFh, 028h, 04Dh, 001h, 0F0h, 0F7h
  db 0E3h, 04Dh, 001h, 0F9h, 04Eh, 08Bh, 054h, 0EDh, 030h, 04Eh, 08Bh, 05Ch, 0EDh, 038h, 048h, 08Bh
  db 044h, 0DFh, 030h, 048h, 08Bh, 054h, 0DFh, 038h, 04Dh, 001h, 0F9h, 04Dh, 001h, 0F0h, 04Dh, 001h
  db 0F0h, 04Eh, 08Bh, 054h, 0EDh, 040h, 04Eh, 08Bh, 05Ch, 0EDh, 048h, 04Ch, 08Bh, 054h, 0DFh, 040h
  db 04Ch, 08Bh, 05Ch, 0DFh, 048h, 0EBh, 015h, 050h, 0B8h, 010h, 000h, 000h, 000h, 090h, 090h, 090h
  db 090h, 090h, 090h, 048h, 0FFh, 0C8h, 075h, 0F5h, 058h, 048h, 0FFh, 0C9h, 048h, 081h, 0F9h, 080h
  db 000h, 000h, 000h, 074h, 0E2h, 048h, 081h, 0F9h, 000h, 001h, 000h, 000h, 074h, 0D9h, 048h, 081h
  db 0F9h, 040h, 001h, 000h, 000h, 074h, 0D0h, 048h, 081h, 0F9h, 080h, 001h, 000h, 000h, 074h, 0C7h
  db 048h, 081h, 0F9h, 020h, 002h, 000h, 000h, 074h, 0BEh, 048h, 0FFh, 0C9h, 00Fh, 085h, 0EEh, 0FEh
  db 0FFh, 0FFh, 041h, 05Ch, 049h, 0FFh, 0CCh, 00Fh, 085h, 05Dh, 0FBh, 0FFh, 0FFh, 0B8h, 0DEh, 0C0h
  db 001h, 0C0h, 0E9h, 0E9h, 00Fh, 000h, 000h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h
  db 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h
  db 041h, 0B9h, 001h, 000h, 000h, 000h, 048h, 031h, 0DBh, 0B8h, 001h, 000h, 000h, 000h, 00Fh, 0A2h
  db 048h, 0C1h, 0EBh, 018h, 041h, 0BCh, 032h, 000h, 000h, 000h, 048h, 083h, 0E3h, 001h, 00Fh, 085h
  db 0D6h, 002h, 000h, 000h, 041h, 0B9h, 000h, 000h, 000h, 000h, 048h, 08Bh, 035h, 0C1h, 0F7h, 0FFh
  db 0FFh, 048h, 081h, 0C6h, 030h, 002h, 000h, 000h, 041h, 0BCh, 032h, 000h, 000h, 000h, 0B8h, 004h
  db 000h, 000h, 000h, 048h, 089h, 006h, 0B8h, 020h, 000h, 000h, 000h, 048h, 089h, 046h, 008h, 0B8h
  db 0FCh, 001h, 000h, 000h, 048h, 089h, 046h, 010h, 0B8h, 010h, 000h, 000h, 000h, 048h, 089h, 046h
  db 018h, 0B8h, 0DCh, 005h, 000h, 000h, 048h, 089h, 046h, 020h, 048h, 08Bh, 035h, 081h, 0F7h, 0FFh
  db 0FFh, 048h, 081h, 0C6h, 058h, 002h, 000h, 000h, 048h, 0C7h, 0C0h, 0FFh, 0FFh, 0FFh, 0FFh, 048h
  db 089h, 006h, 048h, 089h, 046h, 008h, 048h, 089h, 046h, 010h, 048h, 089h, 046h, 018h, 0B8h, 000h
  db 000h, 000h, 000h, 048h, 089h, 046h, 020h, 048h, 089h, 046h, 028h, 048h, 089h, 046h, 030h, 048h
  db 089h, 046h, 038h, 048h, 08Bh, 035h, 048h, 0F7h, 0FFh, 0FFh, 048h, 081h, 0C6h, 0A0h, 002h, 000h
  db 000h, 048h, 0B8h, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 048h, 089h, 006h, 048h, 089h
  db 046h, 008h, 048h, 0B8h, 055h, 055h, 055h, 055h, 055h, 055h, 055h, 055h, 048h, 089h, 046h, 010h
  db 048h, 089h, 046h, 018h, 048h, 08Bh, 035h, 017h, 0F7h, 0FFh, 0FFh, 048h, 081h, 0C6h, 0C0h, 002h
  db 000h, 000h, 048h, 0B8h, 035h, 01Dh, 0C2h, 05Fh, 056h, 014h, 046h, 05Bh, 048h, 089h, 006h, 048h
  db 089h, 046h, 008h, 048h, 0B8h, 05Fh, 09Fh, 060h, 0D7h, 00Dh, 0CBh, 006h, 03Bh, 048h, 089h, 046h
  db 010h, 048h, 089h, 046h, 018h, 048h, 0B8h, 03Fh, 0B0h, 06Ah, 046h, 0E9h, 044h, 08Ch, 034h, 048h
  db 089h, 046h, 020h, 048h, 089h, 046h, 028h, 048h, 0B8h, 05Ah, 014h, 086h, 036h, 0A7h, 034h, 0BCh
  db 037h, 048h, 089h, 046h, 030h, 048h, 089h, 046h, 038h, 048h, 0B8h, 00Bh, 0A0h, 0CBh, 09Ah, 0B0h
  db 0C6h, 097h, 0CCh, 048h, 089h, 046h, 040h, 048h, 089h, 046h, 048h, 048h, 0B8h, 066h, 073h, 025h
  db 0BAh, 0DBh, 0D8h, 088h, 0AFh, 048h, 089h, 046h, 050h, 048h, 089h, 046h, 058h, 048h, 08Bh, 035h
  db 09Eh, 0F6h, 0FFh, 0FFh, 048h, 081h, 0C6h, 020h, 003h, 000h, 000h, 048h, 0B8h, 05Ah, 014h, 086h
  db 036h, 0A7h, 034h, 0BCh, 037h, 048h, 089h, 006h, 048h, 089h, 046h, 008h, 048h, 0B8h, 03Fh, 0B0h
  db 06Ah, 046h, 0E9h, 044h, 08Ch, 034h, 048h, 089h, 046h, 010h, 048h, 089h, 046h, 018h, 048h, 0B8h
  db 05Fh, 09Fh, 060h, 0D7h, 00Dh, 0CBh, 006h, 03Bh, 048h, 089h, 046h, 020h, 048h, 089h, 046h, 028h
  db 048h, 0B8h, 035h, 01Dh, 0C2h, 05Fh, 056h, 014h, 046h, 05Bh, 048h, 089h, 046h, 030h, 048h, 089h
  db 046h, 038h, 048h, 0B8h, 066h, 073h, 025h, 0BAh, 0DBh, 0D8h, 088h, 0AFh, 048h, 089h, 046h, 040h
  db 048h, 089h, 046h, 048h, 048h, 0B8h, 00Bh, 0A0h, 0CBh, 09Ah, 0B0h, 0C6h, 097h, 0CCh, 048h, 089h
  db 046h, 050h, 048h, 089h, 046h, 058h, 048h, 08Bh, 035h, 025h, 0F6h, 0FFh, 0FFh, 048h, 081h, 0C6h
  db 080h, 003h, 000h, 000h, 048h, 0B8h, 0FDh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 06Fh, 048h, 089h
  db 006h, 048h, 089h, 046h, 008h, 0BBh, 000h, 000h, 000h, 000h, 048h, 089h, 05Eh, 010h, 048h, 089h
  db 05Eh, 018h, 048h, 0B8h, 080h, 0A6h, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 048h, 089h, 046h, 020h
  db 048h, 089h, 046h, 028h, 048h, 089h, 05Eh, 030h, 048h, 089h, 05Eh, 038h, 048h, 0B8h, 040h, 053h
  db 055h, 055h, 055h, 055h, 055h, 055h, 048h, 089h, 046h, 040h, 048h, 089h, 046h, 048h, 048h, 089h
  db 05Eh, 050h, 048h, 089h, 05Eh, 058h, 048h, 08Bh, 035h, 0C5h, 0F5h, 0FFh, 0FFh, 048h, 081h, 0C6h
  db 0E0h, 003h, 000h, 000h, 048h, 0B8h, 040h, 053h, 055h, 055h, 055h, 055h, 055h, 055h, 048h, 089h
  db 006h, 048h, 089h, 046h, 008h, 0BBh, 000h, 000h, 000h, 000h, 048h, 089h, 05Eh, 010h, 048h, 089h
  db 05Eh, 018h, 048h, 0B8h, 080h, 0A6h, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 048h, 089h, 046h, 020h
  db 048h, 089h, 046h, 028h, 048h, 089h, 05Eh, 030h, 048h, 089h, 05Eh, 038h, 048h, 0B8h, 000h, 052h
  db 055h, 055h, 055h, 055h, 055h, 055h, 048h, 089h, 046h, 040h, 048h, 089h, 046h, 048h, 048h, 089h
  db 05Eh, 050h, 048h, 089h, 05Eh, 058h, 048h, 08Bh, 035h, 065h, 0F5h, 0FFh, 0FFh, 048h, 081h, 0C6h
  db 040h, 004h, 000h, 000h, 048h, 0B8h, 000h, 0A9h, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 048h, 089h
  db 006h, 048h, 089h, 046h, 008h, 0BBh, 000h, 000h, 000h, 000h, 048h, 089h, 05Eh, 010h, 048h, 089h
  db 05Eh, 018h, 048h, 089h, 046h, 020h, 048h, 089h, 046h, 028h, 048h, 089h, 05Eh, 030h, 048h, 089h
  db 05Eh, 038h, 048h, 0B8h, 000h, 052h, 055h, 055h, 055h, 055h, 055h, 055h, 048h, 089h, 046h, 040h
  db 048h, 089h, 046h, 048h, 048h, 089h, 05Eh, 050h, 048h, 089h, 05Eh, 058h, 0B9h, 015h, 000h, 001h
  db 0C0h, 00Fh, 032h, 025h, 0FFh, 07Fh, 0FFh, 0FFh, 00Fh, 030h, 00Fh, 020h, 0E0h, 025h, 0FFh, 0FDh
  db 0FFh, 0FFh, 00Dh, 000h, 002h, 000h, 000h, 00Fh, 022h, 0E0h, 0E8h, 06Ah, 00Ch, 000h, 000h, 031h
  db 0D2h, 031h, 0C9h, 048h, 031h, 0DBh, 0B8h, 001h, 000h, 000h, 000h, 00Fh, 0A2h, 048h, 0C1h, 0EBh
  db 018h, 048h, 083h, 0E3h, 001h, 049h, 089h, 0D9h, 0BAh, 040h, 000h, 000h, 000h, 048h, 08Bh, 035h
  db 0CEh, 0F4h, 0FFh, 0FFh, 048h, 081h, 0C6h, 008h, 010h, 000h, 000h, 048h, 0C1h, 0E3h, 00Ch, 048h
  db 001h, 0DEh, 048h, 0B8h, 000h, 000h, 000h, 000h, 0A0h, 0AAh, 0AAh, 02Ah, 048h, 0BBh, 0FFh, 0FFh
  db 0FFh, 05Fh, 050h, 055h, 055h, 055h, 048h, 089h, 006h, 048h, 089h, 05Eh, 008h, 048h, 083h, 0C6h
  db 010h, 0FFh, 0CAh, 075h, 0DDh, 0E8h, 00Fh, 00Ch, 000h, 000h, 031h, 0D2h, 031h, 0C9h, 049h, 083h
  db 0F9h, 001h, 00Fh, 084h, 067h, 002h, 000h, 000h, 049h, 089h, 0E5h, 048h, 083h, 0E4h, 0C0h, 0B8h
  db 000h, 000h, 000h, 000h, 050h, 00Fh, 0AEh, 01Ch, 024h, 00Fh, 0AEh, 054h, 024h, 004h, 050h, 0B8h
  db 070h, 017h, 000h, 000h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 048h, 0FFh, 0C8h, 075h
  db 0F3h, 058h, 048h, 08Bh, 035h, 059h, 0F4h, 0FFh, 0FFh, 048h, 081h, 0C6h, 0C0h, 002h, 000h, 000h
  db 0C5h, 07Eh, 06Fh, 00Eh, 0C5h, 07Eh, 06Fh, 056h, 020h, 0C5h, 07Eh, 06Fh, 05Eh, 040h, 048h, 08Bh
  db 035h, 03Dh, 0F4h, 0FFh, 0FFh, 048h, 081h, 0C6h, 020h, 003h, 000h, 000h, 0C5h, 07Eh, 06Fh, 026h
  db 0C5h, 07Eh, 06Fh, 06Eh, 020h, 0C5h, 07Eh, 06Fh, 076h, 040h, 048h, 08Bh, 035h, 021h, 0F4h, 0FFh
  db 0FFh, 048h, 081h, 0C6h, 078h, 002h, 000h, 000h, 0C5h, 0FEh, 06Fh, 006h, 0C5h, 0FEh, 06Fh, 00Eh
  db 048h, 08Bh, 035h, 00Bh, 0F4h, 0FFh, 0FFh, 048h, 081h, 0C6h, 0A0h, 002h, 000h, 000h, 048h, 08Bh
  db 03Dh, 0FDh, 0F3h, 0FFh, 0FFh, 048h, 081h, 0C7h, 0B0h, 002h, 000h, 000h, 0C5h, 0FAh, 06Fh, 006h
  db 0C5h, 0FAh, 06Fh, 017h, 0C5h, 0FAh, 06Fh, 026h, 0C5h, 0FAh, 06Fh, 037h, 0B8h, 000h, 000h, 000h
  db 000h, 048h, 08Bh, 015h, 0DAh, 0F3h, 0FFh, 0FFh, 048h, 081h, 0C2h, 030h, 002h, 000h, 000h, 048h
  db 08Bh, 01Ah, 048h, 08Bh, 015h, 0C9h, 0F3h, 0FFh, 0FFh, 048h, 081h, 0C2h, 038h, 002h, 000h, 000h
  db 048h, 08Bh, 00Ah, 0EBh, 00Bh, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h
  db 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h
  db 0C2h, 0B0h, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 0C4h, 043h
  db 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h
  db 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h
  db 0C2h, 0B0h, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 0C4h, 043h
  db 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 066h, 00Fh, 0E3h, 006h, 0C5h, 0F9h
  db 028h, 00Fh, 066h, 00Fh, 0E3h, 017h, 0C5h, 0F9h, 028h, 01Eh, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h
  db 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 066h, 00Fh, 0E3h, 026h, 0C5h, 0F9h, 028h, 02Fh, 066h, 00Fh
  db 0E3h, 037h, 0C5h, 0F9h, 028h, 03Eh, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h
  db 0C2h, 0B0h, 066h, 00Fh, 0E3h, 006h, 0C5h, 0F9h, 028h, 00Fh, 066h, 00Fh, 0E3h, 017h, 0C5h, 0F9h
  db 028h, 01Eh, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 066h, 00Fh
  db 0E3h, 026h, 0C5h, 0F9h, 028h, 02Fh, 066h, 00Fh, 0E3h, 037h, 0C5h, 0F9h, 028h, 03Eh, 0C4h, 043h
  db 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 066h, 00Fh, 0E3h, 006h, 0C5h, 0F9h
  db 028h, 00Fh, 066h, 00Fh, 0E3h, 017h, 0C5h, 0F9h, 028h, 01Eh, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h
  db 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 066h, 00Fh, 0E3h, 026h, 0C5h, 0F9h, 028h, 02Fh, 066h, 00Fh
  db 0E3h, 037h, 0C5h, 0F9h, 028h, 03Eh, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h
  db 0C2h, 0B0h, 066h, 00Fh, 0E3h, 006h, 0C5h, 0F9h, 028h, 00Fh, 0C4h, 043h, 035h, 069h, 0C2h, 0B0h
  db 090h, 090h, 0EBh, 015h, 050h, 0B8h, 010h, 000h, 000h, 000h, 090h, 090h, 090h, 090h, 090h, 090h
  db 048h, 0FFh, 0C8h, 075h, 0F5h, 058h, 048h, 0FFh, 0C9h, 048h, 083h, 0F9h, 030h, 074h, 0E5h, 048h
  db 083h, 0F9h, 060h, 074h, 0DFh, 048h, 081h, 0F9h, 090h, 000h, 000h, 000h, 074h, 0D6h, 048h, 0FFh
  db 0C9h, 00Fh, 085h, 00Dh, 0FFh, 0FFh, 0FFh, 048h, 0FFh, 0CBh, 074h, 037h, 048h, 08Bh, 035h, 05Fh
  db 0F2h, 0FFh, 0FFh, 048h, 081h, 0C6h, 0A0h, 002h, 000h, 000h, 048h, 08Bh, 03Dh, 051h, 0F2h, 0FFh
  db 0FFh, 048h, 081h, 0C7h, 0B0h, 002h, 000h, 000h, 0B8h, 000h, 000h, 000h, 000h, 048h, 08Bh, 015h
  db 03Eh, 0F2h, 0FFh, 0FFh, 048h, 081h, 0C2h, 040h, 002h, 000h, 000h, 048h, 08Bh, 00Ah, 0E9h, 07Dh
  db 0FEh, 0FFh, 0FFh, 00Fh, 0AEh, 014h, 024h, 04Ch, 089h, 0ECh, 0E9h, 0C5h, 001h, 000h, 000h, 041h
  db 054h, 048h, 08Bh, 015h, 01Ah, 0F2h, 0FFh, 0FFh, 048h, 081h, 0C2h, 050h, 002h, 000h, 000h, 048h
  db 08Bh, 00Ah, 048h, 08Bh, 02Dh, 009h, 0F2h, 0FFh, 0FFh, 048h, 081h, 0C5h, 008h, 010h, 000h, 000h
  db 048h, 0C7h, 0C0h, 090h, 0AFh, 0FAh, 0FFh, 048h, 001h, 0C5h, 048h, 08Bh, 03Dh, 0F1h, 0F1h, 0FFh
  db 0FFh, 048h, 081h, 0C7h, 008h, 020h, 000h, 000h, 048h, 0C7h, 0C0h, 090h, 0B0h, 0FAh, 0FFh, 048h
  db 001h, 0C7h, 0B8h, 03Bh, 08Bh, 009h, 062h, 0BBh, 0AAh, 0AAh, 000h, 000h, 0BAh, 032h, 076h, 0DEh
  db 002h, 041h, 0B8h, 0B7h, 037h, 01Eh, 0A4h, 049h, 0B9h, 000h, 000h, 000h, 000h, 054h, 055h, 055h
  db 001h, 041h, 0BAh, 028h, 0FFh, 073h, 038h, 041h, 0BBh, 000h, 028h, 002h, 000h, 041h, 0BCh, 020h
  db 009h, 040h, 000h, 041h, 0BDh, 0AAh, 0AAh, 000h, 000h, 049h, 0BEh, 000h, 000h, 000h, 000h, 055h
  db 055h, 055h, 000h, 041h, 0BFh, 078h, 049h, 0F6h, 0A8h, 050h, 0B8h, 070h, 017h, 000h, 000h, 090h
  db 090h, 090h, 090h, 090h, 090h, 090h, 090h, 048h, 0FFh, 0C8h, 075h, 0F3h, 058h, 0EBh, 011h, 090h
  db 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h
  db 04Dh, 001h, 0F9h, 04Dh, 001h, 0F0h, 04Eh, 08Bh, 054h, 0EDh, 020h, 04Eh, 08Bh, 05Ch, 0EDh, 028h
  db 048h, 08Bh, 044h, 0DFh, 020h, 048h, 08Bh, 054h, 0DFh, 028h, 0F7h, 0E3h, 04Dh, 001h, 0F0h, 04Dh
  db 001h, 0F9h, 04Eh, 08Bh, 054h, 0EDh, 030h, 04Eh, 08Bh, 05Ch, 0EDh, 038h, 048h, 08Bh, 044h, 0DFh
  db 030h, 048h, 08Bh, 054h, 0DFh, 038h, 04Dh, 001h, 0F9h, 04Dh, 001h, 0F0h, 04Dh, 001h, 0F0h, 04Eh
  db 08Bh, 054h, 0EDh, 040h, 04Eh, 08Bh, 05Ch, 0EDh, 048h, 048h, 08Bh, 044h, 0DFh, 040h, 048h, 08Bh
  db 054h, 0DFh, 048h, 0F7h, 0E3h, 04Dh, 001h, 0F0h, 04Dh, 001h, 0F9h, 04Eh, 08Bh, 054h, 0EDh, 050h
  db 04Eh, 08Bh, 05Ch, 0EDh, 058h, 048h, 08Bh, 044h, 0DFh, 050h, 048h, 08Bh, 054h, 0DFh, 058h, 04Dh
  db 001h, 0F9h, 04Dh, 001h, 0F0h, 04Dh, 001h, 0F0h, 04Eh, 08Bh, 054h, 0EDh, 020h, 04Eh, 08Bh, 05Ch
  db 0EDh, 028h, 048h, 08Bh, 044h, 0DFh, 020h, 048h, 08Bh, 054h, 0DFh, 028h, 04Dh, 001h, 0F0h, 0F7h
  db 0E3h, 04Dh, 001h, 0F9h, 04Eh, 08Bh, 054h, 0EDh, 030h, 04Eh, 08Bh, 05Ch, 0EDh, 038h, 048h, 08Bh
  db 044h, 0DFh, 030h, 048h, 08Bh, 054h, 0DFh, 038h, 04Dh, 001h, 0F9h, 04Dh, 001h, 0F0h, 04Dh, 001h
  db 0F0h, 04Eh, 08Bh, 054h, 0EDh, 040h, 04Eh, 08Bh, 05Ch, 0EDh, 048h, 04Ch, 08Bh, 054h, 0DFh, 040h
  db 04Ch, 08Bh, 05Ch, 0DFh, 048h, 0EBh, 015h, 050h, 0B8h, 010h, 000h, 000h, 000h, 090h, 090h, 090h
  db 090h, 090h, 090h, 048h, 0FFh, 0C8h, 075h, 0F5h, 058h, 048h, 0FFh, 0C9h, 048h, 081h, 0F9h, 080h
  db 000h, 000h, 000h, 074h, 0E2h, 048h, 081h, 0F9h, 000h, 001h, 000h, 000h, 074h, 0D9h, 048h, 081h
  db 0F9h, 040h, 001h, 000h, 000h, 074h, 0D0h, 048h, 081h, 0F9h, 080h, 001h, 000h, 000h, 074h, 0C7h
  db 048h, 081h, 0F9h, 020h, 002h, 000h, 000h, 074h, 0BEh, 048h, 0FFh, 0C9h, 00Fh, 085h, 0EEh, 0FEh
  db 0FFh, 0FFh, 041h, 05Ch, 049h, 0FFh, 0CCh, 00Fh, 085h, 05Dh, 0FBh, 0FFh, 0FFh, 0E8h, 0C7h, 007h
  db 000h, 000h, 0B8h, 0DEh, 0C0h, 001h, 0C0h, 0E9h, 024h, 008h, 000h, 000h, 090h, 090h, 090h, 090h
  db 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h
  db 041h, 0B9h, 001h, 000h, 000h, 000h, 048h, 031h, 0DBh, 0B8h, 001h, 000h, 000h, 000h, 00Fh, 0A2h
  db 048h, 0C1h, 0EBh, 018h, 041h, 0BCh, 032h, 000h, 000h, 000h, 048h, 083h, 0E3h, 001h, 00Fh, 085h
  db 0D6h, 002h, 000h, 000h, 041h, 0B9h, 000h, 000h, 000h, 000h, 048h, 08Bh, 035h, 001h, 0F0h, 0FFh
  db 0FFh, 048h, 081h, 0C6h, 030h, 002h, 000h, 000h, 041h, 0BCh, 032h, 000h, 000h, 000h, 0B8h, 004h
  db 000h, 000h, 000h, 048h, 089h, 006h, 0B8h, 020h, 000h, 000h, 000h, 048h, 089h, 046h, 008h, 0B8h
  db 031h, 001h, 000h, 000h, 048h, 089h, 046h, 010h, 0B8h, 010h, 000h, 000h, 000h, 048h, 089h, 046h
  db 018h, 0B8h, 084h, 003h, 000h, 000h, 048h, 089h, 046h, 020h, 048h, 08Bh, 035h, 0C1h, 0EFh, 0FFh
  db 0FFh, 048h, 081h, 0C6h, 058h, 002h, 000h, 000h, 048h, 0C7h, 0C0h, 0FFh, 0FFh, 0FFh, 0FFh, 048h
  db 089h, 006h, 048h, 089h, 046h, 008h, 048h, 089h, 046h, 010h, 048h, 089h, 046h, 018h, 0B8h, 000h
  db 000h, 000h, 000h, 048h, 089h, 046h, 020h, 048h, 089h, 046h, 028h, 048h, 089h, 046h, 030h, 048h
  db 089h, 046h, 038h, 048h, 08Bh, 035h, 088h, 0EFh, 0FFh, 0FFh, 048h, 081h, 0C6h, 0A0h, 002h, 000h
  db 000h, 048h, 0B8h, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 048h, 089h, 006h, 048h, 089h
  db 046h, 008h, 048h, 0B8h, 055h, 055h, 055h, 055h, 055h, 055h, 055h, 055h, 048h, 089h, 046h, 010h
  db 048h, 089h, 046h, 018h, 048h, 08Bh, 035h, 057h, 0EFh, 0FFh, 0FFh, 048h, 081h, 0C6h, 0C0h, 002h
  db 000h, 000h, 048h, 0B8h, 035h, 01Dh, 0C2h, 05Fh, 056h, 014h, 046h, 05Bh, 048h, 089h, 006h, 048h
  db 089h, 046h, 008h, 048h, 0B8h, 05Fh, 09Fh, 060h, 0D7h, 00Dh, 0CBh, 006h, 03Bh, 048h, 089h, 046h
  db 010h, 048h, 089h, 046h, 018h, 048h, 0B8h, 03Fh, 0B0h, 06Ah, 046h, 0E9h, 044h, 08Ch, 034h, 048h
  db 089h, 046h, 020h, 048h, 089h, 046h, 028h, 048h, 0B8h, 05Ah, 014h, 086h, 036h, 0A7h, 034h, 0BCh
  db 037h, 048h, 089h, 046h, 030h, 048h, 089h, 046h, 038h, 048h, 0B8h, 00Bh, 0A0h, 0CBh, 09Ah, 0B0h
  db 0C6h, 097h, 0CCh, 048h, 089h, 046h, 040h, 048h, 089h, 046h, 048h, 048h, 0B8h, 066h, 073h, 025h
  db 0BAh, 0DBh, 0D8h, 088h, 0AFh, 048h, 089h, 046h, 050h, 048h, 089h, 046h, 058h, 048h, 08Bh, 035h
  db 0DEh, 0EEh, 0FFh, 0FFh, 048h, 081h, 0C6h, 020h, 003h, 000h, 000h, 048h, 0B8h, 05Ah, 014h, 086h
  db 036h, 0A7h, 034h, 0BCh, 037h, 048h, 089h, 006h, 048h, 089h, 046h, 008h, 048h, 0B8h, 03Fh, 0B0h
  db 06Ah, 046h, 0E9h, 044h, 08Ch, 034h, 048h, 089h, 046h, 010h, 048h, 089h, 046h, 018h, 048h, 0B8h
  db 05Fh, 09Fh, 060h, 0D7h, 00Dh, 0CBh, 006h, 03Bh, 048h, 089h, 046h, 020h, 048h, 089h, 046h, 028h
  db 048h, 0B8h, 035h, 01Dh, 0C2h, 05Fh, 056h, 014h, 046h, 05Bh, 048h, 089h, 046h, 030h, 048h, 089h
  db 046h, 038h, 048h, 0B8h, 066h, 073h, 025h, 0BAh, 0DBh, 0D8h, 088h, 0AFh, 048h, 089h, 046h, 040h
  db 048h, 089h, 046h, 048h, 048h, 0B8h, 00Bh, 0A0h, 0CBh, 09Ah, 0B0h, 0C6h, 097h, 0CCh, 048h, 089h
  db 046h, 050h, 048h, 089h, 046h, 058h, 048h, 08Bh, 035h, 065h, 0EEh, 0FFh, 0FFh, 048h, 081h, 0C6h
  db 080h, 003h, 000h, 000h, 048h, 0B8h, 0FDh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 06Fh, 048h, 089h
  db 006h, 048h, 089h, 046h, 008h, 0BBh, 000h, 000h, 000h, 000h, 048h, 089h, 05Eh, 010h, 048h, 089h
  db 05Eh, 018h, 048h, 0B8h, 080h, 0A6h, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 048h, 089h, 046h, 020h
  db 048h, 089h, 046h, 028h, 048h, 089h, 05Eh, 030h, 048h, 089h, 05Eh, 038h, 048h, 0B8h, 040h, 053h
  db 055h, 055h, 055h, 055h, 055h, 055h, 048h, 089h, 046h, 040h, 048h, 089h, 046h, 048h, 048h, 089h
  db 05Eh, 050h, 048h, 089h, 05Eh, 058h, 048h, 08Bh, 035h, 005h, 0EEh, 0FFh, 0FFh, 048h, 081h, 0C6h
  db 0E0h, 003h, 000h, 000h, 048h, 0B8h, 040h, 053h, 055h, 055h, 055h, 055h, 055h, 055h, 048h, 089h
  db 006h, 048h, 089h, 046h, 008h, 0BBh, 000h, 000h, 000h, 000h, 048h, 089h, 05Eh, 010h, 048h, 089h
  db 05Eh, 018h, 048h, 0B8h, 080h, 0A6h, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 048h, 089h, 046h, 020h
  db 048h, 089h, 046h, 028h, 048h, 089h, 05Eh, 030h, 048h, 089h, 05Eh, 038h, 048h, 0B8h, 000h, 052h
  db 055h, 055h, 055h, 055h, 055h, 055h, 048h, 089h, 046h, 040h, 048h, 089h, 046h, 048h, 048h, 089h
  db 05Eh, 050h, 048h, 089h, 05Eh, 058h, 048h, 08Bh, 035h, 0A5h, 0EDh, 0FFh, 0FFh, 048h, 081h, 0C6h
  db 040h, 004h, 000h, 000h, 048h, 0B8h, 000h, 0A9h, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 0AAh, 048h, 089h
  db 006h, 048h, 089h, 046h, 008h, 0BBh, 000h, 000h, 000h, 000h, 048h, 089h, 05Eh, 010h, 048h, 089h
  db 05Eh, 018h, 048h, 089h, 046h, 020h, 048h, 089h, 046h, 028h, 048h, 089h, 05Eh, 030h, 048h, 089h
  db 05Eh, 038h, 048h, 0B8h, 000h, 052h, 055h, 055h, 055h, 055h, 055h, 055h, 048h, 089h, 046h, 040h
  db 048h, 089h, 046h, 048h, 048h, 089h, 05Eh, 050h, 048h, 089h, 05Eh, 058h, 0B9h, 015h, 000h, 001h
  db 0C0h, 00Fh, 032h, 025h, 0FFh, 07Fh, 0FFh, 0FFh, 00Fh, 030h, 00Fh, 020h, 0E0h, 025h, 0FFh, 0FDh
  db 0FFh, 0FFh, 00Dh, 000h, 002h, 000h, 000h, 00Fh, 022h, 0E0h, 0E8h, 0AAh, 004h, 000h, 000h, 031h
  db 0D2h, 031h, 0C9h, 048h, 031h, 0DBh, 0B8h, 001h, 000h, 000h, 000h, 00Fh, 0A2h, 048h, 0C1h, 0EBh
  db 018h, 048h, 083h, 0E3h, 001h, 049h, 089h, 0D9h, 0BAh, 040h, 000h, 000h, 000h, 048h, 08Bh, 035h
  db 00Eh, 0EDh, 0FFh, 0FFh, 048h, 081h, 0C6h, 008h, 010h, 000h, 000h, 048h, 0C1h, 0E3h, 00Ch, 048h
  db 001h, 0DEh, 048h, 0B8h, 000h, 000h, 000h, 000h, 0A0h, 0AAh, 0AAh, 02Ah, 048h, 0BBh, 0FFh, 0FFh
  db 0FFh, 05Fh, 050h, 055h, 055h, 055h, 048h, 089h, 006h, 048h, 089h, 05Eh, 008h, 048h, 083h, 0C6h
  db 010h, 0FFh, 0CAh, 075h, 0DDh, 0E8h, 04Fh, 004h, 000h, 000h, 031h, 0D2h, 031h, 0C9h, 049h, 083h
  db 0F9h, 001h, 00Fh, 084h, 067h, 002h, 000h, 000h, 049h, 089h, 0E5h, 048h, 083h, 0E4h, 0C0h, 0B8h
  db 000h, 000h, 000h, 000h, 050h, 00Fh, 0AEh, 01Ch, 024h, 00Fh, 0AEh, 054h, 024h, 004h, 050h, 0B8h
  db 001h, 000h, 000h, 000h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 048h, 0FFh, 0C8h, 075h
  db 0F3h, 058h, 048h, 08Bh, 035h, 099h, 0ECh, 0FFh, 0FFh, 048h, 081h, 0C6h, 0C0h, 002h, 000h, 000h
  db 0C5h, 07Eh, 06Fh, 00Eh, 0C5h, 07Eh, 06Fh, 056h, 020h, 0C5h, 07Eh, 06Fh, 05Eh, 040h, 048h, 08Bh
  db 035h, 07Dh, 0ECh, 0FFh, 0FFh, 048h, 081h, 0C6h, 020h, 003h, 000h, 000h, 0C5h, 07Eh, 06Fh, 026h
  db 0C5h, 07Eh, 06Fh, 06Eh, 020h, 0C5h, 07Eh, 06Fh, 076h, 040h, 048h, 08Bh, 035h, 061h, 0ECh, 0FFh
  db 0FFh, 048h, 081h, 0C6h, 078h, 002h, 000h, 000h, 0C5h, 0FEh, 06Fh, 006h, 0C5h, 0FEh, 06Fh, 00Eh
  db 048h, 08Bh, 035h, 04Bh, 0ECh, 0FFh, 0FFh, 048h, 081h, 0C6h, 0A0h, 002h, 000h, 000h, 048h, 08Bh
  db 03Dh, 03Dh, 0ECh, 0FFh, 0FFh, 048h, 081h, 0C7h, 0B0h, 002h, 000h, 000h, 0C5h, 0FAh, 06Fh, 006h
  db 0C5h, 0FAh, 06Fh, 017h, 0C5h, 0FAh, 06Fh, 026h, 0C5h, 0FAh, 06Fh, 037h, 0B8h, 000h, 000h, 000h
  db 000h, 048h, 08Bh, 015h, 01Ah, 0ECh, 0FFh, 0FFh, 048h, 081h, 0C2h, 030h, 002h, 000h, 000h, 048h
  db 08Bh, 01Ah, 048h, 08Bh, 015h, 009h, 0ECh, 0FFh, 0FFh, 048h, 081h, 0C2h, 038h, 002h, 000h, 000h
  db 048h, 08Bh, 00Ah, 0EBh, 00Bh, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h
  db 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h
  db 0C2h, 0B0h, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 0C4h, 043h
  db 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h
  db 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h
  db 0C2h, 0B0h, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 0C4h, 043h
  db 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 066h, 00Fh, 0E3h, 006h, 0C5h, 0F9h
  db 028h, 00Fh, 066h, 00Fh, 0E3h, 017h, 0C5h, 0F9h, 028h, 01Eh, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h
  db 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 066h, 00Fh, 0E3h, 026h, 0C5h, 0F9h, 028h, 02Fh, 066h, 00Fh
  db 0E3h, 037h, 0C5h, 0F9h, 028h, 03Eh, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h
  db 0C2h, 0B0h, 066h, 00Fh, 0E3h, 006h, 0C5h, 0F9h, 028h, 00Fh, 066h, 00Fh, 0E3h, 017h, 0C5h, 0F9h
  db 028h, 01Eh, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 066h, 00Fh
  db 0E3h, 026h, 0C5h, 0F9h, 028h, 02Fh, 066h, 00Fh, 0E3h, 037h, 0C5h, 0F9h, 028h, 03Eh, 0C4h, 043h
  db 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 066h, 00Fh, 0E3h, 006h, 0C5h, 0F9h
  db 028h, 00Fh, 066h, 00Fh, 0E3h, 017h, 0C5h, 0F9h, 028h, 01Eh, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h
  db 0C4h, 043h, 035h, 069h, 0C2h, 0B0h, 066h, 00Fh, 0E3h, 026h, 0C5h, 0F9h, 028h, 02Fh, 066h, 00Fh
  db 0E3h, 037h, 0C5h, 0F9h, 028h, 03Eh, 0C4h, 043h, 01Dh, 069h, 0C5h, 0E0h, 0C4h, 043h, 035h, 069h
  db 0C2h, 0B0h, 066h, 00Fh, 0E3h, 006h, 0C5h, 0F9h, 028h, 00Fh, 0C4h, 043h, 035h, 069h, 0C2h, 0B0h
  db 090h, 090h, 0EBh, 015h, 050h, 0B8h, 010h, 000h, 000h, 000h, 090h, 090h, 090h, 090h, 090h, 090h
  db 048h, 0FFh, 0C8h, 075h, 0F5h, 058h, 048h, 0FFh, 0C9h, 048h, 083h, 0F9h, 030h, 074h, 0E5h, 048h
  db 083h, 0F9h, 060h, 074h, 0DFh, 048h, 081h, 0F9h, 090h, 000h, 000h, 000h, 074h, 0D6h, 048h, 0FFh
  db 0C9h, 00Fh, 085h, 00Dh, 0FFh, 0FFh, 0FFh, 048h, 0FFh, 0CBh, 074h, 037h, 048h, 08Bh, 035h, 09Fh
  db 0EAh, 0FFh, 0FFh, 048h, 081h, 0C6h, 0A0h, 002h, 000h, 000h, 048h, 08Bh, 03Dh, 091h, 0EAh, 0FFh
  db 0FFh, 048h, 081h, 0C7h, 0B0h, 002h, 000h, 000h, 0B8h, 000h, 000h, 000h, 000h, 048h, 08Bh, 015h
  db 07Eh, 0EAh, 0FFh, 0FFh, 048h, 081h, 0C2h, 040h, 002h, 000h, 000h, 048h, 08Bh, 00Ah, 0E9h, 07Dh
  db 0FEh, 0FFh, 0FFh, 00Fh, 0AEh, 014h, 024h, 04Ch, 089h, 0ECh, 0E9h, 0C5h, 001h, 000h, 000h, 041h
  db 054h, 048h, 08Bh, 015h, 05Ah, 0EAh, 0FFh, 0FFh, 048h, 081h, 0C2h, 050h, 002h, 000h, 000h, 048h
  db 08Bh, 00Ah, 048h, 08Bh, 02Dh, 049h, 0EAh, 0FFh, 0FFh, 048h, 081h, 0C5h, 008h, 010h, 000h, 000h
  db 048h, 0C7h, 0C0h, 090h, 0AFh, 0FAh, 0FFh, 048h, 001h, 0C5h, 048h, 08Bh, 03Dh, 031h, 0EAh, 0FFh
  db 0FFh, 048h, 081h, 0C7h, 008h, 020h, 000h, 000h, 048h, 0C7h, 0C0h, 090h, 0B0h, 0FAh, 0FFh, 048h
  db 001h, 0C7h, 0B8h, 03Bh, 08Bh, 009h, 062h, 0BBh, 0AAh, 0AAh, 000h, 000h, 0BAh, 032h, 076h, 0DEh
  db 002h, 041h, 0B8h, 0B7h, 037h, 01Eh, 0A4h, 049h, 0B9h, 000h, 000h, 000h, 000h, 054h, 055h, 055h
  db 001h, 041h, 0BAh, 028h, 0FFh, 073h, 038h, 041h, 0BBh, 000h, 028h, 002h, 000h, 041h, 0BCh, 020h
  db 009h, 040h, 000h, 041h, 0BDh, 0AAh, 0AAh, 000h, 000h, 049h, 0BEh, 000h, 000h, 000h, 000h, 055h
  db 055h, 055h, 000h, 041h, 0BFh, 078h, 049h, 0F6h, 0A8h, 050h, 0B8h, 001h, 000h, 000h, 000h, 090h
  db 090h, 090h, 090h, 090h, 090h, 090h, 090h, 048h, 0FFh, 0C8h, 075h, 0F3h, 058h, 0EBh, 011h, 090h
  db 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h, 090h
  db 04Dh, 001h, 0F9h, 04Dh, 001h, 0F0h, 04Eh, 08Bh, 054h, 0EDh, 020h, 04Eh, 08Bh, 05Ch, 0EDh, 028h
  db 048h, 08Bh, 044h, 0DFh, 020h, 048h, 08Bh, 054h, 0DFh, 028h, 0F7h, 0E3h, 04Dh, 001h, 0F0h, 04Dh
  db 001h, 0F9h, 04Eh, 08Bh, 054h, 0EDh, 030h, 04Eh, 08Bh, 05Ch, 0EDh, 038h, 048h, 08Bh, 044h, 0DFh
  db 030h, 048h, 08Bh, 054h, 0DFh, 038h, 04Dh, 001h, 0F9h, 04Dh, 001h, 0F0h, 04Dh, 001h, 0F0h, 04Eh
  db 08Bh, 054h, 0EDh, 040h, 04Eh, 08Bh, 05Ch, 0EDh, 048h, 048h, 08Bh, 044h, 0DFh, 040h, 048h, 08Bh
  db 054h, 0DFh, 048h, 0F7h, 0E3h, 04Dh, 001h, 0F0h, 04Dh, 001h, 0F9h, 04Eh, 08Bh, 054h, 0EDh, 050h
  db 04Eh, 08Bh, 05Ch, 0EDh, 058h, 048h, 08Bh, 044h, 0DFh, 050h, 048h, 08Bh, 054h, 0DFh, 058h, 04Dh
  db 001h, 0F9h, 04Dh, 001h, 0F0h, 04Dh, 001h, 0F0h, 04Eh, 08Bh, 054h, 0EDh, 020h, 04Eh, 08Bh, 05Ch
  db 0EDh, 028h, 048h, 08Bh, 044h, 0DFh, 020h, 048h, 08Bh, 054h, 0DFh, 028h, 04Dh, 001h, 0F0h, 0F7h
  db 0E3h, 04Dh, 001h, 0F9h, 04Eh, 08Bh, 054h, 0EDh, 030h, 04Eh, 08Bh, 05Ch, 0EDh, 038h, 048h, 08Bh
  db 044h, 0DFh, 030h, 048h, 08Bh, 054h, 0DFh, 038h, 04Dh, 001h, 0F9h, 04Dh, 001h, 0F0h, 04Dh, 001h
  db 0F0h, 04Eh, 08Bh, 054h, 0EDh, 040h, 04Eh, 08Bh, 05Ch, 0EDh, 048h, 04Ch, 08Bh, 054h, 0DFh, 040h
  db 04Ch, 08Bh, 05Ch, 0DFh, 048h, 0EBh, 015h, 050h, 0B8h, 010h, 000h, 000h, 000h, 090h, 090h, 090h
  db 090h, 090h, 090h, 048h, 0FFh, 0C8h, 075h, 0F5h, 058h, 048h, 0FFh, 0C9h, 048h, 081h, 0F9h, 080h
  db 000h, 000h, 000h, 074h, 0E2h, 048h, 081h, 0F9h, 000h, 001h, 000h, 000h, 074h, 0D9h, 048h, 081h
  db 0F9h, 040h, 001h, 000h, 000h, 074h, 0D0h, 048h, 081h, 0F9h, 080h, 001h, 000h, 000h, 074h, 0C7h
  db 048h, 081h, 0F9h, 020h, 002h, 000h, 000h, 074h, 0BEh, 048h, 0FFh, 0C9h, 00Fh, 085h, 0EEh, 0FEh
  db 0FFh, 0FFh, 041h, 05Ch, 049h, 0FFh, 0CCh, 00Fh, 085h, 05Dh, 0FBh, 0FFh, 0FFh, 0E8h, 007h, 000h
  db 000h, 000h, 0B8h, 0DEh, 0C0h, 001h, 0C0h, 0EBh, 067h, 09Ch, 050h, 053h, 051h, 052h, 048h, 031h
  db 0DBh, 0B8h, 001h, 000h, 000h, 000h, 00Fh, 0A2h, 048h, 0C1h, 0EBh, 018h, 048h, 083h, 0E3h, 001h
  db 075h, 025h, 0BAh, 0DEh, 0C0h, 001h, 0C0h, 031h, 0C0h, 0B9h, 043h, 010h, 001h, 0C0h, 00Fh, 030h
  db 0B9h, 044h, 010h, 001h, 0C0h, 00Fh, 032h, 081h, 0FAh, 0DEh, 0C0h, 001h, 0C0h, 075h, 0F6h, 031h
  db 0C0h, 031h, 0D2h, 00Fh, 030h, 0EBh, 023h, 0BAh, 0DEh, 0C0h, 001h, 0C0h, 031h, 0C0h, 0B9h, 044h
  db 010h, 001h, 0C0h, 00Fh, 030h, 0B9h, 043h, 010h, 001h, 0C0h, 00Fh, 032h, 081h, 0FAh, 0DEh, 0C0h
  db 001h, 0C0h, 075h, 0F6h, 031h, 0C0h, 031h, 0D2h, 00Fh, 030h, 05Ah, 059h, 05Bh, 058h, 09Dh, 0C3h
  db 0B8h, 0DEh, 0C0h, 001h, 0C0h, 089h, 0C3h, 0C3h
