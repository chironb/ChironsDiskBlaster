/*********************************************************************

main_format_reference.h

This holds all the stuff needed to support brands, systems and drives. 

WHen you want to add support for new stuff, most of the things are 
updated in here. Although there's a little guide I put together 
that outlines the steps. 

*********************************************************************/

#ifndef MAIN_FORMAT_REFERENCE_H
#define MAIN_FORMAT_REFERENCE_H

#ifndef ARRAY_LEN
#define ARRAY_LEN(a) (sizeof(a) / sizeof((a)[0]))
#endif

/****************************************/      
/****************************************/
// Brands
/****************************************/
/****************************************/

typedef enum {
    /*00*/  BRAND_AMIGA,
    /*01*/  BRAND_APPLEII,
    /*02*/  BRAND_ARCHIMEDES,
    /*03*/  BRAND_ATARI8BIT,
    /*04*/  BRAND_ATARIST,
    /*05*/  BRAND_BBCMICRO,
    /*06*/  BRAND_COMMODORE,
    /*07*/  BRAND_DRAGON,
    /*08*/  BRAND_MACINTOSH,
    /*09*/  BRAND_PCCOMPAT,
    /*10*/  BRAND_TANDYCOCO,
    /*11*/  BRAND_OLIVETTI,
            BRAND_FLUX,
            BRAND_COUNT
} brand;

static const char *brand_options[] = {
    /*00*/ "Amiga",
    /*01*/ "Apple",
    /*02*/ "Archimedes",
    /*03*/ "Atari 8-Bit",
    /*04*/ "Atari ST",
    /*05*/ "BBC Micro",
    /*06*/ "Commodore",     
    /*07*/ "Dragon",
    /*08*/ "Macintosh",
    /*09*/ "MS-DOS & PC Compatible",
    /*10*/ "Tandy Color Computer / CoCo",
    /*11*/ "Olivetti",
           "Flux - Archival - Machine Independent"
};/*end-array*/

/********************************************************************************/
/********************************************************************************/
// Systems
/********************************************************************************/
/********************************************************************************/

typedef enum {
    ARCHIMEDES_A310   = 0,
    ARCHIMEDES_A5000  = 1,
    ARCHIMEDES_COUNT  = 2
} archimedes_system;

static const char *archimedes_systems_options[] = {
    /*00*/ "Archimedes A310",
    /*01*/ "Archimedes A5000",
};/*end-array*/

/****************************************/

typedef enum {
    AMIGA_1000  = 0,
    AMIGA_500   = 1,
    // AMIGA_500P  = 
    AMIGA_600   = 2,
    AMIGA_1200  = 3,
    AMIGA_1500  = 4,
    AMIGA_2000  = 5,
    AMIGA_2500  = 6,
    AMIGA_3000  = 7,
    // AMIGA_3000T = ? ,
    AMIGA_4000  = 8 ,
    // AMIGA_4000T = ? ,
    AMIGA_CDTV  = 9 ,
    AMIGA_CD32  = 10,
    AMIGA_COUNT = 11
} amiga_system;

static const char *amiga_systems_options[] = {
    /*00*/ "Amiga 1000",
    /*01*/ "Amiga 500",
    // /**/ "Amiga 500+",
    /*02*/ "Amiga 600",
    /*03*/ "Amiga 1200",
    /*04*/ "Amiga 1500",
    /*05*/ "Amiga 2000",
    /*06*/ "Amiga 2500",
    /*07*/ "Amiga 3000",
    // /**/ "Amiga 3000T",
    /*08*/ "Amiga 4000",
    // /**/ "Amiga 4000T",
    /*09*/ "Amiga CDTV",
    /*10*/ "Amiga CD32"
};/*end-array*/

/****************************************/

// NOTE: For Apple-1 support the only real thing is that guy's IEC thing for the Apple-1.
//       Uncle Bernie's thing is not ready and he's probably not gonna write a DOS for it. I dunno. 
//       So if I were to support the Apple-1 in this program, I would probably have to make it
//       it's own "brand" or "family" since it might not have the Apple II drives but instead 
//       some other drives like the Commodore drives. In which case, based on the way this program
//       program is setup, it's easiest to just make it it's own brand.
//       Otherwise I would need to have a way of showing and hiding specific drives based on the 
//       currently selected computer. For example, I don't want ALL the Commodore drives 
//       showing up for ALL the Apple II computers. I would want the user to pick the 
//       Apple-1 and then ONLY see the Commodore drives, or, pick an Apple IIe and only see the
//       Apple II drives. Therefore the list of drives for the Apple brand would be:
//       All the Apple II drives, and all the Commodore drives, and then the currently selected 
//       computer would determine what drives were shown. I would do it but I don't really want to. 
//       At the moment there is basically no Apple-1 floppy disk solution you can currently 
//       buy or even make as a kit. So there's no point. But if that does come along then I'll
//       make the Apple-1 it's own "brand" or "family" and then it'll get whatever the hell
//       solution is actually available. It's too bad - I wish that Claudio guy would release
//       his Apple-1 interface as open-source!

typedef enum {
 // APPLE_1          ,
    APPLE_II         ,
    APPLE_IIPLUS     ,
    APPLE_BELLHOWELL ,
    APPLE_IIE        ,
    APPLE_IIE_ENH    ,
    APPLE_IIE_PLAT   ,
    APPLE_IIC        ,
    APPLE_IIC_PLUS   ,
    APPLE_IIGS       ,
    APPLE_III        ,
    APPLEII_COUNT    
} appleii_system;

static const char *appleii_systems_options[] = {
 // [ APPLE_1          ] = "Apple-1"                ,
    [ APPLE_II         ] = "Apple II"               ,
    [ APPLE_IIPLUS     ] = "Apple II Plus"          ,
    [ APPLE_BELLHOWELL ] = "Apple II Bell & Howell" ,
    [ APPLE_IIE        ] = "Apple IIe"              ,
    [ APPLE_IIE_ENH    ] = "Apple IIe Enhanced"     ,
    [ APPLE_IIE_PLAT   ] = "Apple IIe Platinum"     ,
    [ APPLE_IIC        ] = "Apple IIc"              ,
    [ APPLE_IIC_PLUS   ] = "Apple IIc Plus"         ,
    [ APPLE_IIGS       ] = "Apple IIGS"             ,
    [ APPLE_III        ] = "Apple ///"
};/*end-array*/

/****************************************/

typedef enum {
    ATARI8_400    = 0,
    ATARI8_800    = 1,
    ATARI8_1200XL = 2,
    ATARI8_600XL  = 3,
    ATARI8_800XL  = 4,
    ATARI8_65XE   = 5,
    ATARI8_130XE  = 6,
    ATARI8_800XE  = 7,
    ATARI8_XEGS   = 8,
    ATARI8_COUNT  = 9
} atari8bit_system;

static const char *atari8bit_systems_options[] = {
    /*00*/ "Atari 400",
    /*01*/ "Atari 800",
    /*02*/ "Atari 1200XL",
    /*03*/ "Atari 600XL",
    /*04*/ "Atari 800XL",
    /*05*/ "Atari 65XE",
    /*06*/ "Atari 130XE",
    /*07*/ "Atari 800XE",
    /*08*/ "Atari XEGS"
};/*end-array*/

/****************************************/

typedef enum {
    ATARIST_520ST     =  0,  
    ATARIST_1040ST    =  1,   
    ATARIST_520ST_FM  =  2,     
    ATARIST_1040STF   =  3,    
    ATARIST_520STF    =  4,   
    ATARIST_MEGA_ST_1 =  5,      
    // ATARIST_MEGA_ST_2 = , ?     
    // ATARIST_MEGA_ST_4 = , ?    
    ATARIST_1040STE   =  6,    
    ATARIST_520STE    =  7,   
    ATARIST_MEGA_STE  =  8,     
    ATARIST_TT030     =  9,  
    ATARIST_FALCON030 = 10, 
    ATARIST_COUNT     = 11
} atarist_system;

static const char *atarist_systems_options[] = {
    /*00*/ "AtariST 520ST",  
    /*01*/ "AtariST 1040ST",   
    /*02*/ "AtariST 520ST FM",     
    /*03*/ "AtariST 1040STF",    
    /*04*/ "AtariST 520STF",   
    /*05*/ "AtariST MEGA ST 1",      
    // /**/ "AtariST MEGA ST 2",      
    // /**/ "AtariST MEGA ST 4",      
    /*06*/ "AtariST 1040STE",    
    /*07*/ "AtariST 520STE",   
    /*08*/ "AtariST MEGA STE",     
    /*09*/ "AtariST TT030",  
    /*10*/ "AtariST FALCON"
};/*end-array*/

/****************************************/

typedef enum {
    BBC_MICRO_A        = 0,
    BBC_MICRO_B        = 1,
    BBC_MICRO_BP64     = 2,
    BBC_MICRO_BP128    = 3,
    BBC_MASTER_128     = 4,
    BBC_MASTER_TURBO   = 5,
    BBC_MASTER_512     = 6,
    BBC_MASTER_ET      = 7,
    BBC_MASTER_COMPACT = 8,
    ACORN_ELECTRON     = 9,
    BBC_COUNT          = 10
} bbcmicro_system;

static const char *bbcmicro_systems_options[] = {
    /*00*/ "BBC Micro Model A",
    /*01*/ "BBC Micro Model B",
    /*02*/ "BBC Micro Model B+ 64K",
    /*03*/ "BBC Micro Model B+ 128K",
    /*04*/ "BBC Master 128",
    /*05*/ "BBC Master Turbo",
    /*06*/ "BBC Master 512",
    /*07*/ "BBC Master ET",
    /*08*/ "BBC Master Compact",
    /*09*/ "Acorn Electron"
};/*end-array*/

/****************************************/

typedef enum {
    CBM_PET_2001 ,
    CBM_PET_3008 ,
    CBM_PET_4016 ,
    CBM_PET_4032 ,
    CBM_PET_8032 ,
    CBM_SUPERPET ,
    CBM_128_80   ,
    CBM_VIC20    ,
    CBM_VIC1001  ,
    CBM_64       ,
    CBM_64C      ,
    CBM_C64GS    ,
    CBM_SX64     ,
    CBM_116      ,
    CBM_16       ,
    CBM_PLUS4    ,
    CBM_128      ,
    CBM_128D     ,
    CBM_COUNT    
} commodore_system;

static const char *commodore_systems_options[] = {
    [ CBM_PET_2001 ] = "Commodore PET 2001"      ,
    [ CBM_PET_3008 ] = "Commodore PET 3008"      ,
    [ CBM_PET_4016 ] = "Commodore PET 4016"      ,
    [ CBM_PET_4032 ] = "Commodore PET 4032"      ,
    [ CBM_PET_8032 ] = "Commodore PET 8032"      ,       
    [ CBM_SUPERPET ] = "Commodore SuperPET 9000" ,
    [ CBM_128_80   ] = "Commodore CBM 128-80"    ,
    [ CBM_VIC20    ] = "Commodore VIC-20"        ,
    [ CBM_VIC1001  ] = "Commodore VIC-1001"      ,
    [ CBM_64       ] = "Commodore 64"            ,
    [ CBM_64C      ] = "Commodore 64C"           ,
    [ CBM_C64GS    ] = "Commodore C64GS"         ,
    [ CBM_SX64     ] = "Commodore SX-64"         ,
    [ CBM_116      ] = "Commodore 116"           ,
    [ CBM_16       ] = "Commodore 16"            ,
    [ CBM_PLUS4    ] = "Commodore Plus/4"        ,
    [ CBM_128      ] = "Commodore 128"           ,
    [ CBM_128D     ] = "Commodore 128D "
};/*end-array*/

/****************************************/

typedef enum {
    MAC_128K             =  0,
    MAC_512K             =  1,
    MAC_512KE            =  2,
    MAC_PLUS             =  3,
    MAC_SE               =  4,
    MAC_SE30             =  5,
    MAC_CLASSIC          =  6,
    MAC_CLASSIC_II       =  7,
    // MAC_COLOR_CLASSIC    =  8,
    // MAC_COLOR_CLASSIC_II =  9,
    MAC_PORTABLE         = 8,
    MAC_COUNT            = 9
} macintosh_system;

static const char *macintosh_systems_options[] = {
    "Macintosh 128K",
    "Macintosh 512K",
    "Macintosh 512Ke",
    "Macintosh Plus",
    "Macintosh SE",
    "Macintosh SE/30",
    "Macintosh Classic",
    "Macintosh Classic II",
    // "Macintosh Color Classic",
    // "Macintosh ColorClassicII",
    "Macintosh Portable"
};/*end-array*/

/****************************************/

typedef enum {
    PC_8088  = 0,
    PC_80286 = 1,
    PC_80386 = 2,
    PC_80486 = 3,
    PC_COUNT = 4
} pccompat_system;

static const char *pccompatibles_systems_options[] = {
    "First PCs & Clones",
    "Early 8088/286 Era",
    "Late 8088/286 Era",
    "386/486/Pentium+ Era"
};/*end-array*/

/****************************************/

typedef enum {
    TANDY_COCO1 = 0,
    TANDY_COCO2 = 1,
    TANDY_COCO3 = 2,
    TANDYCOCO_COUNT
} tandycoco_system;

static const char *tandycoco_systems_options[] = {
    "Tandy Color Computer 1",
    "Tandy Color Computer 2",
    "Tandy Color Computer 3"
};/*end-array*/

/****************************************/

typedef enum {
    DRAGON_32   = 0,
    DRAGON_64   = 1,
    DRAGON_TANO = 2,
    DRAGON_COUNT
} dragon_system;

static const char *dragon_systems_options[] = {
    "Dragon 32",
    "Dragon 64",
    "Dragon Tano"
};/*end-array*/

/****************************************/

typedef enum {
    OLIVETTI_M20 = 0,
    OLIVETTI_COUNT
} olivetti_system;

static const char *olivetti_systems_options[] = {
    "Olivetti M20"
};/*end-array*/

/****************************************/

typedef enum {
    FLUX_READWRITE = 0,
    FLUX_READONLY  = 1,
    FLUX_COUNT     = 2
} flux_mode;

// This is for doing flux stuff because it's machine independant. 
static const char *flux_systems_options[] = { 
    "Read & Write Supported",
    "Only Read Supported"
};/*end-array*/

/****************************************/

/********************************************************************************/
/********************************************************************************/
// Drives
/********************************************************************************/
/********************************************************************************/

typedef enum {
    ARCHIMEDES_DRIVE_DD_D  ,
    ARCHIMEDES_DRIVE_DD_E  ,
    ARCHIMEDES_DRIVE_HD    ,
    ARCHIMEDES_DRIVE_COUNT 
} archimedes_drive;

static const char *archimedes_drive_options[] = {
    [ ARCHIMEDES_DRIVE_DD_D ] = "3.5 DS/DD 800K ADFSd ADL",
    [ ARCHIMEDES_DRIVE_DD_E ] = "3.5 DS/DD 800K ADFSe ADL",
    [ ARCHIMEDES_DRIVE_HD   ] = "3.5 DS/HD 1.6M ADFSf ADL"
};/*end-array*/

