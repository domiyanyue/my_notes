# Understanding SYCL Kernel Execution Model : Global Dimension and Local Dimension

In order to construct your SYCL code to best exploit underlying hardware's capability, a clear understanding of how parallelism is specifed is critical. The 2 important concepts are **global dimension** and **local dimension**. All related concepts should feel natural once you understand them.  

This tutorial will explain **global dimension** and **local dimension** first followed by code examples in SYCL to demonstrate how they are applied in SYCL device code.

## Global Dimension
A global dimension is a 1D, 2D or 3D parallelism defined for each kernel execution. It describes how the whole problem space is paritioned and assigned to **work-items**. A **work-item (thread)** is execute in every point in the global dimension. 

Let's look at some examples:  

**Example 1.** Processing 1024 audio pieces, each work-item deal with 1 audio piece. In this case the global dimension is 1024 and number of work items is 1024.

<p align="center"> 
<img src="1_d_global_dimension.png?raw=true"/> <br>
<em>Fig. 1: Example 1D Global Dimension</em>
</p>


**Example 2.** Processing HD videos of size 1920 x 1080. Depends on the compute task to solve and underlying hardware feature, you may come up with different ways to partition the problem. Let's consider 3 cases in which you will choose different global dimensions. 
  1. Process each pixel with one work-item. In this case, the global dimension is a 2-dimension value of 1920 x 1080, and total number of work-items is `2M ~= 1920 x 1080`. 

<p align="center"> 
<img src="2d_example1.png?raw=true"/> <br>
<em>Fig. 2: Example 2.1 2D Global Dimension 1920 x 1080</em>
</p>
  
  2. The compute task is performed per line, your program proces each line (1920 pixels) with one work-item. You only need 1080 work-items with global dimension (1D) of 1080.

<p align="center"> 
<img src="2d_example2.png?raw=true"/> <br>
<em>Fig. 3: Example 2.2 1D Global Dimension 1080</em>
</p>
  
  3. If your tasks is filter-like and each work-item deal with a 2 x 2 block. The global dimension is 960 (1920 / 2) x 540 (1080 / 2) and number of work-items is `518,400 = 480 x 270`.

<p align="center"> 
<img src="2d_example3.png?raw=true"/> <br>
<em>Fig. 4: Example 2.3 2D Global Dimension 960 x 540</em>
</p>

It should be clear to you at this point, that the choice of **global dimension** dictates how the problem space is divided and dispatched to work-items. The parallelism model is now well-defined, right? Well, not yet. Turns out we haven't considered one essential piece - *synchornization between work-items*. In most hardwares like multi-core CPUs or GPUs which SYCL runs on top of, 2 arbitrary work-items can't be synchronized easily. Only work-items executes on the same **compute unit** can be synchronized. Because of the hardware restriction, we have to be careful on selecting **local dimension**. 

## Local Dimension

In short, the local dimension breaks down global dimension into **work-groups** and only work-items within the same work-group can be synchronized. 

It is required in SYCL that local dimension size must be divisible by global dimension size in each dimension. For example if I have global dimensions of 32 x 100, I can't choose local dimension of 18 (not divisible by 32) x 10 or 16 x 15 (not divisible by 100), but values like 16 x 50, 8 x 25 are legal. 

Now we understand the rational of global dimension and local dimension. Let's how they look like in SYCL.

## Examples
Now let's check some code samples to see how global dimension and local dimension works.

### Example 1. Simple Vector Add : Not specifying work group size
This is typically used when multiple work-items are needed and no local synchronization arocess work group is required. 
The kernel is enqueued with `cl::sycl::parallel_for function` parameterized by a `cl::sycl::range` argument specifying the dimension
and size (in this example, 4 and 1). The work group size is decided by SYCL runtime. 

``` C++
...
      queue.submit([&] (handler& cgh) {
         auto a_acc = a_sycl.get_access<access::mode::read>(cgh);
         auto b_acc = b_sycl.get_access<access::mode::read>(cgh);
         auto c_acc = c_sycl.get_access<access::mode::discard_write>(cgh);

         cgh.parallel_for<class VectorAdd>(range<1>(4), [=] (item<1> iter) {
            c_acc[iter] = a_acc[iter] + b_acc[iter];
         });
      });
...
```




### Example 2. Reduce Sum




## Global Dimension in SYCL
In SYCL, when a kernel is submitted for execution, each instannce is a **work-item** and can be identified by a **global id** which is provided by the index space in SYCL called **ND-range**. A **ND-range** in SYCL defines the **global dimension**, N can 1, 2 or 3. Each kernel execute the same code but can use **global id** to specialize the computation. A typical example is to use **global id** to as array index to access only 1 item. 

## Local Dimension in SYCL
Local Dimension is represented using **work group**. **Work items** are organized into **work groups**. **Work group** provides a coarse-grained decomposition of the index-space. Each **work group** is assigned a unique **work group id** with the same dimensionality of as index space for global dimension. **Work items** in **work group** are assigned a uniqe **local id**. A work-item can be identified global with work group id and local id. The work items within the same work group executed on a single compute unit concurrently. 
