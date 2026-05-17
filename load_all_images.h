/*********************************************************************

load_all_images.h

This is for loading all the images. 

*********************************************************************/

#ifndef LOAD_ALL_IMAGES_H
#define LOAD_ALL_IMAGES_H

/********************************************************************************/
// Generic Images for Other Stuff  
/********************************************************************************/  

struct nk_image about_box_image = load_image(argv[0], "assets/images/application/about_graphic.bmp");

struct nk_image diskimagefile_load_exclamationmark     = load_image(argv[0], "assets/images/files/diskimagefile_load_questionmark.png");
struct nk_image diskimagefile_save_exclamationmark     = load_image(argv[0], "assets/images/files/diskimagefile_save_questionmark.png");

struct nk_image diskimagefile_load_questionmark        = load_image(argv[0], "assets/images/files/diskimagefile_load_questionmark.png");
struct nk_image diskimagefile_save_questionmark        = load_image(argv[0], "assets/images/files/diskimagefile_save_questionmark.png");

struct nk_image diskimagefile_load_350ss               = load_image(argv[0], "assets/images/files/diskimagefile_load_350ss.png");
struct nk_image diskimagefile_load_350dd               = load_image(argv[0], "assets/images/files/diskimagefile_load_350dd.png");
struct nk_image diskimagefile_load_350hd               = load_image(argv[0], "assets/images/files/diskimagefile_load_350hd.png");
struct nk_image diskimagefile_save_350ss               = load_image(argv[0], "assets/images/files/diskimagefile_save_350ss.png");
struct nk_image diskimagefile_save_350dd               = load_image(argv[0], "assets/images/files/diskimagefile_save_350dd.png");
struct nk_image diskimagefile_save_350hd               = load_image(argv[0], "assets/images/files/diskimagefile_save_350hd.png");

struct nk_image diskimagefile_load_525sd               = load_image(argv[0], "assets/images/files/diskimagefile_load_525sd.png");
struct nk_image diskimagefile_load_525dd               = load_image(argv[0], "assets/images/files/diskimagefile_load_525dd.png");
struct nk_image diskimagefile_load_525qd               = load_image(argv[0], "assets/images/files/diskimagefile_load_525qd.png");
struct nk_image diskimagefile_load_525hd               = load_image(argv[0], "assets/images/files/diskimagefile_load_525hd.png");
struct nk_image diskimagefile_save_525sd               = load_image(argv[0], "assets/images/files/diskimagefile_save_525sd.png");
struct nk_image diskimagefile_save_525dd               = load_image(argv[0], "assets/images/files/diskimagefile_save_525dd.png");
struct nk_image diskimagefile_save_525qd               = load_image(argv[0], "assets/images/files/diskimagefile_save_525qd.png");
struct nk_image diskimagefile_save_525hd               = load_image(argv[0], "assets/images/files/diskimagefile_save_525hd.png");

struct nk_image transfer_file2floppy_350ss             = load_image(argv[0], "assets/images/disks/transfer_file2floppy_350ss.png");
struct nk_image transfer_file2floppy_350dd             = load_image(argv[0], "assets/images/disks/transfer_file2floppy_350dd.png");
struct nk_image transfer_file2floppy_350hd             = load_image(argv[0], "assets/images/disks/transfer_file2floppy_350hd.png");
struct nk_image transfer_floppy2file_350ss             = load_image(argv[0], "assets/images/disks/transfer_floppy2file_350ss.png");
struct nk_image transfer_floppy2file_350dd             = load_image(argv[0], "assets/images/disks/transfer_floppy2file_350dd.png");
struct nk_image transfer_floppy2file_350hd             = load_image(argv[0], "assets/images/disks/transfer_floppy2file_350hd.png");

struct nk_image transfer_file2floppy_525sd             = load_image(argv[0], "assets/images/disks/transfer_file2floppy_525sd.png");
struct nk_image transfer_file2floppy_525dd             = load_image(argv[0], "assets/images/disks/transfer_file2floppy_525dd.png");
struct nk_image transfer_file2floppy_525qd             = load_image(argv[0], "assets/images/disks/transfer_file2floppy_525qd.png");
struct nk_image transfer_file2floppy_525hd             = load_image(argv[0], "assets/images/disks/transfer_file2floppy_525hd.png");
struct nk_image transfer_floppy2file_525sd             = load_image(argv[0], "assets/images/disks/transfer_floppy2file_525sd.png");
struct nk_image transfer_floppy2file_525dd             = load_image(argv[0], "assets/images/disks/transfer_floppy2file_525dd.png");
struct nk_image transfer_floppy2file_525qd             = load_image(argv[0], "assets/images/disks/transfer_floppy2file_525qd.png");
struct nk_image transfer_floppy2file_525hd             = load_image(argv[0], "assets/images/disks/transfer_floppy2file_525hd.png");

struct nk_image hardware_greaseweazle_525DD_with_twist = load_image(argv[0], "assets/images/interfaces/hardware_greaseweazle_525DD_with_twist.png");
struct nk_image hardware_greaseweazle_525HD_with_twist = load_image(argv[0], "assets/images/interfaces/hardware_greaseweazle_525HD_with_twist.png");
struct nk_image hardware_greaseweazle_350DD_with_twist = load_image(argv[0], "assets/images/interfaces/hardware_greaseweazle_350DD_with_twist.png");
struct nk_image hardware_greaseweazle_350HD_with_twist = load_image(argv[0], "assets/images/interfaces/hardware_greaseweazle_350HD_with_twist.png");

struct nk_image hardware_greaseweazle_525DD_no_twist   = load_image(argv[0], "assets/images/interfaces/hardware_greaseweazle_525DD_no_twist.png");
struct nk_image hardware_greaseweazle_525HD_no_twist   = load_image(argv[0], "assets/images/interfaces/hardware_greaseweazle_525HD_no_twist.png");
struct nk_image hardware_greaseweazle_350DD_no_twist   = load_image(argv[0], "assets/images/interfaces/hardware_greaseweazle_350DD_no_twist.png");
struct nk_image hardware_greaseweazle_350HD_no_twist   = load_image(argv[0], "assets/images/interfaces/hardware_greaseweazle_350HD_no_twist.png");

