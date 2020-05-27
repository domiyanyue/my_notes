# SYCL Hello World

SYCL is a royalty-free, cross-platform abstraction layer built on top of OpenCL which allows easy adoptation on a range of heterogeneous devices supporting OpenCL. SYCL language spec is based on mordern C++ (C++11 and beyond) and enables codes to be written in a single-source style completely in standard C++. 

Compared to OpenCL, SYCL provides serveral advantages:
* **C++ language**: Developers can leverage C++ features like template to make the code more expressible
and elegant. Also SYCL built-in APIs for queue, buffers are RAII types, which means developers has less book keeping job to manage the memory usage.

* **Single Source**: SYCL host and device code can exists in the same file. It's compilers' job to extract device code from the file and hand it off to the device backend. You don't have to call 2 compilers seperately on host and device anymore. This allows the project to be better orgnaized and deployed.

* **Implicit Data Movement Support**: Unlike OpenCL where a lot of boileplate code are needed for data
transfer between host and device, SYCL provide class "SYCL buffer" and runtime support to help automatically deduce memory movement. We will see it in the example.

In this article, we will look at 

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
In order to make sure the kernel code is targeting the correct platfrom (Intel, Nvdia, AMD, etc) and device (CPU, GPU, FPGA, etc), we have to specify a device selector. The default selector we use here will
target host device.

* **Setup Buffers between host and device**:
```C++
      buffer<float, 1> a_sycl(vec_a.data(), ArraySize);
      buffer<float, 1> b_sycl(vec_b.data(), ArraySize);
      buffer<float, 1> c_sycl(vec_c.data(), ArraySize);
```
In SYCL a buffer is used to maintain an area of memory that can be shared between the host and one or more devices. Here we instantiate a **buffer type** with two *template arguments*: data type `float` and 
data dimension `1`. Then we construct a **buffer instance** which takes two arguments: the first is the data and the second is an the number of elements. 
In this tutorial a buffer object of element type of `float` and dimension 1 with size `ArraySize` is created. 


* **Construct Command Queue**:
```C++
```
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