/****************************************/

typedef enum {
    AMIGA_DRIVE_1010        = 0,
    AMIGA_DRIVE_A4K         = 1,
    AMIGA_DRIVE_COUNT       = 2
} amiga_drive;

static const char *amiga_drive_options[] = {
    "A1010    DS/DD 880K ADF",
    "A4K Int. DS/HD 1.7M ADF"
};/*end-array*/

/****************************************/

typedef enum {
    APPLEII_DRIVE_525_APPDOS_DO  = 0,
    APPLEII_DRIVE_525_PRODOS_PO  = 1,
    APPLEII_DRIVE_525_APPDOS_DSK = 2,
    APPLEII_DRIVE_525_PRODOS_DSK = 3,
    APPLEII_DRIVE_525_NOFS_DSK   = 4,
    APPLEII_DRIVE_35_PD16_2MG    = 5,
    APPLEII_DRIVE_35_HFS_2MG     = 6,
    APPLEII_DRIVE_COUNT          = 7
} appleii_drive;

static const char *appleii_drive_options[] = {
    "5.25 SS/DD 140 ADOS  DO" ,
    "5.25 SS/DD 140 PDOS  PO" ,
    "5.25 SS/DD 140 ADOS  DSK",
    "5.25 SS/DD 140 PDOS  DSK",
    "5.25 SS/DD 140 NoFS  DSK",
    "3.5  DS/DD 800 PD16  2MG",
    "3.5  DS/DD 800 HFS   2MG"
};/*end-array*/

/****************************************/

typedef enum {
    ATARI8_DRIVE_810        = 0,
    ATARI8_DRIVE_1050_DD130 = 1,
    ATARI8_DRIVE_1050_DD180 = 2,
    ATARI8_DRIVE_XF551      = 3,
    ATARI8_DRIVE_COUNT      = 4
} atari8bit_drive;

static const char *atari8bit_drive_options[] = {
    "810   SS/SD  90 ADOS ATR",
    "1050  SS/DD 130 ADOS ATR",
    "1050  SS/DD 180 ADOS ATR",
    "XF551 DS/DD 360 ADOS ATR"
};/*end-array*/

/****************************************/

typedef enum {
    ATARIST_SF354_SSDD_360K_ST  = 0,
    ATARIST_SF314_DSDD_720K_ST  = 1,
    ATARIST_HDINT_DSHD_1_4M_ST  = 2,
    ATARIST_SF354_SSDD_400K_ST  = 3,
    ATARIST_SF354_SSDD_440K_ST  = 4,
    ATARIST_SF354_DSDD_800K_ST  = 5,
    ATARIST_SF354_DSDD_880K_ST  = 6,
    ATARIST_SF354_SSDD_360K_MSA = 7,
    ATARIST_SF314_DSDD_720K_MSA = 8,
    ATARIST_HDINT_DSHD_1_4M_MSA = 9,
    ATARIST_SF354_SSDD_400K_MSA = 10,
    ATARIST_SF354_SSDD_440K_MSA = 11,
    ATARIST_SF354_DSDD_800K_MSA = 12,
    ATARIST_SF354_DSDD_880K_MSA = 13,
    ATARIST_SF354_SSDD_360K_STX = 14,
    ATARIST_SF314_DSDD_720K_STX = 15,
    ATARIST_HDINT_DSHD_1_4M_STX = 16,
    ATARIST_SF354_SSDD_400K_STX = 17,
    ATARIST_SF354_SSDD_440K_STX = 18,
    ATARIST_SF354_DSDD_800K_STX = 19,
    ATARIST_SF354_DSDD_880K_STX = 20,
    ATARIST_DRIVE_COUNT         = 21
} atarist_drive;

static const char *atarist_drive_options[] = {
    "3.5 SF354 SS/DD 360K  ST",
    "3.5 SF314 DS/DD 720K  ST",
    "3.5 HDINT DS/HD 1.4M  ST",
    "3.5 SF354 SS/DD 400K  ST",
    "3.5 SF354 SS/DD 440K  ST",
    "3.5 SF354 DS/DD 800K  ST",
    "3.5 SF354 DS/DD 880K  ST",
    "3.5 SF354 SS/DD 360K MSA",
    "3.5 SF314 DS/DD 720K MSA",
    "3.5 HDINT DS/HD 1.4M MSA",
    "3.5 SF354 SS/DD 400K MSA",
    "3.5 SF354 SS/DD 440K MSA",
    "3.5 SF354 DS/DD 800K MSA",
    "3.5 SF354 DS/DD 880K MSA",
    "3.5 SF354 SS/DD 360K STX",
    "3.5 SF314 DS/DD 720K STX",
    "3.5 HDINT DS/HD 1.4M STX",
    "3.5 SF354 SS/DD 400K STX",
    "3.5 SF354 SS/DD 440K STX",
    "3.5 SF354 DS/DD 800K STX",
    "3.5 SF354 DS/DD 880K STX"
};/*end-array*/

/****************************************/

typedef enum {
    BBC_DRIVE_525_SSSD_100_SSD ,
    BBC_DRIVE_525_SSQD_200_SSD ,
    BBC_DRIVE_525_DSQD_200_DSD ,
    BBC_DRIVE_525_DSQD_400_DSD ,
    BBC_DRIVE_525_SSDD_160_ADL ,
    BBC_DRIVE_525_SSQD_320_ADL ,
    BBC_DRIVE_525_DSQD_640_ADL ,
    BBC_DRIVE_COUNT            
} bbcmicro_drive;

static const char *bbcmicro_drive_options[] = {
    [ BBC_DRIVE_525_SSSD_100_SSD ] = "5.25 SS/SD 100  DFS  SSD" ,
    [ BBC_DRIVE_525_SSQD_200_SSD ] = "5.25 SS/QD 200  DFS  SSD" ,
    [ BBC_DRIVE_525_DSQD_200_DSD ] = "5.25 DS/SD 200  DFS  DSD" ,
    [ BBC_DRIVE_525_DSQD_400_DSD ] = "5.25 DS/QD 400  DFS  DSD" ,
    [ BBC_DRIVE_525_SSDD_160_ADL ] = "5.25 SS/DD 160 ADFSs ADL" ,
    [ BBC_DRIVE_525_SSQD_320_ADL ] = "5.25 SS/QD 320 ADFSm ADL" ,
    [ BBC_DRIVE_525_DSQD_640_ADL ] = "5.25 DS/QD 640 ADFSl ADL"
};/*end-array*/

/****************************************/

typedef enum {
    CBM_DRIVE_2040    ,
    CBM_DRIVE_3040    ,
    CBM_DRIVE_4040    ,
    CBM_DRIVE_2031    ,
    CBM_DRIVE_4031    ,
    CBM_DRIVE_1541    ,
    CBM_DRIVE_1541II  ,
    CBM_DRIVE_1551    ,
    CBM_DRIVE_1570    ,
    CBM_DRIVE_1571    ,
    CBM_DRIVE_1581    ,
    CBM_DRIVE_8050    ,
    CBM_DRIVE_8250    ,
    CBM_DRIVE_SFD1001 ,
    CBM_DRIVE_COUNT   
} commodore_drive;

static const char *commodore_drive_options[] = {
    [ CBM_DRIVE_2040    ] = "2040     SS/DD 171K D64",
    [ CBM_DRIVE_3040    ] = "3040     SS/DD 171K D64",
    [ CBM_DRIVE_4040    ] = "4040     SS/DD 171K D64",
    [ CBM_DRIVE_2031    ] = "2031     SS/DD 171K D64",
    [ CBM_DRIVE_4031    ] = "4031     SS/DD 171K D64",
    [ CBM_DRIVE_1541    ] = "1541     SS/DD 171K D64",
    [ CBM_DRIVE_1541II  ] = "1541-II  SS/DD 171K D64",
    [ CBM_DRIVE_1551    ] = "1551     SS/DD 171K D64",
    [ CBM_DRIVE_1570    ] = "1570     SS/DD 171K D64",
    [ CBM_DRIVE_1571    ] = "1571     DS/DD 342K D71",
    [ CBM_DRIVE_1581    ] = "1581     DS/DD 800K D81",
    [ CBM_DRIVE_8050    ] = "8050     SS/QD 521K D80",
    [ CBM_DRIVE_8250    ] = "8250     DS/QD 1.0M D82",
    [ CBM_DRIVE_SFD1001 ] = "SFD-1001 DS/QD 1.0M D82"
};/*end-array*/

/****************************************/

typedef enum {
    MAC_DRIVE_35_SS_400K = 0,
    MAC_DRIVE_35_DS_800K = 1,
    MAC_DRIVE_35_DS_1M4  = 2,
    MAC_DRIVE_COUNT      = 3
} macintosh_drive;

static const char *macintosh_drive_options[] = {
    "3.5 Drive SS/DD 400K IMG",
    "3.5 Drive DS/DD 800K IMG",
    "3.5 Drive DS/HD 1.4M IMG"
};/*end-array*/

/****************************************/

typedef enum {
    PC_DRIVE_525_DD_360_IMA  = 0,
    PC_DRIVE_525_HD_1M2_IMA  = 1,
    PC_DRIVE_35_DD_720_IMA   = 2,
    PC_DRIVE_35_HD_1M4_IMA   = 3,
    PC_DRIVE_525_DD_360_IMD  = 4,
    PC_DRIVE_525_HD_1M2_IMD  = 5,
    PC_DRIVE_35_DD_720_IMD   = 6,
    PC_DRIVE_35_HD_1M4_IMD   = 7,
    PC_DRIVE_COUNT           = 8
} pccompat_drive;

static const char *pccompatibles_drive_options[] = {
    "5.25 DS/DD 360K DOS IMA",
    "5.25 DS/HD 1.2M DOS IMA",
    "3.5  DS/DD 720K DOS IMA",
    "3.5  DS/HD 1.4M DOS IMA",
    "5.25 DS/DD 360K DOS IMD",
    "5.25 DS/HD 1.2M DOS IMD",
    "3.5  DS/DD 720K DOS IMD",
    "3.5  DS/HD 1.4M DOS IMD"
};/*end-array*/

/****************************************/

typedef enum {
    TANDYCOCO_525_SSDD_158_DECB_DSK ,
    TANDYCOCO_525_SSDD_180_DECB_DSK ,
    TANDYCOCO_525_SSDD_180_OS9_DSK  ,
    TANDYCOCO_525_DSDD_360_OS9_DSK  ,
    TANDYCOCO_525_SSQD_360_OS9_DSK  ,
    TANDYCOCO_525_DSQD_720_OS9_DSK  ,
    TANDYCOCO_35_SSDD_360_OS9_DSK   ,
    TANDYCOCO_35_DSDD_720_OS9_DSK   ,
    TANDYCOCO_DRIVE_COUNT           
} tandycoco_drive;

static const char *tandycoco_drive_options[] = {
    [ TANDYCOCO_525_SSDD_158_DECB_DSK ] = "5.25 SS/DD 158K DECB DSK",
    [ TANDYCOCO_525_SSDD_180_DECB_DSK ] = "5.25 SS/DD 180K DECB DSK",
    [ TANDYCOCO_525_SSDD_180_OS9_DSK  ] = "5.25 SS/DD 180K OS9  DSK", 
    [ TANDYCOCO_525_DSDD_360_OS9_DSK  ] = "5.25 DS/DD 360K OS9  DSK", 
    [ TANDYCOCO_525_SSQD_360_OS9_DSK  ] = "5.25 SS/QD 360K OS9  DSK", 
    [ TANDYCOCO_525_DSQD_720_OS9_DSK  ] = "5.25 DS/QD 720K OS9  DSK", 
    [ TANDYCOCO_35_SSDD_360_OS9_DSK   ] = "3.5  SS/DD 360K OS9  DSK", 
    [ TANDYCOCO_35_DSDD_720_OS9_DSK   ] = "3.5  DS/DD 720K OS9  DSK"  
};/*end-array*/

/****************************************/

typedef enum {
    DRAGON_DDOS_525_SSDD_0180K_DSK = 0,
    DRAGON_DDOS_525_DSDD_0360K_DSK = 1,
    DRAGON_DDOS_525_SSQD_0360K_DSK = 2,
    DRAGON_DDOS_525_DSQD_0720K_DSK = 3,
    DRAGON_DDOS_525_SSDD_0180K_VDK = 4,
    DRAGON_DDOS_525_DSDD_0360K_VDK = 5,
    DRAGON_DDOS_525_SSQD_0360K_VDK = 6,
    DRAGON_DDOS_525_DSQD_0720K_VDK = 7,
    DRAGON_DRIVE_COUNT             = 8
} dragon_drive;

static const char *dragon_drive_options[] = {
    "5.25 SS/DD 180K DDOS DSK",
    "5.25 DS/DD 360K DDOS DSK",
    "5.25 SS/QD 360K DDOS DSK", 
    "5.25 DS/QD 720K DDOS DSK", 
    "5.25 SS/DD 180K DDOS VDK", 
    "5.25 DS/DD 360K DDOS VDK", 
    "5.25 SS/QD 360K DDOS VDK", 
    "5.25 DS/QD 720K DDOS VDK"  
};/*end-array*/

/****************************************/

typedef enum {
    OLIVETTIM20_PCOS_525_DSDD_0280K_IMG = 0,
    OLIVETTIM20_PCOS_525_DSDD_0280K_IMD = 1,
    OLIVETTI_DRIVE_COUNT 
} olivetti_drive;

static const char *olivetti_drive_options[] = {
    "5.25 DS/DD 280K PCOS IMG",
    "5.25 DS/DD 280K PCOS IMD",
};/*end-array*/

/****************************************/

typedef enum {
    FLUX_RW_DRIVE_SCP   = 0,
    FLUX_RW_DRIVE_HFE   = 1,
    FLUX_RW_DRIVE_DMK   = 2,
    FLUX_RW_DRIVE_RAW   = 3,
    FLUX_RW_DRIVE_COUNT = 4
} fluxreadwrite_drive;

static const char *fluxreadwrite_drive_options[] = {
    "SuperCard Pro Flux SCP",
    "HxC Flux Image     HFE",
    "TRS-80 Track Image DMK",
    "Flux (Multi-File)  RAW"
};/*end-array*/

/****************************************/

typedef enum {
    FLUX_RO_DRIVE_IPF   = 0,
    FLUX_RO_DRIVE_CTR   = 1,
    FLUX_RO_DRIVE_A2R   = 2,
    FLUX_RO_DRIVE_COUNT = 3
} fluxreadonly_drive;

static const char *fluxreadonly_drive_options[] = {
    "SPS/CAPS Archive   IPF",
    "CTRaw SPS Flux     CTR",
    "Apple II Flux Arch A2R"
};/*end-array*/

/****************************************/

/**********************************************/
/**********************************************/
// Unique Format Strings
/**********************************************/
/**********************************************/

