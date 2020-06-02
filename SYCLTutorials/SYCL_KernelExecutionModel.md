# SYCL Kernel Execution Model

The first important concepts are global and local dimensions. 

Parallelism is defined by the 1D, 2D, 3D global dimension for each kernel execution. To understand
and describe the problem space, we have to introduce **nd-range** first. An **ND-range** is a N-dimensional index space where N can be one, two or three.

The kernel instance is called a **work-item**. Each work item has a globally identifier called **global id**. A **work-item** (thread) is executed on every point of the global index space. Let look at the following examples:

1. Processing 1k sentences, each work-item (thread) can process 1 sentence. In this case, the problem space is 1D global dimension of size 1024. For each sentence, I will need a thread, in total I need 1024 work-items.

2. Processing HD videos (1920 x 1080), each work-items can deal with pixels at a specific location. In this case, the space is 2D global dimension of size 1920 x 1080. The total number of work-items needed is 2M (= 1920 X 1080).

3. 

What global dimension to choose and how many work-items is needed is dependent on how the problem is modeled.


TODO: put a table here

Please notice, work-items is a logically concept, 2M numbers of work-items does not mean the application needs 2M physical compute units. The mapping between work-items and physical devices is handled by SYCL runtime.

We know what's global dimension, now let's look at local dimensions. The global dimension is breakdown **evenly** into **local work-groups**. For example, if the global dimension is 60 x 128. The local dimension can be 10 x 16, 20 x 64 but not 15 x 17 (17 is not divisable by 128) or 40 (40 is not divisable by 60) x 32. 

Why do we need global dimension and local dimension? Turns out work-items within the same group Typically, Work-items in the same local dimension can be synchronzied. This is bad for software developers because they have to be careful on work groups for synchronization but good for hardware developers because enabling sychronization globally is hard, even if is possible, it usually lead to poor performance.



