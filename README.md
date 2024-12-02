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
- `Sampling Rate`: The rate at which random floating point values are generated for each sensors
- `Runtime`: The total execution time of the program in **SECONDS**
- `Window Size`: The size of the moving average window which in turn increases/decreases the weight of each sample

Example Usage
--------------
➜  turion-space git:(master) ./build/sensor_simulator
```
➜  turion-space git:(master) ./build/sensor_simulator --help
Usage: ./build/sensor_simulator [OPTIONS]
Simulate sensor data with moving average processing.

Options:
  -s, --sensors <sensor_mask_hex>   Specify the sensor mask in hex, 6 bits (e.g., 21 for 0x21, sensors 6 and 1)(default=3F)
  -r, --rate <r1,r2,...>            Specify comma-separated sampling rates (milliseconds) for enabled sensors.(default=100)
  -w, --window <window_size>        Specify the window size for moving average.(default=5)
  -t, --runtime <runtime_sec>       Specify the runtime in seconds. (default=3)
  -h, --help                        Display this help and exit.

Examples:
  ./build/sensor_simulator -s 21 -r 100,200 -w 5 -t 2
  ./build/sensor_simulator --sensors 21 --rate 100,200 --window 5 --runtime 2

This program simulates sensor data with configurable sampling rates, window size, and runtime.
It calculates a moving average over the specified window size and outputs the results for each enabled sensor.
```
➜  turion-space git:(master) ./build/sensor_simulator --sensors 21 --rate 100,200 --window 5 --runtime 2
```
Filling Buffer...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sensor 1: Moving Average = 65.60 | ████████████████████████████████

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sensor 1: Moving Average = 64.16 | ████████████████████████████████

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sensor 1: Moving Average = 67.35 | █████████████████████████████████

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sensor 1: Moving Average = 60.93 | ██████████████████████████████

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sensor 1: Moving Average = 49.99 | ████████████████████████

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sensor 1: Moving Average = 53.56 | ██████████████████████████

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sensor 6: Moving Average = 56.79 | ████████████████████████████

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Cleanup complete.
```