typedef enum {
    cbm1541_cdos_525_ssdd_0171k_d64,
    cbm1541_cdos_525_ssdd_0171k_g64,
    cbm1571_cdos_525_dsdd_0342k_d71,
    cbm8050_cdos_525_dsqd_0521k_d80,
    cbm8250_cdos_525_dsqd_1042k_d82,
    cbmsfd1_cdos_525_dsqd_1042k_d82,
    cbm1581_cdos_350_dsdd_0800k_d81,
    cmdfd2k_cdos_350_dsdd_0800k_d81,
    cmdfd2k_cdos_350_dshd_1620k_d2m,
    cmdfd4k_cdos_350_dshd_1620k_d4m,
    amiga_ados_350_dsdd_0880k_adf,
    amiga_ados_350_dshd_1760k_adf,
    apple2_ados_525_ssdd_0140k_do,
    apple2_pdos_525_ssdd_0140k_po,
    apple2_ados_525_ssdd_0140k_dsk,
    apple2_pdos_525_ssdd_0140k_dsk,
    apple2_nofs_525_ssdd_0140k_dsk,
    apple2gs_pdos_525_ssdd_0140k_2mg,
    apple2gs_hfs_350_dsdd_0800k_2mg,
    apple2gs_pd16_350_dsdd_0800k_2mg,
    mac_mfs_350_ssdd_0400k_dc42,
    mac_hfs_350_dsdd_0800k_dc42,
    mac_hfs_350_ddhd_1440k_dc42,
    mac_hfsp_350_ddhd_1440k_dc42,
    mac_mfs_350_ssdd_0400k_raw,
    mac_hfs_350_dsdd_0800k_raw,
    mac_hfs_350_ddhd_1440k_raw,
    mac_hfsp_350_ddhd_1440k_raw,
    atari_ados_525_sssd_0090k_atr,
    atari_ados_525_ssdd_0130k_atr,
    atari_ados_525_ssdd_0180k_atr,
    atari_ados_525_dsdd_0360k_atr,
    atari_sdos_525_sssd_0090k_atr,
    atari_sdos_525_ssdd_0130k_atr,
    atari_sdos_525_ssdd_0180k_atr,
    atari_sdos_525_dsdd_0360k_atr,
    atari_ados_525_sssd_0090k_xfd,
    atari_ados_525_ssdd_0130k_xfd,
    atari_ados_525_ssdd_0180k_xfd,
    atari_ados_525_dsdd_0360k_xfd,
    atari_sdos_525_sssd_0090k_xfd,
    atari_sdos_525_ssdd_0130k_xfd,
    atari_sdos_525_ssdd_0180k_xfd,
    atari_sdos_525_dsdd_0360k_xfd,
    atarist_fat12_350_ssdd_0360k_st,
    atarist_fat12_350_dsdd_0720k_st,
    atarist_fat12_350_dshd_1440k_st,
    atarist_fat12_350_ssdd_0400k_st,
    atarist_fat12_350_ssdd_0440k_st,
    atarist_fat12_350_dsdd_0800k_st,
    atarist_fat12_350_dsdd_0880k_st,
    atarist_fat12_350_ssdd_0360k_msa,
    atarist_fat12_350_dsdd_0720k_msa,
    atarist_fat12_350_dshd_1440k_msa,
    atarist_fat12_350_ssdd_0400k_msa,
    atarist_fat12_350_ssdd_0440k_msa,
    atarist_fat12_350_dsdd_0800k_msa,
    atarist_fat12_350_dsdd_0880k_msa,
    atarist_fat12_350_ssdd_0360k_stx,
    atarist_fat12_350_dsdd_0720k_stx,
    atarist_fat12_350_dshd_1440k_stx,
    atarist_fat12_350_ssdd_0400k_stx,
    atarist_fat12_350_ssdd_0440k_stx,
    atarist_fat12_350_dsdd_0800k_stx,
    atarist_fat12_350_dsdd_0880k_stx,
    bbc_dfs_525_sssd_0100k_ssd,
    bbc_dfs_525_ssqd_0200k_ssd,
    bbc_dfs_525_dssd_0200k_dsd,
    bbc_dfs_525_dsqd_0400k_dsd,
    bbc_adfss_525_ssdd_0160k_adl,
    bbc_adfsm_525_ssqd_0320k_adl,
    bbc_adfsl_525_dsqd_0640k_adl,
    arch_adfsd_350_dsdd_0800k_adl,
    arch_adfse_350_dsdd_0800k_adl,
    arch_adfsf_350_dshd_1600k_adl,
    msdos_fat12_525_ssdd_0160k_ima,
    msdos_fat12_525_ssdd_0180k_ima,
    msdos_fat12_525_dsdd_0320k_ima,
    msdos_fat12_525_dsdd_0360k_ima,
    msdos_fat12_525_dshd_1200k_ima,
    msdos_fat12_350_dsdd_0800k_ima,
    msdos_fat12_350_dsdd_0720k_ima,
    msdos_fat12_350_dshd_1440k_ima,
    msdos_fat12_350_dshd_1680k_ima,
    msdos_fat12_350_dshd_2880k_ima,
    msdos_fat12_525_ssdd_0160k_imd,
    msdos_fat12_525_ssdd_0180k_imd,
    msdos_fat12_525_dsdd_0320k_imd,
    msdos_fat12_525_dsdd_0360k_imd,
    msdos_fat12_525_dshd_1200k_imd,
    msdos_fat12_350_dsdd_0800k_imd,
    msdos_fat12_350_dsdd_0720k_imd,
    msdos_fat12_350_dshd_1440k_imd,
    msdos_fat12_350_dshd_1680k_imd,
    msdos_fat12_350_dshd_2880k_imd,
    coco_decb_525_ssdd_0158k_dsk,
    coco_decb_525_ssdd_0180k_dsk,
    coco_os9_525_ssdd_0180k_dsk,
    coco_os9_525_dsdd_0360k_dsk,
    coco_os9_525_ssqd_0360k_dsk,
    coco_os9_525_dsqd_0720k_dsk,
    coco_os9_350_ssdd_0360k_dsk,
    coco_os9_350_dsdd_0720k_dsk,
    dragon_ddos_525_ssdd_0180k_dsk,
    dragon_ddos_525_dsdd_0360k_dsk,
    dragon_ddos_525_ssqd_0360k_dsk,
    dragon_ddos_525_dsqd_0720k_dsk,
    dragon_ddos_525_ssdd_0180k_dmk,
    dragon_ddos_525_dsdd_0360k_dmk,
    dragon_ddos_525_ssqd_0360k_dmk,
    dragon_ddos_525_dsqd_0720k_dmk,
    dragon_ddos_525_ssdd_0180k_vdk,
    dragon_ddos_525_dsdd_0360k_vdk,
    dragon_ddos_525_ssqd_0360k_vdk,
    dragon_ddos_525_dsqd_0720k_vdk,
    olivettim20_pcos_525_dsdd_0280_img,
    olivettim20_pcos_525_dsdd_0280_imd,
    FORMAT_ID_COUNT
} FORMAT_ID_TYPE;

/******************************************************************/
/******************************************************************/
/*                       MAIN DRIVE MAP                           */
/******************************************************************/
/******************************************************************/

// This let's use take the currently select brand and drive
// and pulls the relevant data from the main format table
// that has all the details about a given disk image format. 
//
// Like this: 
// main_format_table[ drive_map[brand][drive] ]; 
//  
// This is the thing that let's us know what to do with a given 
// selection. For example this let's us know if the Greaseweazle
// supports the current floppy disk image format. 
//
// This isn't in the main_format_reference.h file because this is 
// what goes from the program itself to the more general 
// concept of floppy disk image formats. This is application of 
// those ideas - the smallest distinct concept of a floppy 
// disk image - to the pracitcal working of this program. 
// 
// Access like this: drive_map[BRAND_AMIGA][AMIGA_DRIVE_1010]

static const int drive_map[BRAND_COUNT][50] = { 

    [ BRAND_AMIGA ] = { 
        [ AMIGA_DRIVE_1010 ] = amiga_ados_350_dsdd_0880k_adf ,
        [ AMIGA_DRIVE_A4K  ] = amiga_ados_350_dshd_1760k_adf
    },
    
    [ BRAND_APPLEII ] = { 
        [ APPLEII_DRIVE_525_APPDOS_DO ] = apple2_ados_525_ssdd_0140k_do    ,
        [ APPLEII_DRIVE_525_PRODOS_PO ] = apple2_pdos_525_ssdd_0140k_po    ,
        [ APPLEII_DRIVE_525_NOFS_DSK  ] = apple2_nofs_525_ssdd_0140k_dsk   ,
        [ APPLEII_DRIVE_525_APPDOS_DSK] = apple2_ados_525_ssdd_0140k_dsk   ,
        [ APPLEII_DRIVE_525_PRODOS_DSK] = apple2_pdos_525_ssdd_0140k_dsk   ,
        [ APPLEII_DRIVE_35_PD16_2MG   ] = apple2gs_pd16_350_dsdd_0800k_2mg ,
        [ APPLEII_DRIVE_35_HFS_2MG    ] = apple2gs_hfs_350_dsdd_0800k_2mg 
    },      

    [ BRAND_ARCHIMEDES ] = { 
        [ ARCHIMEDES_DRIVE_DD_D ] = arch_adfsd_350_dsdd_0800k_adl ,
        [ ARCHIMEDES_DRIVE_DD_E ] = arch_adfse_350_dsdd_0800k_adl ,
        [ ARCHIMEDES_DRIVE_HD   ] = arch_adfsf_350_dshd_1600k_adl
    },

    [ BRAND_ATARI8BIT ] = { 
        [ ATARI8_DRIVE_810        ] = atari_ados_525_sssd_0090k_atr ,
        [ ATARI8_DRIVE_1050_DD130 ] = atari_ados_525_ssdd_0130k_atr ,
        [ ATARI8_DRIVE_1050_DD180 ] = atari_ados_525_ssdd_0180k_atr ,
        [ ATARI8_DRIVE_XF551      ] = atari_ados_525_dsdd_0360k_atr 
    },  

    [ BRAND_ATARIST ] = { 
        [ ATARIST_SF354_SSDD_360K_ST  ] = atarist_fat12_350_ssdd_0360k_st  ,
        [ ATARIST_SF314_DSDD_720K_ST  ] = atarist_fat12_350_dsdd_0720k_st  ,
        [ ATARIST_HDINT_DSHD_1_4M_ST  ] = atarist_fat12_350_dshd_1440k_st  ,
        [ ATARIST_SF354_SSDD_400K_ST  ] = atarist_fat12_350_ssdd_0400k_st  ,
        [ ATARIST_SF354_SSDD_440K_ST  ] = atarist_fat12_350_ssdd_0440k_st  ,
        [ ATARIST_SF354_DSDD_800K_ST  ] = atarist_fat12_350_dsdd_0800k_st  ,
        [ ATARIST_SF354_DSDD_880K_ST  ] = atarist_fat12_350_dsdd_0880k_st  ,
        [ ATARIST_SF354_SSDD_360K_MSA ] = atarist_fat12_350_ssdd_0360k_msa ,
        [ ATARIST_SF314_DSDD_720K_MSA ] = atarist_fat12_350_dsdd_0720k_msa ,
        [ ATARIST_HDINT_DSHD_1_4M_MSA ] = atarist_fat12_350_dshd_1440k_msa ,
        [ ATARIST_SF354_SSDD_400K_MSA ] = atarist_fat12_350_ssdd_0400k_msa ,
        [ ATARIST_SF354_SSDD_440K_MSA ] = atarist_fat12_350_ssdd_0440k_msa ,
        [ ATARIST_SF354_DSDD_800K_MSA ] = atarist_fat12_350_dsdd_0800k_msa ,
        [ ATARIST_SF354_DSDD_880K_MSA ] = atarist_fat12_350_dsdd_0880k_msa ,
        [ ATARIST_SF354_SSDD_360K_STX ] = atarist_fat12_350_ssdd_0360k_stx ,
        [ ATARIST_SF314_DSDD_720K_STX ] = atarist_fat12_350_dsdd_0720k_stx ,
        [ ATARIST_HDINT_DSHD_1_4M_STX ] = atarist_fat12_350_dshd_1440k_stx ,
        [ ATARIST_SF354_SSDD_400K_STX ] = atarist_fat12_350_ssdd_0400k_stx ,
        [ ATARIST_SF354_SSDD_440K_STX ] = atarist_fat12_350_ssdd_0440k_stx ,
        [ ATARIST_SF354_DSDD_800K_STX ] = atarist_fat12_350_dsdd_0800k_stx ,
        [ ATARIST_SF354_DSDD_880K_STX ] = atarist_fat12_350_dsdd_0880k_stx
    },      

    [ BRAND_BBCMICRO ] = { 
        [ BBC_DRIVE_525_SSSD_100_SSD ] = bbc_dfs_525_sssd_0100k_ssd   ,
        [ BBC_DRIVE_525_SSQD_200_SSD ] = bbc_dfs_525_ssqd_0200k_ssd   ,
        [ BBC_DRIVE_525_DSQD_200_DSD ] = bbc_dfs_525_dssd_0200k_dsd   ,
        [ BBC_DRIVE_525_DSQD_400_DSD ] = bbc_dfs_525_dsqd_0400k_dsd   ,
        [ BBC_DRIVE_525_SSDD_160_ADL ] = bbc_adfss_525_ssdd_0160k_adl ,
        [ BBC_DRIVE_525_SSQD_320_ADL ] = bbc_adfsm_525_ssqd_0320k_adl ,
        [ BBC_DRIVE_525_DSQD_640_ADL ] = bbc_adfsl_525_dsqd_0640k_adl ,
    },       

    [ BRAND_COMMODORE ] = { 
        [ CBM_DRIVE_2040    ] = cbm1541_cdos_525_ssdd_0171k_d64 ,          
        [ CBM_DRIVE_3040    ] = cbm1541_cdos_525_ssdd_0171k_d64 ,          
        [ CBM_DRIVE_4040    ] = cbm1541_cdos_525_ssdd_0171k_d64 ,          
        [ CBM_DRIVE_2031    ] = cbm1541_cdos_525_ssdd_0171k_d64 ,          
        [ CBM_DRIVE_4031    ] = cbm1541_cdos_525_ssdd_0171k_d64 ,          
        [ CBM_DRIVE_1541    ] = cbm1541_cdos_525_ssdd_0171k_d64 ,          
        [ CBM_DRIVE_1541II  ] = cbm1541_cdos_525_ssdd_0171k_d64 ,          
        [ CBM_DRIVE_1551    ] = cbm1541_cdos_525_ssdd_0171k_d64 ,          
        [ CBM_DRIVE_1570    ] = cbm1541_cdos_525_ssdd_0171k_d64 ,          
        [ CBM_DRIVE_1571    ] = cbm1571_cdos_525_dsdd_0342k_d71 ,          
        [ CBM_DRIVE_1581    ] = cbm1581_cdos_350_dsdd_0800k_d81 ,          
        [ CBM_DRIVE_8050    ] = cbm8050_cdos_525_dsqd_0521k_d80 ,          
        [ CBM_DRIVE_8250    ] = cbm8250_cdos_525_dsqd_1042k_d82 ,          
        [ CBM_DRIVE_SFD1001 ] = cbmsfd1_cdos_525_dsqd_1042k_d82          
    },         

    [ BRAND_MACINTOSH ] = {  
        [ MAC_DRIVE_35_SS_400K ] = mac_mfs_350_ssdd_0400k_raw ,
        [ MAC_DRIVE_35_DS_800K ] = mac_hfs_350_dsdd_0800k_raw ,
        [ MAC_DRIVE_35_DS_1M4  ] = mac_hfs_350_ddhd_1440k_raw
    },        

    [ BRAND_PCCOMPAT ] = { 
        [ PC_DRIVE_525_DD_360_IMA ] = msdos_fat12_525_dsdd_0360k_ima ,
        [ PC_DRIVE_525_HD_1M2_IMA ] = msdos_fat12_525_dshd_1200k_ima ,
        [ PC_DRIVE_35_DD_720_IMA  ] = msdos_fat12_350_dsdd_0720k_ima ,
        [ PC_DRIVE_35_HD_1M4_IMA  ] = msdos_fat12_350_dshd_1440k_ima ,
        [ PC_DRIVE_525_DD_360_IMD ] = msdos_fat12_525_dsdd_0360k_imd ,
        [ PC_DRIVE_525_HD_1M2_IMD ] = msdos_fat12_525_dshd_1200k_imd ,
        [ PC_DRIVE_35_DD_720_IMD  ] = msdos_fat12_350_dsdd_0720k_imd ,
        [ PC_DRIVE_35_HD_1M4_IMD  ] = msdos_fat12_350_dshd_1440k_imd
    },    

    [ BRAND_TANDYCOCO ] = { 
        [ TANDYCOCO_525_SSDD_158_DECB_DSK ] = coco_decb_525_ssdd_0158k_dsk ,
        [ TANDYCOCO_525_SSDD_180_DECB_DSK ] = coco_decb_525_ssdd_0180k_dsk ,
        [ TANDYCOCO_525_SSDD_180_OS9_DSK  ] = coco_os9_525_ssdd_0180k_dsk  ,
        [ TANDYCOCO_525_DSDD_360_OS9_DSK  ] = coco_os9_525_dsdd_0360k_dsk  ,
        [ TANDYCOCO_525_SSQD_360_OS9_DSK  ] = coco_os9_525_ssqd_0360k_dsk  ,
        [ TANDYCOCO_525_DSQD_720_OS9_DSK  ] = coco_os9_525_dsqd_0720k_dsk  ,
        [ TANDYCOCO_35_SSDD_360_OS9_DSK   ] = coco_os9_350_ssdd_0360k_dsk  ,
        [ TANDYCOCO_35_DSDD_720_OS9_DSK   ] = coco_os9_350_dsdd_0720k_dsk
    },  

    [ BRAND_DRAGON ] = { 
        [ DRAGON_DDOS_525_SSDD_0180K_DSK ] = dragon_ddos_525_ssdd_0180k_dsk ,
        [ DRAGON_DDOS_525_DSDD_0360K_DSK ] = dragon_ddos_525_dsdd_0360k_dsk ,
        [ DRAGON_DDOS_525_SSQD_0360K_DSK ] = dragon_ddos_525_ssqd_0360k_dsk ,
        [ DRAGON_DDOS_525_DSQD_0720K_DSK ] = dragon_ddos_525_dsqd_0720k_dsk ,
        [ DRAGON_DDOS_525_SSDD_0180K_VDK ] = dragon_ddos_525_ssdd_0180k_vdk ,
        [ DRAGON_DDOS_525_DSDD_0360K_VDK ] = dragon_ddos_525_dsdd_0360k_vdk ,
        [ DRAGON_DDOS_525_SSQD_0360K_VDK ] = dragon_ddos_525_ssqd_0360k_vdk ,
        [ DRAGON_DDOS_525_DSQD_0720K_VDK ] = dragon_ddos_525_dsqd_0720k_vdk 
    },  

    [ BRAND_OLIVETTI ] = { 
        [ OLIVETTIM20_PCOS_525_DSDD_0280K_IMG ] = olivettim20_pcos_525_dsdd_0280_img ,
        [ OLIVETTIM20_PCOS_525_DSDD_0280K_IMD ] = olivettim20_pcos_525_dsdd_0280_imd
    }  

    // TODO: We aren't doing this for FLUX things because I'll handle that in code for now I think. 

};/*end-arrray*/

