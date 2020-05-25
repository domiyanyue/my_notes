# C++ feature you need to know for SYCL development

SYCL is built on top of C++11, with feature support for C++14 and C++17.
For new comers who are somehow familiar with C/C++03 but not newer C++ version, 
some language features like lambda expressions can be unfamiliar. This article will help you go through the basics of
new C++ feature you need to know to code in SYCL.

## Functor
In C++, sometimes you want to pass in a function as a parameter to another function like defining your
own sorting algorithm. You can get this behaviour in C++ through **function object (aka functor)**.

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
in std::sort. In the following example, we need a customized function to compare 2 Student struct according to
attribute age. We define and pass in a functor `comparitor` to std::sort as it requires. 

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
  MyCompare comparitor;
  std::sort(vecStudent.begin(), vecStudent.end(), comparitor);
  std::cout << vecStudent[1].age;
  return 0;
}
```
Sometimes we contruct functor when pass in function without declare it like:
```C++
  std::sort(vecStudent.begin(), vecStudent.end(), MyCompare{});
```

In SYCL, we need to use functor to wrap around device code.

## lambda expression
Lambda expression is a syntactic short-cut for functor, you can replace functor with lambda
expression. With lambda expression, usually you can write less code. Think about the previous
student example, in order to do comparison, we added some boilerplate code. Using lambda expression
can simply:
```C++
# using functor
struct MyCompare { 
   bool operator()(const Student &a, const Student &b) {
       return a.x < b.x;
   }
};
...
  MyCompare comparitor;
  std::sort(vecStudent.begin(), vecStudent.end(), comparitor);
```
to:
```C++
# using lambda
   auto comparitor = [](const Student &a, const Student &b) { return a.x < b.y; };
   std::sort(vecStudent.begin(), vecStudent.end(), comparitor);
```
or:
```C++
    std::sort(vecStudent.begin(), vecStudent.end(), [](const Student &a, const Student &b) { return a.x < b.y; });
```
Now let's look into the details of lambda expression.
The basic syntax for lambda expression looks like:
```
[ captures ] (parameters) -> returnTypesDeclaration { lambdaStatements; }
```
*\[capture\]*: The capture clause, also known as the lambda introducer, specifies which outside variables are available
for the lambda function and whether they should be captured by value (copying) or by reference. You will always be able
to identify the start of a lambda expression with the presence of the capture clause. An empty capture clause [] means
capture nothing, in which case the lambda expression body doesn't access variables in the enclosing scope. 

*parameters*: This is the optional parameters list, also known as the lambda declarator. You can omit the parameters
list if you want a function that takes zero arguments.

*returnTypesDeclaration*: This is the return type. Most of the time, compilers can deduce the return type of the lambda
expression when you have zero or one return statement. However, if it makes it easier to understand the code, you can
specify the return type.

*{ lambdaStatements; }*: This is the lambda body. The statements within the lambda body can access the captured variables
and the parameters.

Let's look at some concrete example to help understand:
* Example 1
```C++
   auto comparitor = [](const Student &a, const Student &b) { return a.x < b.x; };
```
One detail we notice is the type of a lamdba expression is auto. In fact it's a compile-time generated type, as user we
always put auto when declare it. Here the lambda expression captures nothing and take 2 parameters, the return type is
automatically deduced by the compile based on expression inside lambda body. If we want to be more explict on return type, 
we can write it as:
```C++
    auto comparitor = [](const Student &a, const Student &b) -> bool { return a.x < b.x; };
```

Capture is a new concept, it provides an easy way to pass in parameters with the scope to lambda expression. 



## template keyword



