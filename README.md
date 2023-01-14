# SWAPK-PROJEKT

This project is from the 5 ECTS course Advanced Programming Concepts that covers many parts of modern C++ features. The project implements an audio effect pipeline that can be configured in an abitrary way. It uses a lot of template metaprogramming as well as concepts in an effort to make it "difficult to use wrongly", however there are still additions that can be made in the future. 

## How to build with ninja

```bash
sudo apt install ninja-build libsndfile-dev
cmake -B build -G Ninja
ninja -C build
```

## DSP pipeline

The goal of the project was to be able to configure a Pipeline by using a statement like: 

```c++
int main() {
Pipeline<Input<"Input-file">, Paralel<Lowpass, Chorus>, Output<"Output-file">> p;
    p.run();
}
```
The project is inspired by the product Line6 Helix which is a multi fx guitar board: 

<img width="625" alt="Screenshot 2022-11-21 at 09 28 29" src="https://user-images.githubusercontent.com/73690332/203002043-c03c0da1-6301-4935-a62d-fbd8d51eb2d6.png">

## Course content that is used in the code:

Templates:
1. Specialisations Y
1. Two phase lookup Y

Containers:
1. Iterators Y
1. Container types Y
1. Algorithms Y

Exceptions:
1. Garanties  Y
1. Exceptions Y

C++11++:
1. Literals Y
1. Moving Y
1. If constexpr Y
1. Promises 
1. Threading Y
1. Variadics Y

Boost:
1. boost::signals2
1. std::variant Y
1. std::function Y
1. std::bind Y

Metaprogrammering og concepts: 
1. Typelists Y
1. Concepts Y

PMR
1. Memory resource Y
1. PMR::vector Y

General:
1. Lambda Y
1. Stongly typed enums Y
1. For loops Y
1. Explicit vs implicit Y
1. Delegating constructors
1. auto Y
1. RAII Y
1. Namespaces Y

# Architecture overview

![Arkitektur](https://user-images.githubusercontent.com/73690332/212496763-795e5dc6-d991-4253-80be-121b0a85eeba.png)