struct nk_image hardware_zoomfloppy_1541               = load_image(argv[0], "assets/images/interfaces/hardware_zoomfloppy_1541.png");
struct nk_image hardware_zoomfloppy_1541II             = load_image(argv[0], "assets/images/interfaces/hardware_zoomfloppy_1541II.png");
struct nk_image hardware_zoomfloppy_1570               = load_image(argv[0], "assets/images/interfaces/hardware_zoomfloppy_1570.png");
struct nk_image hardware_zoomfloppy_1571               = load_image(argv[0], "assets/images/interfaces/hardware_zoomfloppy_1571.png");
struct nk_image hardware_zoomfloppy_1581               = load_image(argv[0], "assets/images/interfaces/hardware_zoomfloppy_1581.png");
struct nk_image hardware_zoomfloppy_sfd1001            = load_image(argv[0], "assets/images/interfaces/hardware_zoomfloppy_sfd1001.png");    
struct nk_image hardware_zoomfloppy_2031               = load_image(argv[0], "assets/images/interfaces/hardware_zoomfloppy_2031.png");
struct nk_image hardware_zoomfloppy_4031               = load_image(argv[0], "assets/images/interfaces/hardware_zoomfloppy_4031.png");
struct nk_image hardware_zoomfloppy_2040               = load_image(argv[0], "assets/images/interfaces/hardware_zoomfloppy_2040.png");
struct nk_image hardware_zoomfloppy_3040               = load_image(argv[0], "assets/images/interfaces/hardware_zoomfloppy_3040.png");
struct nk_image hardware_zoomfloppy_4040               = load_image(argv[0], "assets/images/interfaces/hardware_zoomfloppy_4040.png");
struct nk_image hardware_zoomfloppy_8050               = load_image(argv[0], "assets/images/interfaces/hardware_zoomfloppy_8050.png");
struct nk_image hardware_zoomfloppy_8250               = load_image(argv[0], "assets/images/interfaces/hardware_zoomfloppy_8250.png");

struct nk_image hardware_usbfloppy                     = load_image(argv[0], "assets/images/interfaces/hardware_usbfloppy.png" );

/********************************************************************************/
// Images - Systems
/********************************************************************************/

// Array of pointers to Nuklear images for: Systems - Amiga
struct nk_image images_systems_amiga[AMIGA_COUNT];
// Load images for: Systems - Amiga
images_systems_amiga[ AMIGA_1000  ] = load_image(argv[0], "assets/images/systems/AMIGA_1000.png" );
images_systems_amiga[ AMIGA_500   ] = load_image(argv[0], "assets/images/systems/AMIGA_500.png"  );
// images_systems_amiga[ AMIGA_500P  ] = load_image(argv[0], "assets/images/systems/AMIGA_500.png"  );  // Basically looks the same. 
images_systems_amiga[ AMIGA_600   ] = load_image(argv[0], "assets/images/systems/AMIGA_600.png"  );
images_systems_amiga[ AMIGA_1200  ] = load_image(argv[0], "assets/images/systems/AMIGA_1200.png" );
images_systems_amiga[ AMIGA_1500  ] = load_image(argv[0], "assets/images/systems/AMIGA_1500.png" ); 
images_systems_amiga[ AMIGA_2000  ] = load_image(argv[0], "assets/images/systems/AMIGA_2000.png" );
images_systems_amiga[ AMIGA_2500  ] = load_image(argv[0], "assets/images/systems/AMIGA_2500.png" );
images_systems_amiga[ AMIGA_3000  ] = load_image(argv[0], "assets/images/systems/AMIGA_3000.png" );
// images_systems_amiga[ AMIGA_3000T ] = load_image(argv[0], "assets/images/systems/AMIGA_3000T.png");
images_systems_amiga[ AMIGA_4000  ] = load_image(argv[0], "assets/images/systems/AMIGA_4000.png" );
// images_systems_amiga[ AMIGA_4000T ] = load_image(argv[0], "assets/images/systems/AMIGA_4000T.png");
images_systems_amiga[ AMIGA_CDTV  ] = load_image(argv[0], "assets/images/systems/AMIGA_CDTV.png" );
images_systems_amiga[ AMIGA_CD32  ] = load_image(argv[0], "assets/images/systems/AMIGA_CD32.png" );

// Array of pointers to Nuklear images for: Systems - Apple II
struct nk_image images_systems_appleii[APPLEII_COUNT];
// Load images for: Systems - Apple II
// images_systems_appleii[ APPLE_1          ] = load_image( argv[0] , "assets/images/systems/APPLE_1.png"          );
images_systems_appleii[ APPLE_II         ] = load_image( argv[0] , "assets/images/systems/APPLE_II.png"         );
images_systems_appleii[ APPLE_IIPLUS     ] = load_image( argv[0] , "assets/images/systems/APPLE_IIPLUS.png"     );
images_systems_appleii[ APPLE_BELLHOWELL ] = load_image( argv[0] , "assets/images/systems/APPLE_BELLHOWELL.png" );
images_systems_appleii[ APPLE_IIE        ] = load_image( argv[0] , "assets/images/systems/APPLE_IIE.png"        );
images_systems_appleii[ APPLE_IIE_ENH    ] = load_image( argv[0] , "assets/images/systems/APPLE_IIE.png"        );
images_systems_appleii[ APPLE_IIE_PLAT   ] = load_image( argv[0] , "assets/images/systems/APPLE_IIE_PLAT.png"   );
images_systems_appleii[ APPLE_IIC        ] = load_image( argv[0] , "assets/images/systems/APPLE_IIC.png"        );
images_systems_appleii[ APPLE_IIC_PLUS   ] = load_image( argv[0] , "assets/images/systems/APPLE_IIC.png"        );
images_systems_appleii[ APPLE_IIGS       ] = load_image( argv[0] , "assets/images/systems/APPLE_IIGS.png"       );
images_systems_appleii[ APPLE_III        ] = load_image( argv[0] , "assets/images/systems/APPLE_III.png"        );

// Array of pointers to Nuklear images for: Systems - Apple II
struct nk_image images_systems_archimedes[ARCHIMEDES_COUNT];
// Load images for: Systems - Archimedes
images_systems_archimedes[ ARCHIMEDES_A310  ] = load_image(argv[0], "assets/images/systems/ACORN_ARC_A310.png"  );
images_systems_archimedes[ ARCHIMEDES_A5000 ] = load_image(argv[0], "assets/images/systems/ACORN_ARC_A5000.png" );

// Array of pointers to Nuklear images for: Systems - Atari 8-Bit
struct nk_image images_systems_atari8[ATARI8_COUNT];
// Load images for: Systems - Atari 8-Bit
images_systems_atari8[ ATARI8_400    ] = load_image(argv[0], "assets/images/systems/ATARI8_400.png"   );
images_systems_atari8[ ATARI8_800    ] = load_image(argv[0], "assets/images/systems/ATARI8_800.png"   );
images_systems_atari8[ ATARI8_1200XL ] = load_image(argv[0], "assets/images/systems/ATARI8_1200XL.png");
images_systems_atari8[ ATARI8_600XL  ] = load_image(argv[0], "assets/images/systems/ATARI8_600XL.png" );
images_systems_atari8[ ATARI8_800XL  ] = load_image(argv[0], "assets/images/systems/ATARI8_800XL.png" );
images_systems_atari8[ ATARI8_65XE   ] = load_image(argv[0], "assets/images/systems/ATARI8_130XE.png" ); // Need to find an image for this. 
images_systems_atari8[ ATARI8_130XE  ] = load_image(argv[0], "assets/images/systems/ATARI8_130XE.png" );
images_systems_atari8[ ATARI8_800XE  ] = load_image(argv[0], "assets/images/systems/ATARI8_130XE.png" ); // Need to find an image for this. 
images_systems_atari8[ ATARI8_XEGS   ] = load_image(argv[0], "assets/images/systems/ATARI8_XEGS.png"  );

