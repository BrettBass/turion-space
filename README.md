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
➜  turion-space git:(master) ./build/sensor_simulator
Usage: ./build/sensor_simulator <sensor_mask_hex> <sampling_rate_ms> <runtime_sec> <window size>
➜  turion-space git:(master) ./build/sensor_simulator 15 250 3 5
Filling Buffer...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sensor 1: Moving Average = 29.06 | ██████████████

Sensor 2: Moving Average =  0.00 |

Sensor 3: Moving Average = 36.57 | ██████████████████

Sensor 4: Moving Average =  0.00 |

Sensor 5: Moving Average = 37.49 | ██████████████████

Sensor 6: Moving Average =  0.00 |

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sensor 1: Moving Average = 35.96 | █████████████████

Sensor 2: Moving Average =  0.00 |

Sensor 3: Moving Average = 39.83 | ███████████████████

Sensor 4: Moving Average =  0.00 |

Sensor 5: Moving Average = 43.08 | █████████████████████

Sensor 6: Moving Average =  0.00 |

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
