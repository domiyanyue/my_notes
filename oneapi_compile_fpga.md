# Compile oneAPI program on Intel FPGA

oneAPI is a cross-industry, open, standards-based unified programming model that
delivers a common developer experience across accelerator architectures[1] including FPGA*.
In this article, we will look at how to compile your "hello world" program targeting FPGA using
Intel oneAPI compiler.

Overall oneAPI compiler command is similar to other C++ language like GCC/Clang. 

## Terminologies
 If you are new
to oneAPI and FPGA development, 
Even though oneAPI has abstracted away a lot of FPGA specific burderns in order to make the
development exeperience close to a general processer (CPU), developers with no FPGA experience
still have to learn a few things before getting start. 

* **AOT (ahead-of-time) Compilation**: Unlike CPU/GPU target where JIT (Just-in-Time) compilation
is supported by default. oneAPI FPGA compiler requires AOT (Ahead-of-Time) compilation
where device code is generated at compile time instead of runtime. The reason is simple: compiling
a program on FPGA usually takes hours, it doesn't make sense for user to hit the run button and wait
for hours. 

* **Emulation**: Emulation is the process of compiling same device code and run it on CPU (emulate the
program behaviour on a CPU). Compiling a
program for emulation takes seconds compared to hours for FPGA. The purpose is to verify functionality
on a quick iteration before jumping to next step. 

* **Performance Report**: Despite long compiling time, some key metrics of the performance indicators
can be generated in early compilation stages (minutes to generate report vs hours full compilation).
The users are expected to use those indicate to evaluate if their design can meet the performance requirements. 

## Development Flow Overview
You probably have noticed the new concepts added in previous part are trying to address the same issue: long
compiling time. In order to make the overtall development faster, 2 turn around points were introduced:
* Emulation
* Performance Report Generation

The recommanded development process can be illustrated as:

[pic]

What's new for FPGA is the 2 extra steps added:
* Verify functionality using emulator
* Generate performance report

## Using DPC++ Compiler
Now we have a high level view of the overall process. This part is to execute each step using 
DPC++ compiler.