/******************************************************************/
/******************************************************************/
/*                 ITEMS FOR MAIN_FORMATS_STRUCT                  */
/******************************************************************/
/******************************************************************/

typedef enum {
    SUPPORTED            = 0,
    UNSUPPORTED          = 1,
    INDIRECTLY_SUPPORTED = 2,
    SUPPORT_TYPE_COUNT
} SUPPORT_TYPE;

typedef enum {
    FLOPPYDISK_8_INCH = 0,
    MINIDISK_525_INCH = 1,
    DISKETTE_35_INCH  = 2,
    FLOPPY_SIZE_TYPE_COUNT
} FLOPPY_SIZE_TYPE;

static const char *floppy_size_strings[] = { 
  [ FLOPPYDISK_8_INCH ] = "8"    ,
  [ MINIDISK_525_INCH ] = "5.25" ,
  [ DISKETTE_35_INCH  ] = "3.5"  
};/*end-array*/

typedef enum {
    INVALID_SIDES = 0,
    SS            = 1,
    DS            = 2,
    FLOPPY_SIDED_TYPE_COUNT
} FLOPPY_SIDED_TYPE;

static const char *floppy_sided_strings[] = { 
  [ INVALID_SIDES ] = "??",
  [ SS ]            = "SS",
  [ DS ]            = "DS" 
};/*end-array*/

typedef enum {
    SD = 0, // Single-Density
    DD = 1, // Double-Density
    QD = 2, // Quad-Density
    HD = 3, // High-Density
    ED = 4, // Extended-Density
    TD = 5, // Triple-Density --> It's a thing! https://www.vogons.org/viewtopic.php?t=98550 https://books.google.ca/books?id=rDwEAAAAMBAJ&pg=PA19&redir_esc=y#v=onepage&q&f=false http://bitsavers.informatik.uni-stuttgart.de/pdf/apple/disk/sony/uPD72070_Specification_1991.pdf
    DENSITY_TYPE_COUNT
} DENSITY_TYPE;

static const char *density_type_strings[] = { 
  [ SD ] = "SD",
  [ DD ] = "DD",
  [ QD ] = "QD",
  [ HD ] = "HD",
  [ ED ] = "ED",
  [ TD ] = "TD"
};/*end-array*/

typedef enum {
  GCR = 0,
  FM  = 1,
  MFM = 2,
  ENCODING_TYPE_COUNT
} ENCODING_TYPE;

static const char *encoding_type_strings[] = { 
  [ GCR ] = "GCR",
  [ FM  ] = "FM",
  [ MFM ] = "MFM",
};/*end-array*/

/******************************************************************/
/******************************************************************/
/*                      MAIN_FORMATS_STRUCT                       */
/******************************************************************/
/******************************************************************/

struct main_formats_struct {
  int               format_id;                // Unique Identifier - We use an enum for these. Example: cbm1541_cdos_525_ssdd_0171k_d64
  char              format_id_string[64];     // Unique Identifier String - More of a debugging thing. So we can print out "cbm1541_cdos_525_ssdd_0171k_d64" if I need to. 
  brand             format_brand;             // Use the brand enum for these. 
  char              assoc_model_string[64];   // String that holds the model of disk most associated with this disk image format.
  /* TODO: Remove assoc_model_string. It's not really being used. */
  /* TODO: MAYBE??? I need a associated drive this here. Like this: AMIGA_DRIVE_1010. But I can't because each drive is it's own enum. Damn. */
      
  SUPPORT_TYPE      greaseweazle_supported;   // Used to check if Greaseweazle directly supports this format. 
  SUPPORT_TYPE      zoomfloppy_supported;     // Used to check if ZoomFloppy directly supports this format.
  SUPPORT_TYPE      usbfloppy_supported;      // Used to check if a USB Floppy would directly supports this format.
  SUPPORT_TYPE      kryoflux_supported;       // Used to check if KryoFlux directly supports this format.
  SUPPORT_TYPE      sio2pc_supported;         // Used to check if a SIO2PC usb adapter directly supports this format.
                          
  FLOPPY_SIZE_TYPE  size_inches;              // Type of physical floppy disk.  5.25, 3.5 etc...
  FLOPPY_SIDED_TYPE sides;                    // Number of sides in this format. SS or DS
  DENSITY_TYPE      density;                  // The density of the disk format. SD, DD, etc...
  int               tpi;                      // TODO: Fucking I forgot TPI.
  ENCODING_TYPE     encoding;                 // The type of encoding of the disk format. GCR, FM, etc...
  int               rpm;                      // MAYBE use a negative number for averages. The average RPM of the floppy disk. For Macintosh it's the avergage, so 492.00 is = (394+590)/2 because the slowest tracks spin at 394 and the fastest spins at 590. Actually mayeb that's not the absoluetly correct average but it's good enough. 
  int               tracks;                   // Number of tracks of the disk format. 
  int               heads;                    // Number of heads of the disk format. This should match the FLOPPY_SIDED_TYPE sides entry. 
  int               sectors;                  // MAYBE use a negative number for averages. The average sectors per track. For Commodore 64 this is 19.51 = (394+590)/2 roughly the average sectors per track.
  int               block_size;               // Bytes per sector/block. e.g. 128, 256, 512, 1024
  int               size_kb;                  // Size of the disk image in kilobytes. For example a C64 .D64 image is 171KB.
  char              filesystem_format[32];    // This holds a brief string for the file system type. For example ProDOS, CBM DOS, AmigaDOS, FAT12, etc...
  char              file_ext[16];             // This is the mostly unique and primarily associated file extension for this image format. For example: The .IMG extension is almost unless and could be anything. But the .IMA extension is more associated with the MS-DOS floppy disk images. Same with Acorn .ADF where this is more strongly associated with Amiga and therefore the .ADL is the preferred extension here. 
  char              file_ext_alt1[16];        // Alternate known extensions for this format: 
  char              file_ext_alt2[16];        //   For example Macintosh disk images are often DiskCopy 4.2 based but could be any of IMAGE, IMG, DC42, DIMG.
  char              file_ext_alt3[16];        //   Also this should be used in some code where a user drags and drops a file onto the application, 
  char              file_ext_alt4[16];        //   and the program looks up the file extensions and can determine what format the disk image is, or, 
  char              file_ext_alt5[16];        //   it could tell the user that it could be one of multiple format and they have to pick the right one.
};/*end-struct*/

// TODO: Change all these entries so that the layout of each row matches the ones below 
//       where the last line is the first main file extension for the format. 

/*-------------------------------------------------------------------------------------------------
 * format_table[] - Master disk image format reference table.
 *
 *  { format_id,         format_id_string, format_brand,  assoc_model_string,
 *    gw_supported,      zf_supported,     usb_supported, kf_supported,       sio_supported,
 *    size_inches,       sides,            density,       encoding,           rpm,  tracks, heads, sectors, block, size_kb,
 *    filesystem_format, file_ext,         alt1,          alt2,               alt3,  alt4,  alt5  },
 *
 *------------------------------------------------------------------------------------------------*/
  
/******************************************************************/
/******************************************************************/
/*                   MAIN_FORMATS_STRUCT DATA                     */
/******************************************************************/
/******************************************************************/

