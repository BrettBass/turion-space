# Real-Time Sensor Data Processor Simulation Module

Requirements
--------------
- Linux-based environment
- GNU Compiler
- Make
- Pthreads library for multi-threading

Project layout
--------------

    ├─ build/                   Compiled object files and final executable
    ├─ include/                 Header files
    │  ├─ utils.h/              Utility functions
    │  ├─ output.h/             Formatted print calls
    │  ├─ shared_memory.h/      IPC structure for shared memory
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

Example Output
--------------
➜  turion-space git:(master) ./build/sensor_simulator \
`Usage: ./build/sensor_simulator <sensor_mask_hex> <sampling_rate_ms> <runtime_sec> <window size> ` \
➜  turion-space git:(master) ./build/sensor_simulator 15 500 4 8 
```
Filling Buffer...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sensor 1: Moving Average = 33.97 | ████████████████

Sensor 2: Moving Average =  0.00 |

Sensor 3: Moving Average = 58.97 | █████████████████████████████

Sensor 4: Moving Average =  0.00 |

Sensor 5: Moving Average = 52.27 | ██████████████████████████

Sensor 6: Moving Average =  0.00 |

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sensor 1: Moving Average = 40.65 | ████████████████████

Sensor 2: Moving Average =  0.00 |

Sensor 3: Moving Average = 64.40 | ████████████████████████████████

Sensor 4: Moving Average =  0.00 |

Sensor 5: Moving Average = 61.77 | ██████████████████████████████

Sensor 6: Moving Average =  0.00 |

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Cleanup complete.
```
