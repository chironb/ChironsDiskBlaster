/*******************************************************************************

Chiron's Disk Blaster
Copyright (c) 2026 - GNU GPL v3
Using Nuklear for GUI.

----------

For Compiling: 

cd "/c/Users/micro/Dropbox/Chiron's Disk Blaster/sdl_opengl3/"         

make clean && clear && make

----------

For Application Icon: 

You need to save as .bmp and .png and then convert the .png to .ico using this: 
magick icon.png -define icon:auto-resize=512,256,128,64,48,32,16 icon.ico

Then you have to recompile the program, and then clear out the Windows icon cache: 
taskkill /IM explorer.exe /F
del /A /Q "%localappdata%\IconCache.db"
start explorer.exe

*******************************************************************************/


/********************************************************************/
/*                CHIRON'S DISK BLASTER VERSIONING                  */
/********************************************************************/

#define __MAJOR_VERSIONNUMBER__ 0
#define __MINOR_BUILDNUMBER__   1

/********************************************************************/
/*                CHIRONS CONSOLE DEBUGGING LIBRARY                 */
/********************************************************************/

// #if !defined CHIRONS_CONSOLE_DEBUGGING_LIBRARY_H
//     #define CHIRONS_CONSOLE_DEBUGGING_LIBRARY_H
//     #include "chirons_console_debugging_library.h"
// #endif

/********************************************************************/
/*                            INCLUDES                              */
/********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

#include <GL/glew.h> /* For OpenGL 3 or for Windows 11 even if we are using OpenGL 2 */
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

/* For stb_image.h stuff. */
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <windows.h>
#include <commdlg.h> /* Needed to add when I swtiched to OpenGL 2 */

/* For opening local files and stuff. */
#include <shellapi.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION

/*****/

// #define NK_SDL_GL3_IMPLEMENTATION /* OpenGL 3 */
#define NK_SDL_GL2_IMPLEMENTATION /* OpenGL 2 */

#include "nuklear.h"

// #include "nuklear_sdl_gl3.h" /* OpenGL 3 */
#include "nuklear_sdl_gl2.h" /* OpenGL 2 */

/*****/

#include <setupapi.h> // For detecting USB Floppy Drives in Windows 11. 

#define WINDOW_WIDTH 738
#define WINDOW_HEIGHT 545 // 565 is perfect but there's no room for the drop downs!

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

#include <commctrl.h>
#include <stdbool.h>

// Main disk format info.
#include "main_format_reference.h"  

// Main hardware info. Greaseweazle, ZoomFloppy, etc...
#include "hardware.h"               

// Main file process functions. For Drag and Drop, Open, etc...
#include "file_processing.h"

/********************************************************************************/
/********************************************************************************/
/****************************** OPTIONS VARIBLES ********************************/
/********************************************************************************/
/********************************************************************************/

// These are global varibles set by the external options file. 
// These should also have, eventually, matching entries in the "Options" window.

// [User Interface]
float options_background_color_r = 0.10 ;
float options_background_color_g = 0.18 ;
float options_background_color_b = 0.24 ;
float options_background_color_a = 1.00 ;

// [External Paths]
char options_greaseweazle_path[MAX_PATH]   = "C:\\Program Files\\greaseweazle-1.23" ; 
char options_ciderpress2cli_path[MAX_PATH] = "C:\\Program Files\\cp2_1.2.0_win-x64_sc" ; 
char options_opencbm_path[MAX_PATH]        = "C:\\Program Files\\opencbm-0.4.99.104\amd64" ;
char options_dd_path[MAX_PATH]             = "C:\\Program Files\\dd-0.6beta3" ;

// [External Executables]
char options_greaseweazle_executable[MAX_PATH]    = "gw.exe"      ; /* \"%s\\%s\" options_greaseweazle_path, options_greaseweazle_executable, */
char options_ciderpress2_cli_executable[MAX_PATH] = "cp2.exe"     ; /* \"%s\\%s\" options_ciderpress2cli_path, options_ciderpress2_cli_executable, */
char options_opencbm_cbmctrl_executable[MAX_PATH] = "cbmctrl.exe" ; /* \"%s\\%s\" options_opencbm_path, options_opencbm_cbmctrl_executable, */
char options_opencbm_d64copy_executable[MAX_PATH] = "d64copy.exe" ; /* \"%s\\%s\" options_opencbm_path, options_opencbm_d64copy_executable, */
char options_opencbm_imgcopy_executable[MAX_PATH] = "imgcopy.exe" ; /* \"%s\\%s\" options_opencbm_path, options_opencbm_imgcopy_executable, */
char options_opencbm_d82copy_executable[MAX_PATH] = "d82copy.exe" ; /* \"%s\\%s\" options_opencbm_path, options_opencbm_d82copy_executable, */
char options_dd_executable[MAX_PATH]              = "dd.exe"      ; /* \"%s\\%s\" options_dd_path, options_dd_executable, */

/******************************************************************/
/*************************** FUNCTIONS ****************************/
/******************************************************************/

#ifndef ARRAY_LEN
#define ARRAY_LEN(a) (sizeof(a) / sizeof((a)[0]))
#endif

// DARK MODE STUFF
#include <SDL2/SDL_syswm.h>
#include <dwmapi.h>

// #pragma comment(lib, "dwmapi.lib") // can just delete since it's MSVC-only and does nothing in MinGW
// Call this AFTER you create your SDL window, passing the HWND
void EnableDarkTitleBar(HWND hwnd) {
    BOOL useDarkMode = TRUE;
    DwmSetWindowAttribute(
        hwnd,
        DWMWA_USE_IMMERSIVE_DARK_MODE,  // attribute 20
        &useDarkMode,
        sizeof(useDarkMode)
    );
}

static char selected_image_path[MAX_PATH] = {0};

static int open_file_dialog(void) {
    OPENFILENAME ofn;
    char filename[MAX_PATH] = {0};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize     = sizeof(ofn);
    ofn.hwndOwner       = NULL;
    ofn.lpstrFile       = filename;
    ofn.nMaxFile        = MAX_PATH;
    ofn.lpstrFilter     = "Disk Images\0*.2MG;*.A2R;*.ADF;*.ADL;*.ADM;*.ADS;*.ADZ;*.DO;*.ATR;*.CTR;*.D1M;*.D2M;*.D3M;*.D4M;*.D64;*.D71;*.D80;*.D81;*.D82;*.D88;*.DC42;*.DCP;*.DIM;*.DIMG;*.DMK;*.DO;*.DSD;*.DSK;*.EDSK;*.FD;*.FDI;*.G64;*.HDM;*.HFE;*.IMA;*.IMAGE;*.IMD;*.IMG;*.IPF;*.MGT;*.MSA;*.NFD;*.NSI;*.PO;*.RAW;*.SCP;*.SF7;*.SSD;*.ST;*.STX;*.TD0;*.XDF;*.XFD\0All Files\0*.*\0";
    ofn.nFilterIndex    = 1;
    ofn.lpstrTitle      = "Select Disk Image";
    ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileName(&ofn)) {
        strncpy(selected_image_path, filename, MAX_PATH - 1);
        return 1; // success
    }
    return 0; // cancelled
}

static int save_as_file_dialog(void) {
    OPENFILENAME ofn;
    char filename[MAX_PATH] = {0};
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize     = sizeof(ofn);
    ofn.hwndOwner       = NULL;
    ofn.lpstrFile       = filename;
    ofn.nMaxFile        = MAX_PATH;
    ofn.lpstrFilter     = "Disk Images\0*.2MG;*.A2R;*.ADF;*.ADL;*.ADM;*.ADS;*.ADZ;*.DO;*.ATR;*.CTR;*.D1M;*.D2M;*.D3M;*.D4M;*.D64;*.D71;*.D80;*.D81;*.D82;*.D88;*.DC42;*.DCP;*.DIM;*.DIMG;*.DMK;*.DO;*.DSD;*.DSK;*.EDSK;*.FD;*.FDI;*.G64;*.HDM;*.HFE;*.IMA;*.IMAGE;*.IMD;*.IMG;*.IPF;*.MGT;*.MSA;*.NFD;*.NSI;*.PO;*.RAW;*.SCP;*.SF7;*.SSD;*.ST;*.STX;*.TD0;*.XDF;*.XFD\0All Files\0*.*\0";
    ofn.nFilterIndex    = 1;
    ofn.lpstrTitle      = "Save Disk Image As";
    ofn.Flags           = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
    // TODO: If the user chooses and existing file, the OFN_OVERWRITEPROMPT flag above asks them if they want to overwrite it.
    //       However, this is independent of the "--no-clobber" flag of Greaseweazle. 
    //       So this needs a better solution. At the very least a custom warning. 
    //       I want the default "--no-clobber" to stay, but this dialog box 
    //       should at least let them know that in order to overwrite the file
    //       they need to go to Hardware --> Greaseweazle --> Advanced and beside "Overwrite" click "Allow"
    //       in order to disable overwrite protection (aka "--no-clobber").
    if (GetSaveFileName(&ofn)) {

        // Check for an extension — strrchr finds the last dot
        char *dot        = strrchr(filename, '.');
        char *last_slash = strrchr(filename, '\\');

        // No dot found, OR the dot is before the last backslash 
        // (meaning the dot is in a folder name, not the filename itself)
        if (dot == NULL || (last_slash != NULL && dot < last_slash)) {
            strncat(filename, ".img", MAX_PATH - strlen(filename) - 1);
            MessageBox(NULL,
                "No file extension was entered.\n\n"
                "Your file has been saved with the extension .img\n\n"
                "You may want to rename it once you know the correct format.",
                "Extension Added", MB_OK | MB_ICONINFORMATION);
        }

        strncpy(selected_image_path, filename, MAX_PATH - 1);
        selected_image_path[MAX_PATH - 1] = '\0';
        return 1;
    }
    return 0;
}

// helper to extract just the filename from a full path
static const char *get_filename(const char *path) {
    const char *s = strrchr(path, '\\');
    if (!s) s = strrchr(path, '/'); // handle forward slashes too
    return s ? s + 1 : path;
}

/* Shared helper — same path logic as load image, reused everywhere */
static void build_full_path(const char *argv0, const char *relative_path,
                             char *out, size_t out_size) {
    strncpy(out, argv0, out_size - 1);
    out[out_size - 1] = '\0';
    for (char *p = out; *p; p++) {
        if (*p == '\\') *p = '/';
    }
    char *last_slash = strrchr(out, '/');
    if (last_slash) {
        *(last_slash + 1) = '\0';
    } else {
        out[0] = '\0';
    }
    strncat(out, relative_path, out_size - strlen(out) - 1);
}

// This loads images but also uses main's argv[0] so that 
// if you run the program from the command line or any location 
// it'll load the images correctly as it tries to load from the 
// full program's local path. Images are in their own subfolders. 
static struct nk_image load_image(const char *argv0, const char *relative_path) {

    int w = 0, h = 0, channels = 0;
    struct nk_image img = {0};
    GLuint tex = 0;

    char full_path[1024];
    build_full_path(argv0, relative_path, full_path, sizeof(full_path));

    unsigned char *pixels = stbi_load(full_path, &w, &h, &channels, 4);
    if (!pixels) {
        fprintf(stderr, "stbi_load failed for '%s': %s\n", full_path, stbi_failure_reason());
        return nk_image_id(0);
    }
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); /* OpenGL 3 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* OpenGL 2 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* OpenGL 2 */

    // glGenerateMipmap(GL_TEXTURE_2D); /* OpenGL 3 */
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); /* OpenGL 2 */

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    img = nk_image_id((int)tex);
    img.w = (unsigned short)w;
    img.h = (unsigned short)h;
    img.region[0] = 0;
    img.region[1] = 0;
    img.region[2] = (unsigned short)w;
    img.region[3] = (unsigned short)h;
    stbi_image_free(pixels);
    
    // Debugging print while loading images (disable to supress like a bazllion lines of ouput!):
    // printf("Loaded '%s' -> w=%d h=%d tex=%u\n", full_path, w, h, (unsigned)tex);
    return img;

}/*end-func*/

// Toggle macro. Turns a bool varible "on" or "off".
// The extra parentheses around b are standard macro safety — without them, 
// if you passed in something like *ptr or a + b, the expansion could parse 
// wrong due to operator precedence.
#define toggle(b) ((b) = !(b)) 

bool is_tooltip_text_enabled = TRUE;
bool user_requested_tooltip_state = TRUE;
bool is_tooltip_path_enabled = FALSE;

/* Convenience macro: grab bounds before widget, show tooltip if hovered */
#define TOOLTIP(widget_call, tip) do {                        \
        struct nk_rect _r = nk_widget_bounds(ctx);            \
        widget_call;                                          \
        if (nk_input_is_mouse_hovering_rect(&ctx->input, _r)) \
        if (is_tooltip_text_enabled) {                        \
            nk_tooltip(ctx, tip);                             \
        }/*end-if*/                                           \
}while(0)/*end-define*/


static inline int is_string_valid(const char *string_to_check) { // Inlined so there's zero runtime cost and const-correct so it won't accidentally mutate
    if ((string_to_check != NULL) && (string_to_check[0] != '\0')) // Two conditions in one: not NULL, and not empty string
        return TRUE;
    else
        return FALSE;
    /*end-if*/
}/*end-func*/

// it uses a static buffer, you can't call it 
// twice in the same snprintf — if you ever need 
// two comma-formatted numbers in one format string 
// the second call will overwrite the first. 
// For this use case with just one number it's perfectly fine though.
static const char *with_commas(long n) {
    static char out[32];
    char tmp[32];
    snprintf(tmp, sizeof(tmp), "%li", n);
    int len = strlen(tmp);
    int j = 0;
    for (int i = 0; i < len; i++) {
        if (i > 0 && (len - i) % 3 == 0) out[j++] = ',';
        out[j++] = tmp[i];
    }
    out[j] = '\0';
    return out;
}

/********************************************************************/
/************************** END FUNCTIONS ***************************/
/********************************************************************/

/********************************************************************************/
/***************************** OPENCBM DRIVES ***********************************/
/********************************************************************************/

// OpenCBM imgcopy supported drives: 1541(D64), 1571(D71), 1581(D81), 8050(D80), 8250(D82), SFD-1001(D82) 
// Note: The 1571 can read and write MFM disks. So I'm not sure if OpenCBM supports that at all. 
// Note: The 1570 is like the 1571 but single sided. This means that any high speed stuff that the 1571 can do, it can also do. 
//       Additionally the 1570 could also do MFM stuff as well but only single-sided. 
//       I'm not sure if, from OpenCBM, there is any reason to support the 1570 specifically. The only thing I can think of is the high-speed stuff. 
//       For now I'm not bothering to support it unless I find something specific in OpenCBM to support anything it does over the 1541.

typedef enum {
    CBM_2040   ,
    CBM_3040   ,
    CBM_4040   ,
    CBM_2031   ,
    CBM_4031   ,
    CBM_1541   ,
    CBM_1541II ,
    CBM_1570   ,
    CBM_1571   ,
    CBM_1581   ,
    CBM_8050   ,
    CBM_8250   ,
    SFD_1001   ,
    OPENCBM_DRIVE_COUNT
} opencbm_drive;

static const char *opencbm_drive_options[] = {
    [ CBM_2040   ] = "CBM 2040    D64",
    [ CBM_3040   ] = "CBM 3040    D64",
    [ CBM_4040   ] = "CBM 4040    D64",
    [ CBM_2031   ] = "CBM 2031    D64",
    [ CBM_4031   ] = "CBM 4031    D64",
    [ CBM_1541   ] = "CBM 1541    D64",
    [ CBM_1541II ] = "CBM 1541-II D64",
    [ CBM_1570   ] = "CBM 1570    D64",
    [ CBM_1571   ] = "CBM 1571    D71",
    [ CBM_1581   ] = "CBM 1581    D81",
    [ CBM_8050   ] = "CBM 8050    D80",
    [ CBM_8250   ] = "CBM 8250    D82",
    [ SFD_1001   ] = "SFD-1001    D82",
};/*end-array*/

static const char *opencbm_drive_detect_strings[][4] = {
    [ CBM_2040   ] = { "2040", NULL        },
    [ CBM_3040   ] = { "3040", NULL        }, /* NOTE: This is not a mistake. Both the 1541 and 1541-II report the same string. */
    [ CBM_4040   ] = { "4040", NULL        },
    [ CBM_2031   ] = { "2031", NULL        },
    [ CBM_4031   ] = { "4031", NULL        },
    [ CBM_1541   ] = { "1541", NULL        },
    [ CBM_1541II ] = { "1541", NULL        },
    [ CBM_1570   ] = { "1570", NULL        },
    [ CBM_1571   ] = { "1571", NULL        },
    [ CBM_1581   ] = { "1581", NULL        },
    [ CBM_8050   ] = { "8050", NULL        },
    [ CBM_8250   ] = { "8250", NULL        },
    [ SFD_1001   ] = { "1001", "SFD", NULL },
};

int current_opencbm_drive_selected = CBM_1541;

/********************************************************************************/
// Safety checks: ensure enum COUNTs stay in sync with their arrays
/********************************************************************************/

_Static_assert(ARRAY_LEN(opencbm_drive_options)         == OPENCBM_DRIVE_COUNT, "opencbm_drive_options out of sync with opencbm_drive enum");
_Static_assert(sizeof(opencbm_drive_detect_strings) / 
               sizeof(opencbm_drive_detect_strings[0])  == OPENCBM_DRIVE_COUNT, "opencbm_drive_detect_strings out of sync with opencbm_drive enum");

/******************************************************************************/

/********************************************************************************/
/********************************************************************************/
/************** DATA DICTIONARY - HARDWARE DEFAULTS *****************************/
/********************************************************************************/
/********************************************************************************/

// Maybe turn this section into it's own file at some point.
// #include "window_hardware.c"

#define ENABLED  TRUE
#define DISABLED FALSE

/* --- Hardware Window --- */            /* --- Hardware Defaults --- */
static int hw_gw_drive                   = 0;     /* 0=A/0,    1=B/1 */
static int hw_gw_size                    = 0;     /* 0=5.25,   1=3.5 */
static int hw_gw_density                 = 0;     /* 0=DD,     1=HD */
static int hw_gw_tracks                  = 0;     /* 0=40,     1=80 */
static int hw_gw_steps                   = 0;     /* 0=Single, 1=Double */
static int hw_gw_reset                   = 1;     /* checkbox */
 
// static int hw_zf_side_mode            = 0;     /* 0=Single, 1=Double */
// static int hw_zf_confirm              = 0;     /* checkbox */
// static int hw_zf_format               = 0;     /* checkbox */
static int hw_zf_reset                   = FALSE; /* This resets the IEC bus before doing a thing but the default here is to be UNCHECKED. But this isn't necessary because it already resets the bus when it looks for what drives are connected. But I'll leave it as an option. */
static int hw_zf_iec_device              = 8;     /* IEC drive number */
static int hw_zf_bam_only                = TRUE;  /* OpenCBM option to enable BAM-Only which copies only used blocks and not the whole disk and is thus faster. */
static int hw_zf_format_first            = FALSE; /* Sends the IEC device the format command before writing a disk. */
static int hw_usb_floppy_format_first    = TRUE;  /* Uses local system to format the drive first. This is needed because you can't write a disk with dd on Windows 11 without formatting it first. TODO: When doing a Linux and Mac OS version this will be different probably. */

static const int  floppy_presets[]        = {737280, 1474560};
static const char *floppy_preset_names[]  = {"737280 (DD)", "1474560 (HD)"};
static int        hw_usb_floppy_preset    = 0;
static int        hw_usb_floppy_size      = 737280;
static int        hw_usb_block_size       = 9216;
static int        hw_usb_block_count      = 80;
static char       hw_usb_drive_letter[4]  = "A:";

static int        hw_gw_drive_type        = 1;  /* 0 = Sugart, 1 = PC Bus */
static int        hw_gw_retries           = 3;  /* default 3, range 0-10  */
static int        hw_gw_overwrite         = 0;  /* 0 = off, 1 = on        */
static int        hw_gw_verify            = ENABLED;  /* 0 = off, 1 = on        */

// Had to make this big-ass to prevent GCC compiler warning and RAM is cheap who cares... wait a min... AI is making RAM expensive... okay I'll update this later!
char temp_copy_path[MAX_PATH * 2]; // This is the temp file to use if we need to rename, convert, massage, whatever the file before sending it to Greaseweazle or soemthing else. 
char temp_copy_path_target[MAX_PATH * 2]; // This is the temp file for the filename that we will eventually rename the temp_copy_path to. For example: convert somefile.farts to somefile.poops where the farts file in the inbetween file and poops is the file that MIGHT already exist when we RENAME farts to poops. So we need to check for that. 
char copy_cmd[(MAX_PATH * 4) * 2 + 16];
char temp_gw_convert_fullpathfile[MAX_PATH * 4];

// If this is enabled we set a special string for this. This is for cmd.exe. 
// NOTE: This is the default. Since the code for this runs ONLY IF the 
// Nuklear form is being processed (ie displayed). But by default that form
// isn't displayed. So we set this here to be disabled by default. 
// Meaning that it's just an echo statement and not the command to 
// actually reset the Greaseweazle. However, by default I actually 
// enable this. Therefore there is also code at the beginning 
// of the program to set this based on the default state of the varible. 
// Basically if I set hw_gw_reset=1 then I need to also set the 
// string. So I'll do that with code because otherwise that 
// default is never realized because the Nuklear form code
// isn't displayed by default and therefore hw_gw_reset=1 never 
// gets set. That was a lot sorry but this bug was really bugging me!
char gw_reset_string[MAX_PATH * 4] = "echo \"Greaseweazle reset disabled.\""; 

// Used for error checking before calling the command line to run our command and do disk stuff!
int passed_all_checks = TRUE;

static enum nk_collapse_states greaseweazle_tree_state = NK_MAXIMIZED;
static enum nk_collapse_states zoomfloppy_tree_state   = NK_MINIMIZED;
static enum nk_collapse_states usbfloppy_tree_state    = NK_MINIMIZED;


/************************************************************************/
/****************************** DIRECTION *******************************/
/************************************************************************/

// This is for the direction of what the user is doing. 
// 
// File to Floppy
// Loading a floppy disk image file and writing it to physical floppy disk.
//
// Floppy to File
// Reading a physical floppy disk and saving it to a floppy disk image file.

static const char *direction_options[] = {"File to Floppy>", "Floppy to File<"};

typedef enum {
    WRITING_A_FLOPPY = 0, /* --> file2floppy == loading a file and writing to a floppy */
    READING_A_FLOPPY = 1, /* --> floppy2file == reading a floppy and saving to a file */
    DIRECTION_COUNT
} DIRECTION_SELECTED_TYPE;

static int direction_selected      = WRITING_A_FLOPPY;
static int direction_selected_prev = WRITING_A_FLOPPY;

/************************************************************************/
/************************** END OF DIRECTION ****************************/
/************************************************************************/

/************************** HARDWARE CHOICES ****************************/
#define GREASEWEAZLE_CHOSEN 0
#define ZOOMFLOPPY_CHOSEN   1
#define USBFLOPPY_CHOSEN    2
/************************************************************************/

static int hardware_selected          = GREASEWEAZLE_CHOSEN;
static int previous_hardware_selected = GREASEWEAZLE_CHOSEN;

static const char *hardware_options[] = {"Greaseweazle", "ZoomFloppy", "USB Floppy"};
int current_hardware_count            = ARRAY_LEN(hardware_options);

const char **current_computer_options = commodore_systems_options;            // Default to Amiga.  // const char **current_computer_options = amiga_systems_options; // Default to Amiga. 
int current_computer_count            = ARRAY_LEN(commodore_systems_options); // int current_computer_count            = ARRAY_LEN(amiga_systems_options);
 
const char **current_drive_options    = commodore_drive_options;              // Default to Amiga.  // const char **current_drive_options    = amiga_drive_options; // Default to Amiga. 
int current_drive_count               = ARRAY_LEN(commodore_drive_options);   // int current_drive_count               = ARRAY_LEN(amiga_drive_options);

// This crashes when you pick somethign else then commodore wtf???
static int brand_selected      = BRAND_AMIGA;   
static int computer_selected   = AMIGA_500;           
static int drive_selected      = AMIGA_DRIVE_1010;  
int previous_computer_selected = -1;
int previous_brand_selected    = -1;
int previous_drive_selected    = -1;

// Commnd Line String --> Used to execute the final task on the command line! Also shown in the Details window.
static char gw_cmd_string[MAX_PATH * 5] = "";
static char cmd_string[MAX_PATH*32]     = ""; // TODO: All these strings are probably way too big!

static char zf_reset_string[MAX_PATH * 5]  = "";
static char zf_format_string[MAX_PATH * 7] = "";
static char zf_cmd_string[MAX_PATH * 7]    = "";

static char uf_format_string[512] = "";
static char usb_cmd_string[1024]  = "";

// Window States - Let's us hide and show a window. 
static int task_open     = TRUE  ; 
static int hardware_open = TRUE  ; 
// static int details_open  = FALSE ; // NOT DOING THIS ANYMORE
static int options_open  = FALSE ;
static int about_open    = FALSE ;

// ============================================================
// cbmctrl detect-based drive verification for ZoomFloppy/IEC
// ============================================================

// Run cbmctrl detect, capture output.
// Returns number of lines parsed, or -1 on failure.
// detected_devs: array of device numbers found (caller provides, size MAX_IEC_DEVS)
// detected_models: parallel array of model strings (caller provides)

#define MAX_IEC_DEVS 16
#define DETECT_MODEL_LEN 64

static int run_cbmctrl_detect(
    int      detected_devs[MAX_IEC_DEVS],
    char     detected_models[MAX_IEC_DEVS][DETECT_MODEL_LEN],
    char    *raw_out,
    int      raw_out_size
) {
    // Set up pipes
    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
    CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);

    // Set up process
    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb         = sizeof(si);
    si.dwFlags    = STARTF_USESTDHANDLES;
    si.hStdOutput = hWritePipe;
    si.hStdError  = hWritePipe;
    si.hStdInput  = GetStdHandle(STD_INPUT_HANDLE);

    // char command_string[] = "cbmctrl detect";
    char command_string[MAX_PATH * 3];

    snprintf(command_string, sizeof(command_string), "\"%s\\%s\" detect", options_opencbm_path, options_opencbm_cbmctrl_executable);

    if (!CreateProcessA(NULL, command_string, NULL, NULL, TRUE,
                        CREATE_NO_WINDOW,
                        NULL, NULL, &si, &pi)) {
        CloseHandle(hReadPipe);
        CloseHandle(hWritePipe);
        return -1;
    }

    // Close write end in parent so ReadFile doesn't hang
    CloseHandle(hWritePipe);

    // Read all output line by line
    int  raw_pos = 0;
    int  count   = 0;
    char line[128];
    int  line_pos = 0;
    char ch;
    DWORD bytesRead;

    while (ReadFile(hReadPipe, &ch, 1, &bytesRead, NULL) && bytesRead > 0) {
        // Accumulate into line buffer
        if (line_pos < (int)sizeof(line)-1) {
            line[line_pos++] = ch;
        }
        // Process line on newline
        if (ch == '\n') {
            line[line_pos] = '\0';

            // Accumulate raw output
            int remaining = raw_out_size - raw_pos - 1;
            if (remaining > 0) {
                int n = snprintf(raw_out + raw_pos, remaining, "%s", line);
                if (n > 0) raw_pos += n;
            }

            // Parse " 8: 1571" or "10: 1581" etc.
            int  devnum = 0;
            char model[DETECT_MODEL_LEN] = {0};
            if (sscanf(line, " %d: %63[^\n]", &devnum, model) == 2 && count < MAX_IEC_DEVS) {
                detected_devs[count] = devnum;
                strncpy(detected_models[count], model, DETECT_MODEL_LEN - 1);
                count++;
            }

            line_pos = 0; // reset for next line
        }
    }

    raw_out[raw_pos] = '\0';

    // Clean up
    CloseHandle(hReadPipe);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return count;
}/*end-func*/

// Check whether a detected model string matches the expected drive type.
static int model_matches_drive(const char *model, int drive_enum) {
    if (drive_enum < 0 || drive_enum >= OPENCBM_DRIVE_COUNT) return 0;
    for (int i = 0; i < 4; i++) {
        const char *substr = opencbm_drive_detect_strings[drive_enum][i];
        if (!substr) break;
        if (strstr(model, substr)) return 1;
    }
    return 0;
}

// Call this when the user clicks your "Detect / Verify" button.
// hw_zf_iec_device       = the device number they picked (e.g. 8)
// current_opencbm_drive_selected = their chosen drive enum (e.g. CBM_1571)
int verify_zoomfloppy_drive(int hw_zf_iec_device, int current_opencbm_drive_selected) {
    int  detected_devs[MAX_IEC_DEVS]                      = {0};
    char detected_models[MAX_IEC_DEVS][DETECT_MODEL_LEN]  = {{0}};
    char raw_out[1024]                                    = {0};

    int count = run_cbmctrl_detect(detected_devs, detected_models, raw_out, sizeof(raw_out));

    if (count < 0) {
        MessageBoxA(NULL,
            "Failed to run 'cbmctrl detect'.\n"
            "Make sure opencbm is installed and your ZoomFloppy is connected.\n",
            "Drive Detection Failed",
            MB_OK | MB_ICONERROR
        );
        return FALSE;
    }

    // Look for the user's chosen device number in the detect output
    int found_dev   = 0;
    int model_match = 0;
    char found_model[DETECT_MODEL_LEN] = {0};

    for (int i = 0; i < count; i++) {
        if (detected_devs[i] == hw_zf_iec_device) {
            found_dev = 1;
            strncpy(found_model, detected_models[i], DETECT_MODEL_LEN - 1);
            model_match = model_matches_drive(found_model, current_opencbm_drive_selected);
            break;
        }
    }

    if (!found_dev) {
        char msg[2048];
        snprintf(msg, sizeof(msg),
            "Device %d was not detected on the IEC bus.\n\n"
            "Detected devices:\n%s\n"
            "Check that your drive is powered on and connected.",
            hw_zf_iec_device,
            raw_out[0] ? raw_out : "(no devices found)"
        );
        MessageBoxA(NULL, msg, "Drive Not Found.", MB_OK | MB_ICONWARNING);
        return FALSE;
    }

    if (!model_match) {
        char msg[2048];
        snprintf(msg, sizeof(msg),
            "Device %d was detected as \"%s\",\n"
            "but you have selected \"%s\".\n\n"
            "Full detect output:\n%s",
            hw_zf_iec_device,
            found_model,
            opencbm_drive_options[current_opencbm_drive_selected],
            raw_out
        );
        MessageBoxA(NULL, msg, "Drive Mismatch", MB_OK | MB_ICONWARNING);
        return FALSE;
    }

    return TRUE;

    // All good — optionally show a success toast/status bar update here
    // or just stay silent and let the user proceed

}/*end-func*/

// Call this when the user clicks your "Detect / Verify" button.
// hw_zf_iec_device       = the device number they picked (e.g. 8)
// current_opencbm_drive_selected = their chosen drive enum (e.g. CBM_1571)
void display_zoomfloppy_drives() {
    int  detected_devs[MAX_IEC_DEVS]                      = {0};
    char detected_models[MAX_IEC_DEVS][DETECT_MODEL_LEN]  = {{0}};
    char raw_out[1024]                                    = {0};

    int count = run_cbmctrl_detect(detected_devs, detected_models, raw_out, sizeof(raw_out));

    if (count < 0) {
        MessageBoxA(NULL,
            "Failed to run 'cbmctrl detect'.\n"
            "Make sure opencbm is installed and your ZoomFloppy is connected.\n",
            "Drive Detection Failed",
            MB_OK | MB_ICONERROR
        );
        return;
    }

    char msg[2048];
    snprintf(msg, sizeof(msg),
        "ZoomFloppy / OpenCBM (cbmctrl) has scanned for devices.\n\n"
        "Detected devices:\n%s\n" ,
        raw_out[0] ? raw_out : "(no devices found)"
    );
    MessageBoxA(NULL, msg, "ZoomFloppy / OpenCBM Device Scan", MB_OK | MB_ICONWARNING);

    return;

}/*end-func*/

static int verify_greaseweazle_drive(int drive_type, int drive_ident) {
    char command_string[MAX_PATH * 3];
    char drive_ident_char;
    int  greaseweazle_detected = -1;

    // If either of these strings: options_greaseweazle_path or options_greaseweazle_executable are empty, fail outta here!
    if ( !is_string_valid(options_greaseweazle_path) || !is_string_valid(options_greaseweazle_executable) ) {
        printf("One of these strings: options_greaseweazle_path or options_greaseweazle_executable is empty!\n");
        return -1; 
    };/*end-if*/

    if      (drive_type==0) drive_ident_char = drive_ident + '0';
    else if (drive_type==1) drive_ident_char = drive_ident + 'A';

    // "gw rpm --drive A"
    snprintf(command_string, sizeof(command_string), "\"%s\\%s\" rpm --drive %c",options_greaseweazle_path, options_greaseweazle_executable, drive_ident_char); /* %s\\%s options_greaseweazle_path, options_greaseweazle_executable, */

    printf("command_string:%s\n", command_string);

    // Set up pipes
    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
    CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);

    // Set up process
    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb         = sizeof(si);
    si.dwFlags    = STARTF_USESTDHANDLES;
    si.hStdOutput = hWritePipe;
    si.hStdError  = hWritePipe;  // capture stderr too, same as your 2>&1
    si.hStdInput  = GetStdHandle(STD_INPUT_HANDLE);

    if (!CreateProcessA(NULL, command_string, NULL, NULL, TRUE,
                        CREATE_NO_WINDOW,  // <-- this is the magic flag
                        NULL, NULL, &si, &pi)) {
        CloseHandle(hReadPipe);
        CloseHandle(hWritePipe);
        MessageBox(NULL,
            "Error: Can't seem to run Greaseweazle verify test.\n\n"
            "Check your Greaseweazle and configuration in Options.",
            "Verify Greaseweazle", 
            MB_OK | MB_ICONINFORMATION
        );/*end-MessageBox*/
        greaseweazle_detected = -1;
        return greaseweazle_detected; // return -1;
    }

    // Close write end in parent so ReadFile doesn't hang
    CloseHandle(hWritePipe);

    // Read the first line
    char line[128] = {0};
    DWORD bytesRead;
    ReadFile(hReadPipe, line, sizeof(line)-1, &bytesRead, NULL);

    // Clean up
    CloseHandle(hReadPipe);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    printf("line:`%s`\n", line);

    if (strstr(line, "** FATAL ERROR:") != NULL) {
        
        printf("Greaseweazle ERROR: found this: --> ** FATAL ERROR:\n");
        greaseweazle_detected = FALSE;
        MessageBox(NULL, "Cannot find the Greaseweazle device!\nPlease check your connections and settings.\nGreaseweazle: ** FATAL ERROR:", "Error", MB_OK | MB_ICONERROR);
    
    } else if (strstr(line, "Command Failed: GetFluxStatus: No Index") != NULL) {
        
        printf("Greaseweazle ERROR: found this: --> Command Failed: GetFluxStatus: No Index\n");
        greaseweazle_detected = FALSE;
        MessageBox(NULL, "Greaseweazle cannot detect a working disk in the drive!\nPlease check your connections and settings.\nGreaseweazle: Command Failed: GetFluxStatus: No Index", "Error", MB_OK | MB_ICONERROR);
    
    } else if (strstr(line, "Rate: ") != NULL && strstr(line, " rpm ; Period: ") != NULL && strstr(line, " ms") != NULL) {
        
        printf("Greaseweazle SUCCESS!!!: found this: --> %s \n", line);
        greaseweazle_detected = TRUE;
    
    } else {
        
        printf("Greaseweazle ERROR: ELSE --> Unrecognised output: %s\n", line);
        MessageBox(NULL, "Greaseweazle returned an error!\nPlease check your connections and settings.", "Error", MB_OK | MB_ICONERROR);
        greaseweazle_detected = FALSE;
    
    }

    return greaseweazle_detected;

}/*end-func*/

static int show_greaseweazle_bandwidth() {
    char command_string[MAX_PATH * 3];
    snprintf(command_string, sizeof(command_string), "\"%s\\%s\" bandwidth",
             options_greaseweazle_path, options_greaseweazle_executable);
    printf("command_string:%s\n", command_string);

    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
    CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);

    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb         = sizeof(si);
    si.dwFlags    = STARTF_USESTDHANDLES;
    si.hStdOutput = hWritePipe;
    si.hStdError  = hWritePipe;
    si.hStdInput  = GetStdHandle(STD_INPUT_HANDLE);

    if (!CreateProcessA(NULL, command_string, NULL, NULL, TRUE,
                        CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(hReadPipe);
        CloseHandle(hWritePipe);
        MessageBox(NULL,
            "Error: Can't seem to run Greaseweazle bandwidth test.\n\n"
            "Check your Greaseweazle and configuration in Options.",
            "Greaseweazle Bandwidth Results", MB_OK | MB_ICONINFORMATION);
        return -1;
    }

    CloseHandle(hWritePipe);  // close write end in parent so ReadFile doesn't hang

    // ---- Loop until the pipe is drained ----
    char output[4096] = {0};
    char chunk[512];
    DWORD bytesRead;
    size_t totalLen = 0;

    while (ReadFile(hReadPipe, chunk, sizeof(chunk) - 1, &bytesRead, NULL) && bytesRead > 0) {
        chunk[bytesRead] = '\0';
        // Append to output, guarding against overflow
        if (totalLen + bytesRead < sizeof(output) - 1) {
            memcpy(output + totalLen, chunk, bytesRead);
            totalLen += bytesRead;
            output[totalLen] = '\0';
        }
    }
    
    // ----------------------------------------

    CloseHandle(hReadPipe);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    char *trimmed = output;
    while (*trimmed && isspace((unsigned char)*trimmed)) trimmed++;

    char padded[4096+1024];
    snprintf(padded, sizeof(padded), "Greaseweazle Minimum Requirement: 4.627 Mbps\nGreaseweazle Bandwidth Tests Output:\n                                  %s", trimmed);  // 10 spaces
    printf("trimmed:`%s`\n", trimmed);
    MessageBox(NULL, padded, "Greaseweazle Bandwidth Results", MB_OK | MB_ICONINFORMATION);
    
    return TRUE;

}/*end-func*/

int is_floppy_drive_connected(void) {
    GUID floppy_guid = {0x4d36e980, 0xe325, 0x11ce, {0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18}};
    HDEVINFO dev_info = SetupDiGetClassDevs(&floppy_guid, NULL, NULL, DIGCF_PRESENT);
    if (dev_info == INVALID_HANDLE_VALUE) return 0;

    SP_DEVINFO_DATA dev_data;
    dev_data.cbSize = sizeof(SP_DEVINFO_DATA);
    int found = SetupDiEnumDeviceInfo(dev_info, 0, &dev_data);
    SetupDiDestroyDeviceInfoList(dev_info);
    return found ? 1 : 0;
}/*end-func*/

// Returns: 1 = disk present, 0 = no disk, -1 = no drive
int is_floppy_disk_inserted(char drive_letter) {
    char path[8];
    snprintf(path, sizeof(path), "\\\\.\\%c:", drive_letter);

    HANDLE handle = CreateFileA(
        path, 0,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING,
        FILE_FLAG_NO_BUFFERING, NULL
    );

    if (handle == INVALID_HANDLE_VALUE) return -1;

    DWORD bytes = 0;
    // IOCTL_STORAGE_CHECK_VERIFY2 = 0x002D0800
    BOOL result = DeviceIoControl(
        handle, 0x002D0800,
        NULL, 0, NULL, 0,
        &bytes, NULL
    );

    CloseHandle(handle);
    return result ? 1 : 0;
}/*end-func*/

static int verify_usbfloppy_drive(char drive_letter) {

    int usbfloppy_detected = -1; // Testing what happens. 
    char error_message_string[128];

    if (!is_floppy_drive_connected()) {
        printf("No floppy drive(s) connected.\n");
        usbfloppy_detected = FALSE;
    };/*end-if*/

    printf("Floppy drive(s) detected.\n");

    switch (is_floppy_disk_inserted(drive_letter)) {
        case  1: 
            usbfloppy_detected = TRUE;  
            printf("Drive %c: Disk is inserted.\n", drive_letter); 
        break;

        case  0: 
            usbfloppy_detected = FALSE; 
            snprintf(error_message_string,sizeof(error_message_string),"Drive %c: No disk in drive.\n",drive_letter);
            MessageBox(NULL, error_message_string, "Error", MB_OK | MB_ICONERROR);
            printf("%s", error_message_string); 
        break;

        case -1: 
            usbfloppy_detected = FALSE; 
            snprintf(error_message_string,sizeof(error_message_string),"Drive %c: Could not open drive.\n",drive_letter);
            MessageBox(NULL, error_message_string, "Error", MB_OK | MB_ICONERROR);
            printf("%s", error_message_string);
        break;
    };/*end-switch*/

    return usbfloppy_detected;

}/*end-func*/

// Fills out_buf with the hardware description string from Device Manager.
// Returns 1 on success, 0 on failure.
int get_floppy_device_string(char *out_buf, DWORD buf_size) {
    GUID floppy_guid = {0x4d36e980, 0xe325, 0x11ce, {0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18}};
    HDEVINFO dev_info = SetupDiGetClassDevs(&floppy_guid, NULL, NULL, DIGCF_PRESENT);
    if (dev_info == INVALID_HANDLE_VALUE) return 0;

    SP_DEVINFO_DATA dev_data;
    dev_data.cbSize = sizeof(SP_DEVINFO_DATA);

    if (!SetupDiEnumDeviceInfo(dev_info, 0, &dev_data)) {
        SetupDiDestroyDeviceInfoList(dev_info);
        return 0;
    }

    // Try friendly name first (e.g. "TEAC FD-05PUB USB Device")
    // Fall back to device description if friendly name isn't set
    if (!SetupDiGetDeviceRegistryPropertyA(dev_info, &dev_data, SPDRP_FRIENDLYNAME,
            NULL, (PBYTE)out_buf, buf_size, NULL)) {
        if (!SetupDiGetDeviceRegistryPropertyA(dev_info, &dev_data, SPDRP_DEVICEDESC,
                NULL, (PBYTE)out_buf, buf_size, NULL)) {
            strncpy(out_buf, "Unknown device", buf_size - 1);
            out_buf[buf_size - 1] = '\0';
            SetupDiDestroyDeviceInfoList(dev_info);
            return 0;
        }
    }

    SetupDiDestroyDeviceInfoList(dev_info);
    return 1;
}

/******************************************************************/

typedef struct {
    char model[64];
    char firmware[32];
    char serial[64];
    char usb_speed[64];
    char mcu[64];
    char host_tools[32];
    int  found;
} GreasewazleInfo;

typedef struct {
    char version[32];
    char build_date[32];
    char build_time[32];
    int  found;
    int  device_count;
    char device_count_string[32];
} ZoomFloppyInfo;

/******************************************************************/

// Runs a command and captures its stdout into out_buf.
// Returns 1 on success, 0 on failure.
static int run_command(const char *cmd, char *out_buf, size_t buf_size) {
    // Set up pipes
    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
    CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);

    // Set up process
    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb         = sizeof(si);
    si.dwFlags    = STARTF_USESTDHANDLES;
    si.hStdOutput = hWritePipe;
    si.hStdError  = hWritePipe;
    si.hStdInput  = GetStdHandle(STD_INPUT_HANDLE);

    // CreateProcessA needs a non-const char* so copy it
    char command_string[512];
    strncpy(command_string, cmd, sizeof(command_string)-1);
    command_string[sizeof(command_string)-1] = '\0';

    if (!CreateProcessA(NULL, command_string, NULL, NULL, TRUE,
                        CREATE_NO_WINDOW,
                        NULL, NULL, &si, &pi)) {
        CloseHandle(hReadPipe);
        CloseHandle(hWritePipe);
        return 0;
    }

    // Close write end in parent so ReadFile doesn't hang
    CloseHandle(hWritePipe);

    // Read all output
    size_t total = 0;
    DWORD  bytesRead;
    char   ch;

    while (ReadFile(hReadPipe, &ch, 1, &bytesRead, NULL) && bytesRead > 0) {
        if (total < buf_size - 1) {
            out_buf[total++] = ch;
        }
    }
    out_buf[total] = '\0';

    // Clean up
    CloseHandle(hReadPipe);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return total > 0 ? 1 : 0;
}/*end-func*/

// Parses a line like "  Key:     Value" and copies Value into out_buf.
static void parse_gw_field(const char *line, const char *key, char *out_buf, size_t buf_size) {
    const char *p = strstr(line, key);
    if (!p) return;
    p += strlen(key);
    while (*p == ' ' || *p == '\t') p++;  // skip whitespace
    strncpy(out_buf, p, buf_size - 1);
    out_buf[buf_size - 1] = '\0';
    // trim trailing \r\n
    int len = strlen(out_buf);
    while (len > 0 && (out_buf[len-1] == '\r' || out_buf[len-1] == '\n'))
        out_buf[--len] = '\0';
}

void get_greaseweazle_info(GreasewazleInfo *info) {
    
    char command_string[MAX_PATH * 3];

    memset(info, 0, sizeof(*info));

    char output[2048] = {0};

    snprintf( command_string , sizeof(command_string) , "\"%s\\%s\" info" , options_greaseweazle_path , options_greaseweazle_executable ); 

    // if (!run_command("gw info 2>&1", output, sizeof(output))) { 
    if (!run_command(command_string, output, sizeof(output))) {
        strcpy(info->model, "Not found.");
        return;
    }

    printf("get_greaseweazle_info output:`%s` \n", output);

    // Walk line by line
    char *line = strtok(output, "\n");
    while (line) {
        parse_gw_field(line, "Host Tools:",  info->host_tools, sizeof(info->host_tools));
        parse_gw_field(line, "Model:",       info->model,      sizeof(info->model));
        parse_gw_field(line, "MCU:",         info->mcu,        sizeof(info->mcu));
        parse_gw_field(line, "Firmware:",    info->firmware,   sizeof(info->firmware));
        parse_gw_field(line, "Serial:",      info->serial,     sizeof(info->serial));
        parse_gw_field(line, "USB:",         info->usb_speed,  sizeof(info->usb_speed));
        line = strtok(NULL, "\n");
    }

    info->found = (info->model[0] != '\0') ? 1 : 0;
    if (!info->found) strcpy(info->model, "Not found.");
}

// cbmctrl --version output:
// "cbmctrl version 0.4.99.104, built on Jan 22 2022 at 22:58:48"
void get_zoomfloppy_info(ZoomFloppyInfo *info) {

    char command_string[MAX_PATH * 3];

    memset(info, 0, sizeof(*info));

    char output[2048] = {0};

    snprintf( command_string , sizeof(command_string) , "\"%s\\%s\" --version" , options_opencbm_path , options_opencbm_cbmctrl_executable ); 

    if (!run_command(command_string, output, sizeof(output))) {
        strcpy(info->version,    "Not found.");
        strcpy(info->build_date, "Not found.");
        strcpy(info->build_time, "Not found.");
        return;
    }

    printf("get_zoomfloppy_info output:`%s` \n", output);

    // Parse manually -- find each known keyword and extract what follows
    char *p;

    // Version: grab everything between "version " and ","
    p = strstr(output, "version ");
    if (p) {
        p += 8; // skip "version "
        char *end = strchr(p, ',');
        if (end) {
            int vlen = end - p;
            if (vlen >= (int)sizeof(info->version)) vlen = sizeof(info->version) - 1;
            strncpy(info->version, p, vlen);
            info->version[vlen] = '\0';
        }
    }

    // Build date: grab everything between "built on " and " at "
    p = strstr(output, "built on ");
    if (p) {
        p += 9; // skip "built on "
        char *end = strstr(p, " at ");
        if (end) {
            int dlen = end - p;
            if (dlen >= (int)sizeof(info->build_date)) dlen = sizeof(info->build_date) - 1;
            strncpy(info->build_date, p, dlen);
            info->build_date[dlen] = '\0';
        }
    }

    // Build time: grab everything after " at "
    p = strstr(output, " at ");
    if (p) {
        p += 4; // skip " at "
        strncpy(info->build_time, p, sizeof(info->build_time) - 1);
        info->build_time[sizeof(info->build_time) - 1] = '\0';
        // trim trailing whitespace/\r\n
        int tlen = strlen(info->build_time);
        while (tlen > 0 && (info->build_time[tlen-1] == '\r' ||
                            info->build_time[tlen-1] == '\n' ||
                            info->build_time[tlen-1] == ' '))
            info->build_time[--tlen] = '\0';
    }

    if (info->version[0] && info->build_date[0] && info->build_time[0]) {
        info->found = 1;
    } else {
        strcpy(info->version,    "Unknown.");
        strcpy(info->build_date, "Unknown.");
        strcpy(info->build_time, "Unknown.");
    }
}

typedef struct {
    int  found;
    char version[32];
} CiderPress2Info;

void get_ciderpress2_info(CiderPress2Info *info) {
    char command_string[MAX_PATH * 3];
    memset(info, 0, sizeof(*info));
    char output[2048] = {0};
    snprintf(command_string, sizeof(command_string), "\"%s\\%s\" help",
             options_ciderpress2cli_path, options_ciderpress2_cli_executable);
    if (!run_command(command_string, output, sizeof(output))) {
        strcpy(info->version, "Not found.");
        return;
    }
    printf("get_ciderpress2_info output:`%s` \n", output);
    // The version is on the first line: "CiderPress II Command-Line Utility v1.2.0"
    char *line = strtok(output, "\r\n");
    if (line) {
        // Find the 'v' that precedes the version number
        char *v = strrchr(line, 'v');
        if (v) {
            strncpy(info->version, v, sizeof(info->version) - 1);
            info->version[sizeof(info->version) - 1] = '\0';
            info->found = 1;
        }
    }
    if (!info->found) {
        strcpy(info->version, "Not found.");
    }
}

typedef struct {
    int  found;
    char version[32];
} DDInfo;

void get_dd_info(DDInfo *info) {
    char command_string[MAX_PATH * 3];
    memset(info, 0, sizeof(*info));
    char output[2048] = {0};
    snprintf(command_string, sizeof(command_string), "\"%s\\%s\" --help",
             options_dd_path, options_dd_executable);
    if (!run_command(command_string, output, sizeof(output))) {
        strcpy(info->version, "Not found.");
        return;
    }
    printf("get_dd_info output:`%s` \n", output);
    // The version is on the first line: "rawwrite dd for windows version 0.6beta3."
    char *line = strtok(output, "\r\n");
    if (line) {
        // Find the last space, version number follows it
        char *v = strrchr(line, ' ');
        if (v) {
            v++; // skip the space itself
            strncpy(info->version, v, sizeof(info->version) - 1);
            info->version[sizeof(info->version) - 1] = '\0';
            // Strip trailing period if present
            size_t len = strlen(info->version);
            if (len > 0 && info->version[len - 1] == '.')
                info->version[len - 1] = '\0';
            info->found = 1;
        }
    }
    if (!info->found) {
        strcpy(info->version, "Not found.");
    }
}

/******************************************************************/
/********************** GLOBAL VARIABLES **************************/
/******************************************************************/

// TODO: This obviously is NOT all the global varibles. Fuck. 
//       I need to clean that shit up!

static char recent_files[3][MAX_PATH] = {{0}, {0}, {0}}; //Currently selected file should be recent_files[image_selected-2]
static int  recent_count              = 0;
static int  image_selected            = 0;
static int  previous_image_selected   = -1;

char previously_selected_file[MAX_PATH];
long currently_selected_file_size = -69;
char currently_selected_file_ext[MAX_PATH] = "";
bool has_currently_selected_file_changed = FALSE;

/************************************************************/
/********************* DRAG AND DROP ************************/
/************************************************************/

long get_file_size(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) return -1;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fclose(f);
    return size; // returns size in bytes, or -1 on failure
}

int sizes_within_1_percent(long a, long b) {
    if (a <= 0 || b <= 0) return 0;
    long diff = a - b;
    if (diff < 0) diff = -diff; // abs value
    return diff <= b / 100;
}

// Note that this checks if a is within 10% of b specifically 
// — if you need a symmetric check (within 10% of either), 
// you'd want to compare against the larger of the two values.
int sizes_within_10_percent(long a, long b) {
    if (a <= 0 || b <= 0) return 0;
    long diff = a - b;
    if (diff < 0) diff = -diff; // abs value
    return diff <= b / 10;
}

int within_range(long val, long low, long high) {
    return val >= low && val <= high;
}

/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/

static inline FORMAT_ID_TYPE get_format_from_ext_and_size(bool questions_are_enabled, const long file_size, const char *nodot) { // The questions_are_enabled is so we can turn off the message box triggers. // The nodot is like the extension with the `.` in it. 

    printf("get format from ext and size(questions_are_enabled, file_size, nodot):  `%i`, `%li`, `%s` \n", questions_are_enabled, file_size, nodot);

    printf("File extension: %s\n", nodot);

    /******************************************************************/
    /****************** File Extension Processing *********************/
    /******************************************************************/  
    if ( strcasecmp(nodot,"ADF") == 0 ) { 
        printf( "Amiga / BBC Micro / Archimedes disk image detected!\n" ); // match nodot against main_formats[] to auto-select the format.

        // Amiga File Sizes
        long amiga_dd_size = main_formats[ amiga_ados_350_dsdd_0880k_adf ].size_kb * 1024; 
        long amiga_hd_size = main_formats[ amiga_ados_350_dshd_1760k_adf ].size_kb * 1024; 

        // BBC Micro File Sizes
        long bbc_160_size  = main_formats[ bbc_adfss_525_ssdd_0160k_adl ].size_kb * 1024; 
        long bbc_320_size  = main_formats[ bbc_adfsm_525_ssqd_0320k_adl ].size_kb * 1024; 
        long bbc_640_size  = main_formats[ bbc_adfsl_525_dsqd_0640k_adl ].size_kb * 1024; 

        // Archimedes File Sizes
        long arch_800_size  =  800 * 1024;
        long arch_1600_size = 1600 * 1024;

        if (file_size < 160 * 1024) {
            printf("ADF file size is too small to match any known disk image sizes. The disk image file might be corrupt or unusual.\n");
            if (questions_are_enabled) MessageBox(NULL, "ADF file size is too small to match any known disk image sizes. The disk image file might be corrupt or unusual.", "Info", MB_OK | MB_ICONINFORMATION);
            return FORMAT_ID_COUNT ; // NOTE: We are returning the COUNT because it's a marker that we can't figure this out! TODO: Is this bad? 

        } else if (sizes_within_1_percent(file_size, bbc_160_size)) {
            printf("BBC Micro ADFS-S disk detected! - bbc_adfss_525_ssdd_0160k_adl\n");
            return bbc_adfss_525_ssdd_0160k_adl ;

        } else if (sizes_within_1_percent(file_size, bbc_320_size)) {
            printf("BBC Micro ADFS-M disk detected! - bbc_adfsm_525_ssqd_0320k_adl\n");
            return bbc_adfsm_525_ssqd_0320k_adl ;

        } else if (sizes_within_1_percent(file_size, bbc_640_size)) {
            printf("BBC Micro ADFS-L disk detected! - bbc_adfsl_525_dsqd_0640k_adl\n");
            return bbc_adfsl_525_dsqd_0640k_adl ;

        } else if (sizes_within_1_percent(file_size, arch_800_size)) { // within 1%  800 arch_adfsd_350_dsdd_0800k_adl & arch_adfse_350_dsdd_0800k_adl
            printf("Acorn Archimedes HD 800 KB Disk Image detected! - arch_adfsd_350_dsdd_0800k_adl\n");
            return arch_adfsd_350_dsdd_0800k_adl ;

        } else if (sizes_within_1_percent(file_size, amiga_dd_size)) {
            printf("Amiga DD disk detected! - amiga_ados_350_dsdd_0880k_adf \n");
            return amiga_ados_350_dsdd_0880k_adf ;

        } else if (sizes_within_1_percent(file_size, arch_1600_size)) { // within 1%  1600 arch_adfsf_350_dshd_1600k_adl
            // printf("This is an Acorn Archimedes HD 1600 KB Disk Image. This is not currently supported.\n");
            printf("Acorn Archimedes HD 1600 KB Disk Image detected! - arch_adfsf_350_dshd_1600k_adl\n");
            return arch_adfsf_350_dshd_1600k_adl ;

        } else if (sizes_within_1_percent(file_size, amiga_hd_size)) {
            printf("Amiga HD disk detected! - amiga_ados_350_dshd_1760k_adf\n");
            return amiga_ados_350_dshd_1760k_adf ;

        } else {
            printf("ADF file size doesn't match any known disk image sizes. The disk image file might be corrupt or unusual.\n");
            if (questions_are_enabled) MessageBox(NULL, "ADF file size doesn't match any known disk image sizes. The disk image file might be corrupt or unusual.", "Info", MB_OK | MB_ICONINFORMATION);
            return FORMAT_ID_COUNT ;

        };/*end-if*/

    } else if ( strcasecmp(nodot,"ADL") == 0 ) { 
        printf( "BBC Micro / Archimedes (.ADL) disk image detected!\n" ); // TODO: Actually support Archimedes.

        // BBC Micro File Sizes
        long bbc_160_size  = main_formats[ bbc_adfss_525_ssdd_0160k_adl ].size_kb * 1024; 
        long bbc_320_size  = main_formats[ bbc_adfsm_525_ssqd_0320k_adl ].size_kb * 1024; 
        long bbc_640_size  = main_formats[ bbc_adfsl_525_dsqd_0640k_adl ].size_kb * 1024; 

        // Archimedes File Sizes
        long arch_800_size  =  800 * 1024;
        long arch_1600_size = 1600 * 1024;

        if (file_size < 160 * 1024) {
            printf("ADL file size is too small to match any known disk image sizes. The disk image file might be corrupt or unusual.\n");
            if (questions_are_enabled) MessageBox(NULL, "ADL file size is too small to match any known disk image sizes. The disk image file might be corrupt or unusual.", "Info", MB_OK | MB_ICONINFORMATION);
            return FORMAT_ID_COUNT ;

        } else if (sizes_within_1_percent(file_size, bbc_160_size)) {
            printf("BBC Micro ADFS-S disk detected! - bbc_adfss_525_ssdd_0160k_adl\n");
            return bbc_adfss_525_ssdd_0160k_adl ;

        } else if (sizes_within_1_percent(file_size, bbc_320_size)) {
            printf("BBC Micro ADFS-M disk detected! - bbc_adfsm_525_ssqd_0320k_adl\n");
            return bbc_adfsm_525_ssqd_0320k_adl ;

        } else if (sizes_within_1_percent(file_size, bbc_640_size)) {
            printf("BBC Micro ADFS-L disk detected! - bbc_adfsl_525_dsqd_0640k_adl\n");
            return bbc_adfsl_525_dsqd_0640k_adl ;

        } else if (sizes_within_1_percent(file_size, arch_800_size)) { 
            printf("Acorn Archimedes HD 800 KB Disk Image detected! - arch_adfsd_350_dsdd_0800k_adl\n");
            return arch_adfsd_350_dsdd_0800k_adl ;

        } else if (sizes_within_1_percent(file_size, arch_1600_size)) { 
            printf("Acorn Archimedes HD 1600 KB Disk Image detected! - arch_adfsf_350_dshd_1600k_adl\n");
            return arch_adfsf_350_dshd_1600k_adl ;

        } else {
            printf("ADF file size doesn't match any known disk image sizes. The disk image file might be corrupt or unusual.\n");
            if (questions_are_enabled) MessageBox(NULL, "ADF file size doesn't match any known disk image sizes. The disk image file might be corrupt or unusual.", "Info", MB_OK | MB_ICONINFORMATION);
            return FORMAT_ID_COUNT ;

        };/*end-if*/

    } else if ( strcasecmp(nodot,"DSK") == 0 ) { 
        printf( "DSK format (Apple or Macintosh) disk image detected!\n" ); 

        // apple2_ados_525_ssdd_0114k_d13
        long appleii_d13_size = 113 * 1024; 

        // apple2_ados_525_ssdd_0140k_do & apple2_pdos_525_ssdd_0140k_po
        long appleii_do_size = main_formats[ apple2_ados_525_ssdd_0140k_dsk ].size_kb * 1024; 

        // 158 KB --> Tandy CoCo (157 KB (161,280 bytes)) --> Do within 1 percent.
        long tandycoco_158_size = main_formats[ coco_decb_525_ssdd_0158k_dsk ].size_kb * 1024; 

        // The below is not supported by Greaseweazle. Might be able to do a custom diskcfg or whatever in the future. 
        // apple2_ados_525_ssdd_0160k_do (160 KB (163,840 bytes)) --> Do within 1 percent.
        // long appleii_d0_40t_size = 160 * 1024;

        // 180 KB --> Tandy CoCo
        long tandycoco_180_size = main_formats[ coco_decb_525_ssdd_0180k_dsk ].size_kb * 1024;

        // 180 KB --> Dragon
        // long dragon_180_size = main_formats[ dragon_ddos_525_ssdd_0180k_dsk ].size_kb * 1024;

        // 360 KB --> Tandy CoCo
        long tandycoco_360_size = main_formats[ coco_os9_525_dsdd_0360k_dsk ].size_kb * 1024;

        // 360 KB --> Dragon dragon_ddos_525_dsdd_0360k_dsk  *OR*  dragon_ddos_525_ssqd_0360k_dsk
        // long dragon_360_size = main_formats[ dragon_ddos_525_dsdd_0360k_dsk ].size_kb * 1024;

        // mac_mfs_350_ssdd_0400k_raw
        long mac_400_size  = main_formats[ mac_mfs_350_ssdd_0400k_raw ].size_kb * 1024;

        // 720 KB --> Tandy CoCo
        long tandycoco_720_size = main_formats[ coco_os9_525_dsqd_0720k_dsk ].size_kb * 1024;

        // 720 KB --> Dragon
        // long dragon_720_size = main_formats[ dragon_ddos_525_dsqd_0720k_dsk ].size_kb * 1024;

        // mac_hfs_350_dsdd_0800k_raw
        long mac_800_size  = main_formats[ mac_hfs_350_dsdd_0800k_raw ].size_kb * 1024;

        // mac_hfs_350_ddhd_1440k_raw & mac_hfsp_350_ddhd_1440k_raw
        long mac_1440_size = main_formats[ mac_hfs_350_ddhd_1440k_raw ].size_kb * 1024;

        /******************************************************************/

        if ( within_range( file_size, appleii_d13_size, appleii_d13_size+(5*1024) ) ) { // TODO: Bro this isn't supported!
            printf("Apple II (.DSK) 114 KB disk detected! - apple2_ados_525_ssdd_0114k_d13\n");  
            // apple2_ados_525_ssdd_0114k_d13
            return FORMAT_ID_COUNT ;

        } else if ( within_range( file_size, appleii_do_size, appleii_do_size+(5*1024) ) ) { 
            printf("Apple II (.DSK) 140 KB disk detected! - apple2_ados_525_ssdd_0140k_do\n");  
            return apple2_ados_525_ssdd_0140k_do ;

        // 158 KB --> Tandy CoCo (157 KB (161,280 bytes)) --> Do within 1 percent.
        } else if ( sizes_within_1_percent( file_size, tandycoco_158_size ) ) { // } else if ( within_range( file_size, appleii_d0_40t_size, appleii_d0_40t_size+(5*1024) ) ) { 
            printf("Tandy CoCo (.DSK) 158 KB disk detected! - coco_decb_525_ssdd_0158k_dsk\n");  
            return coco_decb_525_ssdd_0158k_dsk ;

        // The below is not supported by Greaseweazle. Might be able to do a custom diskcfg or whatever in the future.
        // } else if ( sizes_within_1_percent( file_size, appleii_d0_40t_size ) ) { // } else if ( within_range( file_size, appleii_d0_40t_size, appleii_d0_40t_size+(5*1024) ) ) { 
        //     printf("Apple II (.DSK) 160 KB disk detected! - apple2_ados_525_ssdd_0160k_do.\n");  
        //     return apple2_ados_525_ssdd_0160k_do ;

        /******************************************************************/

        // 180 KB --> Tandy CoCo / Dragon
        } else if ( sizes_within_1_percent( file_size, tandycoco_180_size ) ) { // 180 KB (184,320 bytes)
             printf( "Tandy CoCo / Dragon (.DSK) disk image detected!\n" );

            if (questions_are_enabled) {

                int result = MessageBox(NULL,
                    "This 180 KB .DSK file could be Tandy CoCo or Dragon.\n\nIs this an Tandy CoCo disk image?\n\nPlease select one of the following:\n"
                    "Yes --> Tandy CoCo\n"
                    "No --> Dragon",
                    "Ambiguous Disk Image",
                    MB_YESNOCANCEL | MB_ICONINFORMATION 
                );/*end-MessageBox*/

                if (result == IDYES) {
                    printf("Tandy CoCo (.DSK) 180 KB disk chosen! - coco_decb_525_ssdd_0180k_dsk or coco_os9_525_ssdd_0180k_dsk.\n");  
                    return coco_decb_525_ssdd_0180k_dsk ;
                
                } else if (result == IDNO) {
                    printf("Dragon (.DSK) 180 KB disk chosen! - dragon_ddos_525_ssdd_0180k_dsk.\n");  
                    return dragon_ddos_525_ssdd_0180k_dsk ;
                
                } else if (result == IDCANCEL) {
                    return FORMAT_ID_COUNT ;
                
                };/*end-if*/

            } else {

                return coco_decb_525_ssdd_0180k_dsk ;

            };/*end-if*/

        /******************************************************************/

        // 360 KB --> Tandy CoCo
        // warn - Could be one of three different ones! coco_os9_525_dsdd_0360k_dsk  coco_os9_525_ssqd_0360k_dsk     3.5-Inch --> coco_os9_350_ssqd_0360k_dsk
        } else if ( sizes_within_1_percent( file_size, tandycoco_360_size ) ) { 
             printf( "Tandy CoCo / Dragon (.DSK) disk image detected!\n" );

            if (questions_are_enabled) {
                int result = MessageBox(NULL,
                    "This 360 KB .DSK file could be Tandy CoCo or Dragon.\n\nIs this an Tandy CoCo disk image?\n\nPlease select one of the following:\n"
                    "Yes --> Tandy CoCo\n"
                    "No --> Dragon",
                    "Ambiguous Disk Image",
                    MB_YESNOCANCEL | MB_ICONINFORMATION );

                if (result == IDYES) {
                    printf("Tandy CoCo (.DSK) 360 KB disk chosen! - coco_os9_525_dsdd_0360k_dsk or coco_os9_525_ssqd_0360k_dsk (or the 3.5-inch one).\n");  
                    return coco_os9_525_dsdd_0360k_dsk ;
                
                } else if (result == IDNO) {
                    printf( "Dragon (.DSK) 360 KB disk detected! - dragon_ddos_525_dsdd_0360k_dsk.\n" );
                    return coco_os9_525_ssqd_0360k_dsk ;
                
                } else if (result == IDCANCEL) {
                    return FORMAT_ID_COUNT ;
                
                };/*end-if*/

            } else {

                return coco_os9_525_dsdd_0360k_dsk ;

            };/*end-if*/

        /******************************************************************/

        } else if ( within_range( file_size, mac_400_size, mac_400_size+(15*1024) ) ) { 
            printf( "Macintosh (.DSK) 400 KB disk detected! - mac_mfs_350_ssdd_0400k_raw\n" );
            return mac_mfs_350_ssdd_0400k_raw ;

        /******************************************************************/

        // 720 KB --> Tandy CoCo
        // warn - Could be one of two different ones!  coco_os9_525_dsqd_0720k_dsk     3.5-Inch --> coco_os9_350_dsqd_0720k_dsk
        } else if ( sizes_within_1_percent( file_size, tandycoco_720_size ) ) { 
             printf( "Tandy CoCo / Dragon (.DSK) disk image detected!\n" );

            if (questions_are_enabled) {

                int result = MessageBox(NULL,
                    "This 360 KB .DSK file could be Tandy CoCo or Dragon.\n\nIs this an Tandy CoCo disk image?\n\nPlease select one of the following:\n"
                    "Yes --> Tandy CoCo\n"
                    "No --> Dragon",
                    "Ambiguous Disk Image",
                    MB_YESNOCANCEL | MB_ICONINFORMATION );

                if (result == IDYES) {
                    printf("Tandy CoCo (.DSK) 720 KB disk chosen! - coco_os9_525_dsqd_0720k_dsk (or the 3.5-inch one).\n");  
                    return coco_os9_525_dsqd_0720k_dsk ;
                
                } else if (result == IDNO) {
                    printf( "Dragon (.DSK) 360 KB disk detected! - dragon_ddos_525_dsdd_0360k_dsk.\n" );
                    return dragon_ddos_525_dsdd_0360k_dsk ;
                
                } else if (result == IDCANCEL) {
                    return FORMAT_ID_COUNT ;
                
                };/*end-if*/

            } else {

                return coco_os9_525_dsqd_0720k_dsk ;

            };/*end-if*/

        /******************************************************************/

        } else if ( within_range( file_size, mac_800_size, mac_800_size+(15*1024) ) ) { 
            printf( "Macintosh (.DSK) 800 KB disk detected! - mac_hfs_350_dsdd_0800k_raw\n" );
            return mac_hfs_350_dsdd_0800k_raw ;

        } else if ( within_range( file_size, mac_1440_size, mac_1440_size+(15*1024) ) ) { 
            printf( "Macintosh (.DSK) disk image detected! - msdos_fat12_350_dshd_1440k_ima\n" );
            if (questions_are_enabled) MessageBox(NULL, "The file extension .DSK is the worst thing ever.\nI'm sorry but it is.\nThis 1.44 MB image could be a Macintosh or MS-DOS image.\nPlease make sure you've set things correctly.\nGood luck Bud!", "Info", MB_OK | MB_ICONINFORMATION);
            return msdos_fat12_350_dshd_1440k_ima ;

        // TODO: This seems redudant and I can probably remove it. 
        // } else if ( within_range( file_size, 400*1024, 400+(10*1024) ) ) { // Apple II General Catch-All
        //     printf("DSK file size doesn't match any known disk image size. It might be corrupt or unusual or a totally different kind of file because DSK files could be almost anything.\n");
        //     MessageBox(NULL, "DSK file size doesn't match any known disk image size. It might be corrupt or unusual or a totally different kind of file because DSK files could be almost anything.", "Info", MB_OK | MB_ICONINFORMATION);
        //     return FORMAT_ID_COUNT ;

        } else { // Macintosh General Catch-All
            printf("DSK file size doesn't match any known disk image size. It might be corrupt or unusual or a totally different kind of file because DSK files could be almost anything.\n");
            if (questions_are_enabled) MessageBox(NULL, "DSK file size doesn't match any known disk image size. It might be corrupt or unusual or a totally different kind of file because DSK files could be almost anything.", "Info", MB_OK | MB_ICONINFORMATION);
            return FORMAT_ID_COUNT ;

        };/*end-if*/

    } else if ( strcasecmp(nodot,"DO" ) == 0 ) { 
        printf( "Apple II Apple DOS disk image detected! - apple2_ados_525_ssdd_0140k_do\n" ); 

        // TODO: Why is this like this? I think like only one of these is supported by Greaseweazle. 
        //       I need to check. I think the file format is supported but not all these weirdo sizes. 

        // apple2_ados_525_ssdd_0114k_d13
        long appleii_d13_size = 113 * 1024; 

        // apple2_ados_525_ssdd_0140k_do & apple2_pdos_525_ssdd_0140k_po
        long appleii_do_size = main_formats[ apple2_ados_525_ssdd_0140k_do ].size_kb * 1024; 

        // apple2_ados_525_ssdd_0160k_do
        long appleii_d0_40t_size = 160 * 1024;

        if ( within_range( file_size, appleii_d13_size, appleii_d13_size+(5*1024) ) ) { 
            printf("Apple II (.DO) 114 KB disk detected! - apple2_ados_525_ssdd_0114k_do\n");  
            // return apple2_ados_525_ssdd_0114k_do ;
            printf("Not supported. Returning FORMAT_ID_COUNT.\n");  
            return FORMAT_ID_COUNT ;

        } else if ( within_range( file_size, appleii_do_size, appleii_do_size+(5*1024) ) ) { 
            printf("Apple II (.DO) 140 KB disk detected! - apple2_ados_525_ssdd_0140k_do\n");  
            return apple2_ados_525_ssdd_0140k_do ;

        } else if ( within_range( file_size, appleii_d0_40t_size, appleii_d0_40t_size+(5*1024) ) ) { 
            printf("Apple II (.DO) 160 KB disk detected! - apple2_ados_525_ssdd_0160k_do\n");  
            // return apple2_ados_525_ssdd_0160k_do ;
            printf("Not supported. Returning FORMAT_ID_COUNT.\n");  
            return FORMAT_ID_COUNT ;

        } else {
            printf("DO file size doesn't match normal Apple II disk image sizes. The disk image file might be corrupt or unusual.\n");
            if (questions_are_enabled) MessageBox(NULL, "DO file size doesn't match normal Apple II disk image sizes. The disk image file might be corrupt or unusual.", "Info", MB_OK | MB_ICONINFORMATION);
            return FORMAT_ID_COUNT ;

        };/*end-if*/

    } else if ( strcasecmp(nodot,"PO" ) == 0 ) { 
        printf( "Apple II ProDOS disk image detected! - apple2_pdos_525_ssdd_0140k_po\n" ); 

        long appleii_size = main_formats[ apple2_pdos_525_ssdd_0140k_po ].size_kb * 1024; // main_formats[ amiga_ados_350_dsdd_0880k_adf ].size_kb --> This size is: 880 KB  --> AMIGA_DRIVE_1010

        if (sizes_within_1_percent(file_size, appleii_size)) {
            printf("Apple II disk detected!\n");
            return apple2_pdos_525_ssdd_0140k_po ;

        } else {
            printf("PO file size doesn't match normal Apple II disk image sizes. The disk image file might be corrupt or unusual.\n");
            if (questions_are_enabled) MessageBox(NULL, "PO file size doesn't match normal Apple II disk image sizes. The disk image file might be corrupt or unusual.", "Info", MB_OK | MB_ICONINFORMATION);
            return FORMAT_ID_COUNT ;

        };/*end-if*/

    } else if ( strcasecmp(nodot,"ATR") == 0 ) { 
        printf( "Atari 8-bit disk image detected!\n" ); 

        long atr_90_size  = main_formats[ atari_ados_525_sssd_0090k_atr ].size_kb * 1024; 
        long atr_130_size = main_formats[ atari_ados_525_ssdd_0130k_atr ].size_kb * 1024;
        long atr_180_size = main_formats[ atari_ados_525_ssdd_0180k_atr ].size_kb * 1024;
        long atr_360_size = main_formats[ atari_ados_525_dsdd_0360k_atr ].size_kb * 1024;

        if (sizes_within_1_percent(file_size, atr_90_size)) {
            printf("Atari (.ATR) 90 KB disk detected!\n");
            return atari_ados_525_sssd_0090k_atr ;

        } else if (sizes_within_1_percent(file_size, atr_130_size)) {
            printf("Atari (.ATR) 130 KB disk detected!\n");
            return atari_ados_525_ssdd_0130k_atr ;

        } else if (sizes_within_1_percent(file_size, atr_180_size)) {
            printf("Atari (.ATR) 180 KB disk detected!\n");
            return atari_ados_525_ssdd_0180k_atr ;

        } else if (sizes_within_1_percent(file_size, atr_360_size)) {
            printf("Atari (.ATR) 360 KB disk detected!\n");
            return atari_ados_525_dsdd_0360k_atr ;

        } else {
            printf("ATR file size doesn't match any known Atari 8-Bit disk image -- might be corrupt or unusual.\n");
            if (questions_are_enabled) MessageBox(NULL, "ATR file size doesn't match any known Atari 8-Bit disk image.\n It might be corrupt or unusual.", "Info", MB_OK | MB_ICONINFORMATION);
            return FORMAT_ID_COUNT ;

        };/*end-if*/

    } else if ( strcasecmp(nodot,"SSD") == 0 ) { 
        printf( "BBC Micro (.SSD) disk image detected!\n" ); 

        long bbcmicro_ssd_100kb_size = main_formats[ bbc_dfs_525_sssd_0100k_ssd ].size_kb * 1024;
        long bbcmicro_ssd_200kb_size = main_formats[ bbc_dfs_525_ssqd_0200k_ssd ].size_kb * 1024;

        if (within_range(file_size, 0, bbcmicro_ssd_100kb_size)) {
            printf("BBC Micro (.SSD) DFS disk of size 100KB detected!\n");
            return bbc_dfs_525_sssd_0100k_ssd ;

        } else if (within_range(file_size, bbcmicro_ssd_100kb_size+1, bbcmicro_ssd_200kb_size)) {
            printf("BBC Micro (.SSD) DFS disk of size 200KB detected!\n");
            return bbc_dfs_525_ssqd_0200k_ssd ;

        } else {
            printf("SSD file size doesn't match normal BBC Micro disk image sizes. The disk image file might be corrupt or unusual.\n");
            if (questions_are_enabled) MessageBox(NULL, "SSD file size doesn't match normal BBC Micro disk image sizes.\nHowever, this disk image format may only store files only and not all blocks, and I'll have to do some homework and update this program when I figure what to do.", "Info", MB_OK | MB_ICONINFORMATION);
            return FORMAT_ID_COUNT ;

        };/*end-if*/

    } else if ( strcasecmp(nodot,"DSD") == 0 ) { 
        printf( "BBC Micro (.DSD) disk image detected!\n" ); 

        long bbcmicro_dsd_200kb_size = main_formats[ bbc_dfs_525_dssd_0200k_dsd ].size_kb * 1024;
        long bbcmicro_dsd_400kb_size = main_formats[ bbc_dfs_525_dsqd_0400k_dsd ].size_kb * 1024;

        if (within_range(file_size, 0, bbcmicro_dsd_200kb_size)) { // if (sizes_within_1_percent(file_size, bbcmicro_dsd_200kb_size)) {
            printf("BBC Micro (.DSD) DFS disk of size 200KB detected!\n");
            return bbc_dfs_525_dssd_0200k_dsd ;

        } else if (within_range(file_size, bbcmicro_dsd_200kb_size+1, bbcmicro_dsd_400kb_size)) { // } else if (sizes_within_1_percent(file_size, bbcmicro_dsd_400kb_size)) {
            printf("BBC Micro (.DSD) DFS disk of size 400KB detected!\n");
            return bbc_dfs_525_dsqd_0400k_dsd ;

        } else {
            printf("DSD file size doesn't match normal BBC Micro disk image sizes.\n");
            if (questions_are_enabled) MessageBox(NULL, "DSD file size doesn't match normal BBC Micro disk image sizes.\nHowever, this disk image format may only store files only and not all blocks, and I'll have to do some homework and update this program when I figure what to do.", "Info", MB_OK | MB_ICONINFORMATION);
            // I decided to set things to the default drive based on the selected computer. I dunno seems like the thing to do. The user will have to figure out what the right selection is. 
            return FORMAT_ID_COUNT ;

        };/*end-if*/

    } else if ( strcasecmp(nodot,"D64") == 0 ) { 
        printf( "C64/128 / CBM-1541 disk image detected!\n" ); 
        return cbm1541_cdos_525_ssdd_0171k_d64 ;

    } else if ( strcasecmp(nodot,"D71") == 0 ) { 
        printf( "C64/128 / CBM-1571 disk image detected!\n" ); 
        return cbm1571_cdos_525_dsdd_0342k_d71 ;

    } else if ( strcasecmp(nodot,"D81") == 0 ) { 
        printf( "C64/128 / CBM-1581 disk image detected!\n" ); 
        return cbm1581_cdos_350_dsdd_0800k_d81 ;

    } else if ( strcasecmp(nodot,"D80") == 0 ) { 
        printf( "PET/C64/128 / CBM-8050 disk image detected!\n" ); 
        return cbm8050_cdos_525_dsqd_0521k_d80 ;
        if (questions_are_enabled) MessageBox(NULL, "FYI: You need a ZoomFloppy and a CBM 8050, CBM 8250, or CBM SFD-1001 floppy drive to make this disk.\n\nThe only other way to make one of these disks is to get a 5.25-inch 100TPI floppy drive. They are rare. You can't just use any 80 track drive. It's an alignment thing. This format is a 77 track format where the actual placement of the tracks is different enough from a typical 96TPI drive that it's just not going to work.", "Info", MB_OK | MB_ICONINFORMATION);

    } else if ( strcasecmp(nodot,"D82") == 0 ) { 
        printf( "PET/C64/128 / CBM-8250/SFD-1001 disk image detected!\n" ); 
        return cbmsfd1_cdos_525_dsqd_1042k_d82 ;
        if (questions_are_enabled) MessageBox(NULL, "FYI: You need a ZoomFloppy and a CBM 8050, CBM 8250, or CBM SFD-1001 floppy drive to make this disk.\n\nThe only other way to make one of these disks is to get a 5.25-inch 100TPI floppy drive. They are rare. You can't just use any 80 track drive. It's an alignment thing. This format is a 77 track format where the actual placement of the tracks is different enough from a typical 96TPI drive that it's just not going to work.", "Info", MB_OK | MB_ICONINFORMATION);

    } else if ( strcasecmp(nodot,"ST" ) == 0 ) { 
        printf( "Atari ST (.ST) disk image detected!\n" ); 

        long st_0360_size = main_formats[ atarist_fat12_350_ssdd_0360k_st ].size_kb * 1024; 
        long st_0720_size = main_formats[ atarist_fat12_350_dsdd_0720k_st ].size_kb * 1024;
        long st_1440_size = main_formats[ atarist_fat12_350_dshd_1440k_st ].size_kb * 1024;
        long st_0400_size = main_formats[ atarist_fat12_350_ssdd_0400k_st ].size_kb * 1024;
        long st_0440_size = main_formats[ atarist_fat12_350_ssdd_0440k_st ].size_kb * 1024;
        long st_0800_size = main_formats[ atarist_fat12_350_dsdd_0800k_st ].size_kb * 1024;
        long st_0880_size = main_formats[ atarist_fat12_350_dsdd_0880k_st ].size_kb * 1024;

        if        (sizes_within_1_percent(file_size, st_0360_size)) { printf("Atari ST (.ST) 360 KB disk detected!\n");  return atarist_fat12_350_ssdd_0360k_st ;
        } else if (sizes_within_1_percent(file_size, st_0720_size)) { printf("Atari ST (.ST) 720 KB disk detected!\n");  return atarist_fat12_350_dsdd_0720k_st ;
        } else if (sizes_within_1_percent(file_size, st_1440_size)) { printf("Atari ST (.ST) 1440 KB disk detected!\n"); return atarist_fat12_350_dshd_1440k_st ;
        } else if (sizes_within_1_percent(file_size, st_0400_size)) { printf("Atari ST (.ST) 0400 KB disk detected!\n"); return atarist_fat12_350_ssdd_0400k_st ;
        } else if (sizes_within_1_percent(file_size, st_0440_size)) { printf("Atari ST (.ST) 0440 KB disk detected!\n"); return atarist_fat12_350_ssdd_0440k_st ;
        } else if (sizes_within_1_percent(file_size, st_0800_size)) { printf("Atari ST (.ST) 0800 KB disk detected!\n"); return atarist_fat12_350_dsdd_0800k_st ;
        } else if (sizes_within_1_percent(file_size, st_0880_size)) { printf("Atari ST (.ST) 0880 KB disk detected!\n"); return atarist_fat12_350_dsdd_0880k_st ;               

        } else {
            printf("ST file size doesn't match any known Atari ST disk image -- might be corrupt or unusual.\n");
            if (questions_are_enabled) MessageBox(NULL, "ST file size doesn't match any known Atari ST disk image.\nIt might be corrupt or unusual.", "Info", MB_OK | MB_ICONINFORMATION);
            return FORMAT_ID_COUNT ;

        };/*end-if*/

    } else if ( strcasecmp(nodot,"MSA") == 0 ) { 
        printf( "Atari ST (.MSA) disk image detected!\n" ); 

        long st_0360_size = main_formats[ atarist_fat12_350_ssdd_0360k_msa ].size_kb * 1024; 
        long st_0720_size = main_formats[ atarist_fat12_350_dsdd_0720k_msa ].size_kb * 1024;
        long st_1440_size = main_formats[ atarist_fat12_350_dshd_1440k_msa ].size_kb * 1024;
        long st_0400_size = main_formats[ atarist_fat12_350_ssdd_0400k_msa ].size_kb * 1024;
        long st_0440_size = main_formats[ atarist_fat12_350_ssdd_0440k_msa ].size_kb * 1024;
        long st_0800_size = main_formats[ atarist_fat12_350_dsdd_0800k_msa ].size_kb * 1024;
        long st_0880_size = main_formats[ atarist_fat12_350_dsdd_0880k_msa ].size_kb * 1024;

        if        (sizes_within_1_percent(file_size, st_0360_size)) { printf("Atari ST (.MSA) 360 KB disk detected!\n");  return atarist_fat12_350_ssdd_0360k_msa ;
        } else if (sizes_within_1_percent(file_size, st_0720_size)) { printf("Atari ST (.MSA) 720 KB disk detected!\n");  return atarist_fat12_350_dsdd_0720k_msa ;
        } else if (sizes_within_1_percent(file_size, st_1440_size)) { printf("Atari ST (.MSA) 1440 KB disk detected!\n"); return atarist_fat12_350_dshd_1440k_msa ;
        } else if (sizes_within_1_percent(file_size, st_0400_size)) { printf("Atari ST (.MSA) 0400 KB disk detected!\n"); return atarist_fat12_350_ssdd_0400k_msa ;
        } else if (sizes_within_1_percent(file_size, st_0440_size)) { printf("Atari ST (.MSA) 0440 KB disk detected!\n"); return atarist_fat12_350_ssdd_0440k_msa ;
        } else if (sizes_within_1_percent(file_size, st_0800_size)) { printf("Atari ST (.MSA) 0800 KB disk detected!\n"); return atarist_fat12_350_dsdd_0800k_msa ;
        } else if (sizes_within_1_percent(file_size, st_0880_size)) { printf("Atari ST (.MSA) 0880 KB disk detected!\n"); return atarist_fat12_350_dsdd_0880k_msa ;               

        } else {
            printf("MSA file size doesn't match any known Atari MSA disk image -- might be corrupt or unusual.\n");
            if (questions_are_enabled) MessageBox(NULL, "MSA file size doesn't match any known Atari MSA disk image. It might be corrupt or unusual.", "Info", MB_OK | MB_ICONINFORMATION);
            return FORMAT_ID_COUNT ;

        };/*end-if*/

    } else if ( strcasecmp(nodot,"STX") == 0 ) { 
        printf( "Atari ST (.STX) disk image detected!\n" ); 

        long st_0360_size = main_formats[ atarist_fat12_350_ssdd_0360k_stx ].size_kb * 1024 ; 
        long st_0720_size = main_formats[ atarist_fat12_350_dsdd_0720k_stx ].size_kb * 1024 ;
        long st_1440_size = main_formats[ atarist_fat12_350_dshd_1440k_stx ].size_kb * 1024 ;
        long st_0400_size = main_formats[ atarist_fat12_350_ssdd_0400k_stx ].size_kb * 1024 ;
        long st_0440_size = main_formats[ atarist_fat12_350_ssdd_0440k_stx ].size_kb * 1024 ;
        long st_0800_size = main_formats[ atarist_fat12_350_dsdd_0800k_stx ].size_kb * 1024 ;
        long st_0880_size = main_formats[ atarist_fat12_350_dsdd_0880k_stx ].size_kb * 1024 ;

        if        (sizes_within_1_percent(file_size, st_0360_size)) { printf("Atari ST (.MSA) 360 KB disk detected!\n");  return atarist_fat12_350_ssdd_0360k_stx ;
        } else if (sizes_within_1_percent(file_size, st_0720_size)) { printf("Atari ST (.MSA) 720 KB disk detected!\n");  return atarist_fat12_350_dsdd_0720k_stx ;
        } else if (sizes_within_1_percent(file_size, st_1440_size)) { printf("Atari ST (.MSA) 1440 KB disk detected!\n"); return atarist_fat12_350_dshd_1440k_stx ;
        } else if (sizes_within_1_percent(file_size, st_0400_size)) { printf("Atari ST (.MSA) 0400 KB disk detected!\n"); return atarist_fat12_350_ssdd_0400k_stx ;
        } else if (sizes_within_1_percent(file_size, st_0440_size)) { printf("Atari ST (.MSA) 0440 KB disk detected!\n"); return atarist_fat12_350_ssdd_0440k_stx ;
        } else if (sizes_within_1_percent(file_size, st_0800_size)) { printf("Atari ST (.MSA) 0800 KB disk detected!\n"); return atarist_fat12_350_dsdd_0800k_stx ;
        } else if (sizes_within_1_percent(file_size, st_0880_size)) { printf("Atari ST (.MSA) 0880 KB disk detected!\n"); return atarist_fat12_350_dsdd_0880k_stx ;               

        } else {
            printf("ST file size doesn't match any known Atari ST disk image -- might be corrupt or unusual.\n");
            if (questions_are_enabled) MessageBox(NULL, "ST file size doesn't match any known Atari ST disk image. It might be corrupt or unusual.", "Info", MB_OK | MB_ICONINFORMATION);
            return FORMAT_ID_COUNT ;

        };/*end-if*/

    } else if ( strcasecmp(nodot,"IMG") == 0 ) { 

        // atari_ados_525_sssd_0090k_atr
        long atr_90_size = main_formats[ atari_ados_525_sssd_0090k_atr ].size_kb * 1024;

        // apple2_ados_525_ssdd_0114k_d13
        long appleii_d13_size = 113 * 1024; 

        // atari_ados_525_ssdd_0130k_atr
        long atr_130_size = main_formats[ atari_ados_525_ssdd_0130k_atr ].size_kb * 1024;

        // apple2_ados_525_ssdd_0140k_do & apple2_pdos_525_ssdd_0140k_po
        long appleii_do_size = main_formats[ apple2_ados_525_ssdd_0140k_dsk ].size_kb * 1024; 

        // Note: There is also a weirdo format here: ibm.160 and I might not bother supporting with the autodetection either.
        // apple2_ados_525_ssdd_0160k_do
        long appleii_d0_40t_size = 160 * 1024;

        // Note: There is also a weirdo format here: ibm.180 and I might not bother supporting with the autodetection either. 
        //  atari_ados_525_ssdd_0180k_atr 
        long atr_180_size = main_formats[ atari_ados_525_ssdd_0180k_atr ].size_kb * 1024; 

        // olivettim20_pcos_525_dsdd_0280_img
        long olivettim20_280_size  = main_formats[ olivettim20_pcos_525_dsdd_0280_img ].size_kb * 1024 ;

        // NOTE: This is a weirdo format ms-dos PC 5.25" double-sided, 8-sector, 40-track (320KB).**

        // msdos_fat12_525_dsdd_0360k_ima
        long msdos_360_size  = main_formats[ msdos_fat12_525_dsdd_0360k_ima ].size_kb * 1024 ;

        // mac_mfs_350_ssdd_0400k_raw
        long mac_400_size  = main_formats[ mac_mfs_350_ssdd_0400k_raw ].size_kb * 1024;

        // msdos_fat12_350_dsdd_0720k_ima
        long msdos_720_size  = main_formats[ msdos_fat12_350_dsdd_0720k_ima ].size_kb * 1024 ;

        // mac_hfs_350_dsdd_0800k_raw // NOTE: Weirdo formats --> There is a rare Japanese 800KB MS-DOS variant that's non-standard. Maybe I'll just ignore it? Or prompt? I'm not sure. 
        long mac_800_size  = main_formats[ mac_hfs_350_dsdd_0800k_raw ].size_kb * 1024;

        // msdos_fat12_525_dshd_1200k_ima
        long msdos_1200_size = main_formats[ msdos_fat12_525_dshd_1200k_ima ].size_kb * 1024 ;

        // mac_hfs_350_ddhd_1440k_raw & mac_hfsp_350_ddhd_1440k_raw
        // long mac_1440_size = main_formats[ mac_hfs_350_ddhd_1440k_raw ].size_kb * 1024;

        // msdos_fat12_350_dshd_1440k_ima
        long msdos_1440_size = main_formats[ msdos_fat12_350_dshd_1440k_ima ].size_kb * 1024 ;

        // NOTE: The weirdo formats are 1680/DMF(?), abd ED 2880. We can ignore 2880 for now. At some point I'll add 3-inch, 8-inch and Extra High Density formats. 

        if ( within_range( file_size, atr_90_size, atr_90_size+(5*1024) ) ) { 
            printf("Atari (.IMG) 90 KB disk detected!\n");
            return atari_ados_525_sssd_0090k_atr ;

        } else if ( within_range( file_size, appleii_d13_size, appleii_d13_size+(5*1024) ) ) { 
            printf("Apple II (.IMG) 114 KB disk detected! - apple2_ados_525_ssdd_0114k_d0\n");  
            // return apple2_ados_525_ssdd_0114k_d0 ; // BRO???
            return FORMAT_ID_COUNT ;

        } else if ( within_range( file_size, atr_130_size, atr_130_size+(5*1024) ) ) { 
            printf("Atari (.IMG) 130 KB disk detected! - atari_ados_525_ssdd_0130k_atr\n");  
            return atari_ados_525_ssdd_0130k_atr ;

        } else if ( within_range( file_size, appleii_do_size, appleii_do_size+(5*1024) ) ) { 
            printf("Apple II (.IMG) 140 KB disk detected! - apple2_ados_525_ssdd_0140k_do\n");  
            return apple2_ados_525_ssdd_0140k_do ;

        } else if ( within_range( file_size, appleii_d0_40t_size, appleii_d0_40t_size+(5*1024) ) ) { 
            printf("Apple II (.IMG) 160 KB disk detected! - apple2_ados_525_ssdd_0160k_do\n");  
            // return apple2_ados_525_ssdd_0160k_do ; // BRO???
            return FORMAT_ID_COUNT ;

        } else if ( within_range( file_size, atr_180_size, atr_180_size+(5*1024) ) ) { 
            printf("Atari (.IMG) 180 KB disk detected! - atari_ados_525_ssdd_0180k_atr\n");  
            return atari_ados_525_ssdd_0180k_atr ;

        // olivettim20_pcos_525_dsdd_0280_img
        } else if ( within_range( file_size, olivettim20_280_size-(5*1024), olivettim20_280_size+(5*1024) ) ) { 
            printf("Olivetti M20 (.IMG) 280 KB disk detected! - olivettim20_pcos_525_dsdd_0280_img\n");  
            return olivettim20_pcos_525_dsdd_0280_img ;

        } else if ( within_range( file_size, msdos_360_size, msdos_360_size+(10*1024) ) ) { // TODO: ATR 360K (Atari XF551) Disks!!!
            printf( "PC Compatible / MS-DOS (.IMG) disk image detected! - msdos_fat12_525_dsdd_0360k_ima\n" );
            return msdos_fat12_525_dsdd_0360k_ima ;

        } else if ( within_range( file_size, mac_400_size, mac_400_size+(15*1024) ) ) { 
            printf( "Macintosh (.IMG) 800 KB disk detected! - mac_mfs_350_ssdd_0400k_raw\n" );
            return mac_mfs_350_ssdd_0400k_raw ;

        } else if ( within_range( file_size, msdos_720_size, msdos_720_size+(15*1024) ) ) { 
            printf( "PC Compatible / MS-DOS (.IMG) disk image detected! - msdos_fat12_350_dsdd_0720k_ima\n" );
            return msdos_fat12_350_dsdd_0720k_ima ; // IMA is just a better ext than IMG but they are the same and in the main_formats table as an alternative extension. 

        } else if ( within_range( file_size, mac_800_size, mac_800_size+(15*1024) ) ) { 
            printf( "Macintosh (.IMG) 800 KB disk detected! - mac_hfs_350_dsdd_0800k_raw\n" );
            return mac_hfs_350_dsdd_0800k_raw ; // RAW is just a better ext than IMG but they are the same and in the main_formats table as an alternative extension. 

        } else if ( within_range( file_size, msdos_1200_size, msdos_1200_size+(15*1024) ) ) { 
            printf( "PC Compatible / MS-DOS (.IMG) disk image detected! - msdos_fat12_525_dshd_1200k_ima\n" );
            return msdos_fat12_525_dshd_1200k_ima ; // IMA is just a better ext than IMG but they are the same and in the main_formats table as an alternative extension. 

        } else if ( within_range( file_size, msdos_1440_size, msdos_1440_size+(15*1024) ) ) { 
            printf( "PC Compatible / MS-DOS (.IMG) disk image detected!\n" );

            if (questions_are_enabled) {

                int result = MessageBox(NULL,
                    "This 1.44 MB .IMG file could be MS-DOS or Macintosh.\n\nIs this an MS-DOS disk image?\n\nPlease select one of the following:\n"
                    "Yes --> MS-DOS / PC Compatible\n"
                    "No --> Macintosh",
                    "Ambiguous Disk Image",
                    MB_YESNOCANCEL | MB_ICONINFORMATION 
                );/*end-MessageBox*/

                if (result == IDYES) {
                    return msdos_fat12_350_dshd_1440k_ima ;

                } else if (result == IDNO) {
                    return mac_hfs_350_ddhd_1440k_raw ;

                } else if (result == IDCANCEL) {
                    return FORMAT_ID_COUNT ;

                };/*end-if*/

            } else {

                return msdos_fat12_350_dshd_1440k_ima ;

            };/*end-if*/

        } else {
            printf("IMG file size doesn't match any known disk image size. It might be corrupt or unusual or a totally different kind of file because IMG files could be almost anything.\n");
            if (questions_are_enabled) MessageBox(NULL, "IMG file size doesn't match any known disk image size. It might be corrupt or unusual or a totally different kind of file because IMG files could be almost anything.", "Info", MB_OK | MB_ICONINFORMATION);
            return FORMAT_ID_COUNT ;

        };/*end-if*/

    } else if ( strcasecmp(nodot,"IMA") == 0 ) { 
        printf( "PC Compatible / MS-DOS (.IMA) disk image detected!\n" ); 

        long msdos_360_size  = main_formats[ msdos_fat12_525_dsdd_0360k_ima ].size_kb * 1024 ;
        long msdos_1200_size = main_formats[ msdos_fat12_525_dshd_1200k_ima ].size_kb * 1024 ;
        long msdos_720_size  = main_formats[ msdos_fat12_350_dsdd_0720k_ima ].size_kb * 1024 ; // USB Floppy Device support this format. 
        long msdos_1440_size = main_formats[ msdos_fat12_350_dshd_1440k_ima ].size_kb * 1024 ; // USB Floppy Device support this format. 

        if        (sizes_within_1_percent(file_size, msdos_360_size )) { printf("MS-DOS (.IMA) 360 KB disk detected!\n");  return msdos_fat12_525_dsdd_0360k_ima ;
        } else if (sizes_within_1_percent(file_size, msdos_1200_size)) { printf("MS-DOS (.IMA) 1200 KB disk detected!\n"); return msdos_fat12_525_dshd_1200k_ima ;
        } else if (sizes_within_1_percent(file_size, msdos_720_size )) { printf("MS-DOS (.IMA) 720 KB disk detected!\n");  return msdos_fat12_350_dsdd_0720k_ima ;
        } else if (sizes_within_1_percent(file_size, msdos_1440_size)) { printf("MS-DOS (.IMA) 1440 KB disk detected!\n"); return msdos_fat12_350_dshd_1440k_ima ;
        } else {
            printf("IMA file size doesn't match any known MS-DOS disk image size. It might be corrupt or unusual.\n");
            if (questions_are_enabled) MessageBox(NULL, "IMA file size doesn't match any known MS-DOS disk image size. It might be corrupt or unusual.", "Info", MB_OK | MB_ICONINFORMATION);
            return FORMAT_ID_COUNT ;

        };/*end-if*/

        set_current_options( BRAND_PCCOMPAT );

    } else if ( strcasecmp(nodot,"IMD") == 0 ) { 
        printf( "PC Compatible / MS-DOS (.IMD) disk image detected!\n" ); 

        // MS-DOS formats
        long msdos_360_size  = main_formats[ msdos_fat12_525_dsdd_0360k_imd ].size_kb * 1024 ;
        long msdos_1200_size = main_formats[ msdos_fat12_525_dshd_1200k_imd ].size_kb * 1024 ;
        long msdos_720_size  = main_formats[ msdos_fat12_350_dsdd_0720k_imd ].size_kb * 1024 ;
        long msdos_1440_size = main_formats[ msdos_fat12_350_dshd_1440k_imd ].size_kb * 1024 ;
        
        long olivettim20_280_size = main_formats[ olivettim20_pcos_525_dsdd_0280_img ].size_kb * 1024 ;

        if        (sizes_within_1_percent(file_size, msdos_360_size )) { printf("MS-DOS (.IMD) 360 KB disk detected!\n");  return msdos_fat12_525_dsdd_0360k_imd ;
        } else if (sizes_within_1_percent(file_size, msdos_1200_size)) { printf("MS-DOS (.IMD) 1200 KB disk detected!\n"); return msdos_fat12_525_dshd_1200k_imd ;
        } else if (sizes_within_1_percent(file_size, msdos_720_size )) { printf("MS-DOS (.IMD) 720 KB disk detected!\n");  return msdos_fat12_350_dsdd_0720k_imd ;
        } else if (sizes_within_1_percent(file_size, msdos_1440_size)) { printf("MS-DOS (.IMD) 1440 KB disk detected!\n"); return msdos_fat12_350_dshd_1440k_imd ;
        
        } else if ( within_range( file_size, 0, olivettim20_280_size+(5*1024) ) ) { 
            printf("Olivetti M20 (.IMD) 280 KB disk detected! - olivettim20_pcos_525_dsdd_0280_imd\n");  
            return olivettim20_pcos_525_dsdd_0280_imd ;

        } else {
            printf("IMD file size doesn't match any known MS-DOS or Olivetti disk image size. It might be corrupt or unusual.\n");
            if (questions_are_enabled) MessageBox(NULL, "IMD file size doesn't match any known MS-DOS or Olivetti disk image size. It might be corrupt or unusual.", "Info", MB_OK | MB_ICONINFORMATION);
            return FORMAT_ID_COUNT ;

        };/*end-if*/

    } else if ( strcasecmp(nodot,"2MG") == 0 ) { 
        printf( "Apple IIGS (.2MG) disk image detected!\n" );

        // apple2_ados_525_ssdd_0140k_do & apple2_pdos_525_ssdd_0140k_po
        long appleiigs_size = main_formats[ apple2gs_pd16_350_dsdd_0800k_2mg ].size_kb * 1024; 

        // // apple2_ados_525_ssdd_0114k_d13
        // long appleii_d13_size = 113 * 1024; 

        // // apple2_ados_525_ssdd_0140k_do & apple2_pdos_525_ssdd_0140k_po
        // long appleii_do_size = main_formats[ apple2_ados_525_ssdd_0140k_do ].size_kb * 1024; 

        // // apple2_ados_525_ssdd_0160k_do
        // long appleii_d0_40t_size = 160 * 1024;

        if ( within_range( file_size, appleiigs_size, appleiigs_size+(5*1024) ) ) { 
            printf("Apple IIGS (.2MG) 800 KB disk detected! - apple2gs_pd16_350_dsdd_0800k_2mg\n"); 
            return apple2gs_pd16_350_dsdd_0800k_2mg ;

            // } else if ( within_range( file_size, appleii_d13_size, appleii_d13_size+(5*1024) ) ) { 
            //     printf("Apple II (.2MG) 114 KB disk detected! - apple2_ados_525_ssdd_0114k_do\n");  
            //     return apple2_ados_525_ssdd_0114k_do ;

            // } else if ( within_range( file_size, appleii_do_size, appleii_do_size+(5*1024) ) ) { 
            //     printf("Apple II (.2MG) 140 KB disk detected! - apple2_ados_525_ssdd_0140k_do\n");  
            //     return apple2_ados_525_ssdd_0140k_do ;

            // } else if ( within_range( file_size, appleii_d0_40t_size, appleii_d0_40t_size+(5*1024) ) ) { 
            //     printf("Apple II (.2MG) 160 KB disk detected! - apple2_ados_525_ssdd_0160k_do\n");  
            //     return apple2_ados_525_ssdd_0160k_do ;

        } else {
            printf("The 2MG file size doesn't match a 3.5-inch DD 800KB Apple IIGS file. I still need to add supports to ALL the different .2MG that are out there!\n");
            if (questions_are_enabled) MessageBox(NULL, "The 2MG file size doesn't match a 3.5-inch DD 800KB Apple IIGS file. I still need to add supports to ALL the different .2MG that are out there!", "Info", MB_OK | MB_ICONINFORMATION);
            return FORMAT_ID_COUNT ;

        };/*end-if*/

    } else {

        if (questions_are_enabled) MessageBox(NULL, "This file does not appear to be any known disk image file that this program supports.\nPlease check the file.\n\nSorry Bud. Good luck, eh!", "Info", MB_OK | MB_ICONINFORMATION);

    };/*end-if*/

    return FORMAT_ID_COUNT ;

}/*end-func*/

/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/

static inline bool setup_format_from_formatcode(const FORMAT_ID_TYPE given_main_format_code ) { // The nodot is like the extension with the `.` in it. 

    int current_hardware_selected = hardware_selected;
    
    // The default is to switch to Greaseweazle since I figure that's the most common things to do. 
    // Below for 100TPI based formats like the SFD-1001, that section's code switches the below to ZoomFloppy. 
    hardware_selected       = GREASEWEAZLE_CHOSEN ; // Set to Greaseweazle.
    greaseweazle_tree_state = NK_MAXIMIZED        ; // *** Greasweazle ***
    zoomfloppy_tree_state   = NK_MINIMIZED        ; // ZoomFloppy
    usbfloppy_tree_state    = NK_MINIMIZED        ; // USBFloppy

    /******************************************************************/  

    if ( given_main_format_code == bbc_adfss_525_ssdd_0160k_adl ) {

            printf("BBC Micro ADFS-S disk detected! - bbc_adfss_525_ssdd_0160k_adl\n");
            drive_selected    = BBC_DRIVE_525_SSDD_160_ADL;
            computer_selected = BBC_MICRO_B;
            set_current_options(main_formats[ bbc_adfss_525_ssdd_0160k_adl ].format_brand);

    } else if ( given_main_format_code == bbc_adfsm_525_ssqd_0320k_adl ) {

            printf("BBC Micro ADFS-M disk detected! - bbc_adfsm_525_ssqd_0320k_adl\n");
            drive_selected    = BBC_DRIVE_525_SSQD_320_ADL;
            computer_selected = BBC_MICRO_B;
            set_current_options(main_formats[ bbc_adfsm_525_ssqd_0320k_adl ].format_brand);

    } else if ( given_main_format_code == bbc_adfsl_525_dsqd_0640k_adl ) {
            printf("BBC Micro ADFS-L disk detected! - bbc_adfsl_525_dsqd_0640k_adl\n");
            drive_selected    = BBC_DRIVE_525_DSQD_640_ADL;
            computer_selected = BBC_MICRO_B;
            set_current_options(main_formats[ bbc_adfsl_525_dsqd_0640k_adl ].format_brand);

    } else if ( given_main_format_code == arch_adfsd_350_dsdd_0800k_adl ) {
            printf("Acorn Archimedes HD 800 KB Disk Image detected! - arch_adfsd_350_dsdd_0800k_adl\n");
            drive_selected    = ARCHIMEDES_DRIVE_DD_D;
            computer_selected = ARCHIMEDES_A310;
            set_current_options(main_formats[ arch_adfsd_350_dsdd_0800k_adl ].format_brand);

    } else if ( given_main_format_code == amiga_ados_350_dsdd_0880k_adf ) {
            printf("Amiga DD disk detected! - amiga_ados_350_dsdd_0880k_adf \n");
            drive_selected    = AMIGA_DRIVE_1010;
            computer_selected = AMIGA_500;
            set_current_options(main_formats[ amiga_ados_350_dsdd_0880k_adf ].format_brand);

    } else if ( given_main_format_code == arch_adfsf_350_dshd_1600k_adl ) {
            printf("Acorn Archimedes HD 1600 KB Disk Image detected! - arch_adfsf_350_dshd_1600k_adl\n");
            drive_selected    = ARCHIMEDES_DRIVE_HD;
            computer_selected = ARCHIMEDES_A5000;
            set_current_options(main_formats[ arch_adfsf_350_dshd_1600k_adl ].format_brand);

    } else if ( given_main_format_code == amiga_ados_350_dshd_1760k_adf ) {
            printf("Amiga HD disk detected! - amiga_ados_350_dshd_1760k_adf\n");
            drive_selected    = AMIGA_DRIVE_A4K;
            computer_selected = AMIGA_4000;
            set_current_options(main_formats[ amiga_ados_350_dshd_1760k_adf ].format_brand);

    /******************************************************************/

    } else if ( given_main_format_code == bbc_adfss_525_ssdd_0160k_adl ) {
            printf("BBC Micro ADFS-S disk detected! - bbc_adfss_525_ssdd_0160k_adl\n");
            drive_selected    = BBC_DRIVE_525_SSDD_160_ADL;
            computer_selected = BBC_MICRO_B;
            set_current_options(main_formats[ bbc_adfss_525_ssdd_0160k_adl ].format_brand);

    } else if ( given_main_format_code == bbc_adfsm_525_ssqd_0320k_adl ) {
            printf("BBC Micro ADFS-M disk detected! - bbc_adfsm_525_ssqd_0320k_adl\n");
            drive_selected    = BBC_DRIVE_525_SSQD_320_ADL;
            computer_selected = BBC_MICRO_B;
            set_current_options(main_formats[ bbc_adfsm_525_ssqd_0320k_adl ].format_brand);

    } else if ( given_main_format_code == bbc_adfsl_525_dsqd_0640k_adl ) {
            printf("BBC Micro ADFS-L disk detected! - bbc_adfsl_525_dsqd_0640k_adl\n");
            drive_selected    = BBC_DRIVE_525_DSQD_640_ADL;
            computer_selected = BBC_MICRO_B;
            set_current_options(main_formats[ bbc_adfsl_525_dsqd_0640k_adl ].format_brand);

    } else if ( given_main_format_code == arch_adfsd_350_dsdd_0800k_adl ) {
            printf("Acorn Archimedes HD 800 KB Disk Image detected! - arch_adfsd_350_dsdd_0800k_adl\n");
            drive_selected    = ARCHIMEDES_DRIVE_DD_D;
            computer_selected = ARCHIMEDES_A310;
            set_current_options(main_formats[ arch_adfsd_350_dsdd_0800k_adl ].format_brand);

    } else if ( given_main_format_code == arch_adfsf_350_dshd_1600k_adl ) {
            drive_selected    = ARCHIMEDES_DRIVE_HD;
            computer_selected = ARCHIMEDES_A5000;
            set_current_options(main_formats[ arch_adfsf_350_dshd_1600k_adl ].format_brand);

    /******************************************************************/

    // } else if ( given_main_format_code == apple2_ados_525_ssdd_0114k_d13 ) {
    //     printf("Apple II (.DSK) 114 KB disk detected! - apple2_ados_525_ssdd_0114k_d13\n");  
    //     drive_selected = APPLEII_DRIVE_525_APPDOS_DSK ;
    //     computer_selected = APPLE_IIE;
    //     set_current_options( BRAND_APPLEII );

    } else if ( given_main_format_code == apple2_ados_525_ssdd_0140k_do ) {
        printf("Apple II (.DSK) 140 KB disk detected! - apple2_ados_525_ssdd_0140k_do\n");  
        drive_selected = APPLEII_DRIVE_525_APPDOS_DSK ;
        computer_selected = APPLE_IIE;
        set_current_options( BRAND_APPLEII );

    } else if ( given_main_format_code == coco_decb_525_ssdd_0158k_dsk ) {
        printf("Tandy CoCo (.DSK) 158 KB disk detected! - coco_decb_525_ssdd_0158k_dsk\n");  
        drive_selected = TANDYCOCO_525_SSDD_158_DECB_DSK ;
        computer_selected = TANDY_COCO1;
        set_current_options( BRAND_TANDYCOCO );

    /******************************************************************/

    } else if ( given_main_format_code == coco_decb_525_ssdd_0180k_dsk ) {
        printf("Tandy CoCo (.DSK) 180 KB disk chosen! - coco_decb_525_ssdd_0180k_dsk.\n");  
        drive_selected = TANDYCOCO_525_SSDD_180_DECB_DSK ;
        computer_selected = TANDY_COCO2;
        set_current_options( BRAND_TANDYCOCO );

    } else if ( given_main_format_code == coco_os9_525_ssdd_0180k_dsk ) {
        printf("Tandy CoCo (.DSK) 180 KB disk chosen! - coco_os9_525_ssdd_0180k_dsk.\n");  
        drive_selected = TANDYCOCO_525_SSDD_180_OS9_DSK ;
        computer_selected = TANDY_COCO2;
        set_current_options( BRAND_TANDYCOCO );
            
    } else if ( given_main_format_code == dragon_ddos_525_ssdd_0180k_dsk ) {
        printf("Dragon (.DSK) 180 KB disk chosen! - dragon_ddos_525_ssdd_0180k_dsk.\n");  
        drive_selected = DRAGON_DDOS_525_SSDD_0180K_DSK ;
        computer_selected = DRAGON_32;
        set_current_options( BRAND_DRAGON );  

    } else if ( given_main_format_code == coco_os9_525_dsdd_0360k_dsk ) {
        printf("Tandy CoCo (.DSK) 360 KB disk chosen! - coco_os9_525_dsdd_0360k_dsk or coco_os9_525_ssqd_0360k_dsk (or the 3.5-inch one).\n");  
        drive_selected = TANDYCOCO_525_DSDD_360_OS9_DSK ;
        computer_selected = TANDY_COCO3;
        set_current_options( BRAND_TANDYCOCO );
          
    } else if ( given_main_format_code == coco_os9_525_ssqd_0360k_dsk ) {
        printf("Tandy CoCo (.DSK) 360 KB disk chosen! - coco_os9_525_dsdd_0360k_dsk or coco_os9_525_ssqd_0360k_dsk (or the 3.5-inch one).\n");  
        drive_selected = TANDYCOCO_525_SSQD_360_OS9_DSK ;
        computer_selected = TANDY_COCO3;
        set_current_options( BRAND_TANDYCOCO );

    } else if ( given_main_format_code == dragon_ddos_525_dsdd_0360k_dsk ) {
        printf( "Dragon (.DSK) 360 KB disk detected! - dragon_ddos_525_dsdd_0360k_dsk.\n" );
        drive_selected = DRAGON_DDOS_525_DSDD_0360K_DSK ;
        computer_selected = DRAGON_64;
        set_current_options( BRAND_DRAGON );

    } else if ( given_main_format_code == mac_mfs_350_ssdd_0400k_raw ) {
        printf( "Macintosh (.DSK) 400 KB disk detected! - mac_mfs_350_ssdd_0400k_raw\n" );
        drive_selected = MAC_DRIVE_35_SS_400K ;
        computer_selected = MAC_PLUS;
        set_current_options( BRAND_MACINTOSH );

    } else if ( given_main_format_code == coco_os9_525_dsqd_0720k_dsk ) {
        printf("Tandy CoCo (.DSK) 720 KB disk chosen! - coco_os9_525_dsqd_0720k_dsk (or the 3.5-inch one).\n");  
        drive_selected = TANDYCOCO_525_DSQD_720_OS9_DSK ;
        computer_selected = TANDY_COCO3;
        set_current_options( BRAND_TANDYCOCO );

    } else if ( given_main_format_code == dragon_ddos_525_dsdd_0360k_dsk ) {
        printf( "Dragon (.DSK) 360 KB disk detected! - dragon_ddos_525_dsdd_0360k_dsk.\n" );
        drive_selected = DRAGON_DDOS_525_DSQD_0720K_DSK ;
        computer_selected = DRAGON_TANO;
        set_current_options( BRAND_DRAGON );
            
    } else if ( given_main_format_code == mac_hfs_350_dsdd_0800k_raw ) {
        printf( "Macintosh (.DSK) 800 KB disk detected! - mac_hfs_350_dsdd_0800k_raw\n" );
        drive_selected = MAC_DRIVE_35_DS_800K ;
        computer_selected = MAC_PLUS;
        set_current_options( BRAND_MACINTOSH );

    } else if ( given_main_format_code == mac_hfs_350_ddhd_1440k_raw ) {
        printf( "Macintosh (.DSK) (.DSK) disk image detected! - mac_hfs_350_ddhd_1440k_raw\n" );
        drive_selected = MAC_DRIVE_35_DS_1M4 ;
        computer_selected = MAC_SE30;
        set_current_options( BRAND_MACINTOSH );

    /******************************************************************/

    // } else if ( given_main_format_code == apple2_ados_525_ssdd_0114k_do ) {
    //     printf("Apple II (.DO) 114 KB disk detected! - apple2_ados_525_ssdd_0114k_do\n");  
    //     drive_selected = APPLEII_DRIVE_525_APPDOS_DO ;
    //     computer_selected = APPLE_IIE;
    //     set_current_options( BRAND_APPLEII );

    } else if ( given_main_format_code == apple2_ados_525_ssdd_0140k_do ) {
        printf("Apple II (.DO) 140 KB disk detected! - apple2_ados_525_ssdd_0140k_do\n");  
        drive_selected = APPLEII_DRIVE_525_APPDOS_DO ;
        computer_selected = APPLE_IIE;
        set_current_options( BRAND_APPLEII );

    // } else if ( given_main_format_code == apple2_ados_525_ssdd_0160k_do ) {
    //     printf("Apple II (.DO) 160 KB disk detected! - apple2_ados_525_ssdd_0160k_do\n");  
    //     drive_selected = APPLEII_DRIVE_525_APPDOS_DO ;
    //     computer_selected = APPLE_IIE;
    //     set_current_options( BRAND_APPLEII );

    /******************************************************************/

    } else if ( given_main_format_code == apple2_pdos_525_ssdd_0140k_po ) {
        printf("Apple II disk detected!\n");
        drive_selected = APPLEII_DRIVE_525_PRODOS_PO;
        set_current_options( BRAND_APPLEII );

    } else if ( given_main_format_code == atari_ados_525_sssd_0090k_atr ) {
        printf("Atari (.ATR) 90 KB disk detected!\n");
        computer_selected = ATARI8_800;
        drive_selected = ATARI8_DRIVE_810;
        set_current_options( BRAND_ATARI8BIT );

    } else if ( given_main_format_code == atari_ados_525_ssdd_0130k_atr ) {
        printf("Atari (.ATR) 130 KB disk detected!\n");
        computer_selected = ATARI8_800XL;
        drive_selected = ATARI8_DRIVE_1050_DD130;
        set_current_options( BRAND_ATARI8BIT );

    } else if ( given_main_format_code == atari_ados_525_ssdd_0180k_atr ) {
        printf("Atari (.ATR) 180 KB disk detected!\n");
        computer_selected = ATARI8_800XL;
        drive_selected = ATARI8_DRIVE_1050_DD180;
        set_current_options( BRAND_ATARI8BIT );

    } else if ( given_main_format_code == atari_ados_525_dsdd_0360k_atr ) {
        printf("Atari (.ATR) 360 KB disk detected!\n");
        computer_selected = ATARI8_130XE;
        drive_selected = ATARI8_DRIVE_XF551;
        set_current_options( BRAND_ATARI8BIT );

    /******************************************************************/

    } else if ( given_main_format_code == bbc_dfs_525_sssd_0100k_ssd ) {
        printf("BBC Micro (.SSD) DFS disk of size 100KB detected!\n");
        computer_selected = BBC_MICRO_B;
        drive_selected = BBC_DRIVE_525_SSSD_100_SSD;
        set_current_options( BRAND_BBCMICRO );

    } else if ( given_main_format_code == bbc_dfs_525_ssqd_0200k_ssd ) {
        computer_selected = BBC_MICRO_B;
        printf("BBC Micro (.SSD) DFS disk of size 200KB detected!\n");
        drive_selected = BBC_DRIVE_525_SSQD_200_SSD;
        set_current_options( BRAND_BBCMICRO );

    } else if ( given_main_format_code == bbc_dfs_525_dssd_0200k_dsd ) {
        computer_selected = BBC_MICRO_B;
        printf("BBC Micro (.DSD) DFS disk of size 200KB detected!\n");
        drive_selected = BBC_DRIVE_525_DSQD_200_DSD;
        set_current_options( BRAND_BBCMICRO );

    } else if ( given_main_format_code == bbc_dfs_525_dsqd_0400k_dsd ) { 
        computer_selected = BBC_MICRO_B;           
        printf("BBC Micro (.DSD) DFS disk of size 400KB detected!\n");
        drive_selected = BBC_DRIVE_525_DSQD_400_DSD;
        set_current_options( BRAND_BBCMICRO );

    /******************************************************************/

    } else if ( given_main_format_code == cbm1541_cdos_525_ssdd_0171k_d64 ) {
        printf( "C64/128 / CBM-1541 disk image detected!\n" ); 
        computer_selected = CBM_64;
        drive_selected    = CBM_DRIVE_1541;
        set_current_options(main_formats[ cbm1541_cdos_525_ssdd_0171k_d64 ].format_brand);
        if (current_hardware_selected == ZOOMFLOPPY_CHOSEN) {
            hardware_selected       = ZOOMFLOPPY_CHOSEN; // Set to ZoomFloppy.
            greaseweazle_tree_state = NK_MINIMIZED;      // Greasweazle 
            zoomfloppy_tree_state   = NK_MAXIMIZED;      // *** ZoomFloppy ***
            usbfloppy_tree_state    = NK_MINIMIZED;      // USBFloppy
        };/*end-if*/

    } else if ( given_main_format_code == cbm1571_cdos_525_dsdd_0342k_d71 ) {
        printf( "C64/128 / CBM-1571 disk image detected!\n" ); 
        computer_selected = CBM_128;
        drive_selected    = CBM_DRIVE_1571;
        set_current_options(main_formats[ cbm1571_cdos_525_dsdd_0342k_d71 ].format_brand);
        if (current_hardware_selected == ZOOMFLOPPY_CHOSEN) {
            hardware_selected       = ZOOMFLOPPY_CHOSEN; // Set to ZoomFloppy.
            greaseweazle_tree_state = NK_MINIMIZED;      // Greasweazle 
            zoomfloppy_tree_state   = NK_MAXIMIZED;      // *** ZoomFloppy ***
            usbfloppy_tree_state    = NK_MINIMIZED;      // USBFloppy
        };/*end-if*/

    } else if ( given_main_format_code == cbm1581_cdos_350_dsdd_0800k_d81 ) {
        printf( "C64/128 / CBM-1581 disk image detected!\n" ); 
        computer_selected = CBM_64C;
        drive_selected    = CBM_DRIVE_1581;
        set_current_options(main_formats[ cbm1581_cdos_350_dsdd_0800k_d81 ].format_brand);
        if (current_hardware_selected == ZOOMFLOPPY_CHOSEN) {
            hardware_selected       = ZOOMFLOPPY_CHOSEN; // Set to ZoomFloppy.
            greaseweazle_tree_state = NK_MINIMIZED;      // Greasweazle 
            zoomfloppy_tree_state   = NK_MAXIMIZED;      // *** ZoomFloppy ***
            usbfloppy_tree_state    = NK_MINIMIZED;      // USBFloppy
        };/*end-if*/

    } else if ( given_main_format_code == cbm8050_cdos_525_dsqd_0521k_d80 ) {
        printf( "PET/C64/128 / CBM-8050 disk image detected!\n" ); 
        computer_selected = CBM_PET_8032;
        drive_selected    = CBM_DRIVE_8050;
        set_current_options(main_formats[ cbm8050_cdos_525_dsqd_0521k_d80 ].format_brand);
        // TODO: Maybe make this an option that can be enabled or disabled in the future. 
        // Automatically set to ZoomFloppy since 100TPI drives to use with Greaseweazle are very rare. 
        hardware_selected       = ZOOMFLOPPY_CHOSEN; // Set to ZoomFloppy.
        greaseweazle_tree_state = NK_MINIMIZED;      // Greasweazle 
        zoomfloppy_tree_state   = NK_MAXIMIZED;      // *** ZoomFloppy ***
        usbfloppy_tree_state    = NK_MINIMIZED;      // USBFloppy
        MessageBox(NULL, "FYI: You need a ZoomFloppy and a CBM 8050, CBM 8250, or CBM SFD-1001 floppy drive to make this disk.\n\nThe only other way to make one of these disks is to get a 5.25-inch 100TPI floppy drive. They are rare. You can't just use any 80 track drive. It's an alignment thing. This format is a 77 track format where the actual placement of the tracks is different enough from a typical 96TPI drive that it's just not going to work.", "Info", MB_OK | MB_ICONINFORMATION);

    } else if ( given_main_format_code == cbmsfd1_cdos_525_dsqd_1042k_d82 ) {
        printf( "PET/C64/128 / CBM-8250/SFD-1001 disk image detected!\n" ); 
        computer_selected = CBM_128_80;
        drive_selected    = CBM_DRIVE_SFD1001;
        set_current_options(main_formats[ cbmsfd1_cdos_525_dsqd_1042k_d82 ].format_brand);
        // TODO: Maybe make this an option that can be enabled or disabled in the future. 
        // Automatically set to ZoomFloppy since 100TPI drives to use with Greaseweazle are very rare. 
        hardware_selected       = ZOOMFLOPPY_CHOSEN; // Set to ZoomFloppy.
        greaseweazle_tree_state = NK_MINIMIZED;      // Greasweazle 
        zoomfloppy_tree_state   = NK_MAXIMIZED;      // *** ZoomFloppy ***
        usbfloppy_tree_state    = NK_MINIMIZED;      // USBFloppy

        MessageBox(NULL, "FYI: You need a ZoomFloppy and a CBM 8050, CBM 8250, or CBM SFD-1001 floppy drive to make this disk.\n\nThe only other way to make one of these disks is to get a 5.25-inch 100TPI floppy drive. They are rare. You can't just use any 80 track drive. It's an alignment thing. This format is a 77 track format where the actual placement of the tracks is different enough from a typical 96TPI drive that it's just not going to work.", "Info", MB_OK | MB_ICONINFORMATION);

    /******************************************************************/

    } else if (  given_main_format_code == atarist_fat12_350_ssdd_0360k_st ) {
        printf( "Atari ST (.ST) disk image detected!\n" ); 
        computer_selected = ATARIST_1040STF;
        drive_selected = ATARIST_SF354_SSDD_360K_ST;
        set_current_options( BRAND_ATARIST );

    } else if (  given_main_format_code == atarist_fat12_350_dsdd_0720k_st ) {
        printf( "Atari ST (.ST) disk image detected!\n" ); 
        computer_selected = ATARIST_1040STF;
        drive_selected = ATARIST_SF314_DSDD_720K_ST;
        set_current_options( BRAND_ATARIST );

    } else if (  given_main_format_code == atarist_fat12_350_dshd_1440k_st ) {
        printf( "Atari ST (.ST) disk image detected!\n" ); 
        computer_selected = ATARIST_1040STF;
        drive_selected = ATARIST_HDINT_DSHD_1_4M_ST;
        set_current_options( BRAND_ATARIST );

    } else if (  given_main_format_code == atarist_fat12_350_ssdd_0400k_st ) {
        printf( "Atari ST (.ST) disk image detected!\n" ); 
        computer_selected = ATARIST_1040STF;
        drive_selected = ATARIST_SF354_SSDD_400K_ST;
        set_current_options( BRAND_ATARIST );

    } else if (  given_main_format_code == atarist_fat12_350_ssdd_0440k_st ) {
        printf( "Atari ST (.ST) disk image detected!\n" ); 
        computer_selected = ATARIST_1040STF;
        drive_selected = ATARIST_SF354_SSDD_440K_ST;
        set_current_options( BRAND_ATARIST );

    } else if (  given_main_format_code == atarist_fat12_350_dsdd_0800k_st ) {
        printf( "Atari ST (.ST) disk image detected!\n" ); 
        computer_selected = ATARIST_1040STF;
        drive_selected = ATARIST_SF354_DSDD_800K_ST;
        set_current_options( BRAND_ATARIST );

    } else if (  given_main_format_code == atarist_fat12_350_dsdd_0880k_st ) {
        printf( "Atari ST (.ST) disk image detected!\n" ); 
        computer_selected = ATARIST_1040STF;
        drive_selected = ATARIST_SF354_DSDD_880K_ST;               
        set_current_options( BRAND_ATARIST );

    /******************************************************************/

    } else if (  given_main_format_code == atarist_fat12_350_ssdd_0360k_msa ) {
        printf( "Atari ST (.MSA) disk image detected!\n" ); 
        computer_selected = ATARIST_1040STF;
        drive_selected = ATARIST_SF354_SSDD_360K_MSA;
        set_current_options( BRAND_ATARIST );

    } else if (  given_main_format_code == atarist_fat12_350_dsdd_0720k_msa ) {
        printf( "Atari ST (.MSA) disk image detected!\n" ); 
        computer_selected = ATARIST_1040STF;
        drive_selected = ATARIST_SF314_DSDD_720K_MSA;
        set_current_options( BRAND_ATARIST );

    } else if (  given_main_format_code == atarist_fat12_350_dshd_1440k_msa ) {
        printf( "Atari ST (.MSA) disk image detected!\n" ); 
        computer_selected = ATARIST_1040STF;
        drive_selected = ATARIST_HDINT_DSHD_1_4M_MSA;
        set_current_options( BRAND_ATARIST );

    } else if (  given_main_format_code == atarist_fat12_350_ssdd_0400k_msa ) {
        printf( "Atari ST (.MSA) disk image detected!\n" ); 
        computer_selected = ATARIST_1040STF;
        drive_selected = ATARIST_SF354_SSDD_400K_MSA;
        set_current_options( BRAND_ATARIST );

    } else if (  given_main_format_code == atarist_fat12_350_ssdd_0440k_msa ) {
        printf( "Atari ST (.MSA) disk image detected!\n" ); 
        computer_selected = ATARIST_1040STF;
        drive_selected = ATARIST_SF354_SSDD_440K_MSA;
        set_current_options( BRAND_ATARIST );

    } else if (  given_main_format_code == atarist_fat12_350_dsdd_0800k_msa ) {
        printf( "Atari ST (.MSA) disk image detected!\n" ); 
        computer_selected = ATARIST_1040STF;
        drive_selected = ATARIST_SF354_DSDD_800K_MSA;
        set_current_options( BRAND_ATARIST );

    } else if (  given_main_format_code == atarist_fat12_350_dsdd_0880k_msa ) {
        printf( "Atari ST (.MSA) disk image detected!\n" ); 
        computer_selected = ATARIST_1040STF;
        drive_selected = ATARIST_SF354_DSDD_880K_MSA;               
        set_current_options( BRAND_ATARIST );

    /******************************************************************/

    } else if (  given_main_format_code == atarist_fat12_350_ssdd_0360k_stx ) {
        printf( "Atari ST (.STX) disk image detected!\n" ); 
        computer_selected = ATARIST_1040STF;
        drive_selected = ATARIST_SF354_SSDD_360K_STX;
        set_current_options( BRAND_ATARIST );

    } else if (  given_main_format_code == atarist_fat12_350_dsdd_0720k_stx ) {
        printf( "Atari ST (.STX) disk image detected!\n" ); 
        computer_selected = ATARIST_1040STF;
        drive_selected = ATARIST_SF314_DSDD_720K_STX;
        set_current_options( BRAND_ATARIST );

    } else if (  given_main_format_code == atarist_fat12_350_dshd_1440k_stx ) {
        printf( "Atari ST (.STX) disk image detected!\n" ); 
        computer_selected = ATARIST_1040STF;
        drive_selected = ATARIST_HDINT_DSHD_1_4M_STX;
        set_current_options( BRAND_ATARIST );

    } else if (  given_main_format_code == atarist_fat12_350_ssdd_0400k_stx ) {
        printf( "Atari ST (.STX) disk image detected!\n" ); 
        computer_selected = ATARIST_1040STF;
        drive_selected = ATARIST_SF354_SSDD_400K_STX;
        set_current_options( BRAND_ATARIST );

    } else if (  given_main_format_code == atarist_fat12_350_ssdd_0440k_stx ) {
        printf( "Atari ST (.STX) disk image detected!\n" ); 
        computer_selected = ATARIST_1040STF;
        drive_selected = ATARIST_SF354_SSDD_440K_STX;
        set_current_options( BRAND_ATARIST );

    } else if (  given_main_format_code == atarist_fat12_350_dsdd_0800k_stx ) {
        printf( "Atari ST (.STX) disk image detected!\n" ); 
        computer_selected = ATARIST_1040STF;
        drive_selected = ATARIST_SF354_DSDD_800K_STX;
        set_current_options( BRAND_ATARIST );

    } else if (  given_main_format_code == atarist_fat12_350_dsdd_0880k_stx ) {
        printf( "Atari ST (.STX) disk image detected!\n" ); 
        computer_selected = ATARIST_1040STF;
        drive_selected = ATARIST_SF354_DSDD_880K_STX;               
        set_current_options( BRAND_ATARIST );

    /******************************************************************/

    } else if (  given_main_format_code == atari_ados_525_sssd_0090k_atr ) {
        printf("Atari (.IMG) 90 KB disk detected!\n");
        drive_selected = ATARI8_DRIVE_810;
        computer_selected = ATARI8_800;
        set_current_options( BRAND_ATARI8BIT );

    // } else if (  given_main_format_code == apple2_ados_525_ssdd_0114k_d0 ) {
    //     printf("Apple II (.IMG) 114 KB disk detected! - apple2_ados_525_ssdd_0114k_d0\n");  
    //     drive_selected = APPLEII_DRIVE_525_APPDOS_DO ;
    //     computer_selected = APPLE_IIE;
    //     set_current_options( BRAND_APPLEII );

    } else if (  given_main_format_code == atari_ados_525_ssdd_0130k_atr ) {
        printf("Atari (.IMG) 130 KB disk detected! - atari_ados_525_ssdd_0130k_atr\n");  
        drive_selected = ATARI8_DRIVE_1050_DD130;
        computer_selected = ATARI8_800XL;
        set_current_options( BRAND_ATARI8BIT );

    } else if (  given_main_format_code == apple2_ados_525_ssdd_0140k_do ) {
        printf("Apple II (.IMG) 140 KB disk detected! - apple2_ados_525_ssdd_0140k_do\n");  
        drive_selected = APPLEII_DRIVE_525_APPDOS_DO ;
        computer_selected = APPLE_IIE;
        set_current_options( BRAND_APPLEII );

    // } else if (  given_main_format_code == apple2_ados_525_ssdd_0160k_do ) {
    //     printf("Apple II (.IMG) 160 KB disk detected! - apple2_ados_525_ssdd_0160k_do\n");  
    //     drive_selected = APPLEII_DRIVE_525_APPDOS_DO ;
    //     computer_selected = APPLE_IIE;
    //     set_current_options( BRAND_APPLEII );

    } else if (  given_main_format_code == atari_ados_525_ssdd_0180k_atr ) {
        printf("Atari (.IMG) 180 KB disk detected! - atari_ados_525_ssdd_0180k_atr\n");  
        drive_selected = ATARI8_DRIVE_1050_DD130;
        computer_selected = ATARI8_800XL;
        set_current_options( BRAND_ATARI8BIT );

    } else if (  given_main_format_code == olivettim20_pcos_525_dsdd_0280_img ) { 
        printf("Olivetti M20 (.IMG) 280 KB disk detected! - olivettim20_pcos_525_dsdd_0280_img\n");  
        drive_selected = OLIVETTIM20_PCOS_525_DSDD_0280K_IMG;
        computer_selected = OLIVETTI_M20;
        set_current_options( BRAND_OLIVETTI );

    } else if (  given_main_format_code == msdos_fat12_525_dsdd_0360k_ima ) {
        printf( "PC Compatible / MS-DOS (.IMG) disk image detected! - msdos_fat12_525_dsdd_0360k_ima\n" );
        drive_selected = PC_DRIVE_525_DD_360_IMA ;
        computer_selected = PC_8088;
        set_current_options( BRAND_PCCOMPAT );

    } else if (  given_main_format_code == mac_mfs_350_ssdd_0400k_raw ) {
        printf( "Macintosh (.IMG) 800 KB disk detected! - mac_mfs_350_ssdd_0400k_raw\n" );
        drive_selected = MAC_DRIVE_35_SS_400K ;
        computer_selected = MAC_PLUS;
        set_current_options( BRAND_MACINTOSH );

    } else if (  given_main_format_code == msdos_fat12_350_dsdd_0720k_ima ) {
        printf( "PC Compatible / MS-DOS (.IMG) disk image detected! - msdos_fat12_350_dsdd_0720k_ima\n" );
        drive_selected = PC_DRIVE_35_DD_720_IMA ;
        computer_selected = PC_80386;
        set_current_options( BRAND_PCCOMPAT );
        if (current_hardware_selected == USBFLOPPY_CHOSEN) {
            hardware_selected       = USBFLOPPY_CHOSEN;  // Set to ZoomFloppy.
            greaseweazle_tree_state = NK_MINIMIZED;      // Greasweazle 
            zoomfloppy_tree_state   = NK_MINIMIZED;      // *** ZoomFloppy ***
            usbfloppy_tree_state    = NK_MAXIMIZED;      // USBFloppy
        };/*end-if*/

    } else if (  given_main_format_code == mac_hfs_350_dsdd_0800k_raw ) {
        printf( "Macintosh (.IMG) 800 KB disk detected! - mac_hfs_350_dsdd_0800k_raw\n" );
        drive_selected = MAC_DRIVE_35_DS_800K ;
        computer_selected = MAC_PLUS;
        set_current_options( BRAND_MACINTOSH );

    } else if ( given_main_format_code == msdos_fat12_525_dshd_1200k_ima ) {
        printf( "PC Compatible / MS-DOS (.IMG) disk image detected! - msdos_fat12_525_dshd_1200k_ima\n" );
        drive_selected = PC_DRIVE_525_HD_1M2_IMA ;
        computer_selected = PC_80286;
        set_current_options( BRAND_PCCOMPAT );

    } else if ( given_main_format_code == mac_hfs_350_ddhd_1440k_raw ) {
        drive_selected = MAC_DRIVE_35_DS_1M4;
        computer_selected = MAC_SE30;
        set_current_options( BRAND_MACINTOSH );

    /******************************************************************/

    } else if ( given_main_format_code == msdos_fat12_525_dsdd_0360k_ima ) { 
        drive_selected = PC_DRIVE_525_DD_360_IMA; 
        computer_selected = PC_8088 ;
        set_current_options( BRAND_PCCOMPAT );

    } else if ( given_main_format_code == msdos_fat12_525_dshd_1200k_ima ) { 
        drive_selected = PC_DRIVE_525_HD_1M2_IMA; 
        computer_selected = PC_80286;
        set_current_options( BRAND_PCCOMPAT );

    // } else if ( given_main_format_code == msdos_fat12_350_dsdd_0720k_ima ) { // DUPLICATE???
    //     drive_selected = PC_DRIVE_35_DD_720_IMA;  
    //     computer_selected = PC_80386;
    //     set_current_options( BRAND_PCCOMPAT );
    //     if (current_hardware_selected == USBFLOPPY_CHOSEN) {
    //         hardware_selected       = USBFLOPPY_CHOSEN;  // Set to ZoomFloppy.
    //         greaseweazle_tree_state = NK_MINIMIZED;      // Greasweazle 
    //         zoomfloppy_tree_state   = NK_MINIMIZED;      // *** ZoomFloppy ***
    //         usbfloppy_tree_state    = NK_MAXIMIZED;      // USBFloppy
    //     };/*end-if*/

    } else if ( given_main_format_code == msdos_fat12_350_dshd_1440k_ima ) { 
        drive_selected = PC_DRIVE_35_HD_1M4_IMA;  
        computer_selected = PC_80486;
        set_current_options(BRAND_PCCOMPAT);
        if (current_hardware_selected == USBFLOPPY_CHOSEN) {
            hardware_selected       = USBFLOPPY_CHOSEN;  // Set to ZoomFloppy.
            greaseweazle_tree_state = NK_MINIMIZED;      // Greasweazle 
            zoomfloppy_tree_state   = NK_MINIMIZED;      // *** ZoomFloppy ***
            usbfloppy_tree_state    = NK_MAXIMIZED;      // USBFloppy
        };/*end-if*/

    } else if ( given_main_format_code == msdos_fat12_525_dsdd_0360k_imd ) {
        drive_selected = PC_DRIVE_525_DD_360_IMA; 
        computer_selected = PC_8088 ;
        set_current_options(BRAND_PCCOMPAT);

    } else if ( given_main_format_code == msdos_fat12_525_dshd_1200k_imd ) {
        drive_selected = PC_DRIVE_525_HD_1M2_IMA; 
        computer_selected = PC_80286;
        set_current_options(BRAND_PCCOMPAT);

    } else if ( given_main_format_code == msdos_fat12_350_dsdd_0720k_imd ) { 
        drive_selected = PC_DRIVE_35_DD_720_IMA;  
        computer_selected = PC_80386;
        set_current_options(BRAND_PCCOMPAT);
        if (current_hardware_selected == USBFLOPPY_CHOSEN) {
            hardware_selected       = USBFLOPPY_CHOSEN;  // Set to ZoomFloppy.
            greaseweazle_tree_state = NK_MINIMIZED;      // Greasweazle 
            zoomfloppy_tree_state   = NK_MINIMIZED;      // *** ZoomFloppy ***
            usbfloppy_tree_state    = NK_MAXIMIZED;      // USBFloppy
        };/*end-if*/

    } else if ( given_main_format_code == msdos_fat12_350_dshd_1440k_imd ) {
        drive_selected = PC_DRIVE_35_HD_1M4_IMA;  
        computer_selected = PC_80486;
        set_current_options(BRAND_PCCOMPAT);
        if (current_hardware_selected == USBFLOPPY_CHOSEN) {
            hardware_selected       = USBFLOPPY_CHOSEN;  // Set to ZoomFloppy.
            greaseweazle_tree_state = NK_MINIMIZED;      // Greasweazle 
            zoomfloppy_tree_state   = NK_MINIMIZED;      // *** ZoomFloppy ***
            usbfloppy_tree_state    = NK_MAXIMIZED;      // USBFloppy
        };/*end-if*/

    } else if ( given_main_format_code == olivettim20_pcos_525_dsdd_0280_imd ) {
        printf("Olivetti M20 (.IMD) 280 KB disk detected! - olivettim20_pcos_525_Dsdd_0280_imd\n");  
        drive_selected = OLIVETTIM20_PCOS_525_DSDD_0280K_IMD;
        computer_selected = OLIVETTI_M20;
        set_current_options( BRAND_OLIVETTI );

    /******************************************************************/

    } else if ( given_main_format_code == apple2gs_pd16_350_dsdd_0800k_2mg ) {
            printf("Apple IIGS (.2MG) 800 KB disk detected! - apple2gs_pd16_350_dsdd_0800k_2mg\n"); 
            drive_selected = APPLEII_DRIVE_35_PD16_2MG;  
            computer_selected = APPLE_IIGS; 
            set_current_options( BRAND_APPLEII );

    // else if ( given_main_format_code == apple2_ados_525_ssdd_0114k_do ) {
    //         printf("Apple II (.2MG) 114 KB disk detected! - apple2_ados_525_ssdd_0114k_do\n");  
    //         drive_selected = APPLEII_DRIVE_525_APPDOS_DO ;
    //         computer_selected = APPLE_IIE;
    //         set_current_options( BRAND_APPLEII );

    // else if ( given_main_format_code == apple2_ados_525_ssdd_0140k_do ) {
    //         printf("Apple II (.2MG) 140 KB disk detected! - apple2_ados_525_ssdd_0140k_do\n");  
    //         drive_selected = APPLEII_DRIVE_525_APPDOS_DO ;
    //         computer_selected = APPLE_IIE;
    //         set_current_options( BRAND_APPLEII );

    // else if ( given_main_format_code == apple2_ados_525_ssdd_0160k_do ) {
    //         printf("Apple II (.2MG) 160 KB disk detected! - apple2_ados_525_ssdd_0160k_do\n");  
    //         drive_selected = APPLEII_DRIVE_525_APPDOS_DO ;
    //         computer_selected = APPLE_IIE;
    //         set_current_options( BRAND_APPLEII );

    /******************************************************************/

    } else {   

        // Shit didn't work. 
        printf("setup_format_from_formatcode() returns: FALSE \n");
        MessageBox(NULL, "Unable to determine disk image format from file.\nPlease check your file.", "Info", MB_OK | MB_ICONINFORMATION);
        return FALSE; 

    };/*end-if*/

    printf("setup_format_from_formatcode() returns: TRUE \n");
    return TRUE;

}/*end-func*/

/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/

void on_file_dropped(struct nk_context *ctx, const char *path) {

    int already_exists = 0;
    
    long file_size = get_file_size(path);

    // Then do your format detection here
    const char *dot = strrchr(path, '.');

    // So I don't have to do dot+1 all over the joint. 
    const char *nodot = dot+1;

    /******************************************************************/
    // This section attempts to figure out what a dragged and dropped 
    // file is and then tries to make all the correct settings in the 
    // program; Not including hardware settings.
    /******************************************************************/ 

    if (dot) {

        // This takes a file, and it's file size, and
        // figures out what the format really is, and then
        // sets a bunch of global variables. 
        // TODO: The RIGHT way to do this is that this 
        //       should return a format ID like this: 
        //       cbm1541_cdos_525_ssdd_0171k_d64
        //       and then some code afterward sets all the
        //       global settings. This is because the 
        //       format ID is a like to the main_formats
        //       table that deliberatly holds all this
        //       information. So we should be able to pull
        //       all this stuff and set it once and also
        //       eliminate a bunch of code. In fact this 
        //       refactoring will make it both more 
        //       effienct, smaller code, and make it 
        //       easier to understand for the programmer. 
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> Called at line: %d \n", __LINE__);
        if ( setup_format_from_formatcode( get_format_from_ext_and_size(TRUE, file_size, nodot) ) == TRUE ) { 

            // TODO: This is a GLOBALLY USED VARIBLE and shouldn't just be here. 
            //       I need to like seriously deal. This should be part of some thing
            //       that always runs whenever the current file changes. 
            //       If I ever make this so that you can multiple tasks 
            //       open at the same time, then I need to make all these things as
            //       part of that instantiation in some unified way. 
            currently_selected_file_size = get_file_size(path);
            printf("Value of currently_selected_file_size: %li \n", currently_selected_file_size);

            // Now set a global var for the file extension.
            strcpy(currently_selected_file_ext,nodot);

            for (int i = 0; i < recent_count; i++) {

                if (strcmp(recent_files[i], path) == 0) {
                    already_exists = 1;
                    previous_image_selected = image_selected;
                    image_selected = 2 + i;
                    break;
                };/*end-if*/

            };/*end-for*/
            
            if (!already_exists) {

                if (recent_count < 3) recent_count++;

                for (int i = recent_count - 1; i > 0; i--)
                    strncpy(recent_files[i], recent_files[i-1], MAX_PATH - 1);
                /*end-for*/
                
                strncpy(recent_files[0], path, MAX_PATH - 1);
                
                previous_image_selected = image_selected;
                image_selected = 2;
            
            };/*end-if*/

            currently_selected_file_size = get_file_size(path);
            printf("Value of currently_selected_file_size: %li \n", currently_selected_file_size);

            // Now set a global var for the file extension.
            strcpy(currently_selected_file_ext,nodot);

            // Refresh the Hardware window by closing it then opening it again. 
            nk_window_close(ctx, "Hardware");                                      
            if (hardware_open == TRUE) nk_window_show(ctx, "Hardware", NK_SHOWN);

            // Fuck it. Just force this on any drag and drop of a file.  
            has_currently_selected_file_changed = TRUE;

        };/*end-if*/

        /******************************************************************/
        /******************************************************************/
        /******************************************************************/

    };/*end-if*/

}/*end-func*/

/******************************************************************/
/******************* END DRAG AND DROP STUFF **********************/
/******************************************************************/

/* Fix for loosing a click when the Window isn't focused. */
int drop_just_happened = 0; // For drag and drop focus so we don't loose clicks adn have to click twice, one to capture the focus, adn the other to click on the button!
int window_got_focus_during_drop = 0;

WNDPROC originalWndProc;
LRESULT CALLBACK SubclassProc(HWND hwnd, UINT msg,
                               WPARAM wParam, LPARAM lParam) {
    if (msg == WM_MOUSEACTIVATE) {
        printf("WM_MOUSEACTIVATE received\n");
        return MA_ACTIVATE;
    }
    if (msg == WM_ACTIVATEAPP) {
        printf("WM_ACTIVATEAPP wParam=%d\n", (int)wParam);
        if (wParam == TRUE) { SetFocus(hwnd); return 0; }
    }

    if (msg == WM_ACTIVATE && LOWORD(wParam) == WA_ACTIVE && drop_just_happened) {
        drop_just_happened = 0;
        PostMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
        PostMessage(hwnd, WM_LBUTTONUP, 0, 0);
        return 0;
    }
    if (msg == WM_SETFOCUS && drop_just_happened) {
        window_got_focus_during_drop = 1;
    }
    if (msg == WM_SETFOCUS)   printf("WM_SETFOCUS\n");

    if (msg == WM_KILLFOCUS) {
        drop_just_happened = 1;  // assume any focus loss might be a drag starting
        printf("WM_KILLFOCUS\n");
    }

    //if (msg == WM_LBUTTONDOWN) printf("WM_LBUTTONDOWN\n");
    
    return CallWindowProc(originalWndProc, hwnd, msg, wParam, lParam);
}

/********************************************************************************/

#define open_disk_image() do { \
    /* This is where we trigger an file dialog box. */ \
    /* Browse... */ \
    int is_floppy_to_file = (direction_selected == READING_A_FLOPPY); /* TODO: WTF is this? Update this to be non-Forrest-Gump-code. */ \
    int dialog_ok = is_floppy_to_file ? save_as_file_dialog() : open_file_dialog(); \
    if (dialog_ok) { \
        int already_exists = 0; \
        for (int i = 0; i < recent_count; i++) { \
            if (strcmp(recent_files[i], selected_image_path) == 0) { \
                already_exists = 1; \
                previous_image_selected = image_selected; \
                image_selected = 2 + i; \
                break; \
            } \
        } \
        if (!already_exists) { \
            if (recent_count < 3) recent_count++; \
            for (int i = recent_count - 1; i > 0; i--) strncpy(recent_files[i], recent_files[i-1], MAX_PATH - 1); \
            strncpy(recent_files[0], selected_image_path, MAX_PATH - 1); \
            previous_image_selected = image_selected; \
            image_selected = 2; \
        } \
    } else { \
        previous_image_selected = image_selected; \
        image_selected = 0; \
    } \
\
    if ( !(image_selected<2) ){ \
\
        const char *path = recent_files[image_selected-2]; \
\
        /* Copy into your recent_files and set image_selected */ \
        /* so the rest of the program knows a file is chosen  */ \
\
        int already_exists = 0; \
\
        for (int i = 0; i < recent_count; i++) { \
\
            if (strcmp(recent_files[i], path) == 0) { \
                already_exists = 1; \
                previous_image_selected = image_selected; \
                image_selected = 2 + i; \
                break; \
            };/*end-if*/ \
 \
        };/*end-for*/ \
\
        if (!already_exists) { \
\
            if (recent_count < 3) recent_count++; \
\
            for (int i = recent_count - 1; i > 0; i--) \
                strncpy(recent_files[i], recent_files[i-1], MAX_PATH - 1); \
            /*end-for*/ \
\
            strncpy(recent_files[0], path, MAX_PATH - 1); \
\
            previous_image_selected = image_selected; \
            image_selected = 2; \
\
        };/*end-if*/ \
\
        if (direction_selected == WRITING_A_FLOPPY) { \
\
            long file_size = get_file_size(path); \
\
            /* Then do your format detection here */ \
            const char *dot = strrchr(path, '.'); \
\
            /* So I don't have to do dot+1 all over the joint. */ \
            const char *nodot = dot+1; \
\
            if (dot) { \
\
                if ( setup_format_from_formatcode( get_format_from_ext_and_size(TRUE, file_size, nodot) ) == TRUE ) {   \
\
                    currently_selected_file_size = get_file_size(path); \
                    printf("Value of currently_selected_file_size: %li \n", currently_selected_file_size); \
\
                    /* Now set a global var for the file extension. */ \
                    strcpy(currently_selected_file_ext,nodot); \
\
                    /* Refresh the Hardware window by closing it then opening it again. */ \
                    nk_window_close(ctx, "Hardware"); \
                    if (hardware_open == TRUE) nk_window_show(ctx, "Hardware", NK_SHOWN); \
\
                };/*end-if*/ \
\
            };/*end-if*/ \
\
        };/*end-if*/ \
\
    /* Rip out the file extension with the dot. */ \
    const char *dot = strrchr(recent_files[image_selected-2],'.'); \
\
    /* Pull out the dot so I don't have to do dot+1 all over the joint. */ \
    const char *nodot = dot+1; \
\
    /* Now set the file size for the global varible. */ \
    currently_selected_file_size = get_file_size(recent_files[image_selected-2]); \
\
    /* Now set a global var for the file extension. */ \
    strcpy(currently_selected_file_ext,nodot); \
\
    };/*end-if*/ \
\
    /* Fuck it. Just force this on any drag and drop of a file. */ \
    has_currently_selected_file_changed = TRUE; \
\
} while(0) /*end-define*/

static void build_config_path(const char *argv0, const char *filename, char *out, size_t out_size) {
    strncpy(out, argv0, out_size - 1);
    out[out_size - 1] = '\0';
    char *last_slash = strrchr(out, '\\');
    char *last_fwdsl = strrchr(out, '/');
    char *last       = last_slash > last_fwdsl ? last_slash : last_fwdsl;
    if (last) {
        *(last + 1) = '\0';
        strncat(out, filename, out_size - strlen(out) - 1);
    } else {
        strncpy(out, filename, out_size - 1);
    }
}

static void load_config(const char *argv0,
                        float *r, float *g, float *b, float *a,
                        char *greaseweazle_path,           int greaseweazle_path_size,
                        char *ciderpress2commandline_path, int ciderpress2commandline_path_size,
                        char *opencbm_path,                int opencbm_path_size,
                        char *dd_path,                     int dd_path_size,
                        char *greaseweazle_executable,     int greaseweazle_executable_size,
                        char *ciderpress2_cli_executable,  int ciderpress2_cli_executable_size,
                        char *opencbm_cbmctrl_executable,  int opencbm_cbmctrl_executable_size,
                        char *opencbm_d64copy_executable,  int opencbm_d64copy_executable_size,
                        char *opencbm_imgcopy_executable,  int opencbm_imgcopy_executable_size,
                        char *opencbm_d82copy_executable,  int opencbm_d82copy_executable_size,
                        char *dd_executable,               int dd_executable_size               ){
    char filename[512];
    build_config_path(argv0, "chirons_disk_blaster.ini", filename, sizeof(filename));
    printf("CONFIG: Loading %s\n", filename);

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("CONFIG: Could not open %s, using defaults.\n", filename);
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), fp)) {

        // Skip comments and blank lines
        if (line[0] == ';' || line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

        // Skip section headers
        if (line[0] == '[') continue;

        // Find the = sign
        char *equals = strchr(line, '=');
        if (!equals) continue;

        // Extract and trim the key
        char key[128] = {0};
        int klen = (int)(equals - line);
        if (klen > 127) klen = 127;
        strncpy(key, line, klen);
        int k = strlen(key);
        while (k > 0 && (key[k-1] == ' ' || key[k-1] == '\t')) key[--k] = '\0';

        // Extract and trim the value
        char value[512] = {0};
        char *vstart = equals + 1;
        while (*vstart == ' ' || *vstart == '\t') vstart++;
        strncpy(value, vstart, sizeof(value) - 1);
        int vlen = strlen(value);
        while (vlen > 0 && (value[vlen-1] == '\n' || value[vlen-1] == '\r' ||
                             value[vlen-1] == ' '  || value[vlen-1] == '\t')) value[--vlen] = '\0';
        // Strip surrounding quotes if present
        if (vlen >= 2 && value[0] == '"' && value[vlen-1] == '"') {
            value[vlen-1] = '\0';
            memmove(value, value + 1, vlen);
            vlen -= 2;
        }

        printf("CONFIG: key=`%s` value=`%s`\n", key, value);

        if        (strcmp(key, "options_background_color_r") == 0) { *r = (float)atof(value);
        } else if (strcmp(key, "options_background_color_g") == 0) { *g = (float)atof(value);
        } else if (strcmp(key, "options_background_color_b") == 0) { *b = (float)atof(value);
        } else if (strcmp(key, "options_background_color_a") == 0) { *a = (float)atof(value);

        } else if (strcmp(key, "options_greaseweazle_path") == 0) {
            strncpy(greaseweazle_path, value, greaseweazle_path_size - 1);
            greaseweazle_path[greaseweazle_path_size - 1] = '\0';
        } else if (strcmp(key, "options_ciderpress2cli_path") == 0) {
            strncpy(ciderpress2commandline_path, value, ciderpress2commandline_path_size - 1);
            ciderpress2commandline_path[ciderpress2commandline_path_size - 1] = '\0';
        } else if (strcmp(key, "options_opencbm_path") == 0) {
            strncpy(opencbm_path, value, opencbm_path_size - 1);
            opencbm_path[opencbm_path_size - 1] = '\0';

        } else if (strcmp(key, "options_dd_path") == 0) {
            strncpy(dd_path, value, dd_path_size - 1);
            opencbm_path[dd_path_size - 1] = '\0';

        } else if (strcmp(key, "options_greaseweazle_executable") == 0) {
            strncpy(greaseweazle_executable, value, greaseweazle_executable_size - 1);
            greaseweazle_executable[greaseweazle_executable_size - 1] = '\0';
        } else if (strcmp(key, "options_ciderpress2_cli_executable") == 0) {
            strncpy(ciderpress2_cli_executable, value, ciderpress2_cli_executable_size - 1);
            ciderpress2_cli_executable[ciderpress2_cli_executable_size - 1] = '\0';
        } else if (strcmp(key, "options_opencbm_cbmctrl_executable") == 0) {
            strncpy(opencbm_cbmctrl_executable, value, opencbm_cbmctrl_executable_size - 1);
            opencbm_cbmctrl_executable[opencbm_cbmctrl_executable_size - 1] = '\0';
        } else if (strcmp(key, "options_opencbm_d64copy_executable") == 0) {
            strncpy(opencbm_d64copy_executable, value, opencbm_d64copy_executable_size - 1);
            opencbm_d64copy_executable[opencbm_d64copy_executable_size - 1] = '\0';
        } else if (strcmp(key, "options_opencbm_imgcopy_executable") == 0) {
            strncpy(opencbm_imgcopy_executable, value, opencbm_imgcopy_executable_size - 1);
            opencbm_imgcopy_executable[opencbm_imgcopy_executable_size - 1] = '\0';
        } else if (strcmp(key, "options_opencbm_d82copy_executable") == 0) {
            strncpy(opencbm_d82copy_executable, value, opencbm_d82copy_executable_size - 1);
            opencbm_d82copy_executable[opencbm_d82copy_executable_size - 1] = '\0';

        } else if (strcmp(key, "options_dd_executable") == 0) {
            strncpy(dd_executable, value, dd_executable_size - 1);
            dd_executable[dd_executable_size - 1] = '\0';
        }

    }

    fclose(fp);
    printf("CONFIG: Loaded %s\n", filename);

}/*end-func*/

static void save_config(const char *argv0,
                        float r, float g, float b, float a,
                        const char *greaseweazle_path,
                        const char *ciderpress2commandline_path,
                        const char *opencbm_path,
                        const char *dd_path,
                        const char *greaseweazle_executable,
                        const char *ciderpress2_cli_executable,
                        const char *opencbm_cbmctrl_executable,
                        const char *opencbm_d64copy_executable,
                        const char *opencbm_imgcopy_executable,
                        const char *opencbm_d82copy_executable,
                        const char *dd_executable               ) {
    char filename[512];
    build_config_path(argv0, "chirons_disk_blaster.ini", filename, sizeof(filename));
    printf("CONFIG: Saving %s\n", filename);

    // Read the entire existing file into memory
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("CONFIG: Could not open %s for reading, creating new file.\n", filename);
        fp = fopen(filename, "w");
        if (!fp) { printf("CONFIG: Could not create %s.\n", filename); return; }
        fprintf(fp, "; Chiron's Disk Blaster - Initialization and Configuration File\n");
        fprintf(fp, "[User Interface]\n");
        fprintf(fp, "options_background_color_r = %.2f\n", r);
        fprintf(fp, "options_background_color_g = %.2f\n", g);
        fprintf(fp, "options_background_color_b = %.2f\n", b);
        fprintf(fp, "options_background_color_a = %.2f\n", a);
        fprintf(fp, "[External Paths]\n");
        fprintf(fp, "options_greaseweazle_path = \"%s\"\n", greaseweazle_path);
        fprintf(fp, "options_ciderpress2cli_path = \"%s\"\n", ciderpress2commandline_path);
        fprintf(fp, "options_opencbm_path = \"%s\"\n", opencbm_path);
        fprintf(fp, "options_dd_path = \"%s\"\n", dd_path);
        fprintf(fp, "[External Executables]\n");
        fprintf(fp, "options_greaseweazle_executable = \"%s\"\n", greaseweazle_executable);
        fprintf(fp, "options_ciderpress2_cli_executable = \"%s\"\n", ciderpress2_cli_executable);
        fprintf(fp, "options_opencbm_cbmctrl_executable = \"%s\"\n", opencbm_cbmctrl_executable);
        fprintf(fp, "options_opencbm_d64copy_executable = \"%s\"\n", opencbm_d64copy_executable);
        fprintf(fp, "options_opencbm_imgcopy_executable = \"%s\"\n", opencbm_imgcopy_executable);
        fprintf(fp, "options_opencbm_d82copy_executable = \"%s\"\n", opencbm_d82copy_executable);
        fprintf(fp, "options_dd_executable = \"%s\"\n", dd_executable);
        fclose(fp);
        printf("CONFIG: Created new %s\n", filename);
        return;
    }

    // Slurp the whole file into a buffer
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);
    char *file_buf = (char *)malloc(file_size + 1);
    if (!file_buf) { fclose(fp); printf("CONFIG: Out of memory.\n"); return; }
    long bytes_read = (long)fread(file_buf, 1, file_size, fp);
    file_buf[bytes_read] = '\0';
    fclose(fp);

    // Write it back out line by line, replacing matching key strokes
    fp = fopen(filename, "w");
    if (!fp) { free(file_buf); printf("CONFIG: Could not open %s for writing.\n", filename); return; }

    // Track which key strokes we've already written to handle duplicates
    int wrote_r       = 0, wrote_g        = 0, wrote_b            = 0, wrote_a       = 0;
    int wrote_gw_path = 0, wrote_cp2_path = 0, wrote_opencbm_path = 0, wrote_dd_path = 0;
    int wrote_gw_exe  = 0, wrote_cp2_exe  = 0; 
    int wrote_cbmctrl = 0, wrote_d64copy  = 0, wrote_imgcopy      = 0, wrote_d82copy = 0, wrote_dd_exe = 0;

    char *cur = file_buf;
    while (*cur) {

        // Find end of line
        char *eol = strchr(cur, '\n');
        int len = eol ? (int)(eol - cur + 1) : (int)strlen(cur);

        // Copy line into a null-terminated buffer for processing
        char line[512] = {0};
        strncpy(line, cur, len < 511 ? len : 511);

        // Find first non-whitespace character
        char *trimmed = line;
        while (*trimmed == ' ' || *trimmed == '\t') trimmed++;

        // Pass through comments, blank lines, and section headers unchanged
        if (trimmed[0] == ';' || trimmed[0] == '#' ||
            trimmed[0] == '[' || trimmed[0] == '\n' ||
            trimmed[0] == '\r' || trimmed[0] == '\0') {
            fprintf(fp, "%.*s", len, cur);
            cur += len;
            continue;
        }

        // It's a key=value line - find the = sign
        char *equals = strchr(trimmed, '=');
        int matched = 0;
        if (equals) {

            // Extract key - everything before the =
            char key[128] = {0};
            int klen = (int)(equals - trimmed);
            if (klen > 127) klen = 127;
            strncpy(key, trimmed, klen);

            // Trim trailing whitespace from key
            int k = strlen(key);
            while (k > 0 && (key[k-1] == ' ' || key[k-1] == '\t')) key[--k] = '\0';

            if (strcmp(key, "options_background_color_r") == 0) {
                if (!wrote_r)        { fprintf(fp, "options_background_color_r = %.2f\n", r); wrote_r = 1; }
                matched = 1;
            } else if (strcmp(key, "options_background_color_g") == 0) {
                if (!wrote_g)        { fprintf(fp, "options_background_color_g = %.2f\n", g); wrote_g = 1; }
                matched = 1;
            } else if (strcmp(key, "options_background_color_b") == 0) {
                if (!wrote_b)        { fprintf(fp, "options_background_color_b = %.2f\n", b); wrote_b = 1; }
                matched = 1;
            } else if (strcmp(key, "options_background_color_a") == 0) {
                if (!wrote_a)        { fprintf(fp, "options_background_color_a = %.2f\n", a); wrote_a = 1; }
                matched = 1;
            } else if (strcmp(key, "options_greaseweazle_path") == 0) {
                if (!wrote_gw_path)  { fprintf(fp, "options_greaseweazle_path = \"%s\"\n", greaseweazle_path); wrote_gw_path = 1; }
                matched = 1;
            } else if (strcmp(key, "options_ciderpress2cli_path") == 0) {
                if (!wrote_cp2_path) { fprintf(fp, "options_ciderpress2cli_path = \"%s\"\n", ciderpress2commandline_path); wrote_cp2_path = 1; }
                matched = 1;
            } else if (strcmp(key, "options_opencbm_path") == 0) {
                if (!wrote_opencbm_path) { fprintf(fp, "options_opencbm_path = \"%s\"\n", opencbm_path); wrote_opencbm_path = 1; }
                matched = 1;
            } else if (strcmp(key, "options_dd_path") == 0) {
                if (!wrote_dd_path) { fprintf(fp, "options_dd_path = \"%s\"\n", dd_path); wrote_dd_path = 1; }
                matched = 1;
            } else if (strcmp(key, "options_greaseweazle_executable") == 0) {
                if (!wrote_gw_exe)   { fprintf(fp, "options_greaseweazle_executable = \"%s\"\n", greaseweazle_executable); wrote_gw_exe = 1; }
                matched = 1;
            } else if (strcmp(key, "options_ciderpress2_cli_executable") == 0) {
                if (!wrote_cp2_exe)  { fprintf(fp, "options_ciderpress2_cli_executable = \"%s\"\n", ciderpress2_cli_executable); wrote_cp2_exe = 1; }
                matched = 1;
            } else if (strcmp(key, "options_opencbm_cbmctrl_executable") == 0) {
                if (!wrote_cbmctrl)  { fprintf(fp, "options_opencbm_cbmctrl_executable = \"%s\"\n", opencbm_cbmctrl_executable); wrote_cbmctrl = 1; }
                matched = 1;
            } else if (strcmp(key, "options_opencbm_d64copy_executable") == 0) {
                if (!wrote_d64copy)  { fprintf(fp, "options_opencbm_d64copy_executable = \"%s\"\n", opencbm_d64copy_executable); wrote_d64copy = 1; }
                matched = 1;
            } else if (strcmp(key, "options_opencbm_imgcopy_executable") == 0) {
                if (!wrote_imgcopy)  { fprintf(fp, "options_opencbm_imgcopy_executable = \"%s\"\n", opencbm_imgcopy_executable); wrote_imgcopy = 1; }
                matched = 1;
            } else if (strcmp(key, "options_opencbm_d82copy_executable") == 0) {
                if (!wrote_d82copy)  { fprintf(fp, "options_opencbm_d82copy_executable = \"%s\"\n", opencbm_d82copy_executable); wrote_d82copy = 1; }
                matched = 1;
            } else if (strcmp(key, "options_dd_executable") == 0) {
                if (!wrote_dd_exe)  { fprintf(fp, "options_dd_executable = \"%s\"\n", dd_executable); wrote_dd_exe = 1; }
                matched = 1;
            }
        }

        // If we didn't replace it, write the original line as-is
        if (!matched) fprintf(fp, "%.*s", len, cur);

        cur += len;
    }

    fclose(fp);
    free(file_buf);
    printf("CONFIG: Saved %s\n", filename);
}

#include <shobjidl.h>
#include <objbase.h>

// Opens a folder picker dialog and writes the chosen path into out_buf.
// Returns 1 if the user picked something, 0 if they cancelled.
static int browse_for_folder(HWND hwnd, const char *title, char *out_buf, int out_size) {
    IFileDialog *pfd = NULL;
    HRESULT hr = CoCreateInstance(&CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER,
                                   &IID_IFileDialog, (void**)&pfd);
    if (FAILED(hr)) return 0;

    DWORD dwOptions;
    pfd->lpVtbl->GetOptions(pfd, &dwOptions);
    pfd->lpVtbl->SetOptions(pfd, dwOptions | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);

    wchar_t wtitle[256];
    MultiByteToWideChar(CP_UTF8, 0, title, -1, wtitle, 256);
    pfd->lpVtbl->SetTitle(pfd, wtitle);

    hr = pfd->lpVtbl->Show(pfd, hwnd);
    if (SUCCEEDED(hr)) {
        IShellItem *psi = NULL;
        pfd->lpVtbl->GetResult(pfd, &psi);
        PWSTR pszPath = NULL;
        psi->lpVtbl->GetDisplayName(psi, SIGDN_FILESYSPATH, &pszPath);
        WideCharToMultiByte(CP_UTF8, 0, pszPath, -1, out_buf, out_size, NULL, NULL);
        CoTaskMemFree(pszPath);
        psi->lpVtbl->Release(psi);
        pfd->lpVtbl->Release(pfd);
        return 1;
    }
    pfd->lpVtbl->Release(pfd);
    return 0;
}

/********************************************************************************/
/************************* USER INTERFACE CONSTANTS *****************************/
/********************************************************************************/

// UI Colors - Button Colors - Dropdown Colors

// NORMAL --> MOUSE OFF
// windows 11 button color   nk_rgb(  76, 194, 255 ) // bright blue
// windows 11 text color     nk_rgb(  21,  54,  71 ) // very dark blue text 
#define UI_BACK_NORMAL_COLOR nk_rgb(  76, 194, 255 )
#define UI_TEXT_NORMAL_COLOR nk_rgb(  21,  54,  71 )

// HOVER --> MOUSE OVER 
// windows 11 button color  nk_rgb(  71, 177, 233 ) //  a little less bright blue
// windows 11 text color    nk_rgb(  20,  49,  65 ) //  very dark blue text 
#define UI_BACK_HOVER_COLOR nk_rgb(  71, 177, 233 )
#define UI_TEXT_HOVER_COLOR nk_rgb(  20,  49,  65 )

// ACTIVE --> MOUSE DOWN (button press)
// windows 11 button color   nk_rgb(  66, 161, 211 ) // even less bright blue 
// windows 11 text color     nk_rgb(  42, 103, 135 ) // less dark text 
#define UI_BACK_ACTIVE_COLOR nk_rgb(  66, 161, 211 )
#define UI_TEXT_ACTIVE_COLOR nk_rgb(  42, 103, 135 )

// General Colors
#define BLACK   nk_rgb(   0,   0,   0 )
#define GREY    nk_rgb( 127, 127, 127 )         
#define WHITE   nk_rgb( 255, 255, 255 )
#define RED     nk_rgb( 255,   0,   0 )
#define GREEN   nk_rgb(   0, 255,   0 )
#define BLUE    nk_rgb(   0,   0, 255 )
#define CYAN    nk_rgb(   0, 255, 255 )
#define MAGENTA nk_rgb( 255,   0, 255 )
#define YELLOW  nk_rgb( 255, 255,   0 )

/********************************************************************************/
static SDL_AudioDeviceID about_audio = 0;

void play_sound(const char *argv0, const char *relative_path) {
    
    if (about_audio != 0) return;  // already playing

    char full_path[1024];
    build_full_path(argv0, relative_path, full_path, sizeof(full_path));

    SDL_AudioSpec spec, obtained;
    Uint32 len;
    Uint8 *buf;

    if (SDL_LoadWAV(full_path, &spec, &buf, &len) == NULL) {
        fprintf(stderr, "SDL_LoadWAV failed for '%s': %s\n", full_path, SDL_GetError());
        return;
    }

    about_audio = SDL_OpenAudioDevice(NULL, 0, &spec, &obtained, SDL_AUDIO_ALLOW_ANY_CHANGE);
    if (about_audio == 0) {
        fprintf(stderr, "SDL_OpenAudioDevice failed: %s\n", SDL_GetError());
        SDL_FreeWAV(buf);
        return;
    }

    /* Convert if the device gave us a different spec than the WAV */
    if (obtained.freq != spec.freq || obtained.format != spec.format || obtained.channels != spec.channels) {
        SDL_AudioCVT cvt;
        SDL_BuildAudioCVT(&cvt, spec.format, spec.channels, spec.freq,
                                obtained.format, obtained.channels, obtained.freq);
        if (cvt.needed) {
            cvt.len = len;
            cvt.buf = (Uint8*)SDL_malloc(len * cvt.len_mult);
            SDL_memcpy(cvt.buf, buf, len);
            SDL_ConvertAudio(&cvt);
            SDL_QueueAudio(about_audio, cvt.buf, cvt.len_cvt);
            SDL_free(cvt.buf);
        } else {
            SDL_QueueAudio(about_audio, buf, len);
        }
    } else {
        SDL_QueueAudio(about_audio, buf, len);
    }

    SDL_PauseAudioDevice(about_audio, 0);
    SDL_FreeWAV(buf);

}/*end-func*/

void stop_sound(void) {
    if (about_audio != 0) {
        SDL_ClearQueuedAudio(about_audio);
        SDL_CloseAudioDevice(about_audio);
        about_audio = 0;
    }
}/*end-func*/


/********************************************************************************/
/********************************************************************************/
/*********************************** MAIN ***************************************/
/********************************************************************************/
/********************************************************************************/

int main(int argc, char *argv[]) {

    /********** Platform **********/
    SDL_Window *win;
    SDL_GLContext glContext;
    int win_width, win_height;
    int running = 1;

    /* GUI */
    struct nk_context *ctx;
    struct nk_colorf bg;

    #ifdef INCLUDE_CONFIGURATOR
    static struct nk_color color_table[NK_COLOR_COUNT];
    memcpy(color_table, nk_default_color_style, sizeof(color_table));
    #endif

    NK_UNUSED(argc);
    NK_UNUSED(argv);

    /* SDL setup */
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS|SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    };/*end-if*/

    // For options config dialog box windows 11 stuff.
    CoInitialize(NULL);

    /********** Sound **********/
    SDL_AudioSpec wav_spec;
    Uint32 wav_length;
    Uint8 *wav_buffer;

    if (SDL_LoadWAV("about.wav", &wav_spec, &wav_buffer, &wav_length) == NULL) {
        // handle error
    }

    /********************************/
    /******** DISPLAY SPLASH ********/
    /********************************/

    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");

    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    };

    SDL_Window *splash_win = SDL_CreateWindow(
        "",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        658, 372,
        SDL_WINDOW_BORDERLESS | SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP
    );

    char full_path[1024];
    build_full_path(argv[0], "assets/images/application/about_graphic.bmp", full_path, sizeof(full_path));

    SDL_Surface *screen = SDL_GetWindowSurface(splash_win);
    SDL_Surface *image  = SDL_LoadBMP(full_path);
    if (image) {
        SDL_BlitScaled(image, NULL, screen, NULL);
        SDL_FreeSurface(image);
    } else {
        fprintf(stderr, "SDL_LoadBMP failed for '%s': %s\n", full_path, SDL_GetError());
    }
    SDL_UpdateWindowSurface(splash_win);

    /****************************/

    // // SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    // // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    // // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    /****************************/

    win = SDL_CreateWindow("Chiron's Disk Blaster",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_HIDDEN|SDL_WINDOW_RESIZABLE|SDL_WINDOW_ALLOW_HIGHDPI);
    if (!win) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        CoUninitialize();
        return 1;
    };/*end-if*/

    /**************************/
    /******** SET ICON ********/
    /**************************/
    SDL_Surface *icon = SDL_LoadBMP("icon.bmp");
    if (icon) {
        SDL_SetWindowIcon(win, icon);
        SDL_FreeSurface(icon);
    }
    /**************************/

    #ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
    #define DWMWA_USE_IMMERSIVE_DARK_MODE 20
    #endif

    /* Dark Mode title bar */
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(win, &wmInfo);
    HWND hwnd = wmInfo.info.win.window;

    /* Fix for loosing a click when the Window isn't focused. */
    originalWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)SubclassProc);

    /* Enable Dark Mode title bar */
    BOOL useDarkMode = TRUE;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof(useDarkMode));

    glContext = SDL_GL_CreateContext(win);
    // if (!glContext) {
    //     fprintf(stderr, "SDL_GL_CreateContext failed: %s\n", SDL_GetError());
    //     SDL_DestroyWindow(win);
    //     SDL_Quit();
    //     return 1;
    // };/*end-if*/
    if (!glContext) {
        fprintf(stderr, "SDL_GL_CreateContext failed: %s\n", SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "GL Error",
            SDL_GetError(), NULL);  // <-- add this, shows a popup even with -mwindows
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    if (SDL_GL_SetSwapInterval(1) == 0) {
        SDL_Log("VSync enabled");
    } else {
        SDL_Log("VSync not available: %s", SDL_GetError());
    };/*end-if*/

    SDL_GetWindowSize(win, &win_width, &win_height);

    /* OpenGL setup */
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    /* OpenGL 3 or OpenGL 2 on Windows 11 */
    glewExperimental = 1;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to setup GLEW\n");
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    };/*end-if*/

    printf("GL Version: %s\n", glGetString(GL_VERSION));
    printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    fflush(stdout);

    ctx = nk_sdl_init(win);
    { struct nk_font_atlas *atlas; /* WTF IS THIS?!??! */
      nk_sdl_font_stash_begin(&atlas);
      nk_sdl_font_stash_end();
    }

    // Enable SDL Drag and Drop Support
    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

    /****************************************/      
    /****************************************/
    // Load All Images
    /****************************************/
    /****************************************/
    #include "load_all_images.h"
    /****************************************/

    /********************************************************************************/
    /********************************************************************************/
    /******************************** LOAD OPTIONS **********************************/
    /********************************************************************************/
    /********************************************************************************/

    load_config(argv[0],
                &options_background_color_r,
                &options_background_color_g,
                &options_background_color_b,
                &options_background_color_a,
                options_greaseweazle_path,           sizeof(options_greaseweazle_path),
                options_ciderpress2cli_path,         sizeof(options_ciderpress2cli_path),
                options_opencbm_path,                sizeof(options_opencbm_path),
                options_dd_path,                     sizeof(options_dd_path),
                options_greaseweazle_executable,     sizeof(options_greaseweazle_executable),
                options_ciderpress2_cli_executable,  sizeof(options_ciderpress2_cli_executable),
                options_opencbm_cbmctrl_executable,  sizeof(options_opencbm_cbmctrl_executable),
                options_opencbm_d64copy_executable,  sizeof(options_opencbm_d64copy_executable),
                options_opencbm_imgcopy_executable,  sizeof(options_opencbm_imgcopy_executable),
                options_opencbm_d82copy_executable,  sizeof(options_opencbm_d82copy_executable),
                options_dd_executable,               sizeof(options_dd_executable)
    );/*end-load_config*/

    // Background Color :-)
    // bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
    bg.r = options_background_color_r ;
    bg.g = options_background_color_g ; 
    bg.b = options_background_color_b ; 
    bg.a = options_background_color_a ;

    /********************************************************************************/
    /********************************************************************************/
    /********************************************************************************/
    /********************************************************************************/
    /********************************************************************************/

    /********************************************************************************/
    /************************* DEFAULT RESET STRINGS ********************************/
    /********************************************************************************/

    // Greasweazle - Set the default state of the reset string.
    if      ( hw_gw_reset == TRUE  ) snprintf(gw_reset_string , sizeof(gw_reset_string) , "echo \"Greaseweazle reset enabled.\" && \"%s\\%s\" reset" , options_greaseweazle_path , options_greaseweazle_executable); 
    else if ( hw_gw_reset == FALSE ) strcpy(gw_reset_string,"echo \"Greaseweazle reset disabled.\""); 

    // ZoomFloppy - Set the default statse of the reset string.
    if      ( hw_zf_reset == TRUE  ) snprintf(zf_reset_string , sizeof(zf_reset_string) , "echo \"OpenCBM:cbmctrl reset enabled\" && \"%s\\%s\" reset" , options_opencbm_path, options_opencbm_cbmctrl_executable); 
    else if ( hw_zf_reset == FALSE ) strcpy(zf_reset_string,"echo \"OpenCBM:cbmctrl reset disabled.\"");

    /********************************************************************************/
    /************************* Get Hardware Info ************************************/
    /********************************************************************************/

    // Greaseweazle Hardware Info
    
    GreasewazleInfo gw_info = {0};
    get_greaseweazle_info(&gw_info);
    printf("Greaseweazle model: %s\n", gw_info.model);
    printf("Greaseweazle firmware: %s\n", gw_info.firmware);

    // ZoomFloppy Hardware Info

    ZoomFloppyInfo zf_info = {0};
    get_zoomfloppy_info(&zf_info);
    printf("ZoomFloppy version: %s\n", zf_info.version);
    printf("ZoomFloppy build date: %s\n", zf_info.build_date);

    int  detected_devs[MAX_IEC_DEVS]                     = {0};
    char detected_models[MAX_IEC_DEVS][DETECT_MODEL_LEN] = {{0}};
    char raw_out[1024]                                    = {0};
    // int count = run_cbmctrl_detect(detected_devs, detected_models, raw_out, sizeof(raw_out));
    zf_info.device_count = run_cbmctrl_detect(detected_devs, detected_models, raw_out, sizeof(raw_out));

    if (zf_info.device_count == 0) strcpy(zf_info.device_count_string, "No devices found.");
    else snprintf(zf_info.device_count_string, sizeof(zf_info.device_count_string), "%i", zf_info.device_count);

    // USB Floppy Drive Hardware Info
    
    char usbfloppy_device_name[256] = {0};

    if (!is_floppy_drive_connected()) {
        printf("No floppy drive(s) connected.\n");
        strcpy(usbfloppy_device_name,"Not found.");
    } else {
        if (get_floppy_device_string(usbfloppy_device_name, sizeof(usbfloppy_device_name))) {
            printf("Floppy drive detected: %s\n", usbfloppy_device_name);
        } else {
            printf("Floppy drive detected (could not read device name).\n");
        };/*end-if*/
    };/*end-if*/

    printf("usbfloppy_device_name output:`%s` \n", usbfloppy_device_name);

    // CiderPress 2 Software Version Info
    CiderPress2Info cp2_info = {0};
    get_ciderpress2_info(&cp2_info);

    // DD Software Version Info
    DDInfo dd_info = {0};
    get_dd_info(&dd_info);

    /********************************/
    /******** DESTROY SPLASH ********/
    /********************************/

    SDL_DestroyWindow(splash_win); // take down the splash window. this NEEDS to happen BEFORE we might show the FIRST DIALOG BOX! This is becuase it'll be covered up by the always on top splash screen.  
    SDL_ShowWindow(win);  // <-- reveal the window here, everything is ready

    /********************************/

    /***************************************************************/
    /******** DETECT HARDWARE VERSIONS AND TRIGGER WARNINGS ********/
    /***************************************************************/

    // If the Greaseweazle external software is not version "1.23" and it's not "Not found." then it's some version that isn't the one this was made for. FYI: strcmp(gw_info.host_tools,"1.23")==0 <-- strings are equal. Which like... makes sense because it returns info that's relevant... but also fuck you for not just being one. 
    if (strcmp(gw_info.host_tools,"1.23")!=0 && strcmp(gw_info.model,"Not found.")!=0) MessageBox(NULL, "This version of Greaseweazle is not the 1.23 version this program was designed for.\nPlease check your settings in Options!", "Error", MB_OK | MB_ICONERROR);

    // If the ZoomeFloppy/OpenCBM external software is not version "1.23" and it's not "Not found." then it's some version that isn't the one this was made for. FYI: strcmp(gw_info.host_tools,"1.23")==0 <-- strings are equal. Which like... makes sense because it returns info that's relevant... but also fuck you for not just being one. 
    if (strcmp(zf_info.version,"0.4.99.104")!=0 && strcmp(zf_info.version,"Not found.")!=0) MessageBox(NULL, "This version of OpenCBM is not the 0.4.99.104 version this program was designed for.\nPlease check your settings in Options!", "Error", MB_OK | MB_ICONERROR);

    // If the CiderPress2 external software is not version "1.2.0" and it's not "Not found." then it's some version that isn't the one this was made for. FYI: strcmp(gw_info.host_tools,"1.23")==0 <-- strings are equal. Which like... makes sense because it returns info that's relevant... but also fuck you for not just being one. 
    if (strcmp(cp2_info.version,"v1.2.0")!=0 && strcmp(cp2_info.version,"Not found.")!=0) MessageBox(NULL, "This version of CiderPress2 is not the 1.2.0 version this program was designed for.\nPlease check your settings in Options!", "Error", MB_OK | MB_ICONERROR);

    // If the DD external software is not version "0.6beta3" and it's not "Not found." then it's some version that isn't the one this was made for. FYI: strcmp(gw_info.host_tools,"1.23")==0 <-- strings are equal. Which like... makes sense because it returns info that's relevant... but also fuck you for not just being one. 
    if (strcmp(dd_info.version,"0.6beta3")!=0 && strcmp(dd_info.version,"Not found.")!=0) MessageBox(NULL, "This version of dd is not the 0.6beta3 version this program was designed for.\nPlease check your settings in Options!", "Error", MB_OK | MB_ICONERROR);

    // If the CiderPress2 command line version cp2.exe is NOT FOUND AT ALL then warn the user!
    if (strcmp(cp2_info.version,"Not found.")==0) MessageBox(NULL, "The CiderPress2 external program cp2.exe is needed to convert between some files like the AppleIIGS .2MG format. This will not work without the free and open source software.\nPlease check your settings in Options!", "Error", MB_OK | MB_ICONERROR);

    printf("------------------------------------------------------\n");
    printf("gw_info.host_tools: ``%s`` \n", gw_info.host_tools);
    printf("gw_info.model:      ``%s`` \n", gw_info.model);
    printf("zf_info.version:    ``%s`` \n", zf_info.version);
    printf("cp2_info.version:   ``%s`` \n", cp2_info.version);
    printf("dd_info.version:    ``%s`` \n", dd_info.version);
    printf("------------------------------------------------------\n");

    // See if there's literally nothing installed, and thus the program can't do anything!
    if ( strcmp( zf_info.version , "Not found." )  == 0 && 
         strcmp( zf_info.version , "Not found." )  == 0 &&
         strcmp( dd_info.version , "Not found." )  == 0     
        ){

        MessageBox(NULL, "There's literally no external software for accessing floppy drives that this program access. This means that the program can't do anything!\nPlease check your settings in Options.\nSee Help for more information. Thanks!", "Error", MB_OK | MB_ICONERROR);

    };/*end-if*/

    /***************************************************/
    // Process Arguments 
    // For Command Line and File Associating Processing!
    /***************************************************/

    // If there's a first argument on the command line. 
    if ( argc>=2 ){

        const char *path = argv[1];

        // Copy into your recent_files and set image_selected
        // so the rest of the program knows a file is chosen

        int already_exists = 0;
        
        long file_size = get_file_size(path);

        for (int i = 0; i < recent_count; i++) {

            if (strcmp(recent_files[i], path) == 0) {
                already_exists = 1;
                previous_image_selected = image_selected;
                image_selected = 2 + i;
                break;
            };/*end-if*/

        };/*end-for*/
        
        if (!already_exists) {

            if (recent_count < 3) recent_count++;

            for (int i = recent_count - 1; i > 0; i--)
                strncpy(recent_files[i], recent_files[i-1], MAX_PATH - 1);
            /*end-for*/
            
            strncpy(recent_files[0], path, MAX_PATH - 1);
            
            previous_image_selected = image_selected;
            image_selected = 2;
        
        };/*end-if*/

        // Then do your format detection here
        const char *dot = strrchr(path, '.');

        // TODO: This is a GLOBALLY USED VARIBLE and shouldn't just be here. 
        //       I need to like seriously deal. This should be part of some thing
        //       that always runs whenever the current file changes. 
        //       If I ever make this so that you can multiple tasks 
        //       open at the same time, then I need to make all these things as
        //       part of that instantiation in some unified way. 
        // So I don't have to do dot+1 all over the joint. 
        const char *nodot = dot+1;

        // /******************************************************************/
        // // This section attempts to figure out what a dragged and dropped 
        // // file is and then tries to make all the correct settings in the 
        // // program; Not including hardware settings.
        // /******************************************************************/ 

        if (dot) {

            // This takes a file, and it's file size, and
            // figures out what the format really is, and then
            // sets a bunch of global variables. 
            // TODO: The RIGHT way to do this is that this 
            //       should return a format ID like this: 
            //       cbm1541_cdos_525_ssdd_0171k_d64
            //       and then some code afterward sets all the
            //       global settings. This is because the 
            //       format ID is a like to the main_formats
            //       table that deliberatly holds all this
            //       information. So we should be able to pull
            //       all this stuff and set it once and also
            //       eliminate a bunch of code. In fact this 
            //       refactoring will make it both more 
            //       effienct, smaller code, and make it 
            //       easier to understand for the programmer. 
            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> Called at line: %d \n", __LINE__);
            if ( setup_format_from_formatcode( get_format_from_ext_and_size(TRUE, file_size, nodot) ) == TRUE ) { 

                // TODO: This is a GLOBALLY USED VARIBLE and shouldn't just be here. 
                //       I need to like seriously deal. This should be part of some thing
                //       that always runs whenever the current file changes. 
                //       If I ever make this so that you can multiple tasks 
                //       open at the same time, then I need to make all these things as
                //       part of that instantiation in some unified way. 
                currently_selected_file_size = get_file_size(path);
                printf("Value of currently_selected_file_size: %li \n", currently_selected_file_size);

                /* Now set a global var for the file extension. */
                strcpy(currently_selected_file_ext,nodot);

                // Refresh the Hardware window by closing it then opening it again. 
                nk_window_close(ctx, "Hardware");                                      
                if (hardware_open == TRUE) nk_window_show(ctx, "Hardware", NK_SHOWN);

            };/*end-if*/

        };/*end-if*/

    };/*end-if*/

    /***************************************************/
    // End of Process Arguments 
    /***************************************************/

    /***************************************************/
    // Primary Loop 
    /***************************************************/

    while (running) {

        /***************************************************/
        // Current File Changed Check
        /***************************************************/

        if ( (strcmp(previously_selected_file,recent_files[image_selected-2]) != 0) ){

            printf("strcmp(previously_selected_file,recent_files[image_selected-2]) != 0\n");          
            has_currently_selected_file_changed = TRUE;
        
        };/*end-if*/

        if ( previous_image_selected != image_selected ){ // if != 0 <-- strings are DIFFERENT!

            printf("previous_image_selected != image_selected\n");           
            has_currently_selected_file_changed = TRUE;
        
        };/*end-if*/

        /***************************************************/
        // Input
        /***************************************************/

        SDL_Event evt;
        nk_input_begin(ctx);

        while (SDL_PollEvent(&evt)) {
        
            switch (evt.type) {

                /*********************************************/

                case SDL_QUIT: 
                    goto cleanup; 
                break;/*end-case*/

                /*********************************************/
                
                case SDL_KEYDOWN:

                    /*******************/
                    /* Keys Processing */
                    /*******************/

                    // Key pressed: Escape (scancode: 41, keycode: 27) // Key pressed: Q (scancode: 20, keycode: 113)
                    if ( (evt.key.keysym.scancode == 41) || (evt.key.keysym.scancode == 20) ) {

                        int result = MessageBox(NULL,
                            "Are you sure you wish to end the program?\n"
                            // "Yes --> Quits\n"
                            // "No --> Continues"
                            ,
                            "Quit",
                            MB_YESNOCANCEL | MB_ICONINFORMATION 
                        );/*end-MessageBox*/

                        if (result == IDYES) {
                            running = 0;
                        
                        } else if (result == IDNO) {
                            /* Do nothing. */
                            
                        } else if (result == IDCANCEL) {
                            /* Do nothing. */
                        
                        };/*end-if*/

                    };/*end-if*/ 

                    // Key pressed: /? (scancode: 56, keycode: 47)
                    if (evt.key.keysym.scancode == 56) {
                        toggle(is_tooltip_text_enabled);
                        user_requested_tooltip_state = is_tooltip_text_enabled; // Sync these so we can disabled them in the About  Box.
                    };/*end-if*/

                    // Key pressed: P (scancode: 19, keycode: 112)
                    if (evt.key.keysym.scancode == 19) {
                        toggle(is_tooltip_path_enabled); // This shows or hides the full path of the currently select file from being displayed in the tooltip.
                    };/*end-if*/

                    // Key pressed: O (scancode: 18, keycode: 111)
                    if (evt.key.keysym.scancode == 18 ) {
                        if (direction_selected == WRITING_A_FLOPPY) {
                            open_disk_image();
                        } else {
                            direction_selected = WRITING_A_FLOPPY; 
                            // open_disk_image();
                        };/*end-if*/
                    };/*end-if*/

                    // Key pressed: S (scancode: 22, keycode: 115)
                    if (evt.key.keysym.scancode == 22) {
                        if (direction_selected == READING_A_FLOPPY) {
                            open_disk_image();
                        } else {
                            direction_selected = READING_A_FLOPPY; 
                            // open_disk_image();
                        };/*end-if*/
                    };/*end-if*/

                    // Key pressed: F (scancode: 9, keycode: 102)     ( OLD --> Key pressed: B (scancode: 5, keycode: 98) )
                    if (evt.key.keysym.scancode == 9) {
                        
                        previous_brand_selected = brand_selected;

                        if ( brand_selected == (BRAND_COUNT-2) ) brand_selected = 0; // The last brand is FLUX but I'm not using that right now. 
                        else brand_selected++; 
                        /*end-if*/

                        // Fuck it. Just force this on any drag and drop of a file.  
                        has_currently_selected_file_changed = TRUE;

                        set_to_default_system(brand_selected);

                    }/*end-if*/

                    // Key pressed: C (scancode: 6, keycode: 99)
                    if (evt.key.keysym.scancode == 6) {
                        if ( computer_selected == (current_computer_count-1) ) computer_selected = 0; // The last brand is FLUX but I'm not using that right now. 
                        else computer_selected++; 
                        /*end-if*/

                        switch (brand_selected) {
                            case BRAND_AMIGA      : drive_selected = default_drive_amiga[      computer_selected ]; break;
                            case BRAND_APPLEII    : drive_selected = default_drive_appleii[    computer_selected ]; break;
                            case BRAND_ARCHIMEDES : drive_selected = default_drive_archimedes[ computer_selected ]; break;
                            case BRAND_ATARI8BIT  : drive_selected = default_drive_atari8[     computer_selected ]; break;
                            case BRAND_ATARIST    : drive_selected = default_drive_atarist[    computer_selected ]; break;
                            case BRAND_BBCMICRO   : drive_selected = default_drive_bbcmicro[   computer_selected ]; break;
                            case BRAND_COMMODORE  : drive_selected = default_drive_commodore[  computer_selected ]; break;
                            case BRAND_MACINTOSH  : drive_selected = default_drive_macintosh[  computer_selected ]; break;
                            case BRAND_PCCOMPAT   : drive_selected = default_drive_pccompat[   computer_selected ]; break;
                            case BRAND_TANDYCOCO  : drive_selected = default_drive_tandycoco[  computer_selected ]; break;
                            case BRAND_DRAGON     : drive_selected = default_drive_dragon[     computer_selected ]; break;
                            case BRAND_FLUX       : drive_selected = default_drive_flux[       computer_selected ]; break;
                        }/*end-switch*/

                    }/*end-if*/

                    // Key pressed: D (scancode: 7, keycode: 100) 
                    if (evt.key.keysym.scancode == 7) {
                        if ( drive_selected == (current_drive_count-1) ) drive_selected = 0; // The last brand is FLUX but I'm not using that right now. 
                        else drive_selected++; 
                        /*end-if*/
                    }/*end-if*/ 

                    // Key pressed: Z (scancode: 29, keycode: 122)
                    if (evt.key.keysym.scancode == 29) {
                        // greaseweazle_tree_state = NK_MINIMIZED; // Greasweazle 
                        // zoomfloppy_tree_state   = NK_MAXIMIZED; // ZoomFloppy
                        // usbfloppy_tree_state    = NK_MINIMIZED; // USBFloppy
                        if ( current_opencbm_drive_selected == (OPENCBM_DRIVE_COUNT-1) ) current_opencbm_drive_selected = 0; // The last brand is FLUX but I'm not using that right now. 
                        else current_opencbm_drive_selected++; 
                        /*end-if*/
                    }/*end-if*/ 

                    // // Key pressed: F (scancode: 9, keycode: 102)
                    // if (evt.key.keysym.scancode == 9) { // condition ? value_if_true : value_if_false // (direction_selected==WRITING_A_FLOPPY)?"Open":"Save"
                    //     if (direction_selected==WRITING_A_FLOPPY) direction_selected = READING_A_FLOPPY; else direction_selected = WRITING_A_FLOPPY; // WTF didn't this work: // (direction_selected==WRITING_A_FLOPPY) ? direction_selected=READING_A_FLOPPY : direction_selected=WRITING_A_FLOPPY;
                    // }/*end-if*/

                    // Key pressed: W (scancode: 26, keycode: 119)
                    if (evt.key.keysym.scancode == 26) { // condition ? value_if_true : value_if_false // (direction_selected==WRITING_A_FLOPPY)?"Open":"Save"
                        direction_selected = WRITING_A_FLOPPY;
                    }/*end-if*/

                    // Key pressed: R (scancode: 21, keycode: 114)
                    if (evt.key.keysym.scancode == 21) { // condition ? value_if_true : value_if_false // (direction_selected==WRITING_A_FLOPPY)?"Open":"Save"
                        direction_selected = READING_A_FLOPPY; 
                    }/*end-if*/

                    // Key pressed: H (scancode: 11, keycode: 104)
                    if (evt.key.keysym.scancode == 11) {

                        previous_hardware_selected = hardware_selected; // We need this to figure out when the state changes.
                        
                        if ( hardware_selected == (current_hardware_count-1) ) hardware_selected = GREASEWEAZLE_CHOSEN; // int current_hardware_count = ARRAY_LEN(hardware_options);
                        else hardware_selected++; // static const char *hardware_options[] = {"Greaseweazle", "ZoomFloppy", "USB Floppy"};
                        /*end-if*/

                        if (previous_hardware_selected != hardware_selected) {

                            nk_window_close(ctx, "Hardware");
                            if (hardware_open == TRUE) nk_window_show(ctx, "Hardware", NK_SHOWN);

                            if (hardware_selected == GREASEWEAZLE_CHOSEN) {           // Greasweazle 
            
                                greaseweazle_tree_state = NK_MAXIMIZED;               // Greasweazle - Don't need to set a default for the computer and drive because this supports so many formats. 
                                zoomfloppy_tree_state   = NK_MINIMIZED;               // ZoomFloppy
                                usbfloppy_tree_state    = NK_MINIMIZED;               // USBFloppy
            
                            } else if (hardware_selected == ZOOMFLOPPY_CHOSEN) {      // ZoomFloppy
            
                                greaseweazle_tree_state = NK_MINIMIZED;               // Greasweazle 
                                zoomfloppy_tree_state   = NK_MAXIMIZED;               // ZoomFloppy
                                usbfloppy_tree_state    = NK_MINIMIZED;               // USBFloppy
            
                                if ( brand_selected != BRAND_COMMODORE ) {    
                                    brand_selected      = BRAND_COMMODORE;            // Set the current brand to Commodore since that's all that ZoomFloppy works with!
                                    computer_selected   = CBM_64C;                    // Set current computer to Commodore 64C
                                    drive_selected      = CBM_DRIVE_1541II;           // Set current drive to 1541-II
                                };/*end-if*/    
            
                            } else if (hardware_selected == USBFLOPPY_CHOSEN) {       // USBFloppy
            
                                greaseweazle_tree_state = NK_MINIMIZED;               // Greasweazle
                                zoomfloppy_tree_state   = NK_MINIMIZED;               // ZoomFloppy
                                usbfloppy_tree_state    = NK_MAXIMIZED;               // USBFloppy

                                if ( brand_selected != BRAND_PCCOMPAT ) {
                                    brand_selected      = BRAND_PCCOMPAT;         // Set the current brand to Commodore since that's all that ZoomFloppy works with!
                                    computer_selected   = PC_80386;               // Set current computer to Commodore 64C
                                    drive_selected      = PC_DRIVE_35_DD_720_IMA; // Set current drive to DD 720k
                                };/*end-if*/

                            };/*end-if*/

                        } else {

                            // printf("Nothing has changed.\n");

                        };/*end-if*/

                    }/*end-if*/

                    /**********************/

                    // Note these a single key events. Not a key-down and then key-up events.
                    printf("Key pressed: %s (scancode: %d, keycode: %d)\n",  
                        SDL_GetKeyName(evt.key.keysym.sym),
                        evt.key.keysym.scancode,
                        evt.key.keysym.sym);
                    /*end-printf*/

                break;/*end-case*/ 

                /*********************************************/

                case SDL_DROPFILE: {
                    char *dropped_path = evt.drop.file;
                    printf("File dropped: %s\n", dropped_path);
                    
                    /* Process Drag and Drop */
                    on_file_dropped(ctx, dropped_path);

                    // If the user does a drag and drop they probably want to make a floppy.
                    direction_selected      = WRITING_A_FLOPPY;
                    direction_selected_prev = direction_selected; // We don't want to lose the image they have dropped! This will prevent this other code that basically does that when the user changes the direction. 
                    
                    SDL_free(dropped_path);  /* IMPORTANT -- SDL allocates this, you must free it */

                    /* Fix for loosing a click when the Window isn't focused. */
                    SetForegroundWindow(hwnd);
                    SetFocus(hwnd);
                    SDL_RaiseWindow(win);

                break;}/*end-case*/

                /*********************************************/

                case SDL_MOUSEBUTTONDOWN: {
                    const char *btn_name;
                    switch (evt.button.button) {
                        case SDL_BUTTON_LEFT:   btn_name = "LEFT";   break;
                        case SDL_BUTTON_RIGHT:  btn_name = "RIGHT";  break;
                        case SDL_BUTTON_MIDDLE: btn_name = "MIDDLE"; break;
                        case SDL_BUTTON_X1:     btn_name = "X1";     break;
                        case SDL_BUTTON_X2:     btn_name = "X2";     break;
                        default:                btn_name = "UNKNOWN"; break;
                    }
                    printf("Mouse: Button: %d, x: %d, y: %d - DOWN: %s \n",
                        evt.button.button,
                        evt.button.x,
                        evt.button.y,
                        btn_name);
                break;}

                case SDL_MOUSEBUTTONUP: {
                    const char *btn_name;
                    switch (evt.button.button) {
                        case SDL_BUTTON_LEFT:   btn_name = "LEFT";   break;
                        case SDL_BUTTON_RIGHT:  btn_name = "RIGHT";  break;
                        case SDL_BUTTON_MIDDLE: btn_name = "MIDDLE"; break;
                        case SDL_BUTTON_X1:     btn_name = "X1";     break;
                        case SDL_BUTTON_X2:     btn_name = "X2";     break;
                        default:                btn_name = "UNKNOWN"; break;
                    }
                    printf("Mouse: Button: %d, x: %d, y: %d - UP:   %s \n",
                        evt.button.button,
                        evt.button.x,
                        evt.button.y,
                        btn_name);
                break;}

                /*********************************************/

            };/*ends-switch*/

            nk_sdl_handle_event(&evt);

        };/*end-while*/
        
        nk_sdl_handle_grab(); /* optional grabbing behavior */
        nk_input_end(ctx);

        /*********************************************/
        // GUI
        /*********************************************/

        /*********************************************/
        // Windows - Top Menu
        /*********************************************/

        if (nk_begin(ctx, "Menu", nk_rect(5, 5, 438, 80),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {

            nk_layout_row_begin(ctx, NK_STATIC, 30, 10);
            nk_layout_row_push(ctx, 67); // Old value 82

            /***** OPEN / SAVE BUTTON *****/ 
            struct nk_style_button start_style = ctx->style.button;
            /************************************************************************************************************************************/
            /******************************************************************/ /* Below are the NORMAL, or mouse off, colors.                 */
            start_style.normal      = nk_style_item_color(UI_BACK_NORMAL_COLOR); /* The background button or UI element color. The body color.  */
            start_style.text_normal =                     UI_TEXT_NORMAL_COLOR;  /* The button or UI element's text color.                      */
            /******************************************************************/ /***************************************************************/
            /******************************************************************/ /* Below are the HOVER, or mouse over, colors.                 */
            start_style.hover       = nk_style_item_color(UI_BACK_HOVER_COLOR);  /* The background button or UI element color. The body color.  */
            start_style.text_hover  =                     UI_TEXT_HOVER_COLOR;   /* The button or UI element's text color.                      */
            /******************************************************************/ /***************************************************************/
            /******************************************************************/ /* Below are the ACTIVE, or mouse button down, colors.         */
            start_style.active      = nk_style_item_color(UI_BACK_ACTIVE_COLOR); /* The background button or UI element color. The body color.  */  
            start_style.text_active =                     UI_TEXT_ACTIVE_COLOR;  /* The button or UI element's text color.                      */ 
            /************************************************************************************************************************************/
            if ( nk_button_label_styled( ctx , &start_style , (direction_selected==WRITING_A_FLOPPY)?"Open":"Save" ) ){ // --> The direction_selected determines if the button is for opening of saving a disk image file. 
                printf("Open button pressed!\n");
                open_disk_image();
            };/*end-nk_style_button*/

            if (nk_button_label(ctx, "Task")){
                printf("Task button pressed!\n");
                task_open = !task_open; // Toggle the window state so it's open it's now closed and vice-versa.
            };/*end-nk_button_label*/

            if (nk_button_label(ctx, "Hardware")) {
                printf("Hardware button pressed!\n");
                hardware_open = !hardware_open; // Toggle the window state so it's open it's now closed and vice-versa.
            };/*end-nk_button_label*/

            if (nk_button_label(ctx, "Options")) {
                printf("Options button pressed!\n");
                options_open = !options_open; // Toggle the window state so it's open it's now closed and vice-versa.
            };/*end-nk_button_label*/
            
            if (nk_button_label(ctx, "Help")) {
                printf("Help button pressed!\n");
                // Opens a local help.txt file in whatever the default text file viewer is set to.  
                ShellExecuteA(NULL, "open", "help.txt", NULL, NULL, SW_SHOWNORMAL);
            };/*end-if*/

            if (nk_button_label(ctx, "About")) {
                printf("About button pressed!\n");
                // popup_active = 1;
                about_open = !about_open; // Toggle the window state so it's open it's now closed and vice-versa.
            };/*end-nk_button_label*/

            // if (nk_button_label(ctx, "Quit")) {
            //     printf("Quit button pressed!\n");
            //     running = 0;
            // };/*end-nk_button_label*/

            nk_layout_row_end(ctx);

        }/*end-if-menu*/

        nk_end(ctx);

        // This function is also used by the extension processing function. 
        // It sets the current computer and drive based on the current brand. 
        // This populates the drop down but also sets the current 
        // computer and drive do that other processes can use them. 
        set_current_options( brand_selected );

        if (computer_selected > (current_computer_count-1) ) {
            computer_selected = 0;
        };/*end-if*/

        if (drive_selected > (current_drive_count-1) ) {
            drive_selected = 0;
        };/*end-if*/

        /********************************************************************************/
        /********************************************************************************/
        /********************************** TASK WINDOW *********************************/
        /********************************************************************************/
        /********************************************************************************/

        if (task_open == TRUE) {

            if (nk_begin(ctx, "Task", nk_rect(5, 90, 438, 450), 
                NK_WINDOW_BORDER       | 
                NK_WINDOW_MOVABLE      | 
                NK_WINDOW_TITLE        | 
                NK_WINDOW_CLOSABLE     | 
                NK_WINDOW_MINIMIZABLE  | 
                NK_WINDOW_NO_SCROLLBAR 
                )){

                float panel_w = 200;
                float panel_h = 130;
                float small_w = 130;

                /*******************************************/
                /****** ROW 1: one dropdown for BRAND ******/
                /*******************************************/
                
                nk_layout_row_dynamic(ctx, 25, 1);

                // 4CC2FF - R 76 , G 194 , B 255 - Windows 11 Dark Mode Blue
                // this is just a darker shade of it: 3485AD 52 , 133 , 173 
                // lightr shafde #5ECAFF  94, 202, 255

                previous_brand_selected = brand_selected;

                struct nk_style_combo      saved_combo     = ctx->style.combo;
                struct nk_style_item       saved_window_bg = ctx->style.window.fixed_background;
                struct nk_color            saved_bg        = ctx->style.window.background;
                struct nk_style_button     saved_ctx_btn   = ctx->style.contextual_button;

                /************************************************************************************************************************************************/
                /******************************************************************************/ /* Below are the NORMAL, or mouse off, colors.                 */
                ctx->style.combo.normal             = nk_style_item_color(UI_BACK_NORMAL_COLOR); /* The background button or UI element color. The body color.  */
                ctx->style.combo.label_normal       =                     UI_TEXT_NORMAL_COLOR;  /* The button or UI element's text color.                      */
                ctx->style.combo.button.normal      = nk_style_item_color(UI_BACK_NORMAL_COLOR); /* The arrow background button on the right side of the combo. */
                ctx->style.combo.button.text_normal =                     UI_TEXT_NORMAL_COLOR;  /* The arrow itself. The text color if you will.               */
                /******************************************************************************/ /***************************************************************/
                /******************************************************************************/ /* Below are the HOVER, or mouse over, colors.                 */
                ctx->style.combo.hover              = nk_style_item_color(UI_BACK_HOVER_COLOR);  /* The background button or UI element color. The body color.  */
                ctx->style.combo.label_hover        =                     UI_TEXT_HOVER_COLOR;   /* The button or UI element's text color.                      */
                ctx->style.combo.button.hover       = nk_style_item_color(UI_BACK_HOVER_COLOR);  /* The arrow background button on the right side of the combo. */
                ctx->style.combo.button.text_hover  =                     UI_TEXT_HOVER_COLOR;   /* The arrow itself. The text color if you will.               */
                /******************************************************************************/ /***************************************************************/
                /******************************************************************************/ /* Below are the ACTIVE, or mouse button down, colors.         */
                ctx->style.combo.active             = nk_style_item_color(UI_BACK_ACTIVE_COLOR); /* The background button or UI element color. The body color.  */  
                ctx->style.combo.label_active       =                     UI_TEXT_ACTIVE_COLOR;  /* The button or UI element's text color.                      */ 
                ctx->style.combo.button.active      = nk_style_item_color(UI_BACK_ACTIVE_COLOR); /* The arrow background button on the right side of the combo. */
                ctx->style.combo.button.text_active =                     UI_TEXT_ACTIVE_COLOR;  /* The arrow itself. The text color if you will.               */
                /************************************************************************************************************************************************/
                /************************************************************************************************************************************************/
                ctx->style.window.background             = nk_rgb(                     25,  60,  80);  // THESE SET THE DROPDOWN BACKGROUND COLOR!
                ctx->style.window.fixed_background       = nk_style_item_color(nk_rgb( 25,  60,  80)); // THESE SET THE DROPDOWN BACKGROUND COLOR!
                /************************************************************************************************************************************************/
                ctx->style.contextual_button.normal      = nk_style_item_color(nk_rgb( 25,  60,  80)); // THESE SET THE DROPDOWN BACKGROUND COLOR!
                ctx->style.contextual_button.text_normal =                     nk_rgb(255, 255, 255);
                /************************************************************************************************************************************************/
                ctx->style.contextual_button.hover       = nk_style_item_color(nk_rgb( 76, 194, 255));
                ctx->style.contextual_button.text_hover  =                     nk_rgb(255, 255, 255);
                /************************************************************************************************************************************************/
                ctx->style.contextual_button.active      = nk_style_item_color(nk_rgb( 94, 202, 255));
                ctx->style.contextual_button.text_active =                     nk_rgb(255, 255, 255);
                /************************************************************************************************************************************************/
                int num_items       = 2 + (ARRAY_LEN(brand_options) - 1);
                int row_height      = 25;
                int dropdown_height = (num_items * row_height) + 5;
                int count           = ARRAY_LEN(brand_options) - 1;

                if (nk_combo_begin_label(ctx, brand_options[brand_selected], nk_vec2(nk_widget_width(ctx), dropdown_height))) {
                    nk_layout_row_dynamic(ctx, row_height, 1);
                    for (int i = 0; i < count; i++) {
                        if (nk_combo_item_label(ctx, brand_options[i], NK_TEXT_LEFT))
                            brand_selected = i;
                    }
                    nk_combo_end(ctx);
                }

                ctx->style.combo                   = saved_combo;
                ctx->style.window.fixed_background = saved_window_bg;
                ctx->style.window.background       = saved_bg;
                ctx->style.contextual_button       = saved_ctx_btn;

                /*******************************************/
                /************ DEFAULT COMPUTERS ************/
                /*******************************************/

                if (brand_selected != previous_brand_selected) {

                    set_to_default_system(brand_selected);

                    // Fuck it. Just force this on any drag and drop of a file.  
                    has_currently_selected_file_changed = TRUE;

                };/*end-if*/

                /*******************************************/

                /*******************************************/
                /********** ROW 2: Two Dropdowns ***********/
                /*******************************************/
                nk_layout_row_begin(ctx, NK_STATIC, 25, 3);

                    /********** Computer Dropdown ***********/
                    nk_layout_row_push(ctx, panel_w);
                    
                    // We need to do this like this because of the immediate mode nature of Nuklear. 

                    previous_computer_selected = computer_selected;
                    
                    /************************************************************************************************************************************************/
                    /******************************************************************************/ /* Below are the NORMAL, or mouse off, colors.                 */
                    ctx->style.combo.normal             = nk_style_item_color(UI_BACK_NORMAL_COLOR); /* The background button or UI element color. The body color.  */
                    ctx->style.combo.label_normal       =                     UI_TEXT_NORMAL_COLOR;  /* The button or UI element's text color.                      */
                    ctx->style.combo.button.normal      = nk_style_item_color(UI_BACK_NORMAL_COLOR); /* The arrow background button on the right side of the combo. */
                    ctx->style.combo.button.text_normal =                     UI_TEXT_NORMAL_COLOR;  /* The arrow itself. The text color if you will.               */
                    /******************************************************************************/ /***************************************************************/
                    /******************************************************************************/ /* Below are the HOVER, or mouse over, colors.                 */
                    ctx->style.combo.hover              = nk_style_item_color(UI_BACK_HOVER_COLOR);  /* The background button or UI element color. The body color.  */
                    ctx->style.combo.label_hover        =                     UI_TEXT_HOVER_COLOR;   /* The button or UI element's text color.                      */
                    ctx->style.combo.button.hover       = nk_style_item_color(UI_BACK_HOVER_COLOR);  /* The arrow background button on the right side of the combo. */
                    ctx->style.combo.button.text_hover  =                     UI_TEXT_HOVER_COLOR;   /* The arrow itself. The text color if you will.               */
                    /******************************************************************************/ /***************************************************************/
                    /******************************************************************************/ /* Below are the ACTIVE, or mouse button down, colors.         */
                    ctx->style.combo.active             = nk_style_item_color(UI_BACK_ACTIVE_COLOR); /* The background button or UI element color. The body color.  */  
                    ctx->style.combo.label_active       =                     UI_TEXT_ACTIVE_COLOR;  /* The button or UI element's text color.                      */ 
                    ctx->style.combo.button.active      = nk_style_item_color(UI_BACK_ACTIVE_COLOR); /* The arrow background button on the right side of the combo. */
                    ctx->style.combo.button.text_active =                     UI_TEXT_ACTIVE_COLOR;  /* The arrow itself. The text color if you will.               */
                    /************************************************************************************************************************************************/
                    nk_combobox(ctx, current_computer_options, current_computer_count, &computer_selected, 25, nk_vec2(nk_widget_width(ctx), 240)); 
                    ctx->style.combo = saved_combo;

                    if (computer_selected != previous_computer_selected) {

                        switch (brand_selected) {
                            case BRAND_AMIGA      : drive_selected = default_drive_amiga[      computer_selected ]; break;
                            case BRAND_APPLEII    : drive_selected = default_drive_appleii[    computer_selected ]; break;
                            case BRAND_ARCHIMEDES : drive_selected = default_drive_archimedes[ computer_selected ]; break;
                            case BRAND_ATARI8BIT  : drive_selected = default_drive_atari8[     computer_selected ]; break;
                            case BRAND_ATARIST    : drive_selected = default_drive_atarist[    computer_selected ]; break;
                            case BRAND_BBCMICRO   : drive_selected = default_drive_bbcmicro[   computer_selected ]; break;
                            case BRAND_COMMODORE  : drive_selected = default_drive_commodore[  computer_selected ]; break;
                            case BRAND_MACINTOSH  : drive_selected = default_drive_macintosh[  computer_selected ]; break;
                            case BRAND_PCCOMPAT   : drive_selected = default_drive_pccompat[   computer_selected ]; break;
                            case BRAND_TANDYCOCO  : drive_selected = default_drive_tandycoco[  computer_selected ]; break;
                            case BRAND_DRAGON     : drive_selected = default_drive_dragon[     computer_selected ]; break;
                            case BRAND_FLUX       : drive_selected = default_drive_flux[       computer_selected ]; break;
                        };/*end-switch*/

                        // Fuck it. Just force this on any drag and drop of a file.
                        has_currently_selected_file_changed = TRUE;

                    };/*end-if*/

                    /********** Drive Dropdown ***********/
                    nk_layout_row_push(ctx, 18);  // <-- spacer, 10px wide
                    nk_spacing(ctx, 1);

                    nk_layout_row_push(ctx, panel_w);

                    previous_drive_selected = drive_selected;

                    /************************************************************************************************************************************************/
                    /******************************************************************************/ /* Below are the NORMAL, or mouse off, colors.                 */
                    ctx->style.combo.normal             = nk_style_item_color(UI_BACK_NORMAL_COLOR); /* The background button or UI element color. The body color.  */
                    ctx->style.combo.label_normal       =                     UI_TEXT_NORMAL_COLOR;  /* The button or UI element's text color.                      */
                    ctx->style.combo.button.normal      = nk_style_item_color(UI_BACK_NORMAL_COLOR); /* The arrow background button on the right side of the combo. */
                    ctx->style.combo.button.text_normal =                     UI_TEXT_NORMAL_COLOR;  /* The arrow itself. The text color if you will.               */
                    /******************************************************************************/ /***************************************************************/
                    /******************************************************************************/ /* Below are the HOVER, or mouse over, colors.                 */
                    ctx->style.combo.hover              = nk_style_item_color(UI_BACK_HOVER_COLOR);  /* The background button or UI element color. The body color.  */
                    ctx->style.combo.label_hover        =                     UI_TEXT_HOVER_COLOR;   /* The button or UI element's text color.                      */
                    ctx->style.combo.button.hover       = nk_style_item_color(UI_BACK_HOVER_COLOR);  /* The arrow background button on the right side of the combo. */
                    ctx->style.combo.button.text_hover  =                     UI_TEXT_HOVER_COLOR;   /* The arrow itself. The text color if you will.               */
                    /******************************************************************************/ /***************************************************************/
                    /******************************************************************************/ /* Below are the ACTIVE, or mouse button down, colors.         */
                    ctx->style.combo.active             = nk_style_item_color(UI_BACK_ACTIVE_COLOR); /* The background button or UI element color. The body color.  */  
                    ctx->style.combo.label_active       =                     UI_TEXT_ACTIVE_COLOR;  /* The button or UI element's text color.                      */ 
                    ctx->style.combo.button.active      = nk_style_item_color(UI_BACK_ACTIVE_COLOR); /* The arrow background button on the right side of the combo. */
                    ctx->style.combo.button.text_active =                     UI_TEXT_ACTIVE_COLOR;  /* The arrow itself. The text color if you will.               */
                    /************************************************************************************************************************************************/ 
                    nk_combobox(ctx, current_drive_options, current_drive_count, &drive_selected, 25, nk_vec2(nk_widget_width(ctx), 240)); ctx->style.combo = saved_combo;

                    if (drive_selected != previous_drive_selected) {

                        // Fuck it. Just force this on any drag and drop of a file.
                        has_currently_selected_file_changed = TRUE;                    
                    
                    };/*end-if*/

                    nk_layout_row_end(ctx);

                /*******************************************/

                /*******************************************/
                /********* ROW 3: two image panels *********/
                /*******************************************/
                nk_layout_row_begin(ctx, NK_STATIC, panel_h, 3);

                    nk_layout_row_push(ctx, panel_w);

                    /*******************************************/
                    /* computer images */
                    /*******************************************/

                    // Note: These have their own TOOLTIPS for these are in the load_all_images.h
                    update_system_images();

                    nk_layout_row_push(ctx, 18);  // <-- spacer, 10px wide
                    nk_spacing(ctx, 1);

                    nk_layout_row_push(ctx, panel_w);

                    /*******************************************/
                    /* drive image */
                    /*******************************************/

                    // TODO: I think this is refreshing teh images on EVERY frame instead of only when teh user changes it! 
                    //       Need to make this run ONLY when the user changes it!

                    struct nk_rect image_bounds_driveimage = nk_widget_bounds(ctx); // grab bounds BEFORE nk_image

                    update_drive_images(); // This is in load_all_images.h 

                    char drive_format_info_string[256]; // Needed to display the drive info string below.

                    if (nk_input_is_mouse_hovering_rect(&ctx->input, image_bounds_driveimage) ) {
                        
                        // ' '  - Single space so the mouse cursor doesn't cover the text
                        // 5.25 - drive name OR drive size in inches 
                        // "    - quote for inches
                        // DS   - sides
                        // /    - divider
                        // DD   - density
                        // 171K - size in kbytes or mbytes
                        // CDOS - disk operating system format or whatever
                        // D64  - file 

                        sprintf(drive_format_info_string, 
                            /*3.5" DS/DD 10KB OS (ST) 96TPI 300  GCR 35T  2H 19S 99BL - format id string*/
                            " %s\" %s/%s %iKB %s (%s) %iTPI %iRPM %s %iT %iH %iS %iBL - ID:%s ", 

                            floppy_size_strings[  main_formats[ drive_map[brand_selected][drive_selected] ].size_inches ],
                            floppy_sided_strings[ main_formats[ drive_map[brand_selected][drive_selected] ].sides       ],
                            density_type_strings[ main_formats[ drive_map[brand_selected][drive_selected] ].density     ],
                            main_formats[ drive_map[brand_selected][drive_selected] ].size_kb,
                            main_formats[ drive_map[brand_selected][drive_selected] ].filesystem_format,
                            main_formats[ drive_map[brand_selected][drive_selected] ].file_ext,
                            
                            main_formats[ drive_map[brand_selected][drive_selected] ].tpi,
                            main_formats[ drive_map[brand_selected][drive_selected] ].rpm,
                            encoding_type_strings[ main_formats[ drive_map[brand_selected][drive_selected] ].encoding ],
                            main_formats[ drive_map[brand_selected][drive_selected] ].tracks,
                            main_formats[ drive_map[brand_selected][drive_selected] ].heads,
                            main_formats[ drive_map[brand_selected][drive_selected] ].sectors,
                            main_formats[ drive_map[brand_selected][drive_selected] ].block_size,

                            main_formats[ drive_map[brand_selected][drive_selected] ].format_id_string
                         );

                        if (is_tooltip_text_enabled) nk_tooltip(ctx, drive_format_info_string);

                    };/*end-if*/

                nk_layout_row_end(ctx);

                /*******************************************/

                /*******************************************/
                /********** ROW 4: three dropdowns *********/
                /*******************************************/

                nk_layout_row_begin(ctx, NK_STATIC, 25, 5);

                    /********** Disk Image Selected Options Combobox / Dropdown *********/
                    nk_layout_row_push(ctx, small_w);

                    int is_floppy_to_file = (direction_selected == READING_A_FLOPPY); // TODO: WTF is this? Update this to be non-Forrest-Gump code. 

                    const char *image_options_short[5]; // Number of items in file drop down / dropdown file combo box / combobox. 
                    int image_option_count = 0;

                    if (is_floppy_to_file) {
                        // save-as mode: just two options, no recents <-- WAT???
                        image_options_short[image_option_count++] = "No save file set";
                        image_options_short[image_option_count++] = "Browse...";
                        for (int i = 0; i < recent_count; i++)
                            image_options_short[image_option_count++] = get_filename(recent_files[i]);
                    } else {
                        // open mode
                        image_options_short[image_option_count++] = "No file set";
                        image_options_short[image_option_count++] = "Browse...";
                        for (int i = 0; i < recent_count; i++)
                            image_options_short[image_option_count++] = get_filename(recent_files[i]);
                    }

                    const char *preview_label = image_options_short[image_selected];

                    /************************************************************************************************************************************************/
                    /******************************************************************************/ /* Below are the NORMAL, or mouse off, colors.                 */
                    ctx->style.combo.normal             = nk_style_item_color(UI_BACK_NORMAL_COLOR); /* The background button or UI element color. The body color.  */
                    ctx->style.combo.label_normal       =                     UI_TEXT_NORMAL_COLOR;  /* The button or UI element's text color.                      */
                    ctx->style.combo.button.normal      = nk_style_item_color(UI_BACK_NORMAL_COLOR); /* The arrow background button on the right side of the combo. */
                    ctx->style.combo.button.text_normal =                     UI_TEXT_NORMAL_COLOR;  /* The arrow itself. The text color if you will.               */
                    /******************************************************************************/ /***************************************************************/
                    /******************************************************************************/ /* Below are the HOVER, or mouse over, colors.                 */
                    ctx->style.combo.hover              = nk_style_item_color(UI_BACK_HOVER_COLOR);  /* The background button or UI element color. The body color.  */
                    ctx->style.combo.label_hover        =                     UI_TEXT_HOVER_COLOR;   /* The button or UI element's text color.                      */
                    ctx->style.combo.button.hover       = nk_style_item_color(UI_BACK_HOVER_COLOR);  /* The arrow background button on the right side of the combo. */
                    ctx->style.combo.button.text_hover  =                     UI_TEXT_HOVER_COLOR;   /* The arrow itself. The text color if you will.               */
                    /******************************************************************************/ /***************************************************************/
                    /******************************************************************************/ /* Below are the ACTIVE, or mouse button down, colors.         */
                    ctx->style.combo.active             = nk_style_item_color(UI_BACK_ACTIVE_COLOR); /* The background button or UI element color. The body color.  */  
                    ctx->style.combo.label_active       =                     UI_TEXT_ACTIVE_COLOR;  /* The button or UI element's text color.                      */ 
                    ctx->style.combo.button.active      = nk_style_item_color(UI_BACK_ACTIVE_COLOR); /* The arrow background button on the right side of the combo. */
                    ctx->style.combo.button.text_active =                     UI_TEXT_ACTIVE_COLOR;  /* The arrow itself. The text color if you will.               */
                    /************************************************************************************************************************************************/
                    if (nk_combo_begin_label(ctx, preview_label, nk_vec2(small_w, 150))) {
                        nk_layout_row_dynamic(ctx, 25, 1);
                        for (int i = 0; i < image_option_count; i++) {
                            if (i == 0 && image_selected >= 2) continue; // hide placeholder once a file is chosen
                            if (nk_combo_item_label(ctx, image_options_short[i], NK_TEXT_LEFT)) {
                                previous_image_selected = image_selected;
                                image_selected = i;
                                // Fuck it. Just force this on any drag and drop of a file.  
                                has_currently_selected_file_changed = TRUE;
                            }
                        }
                        nk_combo_end(ctx);
                    };/*end-if*/
                    ctx->style.combo = saved_combo;

                    if (image_selected == 1) { // Browse...
                        int dialog_ok = is_floppy_to_file ? save_as_file_dialog() : open_file_dialog();
                        if (dialog_ok) {
                            int already_exists = 0;
                            for (int i = 0; i < recent_count; i++) {
                                if (strcmp(recent_files[i], selected_image_path) == 0) {
                                    already_exists = 1;
                                    previous_image_selected = image_selected;
                                    image_selected = 2 + i;

                                    // Fuck it. Just force this on any drag and drop of a file.  
                                    has_currently_selected_file_changed = TRUE;

                                    break;
                                }
                            }
                            if (!already_exists) {
                                if (recent_count < 3) recent_count++;
                                for (int i = recent_count - 1; i > 0; i--) strncpy(recent_files[i], recent_files[i-1], MAX_PATH - 1);
                                strncpy(recent_files[0], selected_image_path, MAX_PATH - 1);
                                previous_image_selected = image_selected;
                                image_selected = 2;

                                // Fuck it. Just force this on any drag and drop of a file.  
                                has_currently_selected_file_changed = TRUE;

                            }
                        } else {
                            previous_image_selected = image_selected;
                            image_selected = 0;

                            // Fuck it. Just force this on any drag and drop of a file.  
                            has_currently_selected_file_changed = TRUE;

                        }

                        // If in fact a file is set then figure out it's file size once and not on every frame. 
                        // So if(image_selected<2) == TRUE then there is NO file selected.
                        // Therefore the inverse of that is that there is NOT NO FILE SET meaning THERE IS A FILE SET!
                        // But don't try that if we are trying to save a floppy to a file. Only for writing a floppy!
                        if ( !(image_selected<2) && (direction_selected == WRITING_A_FLOPPY) ) {

                            // Rip out the file extension with the dot.
                            const char *dot = strrchr(recent_files[image_selected-2],'.');

                            // Pull out the dot so I don't have to do dot+1 all over the joint. 
                            const char *nodot = dot+1;

                            // Now set the file size for the global varible. 
                            currently_selected_file_size = get_file_size(recent_files[image_selected-2]);

                            // Now set a global var for the file extension.
                            strcpy(currently_selected_file_ext,nodot); // TODO: This CRASHES the whole program if the user is trying to save and enters a filename with no extension. This is because we are doing string shit to empty strings. That's bad. 

                        };/*end-if*/

                        printf("Figuring out disk image file size... currently_selected_file_size:%li\n",currently_selected_file_size);

                    };/*end-if*/

                    /* If image_selected is NOT the same as the previous one AND the currently selected filename is NOT empty AND the user hasn't selected a non-file entry which is image_selected=0 or image_selected=1 then... */
                    if ( previous_image_selected != image_selected && recent_files[image_selected-2][0] != '\0' && !(image_selected<2) ) {     
                        // Path of the currently selected image. 
                        const char *path = recent_files[image_selected-2];

                        // Then do your format detection here
                        const char *dot = strrchr(path, '.');

                        // So I don't have to do dot+1 all over the joint. 
                        const char *nodot = dot+1;

                        currently_selected_file_size = get_file_size(path);
                        printf("Value of currently_selected_file_size: %li \n", currently_selected_file_size);

                        // Now set a global var for the file extension.
                        strcpy(currently_selected_file_ext,nodot);
                    };/*end-if*/

                    /*end-nk_layout_row_push*/

                    /********** SPACER *********/
                    nk_layout_row_push(ctx, 10);
                    nk_spacing(ctx, 1);

                    /********** WRITE / READ DIRECTION DROPDOWN COMBOBOX *********/
                    nk_layout_row_push(ctx, small_w);

                    /************************************************************************************************************************************************/
                    /******************************************************************************/ /* Below are the NORMAL, or mouse off, colors.                 */
                    ctx->style.combo.normal             = nk_style_item_color(UI_BACK_NORMAL_COLOR); /* The background button or UI element color. The body color.  */
                    ctx->style.combo.label_normal       =                     UI_TEXT_NORMAL_COLOR;  /* The button or UI element's text color.                      */
                    ctx->style.combo.button.normal      = nk_style_item_color(UI_BACK_NORMAL_COLOR); /* The arrow background button on the right side of the combo. */
                    ctx->style.combo.button.text_normal =                     UI_TEXT_NORMAL_COLOR;  /* The arrow itself. The text color if you will.               */
                    /******************************************************************************/ /***************************************************************/
                    /******************************************************************************/ /* Below are the HOVER, or mouse over, colors.                 */
                    ctx->style.combo.hover              = nk_style_item_color(UI_BACK_HOVER_COLOR);  /* The background button or UI element color. The body color.  */
                    ctx->style.combo.label_hover        =                     UI_TEXT_HOVER_COLOR;   /* The button or UI element's text color.                      */
                    ctx->style.combo.button.hover       = nk_style_item_color(UI_BACK_HOVER_COLOR);  /* The arrow background button on the right side of the combo. */
                    ctx->style.combo.button.text_hover  =                     UI_TEXT_HOVER_COLOR;   /* The arrow itself. The text color if you will.               */
                    /******************************************************************************/ /***************************************************************/
                    /******************************************************************************/ /* Below are the ACTIVE, or mouse button down, colors.         */
                    ctx->style.combo.active             = nk_style_item_color(UI_BACK_ACTIVE_COLOR); /* The background button or UI element color. The body color.  */  
                    ctx->style.combo.label_active       =                     UI_TEXT_ACTIVE_COLOR;  /* The button or UI element's text color.                      */ 
                    ctx->style.combo.button.active      = nk_style_item_color(UI_BACK_ACTIVE_COLOR); /* The arrow background button on the right side of the combo. */
                    ctx->style.combo.button.text_active =                     UI_TEXT_ACTIVE_COLOR;  /* The arrow itself. The text color if you will.               */
                    /************************************************************************************************************************************************/
                    nk_combobox(ctx, direction_options, 2, &direction_selected, 25, nk_vec2(small_w, 60)); ctx->style.combo = saved_combo;

                    if (direction_selected != direction_selected_prev) {

                        /* runs once when switching to Floppy to File or File to Floppy */
                        previous_image_selected = image_selected;
                        image_selected = 0;

                        // Fuck it. Just force this on any drag and drop of a file.  
                        has_currently_selected_file_changed = TRUE;

                        direction_selected_prev = direction_selected;
                    }

                    /********** SPACER *********/
                    nk_layout_row_push(ctx, 10);
                    nk_spacing(ctx, 1);

                    /********** HARDWARE SELECTED DROPDOWN COMBOBOX *********/
                    nk_layout_row_push(ctx, small_w);

                    previous_hardware_selected = hardware_selected; // We need this to figure out when the state changes. 

                    /************************************************************************************************************************************************/
                    /******************************************************************************/ /* Below are the NORMAL, or mouse off, colors.                 */
                    ctx->style.combo.normal             = nk_style_item_color(UI_BACK_NORMAL_COLOR); /* The background button or UI element color. The body color.  */
                    ctx->style.combo.label_normal       =                     UI_TEXT_NORMAL_COLOR;  /* The button or UI element's text color.                      */
                    ctx->style.combo.button.normal      = nk_style_item_color(UI_BACK_NORMAL_COLOR); /* The arrow background button on the right side of the combo. */
                    ctx->style.combo.button.text_normal =                     UI_TEXT_NORMAL_COLOR;  /* The arrow itself. The text color if you will.               */
                    /******************************************************************************/ /***************************************************************/
                    /******************************************************************************/ /* Below are the HOVER, or mouse over, colors.                 */
                    ctx->style.combo.hover              = nk_style_item_color(UI_BACK_HOVER_COLOR);  /* The background button or UI element color. The body color.  */
                    ctx->style.combo.label_hover        =                     UI_TEXT_HOVER_COLOR;   /* The button or UI element's text color.                      */
                    ctx->style.combo.button.hover       = nk_style_item_color(UI_BACK_HOVER_COLOR);  /* The arrow background button on the right side of the combo. */
                    ctx->style.combo.button.text_hover  =                     UI_TEXT_HOVER_COLOR;   /* The arrow itself. The text color if you will.               */
                    /******************************************************************************/ /***************************************************************/
                    /******************************************************************************/ /* Below are the ACTIVE, or mouse button down, colors.         */
                    ctx->style.combo.active             = nk_style_item_color(UI_BACK_ACTIVE_COLOR); /* The background button or UI element color. The body color.  */  
                    ctx->style.combo.label_active       =                     UI_TEXT_ACTIVE_COLOR;  /* The button or UI element's text color.                      */ 
                    ctx->style.combo.button.active      = nk_style_item_color(UI_BACK_ACTIVE_COLOR); /* The arrow background button on the right side of the combo. */
                    ctx->style.combo.button.text_active =                     UI_TEXT_ACTIVE_COLOR;  /* The arrow itself. The text color if you will.               */
                    /************************************************************************************************************************************************/             
                    nk_combobox(ctx, hardware_options, 3, &hardware_selected, 25, nk_vec2(small_w, 90)); ctx->style.combo = saved_combo;

                    if (previous_hardware_selected != hardware_selected) {

                        nk_window_close(ctx, "Hardware");
                        if (hardware_open == TRUE) nk_window_show(ctx, "Hardware", NK_SHOWN);

                        if (hardware_selected == GREASEWEAZLE_CHOSEN) {           // Greasweazle 
        
                            greaseweazle_tree_state = NK_MAXIMIZED;               // Greasweazle - Don't need to set a default for the computer and drive because this supports so many formats. 
                            zoomfloppy_tree_state   = NK_MINIMIZED;               // ZoomFloppy
                            usbfloppy_tree_state    = NK_MINIMIZED;               // USBFloppy
        
                        } else if (hardware_selected == ZOOMFLOPPY_CHOSEN) {      // ZoomFloppy
        
                            greaseweazle_tree_state = NK_MINIMIZED;               // Greasweazle 
                            zoomfloppy_tree_state   = NK_MAXIMIZED;               // ZoomFloppy
                            usbfloppy_tree_state    = NK_MINIMIZED;               // USBFloppy
        
                            if ( brand_selected != BRAND_COMMODORE ) {    
                                brand_selected      = BRAND_COMMODORE;            // Set the current brand to Commodore since that's all that ZoomFloppy works with!
                                computer_selected   = CBM_64C;                    // Set current computer to Commodore 64C
                                drive_selected      = CBM_DRIVE_1541II;           // Set current drive to 1541-II
                            };/*end-if*/    
        
                        } else if (hardware_selected == USBFLOPPY_CHOSEN) {       // USBFloppy
        
                            greaseweazle_tree_state = NK_MINIMIZED;               // Greasweazle
                            zoomfloppy_tree_state   = NK_MINIMIZED;               // ZoomFloppy
                            usbfloppy_tree_state    = NK_MAXIMIZED;               // USBFloppy

                            if ( brand_selected != BRAND_PCCOMPAT ) {
                                brand_selected      = BRAND_PCCOMPAT;         // Set the current brand to Commodore since that's all that ZoomFloppy works with!
                                computer_selected   = PC_80386;               // Set current computer to Commodore 64C
                                drive_selected      = PC_DRIVE_35_DD_720_IMA; // Set current drive to DD 720k
                            };/*end-if*/

                        };/*end-if*/

                    } else {

                        // printf("Nothing has changed.\n");

                    };/*end-if*/

                nk_layout_row_end(ctx);

                /*******************************************/

                /*******************************************/
                /******** ROW 5: three image panels ********/
                /*******************************************/

                nk_layout_row_begin(ctx, NK_STATIC, panel_h, 5);

                    // we are going to figure out the image to show first and then actually load it.    

                    static struct nk_image current_disk_image = {0};   
                    static FORMAT_ID_TYPE format_from_file; 

                    if ( has_currently_selected_file_changed == TRUE ) {

                        // if writing
                        if ( direction_selected == WRITING_A_FLOPPY ) {

                            // is a file picked? no --> show write question mark icon 
                            if ( (image_selected < 2) ) {
                                
                                current_disk_image = diskimagefile_load_questionmark;

                            } else {

                                // does the file picked exist? no --> show write question mark icon 
                                FILE *probe_1_check = fopen(recent_files[image_selected-2], "rb");

                                if (probe_1_check == NULL) {

                                    printf("CHECKING FILE ----------> File: %s does NOT exist.\n", recent_files[image_selected-2]);
                                    current_disk_image = diskimagefile_load_questionmark;

                                } else { // therefore if a file has been picked that exists then --> look at ext and size and pick the right write icon 

                                    printf("CHECKING FILE ----------> File: %s EXISTS. \n", recent_files[image_selected-2]);

                                    format_from_file = get_format_from_ext_and_size(FALSE, currently_selected_file_size, currently_selected_file_ext);
                                    
                                    if        ( (main_formats[ format_from_file ].size_inches == MINIDISK_525_INCH) && 
                                                (main_formats[ format_from_file ].density     == SD) ){

                                        current_disk_image = diskimagefile_load_525sd;

                                    } else if ( (main_formats[ format_from_file ].size_inches == MINIDISK_525_INCH) && 
                                                (main_formats[ format_from_file ].density     == DD) ){

                                        current_disk_image = diskimagefile_load_525dd;

                                    } else if ( (main_formats[ format_from_file ].size_inches == MINIDISK_525_INCH) && 
                                                (main_formats[ format_from_file ].density     == QD) ){

                                        current_disk_image = diskimagefile_load_525qd;

                                    } else if ( (main_formats[ format_from_file ].size_inches == MINIDISK_525_INCH) && 
                                                (main_formats[ format_from_file ].density     == HD) ){

                                        current_disk_image = diskimagefile_load_525hd;

                                    /*******************************************/

                                    } else if ( (main_formats[ format_from_file ].size_inches == DISKETTE_35_INCH) && 
                                                (main_formats[ format_from_file ].sides       == SS) ){ 

                                        current_disk_image = diskimagefile_load_350ss;

                                    } else if ( (main_formats[ format_from_file ].size_inches == DISKETTE_35_INCH) && 
                                                (main_formats[ format_from_file ].density     == DD) ){ 

                                        current_disk_image = diskimagefile_load_350dd;
                                    
                                    } else if ( (main_formats[ format_from_file ].size_inches == DISKETTE_35_INCH) && 
                                                (main_formats[ format_from_file ].density     == HD) ){

                                        current_disk_image = diskimagefile_load_350hd;   
                                    
                                    } else { 

                                        current_disk_image = diskimagefile_load_exclamationmark; 
                                    
                                    }/*end-if*/

                                };/*end-if*/  

                                fclose(probe_1_check);   

                            };/*end-if*/               

                        // if reading
                        } else if ( direction_selected == READING_A_FLOPPY ) {

                            // is a file picked? no --> show read question mark icon
                            if ( (image_selected < 2) ) {
                                
                                current_disk_image = diskimagefile_save_questionmark;

                            } else {
                                
                                // if a file is picked then --> use the currently selected format and pick the right read icon 
                                if        ( (main_formats[ drive_map[brand_selected][drive_selected] ].size_inches == MINIDISK_525_INCH) && 
                                            (main_formats[ drive_map[brand_selected][drive_selected] ].density     == SD)                ){
                                    
                                    current_disk_image = diskimagefile_save_525sd;

                                } else if ( (main_formats[ drive_map[brand_selected][drive_selected] ].size_inches == MINIDISK_525_INCH) && 
                                            (main_formats[ drive_map[brand_selected][drive_selected] ].density     == DD)                ){
                                    
                                    current_disk_image = diskimagefile_save_525dd;

                                } else if ( (main_formats[ drive_map[brand_selected][drive_selected] ].size_inches == MINIDISK_525_INCH) && 
                                            (main_formats[ drive_map[brand_selected][drive_selected] ].density     == QD)                ){
                                    
                                    current_disk_image = diskimagefile_save_525qd;

                                } else if ( (main_formats[ drive_map[brand_selected][drive_selected] ].size_inches == MINIDISK_525_INCH) && 
                                            (main_formats[ drive_map[brand_selected][drive_selected] ].density     == HD)                ){
                                    
                                    current_disk_image = diskimagefile_save_525hd;

                                /*******************************************/

                                } else if ( (main_formats[ drive_map[brand_selected][drive_selected] ].size_inches == DISKETTE_35_INCH) && 
                                            (main_formats[ drive_map[brand_selected][drive_selected] ].sides       == SS)               ){
                                    
                                    current_disk_image = diskimagefile_save_350ss;

                                } else if ( (main_formats[ drive_map[brand_selected][drive_selected] ].size_inches == DISKETTE_35_INCH) && 
                                            (main_formats[ drive_map[brand_selected][drive_selected] ].density     == DD)               ){
                                    
                                    current_disk_image = diskimagefile_save_350dd;

                                } else if ( (main_formats[ drive_map[brand_selected][drive_selected] ].size_inches == DISKETTE_35_INCH) && 
                                            (main_formats[ drive_map[brand_selected][drive_selected] ].density     == HD)               ){
                                    
                                    current_disk_image = diskimagefile_save_350hd;
                                
                                } else { 

                                    current_disk_image = diskimagefile_save_exclamationmark;

                                };/*end-if*/

                            };/*end-if*/

                        } else { 
                            
                            current_disk_image = diskimagefile_save_exclamationmark;

                        };/*end-if*/

                    };/*end-if*/

                    // This is a classic Nuklear gotcha — nk_widget_is_hovered checks the last laid out widget, but nk_image doesn't consume the widget slot in the same way a button does. So by the time you call nk_widget_is_hovered, Nuklear has already moved on and you're actually checking the next widget's bounds.
                    // The fix is to grab the widget bounds before drawing the image, and do your own bounds check using the mouse position:
                    nk_layout_row_push(ctx, small_w);

                    struct nk_rect image_bounds = nk_widget_bounds(ctx); // grab bounds BEFORE nk_image
                    nk_image(ctx, current_disk_image); // ALWAYS drawn every frame

                    has_currently_selected_file_changed = FALSE;

                    // Tooltip for current selected file. 
                    if ( nk_input_is_mouse_hovering_rect(&ctx->input, image_bounds) && image_selected >= 2 ) {

                        char dir[MAX_PATH * 1];
                        char tooltip_path_buf[MAX_PATH * 2];
                        char tooltip_buf[MAX_PATH * 4];

                        const char *full_path = recent_files[image_selected-2];
                        const char *fname = strrchr(full_path, '\\');
                        fname = fname ? fname + 1 : full_path;  /* if no backslash found, the whole thing is the filename */
                        
                        strncpy(dir, full_path, sizeof(dir) - 1);
                        dir[sizeof(dir) - 1] = '\0';
                        char *last_slash = strrchr(dir, '\\');
                        if (last_slash) *(last_slash + 1) = '\0';  /* keep the trailing backslash */

                        // This creates the tooltip path if it's enabled. But it's a long string so that's why it's an option and something that can be toggled on and off. 
                        if (is_tooltip_path_enabled) snprintf(tooltip_path_buf, sizeof(tooltip_path_buf), " | PATH: %s", dir);
                        else tooltip_path_buf[0] = '\0';
                        /*end-if*/

                        // Finally cook up the tooltip and set it!
                        if (direction_selected == WRITING_A_FLOPPY) {  

                            // If making a floppy we display the disk image file size. // NOTE: We need an extra space in front of tooltips has the mouse cursor can cover them up! 
                            snprintf( tooltip_buf, sizeof(tooltip_buf), " %s - %s bytes.%s", fname, with_commas(currently_selected_file_size), tooltip_path_buf);
                        
                        } else if (direction_selected == READING_A_FLOPPY) { 

                            // If reading a floppy then we haven't written the file yet so there's no size to display. 
                            snprintf( tooltip_buf, sizeof(tooltip_buf), " %s%s", fname , tooltip_path_buf );

                        };/*end-if*/

                        if (is_tooltip_text_enabled) nk_tooltip(ctx, tooltip_buf);          

                    } else if ( nk_input_is_mouse_hovering_rect(&ctx->input, image_bounds) ) {

                        if (is_tooltip_text_enabled) nk_tooltip(ctx, " No file selected! Please choose a file."); // NOTE: We need an extra space in front of tooltips has the mouse cursor can cover them up!

                    };/*end-if*/

                    nk_layout_row_push(ctx, 10);
                    nk_spacing(ctx, 1);

                    nk_layout_row_push(ctx, small_w);

                    // TODO: This could be a two liner where it's a snprintf with the parameters doing the simple is_it_this ? yes : no kinda thing. 
                    //       condition ? value_if_true : value_if_false
                    //       Yeah but like the image selection thing can't do that. Fuck. I dunno. Maybe not. This works who cares. 
                    //       Figure out a better way to do this later. 
                    // NOTE: We need an extra space in front of tooltips has the mouse cursor can cover them up!
                    if        ( (main_formats[drive_map[brand_selected][drive_selected]].size_inches == MINIDISK_525_INCH) && 
                              (main_formats[drive_map[brand_selected][drive_selected]].density       == SD )               &&
                              (direction_selected == WRITING_A_FLOPPY) ) {

                        TOOLTIP( nk_image( ctx, transfer_file2floppy_525sd) , " Load a file on your computer and write it to a real 5.25\" Single-Density floppy disk." );

                    } else if ( (main_formats[drive_map[brand_selected][drive_selected]].size_inches == MINIDISK_525_INCH) && 
                              (main_formats[drive_map[brand_selected][drive_selected]].density       == DD)                &&
                              (direction_selected == WRITING_A_FLOPPY) ) {

                        TOOLTIP( nk_image( ctx, transfer_file2floppy_525dd) , " Load a file on your computer and write it to a real 5.25\" Double-Density floppy disk." );

                    } else if ( (main_formats[drive_map[brand_selected][drive_selected]].size_inches == MINIDISK_525_INCH) && 
                              (main_formats[drive_map[brand_selected][drive_selected]].density       == QD)                &&
                              (direction_selected == WRITING_A_FLOPPY) ) {

                        TOOLTIP( nk_image( ctx, transfer_file2floppy_525qd) , " Load a file on your computer and write it to a real 5.25\" Quad-Density floppy disk." );

                    } else if ( (main_formats[drive_map[brand_selected][drive_selected]].size_inches == MINIDISK_525_INCH) && 
                              (main_formats[drive_map[brand_selected][drive_selected]].density       == HD)                &&
                              (direction_selected == WRITING_A_FLOPPY) ) {

                        TOOLTIP( nk_image( ctx, transfer_file2floppy_525hd) , " Load a file on your computer and write it to a real 5.25\" High-Density floppy disk." );

                    /*******************************************/

                    } else if ( (main_formats[drive_map[brand_selected][drive_selected]].size_inches == MINIDISK_525_INCH) && 
                              (main_formats[drive_map[brand_selected][drive_selected]].density       == SD)                &&
                              (direction_selected == READING_A_FLOPPY) ) {

                        TOOLTIP( nk_image( ctx, transfer_floppy2file_525sd) , " Read a real 5.25\" SD floppy disk and save it as a file to your computer."  );

                    } else if ( (main_formats[drive_map[brand_selected][drive_selected]].size_inches == MINIDISK_525_INCH) && 
                              (main_formats[drive_map[brand_selected][drive_selected]].density       == DD)                &&
                              (direction_selected == READING_A_FLOPPY) ) {

                        TOOLTIP( nk_image( ctx, transfer_floppy2file_525dd) , " Read a real 5.25\" DD floppy disk and save it as a file to your computer."  );

                    } else if ( (main_formats[drive_map[brand_selected][drive_selected]].size_inches == MINIDISK_525_INCH) && 
                              (main_formats[drive_map[brand_selected][drive_selected]].density       == QD)                &&
                              (direction_selected == READING_A_FLOPPY) ) {

                        TOOLTIP( nk_image( ctx, transfer_floppy2file_525qd) , " Read a real 5.25\" QD floppy disk and save it as a file to your computer."  );

                    } else if ( (main_formats[drive_map[brand_selected][drive_selected]].size_inches == MINIDISK_525_INCH) && 
                              (main_formats[drive_map[brand_selected][drive_selected]].density       == HD)                &&
                              (direction_selected == READING_A_FLOPPY) ) {

                        TOOLTIP( nk_image( ctx, transfer_floppy2file_525hd) , " Read a real 5.25\" HD floppy disk and save it as a file to your computer."  );

                    /*******************************************/

                    } else if ( (main_formats[drive_map[brand_selected][drive_selected]].size_inches == DISKETTE_35_INCH) && 
                              (main_formats[drive_map[brand_selected][drive_selected]].sides         == SS)               &&
                              (direction_selected == WRITING_A_FLOPPY) ) {

                        TOOLTIP( nk_image( ctx, transfer_file2floppy_350ss) , " Load a file on your computer and write it to a real 3.5\" Single-Sided floppy disk."  );

                    } else if ( (main_formats[drive_map[brand_selected][drive_selected]].size_inches == DISKETTE_35_INCH) && 
                              (main_formats[drive_map[brand_selected][drive_selected]].density       == DD)               &&
                              (direction_selected == WRITING_A_FLOPPY) ) {

                        TOOLTIP( nk_image( ctx, transfer_file2floppy_350dd) , " Load a file on your computer and write it to a real 3.5\" Double-Density floppy disk."  );

                    } else if ( (main_formats[drive_map[brand_selected][drive_selected]].size_inches == DISKETTE_35_INCH) && 
                              (main_formats[drive_map[brand_selected][drive_selected]].density       == HD)               &&
                              (direction_selected == WRITING_A_FLOPPY) ) {

                        TOOLTIP( nk_image( ctx, transfer_file2floppy_350hd) , " Load a file on your computer and write it to a real 3.5\" High-Density floppy disk."  );

                    /*******************************************/

                    } else if ( (main_formats[drive_map[brand_selected][drive_selected]].size_inches == DISKETTE_35_INCH) && 
                              (main_formats[drive_map[brand_selected][drive_selected]].sides         == SS)               &&
                              (direction_selected == READING_A_FLOPPY) ) {

                        TOOLTIP( nk_image( ctx, transfer_floppy2file_350ss) , " Read a real 3.5\" Single-Sided floppy disk and save it as a file to your computer."   );

                    } else if ( (main_formats[drive_map[brand_selected][drive_selected]].size_inches == DISKETTE_35_INCH) && 
                              (main_formats[drive_map[brand_selected][drive_selected]].density       == DD)               &&
                              (direction_selected == READING_A_FLOPPY) ) {

                        TOOLTIP( nk_image( ctx, transfer_floppy2file_350dd) , " Read a real 3.5\" Double-Density floppy disk and save it as a file to your computer."   );

                    } else if ( (main_formats[drive_map[brand_selected][drive_selected]].size_inches == DISKETTE_35_INCH) && 
                              (main_formats[drive_map[brand_selected][drive_selected]].density       == HD)               &&
                              (direction_selected == READING_A_FLOPPY) ) {

                        TOOLTIP( nk_image( ctx, transfer_floppy2file_350hd) , " Read a real 3.5\" High-Density floppy disk and save it as a file to your computer."   );

                    } /*end-if*/

                    nk_layout_row_push(ctx, 10);
                    nk_spacing(ctx, 1);

                    nk_layout_row_push(ctx, small_w);

                    // NOTE: We need an extra space in front of tooltips has the mouse cursor can cover them up!
                    // Greasweazle // hw_gw_size - 0=5.25, 1=3.5 | hw_gw_tracks - 0=40, 1=80 | hw_gw_drive_type - 0=PCBus, 1=Shugart | hw_gw_drive - 0=A/0, 1=B/1
                    if        (hardware_selected == GREASEWEAZLE_CHOSEN) { 

                        if        ( (hw_gw_size==0) && (hw_gw_tracks==0) ) { 
                            
                            // If they chose the PC BUS and selected the first drive (A) that means it's a twisted cable!
                            if ( (hw_gw_drive_type==1) && (hw_gw_drive==0) ) TOOLTIP( nk_image(ctx, hardware_greaseweazle_525DD_with_twist ), " Greaseweazle --> twisted cable --> 5.25\" DD drive."  ); // Cable has a TWIST.
                            else                                             TOOLTIP( nk_image(ctx, hardware_greaseweazle_525DD_no_twist   ), " Greaseweazle --> straight cable --> 5.25\" DD drive." ); // Straight cable (no twist).

                        } else if ( (hw_gw_size==0) && (hw_gw_tracks==1) ) { // hw_gw_size = 0; 0=5.25, 1=3.5 - hw_gw_tracks = 0; 0=40, 1=80

                            // nk_image(ctx, hardware_greaseweazle_525HD);
                            // If they chose the PC BUS and selected the first drive (A) that means it's a twisted cable!
                            if ( (hw_gw_drive_type==1) && (hw_gw_drive==0) ) TOOLTIP( nk_image(ctx, hardware_greaseweazle_525HD_with_twist ), " Greaseweazle --> twisted cable --> 5.25\" HD drive."  ); // Cable has a TWIST.
                            else                                             TOOLTIP( nk_image(ctx, hardware_greaseweazle_525HD_no_twist   ), " Greaseweazle --> straight cable --> 5.25\" HD drive." ); // Straight cable (no twist).

                        } else if ( (hw_gw_size==1) && (hw_gw_density==0) ) { // hw_gw_size = 0; 0=5.25, 1=3.5 - hw_gw_density = 0; 0=DD, 1=HD

                            // nk_image(ctx, hardware_greaseweazle_350DD);
                            // If they chose the PC BUS and selected the first drive (A) that means it's a twisted cable!
                            if ( (hw_gw_drive_type==1) && (hw_gw_drive==0) ) TOOLTIP( nk_image(ctx, hardware_greaseweazle_350DD_with_twist ), " Greaseweazle --> twisted cable --> 3.5\" DD drive."  ); // Cable has a TWIST.
                            else                                             TOOLTIP( nk_image(ctx, hardware_greaseweazle_350DD_no_twist   ), " Greaseweazle --> straight cable --> 3.5\" DD drive." ); // Straight cable (no twist).

                        } else if ( (hw_gw_size==1) && (hw_gw_density==1) ) { // hw_gw_size = 0; 0=5.25, 1=3.5 - hw_gw_density = 0; 0=DD, 1=HD

                            // nk_image(ctx, hardware_greaseweazle_350HD);
                            // If they chose the PC BUS and selected the first drive (A) that means it's a twisted cable!
                            if ( (hw_gw_drive_type==1) && (hw_gw_drive==0) ) TOOLTIP( nk_image(ctx, hardware_greaseweazle_350HD_with_twist ), " Greaseweazle --> twisted cable --> 3.5\" HD drive."  ); // Cable has a TWIST.
                            else                                             TOOLTIP( nk_image(ctx, hardware_greaseweazle_350HD_no_twist   ), " Greaseweazle --> straight cable --> 3.5\" HD drive." ); // Straight cable (no twist).

                        };/*end-if*/
                    
                    // ZoomFloppy
                    } else if (hardware_selected == ZOOMFLOPPY_CHOSEN) {

                        if      ( current_opencbm_drive_selected == CBM_1541   ) TOOLTIP( nk_image(ctx, hardware_zoomfloppy_1541    ) , " ZoomFloppy --> CBM 1541"     );
                        else if ( current_opencbm_drive_selected == CBM_1541II ) TOOLTIP( nk_image(ctx, hardware_zoomfloppy_1541II  ) , " ZoomFloppy --> CBM 1541-II"  );
                        else if ( current_opencbm_drive_selected == CBM_1570   ) TOOLTIP( nk_image(ctx, hardware_zoomfloppy_1570    ) , " ZoomFloppy --> CBM 1570"     );
                        else if ( current_opencbm_drive_selected == CBM_1571   ) TOOLTIP( nk_image(ctx, hardware_zoomfloppy_1571    ) , " ZoomFloppy --> CBM 1571"     );
                        else if ( current_opencbm_drive_selected == CBM_1581   ) TOOLTIP( nk_image(ctx, hardware_zoomfloppy_1581    ) , " ZoomFloppy --> CBM 1581"     );
                        else if ( current_opencbm_drive_selected == SFD_1001   ) TOOLTIP( nk_image(ctx, hardware_zoomfloppy_sfd1001 ) , " ZoomFloppy --> CBM SFD-1001" );
                        else if ( current_opencbm_drive_selected == CBM_8050   ) TOOLTIP( nk_image(ctx, hardware_zoomfloppy_8050    ) , " ZoomFloppy --> CBM 8050"     );
                        else if ( current_opencbm_drive_selected == CBM_8250   ) TOOLTIP( nk_image(ctx, hardware_zoomfloppy_8250    ) , " ZoomFloppy --> CBM 8250"     );
                        else if ( current_opencbm_drive_selected == CBM_2031   ) TOOLTIP( nk_image(ctx, hardware_zoomfloppy_2031    ) , " ZoomFloppy --> CBM_2031"     );
                        else if ( current_opencbm_drive_selected == CBM_4031   ) TOOLTIP( nk_image(ctx, hardware_zoomfloppy_4031    ) , " ZoomFloppy --> CBM_4031"     ); // The 2031 and 4031 are functionally and astecically the same! 
                        else if ( current_opencbm_drive_selected == CBM_4040   ) TOOLTIP( nk_image(ctx, hardware_zoomfloppy_4040    ) , " ZoomFloppy --> CBM_4040"     );
                        else if ( current_opencbm_drive_selected == CBM_3040   ) TOOLTIP( nk_image(ctx, hardware_zoomfloppy_3040    ) , " ZoomFloppy --> CBM_3040"     );
                        else if ( current_opencbm_drive_selected == CBM_2040   ) TOOLTIP( nk_image(ctx, hardware_zoomfloppy_2040    ) , " ZoomFloppy --> CBM_2040"     );
                    
                    // USB Floppy
                    } else if (hardware_selected == USBFLOPPY_CHOSEN) {
                    
                        nk_image(ctx, hardware_usbfloppy);
                    
                    };/*end-if*/

                    nk_layout_row_end(ctx);

                    /* spacer */
                    nk_layout_row_dynamic(ctx, 3, 1);
                    nk_spacing(ctx, 1);

                /*******************************************/
                /********** ROW 6: Start! Button ***********/
                /*******************************************/

                nk_layout_row_begin(ctx, NK_STATIC, 35, 1);
                
                    nk_layout_row_push(ctx, 425);
                    
                    /********************************************************************************/
                    /********************************************************************************/
                    /*** START BUTTON TO DO ALL THE FINAL CHECKS BEFORE RUNNING THE COMMAND LINE! ***/
                    /********************************************************************************/
                    /********************************************************************************/

                    struct nk_style_button start_style = ctx->style.button;
                    /************************************************************************************************************************************/
                    /******************************************************************/ /* Below are the NORMAL, or mouse off, colors.                 */
                    start_style.normal      = nk_style_item_color(UI_BACK_NORMAL_COLOR); /* The background button or UI element color. The body color.  */
                    start_style.text_normal =                     UI_TEXT_NORMAL_COLOR;  /* The button or UI element's text color.                      */
                    /******************************************************************/ /***************************************************************/
                    /******************************************************************/ /* Below are the HOVER, or mouse over, colors.                 */
                    start_style.hover       = nk_style_item_color(UI_BACK_HOVER_COLOR);  /* The background button or UI element color. The body color.  */
                    start_style.text_hover  =                     UI_TEXT_HOVER_COLOR;   /* The button or UI element's text color.                      */
                    /******************************************************************/ /***************************************************************/
                    /******************************************************************/ /* Below are the ACTIVE, or mouse button down, colors.         */
                    start_style.active      = nk_style_item_color(UI_BACK_ACTIVE_COLOR); /* The background button or UI element color. The body color.  */  
                    start_style.text_active =                     UI_TEXT_ACTIVE_COLOR;  /* The button or UI element's text color.                      */ 
                    /************************************************************************************************************************************/
                    if (nk_button_label_styled(ctx, &start_style, "Start!")) { 
     
                        // Yeah... doing the following crashed the program... as it should HAHA! --> if START_BUTTON_PRESSED:; // This is a HORRID way of adding a keyboard shortcut to this!

                        printf("Start pressed!\n");

                        // This gives us the file extension of the file the user selected. 
                        const char *dot = strrchr(recent_files[image_selected-2], '.');

                        /********************************************************************************/
                        /*************************** GREASEWEAZLE PROCESSING ****************************/
                        /********************************************************************************/
                        if (hardware_selected == GREASEWEAZLE_CHOSEN) { // static const char *hardware_options[]  = {"Greaseweazle", "ZoomFloppy", "USB Floppy"};

                            printf("Greaseweazle processing chosen.\n"); 

                            // Check if the options have set valid strings for the Greaseweazle path and executable.
                            // If not, skip everything else. 
                            if ( !is_string_valid(options_greaseweazle_path) || !is_string_valid(options_greaseweazle_executable) ) {
                                printf("One of these strings: options_greaseweazle_path or options_greaseweazle_executable is empty!\n");
                                MessageBox(NULL, "The Greaseweazle options for path or executable are not set.\nPlease check the Options.", "Error", MB_OK | MB_ICONERROR);
                                goto END_OF_GREASEWEAZLE_PROCESSING; // Stop checking. Just dip-out right outta here!
                            };/*end-if*/

                            passed_all_checks = TRUE;

                            // This gives the ability to disable the greaseweazle check!
                            if (hw_gw_verify == ENABLED) passed_all_checks = verify_greaseweazle_drive(hw_gw_drive_type, hw_gw_drive);
                            // TODO: This is where we should jump over all this shit. I know... goto is the devil. 
                            //       Well... Dijkstra can suck a duck. 'Cause this is how we roll!

                            printf("recent_files[image_selected-2]:%s\n",recent_files[image_selected-2]);

                            // FILE EXISTS --> Filename must not be empty.
                            // The image dropdown has "No file set..." at index 0 and "Browse..." at index 1.
                            // Actual files start at index 2, which is why we use (image_selected - 2) to index into recent_files[].
                            // So if image_selected is less than 2, the user hasn't actually picked a file yet and we bail out here
                            // rather than letting (image_selected - 2) produce a negative array index and crash the program.
                            // if ( recent_files[image_selected-2][0] == '\0' ) { 
                            if (image_selected < 2) {
                                passed_all_checks = FALSE;
                                MessageBox(NULL, "You don't appear to have chosen a file!", "Error", MB_OK | MB_ICONERROR);
                            };/*end-if*/
                            
                            if ( passed_all_checks == FALSE ) goto END_OF_GREASEWEAZLE_PROCESSING; // Stop checking. Just dip-out right outta here!

                            // TODO: New thing: check for presence of any extension at all, then, 
                            //       Check for presenece of first extension in the struct, then check for match, then,
                            //       Check for preseence of second extension in the stuct, then chech for match. 
                            //       Finally if none of this matches, fire off the message box!

                            // First check if the file the user gave even has an extension. It must because this whole vintage computing disk image file formats things is schenanigans! 
                            if ( !is_string_valid(dot+1) ) {
                                passed_all_checks = FALSE;
                                MessageBox(NULL, "The file extension of your file doesn't match the selected disk image format.\nPlease check your file selection.", "Error", MB_OK | MB_ICONERROR);                                
                            };/*end-if*/

                            if ( passed_all_checks == FALSE ) goto END_OF_GREASEWEAZLE_PROCESSING; // Stop checking. Just dip-out right outta here!

                            // Check that this entry in the struct: main_formats[].file_ext is actually a string as it should be. This is a BUG if it's not there so error out and end program!
                            if ( !is_string_valid(main_formats[drive_map[brand_selected][drive_selected]].file_ext) ) {
                                MessageBox(NULL, "Programming Error!\nBro the program struct main_formats[].file_ext has an empty entry.\nThat's BAD!\nThe programmer needs to fix this!\nSorry Bud! Thanks, eh!", "Error", MB_OK | MB_ICONERROR);
                                exit(1); // Exit the whole program because this means main_formats is kinda fucked up!
                            };/*end-if*/

                            // TODO: This needs to check for the rest of the alternative file types. But like that's hard... I have to think about that logic first!
                            // Test if first extension doesn't match...                                        
                            if ( dot && strcasecmp(main_formats[drive_map[brand_selected][drive_selected]].file_ext, dot+1) ) { // If it's an INVALID primary extension...

                                printf("The first extension: %s FAILED TO MATCH the main_formats table first ext: %s \n",main_formats[drive_map[brand_selected][drive_selected]].file_ext,dot+1);

                                // Then if try second and if second doesn't match...                           
                                if ( !(is_string_valid(main_formats[drive_map[brand_selected][drive_selected]].file_ext_alt1))  ||   /* If there's an INVALID ("") alt1 extension string. */
                                     (strcasecmp(main_formats[drive_map[brand_selected][drive_selected]].file_ext_alt1, dot+1))      /* OR if it's a INVALID alt1 extension. */
                                   ) {
                                        // Since both the first and second file extensions don't match, exit with an error.
                                        passed_all_checks = FALSE;
                                        printf("The second extension: %s FAILED TO MATCH the main_formats table first ext: %s \n",main_formats[drive_map[brand_selected][drive_selected]].file_ext_alt1,dot+1);
                                        MessageBox(NULL, "Your file's extension doesn't match the drive you've chosen.\nPlease check your selections.", "Error", MB_OK | MB_ICONERROR);    
                                };/*end-if*/
                                        
                            };/*end-if*/

                            // If at this point we don't have hardware and/or we don't have a file selected,
                            // then we need to just dip-out right here!
                            if ( passed_all_checks == FALSE ) goto END_OF_GREASEWEAZLE_PROCESSING;

                            // printf("dot+1:%s | .file_ext:%s \n", dot+1, main_formats[drive_map[brand_selected][drive_selected]].file_ext);

                            // SUPPORTED --> greaseweazle must support this type of disk image 
                            if (main_formats[ drive_map[brand_selected][drive_selected] ].greaseweazle_supported == UNSUPPORTED ) {
                                MessageBox(NULL, "Greaseweazle doesn't support this format!", "Error", MB_OK | MB_ICONERROR);
                                passed_all_checks = FALSE;
                            };/*end-if*/

                            // DISK SIZE --> They must plug in a drive that matches the format size. (We are only checking what they enter. Greaseweaze can't detect if a drive is 5.25 or 3.5 as far as I know.)
                            // hw_gw_size = 0;         /* 0=5.25, 1=3.5 */
                            // main_formats[ drive_map[brand_selected][drive_selected] ].size_inches  MINIDISK_525_INCH = 1,  DISKETTE_35_INCH  = 2,
                            if ( (main_formats[drive_map[brand_selected][drive_selected]].size_inches == MINIDISK_525_INCH) && (hw_gw_size!=0) ) {
                                MessageBox(NULL, "You have selected a 5.25-inch disk image but have NOT SET 5.25 in Greaseweazle under Hardware. Please check.", "Error", MB_OK | MB_ICONERROR);
                                passed_all_checks = FALSE;
                            };/*end-if*/ 

                            // Check if format disk size in inches matches the selected drive.
                            if ( (main_formats[drive_map[brand_selected][drive_selected]].size_inches == DISKETTE_35_INCH) && (hw_gw_size!=1) ) {
                                MessageBox(NULL, "You have selected a 3.5-inch disk image but have NOT SET 3.5 in Greaseweazle under Hardware. Please check.", "Error", MB_OK | MB_ICONERROR);
                                passed_all_checks = FALSE;
                            };/*end-if*/ 

                            // TRACKS --> The number or tracks of the hardware must match the format OR double stepping should be enabled instead.
                            // hw_gw_tracks = 0;       /* 0=40, 1=80 */
                            // hw_gw_steps = 0;        /* 0=Single, 1=Double */
                            // main_formats[ drive_map[brand_selected][drive_selected] ].tracks    
                            // 
                            // If they picked a 40 drive and a 40 image --> Okay 
                            // If they picked an 80 drive and an 80 image --> Okay
                            // If they picked an 80 drive and a 40 image and picked double stepping --> Okay
                            // if they picked an 80 drive and a 40 image and didn't pick double step do error dialog
                            // Error 40 on an 80. 
                                                                                 
                            if ( ( main_formats[ drive_map[brand_selected][drive_selected] ].tracks <= 40 ) &&    // The format is 40 tracks or less.
                                                                                     ( hw_gw_tracks == 1  ) &&    // The hardware is set to 80-tracks.
                                                                                     ( hw_gw_steps  != 1  )       // The hardware is NOT SET to double-stepping.
                               ){

                                MessageBox(NULL, "You're trying to process a 40-track (or less) image on an 80-track drive without enabling double stepping. You either want to change from 80-track to 40-track, or, set \"Steps:\" to \"Double\" in Greaseweazle under Hardware.", "Error", MB_OK | MB_ICONERROR);
                                passed_all_checks = FALSE;
                            
                            };/*end-if*/

                            // Error 80 on an 40. 
                            if ( (hw_gw_tracks==0) && (main_formats[ drive_map[brand_selected][drive_selected] ].tracks > 40) ) {
                                MessageBox(NULL, "You're trying to process an image greater than 40 tracks (for example: an 80-track or a 77-track image) on a 40-track drive. Please check your Greaseweazle settings under Hardware.", "Error", MB_OK | MB_ICONERROR);
                                passed_all_checks = FALSE;
                            };/*end-if*/

                            // TODO: Also check if they are trying to write a physical 5.25" floppy a 40-track image on an 80-track drive using double stepping. 
                            //       This is an issue as even if it's written in double-density mode the head it too small and it'll be  
                            //       unreliable on native 40-track drives. Test this only: write, 80 drive with double step, 40 image. 

                            // TODO: Maybe also give a little alert when they are about to use a 5.25" HD 80 drive about RPM. 
                            //       Also this should have a setting in options to disable it.

                            // TODO: There should also be a setting in option to disable all these warnings. 

                            // DENSITY --> The density of the drive should match the format. 
                            // hw_gw_density = 0;      /* 0=DD, 1=HD */
                            // main_formats[ drive_map[brand_selected][drive_selected] ].density  
                            // SD = 0, // Single-Density   (set drive to double-density)
                            // DD = 1, // Double-Density   (set drive to double-density)
                            // QD = 2, // Quad-Density     (set drive to double-density)
                            // HD = 3, // High-Density     (set drive to high-density)
                            // ED = 4, // Extended-Density (TODO: NO FUCKING CLUE?!?!?! Probably doesn't matter.)
                            // TD = 5, // Triple-Density   (TODO: NO FUCKING CLUE?!?!?! Probably doesn't matter.)
                            if (    (hw_gw_density != 0)             /* If density is NOT DD and any of SD DD QD is the image format... */
                                            && 
                                    ( (main_formats[drive_map[brand_selected][drive_selected]].density==SD) || 
                                      (main_formats[drive_map[brand_selected][drive_selected]].density==DD) || 
                                      (main_formats[drive_map[brand_selected][drive_selected]].density==QD)    ) ){

                                    MessageBox(NULL, "You've set high-density for a disk image\nthat IS NOT high-density.\nPlease check your hardware settings.", "Error", MB_OK | MB_ICONERROR);
                                    passed_all_checks = FALSE;

                            };/*end-if*/

                            // Check if image and drive density matches.
                            if ( (hw_gw_density != 1) && (main_formats[drive_map[brand_selected][drive_selected]].density==HD) ){

                                    MessageBox(NULL, "You've got a high-density disk image but you've HAVE NOT set for high-density.\nPlease check your hardware settings.", "Error", MB_OK | MB_ICONERROR);
                                    passed_all_checks = FALSE;

                            };/*end-if*/

                            // READ & CLOBBER --> If they are reading a floppy to a file and they have disabled "--no-clobber" so they could in fact clobber a file this should warn them.
                            // (hw_gw_overwrite == 0) ? "--no-clobber" : "", --> Overwrite: [ ] Allow (checked means you CAN and --no-clobber is OFF!)
                            // The "--no-clobber" parameter ONLY APPLIES when we are reading a floppy and saving it to a file.
                            if ( (hw_gw_overwrite == 1) && (passed_all_checks == TRUE) && (direction_selected == READING_A_FLOPPY) ){ 

                                int result = MessageBox(NULL, "You allowed Greaseweazle to \"clobber\" an existing file.\nAre you sure you want to do this?", "Confirm", MB_YESNO | MB_ICONWARNING);
                                if (result != IDYES) {          // If they DO NOT want to proceed...
                                    passed_all_checks = FALSE;  // Flag that we have not passed all checks and will not call the command line to do disk stuff. 
                                };/*end-if*/

                            };/*end-if*/

                            /********************************************************************************/
                            /*************************** RENAME FILES IF NEEDED  ****************************/
                            /********************************************************************************/

                            // If a rename is required, AND the file string is NOT empty, AND we've passed all the checks... then figure out the direction and do the rename stuff where needed. Also WTF is this comment: // THIS NEEDS TO ALSO UPDATE THE DETAILS ENTRY SINCE WE ARE RENAMING THE FILE!
                            if ( ( gw_params[drive_map[brand_selected][drive_selected]].rename_ext_req == TRUE ) &&     /* The current unique image format needs a file rename for gw to work. */
                                 (                                   recent_files[image_selected-2][0] != '\0' ) &&     /* The currently selected filename is NOT empty. */
                                 (                                                   passed_all_checks == TRUE )        /* Don't bother doing any of this if there are other settings that fail the checks we do to help the user. */
                               ) { 

                                // TODO: Shoudln't temp_copy_path be set to empty ('\0') right here???

                                if ( direction_selected == WRITING_A_FLOPPY ) { // direction_selected == WRITING_A_FLOPPY == file2floppy == loading a file and writing to a floppy

                                    printf("Apparently greaseweazle needs us to rename this first from what it is:%s which should be this:%s to this:%s so yeah!\n",dot+1,gw_params[drive_map[brand_selected][drive_selected]].rename_source,gw_params[drive_map[brand_selected][drive_selected]].rename_target);
                                    
                                    // These are declaired at the top of the program before main () // char temp_copy_path[MAX_PATH]; // char copy_cmd[MAX_PATH * 2 + 16];
                                    /* Build destination path: same dir, new extension */

                                    strncpy(temp_copy_path, recent_files[image_selected-2], sizeof(temp_copy_path) - 1); // Copy the current filename to the temp copy to be differently named filename.

                                    temp_copy_path[sizeof(temp_copy_path) - 1] = '\0'; // Make sure last character is NULL for proper string termination. 

                                    char *tdot = strrchr(temp_copy_path, '.'); // Pull out the file extension with the dot?

                                    if (tdot) {

                                        snprintf(tdot, sizeof(temp_copy_path) - (tdot - temp_copy_path), ".%s", gw_params[drive_map[brand_selected][drive_selected]].rename_target); //WTF?
                                    
                                    };/*end-if*/

                                    /* Don't overwrite an existing file - try _1, _2, ... if needed */
                                    char temp_base[MAX_PATH + 16]; // Create a new temp var for doing stuff. 
                                    strncpy(temp_base, temp_copy_path, sizeof(temp_base) - 1); // Copy shit to temp_base. 
                                    temp_base[sizeof(temp_base) - 1] = '\0'; // Make sure string is gooder ending with NULL. 

                                    char *tbase_dot = strrchr(temp_base, '.'); // Pointer to where the dot is in the string??? 
                                    if (tbase_dot) *tbase_dot = '\0';  /* temp_base is now the path minus extension */
                                    int suffix = 1; // create new var and initialize it to one. Why? Don't know. 

                                    FILE *probe = fopen(temp_copy_path, "rb"); // Open a file for probing. Basically see if we can create this file.

                                    while (probe) { // We need to create a new temp file that doesn't exist. So this tries creating new files until one works. That then becomes our new temp file. 
                                        fclose(probe); // close the file. We just needed to know if we COULD open it. 
                                        snprintf(temp_copy_path, sizeof(temp_copy_path), "%s_%d.%s",
                                                 temp_base, suffix,
                                                 gw_params[drive_map[brand_selected][drive_selected]].rename_target);
                                        suffix++;
                                        if (suffix > 9999) { // Give up if there's like almost 10,000 fucking temp ass file here cause we are just cluttering things up at this point. 
                                            printf("ERROR: could not find a free filename for temp copy\n");
                                            // TODO: The above error should really display a message box to the user. Cause like WTF is going brah? Why you got so many temp files? 
                                            break;
                                        }
                                        probe = fopen(temp_copy_path, "rb"); // Next file we try to create. 
                                    }

                                    // As long as we have less than about 10,000 temp files, meaning it's all good we got our temp file,
                                    // then go ahead and run the rename command. 
                                    if (suffix <= 9999) { 
                                        snprintf(copy_cmd, sizeof(copy_cmd), "copy \"%s\" \"%s\"", recent_files[image_selected-2], temp_copy_path); // Build the string. 
                                        if (system(copy_cmd) == 0) {
                                            printf("Temp copy created: %s\n", temp_copy_path);
                                            /* TODO: pass temp_copy_path to Greaseweazle, then remove it when done. Wait WTF??? */
                                        } else {
                                            printf("ERROR: copy command failed. Maybe it's a permissions issue? I dunno. Go check it out!\n");
                                            // TODO: The above error should really display a message box to the user. 

                                        }
                                    }

                                } else if ( direction_selected == READING_A_FLOPPY ) { // direction_selected == READING_A_FLOPPY == floppy2file == reading a floppy and saving to a file

                                    char no_ext[MAX_PATH];
                                    strncpy(no_ext, recent_files[image_selected-2], sizeof(no_ext) - 1);
                                    no_ext[sizeof(no_ext) - 1] = '\0';
                                    char *no_ext_dot = strrchr(no_ext, '.');
                                    if (no_ext_dot) *no_ext_dot = '\0';

                                    // We want this: "C:\stuff\somefile.atr" to become this: "C:\stuff\somefile.img"
                                    snprintf(temp_copy_path, sizeof(temp_copy_path), "%s.%s", no_ext, gw_params[drive_map[brand_selected][drive_selected]].rename_target);

                                    printf("Trying to read a floppy into temp_copy_path: %s \n",temp_copy_path);

                                };/*end-if*/

                            };/*end-if*/

                            if ( ( gw_params[drive_map[brand_selected][drive_selected]].rename_ext_req == TRUE ) &&     /* The current unique image format needs a file rename for gw to work. */
                                 (                                   recent_files[image_selected-2][0] != '\0' ) &&     /* The currently selected filename is NOT empty. */
                                 (                                                   temp_copy_path[0] != '\0' )        /* The current temp_copy_path (created above) is NOT empty. */
                               ) {

                                // If we need to rename (actually copy) the file to make it work with Greaseweazle, then use this new file which is this: temp_copy_path.
                                snprintf(gw_cmd_string, sizeof(gw_cmd_string), 
                                    "\"%s\\%s\" %s --format=%s %s --densel %c --drive=%c --tracks=step=%c --retries %i \"%s\"", 
                                    options_greaseweazle_path , options_greaseweazle_executable ,
                                    (direction_selected == READING_A_FLOPPY) ? "read" : "write", 
                                    gw_params[ drive_map[brand_selected][drive_selected] ].format_string, 
                                    (direction_selected == READING_A_FLOPPY) ? (hw_gw_overwrite == 0) ? "--no-clobber" : "" : "", /* The "--no-clobber" option only works for reading to a file. */
                                    (hw_gw_density == 1) ? 'H' : 'L',
                                    (hw_gw_drive_type == 0) ? hw_gw_drive + '0' : hw_gw_drive + 'A',
                                    (hw_gw_steps == 1) ? '2' : '1',                                             /* This could be more intuitive but it works. hw_gw_steps==0 means "single step" (no extra steps) and hw_gw_steps==1 means "double step" (1 extra step). */
                                    hw_gw_retries,
                                    temp_copy_path 
                                );/*end-snprintf*/

                            } else {

                                // Otherwise the filename and extension is just fine as it is. // 
                                snprintf(gw_cmd_string, sizeof(gw_cmd_string), 
                                    "\"%s\\%s\" %s --format=%s %s --densel %c --drive=%c --tracks=step=%c --retries %i \"%s\"", 
                                    options_greaseweazle_path , options_greaseweazle_executable ,
                                    (direction_selected == READING_A_FLOPPY) ? "read" : "write", 
                                    gw_params[ drive_map[brand_selected][drive_selected] ].format_string, 
                                    (direction_selected == READING_A_FLOPPY) ? (hw_gw_overwrite == 0) ? "--no-clobber" : "" : "", /* The "--no-clobber" option only works for reading to a file. */
                                    (hw_gw_density == 1) ? 'H' : 'L',
                                    (hw_gw_drive_type == 0) ? hw_gw_drive + '0' : hw_gw_drive + 'A',
                                    (hw_gw_steps == 1) ? '2' : '1',                                             /* This could be more intuitive but it works. hw_gw_steps==0 means "single step" (no extra steps) and hw_gw_steps==1 means "double step" (1 extra step). */
                                    hw_gw_retries,
                                    recent_files[image_selected-2] 
                                );/*end-snprintf*/

                            };/*end-if*/

                            /********************************************************************************/
                            /********************* CONVERT FILES IF NEEDED FOR WRITING **********************/
                            /********************************************************************************/

                            if ( ( gw_params[drive_map[brand_selected][drive_selected]].conversion_req == TRUE             ) &&     /* The current unique image format needs a file rename for gw to work. */
                                 (                                   recent_files[image_selected-2][0] != '\0'             ) &&     /* The currently selected filename is NOT empty. */
                                 (                                                  direction_selected == WRITING_A_FLOPPY )        /* Only do this if we are writing a floppy. Not for reading. */
                               ) {

                                strncpy(temp_copy_path, recent_files[image_selected-2], sizeof(temp_copy_path) - 1); // Copy the current filename to the temp copy to be differently named filename.

                                temp_copy_path[sizeof(temp_copy_path) - 1] = '\0'; // Make sure last character is NULL for proper string termination. 

                                char *tdot = strrchr(temp_copy_path, '.'); // Pull out the file extension with the dot?

                                if (tdot) {
                                    // snprintf(tdot, sizeof(temp_copy_path) - (tdot - temp_copy_path), ".%s", gw_params[drive_map[brand_selected][drive_selected]].rename_target); //WTF?
                                    snprintf(tdot, sizeof(temp_copy_path) - (tdot - temp_copy_path), ".hdv"); // Okay I think this copies OVER the extension with the new .hdv extension.
                                };/*end-if*/

                                /* Don't overwrite an existing file - try _1, _2, ... if needed */
                                char temp_base[MAX_PATH + 16]; // Create a new temp var for doing stuff. 
                                strncpy(temp_base, temp_copy_path, sizeof(temp_base) - 1); // Copy shit to temp_base. 
                                temp_base[sizeof(temp_base) - 1] = '\0'; // Make sure string is gooder ending with NULL. 

                                char *tbase_dot = strrchr(temp_base, '.'); // Pointer to where the dot is in the string??? 
                                if (tbase_dot) *tbase_dot = '\0';  /* temp_base is now the path minus extension */

                                // Okay I think this copies OVER the extension with the new .hdv extension.
                                snprintf(temp_copy_path_target, sizeof(temp_copy_path_target), "%s.img", temp_base);            

                                // Create a string, where if the temp file is liek this: `Apple_IIGS_Farts_One.hdv` 
                                // then the target temp file for eventually 
                                // renaming this: `Apple_IIGS_Farts_One.hdv` to this: `Apple_IIGS_Farts_One.img`
                                
                                // I need to check for: Example: Some_File.HDV but also check for Some_File.IMG to make sure there isn't some other past conversion laying around to interfere with our temp files. 
                                int probe_1 = -1; // probe_2 --> 1 means that this file:temp_copy_path_target didn't work and a file like `Apple_IIGS_Farts_One.img` already exists. 
                                int probe_2 = -1; // probe_2 --> 0 means that this file:temp_copy_path_target doesn't exist and we can use it!

                                int suffix = 0; // create new var and initialize it to one. 

                                suffix++; // We are going to try the next filename that might not exist yet and is therefore valid to use!

                                snprintf(temp_copy_path,        sizeof(temp_copy_path),        "%s_%d.hdv", temp_base, suffix); // Create temp string like this: `Apple_IIGS_Farts_One_1.hdv` 
                                snprintf(temp_copy_path_target, sizeof(temp_copy_path_target), "%s_%d.img", temp_base, suffix); // Create temp string like this: `Apple_IIGS_Farts_One_1.img` 

                                // This runs at least once. The check at the top is to see if, in fact, any of the possible temp files already exist.
                                // So this tries creating new files until one works. That then we know what needs to become our new temp file. 
                                // If this doesn't work like this then you get a weird situation where it uses an OLD converted file
                                // and your looking at disk that isn't the latest version of what you expect! This was an annoying bug. 
                                while (1) { 

                                    //probe = fopen(temp_copy_path, "rb"); // Next file we try to create. 
                                    FILE *probe_1_check = fopen(temp_copy_path, "rb");
                                    if (probe_1_check != NULL) {
                                        fclose(probe_1_check);
                                        probe_1 = 1; // File EXISTS - can't use it, something is in the way!
                                        printf("CHECKING IMG FILE ----------> ERROR: File: %s EXISTS - can't use it, something is in the way! \n", temp_copy_path);
                                    } else {
                                        probe_1 = 0; // File does NOT exist - we're clear to use it!
                                        printf("CHECKING IMG FILE ----------> File: %s does NOT exist - we're clear to use it! \n", temp_copy_path);
                                    }
                                    
                                    // code for checking goes here first but also goes again at the end of the while loop.
                                    // Check if temp_copy_path_target (the .img file) already exists
                                    FILE *probe_2_check = fopen(temp_copy_path_target, "rb");
                                    if (probe_2_check != NULL) {
                                        fclose(probe_2_check);
                                        probe_2 = 1; // File EXISTS - can't use it, something is in the way!
                                        printf("ERROR: File EXISTS - can't use it, something is in the way! \n");
                                    } else {
                                        probe_2 = 0; // File does NOT exist - we're clear to use it!
                                        printf("File does NOT exist - we're clear to use it! \n");
                                    }

                                    printf("\n\nLOOP: suffix:%i --> Status of both probes into the files --> probe_1:%i - probe_2:%i \n\n",suffix,probe_1 ,probe_2);
                                    printf("temp_copy_path: %s \n\n", temp_copy_path);
                                    printf("temp_copy_path_target: %s \n\n", temp_copy_path_target);

                                    // This needs to happen here. We need to create the next strings for the files 
                                    // and then check if they are both okay to use. But they need to exist first
                                    // and we need to close the files as well. Then we can exit the loop!
                                    if (probe_1==0 && probe_2==0) {
                                        printf("Both files DO NOT EXIST! Huray!\n");
                                        break; // EXIT LOOP HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                                    } else {
                                        printf("One of the two files DO exist. Fuck...\n");
                                    };/*end-if*/

                                    suffix++; // We are going to try the next filename that might not exist yet and is therefore valid to use!

                                    snprintf(temp_copy_path,        sizeof(temp_copy_path),        "%s_%d.hdv", temp_base, suffix); // Create temp string like this: `Apple_IIGS_Farts_One_1.hdv` 
                                    snprintf(temp_copy_path_target, sizeof(temp_copy_path_target), "%s_%d.img", temp_base, suffix); // Create temp string like this: `Apple_IIGS_Farts_One_1.img` 

                                    if (suffix > 9999) { // Give up if there's like almost 10,000 fucking temp ass file here cause we are just cluttering things up at this point. 
                                        printf("ERROR: could not find a free filename for temp copy. \n");
                                        MessageBox(NULL, "ERROR: The program checked for over 9999 temporary file names and could not find a free filename the the temporary copy of the file for conversion.\nMaybe the issue is file permissions?", "Error", MB_OK | MB_ICONERROR);
                                        break;
                                    }

                                };/*end-while*/

                                printf("IS THIS A GOOD TEMP FILE??? --> temp_copy_path:        \"%s\" \n", temp_copy_path);
                                printf("IS THIS ALSO A GOOD FILE??? --> temp_copy_path_target: \"%s\" \n", temp_copy_path_target);

                                // when we find it, then we do the conversion

                                // As long as we have less than about 10,000 temp files, meaning it's all good we got our temp file,
                                // then go ahead and run the rename command. 
                                if (suffix <= 9999) {                               

                                    // Create new disk image using temp file 
                                    // > ./cp2.exe create-disk-image output.hdv 800k
                                    snprintf(copy_cmd, sizeof(copy_cmd), "\"%s\\%s\" create-disk-image \"%s\" 800k", options_ciderpress2cli_path, options_ciderpress2_cli_executable, temp_copy_path); // Build the string. 

                                    char output_buf[MAX_PATH];

                                    // if (system(copy_cmd) == 0) {
                                    if ( run_command(copy_cmd,output_buf,sizeof(output_buf)) == 1 ) { /* Worked */

                                        printf("WORKED! (1) --> \"%s\\%s\" create-disk-image \"%s\" 800k \n",options_ciderpress2cli_path, options_ciderpress2_cli_executable, temp_copy_path);

                                    } else {
                                        
                                        // printf("ERROR! (1) --> \"%s\\%s\" create-disk-image \"%s\" 800k \n",options_ciderpress2cli_path, options_ciderpress2_cli_executable, temp_copy_path);
                                        printf("ERROR! (1) --> copy_cmd:`%s` \n", copy_cmd);
                                        MessageBox(NULL, "ERROR: The program could not create a file for the convertion. Maybe the issue is file permissions?", "Error", MB_OK | MB_ICONERROR);
                                        
                                        // Jump out of here at the first sign of touble!
                                        goto END_OF_GREASEWEAZLE_PROCESSING;

                                    };/*end-if*/

                                    // Copy blocks from original file to the temp file
                                    // > ./cp2.exe copy-blocks "Apple_IIGS_disk.2mg" output.hdv   
                                    snprintf(copy_cmd, sizeof(copy_cmd), "\"%s\\%s\" copy-blocks \"%s\" \"%s\"", options_ciderpress2cli_path, options_ciderpress2_cli_executable, recent_files[image_selected-2], temp_copy_path); // Build the string. 

                                    //if (system(copy_cmd) == 0) {
                                    if ( run_command(copy_cmd,output_buf,sizeof(output_buf)) == 1 ) { /* Worked */

                                        printf("WORKED! (2) --> \"%s\\%s\" copy-blocks \"%s\" \"%s\" \n", options_ciderpress2cli_path, options_ciderpress2_cli_executable, recent_files[image_selected-2], temp_copy_path);

                                    } else {

                                        // printf("ERROR! (2) --> \"%s\\%s\" copy-blocks \"%s\" \"%s\" \n", options_ciderpress2cli_path, options_ciderpress2_cli_executable, recent_files[image_selected-2], temp_copy_path);
                                        printf("ERROR! (2) --> copy_cmd:`%s` \n", copy_cmd);
                                        MessageBox(NULL, "ERROR: Converting the disk image file failed. Maybe the issue is file permissions?", "Error", MB_OK | MB_ICONERROR);

                                        // Jump out of here at the first sign of touble!
                                        goto END_OF_GREASEWEAZLE_PROCESSING;

                                    };/*end-if*/

                                    // Rename the HDV (intermediate temp file) to IMG (final target file)
                                    // THe only reason this is needed is because ciderpress2 command line
                                    // gets pissed off if we try to do things with an IMG file. 
                                    // The HDV file is just a file of blocks with no extra shit. 
                                    // > Rename-Item output.hdv output.img 
                                    
                                    char rename_buf[MAX_PATH * 3];
                                    char rename_full_path[MAX_PATH];
                                    strncpy(rename_full_path , temp_base, sizeof(rename_full_path)-1);
                                    const char *rename_fname = strrchr(rename_full_path, '\\');
                                    rename_fname = rename_fname ? rename_fname + 1 : rename_full_path;  /* if no backslash found, the whole thing is the filename */
                                    char rename_dir[MAX_PATH];
                                    strncpy(rename_dir, rename_full_path, sizeof(rename_dir)-1);
                                    rename_dir[sizeof(rename_dir) - 1] = '\0';
                                    char *last_slash = strrchr(rename_dir, '\\');
                                    if (last_slash) *(last_slash + 1) = '\0';  /* keep the trailing backslash */
                                    snprintf(rename_buf, sizeof(rename_buf), "%s | PATH: %s", rename_fname, rename_dir);

                                    // Okay so coming out of the above code we've got this: 
                                    // rename_dir   --> String that's the path to the file WITHOUT the final filename in it. 
                                    // rename_fname --> String that's JUST the filename WITHOUT the full path in it. 

                                    snprintf(copy_cmd, sizeof(copy_cmd), "rename \"%s\" \"%s_%d.img\"", temp_copy_path, rename_fname, suffix); // Build the string. 

                                    if (system(copy_cmd) == 0) {
                                        
                                        printf("WORKED! --> rename \"%s\" \"%s_%d.img\" \n", temp_copy_path, rename_fname, suffix);

                                    } else {

                                        printf("ERROR! --> rename \"%s\" \"%s_%d.img\" \n", temp_copy_path, rename_fname, suffix); // rename is a fuckface that doesn't like FULL PATHS for some reason. 
                                        MessageBox(NULL, "ERROR: Renaming the converted file failed! Check for extra temp files and delete them if you can. Thanks!", "Error", MB_OK | MB_ICONERROR);

                                        // Jump out of here at the first sign of touble!
                                        goto END_OF_GREASEWEAZLE_PROCESSING;

                                    };/*end-if*/

                                    // Build up the greaseweazle command string but using the converted files instead of the original files. 
                                    snprintf(gw_cmd_string, sizeof(gw_cmd_string), 
                                        "\"%s\\%s\" %s --format=%s %s --densel %c --drive=%c --tracks=step=%c --retries %i \"%s\\%s_%d.img\"", 
                                        options_greaseweazle_path , options_greaseweazle_executable ,
                                        (direction_selected == READING_A_FLOPPY) ? "read" : "write", 
                                        gw_params[ drive_map[brand_selected][drive_selected] ].format_string, 
                                        (direction_selected == READING_A_FLOPPY) ? (hw_gw_overwrite == 0) ? "--no-clobber" : "" : "", /* The "--no-clobber" option only works for reading to a file. */
                                        (hw_gw_density == 1) ? 'H' : 'L',
                                        (hw_gw_drive_type == 0) ? hw_gw_drive + '0' : hw_gw_drive + 'A',
                                        (hw_gw_steps == 1) ? '2' : '1',                                             /* This could be more intuitive but it works. hw_gw_steps==0 means "single step" (no extra steps) and hw_gw_steps==1 means "double step" (1 extra step). */
                                        hw_gw_retries,
                                        rename_dir, rename_fname, suffix 
                                    );/*end-snprintf*/

                                };/*end-if*/

                            };/*end-if*/

                            /********************************************************************************/
                            /**************** END OF CONVERT FILES IF NEEDED FOR WRITING ********************/
                            /********************************************************************************/

                            /********************************************************************************/
                            /********************* CONVERT FILES IF NEEDED FOR READING **********************/
                            /********************************************************************************/

                            if ( ( gw_params[drive_map[brand_selected][drive_selected]].conversion_req == TRUE             ) &&     /* The current unique image format needs a file rename for gw to work. */
                                 (                                   recent_files[image_selected-2][0] != '\0'             ) &&     /* The currently selected filename is NOT empty. */
                                 (                                                  direction_selected == READING_A_FLOPPY )        /* Only do this if we are reading a floppy. Not for writing. */
                               ) {

                                // This is just the part where we build a little custom command line for Greaseweazle. 
                                // The conversation comes AFTER we have read in the floppy disk. 

                                FILE *probe_user_file_check = fopen( recent_files[image_selected-2] , "rb" );

                                if (probe_user_file_check != NULL) {

                                    fclose(probe_user_file_check);
                                    // probe_1 = 1; // File EXISTS - can't use it, something is in the way!
                                    printf("CHECKING THE USER FILE ----------> ERROR: File: %s EXISTS - can't use it, something is in the way! \n", recent_files[image_selected-2]);
                                    MessageBox(NULL, "ERROR: The user given file already exists and we need to do a conversion so that doesn't work. Please select a new file name. Thanks!", "Error", MB_OK | MB_ICONERROR);
                                    goto END_OF_GREASEWEAZLE_PROCESSING;

                                } else {

                                    fclose(probe_user_file_check);
                                    // probe_1 = 0; // File does NOT exist - we're clear to use it!
                                    printf("CHECKING THE USER FILE ----------> File: %s does NOT exist - we're clear to use it! \n", recent_files[image_selected-2]);
                                }

                                strncpy(temp_copy_path, recent_files[image_selected-2], sizeof(temp_copy_path) - 1); // Copy the current filename to the temp copy to be differently named filename.

                                temp_copy_path[sizeof(temp_copy_path) - 1] = '\0'; // Make sure last character is NULL for proper string termination. 

                                char *tdot = strrchr(temp_copy_path, '.'); // Pull out the file extension with the dot?

                                if (tdot) {
                                    // snprintf(tdot, sizeof(temp_copy_path) - (tdot - temp_copy_path), ".%s", gw_params[drive_map[brand_selected][drive_selected]].rename_target); //WTF?
                                    snprintf(tdot, sizeof(temp_copy_path) - (tdot - temp_copy_path), ".hdv"); // Okay I think this copies OVER the extension with the new .hdv extension.
                                };/*end-if*/

                                /* Don't overwrite an existing file - try _1, _2, ... if needed */
                                char temp_base[MAX_PATH + 16]; // Create a new temp var for doing stuff. 
                                strncpy(temp_base, temp_copy_path, sizeof(temp_base) - 1); // Copy shit to temp_base. 
                                temp_base[sizeof(temp_base) - 1] = '\0'; // Make sure string is gooder ending with NULL. 

                                char *tbase_dot = strrchr(temp_base, '.'); // Pointer to where the dot is in the string??? 
                                if (tbase_dot) *tbase_dot = '\0';  /* temp_base is now the path minus extension */

                                // Okay I think this copies OVER the extension with the new .hdv extension.
                                snprintf(temp_copy_path_target, sizeof(temp_copy_path_target), "%s.img", temp_base);              

                                // Create a string, where if the temp file is liek this: `Apple_IIGS_Farts_One.hdv` 
                                // then the target temp file for eventually 
                                // renaming this: `Apple_IIGS_Farts_One.hdv` to this: `Apple_IIGS_Farts_One.img`
                                
                                // I need to check for: Example: Some_File.HDV but also check for Some_File.IMG to make sure there isn't some other past conversion laying around to interfere with our temp files. 
                                int probe_1 = -1; // probe_2 --> 1 means that this file:temp_copy_path_target didn't work and a file like `Apple_IIGS_Farts_One.img` already exists. 
                                int probe_2 = -1; // probe_2 --> 0 means that this file:temp_copy_path_target doesn't exist and we can use it!

                                int suffix = 0; // create new var and initialize it to one. 

                                suffix++; // We are going to try the next filename that might not exist yet and is therefore valid to use!

                                snprintf(temp_copy_path,        sizeof(temp_copy_path),        "%s_%d.hdv", temp_base, suffix); // Create temp string like this: `Apple_IIGS_Farts_One_1.hdv` 
                                snprintf(temp_copy_path_target, sizeof(temp_copy_path_target), "%s_%d.img", temp_base, suffix); // Create temp string like this: `Apple_IIGS_Farts_One_1.img` 

                                // This runs at least once. The check at the top is to see if, in fact, any of the possible temp files already exist.
                                // So this tries creating new files until one works. That then we know what needs to become our new temp file. 
                                // If this doesn't work like this then you get a weird situation where it uses an OLD converted file
                                // and your looking at disk that isn't the latest version of what you expect! This was an annoying bug. 
                                while (1) { 

                                    //probe = fopen(temp_copy_path, "rb"); // Next file we try to create. 
                                    FILE *probe_1_check = fopen(temp_copy_path, "rb");
                                    if (probe_1_check != NULL) {
                                        fclose(probe_1_check);
                                        probe_1 = 1; // File EXISTS - can't use it, something is in the way!
                                        printf("CHECKING IMG FILE ----------> ERROR: File: %s EXISTS - can't use it, something is in the way! \n", temp_copy_path);
                                    } else {
                                        probe_1 = 0; // File does NOT exist - we're clear to use it!
                                        printf("CHECKING IMG FILE ----------> File: %s does NOT exist - we're clear to use it! \n", temp_copy_path);
                                    }
                                    
                                    // code for checking goes here first but also goes again at the end of the while loop.
                                    // Check if temp_copy_path_target (the .img file) already exists
                                    FILE *probe_2_check = fopen(temp_copy_path_target, "rb");
                                    if (probe_2_check != NULL) {
                                        fclose(probe_2_check);
                                        probe_2 = 1; // File EXISTS - can't use it, something is in the way!
                                        printf("ERROR: File EXISTS - can't use it, something is in the way! \n");
                                    } else {
                                        probe_2 = 0; // File does NOT exist - we're clear to use it!
                                        printf("File does NOT exist - we're clear to use it! \n");
                                    }

                                    printf("\n\nLOOP: suffix:%i --> Status of both probes into the files --> probe_1:%i - probe_2:%i \n\n",suffix,probe_1 ,probe_2);
                                    printf("temp_copy_path: %s \n\n", temp_copy_path);
                                    printf("temp_copy_path_target: %s \n\n", temp_copy_path_target);

                                    // This needs to happen here. We need to create the next strings for the files 
                                    // and then check if they are both okay to use. But they need to exist first
                                    // and we need to close the files as well. Then we can exit the loop!
                                    if (probe_1==0 && probe_2==0) {
                                        printf("Both files DO NOT EXIST! Huray!\n");
                                        break; // EXIT LOOP HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                                    } else {
                                        printf("One of the two files DO exist. Fuck...\n");
                                    };/*end-if*/

                                    suffix++; // We are going to try the next filename that might not exist yet and is therefore valid to use!

                                    snprintf(temp_copy_path,        sizeof(temp_copy_path),        "%s_%d.hdv", temp_base, suffix); // Create temp string like this: `Apple_IIGS_Farts_One_1.hdv` 
                                    snprintf(temp_copy_path_target, sizeof(temp_copy_path_target), "%s_%d.img", temp_base, suffix); // Create temp string like this: `Apple_IIGS_Farts_One_1.img` 

                                    if (suffix > 9999) { // Give up if there's like almost 10,000 fucking temp ass file here cause we are just cluttering things up at this point. 
                                        printf("ERROR: could not find a free filename for temp copy. \n");
                                        MessageBox(NULL, "ERROR: The program checked for over 9999 temporary file names and could not find a free filename the the temporary copy of the file for conversion.\nMaybe the issue is file permissions?", "Error", MB_OK | MB_ICONERROR);
                                        break;
                                    }

                                };/*end-while*/

                                printf("IS THIS A GOOD TEMP FILE??? --> temp_copy_path:        \"%s\" \n", temp_copy_path);
                                printf("IS THIS ALSO A GOOD FILE??? --> temp_copy_path_target: \"%s\" \n", temp_copy_path_target); // temp_copy_path_target  "%s_%d.img" `Apple_IIGS_Farts_One_1.img` 

                                // Build up the greaseweazle command string but using the converted files instead of the original files. 
                                snprintf(gw_cmd_string, sizeof(gw_cmd_string), 
                                    "\"%s\\%s\" %s --format=%s %s --densel %c --drive=%c --tracks=step=%c --retries %i \"%s\"", 
                                    options_greaseweazle_path , options_greaseweazle_executable ,
                                    (direction_selected == READING_A_FLOPPY) ? "read" : "write", 
                                    gw_params[ drive_map[brand_selected][drive_selected] ].format_string, 
                                    (direction_selected == READING_A_FLOPPY) ? (hw_gw_overwrite == 0) ? "--no-clobber" : "" : "", /* The "--no-clobber" option only works for reading to a file. */
                                    (hw_gw_density == 1) ? 'H' : 'L',
                                    (hw_gw_drive_type == 0) ? hw_gw_drive + '0' : hw_gw_drive + 'A',
                                    (hw_gw_steps == 1) ? '2' : '1',                                             /* This could be more intuitive but it works. hw_gw_steps==0 means "single step" (no extra steps) and hw_gw_steps==1 means "double step" (1 extra step). */
                                    hw_gw_retries,
                                    temp_copy_path_target 
                                );/*end-snprintf*/

                                printf("--> We made this for gw for reading:\n%s\n",gw_cmd_string);

                                // This is needed later when we convert the file AFTER Greaseweazle reads it in. 
                                snprintf(temp_gw_convert_fullpathfile, sizeof(temp_gw_convert_fullpathfile), "%s", temp_copy_path_target);
                                printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! --> temp_gw_convert_fullpathfile: %s \n", temp_gw_convert_fullpathfile);

                            };/*end-if*/

                            /********************************************************************************/
                            /**************** END OF CONVERT FILES IF NEEDED FOR READING ********************/
                            /********************************************************************************/

                            /********************************************************************************/
                            /************* SEND TO THE COMMAND LINE AND RUN THE DISK PROCESSING!  ***********/
                            /********************************************************************************/

                            if (passed_all_checks == TRUE) {

                                // TODO: 
                                // If a file needs to be "renamed" here is where it should happen.
                                // If the file is being made then it needs to be "renamed" afterward. 
                                // If the file is being used to write to a physical floppy disk then it's "renamed" before.
                                // This this case it's not really being "renamed" but a temp copy is being made
                                // with a temp filename and the file extension required by Greaseweazle. 

                                // TODO: 
                                // This is where the reset before processing should happen. 
                                // I've noticed that sometimes Greaseweazle gets into tha state that's funky. 
                                // When I reset it's all good. So that's why it's an option in hardware settings. 
                                // But maybe someone wants to turn it off so that's there. 
                                // This is where that needs to happen. Just before we do the actual processing. 
                                // It should open a window maybe and show that it's resetting so 
                                // you can see that it's working. Or... maybe you don't see it but it writes to a file 
                                // and the program reads that file to make sure the reset worked. 
                                // If the reset didn't work then it shouldn't proceed. Yeah that's the thing to do. 

                                // Setup all the things and then launch the process! Below are for reference purposes. TODO: Maybe make this an option? I dunno.
                                // CreateProcess(NULL, "cmd.exe /c echo Hello!", NULL, NULL, FALSE, CREATE_NO_WINDOW,   NULL, NULL, &si, &pi); // Does NOT create a command line window (but it runs),
                                // CreateProcess(NULL, "cmd.exe /c echo Hello!", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi); // Creates a command line windows and closes when done with /c after the cmd. 
                                // CreateProcess(NULL, "cmd.exe /k echo Hello!", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi); // Creates a command line windows and stays open when done with /k after the cmd. 
                                
                                // Add the beginning part! This let's us have things that user can change in Options like this: Close CLI When Complete: [ ] Enable
                                // snprintf(cmd_string, sizeof(cmd_string), "cmd.exe /k %s", gw_cmd_string);

                                if (    ( gw_params[drive_map[brand_selected][drive_selected]].rename_ext_req == TRUE ) &&      /* The current unique image format needs a file rename for gw to work. */
                                        (                                                  direction_selected == READING_A_FLOPPY )            /* floppy2file == reading a floppy and saving to a file */
                                   ){

                                    char rename_buf[MAX_PATH * 3];
                                    const char *rename_full_path = recent_files[image_selected-2];
                                    const char *rename_fname = strrchr(rename_full_path, '\\');
                                    rename_fname = rename_fname ? rename_fname + 1 : rename_full_path;  /* if no backslash found, the whole thing is the filename */
                                    char rename_dir[MAX_PATH];
                                    strncpy(rename_dir, rename_full_path, sizeof(rename_dir) - 1);
                                    rename_dir[sizeof(rename_dir) - 1] = '\0';
                                    char *last_slash = strrchr(rename_dir, '\\');
                                    if (last_slash) *(last_slash + 1) = '\0';  /* keep the trailing backslash */
                                    snprintf(rename_buf, sizeof(rename_buf), "%s | PATH: %s", rename_fname, rename_dir);

                                    // Okay so coming out of the above code we've got this: 
                                    // rename_dir   --> String that's the path to the file WITHOUT the final filename in it. 
                                    // rename_fname --> String that's JUST the filename WITHOUT the full path in it. 

                                    // Version where I echo the greaseweazle command string first.
                                    // snprintf(cmd_string, sizeof(cmd_string), "cmd.exe /k %s && echo %s && %s && echo rename \"%s\" \"%s\" && rename \"%s\" \"%s\"", gw_reset_string, gw_cmd_string, gw_cmd_string, temp_copy_path, rename_fname, temp_copy_path, rename_fname);

                                    /* gw reset enabled */
                                    if      (hw_gw_reset==1) snprintf(cmd_string, sizeof(cmd_string), "cmd.exe /k %s && echo %s && %s && echo rename \"%s\" \"%s\" && rename \"%s\" \"%s\"", gw_reset_string, gw_cmd_string, gw_cmd_string, temp_copy_path, rename_fname, temp_copy_path, rename_fname); 
                                    
                                    /* gw reset disabled */
                                    else if (hw_gw_reset==0) snprintf(cmd_string, sizeof(cmd_string), "cmd.exe /k echo %s && %s && echo rename \"%s\" \"%s\" && rename \"%s\" \"%s\"", gw_cmd_string, gw_cmd_string, temp_copy_path, rename_fname, temp_copy_path, rename_fname);

                                } else if ( ( gw_params[drive_map[brand_selected][drive_selected]].conversion_req == TRUE ) &&      /* The current unique image format needs a file rename for gw to work. */
                                            (                                   recent_files[image_selected-2][0] != '\0' ) &&      /* The currently selected filename is NOT empty. */
                                            (                                      direction_selected == WRITING_A_FLOPPY )         /* Only do this if we are reading a floppy. Not for writing. */
                                          ) {

                                    // For writing a disk image that needs to be converted, all the work is done beforehand, so everything is ready for the Greaseweazle run below.

                                    /* gw reset enabled */
                                    if      (hw_gw_reset==1) snprintf(cmd_string, sizeof(cmd_string), "cmd.exe /k %s && echo %s && %s", gw_reset_string, gw_cmd_string, gw_cmd_string);
                                    
                                    /* gw reset disabled */
                                    else if (hw_gw_reset==0) snprintf(cmd_string, sizeof(cmd_string), "cmd.exe /k echo %s && %s", gw_cmd_string, gw_cmd_string);

                                } else if ( ( gw_params[drive_map[brand_selected][drive_selected]].conversion_req == TRUE ) &&      /* The current unique image format needs a file rename for gw to work. */
                                            (                                   recent_files[image_selected-2][0] != '\0' ) &&      /* The currently selected filename is NOT empty. */
                                            (                                      direction_selected == READING_A_FLOPPY )         /* Only do this if we are reading a floppy. Not for writing. */
                                          ) {

                                    // For reading a disk image that needs to be converted, some work is done beforehand, then Greaseweazle runs, and then some finally work is done.

                                    // build the greaseweazle string, along with the two conversion steps added at the end.       /* %s\\%s options_ciderpress2cli_path, options_ciderpress2_cli_executable, */
                                    char convert_step_one_command[MAX_PATH * 6];
                                    char convert_step_two_command[MAX_PATH * 10];

                                    snprintf(convert_step_one_command, sizeof(convert_step_one_command), "\"%s\\%s\" create-disk-image \"%s\" 800k", options_ciderpress2cli_path, options_ciderpress2_cli_executable,recent_files[image_selected-2]);
                                    snprintf(convert_step_two_command, sizeof(convert_step_two_command), "\"%s\\%s\" copy-blocks \"%s\" \"%s\"", options_ciderpress2cli_path, options_ciderpress2_cli_executable, temp_gw_convert_fullpathfile , recent_files[image_selected-2]);

                                    printf("--> convert_step_one_command: %s \n", convert_step_one_command);
                                    printf("--> convert_step_two_command: %s \n", convert_step_two_command);
     
                                    /* gw reset enabled */
                                    if      (hw_gw_reset==1) snprintf(cmd_string, sizeof(cmd_string), "cmd.exe /k %s && echo %s && %s && %s && %s", gw_reset_string, gw_cmd_string, gw_cmd_string, convert_step_one_command, convert_step_two_command);
                                    
                                    /* gw reset disabled */
                                    else if (hw_gw_reset==0) snprintf(cmd_string, sizeof(cmd_string), "cmd.exe /k echo %s && %s && %s && %s", gw_cmd_string, gw_cmd_string, convert_step_one_command, convert_step_two_command);

                                } else {

                                    // Version where there is no greaseweazle command string echoing.
                                    // snprintf(cmd_string, sizeof(cmd_string), "cmd.exe /k %s && echo %s && %s", gw_reset_string, gw_cmd_string, gw_cmd_string);

                                    /* gw reset enabled */
                                    if      (hw_gw_reset==1) snprintf(cmd_string, sizeof(cmd_string), "cmd.exe /k %s && echo %s && %s", gw_reset_string, gw_cmd_string, gw_cmd_string);
                                    
                                    /* gw reset disabled */
                                    else if (hw_gw_reset==0) snprintf(cmd_string, sizeof(cmd_string), "cmd.exe /k echo %s && %s", gw_cmd_string, gw_cmd_string);
                                    
                                };/*end-if*/

                                STARTUPINFO si = {0};
                                PROCESS_INFORMATION pi = {0};
                                si.cb = sizeof(si);
                                CreateProcess(NULL, cmd_string, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi); // Creates a command line windows and stays open when done with /k after the cmd. 
                                CloseHandle(pi.hProcess);
                                CloseHandle(pi.hThread);

                            };/*end-if*/

                            /********************************************************************************/
                            /********************* CONVERT FILES IF NEEDED FOR READING **********************/
                            /********************************************************************************/

                            /********************************************************************************/
                            /**************** END OF CONVERT FILES IF NEEDED FOR READING ********************/
                            /********************************************************************************/

                            END_OF_GREASEWEAZLE_PROCESSING:;

                            printf("END_OF_GREASEWEAZLE_PROCESSING:;\n");

                        /********************************************************************************/
                        /***************************** ZOOMFLOPPY PROCESSING ****************************/
                        /********************************************************************************/
                        } else if (hardware_selected == ZOOMFLOPPY_CHOSEN) {

                            printf("ZoomFloppy processing chosen.\n");                    

                            /********************************************************************************/
                            /***************************** CONFIGURATION TESTING ****************************/
                            /********************************************************************************/

                            passed_all_checks = TRUE; // We want to run a bunch of checks and if any fail they set this to FALSE and we know not try and run the command on the command line. 

                            // Check if the options have set valid strings for the Greaseweazle path and executable.
                            // If not, skip everything else. 
                            if ( !is_string_valid(options_opencbm_path)               || 
                                 !is_string_valid(options_opencbm_cbmctrl_executable) ||
                                 !is_string_valid(options_opencbm_d64copy_executable) ||
                                 !is_string_valid(options_opencbm_d82copy_executable) ||
                                 !is_string_valid(options_opencbm_imgcopy_executable)    ){

                                printf("One of these strings: options_opencbm_path or options_opencbm_path is empty!\n");
                                MessageBox(NULL, "The OpenCBM (ZoomFloppy) options for path or executable are not set.\nPlease check the settings in Options.", "Error", MB_OK | MB_ICONERROR);
                                goto END_OF_ZOOMFLOPPY_PROCESSING; // Stop checking. Just dip-out right outta here!

                            };/*end-if*/

                            passed_all_checks = verify_zoomfloppy_drive(hw_zf_iec_device, current_opencbm_drive_selected);
                            // TODO: This is where we should jump over all this shit. I know... goto is the devil. 
                            //       Well... Dijkstra can suck a duck! This is how we roll. 

                            // IS A FILE SELECTED? 
                            // FILE EXISTS --> Filename must not be empty.
                            // The image dropdown has "No file set..." at index 0 and "Browse..." at index 1.
                            // Actual files start at index 2, which is why we use (image_selected - 2) to index into recent_files[].
                            // So if image_selected is less than 2, the user hasn't actually picked a file yet and we bail out here
                            // rather than letting (image_selected - 2) produce a negative array index and crash the program.
                            // if ( recent_files[image_selected-2][0] == '\0' ) { 
                            if (image_selected < 2) {

                                MessageBox(NULL, "You don't appear to have chosen a file!", "Error", MB_OK | MB_ICONERROR);
                                passed_all_checks = FALSE;

                            } else { // TODO: THIS IS NOT A GOOD CHECK! NEED A BETTER CHECK HERE FOR WHETHER OR NOT A FILE IS GONNA CRASH THE PROGRAM! For example "something.file" would be okay but like "something." might crash the program.
                                // CHECK FILE EXTENSION AGAINST THE FORMAT'S EXPECTED EXTENSION

                                // Debugging yeah.
                                printf("dot+1:%s | .file_ext:%s \n", dot+1, main_formats[drive_map[brand_selected][drive_selected]].file_ext);
                                
                                if (dot && strcasecmp(main_formats[drive_map[brand_selected][drive_selected]].file_ext, dot+1)) {

                                        MessageBox(NULL, "The file extension of your file doesn't match the selected disk image format.\nPlease check your file selection.", "Error", MB_OK | MB_ICONERROR);
                                        passed_all_checks = FALSE;

                                };/*end-if*/
                            };/*end-if*/

                            // If at this point we don't have hardware and/or we don't have a file selected,
                            // then we need to just dip-out right here!
                            if ( passed_all_checks == FALSE ) goto END_OF_ZOOMFLOPPY_PROCESSING;

                            switch (main_formats[drive_map[brand_selected][drive_selected]].format_id) { 

                                case cbm1541_cdos_525_ssdd_0171k_d64 :
                                    if ( current_opencbm_drive_selected == CBM_1541   ||
                                         current_opencbm_drive_selected == CBM_1541II ||
                                         current_opencbm_drive_selected == CBM_1570   ||
                                         current_opencbm_drive_selected == CBM_1571   ||
                                         current_opencbm_drive_selected == CBM_4040   ||   
                                         current_opencbm_drive_selected == CBM_2031   ){
                                            // passed_all_checks = TRUE;
                                            printf("D64 on CBM_1541, CBM_1570, CBM_1571, CBM_4040 OR CBM_2031!\n");
                                    } else {
                                            MessageBox(NULL, "That disk cannot be made on that ZoomFloppy / OpenCBM / IEC drive.\nSorry Bud!", "Error", MB_OK | MB_ICONERROR);
                                            passed_all_checks = FALSE;
                                    };/*end-if*/
                                break;

                                case cbm1571_cdos_525_dsdd_0342k_d71 :
                                    if ( current_opencbm_drive_selected == CBM_1571 ){
                                            // passed_all_checks = TRUE;
                                            printf("D71 on CBM_1571");
                                    } else {
                                            MessageBox(NULL, "That disk cannot be made on that ZoomFloppy / OpenCBM / IEC drive.\nSorry Bud!", "Error", MB_OK | MB_ICONERROR);
                                            passed_all_checks = FALSE;
                                    };/*end-if*/
                                break;

                                case cbm1581_cdos_350_dsdd_0800k_d81 :
                                    if ( current_opencbm_drive_selected == CBM_1581 ){
                                            // passed_all_checks = TRUE;
                                            printf("D81 on CBM_1581");
                                    } else {
                                            MessageBox(NULL, "That disk cannot be made on that ZoomFloppy / OpenCBM / IEC drive.\nSorry Bud!", "Error", MB_OK | MB_ICONERROR);
                                            passed_all_checks = FALSE;
                                    };/*end-if*/
                                break;

                                case cbm8050_cdos_525_dsqd_0521k_d80 :
                                    if ( current_opencbm_drive_selected == SFD_1001 ||
                                         current_opencbm_drive_selected == CBM_8050 ||
                                         current_opencbm_drive_selected == CBM_8250 ){
                                            // passed_all_checks = TRUE;
                                            printf("D80 on CBM_8050, CBM_8250, SFD_1001");
                                    } else {
                                            MessageBox(NULL, "That disk cannot be made on that ZoomFloppy / OpenCBM / IEC drive.\nSorry Bud!", "Error", MB_OK | MB_ICONERROR);
                                            passed_all_checks = FALSE;
                                    };/*end-if*/
                                break;

                                case cbm8250_cdos_525_dsqd_1042k_d82 :
                                case cbmsfd1_cdos_525_dsqd_1042k_d82 :
                                    if ( current_opencbm_drive_selected == SFD_1001 ||
                                         current_opencbm_drive_selected == CBM_8250 ){
                                            // passed_all_checks = TRUE;
                                            printf("D82 on CBM_8250, SFD_1001");
                                    } else {
                                            MessageBox(NULL, "That disk cannot be made on that ZoomFloppy / OpenCBM / IEC drive.\nSorry Bud!", "Error", MB_OK | MB_ICONERROR);
                                            passed_all_checks = FALSE;
                                    };/*end-if*/
                                break;

                            };/*end-switch*/

                            // SUPPORTED --> zoomfloppy must support this type of disk image 
                            if (main_formats[ drive_map[brand_selected][drive_selected] ].zoomfloppy_supported == UNSUPPORTED ) {
                                MessageBox(NULL, "Sorry but ZoomFloppy doesn't support this format!", "Error", MB_OK | MB_ICONERROR);
                                passed_all_checks = FALSE;
                            };/*end-if*/

                            printf("opencbm_drive_options[current_opencbm_drive_selected]:%s \n", opencbm_drive_options[current_opencbm_drive_selected]);

                            /********************************************************************************/
                            /***************************** COMMAND LINE BUILDING ****************************/
                            /********************************************************************************/

                            if ( passed_all_checks == TRUE ) {

                                switch(current_opencbm_drive_selected){

                                    case CBM_4040 :

                                        printf("CBM_4040 chosen.\n"); 
                                        
                                        if ( direction_selected == WRITING_A_FLOPPY ) { /* file2floppy --> Loading a file and writing to a floppy. */

                                            snprintf(zf_cmd_string, sizeof(zf_cmd_string),                       
                                                /* d64copy -d 4040 -t original test.d64 8 */
                                                "\"%s\\%s\" -d 4040 -t original %c%c \"%s\" %i", 
                                                options_opencbm_path, options_opencbm_d64copy_executable,
                                                (hw_zf_bam_only == FALSE) ? ' ' : '-', /* BAM-Only option. */
                                                (hw_zf_bam_only == FALSE) ? ' ' : 'b', /* BAM-Only value.  */
                                                recent_files[image_selected-2],      /* File to write to IEC device. */
                                                hw_zf_iec_device                       /* IEC device to receive disk image. */
                                            );/*end-snprintf*/

                                        } else if ( direction_selected == READING_A_FLOPPY ) { /* floppy2file --> Reading a floppy and saving to a file. */

                                            snprintf(zf_cmd_string, sizeof(zf_cmd_string), 
                                                "\"%s\\%s\" -d 4040 -t original %c%c %i \"%s\"", 
                                                options_opencbm_path, options_opencbm_d64copy_executable,
                                                (hw_zf_bam_only == FALSE) ? ' ' : '-', /* BAM-Only option. */
                                                (hw_zf_bam_only == FALSE) ? ' ' : 'b', /* BAM-Only value.  */
                                                hw_zf_iec_device,                      /* IEC device to receive disk image. */
                                                recent_files[image_selected-2]       /* File to write to IEC device. */
                                            );/*end-snprintf*/
                    
                                        };/*end-if*/

                                    break;/*end-case*/

                                    case CBM_2031 :

                                        printf("CBM_2031 chosen.\n"); 
                                        
                                        if ( direction_selected == WRITING_A_FLOPPY ) { /* file2floppy --> Loading a file and writing to a floppy. */

                                            snprintf(zf_cmd_string, sizeof(zf_cmd_string), 
                                                /* d64copy -d 2031 -t original test.d64 8 */
                                                "\"%s\\%s\" -d 2031 -t original %c%c \"%s\" %i", 
                                                options_opencbm_path, options_opencbm_d64copy_executable,
                                                (hw_zf_bam_only == FALSE) ? ' ' : '-', /* BAM-Only option. */
                                                (hw_zf_bam_only == FALSE) ? ' ' : 'b', /* BAM-Only value.  */
                                                recent_files[image_selected-2],      /* File to write to IEC device. */
                                                hw_zf_iec_device                       /* IEC device to receive disk image. */
                                            );/*end-snprintf*/

                                        } else if ( direction_selected == READING_A_FLOPPY ) { /* floppy2file --> Reading a floppy and saving to a file. */

                                            snprintf(zf_cmd_string, sizeof(zf_cmd_string), 
                                                "\"%s\\%s\" -d 2031 -t original %c%c %i \"%s\"", 
                                                options_opencbm_path, options_opencbm_d64copy_executable,
                                                (hw_zf_bam_only == FALSE) ? ' ' : '-', /* BAM-Only option. */
                                                (hw_zf_bam_only == FALSE) ? ' ' : 'b', /* BAM-Only value.  */
                                                hw_zf_iec_device,                      /* IEC device to receive disk image. */
                                                recent_files[image_selected-2]       /* File to write to IEC device. */
                                            );/*end-snprintf*/

                                        };/*end-if*/

                                    break;/*end-case*/

                                    case CBM_1541   :
                                    case CBM_1541II :
                                        printf("CBM_1541/CBM_1541II chosen.\n");  
                                        // Create the command line string to run. 
                                        
                                        if ( direction_selected == WRITING_A_FLOPPY ) { /* file2floppy --> Loading a file and writing to a floppy. */

                                            snprintf(zf_cmd_string, sizeof(zf_cmd_string), 
                                                /* d64copy -b .\1541-read-test-01.d64 9 */
                                                "\"%s\\%s\" %c%c \"%s\" %i", 
                                                options_opencbm_path, options_opencbm_d64copy_executable,
                                                (hw_zf_bam_only == FALSE) ? ' ' : '-', /* BAM-Only option. */
                                                (hw_zf_bam_only == FALSE) ? ' ' : 'b', /* BAM-Only value.  */
                                                recent_files[image_selected-2],      /* File to write to IEC device. */
                                                hw_zf_iec_device                       /* IEC device to receive disk image. */
                                            );/*end-snprintf*/

                                        } else if ( direction_selected == READING_A_FLOPPY ) { /* floppy2file --> Reading a floppy and saving to a file. */

                                            snprintf(zf_cmd_string, sizeof(zf_cmd_string), 
                                                /* d64copy -b 9 .\1541-read-test-01.d64 */
                                                "\"%s\\%s\" %c%c %i \"%s\"", 
                                                options_opencbm_path, options_opencbm_d64copy_executable,
                                                (hw_zf_bam_only == FALSE) ? ' ' : '-', /* BAM-Only option. */
                                                (hw_zf_bam_only == FALSE) ? ' ' : 'b', /* BAM-Only value.  */
                                                hw_zf_iec_device,                      /* IEC device to receive disk image. */
                                                recent_files[image_selected-2]       /* File to write to IEC device. */
                                            );/*end-snprintf*/

                                        };/*end-if*/

                                        printf("zf_cmd_string:%s \n", zf_cmd_string);
                                    break;/*end-case*/

                                    case CBM_1570 :
                                        printf("CBM_1570 chosen.\n"); 

                                        // Create the command line string to run.             
                                        
                                        if ( direction_selected == WRITING_A_FLOPPY ) { /* file2floppy --> Loading a file and writing to a floppy. */

                                            snprintf(zf_cmd_string, sizeof(zf_cmd_string), 
                                                "\"%s\\%s\" --drive-type=1570 %c%c \"%s\" %i", 
                                                options_opencbm_path, options_opencbm_d64copy_executable,
                                                (hw_zf_bam_only == FALSE) ? ' ' : '-', /* BAM-Only option. */
                                                (hw_zf_bam_only == FALSE) ? ' ' : 'b', /* BAM-Only value.  */
                                                recent_files[image_selected-2],      /* File to write to IEC device. */
                                                hw_zf_iec_device                       /* IEC device to receive disk image. */
                                            );/*end-snprintf*/

                                        } else if ( direction_selected == READING_A_FLOPPY ) { /* floppy2file --> Reading a floppy and saving to a file. */

                                            snprintf(zf_cmd_string, sizeof(zf_cmd_string), 
                                                "\"%s\\%s\" --drive-type=1570 %c%c %i \"%s\"", 
                                                options_opencbm_path, options_opencbm_d64copy_executable,
                                                (hw_zf_bam_only == FALSE) ? ' ' : '-', /* BAM-Only option. */
                                                (hw_zf_bam_only == FALSE) ? ' ' : 'b', /* BAM-Only value.  */
                                                hw_zf_iec_device,                      /* IEC device to receive disk image. */
                                                recent_files[image_selected-2]       /* File to write to IEC device. */
                                            );/*end-snprintf*/

                                        };/*end-if*/
                                                
                                        printf("zf_cmd_string:%s \n", zf_cmd_string);     

                                    break;/*end-case*/

                                    case CBM_1571 :
                                        printf("CBM_1571 chosen.\n"); 

                                        // Note: This is super messy. I'm sure there's a better way!

                                        static char set_sided_mode[MAX_PATH * 5] = "";

                                        /* If it's a 1571 format then put the drive into double-sided mode (1571 mode) using this: cbmctrl pcommand 8 "u0>m1" */
                                        if ( main_formats[drive_map[brand_selected][drive_selected]].format_id == cbm1571_cdos_525_dsdd_0342k_d71 ) {

                                            snprintf(set_sided_mode,sizeof(set_sided_mode),"\"%s\\%s\" pcommand %i \"u0>m1\" && \"%s\\%s\" --two-sided ",options_opencbm_path, options_opencbm_cbmctrl_executable, hw_zf_iec_device, options_opencbm_path, options_opencbm_d64copy_executable); 

                                        /* If it's a 1541 format then put the drive into single-sided mode (1541 emulation mode) using this: cbmctrl pcommand 8 "u0>m0" */
                                        } else if ( main_formats[drive_map[brand_selected][drive_selected]].format_id == cbm1541_cdos_525_ssdd_0171k_d64 ) {

                                            snprintf(set_sided_mode,sizeof(set_sided_mode),"\"%s\\%s\" pcommand %i \"u0>m0\" && \"%s\\%s\" --two-sided ",options_opencbm_path, options_opencbm_cbmctrl_executable, hw_zf_iec_device, options_opencbm_path, options_opencbm_d64copy_executable); 

                                        };/*end-if*/

                                        if ( direction_selected == WRITING_A_FLOPPY ) { /* file2floppy --> Loading a file and writing to a floppy. */
                                            
                                            snprintf(zf_cmd_string, sizeof(zf_cmd_string), 
                                                /* d64copy -b .\1541-read-test-01.d64 9 */
                                                "%s --drive-type=1571 %c%c \"%s\" %i", 
                                                set_sided_mode,                        /* Start of string from above that puts the 1571 into the right mode. */
                                                (hw_zf_bam_only == FALSE) ? ' ' : '-', /* BAM-Only option. */
                                                (hw_zf_bam_only == FALSE) ? ' ' : 'b', /* BAM-Only value.  */
                                                recent_files[image_selected-2],        /* File to write to IEC device. */
                                                hw_zf_iec_device                       /* IEC device to receive disk image. */
                                            );/*end-snprintf*/

                                        } else if ( direction_selected == READING_A_FLOPPY ) { /* floppy2file --> Reading a floppy and saving to a file. */
                                            
                                            snprintf(zf_cmd_string, sizeof(zf_cmd_string), 
                                                /* d64copy -b 9 .\1541-read-test-01.d64 */
                                                "%s --drive-type=1571 %c%c %i \"%s\"", 
                                                set_sided_mode,                        /* Start of string from above that puts the 1571 into the right mode. */
                                                (hw_zf_bam_only == FALSE) ? ' ' : '-', /* BAM-Only option. */
                                                (hw_zf_bam_only == FALSE) ? ' ' : 'b', /* BAM-Only value.  */
                                                hw_zf_iec_device,                      /* IEC device to receive disk image. */
                                                recent_files[image_selected-2]         /* File to write to IEC device. */
                                            );/*end-snprintf*/

                                        };/*end-if*/
                                                
                                        printf("zf_cmd_string:%s \n", zf_cmd_string);     

                                    break;/*end-case*/

                                    case CBM_1581 :

                                        printf("CBM_1581 chosen.\n");    

                                        // Create the command line string to run. 
                                        
                                        if ( direction_selected == WRITING_A_FLOPPY ) { /* file2floppy --> Loading a file and writing to a floppy. */

                                            snprintf(zf_cmd_string, sizeof(zf_cmd_string), 
                                                "\"%s\\%s\" -d 1581 --no-warp -t original %c%c \"%s\" %i", 
                                                options_opencbm_path, options_opencbm_imgcopy_executable,
                                                (hw_zf_bam_only == FALSE) ? ' ' : '-', /* BAM-Only option. */
                                                (hw_zf_bam_only == FALSE) ? ' ' : 'b', /* BAM-Only value.  */
                                                recent_files[image_selected-2],        /* File to write to IEC device. */
                                                hw_zf_iec_device                       /* IEC device to receive disk image. */
                                            );/*end-snprintf*/

                                        } else if ( direction_selected == READING_A_FLOPPY ) { /* floppy2file --> Reading a floppy and saving to a file. */

                                            snprintf(zf_cmd_string, sizeof(zf_cmd_string), 
                                                "\"%s\\%s\" -d 1581 --no-warp -t original %c%c %i \"%s\"", 
                                                options_opencbm_path, options_opencbm_imgcopy_executable,
                                                (hw_zf_bam_only == FALSE) ? ' ' : '-', /* BAM-Only option. */
                                                (hw_zf_bam_only == FALSE) ? ' ' : 'b', /* BAM-Only value.  */
                                                hw_zf_iec_device,                      /* IEC device to receive disk image. */
                                                recent_files[image_selected-2]         /* File to write to IEC device. */
                                            );/*end-snprintf*/

                                        };/*end-if*/
                                                
                                        printf("zf_cmd_string:%s \n", zf_cmd_string);   

                                    break;/*end-case*/

                                    case CBM_8050 : // TODO: How does ZoomFloppy & OpenCBM determine which of the two physical drives to address on this disk drive unit? 

                                        printf("CBM_8050 chosen.\n");    

                                        if ( direction_selected == WRITING_A_FLOPPY ) { /* file2floppy --> Loading a file and writing to a floppy. */

                                            snprintf(zf_cmd_string, sizeof(zf_cmd_string), 
                                                "\"%s\\%s\" --one-sided %c%c \"%s\" %i", 
                                                options_opencbm_path, options_opencbm_d82copy_executable,
                                                (hw_zf_bam_only == FALSE) ? ' ' : '-', /* BAM-Only option. */
                                                (hw_zf_bam_only == FALSE) ? ' ' : 'b', /* BAM-Only value.  */
                                                recent_files[image_selected-2],        /* File to write to IEC device. */
                                                hw_zf_iec_device                       /* IEC device to receive disk image. */
                                            );/*end-snprintf*/

                                        } else if ( direction_selected == READING_A_FLOPPY ) { /* floppy2file --> Reading a floppy and saving to a file. */

                                            snprintf(zf_cmd_string, sizeof(zf_cmd_string), 
                                                // "d82copy --one-sided %c%c %i \"%s\"", 
                                                "\"%s\\%s\" --one-sided %c%c %i \"%s\"",
                                                options_opencbm_path, options_opencbm_d82copy_executable,
                                                (hw_zf_bam_only == FALSE) ? ' ' : '-', /* BAM-Only option. */
                                                (hw_zf_bam_only == FALSE) ? ' ' : 'b', /* BAM-Only value.  */
                                                hw_zf_iec_device,                      /* IEC device to receive disk image. */
                                                recent_files[image_selected-2]         /* File to write to IEC device. */
                                            );/*end-snprintf*/

                                        };/*end-if*/                      
                                                
                                        printf("zf_cmd_string:%s \n", zf_cmd_string);   

                                    break;/*end-case*/

                                    case CBM_8250 : // TODO: How does ZoomFloppy & OpenCBM determine which of the two physical drives to address on this disk drive unit? 

                                        printf("CBM_8250 chosen.\n");     

                                        if ( direction_selected == WRITING_A_FLOPPY ) { /* file2floppy --> Loading a file and writing to a floppy. */

                                            snprintf(zf_cmd_string, sizeof(zf_cmd_string), 
                                                "\"%s\\%s\" --two-sided %c%c \"%s\" %i", 
                                                options_opencbm_path, options_opencbm_d82copy_executable,
                                                (hw_zf_bam_only == FALSE) ? ' ' : '-', /* BAM-Only option. */
                                                (hw_zf_bam_only == FALSE) ? ' ' : 'b', /* BAM-Only value.  */
                                                recent_files[image_selected-2],        /* File to write to IEC device. */
                                                hw_zf_iec_device                       /* IEC device to receive disk image. */
                                            );/*end-snprintf*/

                                        } else if ( direction_selected == READING_A_FLOPPY ) { /* floppy2file --> Reading a floppy and saving to a file. */

                                            snprintf(zf_cmd_string, sizeof(zf_cmd_string), 
                                                "\"%s\\%s\" --two-sided %c%c %i \"%s\"", 
                                                options_opencbm_path, options_opencbm_d82copy_executable,
                                                (hw_zf_bam_only == FALSE) ? ' ' : '-', /* BAM-Only option. */
                                                (hw_zf_bam_only == FALSE) ? ' ' : 'b', /* BAM-Only value.  */
                                                hw_zf_iec_device,                      /* IEC device to receive disk image. */
                                                recent_files[image_selected-2]         /* File to write to IEC device. */
                                            );/*end-snprintf*/

                                        };/*end-if*/    
                                                
                                        printf("zf_cmd_string:%s \n", zf_cmd_string);  

                                    break;/*end-case*/

                                    case SFD_1001 : // TODO: How does ZoomFloppy & OpenCBM determine which of the two physical drives to address on this disk drive unit? 

                                        printf("SFD_1001 chosen.\n");    

                                        if ( direction_selected == WRITING_A_FLOPPY ) { /* file2floppy --> Loading a file and writing to a floppy. */

                                            snprintf(zf_cmd_string, sizeof(zf_cmd_string), 
                                                "\"%s\\%s\" --two-sided %c%c \"%s\" %i", 
                                                options_opencbm_path, options_opencbm_d82copy_executable,
                                                (hw_zf_bam_only == FALSE) ? ' ' : '-', /* BAM-Only option. */
                                                (hw_zf_bam_only == FALSE) ? ' ' : 'b', /* BAM-Only value.  */
                                                recent_files[image_selected-2],        /* File to write to IEC device. */
                                                hw_zf_iec_device                       /* IEC device to receive disk image. */
                                            );/*end-snprintf*/

                                        } else if ( direction_selected == READING_A_FLOPPY ) { /* floppy2file --> Reading a floppy and saving to a file. */

                                            snprintf(zf_cmd_string, sizeof(zf_cmd_string), 
                                                "\"%s\\%s\" --two-sided %c%c %i \"%s\"", 
                                                options_opencbm_path, options_opencbm_d82copy_executable,
                                                (hw_zf_bam_only == FALSE) ? ' ' : '-', /* BAM-Only option. */
                                                (hw_zf_bam_only == FALSE) ? ' ' : 'b', /* BAM-Only value.  */
                                                hw_zf_iec_device,                      /* IEC device to receive disk image. */
                                                recent_files[image_selected-2]         /* File to write to IEC device. */
                                            );/*end-snprintf*/

                                        };/*end-if*/     
                                                
                                        printf("zf_cmd_string:%s \n", zf_cmd_string);  

                                    break;/*end-case*/

                                };/*end-switch*/

                                /********************************************************************************/
                                /************* SEND TO THE COMMAND LINE AND RUN THE DISK PROCESSING!  ***********/
                                /********************************************************************************/

                                if      ( hw_zf_reset == TRUE )  snprintf(zf_reset_string,sizeof(zf_reset_string),"echo \"\"%s\\%s\" reset\" && \"%s\\%s\" reset", options_opencbm_path, options_opencbm_cbmctrl_executable, options_opencbm_path, options_opencbm_cbmctrl_executable);
                                else if ( hw_zf_reset == FALSE ) strcpy(zf_reset_string,"echo \"OpenCBM:cbmctrl reset disabled\"");
                                /*end-if*/

                                if ( hw_zf_format_first == TRUE ) {
                                    
                                    // If it's a 1571 drive and a 1571 format then put the drive into double-sided mode (1571 mode) using this: cbmctrl pcommand 8 "u0>m1"

                                    if (                                    current_opencbm_drive_selected == CBM_1571 &&
                                         main_formats[drive_map[brand_selected][drive_selected]].format_id == cbm1571_cdos_525_dsdd_0342k_d71 ){                    

                                        // snprintf(zf_format_string, sizeof(zf_format_string),"echo \"Formatting disk in device %i. Please wait...\" && cbmctrl pcommand %i \"u0>m1\" && cbmctrl command %i \"N:empty,00\" && cbmctrl detect && echo \"Formatting complete!\"", hw_zf_iec_device, hw_zf_iec_device, hw_zf_iec_device);
                                        snprintf(
                                            zf_format_string, sizeof(zf_format_string),
                                            "echo \"Formatting disk in device %i. Please wait...\" && \"%s\\%s\" pcommand %i \"u0>m1\" && \"%s\\%s\" command %i \"N:empty,00\" && \"%s\\%s\" detect && echo \"Formatting complete!\"", 
                                            hw_zf_iec_device, 
                                            options_opencbm_path, options_opencbm_cbmctrl_executable,
                                            hw_zf_iec_device, 
                                            options_opencbm_path, options_opencbm_cbmctrl_executable,
                                            hw_zf_iec_device,
                                            options_opencbm_path, options_opencbm_cbmctrl_executable
                                        );/*end-snprintf*/

                                    // If it's a 1571 drive and a 1541 format then put the drive into double-sided mode (1541 emulation mode) using this: cbmctrl pcommand 8 "u0>m0"

                                    } else if (                                    current_opencbm_drive_selected == CBM_1571 &&
                                                main_formats[drive_map[brand_selected][drive_selected]].format_id == cbm1541_cdos_525_ssdd_0171k_d64 ){

                                            // snprintf(zf_format_string, sizeof(zf_format_string),"echo \"Formatting disk in device %i. Please wait...\" && cbmctrl pcommand %i \"u0>m0\" && cbmctrl command %i \"N:empty,00\" && cbmctrl detect && echo \"Formatting complete!\"", hw_zf_iec_device, hw_zf_iec_device, hw_zf_iec_device);
                                            snprintf(
                                                zf_format_string, sizeof(zf_format_string),
                                                "echo \"Formatting disk in device %i. Please wait...\" && \"%s\\%s\" pcommand %i \"u0>m0\" && \"%s\\%s\" command %i \"N:empty,00\" && \"%s\\%s\" detect && echo \"Formatting complete!\"", 
                                                hw_zf_iec_device, 
                                                options_opencbm_path, options_opencbm_cbmctrl_executable,
                                                hw_zf_iec_device, 
                                                options_opencbm_path, options_opencbm_cbmctrl_executable,
                                                hw_zf_iec_device,
                                                options_opencbm_path, options_opencbm_cbmctrl_executable
                                            );

                                    } else {

                                            // snprintf(zf_format_string, sizeof(zf_format_string),"echo \"Formatting disk in device %i. Please wait...\" && cbmctrl command %i \"N:empty,00\" && cbmctrl detect && echo \"Formatting complete!\"", hw_zf_iec_device, hw_zf_iec_device);
                                            snprintf(
                                                zf_format_string, sizeof(zf_format_string),
                                                "echo \"Formatting disk in device %i. Please wait...\" && \"%s\\%s\" command %i \"N:empty,00\" && \"%s\\%s\" detect && echo \"Formatting complete!\"", 
                                                hw_zf_iec_device, 
                                                options_opencbm_path, options_opencbm_cbmctrl_executable,
                                                hw_zf_iec_device, 
                                                options_opencbm_path, options_opencbm_cbmctrl_executable
                                            );

                                    };/*end-if*/

                                } else if ( hw_zf_format_first == FALSE ) {

                                    strcpy(zf_format_string,"echo \"format first disabled\"");

                                };/*end-if*/

                                snprintf(cmd_string, sizeof(cmd_string), "cmd.exe /k %s && %s && %s", zf_reset_string, zf_format_string, zf_cmd_string);

                                STARTUPINFO si = {0};
                                PROCESS_INFORMATION pi = {0};
                                si.cb = sizeof(si);
                                CreateProcess(NULL, cmd_string, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi); // Creates a command line windows and stays open when done with /k after the cmd. 
                                CloseHandle(pi.hProcess);
                                CloseHandle(pi.hThread);

                            };/*end-if*/       
                            
                            END_OF_ZOOMFLOPPY_PROCESSING:;

                        /********************************************************************************/
                        /***************************** USB FLOPPY PROCESSING ****************************/
                        /********************************************************************************/
                        } else if (hardware_selected == USBFLOPPY_CHOSEN) {

                            printf("USB Floppy processing chosen.\n");

                            /********************************************************************************/
                            /***************************** CONFIGURATION TESTING ****************************/
                            /********************************************************************************/

                            passed_all_checks = TRUE; // We want to run a bunch of checks and if any fail they set this to FALSE and we know not try and run the command on the command line. 

                            // Check if the options have set valid strings for the Greaseweazle path and executable.
                            // If not, skip everything else. 
                            if ( !is_string_valid(options_dd_path) || !is_string_valid(options_dd_executable) ) {
                                printf("One of these strings: options_dd_path or options_dd_executable is empty!\n");
                                MessageBox(NULL, "The DD options for path or executable are not set.\nPlease check the Options.", "Error", MB_OK | MB_ICONERROR);
                                goto END_OF_USBFLOPPY_PROCESSING; // Stop checking. Just dip-out right outta here!
                            };/*end-if*/

                            passed_all_checks = verify_usbfloppy_drive( hw_usb_drive_letter[0] );

                            printf("after verify_usbfloppy_drive passed_all_checks:%i\n",passed_all_checks);

                            // IS A FILE SELECTED? 
                            // FILE EXISTS --> Filename must not be empty.
                            // The image dropdown has "No file set..." at index 0 and "Browse..." at index 1.
                            // Actual files start at index 2, which is why we use (image_selected - 2) to index into recent_files[].
                            // So if image_selected is less than 2, the user hasn't actually picked a file yet and we bail out here
                            // rather than letting (image_selected - 2) produce a negative array index and crash the program.

                            if (image_selected < 2) {

                                MessageBox(NULL, "You don't appear to have chosen a file!", "Error", MB_OK | MB_ICONERROR);
                                passed_all_checks = FALSE;

                            } else { // TODO: THIS IS NOT A GOOD CHECK! NEED A BETTER CHECK HERE FOR WHETHER OR NOT A FILE IS GONNA CRASH THE PROGRAM! For example "something.file" would be okay but like "something." might crash the program.
                                
                                // CHECK FILE EXTENSION AGAINST THE FORMAT'S EXPECTED EXTENSION

                                // Debugging yeah.
                                printf("dot+1:%s | .file_ext:%s \n", dot+1, main_formats[drive_map[brand_selected][drive_selected]].file_ext);
                                
                                if (dot && strcasecmp(main_formats[drive_map[brand_selected][drive_selected]].file_ext, dot+1)) {

                                        MessageBox(NULL, "The file extension of your file doesn't match the selected disk image format.\nPlease check your file selection.", "Error", MB_OK | MB_ICONERROR);
                                        passed_all_checks = FALSE;

                                };/*end-if*/

                            };/*end-if*/

                            // If at this point we don't have hardware and/or we don't have a file selected,
                            // then we need to just dip-out right here!
                            if ( passed_all_checks == FALSE ) goto END_OF_USBFLOPPY_PROCESSING;

                                // TODO: This is where we should jump over all this shit. I know... goto is the devil. 
                                //       Well... Dijkstra can suck a duck! This is how we roll. 
        
                                // TODO: Find a utility that converts from the .IMD format into the regular raw 
                                //       format of .ima so that people can still make Dunfield images into disks. 
                                //       This would only work for writing as reading requires flux data like Greaseweazle.

                                if ( hw_usb_floppy_format_first == TRUE && direction_selected == WRITING_A_FLOPPY ) {
                                    
                                    // Whatever drive they picked, DD or HD, this determines how we try to format it on Windows 11. 

                                    if        ( hw_usb_floppy_size == 737280 ) { //format a: /f:720 // hw_usb_floppy_size

                                        // snprintf(uf_format_string, sizeof(uf_format_string),"echo \"Formatting disk in device %c. Please wait...\" && echo \"format %c: /f:720\" && format %c: /f:720", hw_usb_drive_letter[0], hw_usb_drive_letter[0], hw_usb_drive_letter[0]);
                                        snprintf(uf_format_string, sizeof(uf_format_string),"format %c: /f:720", hw_usb_drive_letter[0]);
                                        
                                    } else if ( hw_usb_floppy_size == 1474560 ) {

                                        // snprintf(uf_format_string, sizeof(uf_format_string),"echo \"Formatting disk in device %c. Please wait...\" && echo \"format %c: /f:1440\" && format %c: /f:1440", hw_usb_drive_letter[0], hw_usb_drive_letter[0], hw_usb_drive_letter[0]);
                                        snprintf(uf_format_string, sizeof(uf_format_string),"format %c: /f:1440", hw_usb_drive_letter[0]);
                                        
                                    };/*end-if*/

                                } else {

                                    uf_format_string[0] = ' ' ;
                                    uf_format_string[1] = '\0';

                                };/*end-if*/

                            switch (main_formats[drive_map[brand_selected][drive_selected]].format_id) { 

                                case msdos_fat12_350_dsdd_0720k_ima :
                                case msdos_fat12_350_dshd_1440k_ima : 

                                    /********************************************************************************/
                                    /***************************** COMMAND LINE BUILDING ****************************/
                                    /********************************************************************************/

                                    if ( direction_selected == WRITING_A_FLOPPY ) { /* file2floppy --> Loading a file and writing to a floppy. */

                                        snprintf(usb_cmd_string, sizeof(usb_cmd_string), 
                                            "\"%s\\%s\" if=\"%s\" of=\\\\.\\%c: bs=%i count=%i --progress", 
                                            options_dd_path, options_dd_executable,
                                            recent_files[image_selected-2], /* File to write to IEC device. */
                                            hw_usb_drive_letter[0],           /* Windows drive letter of USB drive. */
                                            hw_usb_block_size,                /* Block size set my the user. */
                                            hw_usb_block_count                /* Block sound set by the user. */
                                        );/*end-snprintf*/

                                    } else if ( direction_selected == READING_A_FLOPPY ) { /* floppy2file --> Reading a floppy and saving to a file. */

                                        snprintf(usb_cmd_string, sizeof(usb_cmd_string), 
                                            "\"%s\\%s\" if=\\\\.\\%c: of=\"%s\" bs=%i count=%i --progress", 
                                            options_dd_path, options_dd_executable,
                                            hw_usb_drive_letter[0],           /* Windows drive letter of USB drive. */
                                            recent_files[image_selected-2], /* File to write to IEC device. */
                                            hw_usb_block_size,                /* Block size set my the user. */
                                            hw_usb_block_count                /* Block sound set by the user. */
                                        );/*end-snprintf*/

                                    };/*end-if*/                 

                                    /********************************************************************************/
                                    /************* SEND TO THE COMMAND LINE AND RUN THE DISK PROCESSING!  ***********/
                                    /********************************************************************************/

                                    // snprintf(cmd_string, sizeof(cmd_string), "cmd.exe /k %s && echo \"%s\" && %s", uf_format_string, usb_cmd_string, usb_cmd_string);

                                    if ( hw_usb_floppy_format_first == TRUE ) snprintf(cmd_string, sizeof(cmd_string), "cmd.exe /k echo \"Formatting Drive %c:\" && %s && %s", hw_usb_drive_letter[0], uf_format_string, usb_cmd_string);
                                    else snprintf(cmd_string, sizeof(cmd_string), "cmd.exe /k echo \"Skipping Formatting...\" && %s", usb_cmd_string);
                                    
                                    STARTUPINFO si = {0};
                                    PROCESS_INFORMATION pi = {0};
                                    si.cb = sizeof(si);
                                    CreateProcess(NULL, cmd_string, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi); // Creates a command line windows and stays open when done with /k after the cmd. 
                                    CloseHandle(pi.hProcess);
                                    CloseHandle(pi.hThread);

                                break;

                                default : 
                                    passed_all_checks = FALSE;
                                    MessageBox(NULL, "That disk cannot be made on that USB Floppy Drive.\nSorry Bud!", "Error", MB_OK | MB_ICONERROR);
                                break;

                            };/*end-switch*/

                             END_OF_USBFLOPPY_PROCESSING:;

                        };/*end-if*/ /* END OF USB FLOPPY PROCESSING */

                    };/*end-if*/
                    /********************************************************************************/
                    /********************************************************************************/
                    /***************************** END OF START BUTTON ******************************/
                    /********************************************************************************/
                    /********************************************************************************/

                nk_layout_row_end(ctx);

            };/*end-if*/

            if (nk_window_is_hidden(ctx, "Task")) task_open = FALSE;

            nk_end(ctx);
            
        };/*end-if*/

        /********************************************************************************/
        /********************************************************************************/
        /****************************** END OF TASK WINDOW ******************************/
        /********************************************************************************/
        /********************************************************************************/

        /********************************************************************************/
        /********************************************************************************/
        /*************************** START OF HARDWARE WINDOW ***************************/
        /********************************************************************************/
        /********************************************************************************/
        if (hardware_open == TRUE) {
            
            if (nk_begin(ctx, "Hardware", nk_rect(448, 5, 285, 535), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_TITLE | NK_WINDOW_CLOSABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE)) {

                /* Enforce minimum window size. Nuklear's nested NK_TREE_NODE panels each track their own
                   scroll offset independently, so when the outer window scrolls, offsets accumulate and
                   produce a parallax-like effect where deeper sections scroll faster than shallower ones.
                   Keeping the window large enough to never need a scrollbar sidesteps this entirely.      */
                // struct nk_rect bounds = nk_window_get_bounds(ctx);
                // if (bounds.w < 285) nk_window_set_size(ctx, "Hardware", nk_vec2(285, bounds.h)); 
                // // if (bounds.h < 535) nk_window_set_size(ctx, "Hardware", nk_vec2(bounds.w, 535)); 
                // // Disabled because it's only the weird left and right parallax that bugs me!
                
                // DISABLED THE ABOVE ALL BECAUSE I DON'T LIKE IT FOR THIS. It makes it feel weird. 
                // TODO: I am still undecided about this. I don't know if I like it either way.
                //       I wish Nuklear devs would just fix it. Oh well it's fine!
                //       Leaving this here for now in case I feel cute and change my mind. 

                #define LABEL_W  100
                #define OPT_W     60
                #define COL_W     85
                #define USB_BTN   20
                #define USB_VAL   80
                #define VAL_W    130

                /* ---- Greaseweazle (collapsible) ---- */
                if (nk_tree_push(ctx, NK_TREE_TAB, "Greaseweazle", greaseweazle_tree_state )) {

                    // TODO: This is where I need to take the info I get from running Greaseweazle at startup. 
                    //      Then I can populate the below stuff with real info. 

                    if (nk_tree_push(ctx, NK_TREE_NODE , "Info", NK_MINIMIZED)) {

                        nk_layout_row_begin(ctx, NK_STATIC, 16, 2);
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "Model:", NK_TEXT_LEFT), " The version of your Greaseweazle hardware."); // NOTE: We need an extra space in front of tooltips has the mouse cursor can cover them up!
                            nk_layout_row_push(ctx, VAL_W);
                            nk_label(ctx, gw_info.model, NK_TEXT_LEFT);
                        nk_layout_row_end(ctx);

                        nk_layout_row_begin(ctx, NK_STATIC, 16, 2);
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "Firmware:", NK_TEXT_LEFT), " The version of the software running inside your Greaseweazle hardware.");
                            nk_layout_row_push(ctx, VAL_W);
                            nk_label(ctx, gw_info.firmware, NK_TEXT_LEFT);
                        nk_layout_row_end(ctx);

                        nk_layout_row_begin(ctx, NK_STATIC, 16, 2);
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "Serial:", NK_TEXT_LEFT), gw_info.serial);
                            nk_layout_row_push(ctx, VAL_W);
                            nk_label(ctx, gw_info.serial, NK_TEXT_LEFT);
                        nk_layout_row_end(ctx);

                        nk_layout_row_begin(ctx, NK_STATIC, 16, 2);
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "Host Tools:", NK_TEXT_LEFT), " Host tools software version.");
                            nk_layout_row_push(ctx, VAL_W);
                            nk_label(ctx, gw_info.host_tools, NK_TEXT_LEFT);
                        nk_layout_row_end(ctx);

                        /* Bandwidth button */
                        nk_layout_row_begin(ctx, NK_STATIC, 20, 4);
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "Check:", NK_TEXT_LEFT),
                                " Press to check bandwidth of the USB port and the Greaseweazle connected to it. This is helpfull if you're having problems with known good hardware and floppy disks.");

                            nk_layout_row_push(ctx, 75);
                            if (nk_button_label(ctx, "Bandwidth")) {
                                printf("Bandwidth pressed!\n");
                                show_greaseweazle_bandwidth();
                            };/*end-nk_button_label*/

                        nk_layout_row_end(ctx);

                        nk_tree_pop(ctx);
                    };/*end-if*/ /*end-nk_tree_push-"Info"*/

                    if (nk_tree_push(ctx, NK_TREE_NODE  , "Device", NK_MAXIMIZED )) {

                        /* ---- Connected Drive Specifications ---- */
                        // nk_layout_row_dynamic(ctx, 6, 1);
                        // nk_spacing(ctx, 1);
                        nk_layout_row_dynamic(ctx, 18, 1);
                        nk_label(ctx, "Connected Drive Specifications:", NK_TEXT_LEFT);

                        /* Floppy size */
                        nk_layout_row_begin(ctx, NK_STATIC, 20, 4);
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "Floppy:", NK_TEXT_LEFT),
                                " Size of the floppy disk drive.");
                            nk_layout_row_push(ctx, OPT_W);
                            if (nk_option_label(ctx, "5.25\"", hw_gw_size == 0)) hw_gw_size = 0;
                            nk_layout_row_push(ctx, 0);
                            nk_spacing(ctx, 1);
                            nk_layout_row_push(ctx, OPT_W);
                            if (nk_option_label(ctx, "3.5\"", hw_gw_size == 1)) hw_gw_size = 1;
                        nk_layout_row_end(ctx);

                        /* Drive Type */
                        {
                            int prev_drive_type = hw_gw_drive_type;
                            nk_layout_row_begin(ctx, NK_STATIC, 20, 4);
                                nk_layout_row_push(ctx, LABEL_W);
                                TOOLTIP(nk_label(ctx, "Drive Type:", NK_TEXT_LEFT),
                                    " Type of drive & cable. PC Bus uses cables with a twist, where A is at the cable end (past the twist), and drive B is in the middle (before the twist)."); //So in Greaseweazle's PC Bus mode, drive A is the one on the end of the cable (past the twist), and drive B is on the middle connector (before the twist). 
                                nk_layout_row_push(ctx, OPT_W);
                                if (nk_option_label(ctx, "PC Bus", hw_gw_drive_type == 1)) hw_gw_drive_type = 1;
                                nk_layout_row_push(ctx, 0);
                                nk_spacing(ctx, 1);
                                nk_layout_row_push(ctx, OPT_W);
                                if (nk_option_label(ctx, "Shugart", hw_gw_drive_type == 0)) hw_gw_drive_type = 0;
                            nk_layout_row_end(ctx);
                            if (hw_gw_drive_type != prev_drive_type)
                                hw_gw_drive = 0;
                        }

                        /* Drive Ident */
                        {
                            int  max_drive = (hw_gw_drive_type == 0) ? 3 : 1;
                            char ident_buf[4];
                            if (hw_gw_drive_type == 0)
                                snprintf(ident_buf, sizeof(ident_buf), "%d", hw_gw_drive);
                            else
                                snprintf(ident_buf, sizeof(ident_buf), "%c", 'A' + hw_gw_drive);

                            nk_layout_row_begin(ctx, NK_STATIC, 22, 4);
                                nk_layout_row_push(ctx, LABEL_W);
                                TOOLTIP(nk_label(ctx, "Drive Ident:", NK_TEXT_LEFT),
                                    " Sets drive to access. Shugart: DS0/DS1/DS2/DS3. PC Bus: twisted(A)/untwisted(B) cable end. PC drives on straight cable: use B. Shugart/Amiga/Atari drives set for DS0 with straight cable: use 0.");
                                nk_layout_row_push(ctx, 20);
                                if (nk_button_label(ctx, "<") && hw_gw_drive > 0) hw_gw_drive--;
                                nk_layout_row_push(ctx, 25);
                                nk_label(ctx, ident_buf, NK_TEXT_CENTERED);
                                nk_layout_row_push(ctx, 20);
                                if (nk_button_label(ctx, ">") && hw_gw_drive < max_drive) hw_gw_drive++;
                            nk_layout_row_end(ctx);
                        }

                        /* Tracks */
                        nk_layout_row_begin(ctx, NK_STATIC, 20, 4);
                        nk_layout_row_push(ctx, LABEL_W);
                        TOOLTIP(nk_label(ctx, "Tracks:", NK_TEXT_LEFT),
                            " If the drive is 40-track and you're writing an 80-track image, this drive will not work.");
                        nk_layout_row_push(ctx, OPT_W);
                        if (nk_option_label(ctx, "40", hw_gw_tracks == 0)) hw_gw_tracks = 0;
                        nk_layout_row_push(ctx, 0);
                        nk_spacing(ctx, 1);
                        nk_layout_row_push(ctx, OPT_W);
                        if (nk_option_label(ctx, "80", hw_gw_tracks == 1)) hw_gw_tracks = 1;

                        // If they pick a 3.5 then they are all 80 track drives so we auto select that below. 
                        if (hw_gw_size == 1) hw_gw_tracks = 1;
                                                    
                        nk_layout_row_end(ctx);

                        nk_layout_row_dynamic(ctx, 18, 1);
                        nk_label(ctx, "Session Settings:", NK_TEXT_LEFT);

                        /* Density */
                        nk_layout_row_begin(ctx, NK_STATIC, 20, 4);
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "Density:", NK_TEXT_LEFT),
                                " Is the drive high-density or double-density? Greaseweazle can set a pin to put the drive in HD or DD mode.");
                            nk_layout_row_push(ctx, OPT_W);
                            if (nk_option_label(ctx, "DD", hw_gw_density == 0)) hw_gw_density = 0;
                            nk_layout_row_push(ctx, 0);
                            nk_spacing(ctx, 1);
                            nk_layout_row_push(ctx, OPT_W);
                            if (nk_option_label(ctx, "HD", hw_gw_density == 1)) hw_gw_density = 1;

                            // If they pick a 40 track drive then it should NEVER be set to high-density.
                            if (hw_gw_tracks == 0) hw_gw_density = 0;

                        nk_layout_row_end(ctx);

                        /* Steps */
                        nk_layout_row_begin(ctx, NK_STATIC, 20, 4);
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "Steps:", NK_TEXT_LEFT),
                                " If the drive is 80-track and you want to read a 40-track disk, enable double-stepping to move the head correctly.");
                            nk_layout_row_push(ctx, OPT_W);
                            if (nk_option_label(ctx, "Single", hw_gw_steps == 0)) hw_gw_steps = 0;
                            nk_layout_row_push(ctx, 0);
                            nk_spacing(ctx, 1);
                            nk_layout_row_push(ctx, OPT_W);
                            if (nk_option_label(ctx, "Double", hw_gw_steps == 1)) hw_gw_steps = 1;

                            // If they pick a 40 track drive then it should NEVER be set to double stepping.
                            if (hw_gw_tracks == 0) hw_gw_steps = 0;

                            // If they pick a 3.5 drive then it should NEVER be set to double stepping.
                            if (hw_gw_size == 1) hw_gw_steps = 0;

                        nk_layout_row_end(ctx);


                        /* Check button */
                        nk_layout_row_begin(ctx, NK_STATIC, 20, 4);
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "Check:", NK_TEXT_LEFT),
                                " Press to check RPM and light-up the LED of the configured drive. This is helpfull when you have more than one drive connected to your Greaseweazle.");

                            nk_layout_row_push(ctx, OPT_W);
                            if (nk_button_label(ctx, "Verify")) {
                                printf("Verify Drive pressed!\n");
                                int result = verify_greaseweazle_drive(hw_gw_drive_type, hw_gw_drive);
                                printf("result:%i\n",result);
                                if ( result == TRUE ){
                                    MessageBox(NULL,
                                        "Greaseweazle has verified the RPM of the drive.\n\n"
                                        "This is helpfull when you have more than one drive connected to your Greaseweazle.\n\n"
                                        "You should see your floppy drive's LED illuminated after pressing the button." ,
                                        "Verify Drive", MB_OK | MB_ICONINFORMATION);
                                    /*end-MessageBox*/
                                };/*end-if*/
                            };/*end-nk_button_label*/

                        nk_layout_row_end(ctx);


                      nk_tree_pop(ctx);
                    };/*end-if*/ /*end-nk_tree_push-"Drive"*/

                if (nk_tree_push(ctx, NK_TREE_NODE  , "Advanced", NK_MINIMIZED)) {

                    /* Retries */
                    nk_layout_row_begin(ctx, NK_STATIC, 22, 4);
                        nk_layout_row_push(ctx, LABEL_W);
                        TOOLTIP(nk_label(ctx, "Retries:", NK_TEXT_LEFT),
                            " Reading a disk in poor condition? More retries may improve the ability to pull good data from it.");
                        nk_layout_row_push(ctx, 20);
                        if (nk_button_label(ctx, "<") && hw_gw_retries > 0) hw_gw_retries--;
                        nk_layout_row_push(ctx, 25);
                        char ret_buf[4]; snprintf(ret_buf, sizeof(ret_buf), "%d", hw_gw_retries);
                        nk_label(ctx, ret_buf, NK_TEXT_CENTERED);
                        nk_layout_row_push(ctx, 20);
                        if (nk_button_label(ctx, ">") && hw_gw_retries < 10) hw_gw_retries++;
                    nk_layout_row_end(ctx);

                    /* Allow Overwrite */
                    nk_layout_row_begin(ctx, NK_STATIC, 20, 3);
                        nk_layout_row_push(ctx, LABEL_W);
                        TOOLTIP(nk_label(ctx, "Overwrite:", NK_TEXT_LEFT),
                            " Greaseweazle's --no-clobber prevents overwriting existing image files. Enable this to allow overwriting an existing image.");
                        nk_layout_row_push(ctx, 20);
                        nk_checkbox_label(ctx, "", &hw_gw_overwrite);
                        nk_layout_row_push(ctx, OPT_W);
                        nk_label(ctx, "Allow", NK_TEXT_LEFT);
                    nk_layout_row_end(ctx);

                    /* Enable Greaseweazle Verify */
                    nk_layout_row_begin(ctx, NK_STATIC, 20, 3);

                        nk_layout_row_push(ctx, LABEL_W);
                        TOOLTIP(nk_label(ctx, "Verify on Init:", NK_TEXT_LEFT),
                            " This verifies that Greaseweazle can find a disk in it's drive before doing anything else. Disabling this speeds things up!");
                        nk_layout_row_push(ctx, 20);
                        nk_checkbox_label(ctx, "", &hw_gw_verify);
                        nk_layout_row_push(ctx, OPT_W);
                        nk_label(ctx, "Enable", NK_TEXT_LEFT);

                    nk_layout_row_end(ctx);

                    /* Reset on Init */
                    nk_layout_row_begin(ctx, NK_STATIC, 20, 3);

                        nk_layout_row_push(ctx, LABEL_W);
                        TOOLTIP(nk_label(ctx, "Reset on Init:", NK_TEXT_LEFT),
                            " This resets Greaseweazle first before processing a read or write session.");
                        nk_layout_row_push(ctx, 20);
                        nk_checkbox_label(ctx, "", &hw_gw_reset);
                        nk_layout_row_push(ctx, OPT_W);
                        nk_label(ctx, "Enable", NK_TEXT_LEFT);

                        // This causes the program to issue a command to reset the Greaseweazle.
                        // This is because sometimes the Greaseweazle gets confused. The reset helps it to unfuck itself. 
                        // If this is enabled we set a special string for cmd.exe. 
                        if      (hw_gw_reset==1) snprintf(gw_reset_string,sizeof(gw_reset_string),"echo \"gw reset enabled\" && \"%s\\%s\" reset", options_greaseweazle_path , options_greaseweazle_executable); 
                        else if (hw_gw_reset==0) strcpy(gw_reset_string,"echo \"gw reset disabled\""); 

                    nk_layout_row_end(ctx);

                  nk_tree_pop(ctx);

                }

                    nk_tree_pop(ctx);

                };/*end-if*/ /*end-nk_tree_push-"Greaseweazle"*/

                /* ---- ZoomFloppy (collapsible) ---- */
                if (nk_tree_push(ctx, NK_TREE_TAB, "ZoomFloppy", zoomfloppy_tree_state)) {

                    if (nk_tree_push(ctx, NK_TREE_NODE , "Info", NK_MINIMIZED)) {

                        /* Info lines */

                        nk_layout_row_begin(ctx, NK_STATIC, 16, 2);
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "Version:", NK_TEXT_LEFT), " This is the version of OpenCBM that's installed."); // NOTE: We need an extra space in front of tooltips has the mouse cursor can cover them up!
                            nk_layout_row_push(ctx, VAL_W);
                            nk_label(ctx, zf_info.version, NK_TEXT_LEFT);
                        nk_layout_row_end(ctx);

                        nk_layout_row_begin(ctx, NK_STATIC, 16, 2);
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "Build Date:", NK_TEXT_LEFT), " The build date of OpenCBM.");
                            nk_layout_row_push(ctx, VAL_W);
                            nk_label(ctx, zf_info.build_date, NK_TEXT_LEFT);
                        nk_layout_row_end(ctx);

                        nk_layout_row_begin(ctx, NK_STATIC, 16, 2);
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "Build Time:", NK_TEXT_LEFT), " The build time of OpenCBM.");
                            nk_layout_row_push(ctx, VAL_W);
                            nk_label(ctx, zf_info.build_time, NK_TEXT_LEFT);
                        nk_layout_row_end(ctx);

                        nk_layout_row_begin(ctx, NK_STATIC, 16, 2);
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "Device Count:", NK_TEXT_LEFT), " How many IEC devices were found.");
                            nk_layout_row_push(ctx, VAL_W);
                            nk_label(ctx, zf_info.device_count_string, NK_TEXT_LEFT);
                        nk_layout_row_end(ctx);

                        nk_tree_pop(ctx);

                    };/*end-if*/ /*end-nk_tree_push-"Info"*/

                    if (nk_tree_push(ctx, NK_TREE_NODE  , "Device", NK_MAXIMIZED )) {

                        /* IEC Drive Device # */
                        nk_layout_row_begin(ctx, NK_STATIC, 22, 4);
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "IEC Device #:", NK_TEXT_LEFT),
                                " The IEC/IEEE-488 device number of the drive you want to access.");
                            nk_layout_row_push(ctx, 20);
                            if (nk_button_label(ctx, "<") && hw_zf_iec_device > 1) hw_zf_iec_device--;
                            nk_layout_row_push(ctx, 25);
                            char iec_buf[8]; snprintf(iec_buf, sizeof(iec_buf), "%d", hw_zf_iec_device);
                            nk_label(ctx, iec_buf, NK_TEXT_CENTERED);
                            nk_layout_row_push(ctx, 20);
                            if (nk_button_label(ctx, ">")) hw_zf_iec_device++;
                        nk_layout_row_end(ctx);

                        /* OpenCBM Drive Selection */
                        nk_layout_row_begin(ctx, NK_STATIC, 20, 4);
                            nk_layout_row_push(ctx, 75);
                            TOOLTIP(nk_label(ctx, "IEC Drive:", NK_TEXT_LEFT), " Drive that's connected to ZoomFloppy.");
                            nk_layout_row_push(ctx, 135);
                            nk_combobox(ctx, opencbm_drive_options, OPENCBM_DRIVE_COUNT, &current_opencbm_drive_selected, 25, nk_vec2(nk_widget_width(ctx), 240));
                        nk_layout_row_end(ctx);

                        nk_layout_row_dynamic(ctx, 18, 1);
                        nk_label(ctx, "Session Settings:", NK_TEXT_LEFT);

                        // TODO: Determine if putting the drive into a single-sided or double-sided mode is needed. 
                        //       I think that the imgcopy utility probably just like does it properly. 
                        //       So if I try to write a single-sided image to a SFD-1001 it probably just
                        //       doesn't send any commands to do the other side. Like it's probably doing
                        //       block reads and writes so like it's fine. 

                        /* Reset on Init */
                        nk_layout_row_begin(ctx, NK_STATIC, 20, 3);
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "Reset on Init:", NK_TEXT_LEFT),
                                " Sends the drive a reset command before starting a read or write session.");
                            nk_layout_row_push(ctx, 20);
                            nk_checkbox_label(ctx, "", &hw_zf_reset);
                            nk_layout_row_push(ctx, OPT_W);
                            nk_label(ctx, "Enable", NK_TEXT_LEFT);
                        nk_layout_row_end(ctx);

                        /* Format First */
                        nk_layout_row_begin(ctx, NK_STATIC, 20, 3);
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "Format First:", NK_TEXT_LEFT),
                                " Be careful! This option formats the disk first before trying to write the image to the disk.");
                            nk_layout_row_push(ctx, 20);
                            nk_checkbox_label(ctx, "", &hw_zf_format_first);
                            nk_layout_row_push(ctx, OPT_W);
                            nk_label(ctx, "Enable", NK_TEXT_LEFT);
                            if ( direction_selected == READING_A_FLOPPY ) hw_zf_format_first = FALSE; /* This precents the user from accidentally formatting a disk BEFORE reading it which would destroy the very disk you're trying to image. floppy2file --> Reading a floppy and saving to a file. */
                        nk_layout_row_end(ctx);

                        /* BAM-Only Option */
                        nk_layout_row_begin(ctx, NK_STATIC, 20, 3);
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "BAM-Only:", NK_TEXT_LEFT),
                                " Fast BAM-Only mode means only used blocks are sent. It's like a \"files only\" mode. It's faster because it doesn't send the entire disk image.");
                            nk_layout_row_push(ctx, 20);
                            nk_checkbox_label(ctx, "", &hw_zf_bam_only);
                            nk_layout_row_push(ctx, OPT_W);
                            nk_label(ctx, "Enable", NK_TEXT_LEFT);
                        nk_layout_row_end(ctx);
                    
                        /* Check button */
                        nk_layout_row_begin(ctx, NK_STATIC, 20, 4);
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "Check:", NK_TEXT_LEFT),
                                " Press to check the devices of OpenCBM and the ZoomFloppy connected to it. This is helpfull because it shows you what IEEE-488/IEC devices are connected and working.");

                            nk_layout_row_push(ctx, 75);
                            if (nk_button_label(ctx, "Scan")) {
                                printf("Scan pressed!\n");
                                display_zoomfloppy_drives();
                            };/*end-nk_button_label*/

                        nk_layout_row_end(ctx);

                        nk_tree_pop(ctx);

                    };/*end-if*/ /*end-nk_tree_push-"Drive"*/

                    nk_tree_pop(ctx);

                };/*end-if*/ /*end-nk_tree_push-"ZoomFloppy"*/

                /*************************************/
                /***** USB 3.5-inch Floppy Drive *****/
                /*************************************/

                if ( nk_tree_push(ctx, NK_TREE_TAB, "USB 3.5-inch Floppy Drive", usbfloppy_tree_state) ) {

                    if (nk_tree_push(ctx, NK_TREE_NODE , "Info", NK_MINIMIZED)) {

                        /* DD Version */
                        nk_layout_row_begin(ctx, NK_STATIC, 16, 2);
                            nk_layout_row_push(ctx, 80);
                            TOOLTIP(nk_label(ctx, "DD Version:", NK_TEXT_LEFT),
                                " Hardware info returned by the system.");
                            nk_layout_row_push(ctx, 150);

                        // nk_label(ctx, "TEAC FD-05PUB USB Device", NK_TEXT_LEFT);
                        nk_label(ctx, dd_info.version, NK_TEXT_LEFT);

                        nk_layout_row_end(ctx);

                        /* Info lines */
                        nk_layout_row_begin(ctx, NK_STATIC, 16, 2);
                            nk_layout_row_push(ctx, 30);
                            TOOLTIP(nk_label(ctx, "Drv:", NK_TEXT_LEFT),
                                " Hardware info returned by the system.");
                            nk_layout_row_push(ctx, 175);

                        // nk_label(ctx, "TEAC FD-05PUB USB Device", NK_TEXT_LEFT);
                        nk_label(ctx, usbfloppy_device_name, NK_TEXT_LEFT);

                        nk_layout_row_end(ctx);

                        nk_tree_pop(ctx);

                    };/*end-if*/ /*end-nk_tree_push-"Info"*/

                    if (nk_tree_push( ctx , NK_TREE_NODE , "Device", NK_MAXIMIZED )) {

                        /* Drive Letter */
                        nk_layout_row_begin(ctx, NK_STATIC, 22, 4); // Below is indented because I want to show nk_layout_row_begin and nk_layout_row_end like it's opening and closing a chunk of grouped together code. 
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "Drive Letter:", NK_TEXT_LEFT),
                                " The Windows drive letter for the USB floppy drive you have plugged in.");
                            nk_layout_row_push(ctx, USB_BTN);
                            if (nk_button_label(ctx, "<") && hw_usb_drive_letter[0] > 'A')
                                hw_usb_drive_letter[0]--;
                            nk_layout_row_push(ctx, USB_VAL);
                            nk_label(ctx, hw_usb_drive_letter, NK_TEXT_CENTERED);
                            nk_layout_row_push(ctx, USB_BTN);
                            if (nk_button_label(ctx, ">") && hw_usb_drive_letter[0] < 'B') // This was 'Z' but now I'm limited it to B so we don't blow away someone's hard drive. // if (nk_button_label(ctx, ">") && hw_usb_drive_letter[0] < 'Z')
                                hw_usb_drive_letter[0]++;
                        nk_layout_row_end(ctx);

                        /* Floppy Size */
                        nk_layout_row_begin(ctx, NK_STATIC, 22, 4); // Below is indented because I want to show nk_layout_row_begin and nk_layout_row_end like it's opening and closing a chunk of grouped together code. 
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "Floppy Size:", NK_TEXT_LEFT),
                                " The overall size of the floppy disk -- either a DD or HD disk.");
                            nk_layout_row_push(ctx, USB_BTN);
                            if (nk_button_label(ctx, "<") && hw_usb_floppy_preset > 0) {
                                hw_usb_floppy_preset--;
                                hw_usb_floppy_size = floppy_presets[hw_usb_floppy_preset];
                                hw_usb_block_count = 80;
                                hw_usb_block_size  = hw_usb_floppy_size / hw_usb_block_count;
                                // hw_usb_block_count = hw_usb_floppy_size / hw_usb_block_size;
                            }
                            nk_layout_row_push(ctx, USB_VAL);
                            nk_label(ctx, floppy_preset_names[hw_usb_floppy_preset], NK_TEXT_CENTERED);
                            nk_layout_row_push(ctx, USB_BTN);
                            if (nk_button_label(ctx, ">") && hw_usb_floppy_preset < 1) {
                                hw_usb_floppy_preset++;
                                hw_usb_floppy_size = floppy_presets[hw_usb_floppy_preset];
                                hw_usb_block_count = 80;
                                hw_usb_block_size  = hw_usb_floppy_size / hw_usb_block_count;
                                // hw_usb_block_count = hw_usb_floppy_size / hw_usb_block_size;
                            }
                        nk_layout_row_end(ctx);

                        nk_layout_row_dynamic(ctx, 18, 1);
                        nk_label(ctx, "Session Settings:", NK_TEXT_LEFT);

                        /* Block Count */
                        nk_layout_row_begin(ctx, NK_STATIC, 22, 4); // Below is indented because I want to show nk_layout_row_begin and nk_layout_row_end like it's opening and closing a chunk of grouped together code. 
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "Block Count:", NK_TEXT_LEFT),
                                " How many blocks the image is broken into during a transfer. Dependent on overall disk size and block size.");
                            nk_layout_row_push(ctx, USB_BTN);
                            if (nk_button_label(ctx, "<")) {
                                int next = hw_usb_block_count - 1;
                                while (next >= 1 && hw_usb_floppy_size % next != 0) next--;
                                if (next >= 1) {
                                    hw_usb_block_count = next;
                                    hw_usb_block_size  = hw_usb_floppy_size / hw_usb_block_count;
                                }
                            }
                            nk_layout_row_push(ctx, USB_VAL);
                            char bc_buf[16]; snprintf(bc_buf, sizeof(bc_buf), "%d", hw_usb_block_count);
                            nk_label(ctx, bc_buf, NK_TEXT_CENTERED);
                            nk_layout_row_push(ctx, USB_BTN);
                            if (nk_button_label(ctx, ">")) {
                                int next = hw_usb_block_count + 1;
                                while (next < hw_usb_floppy_size && hw_usb_floppy_size % next != 0) next++;
                                if (next < hw_usb_floppy_size && hw_usb_floppy_size / next >= 128) {
                                    hw_usb_block_count = next;
                                    hw_usb_block_size  = hw_usb_floppy_size / hw_usb_block_count;
                                }
                            }
                        nk_layout_row_end(ctx);

                        /* Block Size */
                        nk_layout_row_begin(ctx, NK_STATIC, 22, 4); // Below is indented because I want to show nk_layout_row_begin and nk_layout_row_end like it's opening and closing a chunk of grouped together code. 
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "Block Size:", NK_TEXT_LEFT),
                                " Size in bytes of an individual block. A larger block size may result in a faster read and affects the Block Count.");
                            nk_layout_row_push(ctx, USB_BTN);
                            
                            if (nk_button_label(ctx, "<")) {
                                int next = hw_usb_block_size - 1;
                                while (next >= 128 && hw_usb_floppy_size % next != 0) next--;
                                if (next >= 128) {
                                    hw_usb_block_size  = next;
                                    hw_usb_block_count = hw_usb_floppy_size / hw_usb_block_size;
                                }
                            }

                            nk_layout_row_push(ctx, USB_VAL);
                            char bs_buf[8]; snprintf(bs_buf, sizeof(bs_buf), "%d", hw_usb_block_size);
                            nk_label(ctx, bs_buf, NK_TEXT_CENTERED);
                            nk_layout_row_push(ctx, USB_BTN);
                            
                            if (nk_button_label(ctx, ">")) {
                                int next = hw_usb_block_size + 1;
                                while (next < hw_usb_floppy_size && hw_usb_floppy_size % next != 0) next++;
                                if (next < hw_usb_floppy_size) {
                                    hw_usb_block_size  = next;
                                    hw_usb_block_count = hw_usb_floppy_size / hw_usb_block_size;
                                }
                            }
                        nk_layout_row_end(ctx);

                        /* Format First */
                        nk_layout_row_begin(ctx, NK_STATIC, 20, 3);
                            nk_layout_row_push(ctx, LABEL_W);
                            TOOLTIP(nk_label(ctx, "Format First:", NK_TEXT_LEFT),
                                " Be careful! This option formats the disk in teh USB Floppy Drive first before trying to write the image to the disk.");
                            nk_layout_row_push(ctx, 20);
                            nk_checkbox_label(ctx, "", &hw_usb_floppy_format_first);
                            nk_layout_row_push(ctx, OPT_W);
                            nk_label(ctx, "Enable", NK_TEXT_LEFT);
                            if ( direction_selected == READING_A_FLOPPY ) hw_usb_floppy_format_first = FALSE; /* This precents the user from accidentally formatting a disk BEFORE reading it which would destroy the very disk you're trying to image. floppy2file --> Reading a floppy and saving to a file. */
                        nk_layout_row_end(ctx);

                        nk_tree_pop(ctx);

                    };/*end-if*/ /*end-nk_tree_push-"Drive"*/

                    nk_tree_pop(ctx);   

                };/*end-if*/ /*end-nk_tree_push-"USB 3.5-inch Floppy Drive"*/

                #undef TOOLTIP
                #undef USB_BTN
                #undef USB_VAL
                #undef LABEL_W
                #undef OPT_W
                #undef COL_W
                #undef VAL_W

            };/*end-if*/ /*end-nk_begin-"Hardware"*/

            if (nk_window_is_hidden(ctx, "Hardware"))
                hardware_open = FALSE;
            nk_end(ctx);

        };/*end-if*/ /* end of hardware_open == TRUE */

        /* --- Options Window --- */
        if (options_open == TRUE) {
            if (nk_begin(ctx, "Options", nk_rect(175, 10, 400, 525),
                NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_TITLE | NK_WINDOW_CLOSABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE))
            {

                nk_layout_row_dynamic(ctx, 18, 1);
                nk_label(ctx, "Program Options", NK_TEXT_LEFT);

                nk_layout_row_dynamic(ctx, 18, 1);

                nk_layout_row_dynamic(ctx, 18, 1);
                nk_label(ctx, "Don't forget to scroll to the bottom", NK_TEXT_LEFT);
                nk_layout_row_dynamic(ctx, 18, 1);
                nk_label(ctx, "and click Save Options!", NK_TEXT_LEFT);

                nk_layout_row_dynamic(ctx, 18, 1);

                /******************************************/
                /********** Background Color **************/
                /******************************************/
                nk_layout_row_dynamic(ctx, 18, 1);
                nk_label(ctx, "User Interface:", NK_TEXT_LEFT);
                nk_layout_row_dynamic(ctx, 18, 1);
                nk_label(ctx, "Background Color:", NK_TEXT_LEFT);
                nk_layout_row_dynamic(ctx, 25, 1);
                if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx), 400))) {
                    nk_layout_row_dynamic(ctx, 120, 1);
                    bg = nk_color_picker(ctx, bg, NK_RGBA);
                    nk_layout_row_dynamic(ctx, 25, 1);
                    bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
                    bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
                    bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
                    bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f);
                    nk_combo_end(ctx);
                }

                /******************************************/
                /************* External Paths *************/
                /******************************************/
                nk_layout_row_dynamic(ctx, 10, 1); // spacer
                nk_layout_row_dynamic(ctx, 18, 1);
                nk_label(ctx, "External Paths:", NK_TEXT_LEFT);

                #define BROWSE_BUTTON_WIDTH 70
                #define EDIT_BOX_HEIGHT     25
                float path_widths[] = { nk_widget_width(ctx) - BROWSE_BUTTON_WIDTH - 8, BROWSE_BUTTON_WIDTH };

                // Greaseweazle Path
                nk_layout_row_dynamic(ctx, 18, 1);
                nk_label(ctx, "Greaseweazle Path:", NK_TEXT_LEFT);
                nk_layout_row(ctx, NK_STATIC, EDIT_BOX_HEIGHT, 2, path_widths);
                static int gw_path_len = 0;
                gw_path_len = (int)strlen(options_greaseweazle_path);
                nk_edit_string(ctx, NK_EDIT_FIELD | NK_EDIT_READ_ONLY, options_greaseweazle_path, &gw_path_len, sizeof(options_greaseweazle_path), nk_filter_default);
                if (nk_button_label(ctx, "Browse")) {
                    browse_for_folder(hwnd, "Select Greaseweazle Folder", options_greaseweazle_path, sizeof(options_greaseweazle_path));
                }

                // CiderPress2 Path
                nk_layout_row_dynamic(ctx, 18, 1);
                nk_label(ctx, "CiderPress2 Path:", NK_TEXT_LEFT);
                nk_layout_row(ctx, NK_STATIC, EDIT_BOX_HEIGHT, 2, path_widths);
                static int cp2_path_len = 0;
                cp2_path_len = (int)strlen(options_ciderpress2cli_path);
                nk_edit_string(ctx, NK_EDIT_FIELD | NK_EDIT_READ_ONLY, options_ciderpress2cli_path, &cp2_path_len, sizeof(options_ciderpress2cli_path), nk_filter_default);
                if (nk_button_label(ctx, "Browse")) {
                    browse_for_folder(hwnd, "Select CiderPress2 Folder", options_ciderpress2cli_path, sizeof(options_ciderpress2cli_path));
                }

                // OpenCBM Path
                nk_layout_row_dynamic(ctx, 18, 1);
                nk_label(ctx, "OpenCBM Path:", NK_TEXT_LEFT);
                nk_layout_row(ctx, NK_STATIC, EDIT_BOX_HEIGHT, 2, path_widths);
                static int opencbm_path_len = 0;
                opencbm_path_len = (int)strlen(options_opencbm_path);
                nk_edit_string(ctx, NK_EDIT_FIELD | NK_EDIT_READ_ONLY, options_opencbm_path, &opencbm_path_len, sizeof(options_opencbm_path), nk_filter_default);
                if (nk_button_label(ctx, "Browse")) {
                    browse_for_folder(hwnd, "Select OpenCBM Folder", options_opencbm_path, sizeof(options_opencbm_path));
                }

                // DD Path
                nk_layout_row_dynamic(ctx, 18, 1);
                nk_label(ctx, "DD Path:", NK_TEXT_LEFT);
                nk_layout_row(ctx, NK_STATIC, EDIT_BOX_HEIGHT, 2, path_widths);
                static int dd_path_len = 0;
                dd_path_len = (int)strlen(options_dd_path);
                nk_edit_string(ctx, NK_EDIT_FIELD | NK_EDIT_READ_ONLY, options_dd_path, &dd_path_len, sizeof(options_dd_path), nk_filter_default);
                if (nk_button_label(ctx, "Browse")) {
                    browse_for_folder(hwnd, "Select DD Folder", options_dd_path, sizeof(options_dd_path));
                }

                /******************************************/
                /*************** Save Button **************/
                /******************************************/
                nk_layout_row_dynamic(ctx, 10, 1); // spacer
                nk_layout_row_dynamic(ctx, 30, 1);

                struct nk_style_button start_style = ctx->style.button;
                /************************************************************************************************************************************/
                /******************************************************************/ /* Below are the NORMAL, or mouse off, colors.                 */
                start_style.normal      = nk_style_item_color(UI_BACK_NORMAL_COLOR); /* The background button or UI element color. The body color.  */
                start_style.text_normal =                     UI_TEXT_NORMAL_COLOR;  /* The button or UI element's text color.                      */
                /******************************************************************/ /***************************************************************/
                /******************************************************************/ /* Below are the HOVER, or mouse over, colors.                 */
                start_style.hover       = nk_style_item_color(UI_BACK_HOVER_COLOR);  /* The background button or UI element color. The body color.  */
                start_style.text_hover  =                     UI_TEXT_HOVER_COLOR;   /* The button or UI element's text color.                      */
                /******************************************************************/ /***************************************************************/
                /******************************************************************/ /* Below are the ACTIVE, or mouse button down, colors.         */
                start_style.active      = nk_style_item_color(UI_BACK_ACTIVE_COLOR); /* The background button or UI element color. The body color.  */  
                start_style.text_active =                     UI_TEXT_ACTIVE_COLOR;  /* The button or UI element's text color.                      */ 
                /************************************************************************************************************************************/
                // if (nk_button_label(ctx, "Save Options")) {
                if (nk_button_label_styled(ctx, &start_style, "Save Options")) { 
                    
                    options_background_color_r = bg.r;
                    options_background_color_g = bg.g;
                    options_background_color_b = bg.b;
                    options_background_color_a = bg.a;
                    
                    save_config(argv[0],
                                options_background_color_r,
                                options_background_color_g,
                                options_background_color_b,
                                options_background_color_a,
                                options_greaseweazle_path,
                                options_ciderpress2cli_path,
                                options_opencbm_path,
                                options_dd_path,
                                options_greaseweazle_executable,
                                options_ciderpress2_cli_executable,
                                options_opencbm_cbmctrl_executable,
                                options_opencbm_d64copy_executable,
                                options_opencbm_imgcopy_executable,
                                options_opencbm_d82copy_executable,
                                options_dd_executable
                    );/*end-save_config*/

                    MessageBoxA(NULL, "Please restart the program after changing settings.", "Restart Reminder", MB_OK | MB_ICONWARNING);

                };/*end-nk_button_label*/

            };/*end-if*/
            if (nk_window_is_hidden(ctx, "Options")) options_open = FALSE;
            nk_end(ctx);

        };/*end-if*/

        /**********************************************************/
        /************************ About Box ***********************/
        /**********************************************************/

        /******************** About Box Window ********************/

        if (about_open == TRUE) {

            play_sound(argv[0], "assets/sounds/about.wav");

            // BACKUP --> We need to turn OFF the tooltips while this window is open. We can turn them back on again when we are done.
            // THIS ISN'T NEEDED - What is needed is some place to store the state the last time a user did something. 
            //                     So what that means is that we need to track WHEN that status has changed, 
            //                     for example the user pressed the /? key and that means they want it toggled, let's say turned off. 
            //                     So in that moment, when we toggle it, we should store something like last_user_request_tooltip_state
            //                     as the backup. Then when we restore it, we only need to restore it from this varible, 
            //                     and we dont' need to back it up at all. 

            // We need to turn OFF the tooltips while this window is open. We can turn them back on again when we are done.
            is_tooltip_text_enabled = FALSE; 

            // WINDOW_WIDTH  738 
            // WINDOW_HEIGHT 545

            #define ABOUT_IMAGE_WIDTH  658
            #define ABOUT_IMAGE_HEIGHT 372

            #define ABOUT_BOX_WIDTH  (ABOUT_IMAGE_WIDTH  + 10)
            #define ABOUT_BOX_HEIGHT (ABOUT_IMAGE_HEIGHT + 90) // Remember: The brackets keep things together. Otherwise you get shit like this where you don't expect it: (WINDOW_HEIGHT-372+100)/2

            // WINDOW_WIDTH  738 
            // WINDOW_HEIGHT 545

            if (nk_begin(ctx, "About", 
                nk_rect( (WINDOW_WIDTH-ABOUT_BOX_WIDTH)/2 , (WINDOW_HEIGHT-ABOUT_BOX_HEIGHT)/2 , ABOUT_BOX_WIDTH , ABOUT_BOX_HEIGHT ),
                NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_TITLE | NK_WINDOW_CLOSABLE | NK_WINDOW_NO_SCROLLBAR)
                ) {
                
                /***** ABOUT BOX IMAGE *****/ 
                nk_layout_row_dynamic(ctx, ABOUT_IMAGE_HEIGHT, 1);
                struct nk_rect image_bounds;
                nk_widget(&image_bounds, ctx);
                struct nk_command_buffer *canvas = nk_window_get_canvas(ctx);
                nk_draw_image(
                    canvas, 
                    nk_rect(image_bounds.x, image_bounds.y, ABOUT_IMAGE_WIDTH, ABOUT_IMAGE_HEIGHT),
                    &about_box_image, 
                    nk_white
                );/*end-nk_draw_image*/

                /***** OKAY! BUTTON *****/ 
                struct nk_style_button okay_style = ctx->style.button;

                /************************************************************************************************************************************/
                /******************************************************************/ /* Below are the NORMAL, or mouse off, colors.                 */
                okay_style.normal      = nk_style_item_color(UI_BACK_NORMAL_COLOR); /* The background button or UI element color. The body color.  */
                okay_style.text_normal =                     UI_TEXT_NORMAL_COLOR;  /* The button or UI element's text color.                      */
                /******************************************************************/ /***************************************************************/
                /******************************************************************/ /* Below are the HOVER, or mouse over, colors.                 */
                okay_style.hover       = nk_style_item_color(UI_BACK_HOVER_COLOR);  /* The background button or UI element color. The body color.  */
                okay_style.text_hover  =                     UI_TEXT_HOVER_COLOR;   /* The button or UI element's text color.                      */
                /******************************************************************/ /***************************************************************/
                /******************************************************************/ /* Below are the ACTIVE, or mouse button down, colors.         */
                okay_style.active      = nk_style_item_color(UI_BACK_ACTIVE_COLOR); /* The background button or UI element color. The body color.  */  
                okay_style.text_active =                     UI_TEXT_ACTIVE_COLOR;  /* The button or UI element's text color.                      */ 
                /************************************************************************************************************************************/

                #define ABOUTBOX_BUTTON_SIZE         60
                #define ABOUTBOX_BUTTON_SIZE_PADDING  7
                #define ABOUTBOX_NUMBER_OF_BUTTONS    4
                float widths[] = { 
                    ABOUTBOX_BUTTON_SIZE, /* Credits */
                    ABOUTBOX_BUTTON_SIZE, /* GitHub */
                    ABOUTBOX_BUTTON_SIZE, /* Tip */
                    ABOUT_BOX_WIDTH-((ABOUTBOX_BUTTON_SIZE+ABOUTBOX_BUTTON_SIZE_PADDING)*ABOUTBOX_NUMBER_OF_BUTTONS), 
                    ABOUTBOX_BUTTON_SIZE  /* Okay! */ 
                };/*end-array*/

                nk_layout_row(ctx, NK_STATIC, 30, 5, widths); 

                /* Credits Button */
                if (nk_button_label(ctx, "Credits")) {
                    // Opens a local credits.txt file in whatever the default text file viewer is set to.  
                    ShellExecuteA(NULL, "open", "credits.txt", NULL, NULL, SW_SHOWNORMAL);
                };/*end-if*/

                /* GitHub Button */
                if (nk_button_label(ctx, "GitHub")) {
                    // Opens "https://github.com/chironb/ChironsDiskBlaster" in whatever the default web browser is set to. 
                    ShellExecuteA(NULL, "open", "https://github.com/chironb/ChironsDiskBlaster", NULL, NULL, SW_SHOWNORMAL);
                };/*end-if*/

                /* Buy Button */
                if (nk_button_label(ctx, "Buy")) {
                    // Opens "https://www.chiron-studios.com/products/chirons-disk-blaster" in whatever the default web browser is set to. 
                    ShellExecuteA(NULL, "open", "https://www.chiron-studios.com/products/chirons-disk-blaster", NULL, NULL, SW_SHOWNORMAL);
                };/*end-if*/

                static char build_str[128];
                snprintf(build_str, sizeof(build_str), "Beta - v%i.%02i - %s at %s", __MAJOR_VERSIONNUMBER__, __MINOR_BUILDNUMBER__, __DATE__, __TIME__);
                nk_label(ctx, build_str, NK_TEXT_CENTERED);

                if (nk_button_label_styled(ctx, &okay_style, "Okay!")) {

                    about_open = FALSE;

                    is_tooltip_text_enabled = user_requested_tooltip_state;

                    stop_sound();

                };/*end-if*/

            };/*end-if*/

            if (nk_window_is_hidden(ctx, "About")) {
                about_open = FALSE;
                stop_sound();
            };/*end-if*/

            nk_end(ctx);

        };/*end-if*/

        /**********************************************************/

        /**********************************************************/
        /************************** Draw **************************/
        /**********************************************************/

        SDL_GetWindowSize(win, &win_width, &win_height);
        glViewport(0, 0, win_width, win_height);
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Ensure blending and texture unit 0 are active for Nuklear renderer */
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glActiveTexture(GL_TEXTURE0);

        /* Nuklear render (GL state: active texture 0, blending enabled) */
        // nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY); /* OpenGL 3 */
        nk_sdl_render(NK_ANTI_ALIASING_ON); /* OpenGL 2 */
        SDL_GL_SwapWindow(win);

        // Now update the previous with the current so it only lasts one cycle of this loop.
        strncpy(previously_selected_file,recent_files[image_selected-2],MAX_PATH);
        previous_image_selected = image_selected;
        // has_currently_selected_file_changed = FALSE;

    };/*end-while*/

    /**********************************************************/
    /********************* End Primary Loop *******************/
    /**********************************************************/

    cleanup:

    // TODO: Actually do the right cleanups! Maybe there's an easier way to do this? 
    // 
    /* cleanup image texture */
    // if (rocket_tex) {
    //     glDeleteTextures(1, &rocket_tex);
    //     rocket_tex = 0;
    // }
    // if (rocket_inv_tex) {
    //     glDeleteTextures(1, &rocket_inv_tex);
    //     rocket_inv_tex = 0;
    // }
    // if (rocket_grey_tex) {
    //     glDeleteTextures(1, &rocket_grey_tex);
    //     rocket_grey_tex = 0;
    // }

    nk_sdl_shutdown();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(win);
    SDL_Quit();

    // Computer... End program.
    return 0;

}/*end-main*/