static const struct main_formats_struct main_formats[] = {

    /*************/
    /* Commodore */
    /*************/

    { cbm1541_cdos_525_ssdd_0171k_d64, "cbm1541_cdos_525_ssdd_0171k_d64", BRAND_COMMODORE, "Commodore 1541", 
      SUPPORTED,   SUPPORTED,   UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, GCR, 300, 35, 1, 19, 256, 171,
      "CBM DOS", "D64", "", "", "", "", "" },

    { cbm1541_cdos_525_ssdd_0171k_g64, "cbm1541_cdos_525_ssdd_0171k_g64", BRAND_COMMODORE, "Commodore 1541",
      UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, GCR, 300, 35, 1, 19, 256, 171,
      "CBM DOS", "G64", "", "", "", "", "" },

    { cbm1571_cdos_525_dsdd_0342k_d71, "cbm1571_cdos_525_dsdd_0342k_d71", BRAND_COMMODORE, "Commodore 1571",
      SUPPORTED,   SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      MINIDISK_525_INCH, DS, DD, 48, GCR, 300, 35, 2, 19, 256, 342,
      "CBM DOS", "D71", "", "", "", "", "" },

    { cbm8050_cdos_525_dsqd_0521k_d80, "cbm8050_cdos_525_dsqd_0521k_d80", BRAND_COMMODORE, "Commodore 8050",
      UNSUPPORTED, SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      MINIDISK_525_INCH, DS, QD, 100, GCR, 300, 77, 1, 27, 256, 521,
      "CBM DOS", "D80", "", "", "", "", "" },

    { cbm8250_cdos_525_dsqd_1042k_d82, "cbm8250_cdos_525_dsqd_1042k_d82", BRAND_COMMODORE, "Commodore 8250",
      UNSUPPORTED, SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      MINIDISK_525_INCH, DS, QD, 100, GCR, 300, 77, 2, 27, 256, 1042,
      "CBM DOS", "D82", "", "", "", "", "" },

    { cbmsfd1_cdos_525_dsqd_1042k_d82, "cbmsfd1_cdos_525_dsqd_1042k_d82", BRAND_COMMODORE, "Commodore SFD-1001",
      UNSUPPORTED, SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      MINIDISK_525_INCH, DS, QD, 100, GCR, 300, 77, 2, 27, 256, 1042,
      "CBM DOS", "D82", "", "", "", "", "" },

    { cbm1581_cdos_350_dsdd_0800k_d81, "cbm1581_cdos_350_dsdd_0800k_d81", BRAND_COMMODORE, "Commodore 1581",
      SUPPORTED,   SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 135, GCR, 300, 80, 2, 10, 512, 800,
      "CBM DOS", "D81", "", "", "", "", "" },

    { cmdfd2k_cdos_350_dsdd_0800k_d81, "cmdfd2k_cdos_350_dsdd_0800k_d81", BRAND_COMMODORE, "CMD FD 2000",
      SUPPORTED,   SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 135, GCR, 300, 81, 2, 10, 512, 810,
      "CBM DOS", "D81", "", "", "", "", "" },

    { cmdfd2k_cdos_350_dshd_1620k_d2m, "cmdfd2k_cdos_350_dshd_1620k_d2m", BRAND_COMMODORE, "CMD FD 2000",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, DS, HD, 135, GCR, 300, 81, 2, 10, 1024, 1620,
      "CBM DOS", "D2M", "", "", "", "", "" },

    { cmdfd4k_cdos_350_dshd_1620k_d4m, "cmdfd4k_cdos_350_dshd_1620k_d4m", BRAND_COMMODORE, "CMD FD 4000",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, DS, ED, 135, GCR, 300, 81, 2, 20, 1024, 3240,
      "CBM DOS", "D4M", "", "", "", "", "" },

    /*********/ 
    /* Amiga */
    /*********/

    { amiga_ados_350_dsdd_0880k_adf, "amiga_ados_350_dsdd_0880k_adf", BRAND_AMIGA, "Amiga 1010",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 135, MFM, 300, 80, 2, 11, 512, 880,
      "AmigaDOS", "ADF", "ADZ", "", "", "", "" },

    { amiga_ados_350_dshd_1760k_adf, "amiga_ados_350_dshd_1760k_adf", BRAND_AMIGA, "Amiga 4000 Int. (Chinon FB-357A)",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, DS, HD, 135, MFM, 150, 80, 2, 22, 512, 1760,
      "AmigaDOS", "ADF", "ADZ", "", "", "", "" },

    /************/
    /* Apple II */
    /************/

    { apple2_ados_525_ssdd_0140k_do, "apple2_ados_525_ssdd_0140k_do", BRAND_APPLEII, "Apple Disk II",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, GCR, 300, 35, 1, 16, 256, 140,
      "Apple DOS", "DO", "D13", "", "", "", "" },

    { apple2_pdos_525_ssdd_0140k_po, "apple2_pdos_525_ssdd_0140k_po", BRAND_APPLEII, "Apple Disk II",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, GCR, 300, 35, 1, 16, 256, 140,
      "ProDOS", "PO", "", "", "", "", "" },

    { apple2_ados_525_ssdd_0140k_dsk, "apple2_ados_525_ssdd_0140k_dsk", BRAND_APPLEII, "Apple Disk II",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, GCR, 300, 35, 1, 16, 256, 140,
      "Apple DOS", "DSK", "D13", "", "", "", "" },

    { apple2_pdos_525_ssdd_0140k_dsk, "apple2_pdos_525_ssdd_0140k_dsk", BRAND_APPLEII, "Apple Disk II",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, GCR, 300, 35, 1, 16, 256, 140,
      "ProDOS", "DSK", "D13", "", "", "", "" },

    { apple2_nofs_525_ssdd_0140k_dsk, "apple2_nofs_525_ssdd_0140k_dsk", BRAND_APPLEII, "Apple Disk II",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, GCR, 300, 35, 1, 16, 256, 140,
      "None", "DSK", "D13", "", "", "", "" },

    { apple2gs_pdos_525_ssdd_0140k_2mg, "apple2gs_pdos_525_ssdd_0140k_2mg", BRAND_APPLEII, "Apple 5.25\" Disk",
      UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      MINIDISK_525_INCH, SS, DD, 135, GCR, 300, 35, 1, 16, 256, 140,
      "ProDOS", "2MG", "", "", "", "", "" },

    { apple2gs_hfs_350_dsdd_0800k_2mg, "apple2gs_hfs_350_dsdd_0800k_2mg", BRAND_APPLEII, "Apple 3.5\" Disk",
      SUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 135, GCR, 300, 80, 2, 10, 512, 800,
      "HFS", "2MG", "", "", "", "", "" },

    { apple2gs_pd16_350_dsdd_0800k_2mg, "apple2gs_pd16_350_dsdd_0800k_2mg", BRAND_APPLEII, "Apple 3.5\" Disk",
      SUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 135, GCR, 300, 80, 2, 10, 512, 800,
      "ProDOS 16", "2MG", "", "", "", "", "" },

    /*************/
    /* Macintosh */
    /*************/

    { mac_mfs_350_ssdd_0400k_dc42, "mac_mfs_350_ssdd_0400k_dc42", BRAND_MACINTOSH, "Apple 3.5\" Disk",
      UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      DISKETTE_35_INCH, SS, DD, 135, GCR, 492, 80, 1, 10, 512, 400,
      "MFS", "IMG", "DSK", "DC42", "DIMG", "", "" },

    { mac_hfs_350_dsdd_0800k_dc42, "mac_hfs_350_dsdd_0800k_dc42", BRAND_MACINTOSH, "Apple 3.5\" Disk",
      UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 135, GCR, 492, 80, 2, 10, 512, 800,
      "MFS", "IMG", "DSK", "DC42", "DIMG", "", "" },

    { mac_hfs_350_ddhd_1440k_dc42, "mac_hfs_350_ddhd_1440k_dc42", BRAND_MACINTOSH, "Apple 3.5\" Disk",
      UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      DISKETTE_35_INCH, DS, HD, 135, MFM, 300, 80, 2, 18, 512, 1440,
      "HFS", "IMG", "DSK", "DC42", "DIMG", "", "" },

    { mac_hfsp_350_ddhd_1440k_dc42, "mac_hfsp_350_ddhd_1440k_dc42", BRAND_MACINTOSH, "Apple 3.5\" Disk",
      UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      DISKETTE_35_INCH, DS, HD, 135, MFM, 300, 80, 2, 18, 512, 1440,
      "HFS+", "IMG", "DSK", "DC42", "DIMG", "", "" },

    { mac_mfs_350_ssdd_0400k_raw, "mac_mfs_350_ssdd_0400k_raw", BRAND_MACINTOSH, "Apple 3.5\" Disk",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      DISKETTE_35_INCH, SS, DD, 135, GCR, 492, 80, 1, 10, 512, 400,
      "MFS", "IMG", "DSK", "IMAGE", "", "", "" },

    { mac_hfs_350_dsdd_0800k_raw, "mac_hfs_350_dsdd_0800k_raw", BRAND_MACINTOSH, "Apple 3.5\" Disk",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 135, GCR, 492, 80, 2, 10, 512, 800,
      "MFS", "IMG", "DSK", "IMAGE", "", "", "" },

    { mac_hfs_350_ddhd_1440k_raw, "mac_hfs_350_ddhd_1440k_raw", BRAND_MACINTOSH, "Apple 3.5\" Disk",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      DISKETTE_35_INCH, DS, HD, 135, MFM, 300, 80, 2, 18, 512, 1440,
      "HFS", "IMG", "DSK", "IMAGE", "", "", "" },

    { mac_hfsp_350_ddhd_1440k_raw, "mac_hfsp_350_ddhd_1440k_raw", BRAND_MACINTOSH, "Apple 3.5\" Disk",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      DISKETTE_35_INCH, DS, HD, 135, MFM, 300, 80, 2, 18, 512, 1440,
      "HFS+", "IMG", "DSK", "IMAGE", "", "", "" },

    /*********************/
    /* Atari 8-bit - ATR */
    /*********************/

    { atari_ados_525_sssd_0090k_atr, "atari_ados_525_sssd_0090k_atr", BRAND_ATARI8BIT, "Atari 810",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, SUPPORTED,
      MINIDISK_525_INCH, SS, SD, 48, FM, 300, 40, 1, 18, 128, 90,
      "Atari DOS", "ATR", "IMG", "", "", "", "" },

    { atari_ados_525_ssdd_0130k_atr, "atari_ados_525_ssdd_0130k_atr", BRAND_ATARI8BIT, "Atari 1050",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, SUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, MFM, 300, 40, 1, 26, 128, 130,
      "Atari DOS", "ATR", "IMG", "", "", "", "" },

    { atari_ados_525_ssdd_0180k_atr, "atari_ados_525_ssdd_0180k_atr", BRAND_ATARI8BIT, "Atari 1050",
      UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, SUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, MFM, 300, 40, 1, 18, 256, 180,
      "Atari DOS", "ATR", "IMG", "", "", "", "" },

    { atari_ados_525_dsdd_0360k_atr, "atari_ados_525_dsdd_0360k_atr", BRAND_ATARI8BIT, "Atari XF551",
      UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, SUPPORTED,
      MINIDISK_525_INCH, DS, DD, 48, MFM, 300, 40, 2, 18, 256, 360,
      "Atari DOS", "ATR", "IMG", "", "", "", "" },

    { atari_sdos_525_sssd_0090k_atr, "atari_sdos_525_sssd_0090k_atr", BRAND_ATARI8BIT, "Atari 810",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, SUPPORTED,
      MINIDISK_525_INCH, SS, SD, 48, FM, 300, 40, 1, 18, 128, 90,
      "SpartaDOS", "ATR", "IMG", "", "", "", "" },

    { atari_sdos_525_ssdd_0130k_atr, "atari_sdos_525_ssdd_0130k_atr", BRAND_ATARI8BIT, "Atari 1050",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, SUPPORTED,
      MINIDISK_525_INCH, SS, SD, 48, MFM, 300, 40, 1, 26, 128, 130,
      "SpartaDOS", "ATR", "IMG", "", "", "", "" },

    { atari_sdos_525_ssdd_0180k_atr, "atari_sdos_525_ssdd_0180k_atr", BRAND_ATARI8BIT, "Atari 1050",
      UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, SUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, MFM, 300, 40, 1, 18, 256, 180,
      "SpartaDOS", "ATR", "IMG", "", "", "", "" },

    { atari_sdos_525_dsdd_0360k_atr, "atari_sdos_525_dsdd_0360k_atr", BRAND_ATARI8BIT, "Atari XF551",
      UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, SUPPORTED,
      MINIDISK_525_INCH, DS, DD, 48, MFM, 300, 40, 2, 18, 256, 360,
      "SpartaDOS", "ATR", "IMG", "", "", "", "" },

    /*********************/
    /* Atari 8-bit - XFD */
    /*********************/

    { atari_ados_525_sssd_0090k_xfd, "atari_ados_525_sssd_0090k_xfd", BRAND_ATARI8BIT, "Atari 810",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   SUPPORTED,
      MINIDISK_525_INCH, SS, SD, 48, FM, 300, 40, 1, 18, 128, 90,
      "Atari DOS", "XFD", "", "", "", "", "" },

    { atari_ados_525_ssdd_0130k_xfd, "atari_ados_525_ssdd_0130k_xfd", BRAND_ATARI8BIT, "Atari 1050",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   SUPPORTED,
      MINIDISK_525_INCH, SS, SD, 48, MFM, 300, 40, 1, 26, 128, 130,
      "Atari DOS", "XFD", "", "", "", "", "" },

    { atari_ados_525_ssdd_0180k_xfd, "atari_ados_525_ssdd_0180k_xfd", BRAND_ATARI8BIT, "Atari 1050",
      UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, SUPPORTED,   SUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, MFM, 300, 40, 1, 18, 256, 180,
      "Atari DOS", "XFD", "", "", "", "", "" },

    { atari_ados_525_dsdd_0360k_xfd, "atari_ados_525_dsdd_0360k_xfd", BRAND_ATARI8BIT, "Atari XF551",
      UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, SUPPORTED,   SUPPORTED,
      MINIDISK_525_INCH, DS, DD, 48, MFM, 300, 40, 2, 18, 256, 360,
      "Atari DOS", "XFD", "", "", "", "", "" },

    { atari_sdos_525_sssd_0090k_xfd, "atari_sdos_525_sssd_0090k_xfd", BRAND_ATARI8BIT, "Atari 810",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   SUPPORTED,
      MINIDISK_525_INCH, SS, SD, 48, FM, 300, 40, 1, 18, 128, 90,
      "SpartaDOS", "XFD", "", "", "", "", "" },

    { atari_sdos_525_ssdd_0130k_xfd, "atari_sdos_525_ssdd_0130k_xfd", BRAND_ATARI8BIT, "Atari 1050",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   SUPPORTED,
      MINIDISK_525_INCH, SS, SD, 48, MFM, 300, 40, 1, 26, 128, 130,
      "SpartaDOS", "XFD", "", "", "", "", "" },

    { atari_sdos_525_ssdd_0180k_xfd, "atari_sdos_525_ssdd_0180k_xfd", BRAND_ATARI8BIT, "Atari 1050",
      UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, SUPPORTED,   SUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, MFM, 300, 40, 1, 18, 256, 180,
      "SpartaDOS", "XFD", "", "", "", "", "" },

    { atari_sdos_525_dsdd_0360k_xfd, "atari_sdos_525_dsdd_0360k_xfd", BRAND_ATARI8BIT, "Atari XF551",
      UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, SUPPORTED,   SUPPORTED,
      MINIDISK_525_INCH, DS, DD, 48, MFM, 300, 40, 2, 18, 256, 360,
      "SpartaDOS", "XFD", "", "", "", "", "" },

    /*****************/
    /* Atari ST - ST */
    /*****************/

    { atarist_fat12_350_ssdd_0360k_st, "atarist_fat12_350_ssdd_0360k_st", BRAND_ATARIST, "SF354 Disk Drive",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, SS, DD, 135, MFM, 300, 80, 1, 9, 512, 360,
      "FAT12", "ST", "", "", "", "", "" },

    { atarist_fat12_350_dsdd_0720k_st, "atarist_fat12_350_dsdd_0720k_st", BRAND_ATARIST, "SF314 Disk Drive",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 135, MFM, 300, 80, 2, 9, 512, 720,
      "FAT12", "ST", "", "", "", "", "" },

    { atarist_fat12_350_dshd_1440k_st, "atarist_fat12_350_dshd_1440k_st", BRAND_ATARIST, "STE, TT, Falcon Int.",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, DS, HD, 135, MFM, 300, 80, 2, 18, 512, 1440,
      "FAT12", "ST", "", "", "", "", "" },

    { atarist_fat12_350_ssdd_0400k_st, "atarist_fat12_350_ssdd_0400k_st", BRAND_ATARIST, "SF354 Disk Drive",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, SS, DD, 135, MFM, 300, 80, 1, 10, 512, 400,
      "FAT12", "ST", "", "", "", "", "" },

    { atarist_fat12_350_ssdd_0440k_st, "atarist_fat12_350_ssdd_0440k_st", BRAND_ATARIST, "SF354 Disk Drive",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, SS, DD, 135, MFM, 300, 80, 1, 11, 512, 440,
      "FAT12", "ST", "", "", "", "", "" },

    { atarist_fat12_350_dsdd_0800k_st, "atarist_fat12_350_dsdd_0800k_st", BRAND_ATARIST, "SF354 Disk Drive",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 135, MFM, 300, 80, 2, 10, 512, 800,
      "FAT12", "ST", "", "", "", "", "" },

    { atarist_fat12_350_dsdd_0880k_st, "atarist_fat12_350_dsdd_0880k_st", BRAND_ATARIST, "SF354 Disk Drive",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 135, MFM, 300, 80, 2, 11, 512, 880,
      "FAT12", "ST", "", "", "", "", "" },

    /******************/
    /* Atari ST - MSA */
    /******************/

    { atarist_fat12_350_ssdd_0360k_msa, "atarist_fat12_350_ssdd_0360k_msa", BRAND_ATARIST, "SF354 Disk Drive",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, SS, DD, 135, MFM, 300, 80, 1, 9, 512, 360,
      "FAT12", "MSA", "", "", "", "", "" },

    { atarist_fat12_350_dsdd_0720k_msa, "atarist_fat12_350_dsdd_0720k_msa", BRAND_ATARIST, "SF314 Disk Drive",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 135, MFM, 300, 80, 2, 9, 512, 720,
      "FAT12", "MSA", "", "", "", "", "" },

    { atarist_fat12_350_dshd_1440k_msa, "atarist_fat12_350_dshd_1440k_msa", BRAND_ATARIST, "STE, TT, Falcon Int.",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, DS, HD, 135, MFM, 300, 80, 2, 18, 512, 1440,
      "FAT12", "MSA", "", "", "", "", "" },

    { atarist_fat12_350_ssdd_0400k_msa, "atarist_fat12_350_ssdd_0400k_msa", BRAND_ATARIST, "SF354 Disk Drive",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, SS, DD, 135, MFM, 300, 80, 1, 10, 512, 400,
      "FAT12", "MSA", "", "", "", "", "" },

    { atarist_fat12_350_ssdd_0440k_msa, "atarist_fat12_350_ssdd_0440k_msa", BRAND_ATARIST, "SF354 Disk Drive",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, SS, DD, 135, MFM, 300, 80, 1, 11, 512, 440,
      "FAT12", "MSA", "", "", "", "", "" },

    { atarist_fat12_350_dsdd_0800k_msa, "atarist_fat12_350_dsdd_0800k_msa", BRAND_ATARIST, "SF354 Disk Drive",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 135, MFM, 300, 80, 2, 10, 512, 800,
      "FAT12", "MSA", "", "", "", "", "" },

    { atarist_fat12_350_dsdd_0880k_msa, "atarist_fat12_350_dsdd_0880k_msa", BRAND_ATARIST, "SF354 Disk Drive",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 135, MFM, 300, 80, 2, 11, 512, 880,
      "FAT12", "MSA", "", "", "", "", "" },

    /******************/
    /* Atari ST - STX */
    /******************/

    { atarist_fat12_350_ssdd_0360k_stx, "atarist_fat12_350_ssdd_0360k_stx", BRAND_ATARIST, "SF354 Disk Drive",
      UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, SS, DD, 135, MFM, 300, 80, 1, 9, 512, 360,
      "FAT12", "STX", "", "", "", "", "" },

    { atarist_fat12_350_dsdd_0720k_stx, "atarist_fat12_350_dsdd_0720k_stx", BRAND_ATARIST, "SF314 Disk Drive",
      UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 135, MFM, 300, 80, 2, 9, 512, 720,
      "FAT12", "STX", "", "", "", "", "" },

    { atarist_fat12_350_dshd_1440k_stx, "atarist_fat12_350_dshd_1440k_stx", BRAND_ATARIST, "STE, TT, Falcon Int.",
      UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, DS, HD, 135, MFM, 300, 80, 2, 18, 512, 1440,
      "FAT12", "STX", "", "", "", "", "" },

    { atarist_fat12_350_ssdd_0400k_stx, "atarist_fat12_350_ssdd_0400k_stx", BRAND_ATARIST, "SF354 Disk Drive",
      UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, SS, DD, 135, MFM, 300, 80, 1, 10, 512, 400,
      "FAT12", "STX", "", "", "", "", "" },

    { atarist_fat12_350_ssdd_0440k_stx, "atarist_fat12_350_ssdd_0440k_stx", BRAND_ATARIST, "SF354 Disk Drive",
      UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, SS, DD, 135, MFM, 300, 80, 1, 11, 512, 440,
      "FAT12", "STX", "", "", "", "", "" },

    { atarist_fat12_350_dsdd_0800k_stx, "atarist_fat12_350_dsdd_0800k_stx", BRAND_ATARIST, "SF354 Disk Drive",
      UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 135, MFM, 300, 80, 2, 10, 512, 800,
      "FAT12", "STX", "", "", "", "", "" },

    { atarist_fat12_350_dsdd_0880k_stx, "atarist_fat12_350_dsdd_0880k_stx", BRAND_ATARIST, "SF354 Disk Drive",
      UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 135, MFM, 300, 80, 2, 11, 512, 880,
      "FAT12", "STX", "", "", "", "", "" },

    /*************/
    /* BBC Micro */
    /*************/

    { bbc_dfs_525_sssd_0100k_ssd, "bbc_dfs_525_sssd_0100k_ssd", BRAND_BBCMICRO, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      MINIDISK_525_INCH, SS, SD, 48, FM, 300, 40, 1, 10, 256, 100,
      "DFS", "SSD", "", "", "", "", "" },

    { bbc_dfs_525_ssqd_0200k_ssd, "bbc_dfs_525_ssqd_0200k_ssd", BRAND_BBCMICRO, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      MINIDISK_525_INCH, SS, QD, 96, FM, 300, 80, 1, 10, 256, 200,
      "DFS", "SSD", "", "", "", "", "" },

    { bbc_dfs_525_dssd_0200k_dsd, "bbc_dfs_525_dssd_0200k_dsd", BRAND_BBCMICRO, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      MINIDISK_525_INCH, DS, SD, 48, FM, 300, 40, 2, 10, 256, 200,
      "DFS", "DSD", "", "", "", "", "" },

    { bbc_dfs_525_dsqd_0400k_dsd, "bbc_dfs_525_dsqd_0400k_dsd", BRAND_BBCMICRO, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      MINIDISK_525_INCH, DS, QD, 96, FM, 300, 80, 2, 10, 256, 400,
      "DFS", "DSD", "", "", "", "", "" },

    { bbc_adfss_525_ssdd_0160k_adl, "bbc_adfss_525_ssdd_0160k_adl", BRAND_BBCMICRO, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, MFM, 300, 40, 1, 16, 256, 160,
      "ADFS S", "ADL", "ADF", "", "", "", "" },

    { bbc_adfsm_525_ssqd_0320k_adl, "bbc_adfsm_525_ssqd_0320k_adl", BRAND_BBCMICRO, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      MINIDISK_525_INCH, SS, QD, 96, MFM, 300, 80, 1, 16, 256, 320,
      "ADFS M", "ADL", "ADF", "", "", "", "" },

    { bbc_adfsl_525_dsqd_0640k_adl, "bbc_adfsl_525_dsqd_0640k_adl", BRAND_BBCMICRO, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      MINIDISK_525_INCH, DS, QD, 96, MFM, 300, 80, 2, 16, 256, 640,
      "ADFS L", "ADL", "ADF", "", "", "", "" },

    /********************/
    /* Acorn Archimedes */
    /********************/

    { arch_adfsd_350_dsdd_0800k_adl, "arch_adfsd_350_dsdd_0800k_adl", BRAND_ARCHIMEDES, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 135, MFM, 300, 80, 2, 5, 1024, 800,
      "ADFS D", "ADL", "ADF", "", "", "", "" },

    { arch_adfse_350_dsdd_0800k_adl, "arch_adfse_350_dsdd_0800k_adl", BRAND_ARCHIMEDES, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 135, MFM, 300, 80, 2, 5, 1024, 800,
      "ADFS E", "ADL", "ADF", "", "", "", "" },

    { arch_adfsf_350_dshd_1600k_adl, "arch_adfsf_350_dshd_1600k_adl", BRAND_ARCHIMEDES, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      DISKETTE_35_INCH, DS, HD, 135, MFM, 300, 80, 2, 10, 1024, 1600,
      "ADFS F", "ADL", "ADF", "", "", "", "" },

    /**********/
    /* MS-DOS */
    /**********/

    { msdos_fat12_525_ssdd_0160k_ima, "msdos_fat12_525_ssdd_0160k_ima", BRAND_PCCOMPAT, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, MFM, 300, 40, 1, 8, 512, 160,
      "FAT12", "IMA", "IMG", "", "", "", "" },

    { msdos_fat12_525_ssdd_0180k_ima, "msdos_fat12_525_ssdd_0180k_ima", BRAND_PCCOMPAT, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, MFM, 300, 40, 1, 9, 512, 180,
      "FAT12", "IMA", "IMG", "", "", "", "" },

    { msdos_fat12_525_dsdd_0320k_ima, "msdos_fat12_525_dsdd_0320k_ima", BRAND_PCCOMPAT, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, DS, DD, 48, MFM, 300, 40, 2, 8, 512, 320,
      "FAT12", "IMA", "IMG", "", "", "", "" },

    { msdos_fat12_525_dsdd_0360k_ima, "msdos_fat12_525_dsdd_0360k_ima", BRAND_PCCOMPAT, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, DS, DD, 48, MFM, 300, 40, 2, 9, 512, 360,
      "FAT12", "IMA", "IMG", "", "", "", "" },

    { msdos_fat12_525_dshd_1200k_ima, "msdos_fat12_525_dshd_1200k_ima", BRAND_PCCOMPAT, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, DS, HD, 96, MFM, 360, 80, 2, 15, 512, 1200,
      "FAT12", "IMA", "IMG", "", "", "", "" },

    { msdos_fat12_350_dsdd_0800k_ima, "msdos_fat12_350_dsdd_0800k_ima", BRAND_PCCOMPAT, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 135, MFM, 300, 80, 2, 10, 512, 800,
      "FAT12", "IMA", "IMG", "", "", "", "" },

    { msdos_fat12_350_dsdd_0720k_ima, "msdos_fat12_350_dsdd_0720k_ima", BRAND_PCCOMPAT, "N/A",
      SUPPORTED,   UNSUPPORTED, SUPPORTED,   SUPPORTED,   UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 135, MFM, 300, 80, 2, 9, 512, 720,
      "FAT12", "IMA", "IMG", "", "", "", "" },

    { msdos_fat12_350_dshd_1440k_ima, "msdos_fat12_350_dshd_1440k_ima", BRAND_PCCOMPAT, "N/A",
      SUPPORTED,   UNSUPPORTED, SUPPORTED,   SUPPORTED,   UNSUPPORTED,
      DISKETTE_35_INCH, DS, HD, 135, MFM, 300, 80, 2, 18, 512, 1440,
      "FAT12", "IMA", "IMG", "", "", "", "" },

    { msdos_fat12_350_dshd_1680k_ima, "msdos_fat12_350_dshd_1680k_ima", BRAND_PCCOMPAT, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      DISKETTE_35_INCH, DS, HD, 135, MFM, 300, 80, 2, 21, 512, 1680,
      "FAT12", "IMA", "IMG", "", "", "", "" },

    { msdos_fat12_350_dshd_2880k_ima, "msdos_fat12_350_dshd_2880k_ima", BRAND_PCCOMPAT, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      DISKETTE_35_INCH, DS, ED, 135, MFM, 300, 80, 2, 36, 512, 2880,
      "FAT12/16", "IMA", "IMG", "", "", "", "" },

    { msdos_fat12_525_ssdd_0160k_imd, "msdos_fat12_525_ssdd_0160k_imd", BRAND_PCCOMPAT, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, MFM, 300, 40, 1, 8, 512, 160,
      "FAT12", "IMD", "", "", "", "", "" },

    { msdos_fat12_525_ssdd_0180k_imd, "msdos_fat12_525_ssdd_0180k_imd", BRAND_PCCOMPAT, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, MFM, 300, 40, 1, 9, 512, 180,
      "FAT12", "IMD", "", "", "", "", "" },

    { msdos_fat12_525_dsdd_0320k_imd, "msdos_fat12_525_dsdd_0320k_imd", BRAND_PCCOMPAT, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, DS, DD, 48, MFM, 300, 40, 2, 8, 512, 320,
      "FAT12", "IMD", "", "", "", "", "" },

    { msdos_fat12_525_dsdd_0360k_imd, "msdos_fat12_525_dsdd_0360k_imd", BRAND_PCCOMPAT, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, DS, DD, 48, MFM, 300, 40, 2, 9, 512, 360,
      "FAT12", "IMD", "", "", "", "", "" },

    { msdos_fat12_525_dshd_1200k_imd, "msdos_fat12_525_dshd_1200k_imd", BRAND_PCCOMPAT, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, DS, HD, 96, MFM, 360, 80, 2, 15, 512, 1200,
      "FAT12", "IMD", "", "", "", "", "" },

    { msdos_fat12_350_dsdd_0800k_imd, "msdos_fat12_350_dsdd_0800k_imd", BRAND_PCCOMPAT, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 135, MFM, 300, 80, 2, 10, 512, 800,
      "FAT12", "IMD", "", "", "", "", "" },

    { msdos_fat12_350_dsdd_0720k_imd, "msdos_fat12_350_dsdd_0720k_imd", BRAND_PCCOMPAT, "N/A",
      SUPPORTED,   UNSUPPORTED, SUPPORTED,   SUPPORTED,   UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 135, MFM, 300, 80, 2, 9, 512, 720,
      "FAT12", "IMD", "", "", "", "", "" },

    { msdos_fat12_350_dshd_1440k_imd, "msdos_fat12_350_dshd_1440k_imd", BRAND_PCCOMPAT, "N/A",
      SUPPORTED,   UNSUPPORTED, SUPPORTED,   SUPPORTED,   UNSUPPORTED,
      DISKETTE_35_INCH, DS, HD, 135, MFM, 300, 80, 2, 18, 512, 1440,
      "FAT12", "IMD", "", "", "", "", "" },

    { msdos_fat12_350_dshd_1680k_imd, "msdos_fat12_350_dshd_1680k_imd", BRAND_PCCOMPAT, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      DISKETTE_35_INCH, DS, HD, 135, MFM, 300, 80, 2, 21, 512, 1680,
      "FAT12", "IMD", "", "", "", "", "" },

    { msdos_fat12_350_dshd_2880k_imd, "msdos_fat12_350_dshd_2880k_imd", BRAND_PCCOMPAT, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      DISKETTE_35_INCH, DS, ED, 135, MFM, 300, 80, 2, 36, 512, 2880,
      "FAT12/16", "IMD", "", "", "", "", "" },

    /**************/
    /* Tandy CoCo */
    /**************/

    { coco_decb_525_ssdd_0158k_dsk, "coco_decb_525_ssdd_0158k_dsk", BRAND_TANDYCOCO, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, MFM, 300, 35, 1, 18, 256, 158, "DECB", 
      "DSK", "", "", "", "", "" },

    { coco_decb_525_ssdd_0180k_dsk, "coco_decb_525_ssdd_0180k_dsk", BRAND_TANDYCOCO, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, MFM, 300, 40, 1, 18, 256, 180, "DECB", 
      "DSK", "", "", "", "", "" },

    { coco_os9_525_ssdd_0180k_dsk, "coco_os9_525_ssdd_0180k_dsk", BRAND_TANDYCOCO, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, MFM, 300, 40, 1, 18, 256, 180, "OS9", 
      "DSK", "", "", "", "", "" },

    { coco_os9_525_dsdd_0360k_dsk, "coco_os9_525_dsdd_0360k_dsk", BRAND_TANDYCOCO, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, DS, DD, 48, MFM, 300, 40, 2, 18, 256, 360, "OS9", 
      "DSK", "", "", "", "", "" },

    { coco_os9_525_ssqd_0360k_dsk, "coco_os9_525_ssqd_0360k_dsk", BRAND_TANDYCOCO, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, SS, QD, 48, MFM, 300, 80, 1, 18, 256, 360, "OS9", 
      "DSK", "", "", "", "", "" },

    { coco_os9_525_dsqd_0720k_dsk, "coco_os9_525_dsqd_0720k_dsk", BRAND_TANDYCOCO, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, DS, QD, 48, MFM, 300, 80, 2, 18, 256, 720, "OS9", 
      "DSK", "", "", "", "", "" },

    { coco_os9_350_ssdd_0360k_dsk, "coco_os9_350_ssdd_0360k_dsk", BRAND_TANDYCOCO, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      DISKETTE_35_INCH, SS, DD, 48, MFM, 300, 80, 1, 18, 256, 360, "OS9", 
      "DSK", "", "", "", "", "" },

    { coco_os9_350_dsdd_0720k_dsk, "coco_os9_350_dsdd_0720k_dsk", BRAND_TANDYCOCO, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      DISKETTE_35_INCH, DS, DD, 48, MFM, 300, 80, 2, 18, 256, 720, "OS9", 
      "DSK", "", "", "", "", "" },

    /********************/
    /* Dragon Computers */
    /********************/

    { dragon_ddos_525_ssdd_0180k_dsk, "dragon_ddos_525_ssdd_0180k_dsk", BRAND_DRAGON, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, MFM, 300, 40, 1, 18, 256, 180, "DragonDOS", 
      "DSK", "", "", "", "", "" },

    { dragon_ddos_525_dsdd_0360k_dsk, "dragon_ddos_525_dsdd_0360k_dsk", BRAND_DRAGON, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, DS, DD, 48, MFM, 300, 40, 2, 18, 256, 360, "DragonDOS", 
      "DSK", "", "", "", "", "" },

    { dragon_ddos_525_ssqd_0360k_dsk, "dragon_ddos_525_ssqd_0360k_dsk", BRAND_DRAGON, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, SS, QD, 48, MFM, 300, 80, 1, 18, 256, 360, "DragonDOS", 
      "DSK", "", "", "", "", "" },

    { dragon_ddos_525_dsqd_0720k_dsk, "dragon_ddos_525_dsqd_0720k_dsk", BRAND_DRAGON, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, DS, QD, 48, MFM, 300, 80, 2, 18, 256, 720, "DragonDOS", 
      "DSK", "", "", "", "", "" },

    { dragon_ddos_525_ssdd_0180k_dmk, "dragon_ddos_525_ssdd_0180k_dmk", BRAND_DRAGON, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, MFM, 300, 40, 1, 18, 256, 180, "DragonDOS", 
      "DMK", "", "", "", "", "" },

    { dragon_ddos_525_dsdd_0360k_dmk, "dragon_ddos_525_dsdd_0360k_dmk", BRAND_DRAGON, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, DS, DD, 48, MFM, 300, 40, 2, 18, 256, 360, "DragonDOS", 
      "DMK", "", "", "", "", "" },

    { dragon_ddos_525_ssqd_0360k_dmk, "dragon_ddos_525_ssqd_0360k_dmk", BRAND_DRAGON, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, SS, QD, 48, MFM, 300, 80, 1, 18, 256, 360, "DragonDOS", 
      "DMK", "", "", "", "", "" },

    { dragon_ddos_525_dsqd_0720k_dmk, "dragon_ddos_525_dsqd_0720k_dmk", BRAND_DRAGON, "N/A",
      SUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, DS, QD, 48, MFM, 300, 80, 2, 18, 256, 720, "DragonDOS", 
      "DMK", "", "", "", "", "" },

    { dragon_ddos_525_ssdd_0180k_vdk, "dragon_ddos_525_ssdd_0180k_vdk", BRAND_DRAGON, "N/A",
      UNSUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, SS, DD, 48, MFM, 300, 40, 1, 18, 256, 180, "DragonDOS", 
      "VDK", "", "", "", "", "" },

    { dragon_ddos_525_dsdd_0360k_vdk, "dragon_ddos_525_dsdd_0360k_vdk", BRAND_DRAGON, "N/A",
      UNSUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, DS, DD, 48, MFM, 300, 40, 2, 18, 256, 360, "DragonDOS", 
      "VDK", "", "", "", "", "" },

    { dragon_ddos_525_ssqd_0360k_vdk, "dragon_ddos_525_ssqd_0360k_vdk", BRAND_DRAGON, "N/A",
      UNSUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, SS, QD, 48, MFM, 300, 80, 1, 18, 256, 360, "DragonDOS", 
      "VDK", "", "", "", "", "" },

    { dragon_ddos_525_dsqd_0720k_vdk, "dragon_ddos_525_dsqd_0720k_vdk", BRAND_DRAGON, "N/A",
      UNSUPPORTED,   UNSUPPORTED, UNSUPPORTED, SUPPORTED,   UNSUPPORTED,
      MINIDISK_525_INCH, DS, QD, 48, MFM, 300, 80, 2, 18, 256, 720, "DragonDOS", 
      "VDK", "", "", "", "", "" },

    /*************************/
    /* Olivetti M20 Computer */
    /*************************/

    { olivettim20_pcos_525_dsdd_0280_img, "olivettim20_pcos_525_dsdd_0280_img", BRAND_OLIVETTI, "N/A",
      SUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      MINIDISK_525_INCH, DS, DD, 48, MFM, 300, 35, 2, 16, 256, 280, "PCOS", 
      "IMG", "", "", "", "", "" },

    { olivettim20_pcos_525_dsdd_0280_imd, "olivettim20_pcos_525_dsdd_0280_imd", BRAND_OLIVETTI, "N/A",
      SUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED, UNSUPPORTED,
      MINIDISK_525_INCH, DS, DD, 48, MFM, 300, 35, 2, 16, 256, 280, "PCOS", 
      "IMD", "", "", "", "", "" },

};/*end-array*/

