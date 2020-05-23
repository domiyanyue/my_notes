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


### Emulation Compile
```bash
dpcpp -fintelfpga <source_files> -o <executable_name>
```
Notice -fintelfpga is a magic flag that ecapsulate special recipes for FPGA including but not limited to:
* Do AOT (ahead-of-time) compilation instead of JIT.
* Support Intel FPGA language extensions.
* Generate intemediate files that's suitable for FPGA development flow.

### Generating Performace Report
```bash
dpcpp -fintelfpga -fsycl-link <source_files> -o <output> -Xshardware
```
The difference here is a new flag "-fsycl-link" and "-Xshardware". 
* -fsycl-link: What this flag does is to generate device code (code running on FPGA)
and stop at the point where there is enough information for performance report.
* -Xshardware: targeting FPGA hardware image instead of emulation.  

The output of this command has 2 parts:
* output.a (linux)/.lib (Windows): an archieve for intermediate files dumped at this stage. 
* output.prj: a folder containing a HTML based performace report in `output./prj/reports/report.html` 

### Create FPGA images
```bash
# Resume from previous steps
dpcpp -fintelfpga output.a/lib -o <executable> -Xshardware
# Start from source file
dpcpp -fintelfpga <source files> -o <executable> -Xshardware
```
The user can resume from previous intermediate files or directly from source file. 
