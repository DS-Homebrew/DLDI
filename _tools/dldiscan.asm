;:----------------- DLDI Scan 2018 nocash
;------------------
;assemble via TASM 5.0 or similar
;MCOM32.BAT
; @ echo off
; tasm32 /ml %1 %2 %3 %4 %5
; IF exist %1.RC     brcc32 %1 %2 %3 %4 %5
; IF exist %1.RC     tlink32 %1,%1,,,,%1 -x -Tpe -ap -V4.0
; IF NOT exist %1.RC tlink32 %1,%1,,,,   -x -Tpe -ap -V4.0
; del %1.obj
;------------------
.386
.model flat,STDCALL
.code
locals
includelib \talky\tasm32\import32.lib
extrn ExitProcess    :proc
extrn GetStdHandle   :proc
MB_OK                   equ 0
MB_ICONEXCLAMATION      equ 30h
STD_INPUT_HANDLE        equ -10
STD_OUTPUT_HANDLE       equ -11
MAX_PATH                equ 260
FILE_ATTRIBUTE_NORMAL   equ 080h
CREATE_ALWAYS           equ 2
OPEN_EXISTING           equ 3
GENERIC_READ            equ 80000000h
GENERIC_WRITE           equ 40000000h
FILE_SHARE_READ         equ 1h
GMEM_FIXED              equ 0
GMEM_ZEROINIT           equ 40h
extrn CreateFileA   :proc
extrn WriteFile     :proc
extrn GetFileSize   :proc
extrn ReadFile      :proc
extrn CloseHandle   :proc
extrn GlobalAlloc   :proc
extrn GlobalFree    :proc
;------------------
;:----------------- Data
;------------------
.data
;------------------
align 4
 std_out        dd 0
 diskresult     dd 0
 computed_crc32 dd 0
 ddmem_start    dd 0
 ddmem_end      dd 0
 ddmem_size     dd 0
 nds_buf        dd 0
 dldi_offset_in_nds dd 0
 area_name      dd 0
 all_size       dd 0
 got_size       dd 0
 glue_size      dd 0
 bss_size       dd 0
;------------------
 align 4
 wrchr_buf    db 0
;------------------
txt_hello:
 db 'DLDISCAN - DLDI Scanning tool 2018 nocash',0dh,0ah
 db 0
txt_headline_dldi:
 db 'Fix Feature ddmem    all  glue got  bss  sizes filesize crc32    name',0
txt_headline_nds:
 db 'Fix Feature rambase  all  glue got  bss  sizes filesize rombase  name game area',0
;---
txt_arm9_area                   db 'ARM9',0
txt_arm7_area                   db 'ARM7',0
txt_other_area                  db '-??-',0   ;occurs in ".sc.nds" and ".ds.gba" files (weirdly those have ASCII string at nds hdr[020h], saying " NDS loader for GBA flashcards. v1.1.") (weirder: the actual nds header is at file offset 0200h!!!)
txt_warn_bugged_fixall          db 'WARN:UsesBuggedFixAll',0
txt_warn_overlap_got_glue       db 'WARN:OVERLAP ',0
txt_warn_unaligned_ptrs         db 'WARN:UNALIGNED ',0
txt_warn_got_glue_struct        db 'WARN:STRUCT ',0
txt_warn_non_ddmem_in_got_glue  db 'WARN:NONDDMEM ',0
txt_warn_bss_excess             db 'WARN:BSS_EXCESS ',0
txt_warn_end_ptr_overkill       db 'WARN:OVERKILLEND ',0
txt_warn_exceed_32kbyte         db 'WARN:EXCEED32K ',0
txt_warn_id_version             db 'WARN:ID/VERSION ',0
txt_warn_thumb                  db 'WARN:THUMB ',0
txt_warn_crc_unknown            db 'WARN:CRC=UNKNOWN',0
;- - -
txt_warn_non_64byte_aligned     db 'WARN:NOALIGN64 ',0
txt_warn_non_32kbyte_alloc      db 'WARN:NOALLOC32K',0
;- - -
findname_mask_dldi db '*.dldi',0
findname_mask_nds  db '*.nds',0
;---
crc32_list_start:
 dd 0A12DEDB9h ;default.dldi
 dd 01B0146E0h ;acek.dldi
 dd 0A2A189ECh ;cyds.dldi
 dd 0435CCCFDh ;gmtf.dldi
 dd 02AEF560Dh ;gmtf2.dldi
 dd 0BF1CADC2h ;dlms.dldi
 dd 061852856h ;dlms_moon.dldi
 dd 0814BFBA4h ;dlms3.dldi
 dd 0AD52070Dh ;dlms4.dldi
 dd 0C4760A67h ;dsx.dldi
 dd 07C3E3952h ;ewsd.dldi
 dd 0115B4FE3h ;ezsd.dldi
 dd 00D2F707Bh ;ez5s.dldi
 dd 0CA19D819h ;ez5sdhc.dldi
 dd 05E7F4D4Bh ;g6fl.dldi
 dd 01C0DB9ABh ;g6real.dldi
 dd 07F2F05DBh ;m3cf.dldi
 dd 0F784899Fh ;m3sd.dldi
 dd 0E113B19Fh ;m3sd_alt.dldi
 dd 0F60C95F8h ;mmcf.dldi
 dd 0B8A8B6F6h ;mpcf.dldi
 dd 08AB819BAh ;mpsd.dldi
 dd 0B6BD2DA8h ;nmmc.dldi
 dd 01C0A7124h ;mk5n.dldi
 dd 0BB6635E2h ;nsd2.dldi
 dd 05A582569h ;nsd2v2.dldi
 dd 0BA8E0A84h ;njsd.dldi
 dd 01F0B6682h ;x9sd6.dldi
 dd 04500CED0h ;r4tf.dldi
 dd 0487F9E66h ;r4tf_v2.dldi
 dd 0C37E0574h ;sccf.dldi
 dd 0284E6CCFh ;scds.dldi
 dd 0A55FC880h ;scds1.dldi
 dd 0FC50EBF0h ;scds2.dldi
 dd 0EA8E72BFh ;scdssdhc.dldi
 dd 02B4B9CFCh ;scdssdhc1.dldi
 dd 0D41872EAh ;scsd.dldi
 dd 0A10A15A7h ;scsd_moon.dldi
 dd 0F27EC518h ;sclt.dldi
 dd 0E4140E7Ch ;SCRB.dldi
 dd 08D3B9260h ;killer.dldi
 dd 0C1D6DD19h ;ak2_sd_official.dldi (official version, unlike the akaio one)
 dd 069F95B8Ch ;RPGS WARN:THUMB WARN:OVERKILLEND ak2_sd_akaio190.dldi
 dd 042BA42ADh ;RPGS WARN:THUMB WARN:OVERKILLEND ak2_sd_akaio181.dldi
 dd 06E410A07h ;RPGN WARN:THUMB WARN:OVERKILLEND rpg_nand_akaio181-190.dldi  ;highly bugged: alloc's only 8K, but BSS zerofills MORE than 16K
 dd 04B959C56h ;RPGS WARN:THUMB                  rpg_sd_akaio181-190.dldi
