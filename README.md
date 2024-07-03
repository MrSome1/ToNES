
## ToNES

Road to building an NES step by step


## Build

Using the typical cmake routine

```shell
export Qt5_DIR="~/Qt/5.15.2/gcc_64/lib/cmake/Qt5"
cd ${path to FoolNES}
mkdir build && cd build
cmake .. && make
```

Then find the binary file **ToNES** in folder **build**


## Run

```shell
ToNES ${path to .nes file}

# e.g.
ToNES examples/nes/example.nes
```


## TODO

- PPU
- APU
- Controller