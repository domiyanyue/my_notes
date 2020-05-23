# Compile oneAPI program on Intel FPGA

oneAPI is a cross-industry, open, standards-based unified programming model that
delivers a common developer experience across accelerator architectures[1] including FPGA*.
In this article, we will look at how to compile your "hello world" program targeting FPGA.

## Terminologies
Even though oneAPI has abstracted away a lot of FPGA specific burderns in order to make the
development exeperience close to a general processer (CPU), developers with no FPGA experience
still have to learn a few things before getting start. 

* **AOT (ahead-of-time) Compilation**: Unlike CPU/GPU target where JIT (Just-in-Time) compilation
is supported by default. oneAPI FPGA compiler requires AOT (Ahead-of-Time) compilation
where device code is generated at compile time instead of runtime. The reason is simple: compiling
a program on FPGA usually takes hours, it doesn't make sense for user to hit the run button and wait
for hours. 

* **Emulation**: Emulation is the process of compiling same device and run it on CPU. Compiling a
program for emulation takes seconds compared to hours for FPGA. The purpose is to verify functionality
on a quick iteration before jumping to next step. 

* **Performance Report**: 
