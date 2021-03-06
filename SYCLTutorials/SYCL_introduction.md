# Introduction to SYCL

Welcome to my SYCL tutorial! SYCL is a single source heterogeneous programming model built on top of OpenCL that allows programmers to write heterogeneous application completely using C++. If you are familiar with OpenCL, the concepts in this tutorial should be familiar to you and you can focus on what's new in SYCL. If you are not, don't worry, this won't require any background knowledge in OpenCL. 

## A Brief Background 
Heterogenous computing refers to systems that use more than one kind of processor. The types of processors can be combinations of CPU, GPU, FPGA, DSP, etc. In recently years(2010 -), the slowing down of Moore's Law has limited the speedup of general purpose (CPU-centric) compute platform. Many compute intensive tasks has switched to heterogeous computing including machine learning, image processing, natural language processing and big data application. 

Various of heterogenous programming model emerged to make programming for heterogeneous systems easy such as OpenCL, CUDA, OpenACC, etc. OpenCL is one of the widely adopted one by most hardware vendors. It has a well-defined execution model that is portable across all types of devices.  However, OpenCL suffered from three major complaints:

1. Limited support for C++. OpenCL is originally based on C99 standard. Developers do not benefit from new features in modern C++.  

2. The weak link between the host and device code is error-prone. Developers have to write in 2 different languages and compile host and device parts using different compilers. Often, users have to write their stringify script for purpose like code generation to automate the development process.

3. OpenCL is too verbose for many developers who don't want to explicitly write every low-level operation like memory transaction between host and device.

SYCL was born reactive to OpenCL's pros and cons and aimed to be better heterogeneous framework.
1. It inherited the good execution model of OpenCL.
2. SYCL is purely based on C++. 
3. SYCL is a single source (no separation of device and host) programming model.
4. SYCL extends the programming model that allows developers to express at a high level of abstraction.           

## Basic Structure of A Heterogeous Application
Before jumping into SYCL code, we need to understand the basic structure of a heteregeous application. 

## What Does SYCL Look Like? 
I will lead you through a simple SYCL code sample performing vector add. This will give you an idea of the structure of a SYCL application. Please don't pay too much attention to details but focus on the higher level concepts. Here is the code:

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
         auto c_acc = c_sycl.get_access<access::mode::write>(cgh);

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
Let's break it down to the basic build blocks of a SYCL application:
### Include Header Files
```C++
#include <CL/sycl.hpp>

using namespace cl::sycl;
```
SYCL applications must include `CL/sycl.hpp` which contains APIs for SYCL runtime types like queue, buffer, device, etc. They all live under `cl::sycl` namespace. For the simplicity of this example, we put `using namespace` command at the beginning of the file.

### Select Device
```C++
default_selector device_selector;
```
This is how you specify the device (CPU, GPU, FPGA, etc) to execute on. SYCL provides a `default_selector` that will select an existing device in the system. SYCL also provides `cpu_selector`, `gpu_selector`, and allow you to customize your selector. In this example, we choose `default_selector` which let runtime picks for us.

### Setup Buffers
```C++
{
      buffer<float, 1> a_sycl(vec_a.data(), ArraySize);
      buffer<float, 1> b_sycl(vec_b.data(), ArraySize);
      buffer<float, 1> c_sycl(vec_c.data(), ArraySize);
}
```
In SYCL, a buffer is used to maintain an area of memory that can be shared between the host and one or more devices. Here we instantiate a **buffer type** with two *template parameters*: data type `float` and data dimension `1`. We also construct a **buffer instance** with two arguments: the first is the data source and the second one is the number of elements. SYCL provides interfaces for constructing buffers from different types of data sources like `std::vector` or `C arrays`.  
In the first line of this example, we create a 1-dimensional buffer object of containing element `float` of size `ArraySize` and initialized it with data in `vec_a`. 

Notice in the code there is a scope `{}` around buffers. This scope defines the life-span of buffer. When the buffer is constructed inside the scope, it automatically gets the ownership of the data. When the buffer goes out of scope, it copies data back to `vec_a`, `vec_b` and `vec_c`. The memory movement between host and device is handled implicitly in the buffer's constructor and destructor. 

