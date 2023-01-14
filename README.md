# SWAPK-PROJEKT


```bash
sudo apt install ninja-build libsndfile-dev
cmake -B build -G Ninja
ninja -C build
```

## DSP pipeline

Inspiration Line 6 Helix, der kombinerer effekter i en pipeline, der sætter effekter sammen. 

Input vil være en lydfil (libsnd)   
Output vil være en lydfil (libsnd)

Run, kunne være compile-time.

```c++
int main() {
Pipeline<Input<"Input-file">, Paralel<Lowpass, Chorus>, Output<"Output-file">> p;
    p.run();
}
```
<img width="625" alt="Screenshot 2022-11-21 at 09 28 29" src="https://user-images.githubusercontent.com/73690332/203002043-c03c0da1-6301-4935-a62d-fbd8d51eb2d6.png">

## Indhold:

Templates:
1. Specialiseringer Y
1. Two phase lookup Y

Containers:
1. Iterator Y
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

# Block diagram initial 

Link til diagram: 
https://aarhusuniversitet-my.sharepoint.com/:u:/g/personal/au648612_uni_au_dk/EcsCmtTavkhBlJOlkQ9E_aQB1irTrvyG_Dfl7ikIvrYyEw?e=2HSrf7

<img width="929" alt="Screenshot 2022-12-01 at 10 17 33" src="https://user-images.githubusercontent.com/73690332/205014032-ded14ac7-09b6-4c2c-b044-7fb186780b44.png">
