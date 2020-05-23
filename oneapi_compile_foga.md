# Compile oneAPI program on Intel FPGA

oneAPI is a cross-industry, open, standards-based unified programming model that
delivers a common developer experience across accelerator architectures[1] including FPGA*.
In this article, we will look at how to compile your "hello world" program targeting FPGA.

## Terminologies
Even though oneAPI has abstracted away a lot of FPGA specific burderns in order to make the
development exeperience close to a general processer (CPU), developers still have to learn a
few things before getting start. 

* AOT (ahead-of-time) Compilation: oneAPI FPGA compiler requires offlline Unlike JIT (Just-in-Time)
compilation where device code is generated at runtime which is used in a lot of heterogeous compilers.
FPGA image are generated 
