# C++ feature you know to know before start SYCL

SYCL is built on top of C++11, with feature support for C++14 and C++17.
For a lot of new comers who are somehow familiar with C/C++03, some language features like
lambda expressions are not obvous. This article will help you go through the basics of
new C++ feature you need to know to code in SYCL.

## functor
In C++, sometimes you want to pass in a function as a parameter to another function like defining your
own sorting algorithm. You can get this behaviour in C++ through function object **functor**.

You can define a functor and use it in this way. In the followin example, we define a class called myFunctor
and overload the operator () to be a function returning sum of 2 parameters passing in.

```C++
#include <iostream>

class myFunctor
{
    public:
        myFunctor () {}
        int operator() (int x, int y) { return x + y; }
};

int main(){
    myFunctor my_functor;
    std::cout << my_functor(1,2);
    return 0;
}
```
A more complicated and real word example to use functor is when we want to customize a comparison function
in std::sort.

```C++
#include <vector>
#include <iostream>

struct Student {
  int age;
  Student(int x) age(x) {}
};

struct MyCompare { 
   bool operator()(const Student &a, const Student &b) {
       return a.x < b.x;
   }
};

int main(){
  std::vector<Student> vecStudent{{1}, {3}, {2}};
  std::sort(vecStudent.begin(), vecStudent.end(), MyCompare);
  std::cout << vecStudent[1].age;
  return 0;
}
```


## lambda expression

## template keyword



