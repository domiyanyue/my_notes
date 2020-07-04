# Move Semantics and Rvalue Reference in C++

Move semantics and rvalue reference are two powerful yet confusing features added in C++11. In this article, I will explain them from a programmers perspective:
1. What problem it tries to address. 
2. 

## Problem: Unnecessary Copy of Objects

One of C++'s advantages is it can produce fast programs. However, one problem has existed to slow down C++ programs C++11: unnecessary copy of objects.
Take a look at following example:

```C++
#include <vector>
using namespace std;

vector<int> createArray(int n){
  vector<int> ret;
  for(int i = 0; i < n; i++){
    ret.push_back(i * i);
  }
  return ret;
}

int main(){
  vector<int> vec_a = createArray(5);
}
```

Before C++11, the above code will generate terrible performance due to copy of array ret. There can be up to 2 copies, one is generated when return value is created in
function createArray, the second is when we assign the return value to `vec_a` in main function. The first copy can be avoid if compiler applies return value optimization. The second is unavoidable because a copy constructor of vector is called which will allocate memory space for vec_a and copy values from temproary values returned by function createArray. 


