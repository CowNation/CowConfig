# C++
clang++-7 -pthread -o main.exe Examples/cpp-Example.cpp
./main.exe
rm main.exe

# CSharp
mcs -out:main.exe Examples/csharp-Example.cs Source/csharp-CowConfig.cs
mono main.exe
rm main.exe