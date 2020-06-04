# Introduction to SYCL

SYCL is a single source heteogeous programming model built on top of OpenCL that allows programmers write heterogenous application completely using C++. If you are familiar with OpenCL, the concepts in this tutorial should be familiar to you and you can mostly focusing on what's new in SYCL. If you are not, don't worry, this won't require any background knowledge in OpenCL. 

## A brief history and background 
Heterogenous Computing refer to systems that uses more than one kind of processors (CPU + GPU, CPU + FPGA, CPU + DSP). 



SYCL is a royalty-free, cross-platform abstraction layer built on top of OpenCL. SYCL language spec is based on mordern C++ (C++11 and beyond). This enables codes to be written in a single-source style completely in standard C++. 

Compared to OpenCL, SYCL provides serveral advantages:
* **C++ language**: Developers can leverage C++ features like template to make the code more expressible
and elegant. Also SYCL built-in APIs for queue, buffers are RAII types, which means developers has less book keeping job to control the life cycle of them.

* **Single Source**: SYCL host and device code can exists in the same file. It's compilers' job to extract device code from the file and hand it off to the selected device backend compiler. You don't have to run 2 compiles seperately on host and device anymore. This allows the project to be better orgnaized and deployed.

* **Implicit Data Movement Support**: Unlike OpenCL where a lot of boileplate code are needed for data
transfer between host and device, SYCL provide class "SYCL buffer" and runtime support to help automatically deduce memory movement. We will see it in the example.

In this article, we will look at a simple example that performs element wise sum.

## Example: VectorAdd
```C++
#include <iostream>
#include <vector>
#include <CL/sycl.hpp>

using namespace cl::sycl;
class VectorAdd;

int main() {
   const int ArraySize = 4;
   std::vector<float> vec_a{1.0f, 2.0f, 3.0f, 4.0f};
   std::vector<float> vec_b{5.0f, 6.0f, 7.0f, 8.0f};
   std::vector<float> vec_c(ArraySize);

   default_selector device_selector;

   queue queue(device_selector);
   std::cout << "Running on "
             << queue.get_device().get_info<info::device::name>()
             << "\n";
   {
      buffer<float, 1> a_sycl(vec_a.data(), ArraySize);
      buffer<float, 1> b_sycl(vec_b.data(), ArraySize);
      buffer<float, 1> c_sycl(vec_c.data(), ArraySize);
  
      queue.submit([&] (handler& cgh) {
         auto a_acc = a_sycl.get_access<access::mode::read>(cgh);
         auto b_acc = b_sycl.get_access<access::mode::read>(cgh);
         auto c_acc = c_sycl.get_access<access::mode::discard_write>(cgh);

         cgh.parallel_for<class VectorAdd>(range<1>(ArraySize), [=] (item<1> item) {
            c_acc[item] = a_acc[item] + b_acc[item];
         });
      });
   }

   for(int i = 0; i < ArraySize; i++){
       std::cout << vec_a[i] << " + " << vec_b[i] <<  " = " << vec_c[i] << std::endl;
   }
		
   return 0;
}

```
Let's take a quick look at essential building blocks for a SYCL program:
* **Header Files Inclusion**: 
```C++
#include <CL/sycl.hpp>

using namespace cl::sycl;
```
SYCL applications must include `CL/sycl.hpp` which contains APIs for SYCL runtime types like queue,
buffer, device, etc. They all live under `cl::sycl` namespace. For the simplicity of this example, we put `using namespace` command at the beginning of the file.

* **Select Device**:
```C++
default_selector device_selector;
```
This is the place where you want specfy which device (CPU, GPU, FPGA, etc) SYCL kernel execute on. SYCL provde a `default_selector` that will select a existing device in the system. SYCL also use `cpu_selector` or `gpu_selector`.

* **Setup Buffers between host and device**:
```C++
      buffer<float, 1> a_sycl(vec_a.data(), ArraySize);
      buffer<float, 1> b_sycl(vec_b.data(), ArraySize);
      buffer<float, 1> c_sycl(vec_c.data(), ArraySize);
```
In SYCL a buffer is used to maintain an area of memory that can be shared between the host and one or more devices. Here we instantiate a **buffer type** with two *template arguments*: data type `float` and 
data dimension `1`. Then we construct a **buffer instance** which takes two arguments: the first is the data and the second is an the number of elements. 
In this tutorial a buffer object of element type of `float` and dimension 1 with size `ArraySize` is created. 

* **Command Group**

* **Construct Command Queue and Submit Command Group**:
```C++
```
Command queue in SYCL submit a command group

* **Specify Accessors**:
```C++
         auto a_acc = a_sycl.get_access<access::mode::read>(cgh);
         auto b_acc = b_sycl.get_access<access::mode::read>(cgh);
         auto c_acc = c_sycl.get_access<access::mode::discard_write>(cgh);
```


* **Kernel Function**:
```C++
         cgh.parallel_for<class VectorAdd>(range<1>(ArraySize), [=] (item<1> item) {
            c_acc[item] = a_acc[item] + b_acc[item];
         });
```

## Summary
In this article, we demonstrate the basics of SYCL by going through a simple example. It covers
a complete SYCL application including:
* construct host program
* write kernel function
* launch kernel from host

