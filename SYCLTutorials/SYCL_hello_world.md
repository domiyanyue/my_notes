# Introduction to SYCL

Welcome to my SYCL tutorial! SYCL is a single source heteogeous programming model built on top of OpenCL that allow programmers to write heterogenous application completely using C++. If you are familiar with OpenCL, the concepts in this tutorial should be familiar to you and you can focus on what's new in SYCL. If you are not, don't worry, this won't require any background knowledge in OpenCL. 

## A Brief Background 
Heterogenous computing refer to systems that uses more than one kind of processors (CPU + GPU, CPU + FPGA, CPU + DSP). To make programming for heterogeous system easy, people have come up with different programming models including OpenCL, CUDA, OpenACC, etc. 
OpenCL is one of the widely adopted one. It has a well-defined execution model that is portable across all type of devices. 
However, OpenCL also received a lot of complains:
1. Limited support for C++. Developers do not benefit from new features in modern in C++.  
2. The weak link between host and device code is error-prone. Developers have to write in 2 different languages and compile host and device part using different compilers. Oftenly, users have to write their stringgify script to automate the development process.
3. OpenCL is too verbose for many developers who don't want to explictily write every low-level operation like memory transaction between host and device.

SYCL was born reactive to OpenCL's pros and cons and aimed to a better heterogenous framework.
1. It inheritant the good execution model of OpenCL.
2. SYCL is purely based on C++. 
3. SYCL is a single source (no serparation of device and host) programming model that allow developers to express at a high level of abstraction.            

## What does SYCL looks like : Example Simple Vector Add
I will lead you through a simple SYCL code sample. Hopefully this gives you an idea of the structure of a SYCL application.
Please don't pay too much attention to details, I will mention them but just for the purpose of understanding the basics.
Notice, it is important that you know know C++ functor and lambda express which is described here. 

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
* **Header Files Inclusion**
```C++
#include <CL/sycl.hpp>

using namespace cl::sycl;
```
SYCL applications must include `CL/sycl.hpp` which contains APIs for SYCL runtime types like queue, buffer, device, etc. They all live under `cl::sycl` namespace. For the simplicity of this example, we put `using namespace` command at the beginning of the file.

* **Select Device**
```C++
default_selector device_selector;
```
This is how you specfy device (CPU, GPU, FPGA, etc) to execute on. SYCL provde a `default_selector` that will select a existing device in the system. SYCL also provide `cpu_selector`, `gpu_selector` and allow you to customize your selector. In this example, we will `default_selector` which use the device runtime pick for us.

* **Setup Buffers between host and device**
```C++
      buffer<float, 1> a_sycl(vec_a.data(), ArraySize);
      buffer<float, 1> b_sycl(vec_b.data(), ArraySize);
      buffer<float, 1> c_sycl(vec_c.data(), ArraySize);
```
In SYCL, a buffer is used to maintain an area of memory that can be shared between the host and one or more devices. Here we instantiate a **buffer type** with two *template arguments*: data type `float` and data dimension `1`. We also construct a **buffer instance** with two arguments: the first is the data source and the second one is an the number of elements. SYCL provide interfaces for constructing buffers from different types of data sources like `std::vector` or `C arrays`.  
In tthe first line of this example, we create a 1 dimensional buffer object of containing element `float` of size `ArraySize` and initiliza it with data in `vec_a`. 

* **Command Group**
A command group is a single unit of work that will be executed on device. You can see the command group
is passed as a functor (function object) parameter to to `submit` function. It also accepts a parameter `handler` constructed by SYCL runtime which gives user ability to access command group scope APIs. 
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
In this example and most cases, the command group consists of a kernel function (defined by a kernel function enqueue API `parallel_for` which we will introduce later) and inputs and outputs defined by **accessor** object initilized by `get_access` API. 

* **Construct Command Queue and Submit Command Group**
```C++
...
	queue queue(device_selector);
	queue.submit([&] (handler& cgh) {
	...
	}
```
A SYCL **queue** connects **command groups** with certain device. In this example, We first construct a
queue specifying all **command groups** submited by this queue will run on `device_selector` (argument). Then we submit a command group to the device asynchronously. The submit command will return immediately and the execution of command group will start later.

* **Specify Accessors**
```C++
         auto a_acc = a_sycl.get_access<access::mode::read>(cgh);
         auto b_acc = b_sycl.get_access<access::mode::read>(cgh);
         auto c_acc = c_sycl.get_access<access::mode::write>(cgh);
```
Above commands create **accessors** which read from/write to memory. In this case, it gives us access to formerly created **buffer** objects. **accessor** is the only way to access buffer in SYCL. When creating an accessor, we have to provide:
1. `access mode` (read, write, read_write, etc.) as template parameter to specify the baivour of accessor. Compiler can optimizate the code if it knows the accessor's limitation -- i.e., read only or write only.
2. A **command group handler** `cgh` which indicates that the accessor will be available in kernel within this command group scope. This looks strange to me at the beginning. it hascertain language concern, for now, you don't have to worry too much about it. Just remeber to pass in the handler when creating an accessor. 

* **Kernel Function**:
```C++
         cgh.parallel_for<class VectorAdd>(range<1>(ArraySize), [=] (item<1> item) {
            c_acc[item] = a_acc[item] + b_acc[item];
         });
```
Kernel code is defined as a lambda expression, `parallel_for` here is a API defining the execution model of the kernel, `class VectorAdd` is a templated parameter by design to give the kernel a name. You can see we don't have a defin

SYCL is a royalty-free, cross-platform abstraction layer built on top of OpenCL. SYCL language spec is based on mordern C++ (C++11 and beyond). This enables codes to be written in a single-source style completely in standard C++. 

Compared to OpenCL, SYCL provides serveral advantages:
* **C++ language**: Developers can leverage C++ features like template to make the code more expressible
and elegant. Also SYCL built-in APIs for queue, buffers are RAII types, which means developers has less book keeping job to control the life cycle of them.

* **Single Source**: SYCL host and device code can exists in the same file. It's compilers' job to extract device code from the file and hand it off to the selected device backend compiler. You don't have to run 2 compiles seperately on host and device anymore. This allows the project to be better orgnaized and deployed.

* **Implicit Data Movement Support**: Unlike OpenCL where a lot of boileplate code are needed for data
transfer between host and device, SYCL provide class "SYCL buffer" and runtime support to help automatically deduce memory movement. We will see it in the example.

In this article, we will look at a simple example that performs element wise sum.


## Summary
In this article, we demonstrate the basics of SYCL by going through a simple example. It covers
a complete SYCL application including:
* construct host program
* write kernel function
* launch kernel from host