/******************************************************************************/
// format_table[] - Master disk image format reference table.
//
//  { format_id, format_id_string, format_brand, assoc_model_string,
//    gw_supported, zf_supported, usb_supported, kf_supported, sio_supported,
//    size_inches,  sides, density, encoding, rpm, tracks, heads, sectors, block, size_kb, filesystem_format
//    file_ext, alt1, alt2, alt3, alt4, alt5 },
//
/******************************************************************************/
  
/******************************************************************************/
/******************************************************************************/
/*                        ASSERTS FOR SAFETY CHECKING                         */
/*                                                                            */
/*      Safety checks: ensure enum COUNTs stay in sync with their arrays      */
/******************************************************************************/
/******************************************************************************/

_Static_assert(ARRAY_LEN(brand_options)                 == BRAND_COUNT,         "brand_options out of sync with brand enum");

/******************************************************************************/
       
_Static_assert(ARRAY_LEN(amiga_systems_options)         == AMIGA_COUNT,         "amiga_systems_options out of sync with amiga_system enum"           );
_Static_assert(ARRAY_LEN(appleii_systems_options)       == APPLEII_COUNT,       "appleii_systems_options out of sync with appleii_system enum"       );
_Static_assert(ARRAY_LEN(archimedes_systems_options)    == ARCHIMEDES_COUNT,    "archimedes_systems_options out of sync with archimedes_system enum" );
_Static_assert(ARRAY_LEN(atari8bit_systems_options)     == ATARI8_COUNT,        "atari8bit_systems_options out of sync with atari8bit_system enum"   );
_Static_assert(ARRAY_LEN(atarist_systems_options)       == ATARIST_COUNT,       "atarist_systems_options out of sync with atari8bit_system enum"     );
_Static_assert(ARRAY_LEN(bbcmicro_systems_options)      == BBC_COUNT,           "bbcmicro_systems_options out of sync with bbcmicro_system enum"     );
_Static_assert(ARRAY_LEN(commodore_systems_options)     == CBM_COUNT,           "commodore_systems_options out of sync with commodore_system enum"   );
_Static_assert(ARRAY_LEN(macintosh_systems_options)     == MAC_COUNT,           "macintosh_systems_options out of sync with macintosh_system enum"   );
_Static_assert(ARRAY_LEN(pccompatibles_systems_options) == PC_COUNT,            "pccompatibles_systems_options out of sync with pccompat_system enum");
_Static_assert(ARRAY_LEN(tandycoco_systems_options)     == TANDYCOCO_COUNT,     "tandycoco_systems_options out of sync with tandycoco_system enum"   );
_Static_assert(ARRAY_LEN(dragon_systems_options)        == DRAGON_COUNT,        "dragon_systems_options out of sync with tandycoco_system enum"      );
_Static_assert(ARRAY_LEN(olivetti_systems_options)      == OLIVETTI_COUNT,      "olivetti_system_options out of sync with tandycoco_system enum"     );