// Array of pointers to Nuklear images for: Systems - Atari ST
struct nk_image images_systems_atarist[ATARIST_COUNT];
// Load images for: Systems - Atari ST
images_systems_atarist[ ATARIST_520ST     ] = load_image(argv[0], "assets/images/systems/ATARIST_520.png"   );
images_systems_atarist[ ATARIST_1040ST    ] = load_image(argv[0], "assets/images/systems/ATARIST_1040.png"  );
images_systems_atarist[ ATARIST_520ST_FM  ] = load_image(argv[0], "assets/images/systems/ATARIST_520.png"   );
images_systems_atarist[ ATARIST_1040STF   ] = load_image(argv[0], "assets/images/systems/ATARIST_1040.png"  );
images_systems_atarist[ ATARIST_520STF    ] = load_image(argv[0], "assets/images/systems/ATARIST_520.png"   );
images_systems_atarist[ ATARIST_MEGA_ST_1 ] = load_image(argv[0], "assets/images/systems/ATARIST_MEGA.png"  );
// images_systems_atarist[ ATARIST_MEGA_ST_2 ] = load_image(argv[0], "assets/images/systems/ATARIST_MEGA.png"  );
// images_systems_atarist[ ATARIST_MEGA_ST_4 ] = load_image(argv[0], "assets/images/systems/ATARIST_MEGA.png"  );
images_systems_atarist[ ATARIST_1040STE   ] = load_image(argv[0], "assets/images/systems/ATARIST_1040.png"  );
images_systems_atarist[ ATARIST_520STE    ] = load_image(argv[0], "assets/images/systems/ATARIST_520.png"   );
images_systems_atarist[ ATARIST_MEGA_STE  ] = load_image(argv[0], "assets/images/systems/ATARIST_MEGA.png"  );
images_systems_atarist[ ATARIST_TT030     ] = load_image(argv[0], "assets/images/systems/ATARIST_TT030.png" );
images_systems_atarist[ ATARIST_FALCON030 ] = load_image(argv[0], "assets/images/systems/ATARIST_FALCON.png");

// Array of pointers to Nuklear images for: Systems - BBC Micro
struct nk_image images_systems_bbcmicro[BBC_COUNT];
// Load images for: Systems - BBC Micro
images_systems_bbcmicro[ BBC_MICRO_A        ] = load_image(argv[0], "assets/images/systems/BBC_MICRO.png"         );
images_systems_bbcmicro[ BBC_MICRO_B        ] = load_image(argv[0], "assets/images/systems/BBC_MICRO.png"         );
images_systems_bbcmicro[ BBC_MICRO_BP64     ] = load_image(argv[0], "assets/images/systems/BBC_MICRO.png"         );
images_systems_bbcmicro[ BBC_MICRO_BP128    ] = load_image(argv[0], "assets/images/systems/BBC_MICRO.png"         );
images_systems_bbcmicro[ BBC_MASTER_128     ] = load_image(argv[0], "assets/images/systems/BBC_MASTER.png"        );
images_systems_bbcmicro[ BBC_MASTER_TURBO   ] = load_image(argv[0], "assets/images/systems/BBC_MASTER.png"        );
images_systems_bbcmicro[ BBC_MASTER_512     ] = load_image(argv[0], "assets/images/systems/BBC_MASTER.png"        );
images_systems_bbcmicro[ BBC_MASTER_ET      ] = load_image(argv[0], "assets/images/systems/BBC_MASTER.png"        );
images_systems_bbcmicro[ BBC_MASTER_COMPACT ] = load_image(argv[0], "assets/images/systems/BBC_MASTER_COMPACT.png");
images_systems_bbcmicro[ ACORN_ELECTRON     ] = load_image(argv[0], "assets/images/systems/ACORN_ELECTRON.png"    );

// Array of pointers to Nuklear images for: Systems - Commodore 
struct nk_image images_systems_commodore[CBM_COUNT];
// Load images for: Systems - Commodore
images_systems_commodore[ CBM_PET_2001 ] = load_image(argv[0], "assets/images/systems/CBM_PET_2001.png");
images_systems_commodore[ CBM_PET_3008 ] = load_image(argv[0], "assets/images/systems/CBM_PET_3008.png");
// images_systems_commodore[ CBM_PET_3016 ] = load_image(argv[0], "assets/images/systems/CBM_PET_3008.png"); // These all kinda look the same. Temp for now. TODO: Update these with the real deal! 
// images_systems_commodore[ CBM_PET_3032 ] = load_image(argv[0], "assets/images/systems/CBM_PET_3008.png"); // These all kinda look the same. Temp for now. TODO: Update these with the real deal! 
images_systems_commodore[ CBM_PET_4016 ] = load_image(argv[0], "assets/images/systems/CBM_PET_4016.png"); // These all kinda look the same. Temp for now. TODO: Update these with the real deal! 
images_systems_commodore[ CBM_PET_4032 ] = load_image(argv[0], "assets/images/systems/CBM_PET_4032.png"); // Looks the same as 8032
images_systems_commodore[ CBM_PET_8032 ] = load_image(argv[0], "assets/images/systems/CBM_PET_8032.png");
images_systems_commodore[ CBM_128_80 ]   = load_image(argv[0], "assets/images/systems/CBM_128_80.png");
images_systems_commodore[ CBM_SUPERPET ] = load_image(argv[0], "assets/images/systems/CBM_SUPERPET.png");
images_systems_commodore[ CBM_VIC20    ] = load_image(argv[0], "assets/images/systems/CBM_VIC20.png"   );
images_systems_commodore[ CBM_VIC1001  ] = load_image(argv[0], "assets/images/systems/CBM_VIC20.png"   ); // Same as VIC-20
images_systems_commodore[ CBM_64       ] = load_image(argv[0], "assets/images/systems/CBM_64.png"      );
images_systems_commodore[ CBM_64C      ] = load_image(argv[0], "assets/images/systems/CBM_64C.png"     );
images_systems_commodore[ CBM_C64GS    ] = load_image(argv[0], "assets/images/systems/CBM_C64GS.png"   );
images_systems_commodore[ CBM_SX64     ] = load_image(argv[0], "assets/images/systems/CBM_SX64.png"    );
images_systems_commodore[ CBM_116      ] = load_image(argv[0], "assets/images/systems/CBM_116.png"     );
images_systems_commodore[ CBM_16       ] = load_image(argv[0], "assets/images/systems/CBM_16.png"      );
images_systems_commodore[ CBM_PLUS4    ] = load_image(argv[0], "assets/images/systems/CBM_PLUS4.png"   );
images_systems_commodore[ CBM_128      ] = load_image(argv[0], "assets/images/systems/CBM_128.png"     );
images_systems_commodore[ CBM_128D     ] = load_image(argv[0], "assets/images/systems/CBM_128D.png"    );
// images_systems_commodore[ CBM_128DCR   ] = load_image(argv[0], "assets/images/systems/CBM_128D.png"    ); // Looks the same as 128D.

