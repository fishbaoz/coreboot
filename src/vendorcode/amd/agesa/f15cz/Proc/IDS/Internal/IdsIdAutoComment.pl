if (@ARGV == 0) {
	$AGESA_ROOT= "..\\..\\..\\";
} else {
	$AGESA_ROOT= $ARGV[0];
}
if (-e $AGESA_ROOT."amd.h"){
	print "AGESA Root is valid\n"
} else {
	print "ERROR: Could not find file Amd.h at path: [".$AGESA_ROOT."]\n\n";
	print "2 ways to solve it:\n";
	print "\t(1)  Put script to proc\\ids\\ under AGESA root\n";
	print "\t(2)  Add AGESA root as script 1st parameter\n";
	print "\te.g. Perl IdsIdAutoComment.pl [AGESA_ROOT]\n";
	print "\tDon't forget \"\\\"trailing of AGESA_ROOT\n";
	exit;
}
	
$AGESAH_INCLUDE_PATH= $AGESA_ROOT."Agesa.h";
$AGESAINC_INCLUDE_PATH= $AGESA_ROOT."legacy\\agesa.inc";
$IDSINTH_INCLUDE_PATH= $AGESA_ROOT."Proc\\IDS\\Internal\\IdsInternal.h";
$IDSINTINC_INCLUDE_PATH= $AGESA_ROOT."Proc\\IDS\\Internal\\IdsIntnl.inc";


open AGESAH, "<", $AGESAH_INCLUDE_PATH
	or die "Can't open Agesa.h.\n";
open AGESAINC, "<", $AGESAINC_INCLUDE_PATH
	or die "Can't open Agesa.inc.\n";



	
#Update Agesa.h
	@agesah = <AGESAH>;
	@agesainc = <AGESAINC>;

	$IdsPatternFound = 0;
	$i = 1;
	foreach (@agesah) {
		#direct wrtie to new array until we found signature/// AGESA IDS NV ID Definitions
		if (/^\/\/\/ AGESA IDS NV ID Definitions$/) {
			$IdsPatternFound = 1;			
		} elsif (/^} IDS_EX_NV_ID;$/) {
			$IdsPatternFound = 0;
		}

		if ($IdsPatternFound == 1) {
			if(/^\s+(.+?),\s+\/\/\/<\s*0x([A-Fa-f0-9]+?)\s+(.*)$/) {
				push @newagesah, sprintf "  %-50s///< 0x%04X %95s\n",$1.",",$i,$3;
				push @newagesainc, sprintf "  %-50sEQU    %04Xh; <%95s\n",$1,$i++,$3;
				
			} elsif (/^\s+AGESA_IDS_EXT_ID_START.*$/) {
			#  AGESA_IDS_EXT_ID_START                   = 0x0000,///< 0x0000                                    specify the start of external NV id
				push @newagesainc,
				sprintf "  %-50sEQU    %04Xh; <%95s\n","AGESA_IDS_EXT_ID_START",0,"specify the start of external NV id";
				push @newagesah,
				sprintf "  AGESA_IDS_EXT_ID_START%27s,///< 0x%04X %95s\n","= 0x0000",0,"specify the start of external NV id";;
			} else {
				push @newagesah, $_;
				if (/^\s*$/) {
					push @newagesainc, $_;
				}
			}
		} else {
			push @newagesah, $_;
		}
	}
	
	$CurrentId = $i;
	close ($AGESAH);
	open AGESAH, ">", $AGESAH_INCLUDE_PATH
	or die "Can't open Agesa.h.\n";
	foreach (@newagesah) {
		print AGESAH $_;
	}
	printf "%-30supdated     [ok]\n","Agesa.h";