_Static_assert(ARRAY_LEN(flux_systems_options)          == FLUX_COUNT,          "flux_systems_options out of sync with flux_mode enum"               );

/******************************************************************************/

_Static_assert(ARRAY_LEN(amiga_drive_options)           == AMIGA_DRIVE_COUNT,     "amiga_drive_options out of sync"        );
_Static_assert(ARRAY_LEN(appleii_drive_options)         == APPLEII_DRIVE_COUNT,   "appleii_drive_options out of sync"      );
_Static_assert(ARRAY_LEN(atari8bit_drive_options)       == ATARI8_DRIVE_COUNT,    "atari8bit_drive_options out of sync"    );
_Static_assert(ARRAY_LEN(atarist_drive_options)         == ATARIST_DRIVE_COUNT,   "atarist_drive_options out of sync"      );
_Static_assert(ARRAY_LEN(bbcmicro_drive_options)        == BBC_DRIVE_COUNT,       "bbcmicro_drive_options out of sync"     );
_Static_assert(ARRAY_LEN(commodore_drive_options)       == CBM_DRIVE_COUNT,       "commodore_drive_options out of sync"    );
_Static_assert(ARRAY_LEN(macintosh_drive_options)       == MAC_DRIVE_COUNT,       "macintosh_drive_options out of sync"    );
_Static_assert(ARRAY_LEN(pccompatibles_drive_options)   == PC_DRIVE_COUNT,        "pccompatibles_drive_options out of sync");
_Static_assert(ARRAY_LEN(tandycoco_drive_options)       == TANDYCOCO_DRIVE_COUNT, "tandycoco_drive_options out of sync"    );
_Static_assert(ARRAY_LEN(dragon_drive_options)          == DRAGON_DRIVE_COUNT,    "dragon_drive_options out of sync"       );
_Static_assert(ARRAY_LEN(olivetti_drive_options)        == OLIVETTI_DRIVE_COUNT,  "olivetti_drive_options out of sync"     );

_Static_assert(ARRAY_LEN(fluxreadwrite_drive_options)   == FLUX_RW_DRIVE_COUNT,  "fluxreadwrite_drive_options out of sync");
_Static_assert(ARRAY_LEN(fluxreadonly_drive_options)    == FLUX_RO_DRIVE_COUNT,  "fluxreadonly_drive_options out of sync" );

/******************************************************************************/

/******************************************************************/
/******************************************************************/
/*                      SET_CURRENT_OPTIONS                       */
/******************************************************************/
/******************************************************************/

// TODO: This should be a real function. Instead this is a define macro so I can fuck with global variables like an animal! :-)

#define set_current_options( given_brand ) \
    switch ( given_brand ) { \
        \
        case BRAND_AMIGA :  \
            brand_selected           = BRAND_AMIGA; \
            current_computer_options = amiga_systems_options; \
            current_computer_count   = AMIGA_COUNT; \
            current_drive_options    = amiga_drive_options; \
            current_drive_count      = AMIGA_DRIVE_COUNT; \
        break; \
        \
        case BRAND_APPLEII :  \
            brand_selected           = BRAND_APPLEII; \
            current_computer_options = appleii_systems_options; \
            current_computer_count   = APPLEII_COUNT; \
            current_drive_options    = appleii_drive_options; \
            current_drive_count      = APPLEII_DRIVE_COUNT; \
        break; \
        \
        case BRAND_ARCHIMEDES :  \
            brand_selected           = BRAND_ARCHIMEDES; \
            current_computer_options = archimedes_systems_options; \
            current_computer_count   = ARCHIMEDES_COUNT; \
            current_drive_options    = archimedes_drive_options; \
            current_drive_count      = ARCHIMEDES_DRIVE_COUNT; \
        break; \
        \
        case BRAND_ATARI8BIT :  \
            brand_selected           = BRAND_ATARI8BIT; \
            current_computer_options = atari8bit_systems_options; \
            current_computer_count   = ATARI8_COUNT; \
            current_drive_options    = atari8bit_drive_options; \
            current_drive_count      = ATARI8_DRIVE_COUNT; \
        break; \
        \
        case BRAND_ATARIST :  \
            brand_selected           = BRAND_ATARIST; \
            current_computer_options = atarist_systems_options; \
            current_computer_count   = ATARIST_COUNT; \
            current_drive_options    = atarist_drive_options; \
            current_drive_count      = ATARIST_DRIVE_COUNT; \
        break; \
        \
        case BRAND_BBCMICRO :  \
            brand_selected           = BRAND_BBCMICRO; \
            current_computer_options = bbcmicro_systems_options; \
            current_computer_count   = BBC_COUNT; \
            current_drive_options    = bbcmicro_drive_options; \
            current_drive_count      = BBC_DRIVE_COUNT; \
        break; \
        \
        case BRAND_COMMODORE :  \
            brand_selected           = BRAND_COMMODORE; \
            current_computer_options = commodore_systems_options; \
            current_computer_count   = CBM_COUNT; \
            current_drive_options    = commodore_drive_options; \
            current_drive_count      = CBM_DRIVE_COUNT; \
        break; \
        \
        case BRAND_MACINTOSH :  \
            brand_selected           = BRAND_MACINTOSH; \
            current_computer_options = macintosh_systems_options; \
            current_computer_count   = MAC_COUNT; \
            current_drive_options    = macintosh_drive_options; \
            current_drive_count      = MAC_DRIVE_COUNT; \
        break; \
        \
        case BRAND_PCCOMPAT :  \
            brand_selected           = BRAND_PCCOMPAT; \
            current_computer_options = pccompatibles_systems_options; \
            current_computer_count   = PC_COUNT; \
            current_drive_options    = pccompatibles_drive_options; \
            current_drive_count      = PC_DRIVE_COUNT; \
        break; \
        \
        case BRAND_TANDYCOCO : \
            brand_selected           = BRAND_TANDYCOCO; \
            current_computer_options = tandycoco_systems_options; \
            current_computer_count   = TANDYCOCO_COUNT; \
            current_drive_options    = tandycoco_drive_options; \
            current_drive_count      = TANDYCOCO_DRIVE_COUNT; \
        break; \
        \
        case BRAND_DRAGON : \
            brand_selected           = BRAND_DRAGON; \
            current_computer_options = dragon_systems_options; \
            current_computer_count   = DRAGON_COUNT; \
            current_drive_options    = dragon_drive_options; \
            current_drive_count      = DRAGON_DRIVE_COUNT; \
        break; \
        \
        case BRAND_OLIVETTI : \
            brand_selected           = BRAND_OLIVETTI; \
            current_computer_options = olivetti_systems_options; \
            current_computer_count   = OLIVETTI_COUNT; \
            current_drive_options    = olivetti_drive_options; \
            current_drive_count      = OLIVETTI_DRIVE_COUNT; \
        break; \
        \
        case BRAND_FLUX : /* Do nothing. */ break; /* This surpresses a compiler warning about unused stuff. */ \
        case BRAND_COUNT : /* Do nothing. */ break; /* This surpresses a compiler warning about unused stuff. */ \
    };/*end-switch*/ \
