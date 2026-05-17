--------------------------------------------------------------------------------
Introduction
--------------------------------------------------------------------------------

Chiron's Disk Blaster is an modern desktop application for people who want to read and write real floppy disks for vintage computers using a clean and simple interface. It serves as a front-end for Greaseweazle, ZoomFloppy and even standard USB Floppy Drives, allowing the user to use one application across multiple solutions. 

It also provided enhanced support by converting files to and from whatever is needed by Greaseweazle. For example the ATR and 2MG are supported by converting the formats are not directedly supported by Greaseweazle using CiderPress2. You just have to show Chiron's Disk Blaster where to find your local installation of the various external tools used to bring it all together. 

It provides relief from knowing and navigating all the complicated details around programs like Greaseweazle host tools and ZoomFloppy with OpenCBM. Unlike other GUI's, this program doesn't just show a bunch of controls for each and every command line option. Instead it figures out a lot of the details for you and if you try to do something that doesn't work it'll prompt you before you do and actually try to make a disk. 

When you open or drag-and-drop a file onto the app it will identify the format automatically and set the family, computer, and drive. For the rare file that is genuinely ambiguous it will prompt you to clarify. A couple of clicks to tell it about your hardware and it's ready to read or a write a disk!

The interface also has visual icons and elements that make it clear what you're doing. It shows the computer, a real world drive, an icon that shows the disk image file and what kind of floppy it's intended for, a read/write icon that shows direction information is flowing, and a hardware icon that embeds pictures of the current hardware settings. This gives you the ability of at-a-glance checking that you've correctly setup your job.

Chiron's Disk Blaster supports a wide range of systems: 
Commodore 64, 128, Amiga, VIC-20, PET, and CBM drives; Apple II, Apple IIGS, and classic Macintosh; Atari 8-bit (ATR) and Atari ST; BBC Micro; Acorn Archimedes; Dragon; Tandy CoCo; and standard PC formats.

Keyboard shortcuts, configurable paths for all external tools, and smart safety checks (like preventing a format-first wipe when you meant to read a disk) keep things simply moving without gotchas.

It's released under the GNU GPL 3, an open source licence!

Please see the following files for more information: 
- Help.txt    --> More detailed instructions on setting up and using the program.
- Credits.txt --> More details about images sourced from Wikipedia and Wikimedia. 
- Licence.txt --> This is the text of the GNU GPL 3 licence.

Please note: On Mesa3D and OpenGL (opengl32.dll)
This version comes with the Mesa3D driver for Windows. This is an open-source library that provides software rendering capabilities to OpenGL applications. Note that software rendering is a LOT slower than 3D accelerators, so this library should only be used as a last resort, for older machines that may not have newer OpenGL driver support. For example my ThinkPad T420 has an Intel 3000 series CPU and build in GPU. Without this Mesa3D driver the program would not work on it, since Windows 11 stopped supporting OpenGL on this CPU/GPU combo. The Mesa3D driver means it will work as expected. However, it is much more efficient and performant to use your Windows 11 driver if you're computer isn't a 15 year old laptop. You can therefore rename or delete this file: opengl32.dll and Windows 11 will automatically use the system driver instead when Chiron's Disk Blaster runs. 