// Array of pointers to Nuklear images for: Systems - Macintosh 
struct nk_image images_systems_macintosh[MAC_COUNT];
// Load images for: Systems - Macintosh
images_systems_macintosh[ MAC_128K             ] = load_image(argv[0], "assets/images/systems/MAC_128K.png"         );
images_systems_macintosh[ MAC_512K             ] = load_image(argv[0], "assets/images/systems/MAC_128K.png"         ); // Looks like MAC_128K.
images_systems_macintosh[ MAC_512KE            ] = load_image(argv[0], "assets/images/systems/MAC_128K.png"         ); // Looks like MAC_128K.
images_systems_macintosh[ MAC_PLUS             ] = load_image(argv[0], "assets/images/systems/MAC_PLUS.png"         );
images_systems_macintosh[ MAC_SE               ] = load_image(argv[0], "assets/images/systems/MAC_SE.png"           );
images_systems_macintosh[ MAC_SE30             ] = load_image(argv[0], "assets/images/systems/MAC_SE30.png"         );
images_systems_macintosh[ MAC_CLASSIC          ] = load_image(argv[0], "assets/images/systems/MAC_CLASSIC.png"      );
images_systems_macintosh[ MAC_CLASSIC_II       ] = load_image(argv[0], "assets/images/systems/MAC_CLASSIC_II.png"   );
// images_systems_macintosh[ MAC_COLOR_CLASSIC    ] = load_image(argv[0], "assets/images/systems/MAC_COLOR_CLASSIC.png");
// images_systems_macintosh[ MAC_COLOR_CLASSIC_II ] = load_image(argv[0], "assets/images/systems/MAC_COLOR_CLASSIC.png"); // Looks the same as MAC_COLOR_CLASSIC.
images_systems_macintosh[ MAC_PORTABLE         ] = load_image(argv[0], "assets/images/systems/MAC_PORTABLE.png"     );

// Array of pointers to Nuklear images for: Systems - PC Compatibles
struct nk_image images_systems_pccompat[PC_COUNT];
// Load images for: Systems - pccompat
images_systems_pccompat[ PC_8088  ] = load_image(argv[0], "assets/images/systems/PC_8088.png" );
images_systems_pccompat[ PC_80286 ] = load_image(argv[0], "assets/images/systems/PC_80286.png");
images_systems_pccompat[ PC_80386 ] = load_image(argv[0], "assets/images/systems/PC_80386.png");
images_systems_pccompat[ PC_80486 ] = load_image(argv[0], "assets/images/systems/PC_80486.png");

// Array of pointers to Nuklear images for: Systems - PC Compatibles
struct nk_image images_systems_tandycoco[TANDYCOCO_COUNT];
// Load images for: Systems - Tandy CoCo
images_systems_tandycoco[ TANDY_COCO1 ] = load_image(argv[0], "assets/images/systems/TANDY_COCO_1.png");
images_systems_tandycoco[ TANDY_COCO2 ] = load_image(argv[0], "assets/images/systems/TANDY_COCO_2.png");
images_systems_tandycoco[ TANDY_COCO3 ] = load_image(argv[0], "assets/images/systems/TANDY_COCO_3.png");

// Array of pointers to Nuklear images for: Systems - PC Compatibles
struct nk_image images_systems_dragon[DRAGON_COUNT];
// Load images for: Systems - Dragon
images_systems_dragon[ DRAGON_32 ]   = load_image(argv[0], "assets/images/systems/DRAGON_32.png");
images_systems_dragon[ DRAGON_64 ]   = load_image(argv[0], "assets/images/systems/DRAGON_64.png");
images_systems_dragon[ DRAGON_TANO ] = load_image(argv[0], "assets/images/systems/DRAGON_TANO.png");

// Array of pointers to Nuklear images for: Systems - Olivetti
struct nk_image images_systems_olivetti[OLIVETTI_COUNT];
// Load images for: Systems - Dragon
images_systems_olivetti[ OLIVETTI_M20 ] = load_image(argv[0], "assets/images/systems/OLIVETTI_M20.png");

// Array of pointers to Nuklear images for: Systems - Flux Mode
struct nk_image images_systems_flux_mode[FLUX_COUNT];
// Load images for: Systems - flux_mode
images_systems_flux_mode[ FLUX_READWRITE ] = load_image(argv[0], "assets/images/systems/FLUX_READWRITE.png");
images_systems_flux_mode[ FLUX_READONLY  ] = load_image(argv[0], "assets/images/systems/FLUX_READONLY.png" );

/********************************************************************************/
// Images - Drives
/********************************************************************************/

// Array of pointers to Nuklear images for: Drives - Amiga
struct nk_image images_drives_amiga[AMIGA_DRIVE_COUNT];
// Load images for: Drives - Amiga
images_drives_amiga[ AMIGA_DRIVE_1010 ] = load_image(argv[0], "assets/images/drives/AMIGA_DRIVE_1010.png");
images_drives_amiga[ AMIGA_DRIVE_A4K  ] = load_image(argv[0], "assets/images/drives/AMIGA_DRIVE_A4K.png" );

// Array of pointers to Nuklear images for: Drives - Apple II
struct nk_image images_drives_appleii[APPLEII_DRIVE_COUNT];
// Load images for: Drives - Apple II
images_drives_appleii[ APPLEII_DRIVE_525_APPDOS_DO  ] = load_image(argv[0], "assets/images/drives/APPLEII_DRIVE_525.png"  );
images_drives_appleii[ APPLEII_DRIVE_525_PRODOS_PO  ] = load_image(argv[0], "assets/images/drives/APPLEIIC_DRIVE_525.png" );
images_drives_appleii[ APPLEII_DRIVE_525_NOFS_DSK   ] = load_image(argv[0], "assets/images/drives/APPLEIIC_DRIVE_525.png" );
images_drives_appleii[ APPLEII_DRIVE_525_APPDOS_DSK ] = load_image(argv[0], "assets/images/drives/APPLEII_DRIVE_525.png"  );
images_drives_appleii[ APPLEII_DRIVE_525_PRODOS_DSK ] = load_image(argv[0], "assets/images/drives/APPLEIIC_DRIVE_525.png" );
images_drives_appleii[ APPLEII_DRIVE_35_PD16_2MG    ] = load_image(argv[0], "assets/images/drives/MAC_DRIVE_35_DS_1M4.png");
images_drives_appleii[ APPLEII_DRIVE_35_HFS_2MG     ] = load_image(argv[0], "assets/images/drives/MAC_DRIVE_35_DS_1M4.png");

