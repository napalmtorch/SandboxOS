### SandboxOS
A simple operating system which focuses on interpreting 90% of user code in a safe sandboxed instance 
Developed by Nik300 and napalmtorch 

### Building
To build our operating system, simply clone the repo, cd the root repo directory, and run the build/scripts/build_all.sh

### Adding files to RAMDISK
To add additional files to the RAMDISK image, use the 'add' command in the RAMDISK build script located at build/disks/a/build 

### CURRENTLY IMPLEMENTED
- Multiboot modules
- Serial Debugging
- GDT/IDT
- Physical Memory Manager
- Heap Allocator
- RAMFS
- VFS Abstraction Layer
- Scheduler/Threading
- Garbage Collector
- PIT

### TO DO
- Improve std::string
- PCI Device Emumeration
- ACPI
- AHCI Disk Controller
- USTAR/FAT32 File Systems
- VBE
- RTC
- Language Interpreter