### Create Command Group
A command group is a single unit of work that will be executed on the device. You can see the command group
is passed as a functor (function object) parameter to to `submit` function. It accepts a parameter `handler` constructed by SYCL runtime which gives users the ability to access command group scope APIs. 
```C++
      queue.submit([&] (handler& cgh) { // start of command group
         // inputs and outputs accessor
         auto a_acc = a_sycl.get_access<access::mode::read>(cgh);
         auto b_acc = b_sycl.get_access<access::mode::read>(cgh);
         auto c_acc = c_sycl.get_access<access::mode::write>(cgh);
     
     // kernel function enqueue API `parallel_for`
         cgh.parallel_for<class VectorAdd>(range<1>(ArraySize), [=] (item<1> item) {
            c_acc[item] = a_acc[item] + b_acc[item];
         }); // end of command group
      });
```
In this case, the command group consists of a kernel function (defined by a kernel function enqueue API `parallel_for` which we will introduce later) and inputs and outputs defined by **accessor** object initialized by `get_access` API. I will cover kernel function and accessor in the following content.

### Construct Command Queue and Submit Command Group
```C++
...
    queue queue(device_selector);
    queue.submit([&] (handler& cgh) {
    ...
    }
```
A SYCL **queue** submits and triggers execution of **command group(s)** on a certain device. In this example, We first construct a
queue specifying the device it will submit to. Then we submit a command group to the device asynchronously. The submit command will return immediately and the execution of the command group will start later.

### Specify Accessors

**accessor** is the class to access buffer in SYCL. In this example, they are declared in command group to specify inputs and outputs from/to global memory.

```C++
         auto a_acc = a_sycl.get_access<access::mode::read>(cgh);
         auto b_acc = b_sycl.get_access<access::mode::read>(cgh);
         auto c_acc = c_sycl.get_access<access::mode::write>(cgh);
```
`<buffer>.get_access` returns accessor and gives it access to formerly created **buffer** objects `(a_sycl, b_sycl, c_sycl)`. For every accessor, there are three basic attributes:
1. **buffer**: memory it access which is determined when created. 
2. **access mode**: passed as template parameter. Typical values are read, write, read_write. This gives hints to the compiler to optimize the implementation. 
3. **command group handler**: the argument `cgh` indicates that the accessor will be available in kernel within this command group scope.

### Write Kernel Function
A kernel function is defined with 3 parts: data parallel model, kernel function body and kernel name. 
```C++
         cgh.parallel_for<class VectorAdd>(range<1>(ArraySize), [=] (item<1> item) {
            c_acc[item] = a_acc[item] + b_acc[item];
         });
```
In this example, data parallel model is defined by `parallel_for` API and `range` argument's type. Combined, they indicates the kernel will follow basic data parallel model which execute as multiple **work-items (threads)**. Other data parallel models are work-group data parallel, single task, and hierarchical data parallel.  

The kernel function body is encapsulated as a function object. It accepts an `item` as argument instructed by basic data parallel model. Inside the kernel function body, we add value from buffer `a_sycl` and `b_sycl` and save the result in `c_sycl` through accessors. Kernel argument `item` is **work-item id** (think of it as thread id) which tells the kernel that each execution of instance only add the value at **work-item id** position. Together, all threads will run in parallel to finish the work.

Kernel name is marked with `class VectorAdd`. Notice, it it a type and must be a class declared in global scope. 

## Summary

In this tutorial, we studied a "vector add" which includes basic elements of a SYCL program:
* Declare a SYCL queue object associated with certain devices.
* Use buffers to implicitly transfer ownership of memory between host and device.
* Write a command group that includes kernel function and accessors and submit it to the device.

We experienced SYCL's advantages as a programmnig model:
* **Support C++ language**: Developers can leverage C++ features to make the code more expressible. Also SYCL built-in APIs for queue, buffers are RAII types, which means developers has less bookkeeping job to control the life cycle of them.

* **Single Source**: SYCL host and device code can exist in the same file. You don't have to run 2 compiles separately on host and device anymore. This allows the project to be better organized and deployed.

* **Implicit Data Movement Support**: Unlike OpenCL and some other language where a lot of boilerplate code is needed for data
transfer between host and device, SYCL provides class "SYCL buffer" and runtime support to help automatically deduce memory movement.

The new terms introduced in this tutorial may be overwhelming at this point especially if you are new to heterogeneous computing. They will be covered in the following tutorials. Stay tuned! 