// Array of pointers to Nuklear images for: Drives - Amiga
struct nk_image images_drives_archimedes[ARCHIMEDES_DRIVE_COUNT];
// Load images for: Drives - Archimedes
images_drives_archimedes[ ARCHIMEDES_DRIVE_DD_D ] = load_image(argv[0], "assets/images/drives/ACORN_35_DRIVE_LIGHT.png"  );
images_drives_archimedes[ ARCHIMEDES_DRIVE_DD_E ] = load_image(argv[0], "assets/images/drives/ACORN_35_DRIVE_LIGHT_2.png");
images_drives_archimedes[ ARCHIMEDES_DRIVE_HD   ] = load_image(argv[0], "assets/images/drives/ACORN_35_DRIVE_DARK.png"   );

// Array of pointers to Nuklear images for: Drive - Atari 8-Bit
struct nk_image images_drives_atari8[ATARI8_DRIVE_COUNT];
// Load images for: Drives - Atari 8-Bit
images_drives_atari8[ ATARI8_DRIVE_810        ] = load_image(argv[0], "assets/images/drives/ATARI8_DRIVE_810.png"  );
images_drives_atari8[ ATARI8_DRIVE_1050_DD130 ] = load_image(argv[0], "assets/images/drives/ATARI8_DRIVE_1050.png" );
images_drives_atari8[ ATARI8_DRIVE_1050_DD180 ] = load_image(argv[0], "assets/images/drives/ATARI8_DRIVE_1050.png" );
images_drives_atari8[ ATARI8_DRIVE_XF551      ] = load_image(argv[0], "assets/images/drives/ATARI8_DRIVE_XF551.png");

// Array of pointers to Nuklear images for: Drive - Atari 8-Bit
struct nk_image images_drives_atarist[ATARIST_DRIVE_COUNT];
// Load images for: Drives - Atari 8-Bit
images_drives_atarist[ ATARIST_SF354_SSDD_360K_ST  ] = load_image(argv[0], "assets/images/drives/ATARIST_ST354.png");
images_drives_atarist[ ATARIST_SF314_DSDD_720K_ST  ] = load_image(argv[0], "assets/images/drives/ATARIST_ST314.png");
images_drives_atarist[ ATARIST_HDINT_DSHD_1_4M_ST  ] = load_image(argv[0], "assets/images/drives/ATARIST_INT.png"  );
images_drives_atarist[ ATARIST_SF354_SSDD_400K_ST  ] = load_image(argv[0], "assets/images/drives/ATARIST_ST354.png");
images_drives_atarist[ ATARIST_SF354_SSDD_440K_ST  ] = load_image(argv[0], "assets/images/drives/ATARIST_ST354.png");
images_drives_atarist[ ATARIST_SF354_DSDD_800K_ST  ] = load_image(argv[0], "assets/images/drives/ATARIST_ST354.png");
images_drives_atarist[ ATARIST_SF354_DSDD_880K_ST  ] = load_image(argv[0], "assets/images/drives/ATARIST_ST354.png");
images_drives_atarist[ ATARIST_SF354_SSDD_360K_MSA ] = load_image(argv[0], "assets/images/drives/ATARIST_ST354.png");
images_drives_atarist[ ATARIST_SF314_DSDD_720K_MSA ] = load_image(argv[0], "assets/images/drives/ATARIST_ST314.png");
images_drives_atarist[ ATARIST_HDINT_DSHD_1_4M_MSA ] = load_image(argv[0], "assets/images/drives/ATARIST_INT.png"  );
images_drives_atarist[ ATARIST_SF354_SSDD_400K_MSA ] = load_image(argv[0], "assets/images/drives/ATARIST_ST354.png");
images_drives_atarist[ ATARIST_SF354_SSDD_440K_MSA ] = load_image(argv[0], "assets/images/drives/ATARIST_ST354.png");
images_drives_atarist[ ATARIST_SF354_DSDD_800K_MSA ] = load_image(argv[0], "assets/images/drives/ATARIST_ST354.png");
images_drives_atarist[ ATARIST_SF354_DSDD_880K_MSA ] = load_image(argv[0], "assets/images/drives/ATARIST_ST354.png");
images_drives_atarist[ ATARIST_SF354_SSDD_360K_STX ] = load_image(argv[0], "assets/images/drives/ATARIST_ST354.png");
images_drives_atarist[ ATARIST_SF314_DSDD_720K_STX ] = load_image(argv[0], "assets/images/drives/ATARIST_ST314.png");
images_drives_atarist[ ATARIST_HDINT_DSHD_1_4M_STX ] = load_image(argv[0], "assets/images/drives/ATARIST_INT.png"  );
images_drives_atarist[ ATARIST_SF354_SSDD_400K_STX ] = load_image(argv[0], "assets/images/drives/ATARIST_ST354.png");
images_drives_atarist[ ATARIST_SF354_SSDD_440K_STX ] = load_image(argv[0], "assets/images/drives/ATARIST_ST354.png");
images_drives_atarist[ ATARIST_SF354_DSDD_800K_STX ] = load_image(argv[0], "assets/images/drives/ATARIST_ST354.png");
images_drives_atarist[ ATARIST_SF354_DSDD_880K_STX ] = load_image(argv[0], "assets/images/drives/ATARIST_ST354.png");

// Array of pointers to Nuklear images for: Drives - BBC Micro
struct nk_image images_drives_bbcmicro[BBC_DRIVE_COUNT];
// Load images for: Drives - BBC Micro
images_drives_bbcmicro[ BBC_DRIVE_525_SSSD_100_SSD ] = load_image(argv[0], "assets/images/drives/BBC_DRIVE_525_SS.png");
images_drives_bbcmicro[ BBC_DRIVE_525_SSQD_200_SSD ] = load_image(argv[0], "assets/images/drives/BBC_DRIVE_525_DS.png");
images_drives_bbcmicro[ BBC_DRIVE_525_DSQD_200_DSD ] = load_image(argv[0], "assets/images/drives/BBC_DRIVE_525_DS.png");
images_drives_bbcmicro[ BBC_DRIVE_525_DSQD_400_DSD ] = load_image(argv[0], "assets/images/drives/BBC_DRIVE_525_DS.png");
images_drives_bbcmicro[ BBC_DRIVE_525_SSDD_160_ADL ] = load_image(argv[0], "assets/images/drives/BBC_DRIVE_525_SS.png");
images_drives_bbcmicro[ BBC_DRIVE_525_SSQD_320_ADL ] = load_image(argv[0], "assets/images/drives/BBC_DRIVE_525_DS.png");
images_drives_bbcmicro[ BBC_DRIVE_525_DSQD_640_ADL ] = load_image(argv[0], "assets/images/drives/BBC_DRIVE_525_DS.png");

