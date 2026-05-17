/*********************************************************************

hardware.h

This is the file that holds information 
that let's the program know how to work 
with a given hardware device. 
So like all the settings and whatnot.

*********************************************************************/

#ifndef HARDWARE_H
#define HARDWARE_H

// Main disk format info.
// We need this for the FORMAT_ID_TYPE enum 
// that gives us the identifiers like: 
// cbm1541_cdos_525_ssdd_0171k_d64
#include "main_format_reference.h"  

#define BOOL unsigned int

// The index is the --> Unique Identifier - We use an enum for these. Example: cbm1541_cdos_525_ssdd_0171k_d64
struct gw_params_struct {
	char format_string[128]; // This is the parameter we send to greaseweazle do it know what format to use when reading or writing a disk. 
	BOOL rename_ext_req;     // Do we need to rename the ext before greaseweazle will accept the file? If yes then this is TRUE. 
	char rename_source[64];  // If we need to rename the extension, this is the original file extension. --> It occurs to me that maybe this info already exists in the main format table but actually there might be multiple file extensions so we need to know exactly what ext we are looking for. Or do we??? Maybe we don't care and just rename it anyway? Maybe not... maybe that should trigger an error? Yeah I think so. 
	char rename_target[64];  // If we need to rename the extension, this is what we need to rename it to in order for Greaseweazle to be happy with it. 
	BOOL conversion_req;     // If we need to use some external program to do some conversion to the whole file before greaseweazle will accept it, then this is TRUE. // TODO: I'm not sure how to handle the conversions right now. Might just be custom code and all we need here is the conversion_req to let us know that we need to do it. 
};/*end-struct*/

