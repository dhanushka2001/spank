# spank-linux

Slap your Linux laptop, it yells back.

Inspired by **taigrr**'s [spank](https://github.com/taigrr/spank/) for MacBooks, featured in this viral [tweet](https://x.com/jlxc2001/status/2027444452848243075?s=20). Switched from Go to C++, and for Linux laptops instead of MacBooks.

Uses the laptop's accelerometer via the Linux IIO (Industrial I/O) subsystem to detect physical hits on your laptop and plays audio responses using [miniaudio](https://miniaud.io/index.html).

## Requirements

- Laptop running Linux and with an accelerometer (typically featured in 2-in-1 laptops like the HP Spectre/Pavilion x360 series).
- `g++`
- `cmake`
- `ninja` or `make`

## Install

```console
git clone https://github.com/dhanushka2001/spank-linux
```

```console
cd spank-linux
mkdir build
cd build
cmake .. -G Ninja
ninja
```

And run:

```console
./slap
```

## Debugging

Use this command to get list of IIO devices:

```console
grep -R . /sys/bus/iio/devices/iio:device*/name
```

Use this command to watch the value of the accelerometer (replacing ``iio:device3/in_accel_x_raw`` if needed):

```console
watch -n 0.05 cat /sys/bus/iio/devices/iio:device3/in_accel_x_raw
```

These settings can be tuned in ``main.cpp``, in the future I may make these input parameters:

```cpp
// tune these
double threshold = 500.0;
double upper_bound = 1000000.0;
std::chrono::milliseconds listening_time(700);
std::chrono::milliseconds sleep_time(200);
```