// Array of pointers to Nuklear images for: Drives - Commodore 
struct nk_image images_drives_commodore[CBM_DRIVE_COUNT];
// Load images for: Drives - Commodore
images_drives_commodore[ CBM_DRIVE_2040    ] = load_image(argv[0], "assets/images/drives/CBM_DRIVE_2040.png"   );
images_drives_commodore[ CBM_DRIVE_3040    ] = load_image(argv[0], "assets/images/drives/CBM_DRIVE_3040.png"   );
images_drives_commodore[ CBM_DRIVE_4040    ] = load_image(argv[0], "assets/images/drives/CBM_DRIVE_4040.png"   );
images_drives_commodore[ CBM_DRIVE_2031    ] = load_image(argv[0], "assets/images/drives/CBM_DRIVE_2031.png"   );
images_drives_commodore[ CBM_DRIVE_4031    ] = load_image(argv[0], "assets/images/drives/CBM_DRIVE_4031.png"   );
images_drives_commodore[ CBM_DRIVE_1541    ] = load_image(argv[0], "assets/images/drives/CBM_DRIVE_1541.png"   );  
images_drives_commodore[ CBM_DRIVE_1541II  ] = load_image(argv[0], "assets/images/drives/CBM_DRIVE_1541II.png" ); // DONE! TODO: Create entry for 1541-II jsut because it's the coolest boy in school! 
images_drives_commodore[ CBM_DRIVE_1551    ] = load_image(argv[0], "assets/images/drives/CBM_DRIVE_1551.png"   );
images_drives_commodore[ CBM_DRIVE_1570    ] = load_image(argv[0], "assets/images/drives/CBM_DRIVE_1570.png"   );
images_drives_commodore[ CBM_DRIVE_1571    ] = load_image(argv[0], "assets/images/drives/CBM_DRIVE_1571.png"   );
images_drives_commodore[ CBM_DRIVE_1581    ] = load_image(argv[0], "assets/images/drives/CBM_DRIVE_1581.png"   );
images_drives_commodore[ CBM_DRIVE_8050    ] = load_image(argv[0], "assets/images/drives/CBM_DRIVE_8050.png"   );
images_drives_commodore[ CBM_DRIVE_8250    ] = load_image(argv[0], "assets/images/drives/CBM_DRIVE_8250.png"   );
images_drives_commodore[ CBM_DRIVE_SFD1001 ] = load_image(argv[0], "assets/images/drives/CBM_DRIVE_SFD1001.png");

// Array of pointers to Nuklear images for: Drives - Macintosh
struct nk_image images_drives_macintosh[MAC_DRIVE_COUNT];
// Load images for: Drives - macintosh
images_drives_macintosh[ MAC_DRIVE_35_SS_400K ] = load_image(argv[0], "assets/images/drives/MAC_DRIVE_35_SS_400K.png");
images_drives_macintosh[ MAC_DRIVE_35_DS_800K ] = load_image(argv[0], "assets/images/drives/MAC_DRIVE_35_DS_800K.png");
images_drives_macintosh[ MAC_DRIVE_35_DS_1M4  ] = load_image(argv[0], "assets/images/drives/MAC_DRIVE_35_DS_1M4.png" );

// Array of pointers to Nuklear images for: Drives - PC Compatibles 
struct nk_image images_drives_pccompat[PC_DRIVE_COUNT];
// Load images for: Drives - PC Compatibles 
images_drives_pccompat[ PC_DRIVE_525_DD_360_IMA ] = load_image(argv[0], "assets/images/drives/PC_DRIVE_525_DD.png ");
images_drives_pccompat[ PC_DRIVE_525_HD_1M2_IMA ] = load_image(argv[0], "assets/images/drives/PC_DRIVE_525_HD.png ");
images_drives_pccompat[ PC_DRIVE_35_DD_720_IMA  ] = load_image(argv[0], "assets/images/drives/PC_DRIVE_35_DD.png  ");
images_drives_pccompat[ PC_DRIVE_35_HD_1M4_IMA  ] = load_image(argv[0], "assets/images/drives/PC_DRIVE_35_HD.png  ");
images_drives_pccompat[ PC_DRIVE_525_DD_360_IMD ] = load_image(argv[0], "assets/images/drives/PC_DRIVE_525_DD.png "); // This is just another file format and don't need their own icons. 
images_drives_pccompat[ PC_DRIVE_525_HD_1M2_IMD ] = load_image(argv[0], "assets/images/drives/PC_DRIVE_525_HD.png "); // This is just another file format and don't need their own icons. 
images_drives_pccompat[ PC_DRIVE_35_DD_720_IMD  ] = load_image(argv[0], "assets/images/drives/PC_DRIVE_35_DD.png  ");  // This is just another file format and don't need their own icons. 
images_drives_pccompat[ PC_DRIVE_35_HD_1M4_IMD  ] = load_image(argv[0], "assets/images/drives/PC_DRIVE_35_HD.png  ");  // This is just another file format and don't need their own icons. 

// Array of pointers to Nuklear images for: Drives - Tandy CoCO
struct nk_image images_drives_tandycoco[TANDYCOCO_DRIVE_COUNT]; // BUG? WAS TANDYCOCO_COUNT but that screwed up the PC Compatibles drive images. This is becasue TANDYCOCO_COUNT is too short so I was writing away into memory no-mans-land. That screwed up the pc compatibles probably because some of that got clobbered or sometihng. 
// Load images for: Drives - PC Compatibles 
images_drives_tandycoco[ TANDYCOCO_525_SSDD_158_DECB_DSK ] = load_image(argv[0], "assets/images/drives/TANDY_COCO_FULLHEIGHT.png");
images_drives_tandycoco[ TANDYCOCO_525_SSDD_180_DECB_DSK ] = load_image(argv[0], "assets/images/drives/TANDY_COCO_40TRACK.png");
images_drives_tandycoco[ TANDYCOCO_525_SSDD_180_OS9_DSK  ] = load_image(argv[0], "assets/images/drives/TANDY_COCO_40TRACK.png" );
images_drives_tandycoco[ TANDYCOCO_525_DSDD_360_OS9_DSK  ] = load_image(argv[0], "assets/images/drives/TANDY_COCO_40TRACK.png" );
images_drives_tandycoco[ TANDYCOCO_525_SSQD_360_OS9_DSK  ] = load_image(argv[0], "assets/images/drives/TANDY_COCO_80TRACK.png"); // This is just another file format and don't need their own icons. 
images_drives_tandycoco[ TANDYCOCO_525_DSQD_720_OS9_DSK  ] = load_image(argv[0], "assets/images/drives/TANDY_COCO_80TRACK.png"); // This is just another file format and don't need their own icons. 
images_drives_tandycoco[ TANDYCOCO_35_SSDD_360_OS9_DSK   ] = load_image(argv[0], "assets/images/drives/TANDY_COCO_35INCH.png" );  // This is just another file format and don't need their own icons. 
images_drives_tandycoco[ TANDYCOCO_35_DSDD_720_OS9_DSK   ] = load_image(argv[0], "assets/images/drives/TANDY_COCO_35INCH.png" );  // This is just another file format and don't need their own icons. 

