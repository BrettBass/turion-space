# Real-Time Sensor Data Processor Simulation Module

Project layout
--------------

    ├─ build/                   Compiled object files and final executable
    ├─ include/                 Header files
    │  ├─ shared_memory.h/      IPC structure for shared memory
    │  ├─ utils.h/              Utility functions
    │  ├─ output.h/             Formatted print calls
    │  ├─ moving_average.h/     Processing Module
    │  └─ sensor_simulator.h/   Simulation Module
    ├─ src/                     Source Files
    │  ├─ main.c/               Entry point
    │  ├─ utils.c/              Utility definitions
    │  ├─ output.c/             Formatted print calls
    │  ├─ moving_average.c/     Processing Logic
    │  └─ sensor_simulator.c/   Simulation Logic
    ├─ Makefile                 Build script
    └─ README.md                this file
