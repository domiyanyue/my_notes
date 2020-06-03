#include <iostream>

#include <vector>

#include <CL/sycl.hpp>

class VectorAddReduction;

using namespace cl::sycl;

int main() {
    const int ArraySize = 8;
    const int WorkGroupSize = 4;
    std::vector < float > vec_nums {
        3.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f
    };

    default_selector device_selector;

    queue queue(device_selector);
    std::cout << "Running on " <<
        queue.get_device().get_info < info::device::name > () <<
        "\n"; {
        buffer < float, 1 > buff(vec_nums.data(), ArraySize);


        queue.submit([ & ](handler & cgh) {

            accessor < float, 1, access::mode::read_write, access::target::local > local_mem(range < 1 > (WorkGroupSize), cgh);
            auto global_mem = buff.get_access < access::mode::read_write > (cgh);


            cl::sycl::stream os(1024, 128, cgh);
            cgh.parallel_for < class VectorAddReduction > (nd_range < 1 > (ArraySize, WorkGroupSize),
                [ = ](nd_item < 1 > item) {
                    //   <<Perform load into local memory>>
                    size_t local_id = item.get_local_linear_id();
                    size_t global_id = item.get_global_linear_id();
                    os << "local id: " << local_id << cl::sycl::endl;
                    os << "global_id: " << global_id << cl::sycl::endl;
                    local_mem[local_id] = 0;

                    if ((2 * global_id) < ArraySize) {
                        local_mem[local_id] = global_mem[2 * global_id] + global_mem[2 * global_id + 1];
                    }

                    item.barrier(access::fence_space::local_space);
                    //   <<Reduce into one element>>
                    for (size_t stride = 1; stride < WorkGroupSize; stride *= 2) {
                        auto idx = 2 * stride * local_id;
                        if (idx < WorkGroupSize) {
                            local_mem[idx] = local_mem[idx] + local_mem[idx + stride];
                        }

                        item.barrier(access::fence_space::local_space);
                    }

                    //   <<Write group result to global memory>>
                    if (local_id == 0) {
                        global_mem[item.get_group_linear_id()] = local_mem[0];
                    }
                }
            );

        });
    }

    std::cout << "Sum: " << vec_nums[0] << std::endl;

    return 0;
}