// Array of pointers to Nuklear images for: Drives - Dragon
struct nk_image images_drives_dragon[DRAGON_DRIVE_COUNT]; // BUG? WAS TANDYCOCO_COUNT but that screwed up the PC Compatibles drive images. This is becasue TANDYCOCO_COUNT is too short so I was writing away into memory no-mans-land. That screwed up the pc compatibles probably because some of that got clobbered or sometihng. 
// Load images for: Drives - Dragon
images_drives_dragon[ DRAGON_DDOS_525_SSDD_0180K_DSK ] = load_image(argv[0], "assets/images/drives/DRAGON_525_ORIGINAL_DRIVE.png");
images_drives_dragon[ DRAGON_DDOS_525_DSDD_0360K_DSK ] = load_image(argv[0], "assets/images/drives/DRAGON_525_CUMANA_DRIVE.png");
images_drives_dragon[ DRAGON_DDOS_525_SSQD_0360K_DSK ] = load_image(argv[0], "assets/images/drives/DRAGON_525_CUMANA_DRIVE.png");
images_drives_dragon[ DRAGON_DDOS_525_DSQD_0720K_DSK ] = load_image(argv[0], "assets/images/drives/DRAGON_525_CUMANA_DRIVE.png");
images_drives_dragon[ DRAGON_DDOS_525_SSDD_0180K_VDK ] = load_image(argv[0], "assets/images/drives/DRAGON_525_ORIGINAL_DRIVE.png");// This is just another file format and don't need their own icons. 
images_drives_dragon[ DRAGON_DDOS_525_DSDD_0360K_VDK ] = load_image(argv[0], "assets/images/drives/DRAGON_525_CUMANA_DRIVE.png");// This is just another file format and don't need their own icons. 
images_drives_dragon[ DRAGON_DDOS_525_SSQD_0360K_VDK ] = load_image(argv[0], "assets/images/drives/DRAGON_525_CUMANA_DRIVE.png");  // This is just another file format and don't need their own icons. 
images_drives_dragon[ DRAGON_DDOS_525_DSQD_0720K_VDK ] = load_image(argv[0], "assets/images/drives/DRAGON_525_CUMANA_DRIVE.png");  // This is just another file format and don't need their own icons. 

// Array of pointers to Nuklear images for: Drives - Olivetti
struct nk_image images_drives_olivetti[OLIVETTI_DRIVE_COUNT]; // BUG? WAS TANDYCOCO_COUNT but that screwed up the PC Compatibles drive images. This is becasue TANDYCOCO_COUNT is too short so I was writing away into memory no-mans-land. That screwed up the pc compatibles probably because some of that got clobbered or sometihng. 
// Load images for: Drives - olivetti
images_drives_olivetti[ OLIVETTIM20_PCOS_525_DSDD_0280K_IMG ] = load_image(argv[0], "assets/images/drives/OLIVETTI_M20_525_DRIVE.png"); // Generic Drive: "assets/images/drives/GENERIC_DRIVE.png"
images_drives_olivetti[ OLIVETTIM20_PCOS_525_DSDD_0280K_IMD ] = load_image(argv[0], "assets/images/drives/OLIVETTI_M20_525_DRIVE.png"); // Generic Drive: "assets/images/drives/GENERIC_DRIVE.png"

// Array of pointers to Nuklear images for: Drives - fluxreadwrite_drive
struct nk_image images_drives_fluxreadwrite_drive[FLUX_RW_DRIVE_COUNT];
// Load images for: Drives - fluxreadwrite_drive
images_drives_fluxreadwrite_drive[ FLUX_RW_DRIVE_SCP ] = load_image(argv[0], "assets/images/drives/FLUX_RW_DRIVE_SCP.png");
images_drives_fluxreadwrite_drive[ FLUX_RW_DRIVE_HFE ] = load_image(argv[0], "assets/images/drives/FLUX_RW_DRIVE_HFE.png");
images_drives_fluxreadwrite_drive[ FLUX_RW_DRIVE_DMK ] = load_image(argv[0], "assets/images/drives/FLUX_RW_DRIVE_DMK.png");
images_drives_fluxreadwrite_drive[ FLUX_RW_DRIVE_RAW ] = load_image(argv[0], "assets/images/drives/FLUX_RW_DRIVE_RAW.png");

// Array of pointers to Nuklear images for: Drives - fluxreadonly_drive
struct nk_image images_drives_fluxreadonly_drive[FLUX_RO_DRIVE_COUNT];
// Load images for: Drives - ffluxreadonly_drive
images_drives_fluxreadonly_drive[ FLUX_RO_DRIVE_IPF ] = load_image(argv[0], "assets/images/drives/FLUX_RO_DRIVE_IPF.png");
images_drives_fluxreadonly_drive[ FLUX_RO_DRIVE_CTR ] = load_image(argv[0], "assets/images/drives/FLUX_RO_DRIVE_CTR.png");
images_drives_fluxreadonly_drive[ FLUX_RO_DRIVE_A2R ] = load_image(argv[0], "assets/images/drives/FLUX_RO_DRIVE_A2R.png");

/********************************************************************************/

/*

Quotes: Sources

"One of the all-time greatest home computers ever!" 
- Chiron Bramberger, author: "Chiron's Disk Blaster" -- Literally this source code!

Triumph of the Nerds
https://www.pbs.org/nerds/part1.html
"This is an amazing bit of engineering that we can attribute to Steve Wozniak who is certainly the Mozart of digital design and all told it turned the Apple 2 into a sensation." - Bob Cringely

Computer Chronicles 1985 - Jack Tramiel
https://www.youtube.com/watch?v=eaKukgUrxuA
"We are a company which we like to sell to the masses, not to the classes." - Jack Tramiel

Steve Ballmer at .NET presentation: Developers (HQ, Extended)
https://archive.org/details/youtube-XxbJw8PrIkc
"Developers developers developers developers developers developers!" - Steve Ballmer

Inside RadioShack's slow-motion collapse: Why the fall of the 94-year-old electronics chain didn't have to be this way
https://financialpost.com/news/retail-marketing/inside-radioshacks-slow-motion-collapse-why-the-fall-of-the-94-year-old-electronics-chain-didnt-have-to-be-this-way
"We're not looking for the guy who wants to spend his entire paycheck on a sound system." - Charles Tandy

Nolan Bushnell / Atari
https://www.azquotes.com/author/2257-Nolan_Bushnell
"Everyone who's ever taken a shower has an idea. It's the person who gets out of the shower, dries off and does something about it who makes a difference." - Nolan Bushnell

Tim Wickes / Olivetti
https://nosher.net/archives/computers/pcw-1984-12-00_002
Tim Wickes / Olivetti - From a nosher.net archive of a Personal Computer World 
magazine advertisement/article from December 1984, covering Olivetti's aggressive 
marketing campaign against IBM for the M24.
"Olivetti has won any technical battle hands down. Now it's gloves off in the marketing arena." - Tim Wickes, Olivetti PR

Richard Wadman / Dragon Data
https://archive.worldofdragon.org/index.php?title=Dragon_History
Richard Wadman / Dragon Data - From the Dragon Archive (archive.worldofdragon.org), 
a detailed history of Dragon Data. The same quote also appeared mirrored on 
binarydinosaurs.co.uk and oocities.org, all drawing from the same Dragon history article.
"The combination of the 64, OS-9 and drives gives the company the means to enter other markets." - Richard Wadman, Dragon Data

Sam Tramiel / Atari
https://microzeit.com/en-us/blogs/news/famous-quotes-from-the-industry-sam-tramiel
Sam Tramiel / Atari - From a Microzeit Publishing blog post (microzeit.com) dedicated 
specifically to famous Sam Tramiel quotes, describing it as his well-known marketing 
slogan from his time as Atari President & CEO.
"We promised. We delivered. With pride, determination, and good old ATARI know-how." - Sam Tramiel

*/