static const struct gw_params_struct gw_params[] = {
   // unique format identifier               greaseweazle format       ren req  source  target  convert
	[ cbm1541_cdos_525_ssdd_0171k_d64    ] = { "commodore.1541"          , FALSE , ""    , ""    , FALSE },
	[ cbm1541_cdos_525_ssdd_0171k_g64    ] = { "commodore.1541"          , FALSE , ""    , ""    , FALSE },
	[ cbm1571_cdos_525_dsdd_0342k_d71    ] = { "commodore.1571"          , FALSE , ""    , ""    , FALSE },
	[ cbm8050_cdos_525_dsqd_0521k_d80    ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ cbm8250_cdos_525_dsqd_1042k_d82    ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ cbmsfd1_cdos_525_dsqd_1042k_d82    ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ cbm1581_cdos_350_dsdd_0800k_d81    ] = { "commodore.1581"          , FALSE , ""    , ""    , FALSE },
	[ cmdfd2k_cdos_350_dsdd_0800k_d81    ] = { "commodore.cmd.fd2000.dd" , FALSE , ""    , ""    , FALSE },
	[ cmdfd2k_cdos_350_dshd_1620k_d2m    ] = { "commodore.cmd.fd2000.hd" , FALSE , ""    , ""    , FALSE },
	[ cmdfd4k_cdos_350_dshd_1620k_d4m    ] = { "commodore.cmd.fd4000.ed" , FALSE , ""    , ""    , FALSE },
	[ amiga_ados_350_dsdd_0880k_adf      ] = { "amiga.amigados"          , FALSE , ""    , ""    , FALSE },
	[ amiga_ados_350_dshd_1760k_adf      ] = { "amiga.amigados_hd"       , FALSE , ""    , ""    , FALSE },
	[ apple2_ados_525_ssdd_0140k_do      ] = { "apple2.appledos.140"     , FALSE , ""    , ""    , FALSE },
	[ apple2_pdos_525_ssdd_0140k_po      ] = { "apple2.prodos.140"       , FALSE , ""    , ""    , FALSE },
	[ apple2_ados_525_ssdd_0140k_dsk     ] = { "apple2.appledos.140"     , FALSE , ""    , ""    , FALSE },
	[ apple2_pdos_525_ssdd_0140k_dsk     ] = { "apple2.prodos.140"       , FALSE , ""    , ""    , FALSE },
	[ apple2_nofs_525_ssdd_0140k_dsk     ] = { "apple2.nofs.140"         , FALSE , ""    , ""    , FALSE },
	[ apple2gs_pdos_525_ssdd_0140k_2mg   ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ apple2gs_hfs_350_dsdd_0800k_2mg    ] = { "mac.800"                 , FALSE , ""    , ""    , TRUE  },
	[ apple2gs_pd16_350_dsdd_0800k_2mg   ] = { "mac.800"                 , FALSE , ""    , ""    , TRUE  },
	[ mac_mfs_350_ssdd_0400k_dc42        ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ mac_hfs_350_dsdd_0800k_dc42        ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ mac_hfs_350_ddhd_1440k_dc42        ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ mac_hfsp_350_ddhd_1440k_dc42       ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ mac_mfs_350_ssdd_0400k_raw         ] = { "mac.400"                 , FALSE , ""    , ""    , FALSE },
	[ mac_hfs_350_dsdd_0800k_raw         ] = { "mac.800"                 , FALSE , ""    , ""    , FALSE },
	[ mac_hfs_350_ddhd_1440k_raw         ] = { "ibm.1440"                , FALSE , ""    , ""    , FALSE },
	[ mac_hfsp_350_ddhd_1440k_raw        ] = { "ibm.1440"                , FALSE , ""    , ""    , FALSE },
	[ atari_ados_525_sssd_0090k_atr      ] = { "atari.90"                , TRUE  , "ATR" , "IMG" , FALSE },
	[ atari_ados_525_ssdd_0130k_atr      ] = { "atari.130"               , TRUE  , "ATR" , "IMG" , FALSE },
	[ atari_ados_525_ssdd_0180k_atr      ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ atari_ados_525_dsdd_0360k_atr      ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ atari_sdos_525_sssd_0090k_atr      ] = { "atari.90"                , TRUE  , "ATR" , "IMG" , FALSE },
	[ atari_sdos_525_ssdd_0130k_atr      ] = { "atari.130"               , TRUE  , "ATR" , "IMG" , FALSE },
	[ atari_sdos_525_ssdd_0180k_atr      ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ atari_sdos_525_dsdd_0360k_atr      ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ atari_ados_525_sssd_0090k_xfd      ] = { "atari.90"                , FALSE , ""    , ""    , FALSE },
	[ atari_ados_525_ssdd_0130k_xfd      ] = { "atari.130"               , FALSE , ""    , ""    , FALSE },
	[ atari_ados_525_ssdd_0180k_xfd      ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ atari_ados_525_dsdd_0360k_xfd      ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ atari_sdos_525_sssd_0090k_xfd      ] = { "atari.90"                , FALSE , ""    , ""    , FALSE },
	[ atari_sdos_525_ssdd_0130k_xfd      ] = { "atari.130"               , FALSE , ""    , ""    , FALSE },
	[ atari_sdos_525_ssdd_0180k_xfd      ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ atari_sdos_525_dsdd_0360k_xfd      ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ atarist_fat12_350_ssdd_0360k_st    ] = { "atarist.360"             , FALSE , ""    , ""    , FALSE },
	[ atarist_fat12_350_dsdd_0720k_st    ] = { "atarist.720"             , FALSE , ""    , ""    , FALSE },
	[ atarist_fat12_350_dshd_1440k_st    ] = { "ibm.1440"                , FALSE , ""    , ""    , FALSE },
	[ atarist_fat12_350_ssdd_0400k_st    ] = { "atarist.400"             , FALSE , ""    , ""    , FALSE },
	[ atarist_fat12_350_ssdd_0440k_st    ] = { "atarist.440"             , FALSE , ""    , ""    , FALSE },
	[ atarist_fat12_350_dsdd_0800k_st    ] = { "atarist.800"             , FALSE , ""    , ""    , FALSE },
	[ atarist_fat12_350_dsdd_0880k_st    ] = { "atarist.880"             , FALSE , ""    , ""    , FALSE },
	[ atarist_fat12_350_ssdd_0360k_msa   ] = { "atarist.360"             , FALSE , ""    , ""    , FALSE },
	[ atarist_fat12_350_dsdd_0720k_msa   ] = { "atarist.720"             , FALSE , ""    , ""    , FALSE },
	[ atarist_fat12_350_dshd_1440k_msa   ] = { "ibm.1440"                , FALSE , ""    , ""    , FALSE },
	[ atarist_fat12_350_ssdd_0400k_msa   ] = { "atarist.400"             , FALSE , ""    , ""    , FALSE },
	[ atarist_fat12_350_ssdd_0440k_msa   ] = { "atarist.440"             , FALSE , ""    , ""    , FALSE },
	[ atarist_fat12_350_dsdd_0800k_msa   ] = { "atarist.800"             , FALSE , ""    , ""    , FALSE },
	[ atarist_fat12_350_dsdd_0880k_msa   ] = { "atarist.880"             , FALSE , ""    , ""    , FALSE },
	[ atarist_fat12_350_ssdd_0360k_stx   ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ atarist_fat12_350_dsdd_0720k_stx   ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ atarist_fat12_350_dshd_1440k_stx   ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ atarist_fat12_350_ssdd_0400k_stx   ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ atarist_fat12_350_ssdd_0440k_stx   ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ atarist_fat12_350_dsdd_0800k_stx   ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ atarist_fat12_350_dsdd_0880k_stx   ] = { ""                        , FALSE , ""    , ""    , FALSE },
	[ bbc_dfs_525_sssd_0100k_ssd         ] = { "acorn.dfs.ss"            , FALSE , ""    , ""    , FALSE },
	[ bbc_dfs_525_ssqd_0200k_ssd         ] = { "acorn.dfs.ss80"          , FALSE , ""    , ""    , FALSE },
	[ bbc_dfs_525_dssd_0200k_dsd         ] = { "acorn.dfs.ds"            , FALSE , ""    , ""    , FALSE },
	[ bbc_dfs_525_dsqd_0400k_dsd         ] = { "acorn.dfs.ds80"          , FALSE , ""    , ""    , FALSE },
	[ bbc_adfss_525_ssdd_0160k_adl       ] = { "acorn.adfs.160"          , FALSE , ""    , ""    , FALSE },
	[ bbc_adfsm_525_ssqd_0320k_adl       ] = { "acorn.adfs.320"          , FALSE , ""    , ""    , FALSE },
	[ bbc_adfsl_525_dsqd_0640k_adl       ] = { "acorn.adfs.640"          , FALSE , ""    , ""    , FALSE },
	[ arch_adfsd_350_dsdd_0800k_adl      ] = { "acorn.adfs.800"          , FALSE , ""    , ""    , FALSE },
	[ arch_adfse_350_dsdd_0800k_adl      ] = { "acorn.adfs.800"          , FALSE , ""    , ""    , FALSE },
	[ arch_adfsf_350_dshd_1600k_adl      ] = { "acorn.adfs.1600"         , FALSE , ""    , ""    , FALSE },
	[ msdos_fat12_525_ssdd_0160k_ima     ] = { "ibm.160"                 , FALSE , ""    , ""    , FALSE },
	[ msdos_fat12_525_ssdd_0180k_ima     ] = { "ibm.180"                 , FALSE , ""    , ""    , FALSE },
	[ msdos_fat12_525_dsdd_0320k_ima     ] = { "ibm.320"                 , FALSE , ""    , ""    , FALSE },
	[ msdos_fat12_525_dsdd_0360k_ima     ] = { "ibm.360"                 , FALSE , ""    , ""    , FALSE },
	[ msdos_fat12_525_dshd_1200k_ima     ] = { "ibm.1200"                , FALSE , ""    , ""    , FALSE },
	[ msdos_fat12_350_dsdd_0800k_ima     ] = { "ibm.800"                 , FALSE , ""    , ""    , FALSE },
	[ msdos_fat12_350_dsdd_0720k_ima     ] = { "ibm.720"                 , FALSE , ""    , ""    , FALSE },
	[ msdos_fat12_350_dshd_1440k_ima     ] = { "ibm.1440"                , FALSE , ""    , ""    , FALSE },
	[ msdos_fat12_350_dshd_1680k_ima     ] = { "Ibm.1680"                , FALSE , ""    , ""    , FALSE },
	[ msdos_fat12_350_dshd_2880k_ima     ] = { "ibm.2880"                , FALSE , ""    , ""    , FALSE },
	[ coco_decb_525_ssdd_0158k_dsk       ] = { "coco.decb"               , FALSE , ""    , ""    , FALSE },
	[ coco_decb_525_ssdd_0180k_dsk       ] = { "coco.decb.40t"           , FALSE , ""    , ""    , FALSE },
	[ coco_os9_525_ssdd_0180k_dsk        ] = { "coco.os9.40ss"           , FALSE , ""    , ""    , FALSE },
	[ coco_os9_525_dsdd_0360k_dsk        ] = { "coco.os9.40ds"           , FALSE , ""    , ""    , FALSE },
	[ coco_os9_525_ssqd_0360k_dsk        ] = { "coco.os9.80ss"           , FALSE , ""    , ""    , FALSE },
	[ coco_os9_525_dsqd_0720k_dsk        ] = { "coco.os9.80ds"           , FALSE , ""    , ""    , FALSE },
	[ coco_os9_350_ssdd_0360k_dsk        ] = { "coco.os9.80ss"           , FALSE , ""    , ""    , FALSE },
	[ coco_os9_350_dsdd_0720k_dsk        ] = { "coco.os9.80ds"           , FALSE , ""    , ""    , FALSE },
	[ dragon_ddos_525_ssdd_0180k_dsk     ] = { "dragon.40ss"             , FALSE , ""    , ""    , FALSE },
	[ dragon_ddos_525_dsdd_0360k_dsk     ] = { "dragon.40ds"             , FALSE , ""    , ""    , FALSE },
	[ dragon_ddos_525_ssqd_0360k_dsk     ] = { "dragon.80ss"             , FALSE , ""    , ""    , FALSE },
	[ dragon_ddos_525_dsqd_0720k_dsk     ] = { "dragon.80ds"             , FALSE , ""    , ""    , FALSE },
	[ dragon_ddos_525_ssdd_0180k_dmk     ] = { "dragon.40ss"             , FALSE , ""    , ""    , FALSE },
	[ dragon_ddos_525_dsdd_0360k_dmk     ] = { "dragon.40ds"             , FALSE , ""    , ""    , FALSE },
	[ dragon_ddos_525_ssqd_0360k_dmk     ] = { "dragon.80ss"             , FALSE , ""    , ""    , FALSE },
	[ dragon_ddos_525_dsqd_0720k_dmk     ] = { "dragon.80ds"             , FALSE , ""    , ""    , FALSE },
	[ olivettim20_pcos_525_dsdd_0280_img ] = { "olivetti.m20"            , FALSE , ""    , ""    , FALSE },
	[ olivettim20_pcos_525_dsdd_0280_imd ] = { "olivetti.m20"            , FALSE , ""    , ""    , FALSE },

};/*end-array*/

#endif /* HARDWARE_H */