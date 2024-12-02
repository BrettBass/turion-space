# Real-Time Sensor Data Processor Simulation Module

This project implements a real-time sensor data processing module for a spacecraft, running on an embedded Linux system. The system collects data from multiple (1-6) sensors, calculates their moving averages over a configurable window size, and prints the results in real-time. This project uses circular buffers for efficient memory management and multi-threading using pthreads for concurrent data processing.

Features
---------
- **Data Acquisition**: Simulates sensor data by generating random floating-point values for each sensor. This generations happens on a configurable sampling rate sampling rate and stored in a buffer.
- **Real-Time Analysis**: Continuously processes the moving average of the sensor readings over a configurable window size thought a circular buffer.
- **Inter-Process Communication**: Uses pthreads along with shared memory to handle concurant data processing and output.
- **Output**: Prints the real-time moving average of each sensors's data whenever an update takes place.
- **Shutdown**: The program runs for a configurable configurable amount of time and then gracfully shuts down by deallocating memory and terminating all pthreads.

Requirements
--------------
- Linux-based environment
- GNU Compiler
- Make
- Pthreads library for multi-threading

Installation
--------------
### Steps
1. Clone the repository (if applicable)
```
git clone https://github.com/BrettBass/turion-space
cd turion-space
```
2. Build the project
```
make
```
This will compile the source code and produce an executable in the `build/` directory

Project layout
--------------

    ├─ build/                   Compiled object files and final executable
    ├─ include/                 Header files
    │  ├─ utils.h/              Utility functions
    │  ├─ shared_memory.h/      IPC structure for shared memory
    │  ├─ moving_average.h/     Processing Module
    │  └─ sensor_simulator.h/   Simulation Module
    ├─ src/                     Source Files
    │  ├─ main.c/               Entry point
    │  ├─ utils.c/              Utility definitions
    │  ├─ moving_average.c/     Processing Logic
    │  └─ sensor_simulator.c/   Simulation Logic
    ├─ Makefile                 Build script
    └─ README.md                this file

Command Line Arguments
--------------
The Program uses 4 configuration parameters passed through command line
- `Sensor Mask`: A 6 bit mask passed by **HEX** used to specify which sensors are running
- `Sampling Rate`: The rate at which random floating point values are generated for the sensors
- `Runtime`: The total execution time of the program in **SECONDS**
- `Window Size`: The size of the moving average window which in turn increases/decreases the weight of each sample

Example Usage
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