/*end-define*/

/******************************************************************/
/******************************************************************/
/*                       DEFAULT COMPUTERS                        */
/******************************************************************/
/******************************************************************/

// TODO: This should be a real function. Instead this is a define macro so I can fuck with global variables like an animal! :-)

#define set_to_default_system( given_brand ) \
  switch (brand_selected) { \
      case BRAND_AMIGA:      computer_selected = AMIGA_500;         drive_selected = default_drive_amiga[      computer_selected ];break; \
      case BRAND_APPLEII:    computer_selected = APPLE_IIE;         drive_selected = default_drive_appleii[    computer_selected ];break; \
      case BRAND_ARCHIMEDES: computer_selected = ARCHIMEDES_A310;   drive_selected = default_drive_archimedes[ computer_selected ];break; \
      case BRAND_ATARI8BIT:  computer_selected = ATARI8_800XL;      drive_selected = default_drive_atari8[     computer_selected ];break; \
      case BRAND_ATARIST:    computer_selected = ATARIST_1040STF;   drive_selected = default_drive_atarist[    computer_selected ];break; \
      case BRAND_BBCMICRO:   computer_selected = BBC_MICRO_B;       drive_selected = default_drive_bbcmicro[   computer_selected ];break; \
      case BRAND_COMMODORE:  computer_selected = CBM_64;            drive_selected = default_drive_commodore[  computer_selected ];break; \
      case BRAND_MACINTOSH:  computer_selected = MAC_PLUS;          drive_selected = default_drive_macintosh[  computer_selected ];break; \
      case BRAND_PCCOMPAT:   computer_selected = PC_8088;           drive_selected = default_drive_pccompat[   computer_selected ];break; \
      case BRAND_TANDYCOCO:  computer_selected = TANDY_COCO1;       drive_selected = default_drive_tandycoco[  computer_selected ];break; \
      case BRAND_DRAGON:     computer_selected = DRAGON_32;         drive_selected = default_drive_dragon[     computer_selected ];break; \
      case BRAND_OLIVETTI:   computer_selected = OLIVETTI_M20;      drive_selected = default_drive_olivetti[   computer_selected ];break; \
      case BRAND_FLUX:       computer_selected = FLUX_RW_DRIVE_SCP; drive_selected = default_drive_flux[       computer_selected ];break; \
  };/*end-switch*/ \
  set_current_options( given_brand ); \
/*end-define*/

/**********************************************/
/**********************************************/
// Default Drives (Drive Defaults) For Computers
/**********************************************/
/**********************************************/

static const int default_drive_amiga[AMIGA_COUNT] = {
    /* AMIGA_1000  */ AMIGA_DRIVE_1010,
    /* AMIGA_500   */ AMIGA_DRIVE_1010,
    // /* AMIGA_500P  */ AMIGA_DRIVE_1010,
    /* AMIGA_600   */ AMIGA_DRIVE_1010,
    /* AMIGA_1200  */ AMIGA_DRIVE_1010,
    /* AMIGA_1500  */ AMIGA_DRIVE_1010,
    /* AMIGA_2000  */ AMIGA_DRIVE_1010,
    /* AMIGA_2500  */ AMIGA_DRIVE_1010,
    /* AMIGA_3000  */ AMIGA_DRIVE_1010,
    // /* AMIGA_3000T */ AMIGA_DRIVE_1010,
    /* AMIGA_4000  */ AMIGA_DRIVE_A4K,
    // /* AMIGA_4000T */ AMIGA_DRIVE_A4K,
    /* AMIGA_CDTV  */ AMIGA_DRIVE_1010,
    /* AMIGA_CD32  */ AMIGA_DRIVE_1010
};/*end-array*/

// static const int default_drive_appleii[ APPLEII_COUNT ] = {
//     /* APPLE_II         */ APPLEII_DRIVE_525_APPDOS_DSK,
//     /* APPLE_IIPLUS     */ APPLEII_DRIVE_525_APPDOS_DSK,
//     // /* APPLE_EUROPLUS   */ APPLEII_DRIVE_525_APPDOS_DSK,
//     // /* APPLE_JPLUS      */ APPLEII_DRIVE_525_APPDOS_DSK,
//     /* APPLE_BELLHOWELL */ APPLEII_DRIVE_525_APPDOS_DSK,
//     /* APPLE_IIE        */ APPLEII_DRIVE_525_PRODOS_DSK,
//     /* APPLE_IIE_ENH    */ APPLEII_DRIVE_525_PRODOS_DSK,
//     /* APPLE_IIE_PLAT   */ APPLEII_DRIVE_525_PRODOS_DSK,
//     /* APPLE_IIC        */ APPLEII_DRIVE_525_PRODOS_DSK,
//     /* APPLE_IIC_PLUS   */ APPLEII_DRIVE_35_PD16_2MG,
//     /* APPLE_IIGS       */ APPLEII_DRIVE_35_PD16_2MG
// };/*end-array*/

static const int default_drive_appleii[ APPLEII_COUNT ] = {
     // [ APPLE_1          ] = APPLEII_DRIVE_525_APPDOS_DSK,        
     [ APPLE_II         ] = APPLEII_DRIVE_525_APPDOS_DSK,        
     [ APPLE_IIPLUS     ] = APPLEII_DRIVE_525_APPDOS_DSK,        
     [ APPLE_BELLHOWELL ] = APPLEII_DRIVE_525_APPDOS_DSK,        
     [ APPLE_IIE        ] = APPLEII_DRIVE_525_PRODOS_DSK,        
     [ APPLE_IIE_ENH    ] = APPLEII_DRIVE_525_PRODOS_DSK,        
     [ APPLE_IIE_PLAT   ] = APPLEII_DRIVE_525_PRODOS_DSK,        
     [ APPLE_IIC        ] = APPLEII_DRIVE_525_PRODOS_DSK,        
     [ APPLE_IIC_PLUS   ] = APPLEII_DRIVE_35_PD16_2MG,        
     [ APPLE_IIGS       ] = APPLEII_DRIVE_35_PD16_2MG,        
     [ APPLE_III        ] = APPLEII_DRIVE_525_APPDOS_DSK         
};/*end-array*/

static const int default_drive_archimedes[ ARCHIMEDES_COUNT ] = {
    /* ARCHIMEDES_A310  */ ARCHIMEDES_DRIVE_DD_D,
    /* ARCHIMEDES_A5000 */ ARCHIMEDES_DRIVE_HD,
};/*end-array*/

static const int default_drive_atari8[ ATARI8_COUNT ] = {
    /* ATARI8_400    */ ATARI8_DRIVE_810,
    /* ATARI8_800    */ ATARI8_DRIVE_810,
    /* ATARI8_1200XL */ ATARI8_DRIVE_1050_DD130,
    /* ATARI8_600XL  */ ATARI8_DRIVE_1050_DD130,
    /* ATARI8_800XL  */ ATARI8_DRIVE_1050_DD130,
    /* ATARI8_65XE   */ ATARI8_DRIVE_XF551,
    /* ATARI8_130XE  */ ATARI8_DRIVE_XF551,
    /* ATARI8_800XE  */ ATARI8_DRIVE_XF551,
    /* ATARI8_XEGS   */ ATARI8_DRIVE_XF551
};/*end-array*/

static const int default_drive_atarist[ ATARIST_COUNT ] = {
    /* ATARIST_520ST     */ ATARIST_SF354_SSDD_360K_ST, 
    /* ATARIST_1040ST    */ ATARIST_SF314_DSDD_720K_ST,
    /* ATARIST_520ST_FM  */ ATARIST_SF354_SSDD_360K_ST,
    /* ATARIST_1040STF   */ ATARIST_SF314_DSDD_720K_ST,
    /* ATARIST_520STF    */ ATARIST_SF314_DSDD_720K_ST,
    /* ATARIST_MEGA_ST_1 */ ATARIST_SF314_DSDD_720K_ST,
    // /* ATARIST_MEGA_ST_2 */ ATARIST_SF314_DSDD_720K_ST,
    // /* ATARIST_MEGA_ST_4 */ ATARIST_SF314_DSDD_720K_ST,
    /* ATARIST_1040STE   */ ATARIST_SF314_DSDD_720K_ST,
    /* ATARIST_520STE    */ ATARIST_SF314_DSDD_720K_ST,
    /* ATARIST_MEGA_STE  */ ATARIST_SF314_DSDD_720K_ST,
    /* ATARIST_TT030     */ ATARIST_HDINT_DSHD_1_4M_ST,
    /* ATARIST_FALCON030 */ ATARIST_HDINT_DSHD_1_4M_ST
};/*end-array*/

static const int default_drive_bbcmicro[ BBC_COUNT ] = {
    /* BBC_MICRO_A        */ BBC_DRIVE_525_SSSD_100_SSD,
    /* BBC_MICRO_B        */ BBC_DRIVE_525_SSQD_200_SSD,
    /* BBC_MICRO_BP64     */ BBC_DRIVE_525_SSQD_200_SSD,
    /* BBC_MICRO_BP128    */ BBC_DRIVE_525_SSQD_200_SSD,
    /* BBC_MASTER_128     */ BBC_DRIVE_525_DSQD_640_ADL,
    /* BBC_MASTER_TURBO   */ BBC_DRIVE_525_DSQD_640_ADL,
    /* BBC_MASTER_512     */ BBC_DRIVE_525_DSQD_640_ADL,
    /* BBC_MASTER_ET      */ BBC_DRIVE_525_DSQD_640_ADL,
    /* BBC_MASTER_COMPACT */ BBC_DRIVE_525_DSQD_640_ADL,
    /* ACORN_ELECTRON     */ BBC_DRIVE_525_DSQD_640_ADL
};/*end-array*/

static const int default_drive_commodore[ CBM_COUNT ] = {
    /* CBM_PET_2001 */ CBM_DRIVE_2040,
    /* CBM_PET_3008 */ CBM_DRIVE_3040,
    // /* CBM_PET_3016 */ CBM_DRIVE_3040,
    // /* CBM_PET_3032 */ CBM_DRIVE_3040,
    /* CBM_PET_4016 */ CBM_DRIVE_4040,
    /* CBM_PET_4032 */ CBM_DRIVE_4040,
    /* CBM_PET_8032 */ CBM_DRIVE_8050, /* TODO: These are done by order. But they should be done using the system name. I justed the order and had to change this when I shouldn't have to. Need to update this. */
    /* CBM_SUPERPET */ CBM_DRIVE_8250,
    /* CBM_PET_8096 */ CBM_DRIVE_SFD1001,
    /* CBM_VIC20    */ CBM_DRIVE_1541,
    /* CBM_VIC1001  */ CBM_DRIVE_1541,
    /* CBM_64       */ CBM_DRIVE_1541,
    /* CBM_64C      */ CBM_DRIVE_1541II,
    /* CBM_C64GS    */ CBM_DRIVE_1541II,
    /* CBM_SX64     */ CBM_DRIVE_1541II,
    /* CBM_116      */ CBM_DRIVE_1551,
    /* CBM_16       */ CBM_DRIVE_1551,
    /* CBM_PLUS4    */ CBM_DRIVE_1551,
    /* CBM_128      */ CBM_DRIVE_1571,
    /* CBM_128D     */ CBM_DRIVE_1571
    // /* CBM_128DCR   */ CBM_DRIVE_1571
};/*end-array*/

static const int default_drive_macintosh[ MAC_COUNT ] = {
    /* MAC_128K             */ MAC_DRIVE_35_SS_400K,
    /* MAC_512K             */ MAC_DRIVE_35_DS_800K,
    /* MAC_512KE            */ MAC_DRIVE_35_DS_800K,
    /* MAC_PLUS             */ MAC_DRIVE_35_DS_800K,
    /* MAC_SE               */ MAC_DRIVE_35_DS_800K,
    /* MAC_SE30             */ MAC_DRIVE_35_DS_1M4,
    /* MAC_CLASSIC          */ MAC_DRIVE_35_DS_1M4,
    /* MAC_CLASSIC_II       */ MAC_DRIVE_35_DS_1M4,
    // /* MAC_COLOR_CLASSIC    */ MAC_DRIVE_35_DS_1M4,
    // /* MAC_COLOR_CLASSIC_II */ MAC_DRIVE_35_DS_1M4,
    /* MAC_PORTABLE         */ MAC_DRIVE_35_DS_1M4
};/*end-array*/
 
static const int default_drive_pccompat[ PC_COUNT ] = {
    /* PC_8088  */ PC_DRIVE_525_DD_360_IMA,
    /* PC_80286 */ PC_DRIVE_525_HD_1M2_IMA,
    /* PC_80386 */ PC_DRIVE_35_DD_720_IMA,
    /* PC_80486 */ PC_DRIVE_35_HD_1M4_IMA
};/*end-array*/

static const int default_drive_tandycoco[ TANDYCOCO_COUNT ] = {
    /* TANDY_COCO1 */ TANDYCOCO_525_SSDD_158_DECB_DSK,
    /* TANDY_COCO2 */ TANDYCOCO_525_SSDD_180_DECB_DSK,
    /* TANDY_COCO3 */ TANDYCOCO_525_DSQD_720_OS9_DSK,
};/*end-array*/

static const int default_drive_dragon[ DRAGON_COUNT ] = {
    /* DRAGON_32   */ DRAGON_DDOS_525_SSDD_0180K_DSK,
    /* DRAGON_64   */ DRAGON_DDOS_525_DSDD_0360K_DSK,
    /* DRAGON_TANO */ DRAGON_DDOS_525_DSQD_0720K_DSK,
};/*end-array*/

static const int default_drive_olivetti[ OLIVETTI_COUNT ] = {
    /* OLIVETTI_M20 */ OLIVETTIM20_PCOS_525_DSDD_0280K_IMG,
};/*end-array*/

static const int default_drive_flux[ FLUX_COUNT ] = {
    /* FLUX_READWRITE  */ FLUX_RW_DRIVE_SCP,
    /* FLUX_READONLY   */ FLUX_RO_DRIVE_IPF
};/*end-array*/

/******************************************************************/

#endif /* MAIN_FORMAT_REFERENCE_H */