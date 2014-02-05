# numba

numba is a tool designed for analyzing a set of numbers. It can find averages, modes, the median, and more, given a set of numbers. It is also capable of sorting numbers from smallest to largest and from largest to smallest.

## Building numba

Before building numba from source, run `autogen.sh`. I recommend creating a subdirectory to build numba, and configuring from there:

```
mkdir build
cd build
../configure
```

To compile, and then install numba:

```
make
make install
```

## Does it have any limitations?

Yes, numba has some limitations. Currently, floating-point numbers are not supported, and numba cannot read a set of numbers from a text file. These features will likely be added in the future.

## Can I contribute?

Of course! I welcome contributions, even if it is only a bug report. numba is licensed under the MIT license, which allows you to even use numba's code in your own projects!