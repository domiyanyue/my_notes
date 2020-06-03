# Understanding SYCL Kernel Execution Model : Global Dimension and Local Dimension

In order to construct your SYCL device code for fast execution, a clear understanding of how parallelism is specifed is critical. The 2  important concepts are **global dimension** and **local dimension**. Once you know the rational behind them, all related concepts should feel natural. 

I will start this tutorial by explain **global dimension** and **local dimension** first followed by code examples in SYCL to demonstrates how they are applied in SYCL device code.

## Global Dimension
A global dimension is a 1D, 2D or 3D parallelism defined for each kernel execution. It describes how the whole problem space is paritioned and dispathed to work-item. A work-item (thread) is execute in every point in the global dimension. 

Let's take a few example:
**Example 1.** Processing 1024 audio pieces, each work-item deal with 1 audio piece. In this case the global dimension is 1024 and number of work items is 1024.



**Example 2.** Processing HD videos of size 1920 x 1080. Depends on the nature of problem and underlying hardware feature, you may come up with different ways to partition the problem. Let's consider 3 cases in which you will use different global dimensions. 
  1. Say you want to process each pixel with one work-item, in this case, the global dimension is a 2-dimension value of 1920 x 1080, and total work-item number is `2M ~= 1920 x 1080`.  
  
  2. If your compute task is performed per line, your compute task proces each line (1920 pixels) with one work-item. You only need 1080 work-items.
  
  3. If your tasks is filter-like and each work-item deal with a 4 x 4 block. The global dimension is 480 (1920 / 4) x 270 (1080 / 4) and number of work-items is `129,600 = 480 x 270`.

At this point, hope the concept is clear to you. Global dimension refects how the problem input is divided in a 1D/2D/3D way, a work-item deal with each unit of of the whole problem space. How to partition the problem is not a easy job which requires a lot of consideration. We will not cover that in this tutorial. 

Now we have dispatched the problem to each work-items and problem is solved, right? No yet. Turns out for more complicated problems, there is one important part we haven't consider, that is synchronization between work-items. In hardware design, the compute units which work-items works on can only be synchronized with compute units nearby with low cost. The reflect such hehaviour in software model, we need to introduce the concept of local dimension. 

## Local Dimension

The local dimension breaks down global dimension into **local work-groups**. It is required in SYCL that local dimension size must be divisible by global dimension size in each dimension. For example if I have global dimensions of 32 x 100, I can't choose local  dimension of 18 (not divisible by 32) x 10 or 16 x 15 (not divisible by 100), but values like 16 x 50, 8 x 25 are legal. Each work-group is logically executed together on one **compute unit**. Synchronization is only allowed between work-items within the same work-group. Why? Because hardware is easy to built in this way. This limitation also ask the developers to carefully select work-group size. 

Now we understand the rational of global dimension and local dimension. Let's how they look like in SYCL.

## Global Dimension in SYCL
In SYCL, when a kernel is submitted for execution, each instannce is a **work-item** and can be identified by a **global id** which is provided by the index space in SYCL called **ND-range**. A **ND-range** in SYCL defines the **global dimension**, N can 1, 2 or 3. Each kernel execute the same code but can use **global id** to specialize the computation. A typical example is to use **global id** to as array index to access only 1 item. 

## Local Dimension in SYCL
Local Dimension is represented using **work group**. **Work items** are organized into **work groups**. **Work group** provides a coarse-grained decomposition of the index-space. Each **work group** is assigned a unique **work group id** with the same dimensionality of as index space for global dimension. **Work items** in **work group** are assigned a uniqe **local id**. A work-item can be identified global with work group id and local id. The work items within the same work group executed on a single compute unit concurrently. 