crc32_list_end:
;---
wanted_dldi_id_version:
 db 0EDh,0A5h,08Dh,0BFh  ;00h 4  DLDI ID       (EDh,A5h,8Dh,BFh) (aka BF8DA5EDh)  ;\patching tools will
 db 20h,"Chishm",00h     ;04h 8  DLDI String   (20h,"Chishm",00h)                 ; refuse any other
 db 01h                  ;0Ch 1  DLDI Version  (01h in .dldi, don't care in .nds) ;/values
wanted_dldi_id_version_end:
;---
;------------------
.data?
 dldi_buffer db 10000h dup (?)
;------------------
.code
;------------------
;:----------------- Standalone Video I/O
;------------------
wrchr:
 pusha
 mov  byte ptr [wrchr_buf],al
 push 0                    ;set to 0 in most cases
 push offset diskresult    ;pointer to number of read bytes
 push 1                         ;length
 push offset wrchr_buf          ;source
 push dword ptr ds:[std_out]    ;handle
 call WriteFile
 popa
 ret
;------------------
wrstr_edx:
@@lop:
 mov  al,byte ptr [edx]
 inc  edx
 cmp  al,00h
 jz   short @@done
 call wrchr
 jmp  short @@lop
@@done:
 ret
;------------------
skipstr_esi:
@@lop:
 lodsb
 cmp  al,00h
 jnz  short @@lop
 ret
;------------------
wrspc:
 push eax
 mov  al,20h
 call wrchr
 pop  eax
 ret
;------------------
wrcrlf:
 push eax
 mov  al,0dh
 call wrchr
 mov  al,0ah
 call wrchr
 pop  eax
 ret
;------------------
wr_decimal_al:  ;supports only 2 digits (00..99)
 pusha
 mov  ah,0
 mov  dl,10
 div  dl
 or   ax,3030h
 call wrchr
 mov  al,ah ;remainder
 call wrchr
 popa
 ret
;------------------
wrdigital:
 push eax
 and  al,0fh
 cmp  al,09h
 jbe  short @@this
 add  al,07h
@@this:
 add  al,30h
 call wrchr
 pop  eax
 ret
;------------------
wrhexal:
 ror  al,4
 call wrdigital
 ror  al,4
 jmp  wrdigital
;------------------
wrhexax:
 xchg al,ah
 call wrhexal
 xchg al,ah
 jmp  wrhexal
;------------------
wrhexeax:
 ror  eax,16
 call wrhexax
 ror  eax,16
 jmp  wrhexax
;------------------
wrascii4chars_eax:
 mov  ecx,4
@@lop:
 cmp  al,20h
 jb   short @@huh
 cmp  al,7eh
 jbe  short @@this
@@huh:
 mov  al,7fh
@@this:
 call wrchr
 ror  eax,8
 loop short @@lop
 ret
;------------------
;:----------------- File I/O
;------------------
extrn FindFirstFileA :proc
extrn FindNextFileA  :proc
extrn FindClose      :proc
extrn FileTimeToLocalFileTime :proc
extrn FileTimeToDosDateTime   :proc
FILE_ATTRIBUTE_DIRECTORY             equ 10h
INVALID_HANDLE_VALUE                 equ -1
;------------------
diskfindfirst:  ;in: edx=search name, out: cy, al, es:bx
 pusha
 push offset flat_find_array
 push edx   ;search name
 call FindFirstFileA
 mov  dword ptr ds:[flat_find_handle],eax
 cmp  eax,INVALID_HANDLE_VALUE
 popa
 jnz  short diskfindinj ;okay!
 stc            ;cy = fail (probably none found at all)
 ret
;---
diskfindinj:
 test dword ptr ds:[flat_found_attr],FILE_ATTRIBUTE_DIRECTORY ;\
 mov  al,0                                                    ;
 jz   short @@non_dir                                         ; al=attr
 or   al,10h   ;dos-style directory-attribute                 ;
@@non_dir:                                                    ;/
;IF fmaxlen EQ 8+1+3+1   ;THIS ONLY FOR SHORT NAMES
;  mov  ebx,offset flat_found_dosname   ;ebx = dosname
;  cmp  byte ptr ds:[ebx],0             ;separate DOSNAME exists?
;  jnz  short @@good                    ;(exists only if NAME isn't a DOSNAME)
;ENDIF
 mov  ebx,offset flat_found_name        ;ebx = name (no separate DOSNAME)
@@good:
 clc                                            ;cy  = okay
 ret
;------------------
diskfindnext:  ;out: cy, al, es:bx
 pusha
 push offset flat_find_array
 push dword ptr ds:[flat_find_handle]
 call FindNextFileA
 or   eax,eax
 popa
 jnz  short diskfindinj ;---YUP, FOUND SOMETHING MORE ... !!!
;- - -
 push dword ptr ds:[flat_find_handle]   ;\NOPE, CLOSE...
 call FindClose                         ;/
 stc                    ;-flag no more found
 ret
;------------------
.data?
;---
flat_find_handle dd ?
;---
flat_find_array:
 flat_found_attr    dd  ?          ;buffer for attributes
 flat_creation_time dd  ?,?        ;file time stamp (creation)
                    dd  ?,?        ;file time stamp (last access)
 flat_lastwr_time   dd  ?,?        ;file time stamp (last write)
 flat_found_size    dd  ?,?        ;file size (high, low) (!!)
                    dd  ?,?        ;reserved
 flat_found_name    db  MAX_PATH dup (?) ;long filename
 flat_found_dosname db  14 dup (?)       ;dos filename
  dw ?  ;alignment padding
 ;*** the DOSNAME entry is ONLY used if the file has a NON-DOSNAME
 ;*** (ie. with more than 8 char, and/or lowercase chars)
 ;*** otherwise the NAME entry contains the DOSNAME, whereas the
 ;*** actual DOSNAME entry is empty (00)
;---
.code
;------------------
load_file_to_mem:       ;in: eax=filename, edi=buf, out: dldi_buffer..., ebp=filesize
 push 0       ;for Win95 always 0       ;\
 push FILE_ATTRIBUTE_NORMAL             ;
 push OPEN_EXISTING                     ;
 push 0       ;no Security-attributes   ;
 push FILE_SHARE_READ                   ; open source file
 push GENERIC_READ   ;readaccess only   ;
 push eax            ;filename          ;
 call CreateFileA                       ;
 mov  ebx,eax        ;filehandle        ;
 inc  eax            ;FFFFFFFFh -> zf=1 ;
 jz   short @@fail                      ;/
 push 0 ;set to 0 in most cases         ;\
 push offset diskresult  ;ptr to result ;
 push dword ptr [flat_found_size+4]     ; read file to memory
 push edi  ;buf                         ;
 push ebx  ;filehandle                  ;
 call ReadFile                          ;/
 push ebx  ;filehandle                  ;\close file
 call CloseHandle                       ;/
 mov  ebp,dword ptr [flat_found_size+4] ;-
 clc    ;cy=0=okay
 ret
;---
@@fail:
 stc    ;cy=1=fail
 ret
;------------------
;:----------------- Memory Allocation
;------------------
alloc_buf:
 mov  eax,dword ptr [flat_found_size+4]
 add  eax,8000h       ;size+extra space ;\
 push eax      ;size                    ;
 push GMEM_FIXED                        ; alloc memory
 call GlobalAlloc                       ;
 mov  dword ptr [nds_buf],eax           ;
 mov  edi,eax  ;memory handle           ;
 or   eax,eax  ;memory handle           ;
;jz   short @@fail                      ;/
 ret          ;out: zf
;------------------
dealloc_buf:
 mov  edi,dword ptr [nds_buf]
 or   edi,edi
 jz   short @@none
 push edi  ;memory handle
 call GlobalFree
 xor  edi,edi ;now mark none allocated
@@none:
 ret
;------------------
;:----------------- CRC32
;------------------
make_crc32_table:
 pusha
 xor  ebx,ebx                           ;\
@@crc_lop1:                             ;
 mov  eax,ebx                           ;
 mov  ecx,8                             ;
@@crc_lop2:                             ; make crc table
 shr  eax,1                             ;
 jnc  short @@crc_cont                  ;
 xor  eax,0edb88320h                    ;
@@crc_cont:                             ;
 loop short @@crc_lop2                  ;
 mov  dword ptr [crc32_table+ebx*4],eax ;
 inc  bl  ;ebx/lsb (zf)                 ;
 jnz  short @@crc_lop1                  ;/
 popa
 ret
;------------------
crc32:  ;in: esi=src, ecx=len  ;for internal-diag (debug version) and ards cheats (gaming version)
 mov  eax,-1  ;initial crc value
calc_crc32_esi_ecx_eax:
 mov  dword ptr [@@initial],eax
 push edx
 mov  edx,dword ptr [@@initial]  ;initial crc value
 xor  eax,eax  ;clear MSBs              ;\
 jcxz short @@crc_skip                  ;
@@crc_lop:                              ;
 lods byte ptr ds:[esi]                 ;
 xor  al,dl                             ; update crc
 shr  edx,8                             ;
 xor  edx,dword ptr [crc32_table+eax*4] ;
 loop short @@crc_lop                   ;
@@crc_skip:                             ;/
 xor  edx,-1  ;final crc value
 mov  eax,edx
 pop  edx
 ret
;---
.data?
align 4
 @@initial dd ?
 crc32_table dd 100h dup (?)
.code
;------------------
;:----------------- Warnings
;------------------
warn_if_fix_all_used:              ;-WARN: ensure no FIX_ALL
 test byte ptr [dldi_buffer+0eh],01h    ;\test FIX_ALL (bugged)
 jz   short @@okay                      ;/
 mov  edx,offset txt_warn_bugged_fixall ;\WARN
 call wrstr_edx                         ;/
@@okay:
 ret
;------------------
warn_if_thumb_functions:          ;-WARN: check if functions with THUMB-bit
 mov  esi,offset dldi_buffer+68h
 mov  ecx,6
 xor  edx,edx
@@lop:
 lodsd
 or   edx,eax           ;merge THUMB bits
 loop short @@lop
 test dl,3              ;test THUMB bit0 (or misaligned ARM bit1)
 jz   short @@okay
 mov  edx,offset txt_warn_thumb ;\WARN
 call wrstr_edx                 ;/
@@okay:
 ret
;------------------
;warn_if_overlap_got_glue:         ;-WARN: check if GLUE+GOT overlap (if both used)
;        xxx
; mov  edx,offset txt_warn_overlap_got_glue      ;\WARN
; call wrstr_edx                                 ;/
;@@okay:
; ret
;------------------
warn_if_non_ddmem_got_glue:       ;-WARN: non-ddmem values in GOT+GLUE area

IF 0
 mov  eax,dword ptr [dldi_buffer+48h]  ;GLUE start      ;\
 mov  ecx,dword ptr [dldi_buffer+4ch]  ;GLUE end        ; small ARM code chunks
 call @@check_here                                      ; with ptrs in pool!!!
 jc   short @@oops_edx                                  ;/    (eg. in nmmc.dldi)
ENDIF

 mov  eax,dword ptr [dldi_buffer+50h]  ;GOT start       ;\
 mov  ecx,dword ptr [dldi_buffer+54h]  ;GOT end         ; contains ONLY ptrs
 call @@check_here                                      ; (or ZEROes)
 jc   short @@oops_edx                                  ;/
 jmp  short @@okay
;---
@@oops_edx:
;mov  edx,edx   ;txt                            ;\WARN
 call wrstr_edx                                 ;/
@@okay:
 ret
;--- --- ---
@@check_here:
 cmp  ecx,dword ptr [ddmem_end]
 ja   short @@check_bad_hdr
 sub  ecx,eax   ;len=end-start
 jz   short @@check_done        ;size can be 0 (eg. for GLUE at BF800080h..BF800080h in mpsd.dldi, which is flagged to have FIX_GLUE)
 jb   short @@check_bad_hdr
 sub  eax,dword ptr [ddmem_start]
 jb   short @@check_bad_hdr
 cmp  eax,80h  ;hdr size
 jb   short @@check_bad_hdr
 add  eax,offset dldi_buffer
 mov  esi,eax
@@check_lop:
 lodsd
 cmp  eax,0                             ;\allow LEADING ZEROES (for mpsd.dldi)
 jz   short @@check_ignore_zero         ;/
 cmp  eax,dword ptr [ddmem_start]       ;\
 jb   short @@check_bad_content         ; want values in ddmem start..end range
 cmp  eax,dword ptr [ddmem_end]         ;
 jae  short @@check_bad_content         ;/
@@check_ignore_zero:
 sub  ecx,4
 ja   short @@check_lop
@@check_done:
 clc    ;okay
 ret
;---
@@check_bad_hdr:
 mov  edx,offset txt_warn_got_glue_struct
 stc    ;bad
 ret
;---
@@check_bad_content:
 mov  edx,offset txt_warn_non_ddmem_in_got_glue
 stc    ;bad
 ret
;------------------
warn_if_unaligned_ptrs:       ;-WARN: unaligned GOT+GLUE+ALL+BSS ptrs
 mov  esi,offset dldi_buffer+40h
 mov  ecx,8
 xor  edx,edx
@@lop:
 lodsd
 or   edx,eax           ;merge align
 loop short @@lop
 test dl,3              ;test align
 jz   short @@okay
 mov  edx,offset txt_warn_unaligned_ptrs        ;\WARN
 call wrstr_edx                                 ;/
@@okay:
 ret
;------------------
warn_if_max_end_ptr_overkill:     ;-WARN: check MAX end ptr's
 mov  eax,dword ptr [ddmem_end]
 mov  ecx,dword ptr [flat_found_size+4] ;-filesize (begin of UNINIT memory, which contains OLD data from .nds file at patching time)
 add  ecx,dword ptr [ddmem_start]
;- - -
 test byte ptr [dldi_buffer+0eh],8      ;\FIX_BSS
 jz   short @@skip_bss                  ;/
 cmp  dword ptr [bss_size],0            ;\BSS size
 jz   short @@skip_bss                  ;/
 cmp  dword ptr [dldi_buffer+5ch],eax   ;\BSS end
 ja   short @@oops_bss_excess           ;/  (total EXCESS in rpg_nand.dldi, which claims to use only 8K in hdr[0Dh], but BSS is MUCH larger)
@@skip_bss:
;- - -
 sub  eax,3  ;<-- to avoid adjusted WORDs exceeding end (vs total size)
 sub  ecx,3  ;<-- to avoid adjusted WORDs exceeding end (vs FILESIZE)
;- - -
 test byte ptr [dldi_buffer+0eh],4      ;\FIX_GOT
 jz   short @@skip_got                  ;/
 cmp  dword ptr [got_size],0            ;\GOT size
 jz   short @@skip_got                  ;/
 cmp  dword ptr [dldi_buffer+54h],eax   ;\GOT end
 ja   short @@oops                      ;/
 cmp  dword ptr [dldi_buffer+54h],ecx   ;\GOT end vs filesize
 ja   short @@oops                      ;/
@@skip_got:
;- - -
 test byte ptr [dldi_buffer+0eh],2      ;\FIX_GLUE
 jz   short @@skip_glue                 ;/
 cmp  dword ptr [glue_size],0           ;\GLUE size
 jz   short @@skip_glue                 ;/
 cmp  dword ptr [dldi_buffer+4ch],eax   ;\GLUE end
 ja   short @@oops                      ;/
 cmp  dword ptr [dldi_buffer+4ch],ecx   ;\GLUE end vs filesize
 ja   short @@oops                      ;/
@@skip_glue:
;- - -
 test byte ptr [dldi_buffer+0eh],1      ;\FIX_ALL
 jz   short @@skip_all                  ;/
 cmp  dword ptr [all_size],0            ;\ALL size
 jz   short @@skip_all                  ;/
 cmp  dword ptr [dldi_buffer+44h],eax   ;\ALL end
 ja   short @@oops                      ;/
 cmp  dword ptr [dldi_buffer+44h],ecx   ;\ALL end vs filesize
 ja   short @@oops                      ;/
@@skip_all:
;- - -
 jmp  short @@okay
@@okay:
 ret
;---
@@oops:
 mov  edx,offset txt_warn_end_ptr_overkill      ;\WARN
 call wrstr_edx                                 ;/
 ret
;---
@@oops_bss_excess:
 mov  edx,offset txt_warn_bss_excess            ;\WARN BADLY!
 call wrstr_edx                                 ;/
 ret

;------------------
warn_if_exceed_32kbyte:     ;-WARN: exceed 32Kbytes
 cmp  byte ptr [dldi_buffer+0dh],0fh  ;aka 1 shl 15     ;\max size
 jbe  short @@okay                                      ;/
 mov  edx,offset txt_warn_exceed_32kbyte        ;\WARN
 call wrstr_edx                                 ;/
@@okay:
 ret
;------------------
warn_if_id_version_bad:           ;-WARN: verify ID+version, show ok/bad
 mov  esi,offset dldi_buffer+00h
 mov  edi,offset wanted_dldi_id_version
 mov  ecx,wanted_dldi_id_version_end-wanted_dldi_id_version
 repe cmpsb
 je   short @@okay
 mov  edx,offset txt_warn_id_version    ;\WARN
 call wrstr_edx                         ;/
@@okay:
 ret
;------------------
warn_if_unknown_crc32:      ;-WARN: unknown CRC32 (compare against known list)
 mov  eax,dword ptr [computed_crc32]
 mov  edi,offset crc32_list_start
 mov  ecx,(crc32_list_end-crc32_list_start)/4
 repne scasd
 je   short @@okay
 mov  edx,offset txt_warn_crc_unknown   ;\WARN
 call wrstr_edx                         ;/
@@okay:
 ret
;------------------
warn_if_non_64byte_aligned:     ;-WARN: not 64-byte aligned     ;\specific for .nds
 test dword ptr [dldi_offset_in_nds],3fh        ;\
 jz   short @@okay                              ;/
 mov  edx,offset txt_warn_non_64byte_aligned    ;\WARN
 call wrstr_edx                                 ;/
@@okay:
 ret
;------------------
warn_if_non_32kbyte_alloc:      ;-WARN: allocated isn't 32Kbyte ;/
 cmp  byte ptr [dldi_buffer+0fh],0fh            ;\
 je   short @@okay                              ;/
 mov  edx,offset txt_warn_non_32kbyte_alloc     ;\WARN
 call wrstr_edx                                 ;/
@@okay:
 ret
;------------------
show_if_arm9_arm7_area:
 mov  edi,20h       ;hdr[02xh]          ;\
 call @@check_here                      ; ARM9
 mov  edx,offset txt_arm9_area          ;
 jc   short @@this                      ;/
 mov  edi,30h       ;hdr[03xh]          ;\
 call @@check_here                      ; ARM7
 mov  edx,offset txt_arm7_area          ;
 jc   short @@this                      ;/
 mov  edx,offset txt_other_area         ;-Other
@@this:
 mov  dword ptr [area_name],edx         ;-memorize
 call wrstr_edx                         ;-show
 ret
;--- --- ---
@@check_here:
 add  edi,dword ptr [nds_buf]
 mov  eax,dword ptr [edi+00h] ;rom.offset
 cmp  dword ptr [dldi_offset_in_nds],eax
 jb   short @@not_here
 add  eax,dword ptr [edi+0ch] ;size
 cmp  dword ptr [dldi_offset_in_nds],eax
 jae  short @@not_here
 stc    ;match
 ret
;---
@@not_here:
 clc    ;nope
 ret
;------------------
;:----------------- Scan DLDI files
;------------------
scan_dldi_files:
 mov  edx,offset txt_headline_dldi      ;\
 call wrstr_edx                         ;
 call wrcrlf                            ;/
 mov  edx,offset findname_mask_dldi
 call diskfindfirst  ;in: cs:dx=search name, out: cy, al, es:bx
 jc         @@find_lop_done
@@find_lop:
 test dword ptr ds:[flat_found_attr],FILE_ATTRIBUTE_DIRECTORY ;\
 jnz        @@skip_dir                                        ;/
 mov  eax,offset flat_found_name                ;\
 mov  edi,offset offset dldi_buffer             ; load .dldi file
 call load_file_to_mem                          ;
 jc         @@fail                              ;/
 mov  esi,offset offset dldi_buffer
 mov  al,byte ptr [dldi_buffer+0eh]     ;\show FIX_xxx
 call wrhexal                           ;/
 call wrspc
 mov  eax,dword ptr [dldi_buffer+64h]   ;\show FEATURE_xxx
 call wrhexeax                          ;/
 call wrspc
 mov  eax,dword ptr [dldi_buffer+40h]   ;\
 mov  dword ptr [ddmem_start],eax       ; show ddmem text start
 call wrhexeax                          ;/
 call wrspc
 mov  eax,dword ptr [dldi_buffer+44h]   ;\
 sub  eax,dword ptr [dldi_buffer+40h]   ; show size for FIX_ALL
 mov  dword ptr [all_size],eax          ;
 call wrhexax                           ;/
 call wrspc
 mov  eax,dword ptr [dldi_buffer+4ch]   ;\
 sub  eax,dword ptr [dldi_buffer+48h]   ; show size for FIX_GLUE
 mov  dword ptr [glue_size],eax         ;
 call wrhexax                           ;/
 call wrspc
 mov  eax,dword ptr [dldi_buffer+54h]   ;\
 sub  eax,dword ptr [dldi_buffer+50h]   ; show size for FIX_GOT
 mov  dword ptr [got_size],eax          ;
 call wrhexax                           ;/
 call wrspc
 mov  eax,dword ptr [dldi_buffer+5ch]   ;\
 sub  eax,dword ptr [dldi_buffer+58h]   ; show size for FIX_BSS
 mov  dword ptr [bss_size],eax          ;
 call wrhexax                           ;/
 call wrspc
 mov  al,byte ptr [dldi_buffer+0dh]     ;\show .dldi size
 mov  edx,1                      ;\     ;
 mov  cl,al                      ;      ; and
 shl  edx,cl                     ;      ; compute/memorize size/end
 mov  dword ptr [ddmem_size],edx ;/     ;
 add  edx,dword ptr [ddmem_start];\     ;
 mov  dword ptr [ddmem_end],edx  ;/     ;
 call wrhexal                           ;/
 call wrspc
 mov  al,byte ptr [dldi_buffer+0fh]     ;\show .nds size (blah in .dldi)
 call wrhexal                           ;/
 call wrspc
 mov  eax,dword ptr [flat_found_size+4] ;\show filesize
 call wrhexeax                          ;/
 call wrspc
 mov  esi,offset dldi_buffer            ;\
 mov  ecx,dword ptr [flat_found_size+4] ; show crc32
 call crc32                             ; (and memorize it for warning)
 mov  dword ptr [computed_crc32],eax    ;
 call wrhexeax                          ;/
 call wrspc
 mov  eax,dword ptr [dldi_buffer+60h]   ;\show 4-char short name
 call wrascii4chars_eax                 ;/
 call wrspc
 call warn_if_exceed_32kbyte            ;-WARN: exceed 32Kbytes
 call warn_if_fix_all_used              ;-WARN: ensure no FIX_ALL
 call warn_if_thumb_functions           ;-WARN: check if functions with THUMB-bit
;call warn_if_overlap_got_glue          ;-WARN: check if GLUE+GOT overlap (if both used)
 call warn_if_unaligned_ptrs            ;-WARN: unaligned GOT+GLUE+ALL+BSS ptrs
 call warn_if_non_ddmem_got_glue        ;-WARN: non-ddmem values in GOT+GLUE area
        ;XXX warn on BF80h at "(addr AND 3)<>2" in above area (or directly after that area, in next three bytes)
 call warn_if_max_end_ptr_overkill      ;-WARN: check MAX end ptr's
 call warn_if_id_version_bad            ;-WARN: verify ID+version, show ok/bad
 call warn_if_unknown_crc32             ;-WARN: unknown CRC32 (compare against known list)
@@fail:
 call wrspc
 mov  edx,offset flat_found_name        ;\show filename
 call wrstr_edx  ;error name            ;/
 call wrcrlf
@@skip_dir:
 call diskfindnext
 jnc        @@find_lop
@@find_lop_done:
 ret
;------------------
;:----------------- Scan NDS files
;------------------
slow_find_dldi:        ;search id/string
 mov  edi,dword ptr [nds_buf]
 mov  ecx,dword ptr [flat_found_size+4]
 shr  ecx,2      ;div4
 sub  ecx,80h/4
 jbe  short @@not_found
@@search_more:
 mov  eax,0BF8DA5EDh                            ;\
 repne scasd                                    ; search ID word
 jne  short @@not_found                         ;/
 pusha                                          ;\
 mov  esi,offset wanted_dldi_id_version+4       ; also compare following
 mov  ecx,8                                     ; 8-byte ID string
 repe cmpsb                                     ; (required for dsdoom1.2.0,
 popa                                           ; which has ID word at file
 je   short @@found                             ; offset 54A60h, but no
 jcxz short @@not_found                         ; ID string at that place)
 jmp  short @@search_more                       ;/
;---
@@found:
 sub  edi,4  ;stepback onto ID word
 sub  edi,dword ptr [nds_buf]
 mov  dword ptr [dldi_offset_in_nds],edi
 clc    ;found
 ret
;---
@@not_found:
 stc
 ret
;---
;note:
; DLDI is used in dsdoom111.nds, dsdoom112.nds, dsdoom120.nds, dsdoom121.nds
; DLDI isn't used in dsdoom100.nds
;------------------
scan_nds_files:
 mov  edx,offset txt_headline_nds       ;\
 call wrstr_edx                         ;
 call wrcrlf                            ;/
 mov  edx,offset findname_mask_nds
 call diskfindfirst  ;in: cs:dx=search name, out: cy, al, es:bx
 jc         @@find_lop_done
@@find_lop:
 test dword ptr ds:[flat_found_attr],FILE_ATTRIBUTE_DIRECTORY ;\
 jnz        @@skip_dir                                        ;/
 call alloc_buf                                 ;\alloc buf
 jz         @@alloc_failed                      ;/
 mov  eax,offset flat_found_name                ;\
 mov  edi,dword ptr [nds_buf]                   ; load .nds file
 call load_file_to_mem                          ;
 jc         @@fail                              ;/
 call slow_find_dldi                            ;\search id/string
 jc         @@skip_non_dldi                     ;/
 mov  esi,dword ptr [dldi_offset_in_nds];\
 add  esi,dword ptr [nds_buf]           ;
 mov  edi,offset offset dldi_buffer     ;
 mov  cl,byte ptr [dldi_buffer+0fh]     ;
 mov  eax,1                             ;
 shl  eax,cl                            ; copy to dldi_buffer
 mov  ecx,eax                           ;
 cmp  ecx,80h                     ;\    ;
 jae  short @@min_okay            ;     ;
 mov  ecx,80h  ;want hdr at least ;     ;
@@min_okay:                       ;/    ;
 cmp  ecx,8000h                   ;\    ;
 jbe  short @@max_okay            ;     ;
 mov  ecx,8000h  ;want no more    ;     ;
@@max_okay:                       ;/    ;
 rep  movsb                             ;/
 mov  esi,offset offset dldi_buffer
 mov  al,byte ptr [dldi_buffer+0eh]     ;\show FIX_xxx
 call wrhexal                           ;/
 call wrspc
 mov  eax,dword ptr [dldi_buffer+64h]   ;\show FEATURE_xxx
 call wrhexeax                          ;/
 call wrspc
 mov  eax,dword ptr [dldi_buffer+40h]   ;\
 mov  dword ptr [ddmem_start],eax       ; show RAM text start (RAM base)
 call wrhexeax                          ;/
 call wrspc
 mov  eax,dword ptr [dldi_buffer+44h]   ;\
 sub  eax,dword ptr [dldi_buffer+40h]   ; show size for FIX_ALL
 mov  dword ptr [all_size],eax          ;
 call wrhexax                           ;/
 call wrspc
 mov  eax,dword ptr [dldi_buffer+4ch]   ;\
 sub  eax,dword ptr [dldi_buffer+48h]   ; show size for FIX_GLUE
 mov  dword ptr [glue_size],eax         ;
 call wrhexax                           ;/
 call wrspc
 mov  eax,dword ptr [dldi_buffer+54h]   ;\
 sub  eax,dword ptr [dldi_buffer+50h]   ; show size for FIX_GOT
 mov  dword ptr [got_size],eax          ;
 call wrhexax                           ;/
 call wrspc
 mov  eax,dword ptr [dldi_buffer+5ch]   ;\
 sub  eax,dword ptr [dldi_buffer+58h]   ; show size for FIX_BSS
 mov  dword ptr [bss_size],eax          ;
 call wrhexax                           ;/
 call wrspc
 mov  al,byte ptr [dldi_buffer+0dh]     ;\show .dldi size
 mov  edx,1                      ;\     ;
 mov  cl,al                      ;      ; and
 shl  edx,cl                     ;      ; compute/memorize size/end
 mov  dword ptr [ddmem_size],edx ;/     ;
 add  edx,dword ptr [ddmem_start];\     ;
 mov  dword ptr [ddmem_end],edx  ;/     ;
 call wrhexal                           ;/
 call wrspc
 mov  al,byte ptr [dldi_buffer+0fh]     ;\show .nds size (ALLOCATED mem)
 call wrhexal                           ;/
 call wrspc
 mov  eax,dword ptr [flat_found_size+4] ;\show filesize
 call wrhexeax                          ;/
 call wrspc
 mov  eax,dword ptr [dldi_offset_in_nds];\show offset in .nds file
 call wrhexeax                          ;/
 call wrspc
;mov  esi,offset dldi_buffer            ;\
;mov  ecx,dword ptr [flat_found_size+4] ; show crc32
;call crc32                             ; (and memorize it for warning)
;mov  dword ptr [computed_crc32],eax    ;
;call wrhexeax                          ;/
;call wrspc
 mov  eax,dword ptr [dldi_buffer+60h]   ;\show 4-char short name
 call wrascii4chars_eax                 ;/
 call wrspc
 mov  esi,dword ptr [nds_buf]           ;\show .nds hdr gamecode (4-char)
 mov  eax,dword ptr [esi+0ch]           ; (usually "####" for homebrew)
 call wrascii4chars_eax                 ;/(can be used to 'detect' possible-dldi-support)
 call wrspc
 call show_if_arm9_arm7_area            ;-show ARM9/ARM7 area (or other)
 call wrspc
;- - -
 call warn_if_non_64byte_aligned        ;-WARN: not 64-byte aligned     ;\specific for .nds
 call warn_if_non_32kbyte_alloc         ;-WARN: allocated isn't 32Kbyte ;/
  ;xxx test/show usual dldi stuff (yet don't insist on BF800000h in .nds)
  ;xxx detect if "default" driver/name
  ;xxx try determine matching .dldi file (exclude-OR-unadjust relocation)
  ;xxx try detect if .nds header is homebrew (eg. with "####", without NitroROM, etc.)
  ;xxx WARN if NOT within arm9 or arm7 bootcode --> cmp  dword ptr [area_name],,offset txt_other_area
;- - -
 call warn_if_exceed_32kbyte            ;-WARN: exceed 32Kbytes (.dldi size)
 call warn_if_fix_all_used              ;-WARN: ensure no FIX_ALL
 call warn_if_thumb_functions           ;-WARN: check if functions with THUMB-bit
;call warn_if_overlap_got_glue          ;-WARN: check if GLUE+GOT overlap (if both used)
 call warn_if_unaligned_ptrs            ;-WARN: unaligned GOT+GLUE+ALL+BSS ptrs
 call warn_if_non_ddmem_got_glue        ;-WARN: non-ddmem values in GOT+GLUE area
        ;XXX warn on BF80h at "(addr AND 3)<>2" in above area (or directly after that area, in next three bytes)
;call warn_if_max_end_ptr_overkill      ;-WARN: check MAX end ptr's
 call warn_if_id_version_bad            ;-WARN: verify ID+version, show ok/bad
;call warn_if_unknown_crc32             ;-WARN: unknown CRC32 (compare against known list)
@@fail:
 call wrspc
 mov  edx,offset flat_found_name        ;\show filename
 call wrstr_edx  ;error name            ;/
 call wrcrlf
@@skip_non_dldi:
 call dealloc_buf
@@alloc_failed:
@@skip_dir:
 call diskfindnext
 jnc        @@find_lop
@@find_lop_done:
 ret
;------------------
;:----------------- Main
;------------------
start:
 push STD_OUTPUT_HANDLE         ;\
 call GetStdHandle              ;
 mov  dword ptr [std_out],eax   ;/
 mov  edx,offset txt_hello      ;\
 call wrstr_edx                 ;/
 call make_crc32_table          ;-
 ;- - -
 call scan_dldi_files
 call wrcrlf
 call scan_nds_files
 ;- - -
 push 0  ;return value
 call ExitProcess               ;-terminate program
;------------------
end start
