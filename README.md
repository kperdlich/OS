![Linux-Build](https://github.com/kperdlich/OS/actions/workflows/linux-build.yml/badge.svg)

# OS Sandbox
- ADS: Custom algorithm & data structures. Replacement for STL.
- Userland: List of user-space running applications.
    - LibGUI: Qt style GUI library which currently runs on Linux using SDL2. ![LibGUI](/Docs/Screenshot%20at%202024-08-22%2017-46-16.png)
  
- Kernel: Old i386 Kernel

## Kernel build instructions

### Prerequisites 
Install all dependencies
#### Ubuntu
```
sudo apt install build-essential cmake libmpfr-dev libmpc-dev libgmp-dev e2fsprogs ninja-build qemu-system-gui qemu-system-x86 qemu-utils
```


### Build Toolchain
In order to build the kernel you need to build the toolchain first using
```
./BuildToolchain.sh 
```