// TODO: Make this better. These are FAKE functions because they go ahead and change like global variables. This is probaby bad. 
#define update_system_images() \
    switch (brand_selected) { \
        case BRAND_AMIGA      : TOOLTIP( nk_image( ctx , images_systems_amiga[      computer_selected ] ), " \"One of the all-time greatest home computers ever!\" - C. Bramberger"); break; \
        case BRAND_APPLEII    : TOOLTIP( nk_image( ctx , images_systems_appleii[    computer_selected ] ), " \"This is an amazing bit of engineering that we can attribute to Steve Wozniak who is certainly the Mozart of digital design and all told it turned the Apple 2 into a sensation.\" - Bob Cringely"); break; \
        case BRAND_ARCHIMEDES : TOOLTIP( nk_image( ctx , images_systems_archimedes[ computer_selected ] ), " The ARM that flexed on the world!"); break; \
        case BRAND_ATARI8BIT  : TOOLTIP( nk_image( ctx , images_systems_atari8[     computer_selected ] ), " \"Everyone who's ever taken a shower has an idea. It's the person who gets out of the shower, dries off and does something about it who makes a difference.\" Nolan Bushnell"); break; \
        case BRAND_ATARIST    : TOOLTIP( nk_image( ctx , images_systems_atarist[    computer_selected ] ), " \"We promised. We delivered. With pride, determination, and good old Atari know-how.\" - Sam Tramiel"); break; \
        case BRAND_BBCMICRO   : TOOLTIP( nk_image( ctx , images_systems_bbcmicro[   computer_selected ] ), " God bless the Beeb!"); break; \
        case BRAND_COMMODORE  : TOOLTIP( nk_image( ctx , images_systems_commodore[  computer_selected ] ), " \"We are a company which we like to sell to the masses, not to the classes.\" - Jack Tramiel"); break; \
        case BRAND_MACINTOSH  : TOOLTIP( nk_image( ctx , images_systems_macintosh[  computer_selected ] ), " Created by legends! Jef Raskin, Brian Howard, Marc LeBrun, Burrell Smith, Joanna Hoffman, Bud Tribble, Susan Care..."); break;  \
        case BRAND_PCCOMPAT   : TOOLTIP( nk_image( ctx , images_systems_pccompat[   computer_selected ] ), " \"Developers developers developers developers developers developers!\" - Steve Ballmer"); break;  \
        case BRAND_TANDYCOCO  : TOOLTIP( nk_image( ctx , images_systems_tandycoco[  computer_selected ] ), " \"We're not looking for the guy who wants to spend his entire paycheck on a sound system.\" - Charles Tandy"); break;  \
        case BRAND_DRAGON     : TOOLTIP( nk_image( ctx , images_systems_dragon[     computer_selected ] ), " \"The combination of the 64, OS-9 and drives gives the company the means to enter other markets.\" - Richard Wadman, Dragon Data"); break; /* " \"A Dragon quote goes here.\" - Nerb McDorklington" */ \
        case BRAND_OLIVETTI   : TOOLTIP( nk_image( ctx , images_systems_olivetti[   computer_selected ] ), " \"Olivetti has won any technical battle hands down. Now it's gloves off in the marketing arena.\" - Tim Wickes, Olivetti PR"); break; /* " \"An Olivetti quote goes here.\" - Dorb McNerdison" */ \
        case BRAND_FLUX       : nk_image( ctx , images_systems_flux_mode[ computer_selected ] ); break; \
    };/*end-switch*/ \
/*end-define*/

/********************************************************************************/

// TODO: Make this better. These are FAKE functions because they go ahead and change like global variables. This is probaby bad. 
#define update_drive_images() \
    switch (brand_selected) { \
        case BRAND_AMIGA     : nk_image( ctx , images_drives_amiga[      drive_selected ] ); break; \
        case BRAND_APPLEII   : nk_image( ctx , images_drives_appleii[    drive_selected ] ); break; \
        case BRAND_ARCHIMEDES: nk_image( ctx , images_drives_archimedes[ drive_selected ] ); break; \
        case BRAND_ATARI8BIT : nk_image( ctx , images_drives_atari8[     drive_selected ] ); break; \
        case BRAND_ATARIST   : nk_image( ctx , images_drives_atarist[    drive_selected ] ); break; \
        case BRAND_BBCMICRO  : nk_image( ctx , images_drives_bbcmicro[   drive_selected ] ); break; \
        case BRAND_COMMODORE : nk_image( ctx , images_drives_commodore[  drive_selected ] ); break; \
        case BRAND_MACINTOSH : nk_image( ctx , images_drives_macintosh[  drive_selected ] ); break; \
        case BRAND_PCCOMPAT  : nk_image( ctx , images_drives_pccompat[   drive_selected ] ); break; \
        case BRAND_TANDYCOCO : nk_image( ctx , images_drives_tandycoco[  drive_selected ] ); break; \
        case BRAND_DRAGON    : nk_image( ctx , images_drives_dragon[     drive_selected ] ); break; \
        case BRAND_OLIVETTI  : nk_image( ctx , images_drives_olivetti[   drive_selected ] ); break; \
        case BRAND_FLUX :  \
            if        (computer_selected == FLUX_READWRITE) { \
                nk_image( ctx , images_drives_fluxreadwrite_drive[ drive_selected ] );  \
            } else if (computer_selected == FLUX_READONLY) { \
                nk_image( ctx , images_drives_fluxreadonly_drive[  drive_selected ] ); \
            };/*end-if*/ \
        break; \
    };/*end-switch*/ \
/*end-define*/

/********************************************************************************/

#endif /* LOAD_ALL_IMAGES_H */