#Update agesa.inc
	$i = 0;
	foreach $line(@agesainc) {
		if ($line =~ /^\s*; AGESA IDS NV ID Definitions$/) {
			$IdsPatternFound = 1;
			push @modifyagesainc, $line;
			next;
		} elsif ($line =~ /^\s*AGESA_IDS_EXT_ID_END.*$/) {
			$IdsPatternFound = 0;
			next;
		}
		if ($IdsPatternFound == 1) {
			
		} else {
			push @modifyagesainc, $line;
		}
	}

	$IdsPatternFound = 0;
	while ($i < @modifyagesainc) {
		$_ =  $modifyagesainc[$i++];
		if (/^\s*; AGESA IDS NV ID Definitions$/) {
			$IdsPatternFound = 1;
			last;
		}
	}
	$k = $i;
	$lenmodifyagesainc = scalar @modifyagesainc;
	for ($j = 0; $j < ($lenmodifyagesainc - $i+1); $j++, $i++) {
		$modifyagesainc [$i + @newagesainc +$j]  = $modifyagesainc [$i];
	}

	for ($j = 0; $j < @newagesainc; $j++, $k++) {
		$modifyagesainc [$k] = $newagesainc[$j];
	}
	close (AGESAINC);
	open AGESAINC, ">", $AGESAINC_INCLUDE_PATH
	or die "Can't open Agesa.inc.\n";
	foreach (@modifyagesainc) {
		print AGESAINC $_;
	}
	
	printf "%-30supdated     [ok]\n","agesa.inc";

#Update IdsInternal.h
open IDSINTH, "<", $IDSINTH_INCLUDE_PATH
	or die "Can't open IdsInternal.h.\n";
	@idsinth = <IDSINTH>;
	$IdsPatternFound = 0;
	$i = $CurrentId;
	foreach (@idsinth) {
		#direct wrtie to new array until we found signature/// AGESA IDS NV ID Definitions
		if (/^\/\/\/\s*AGESA IDS Internal NV ID Definitions$/) {
			$IdsPatternFound = 1;			
		} elsif (/^} IDS_INT_NV_ID;$/) {
			$IdsPatternFound = 0;
		}

		if ($IdsPatternFound == 1) {
			if(/^\s+(.+?),\s+\/\/\/<\s*0x([A-Fa-f0-9]+?)\s+(.*)$/) {
				push @newidsinth, sprintf "  %-60s///< 0x%04X %95s\n",$1.",",$i,$3;
				push @newidsintinc, sprintf "  %-60sEQU    %04Xh; <%95s\n",$1,$i++,$3;
				
			} elsif (/^\s+AGESA_IDS_INT_ID_START\s+=\s+AGESA_IDS_EXT_ID_END\s*\+\s*1,\/\/\/\<\s*0x([A-Fa-f0-9]+?)\s+(.*)$/) {
#  AGESA_IDS_INT_ID_START =   AGESA_IDS_EXT_ID_END+1,///< 0x001D                                                             specify the start of internal NV id
				push @newidsintinc,
				sprintf "  %-50sEQU    %04Xh; <%95s\n",AGESA_IDS_INT_ID_START,$i,$2;
				push @newidsinth,
				sprintf  "  %-50s///< 0x%04X %95s\n","AGESA_IDS_INT_ID_START = AGESA_IDS_EXT_ID_END + 1,",$i++,$2;
			} else {
				push @newidsinth, $_;
				if (/^\s*$/) {
					push @newidsintinc, $_;
				}
			}
		} else {
			push @newidsinth, $_;
		}
	}

	close (IDSINTH);
	open IDSINTH, ">", $IDSINTH_INCLUDE_PATH
	or die "Can't open idsinternal.h.\n";
	foreach (@newidsinth) {
		print IDSINTH $_;
	}

	printf "%-30supdated     [ok]\n","IdsInternal.h";
#update IdsIntnl.inc

	open IDSINTINC, ">", $IDSINTINC_INCLUDE_PATH
	or die "Can't open".$IDSINTINC_INCLUDE_PATH."\n";
	

print IDSINTINC '; ****************************************************************************
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
; */';
 
print IDSINTINC "
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
; * PROVIDED TO YOU BY AMD ARE PROVIDED \"AS IS\" WITHOUT ANY EXPRESS OR IMPLIED
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
; * \"RESTRICTED RIGHTS.\" Use, duplication, or disclosure by the Government is
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

";

	foreach (@newidsintinc) {
		print IDSINTINC $_;
	}
	printf "%-30supdated     [ok]\n","IdsIntnl.inc";

close (AGESAH);
close (AGESAINC);
close (IDSINTH);
close (IDSINTINC);