# slap

Slap your laptop, it yells back.

Inspired by **taigrr**'s [spank](https://github.com/taigrr/spank/) for the MacBook featured in this viral [tweet](https://x.com/jlxc2001/status/2027444452848243075?s=20). Switched from Go to C++ and for laptops on Linux instead of MacBooks.

Uses the laptop's accelerometer via the Linux IIO (Industrial I/O) subsystem to detect physical hits on your laptop and plays audio responses using [miniaudio](https://miniaud.io/index.html).

## Requirements

- Laptop running Linux and with an accelerometer (typically featured in 2-in-1 laptops like the HP Spectre/Pavilion x360 series).
- `g++`
- `cmake`
- `ninja` or `make`

## Install

```console
git clone https://github.com/dhanushka2001/slap
```

```console
cd slap
mkdir build
cd build
cmake .. -G Ninja
ninja
```

And run:

```console
./slap
```

