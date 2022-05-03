## Disclaimer

Tested on Win11 x64, MSVC 14.29.30133, CMake 3.20.2

## Instructions

1. Run lab5.exe
2. Try not to break it

## Commands (variant 1)

Note: `PATH` - is a path inside VFS, `REAL_PATH` - path in real FS.

- `init [PATH]` - initializes VFS. If PATH is not specified, seeds test data
- `quit` - quit from program
- `save <REAL_PATH>` - saves VFS to the specified file
- `load <REAL_PATH>` - loads VFS from the specified file
- `pwd` - prints current working directory
- `cd <PATH>` - changed current directory. Supports absolute paths, relative paths (only without `./`) and `..` to change to parent dir.
- `ls` - prints content of current directory
- `rename <PATH> <NEW_NAME>` - renames the specified file. DOES NOT behave like `mv` in UNIX, it only renames file.
- `copy <SRC_PATH> <DST_PATH>` - copies file from SRC to DST
- `rm <PATH>` - deletes file
- `mount <REAL_PATH> <PATH>` - mounts real directory on virtual directory
- `umount <PATH>` - unmounts real directory