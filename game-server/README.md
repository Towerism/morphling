# Morphling Game Server
## Requirements
- CMake 2.8.12
- GCC 6
- libcurl

## Building
To build the game server, you must create the build tree, generate the build
files, and then run `make`.
```
$ mkdir build && cd build
$ cmake ..
$ make
```

By default the server is compiled using `Release` flags. To compile with debug
flags, generate the build files like so:
```
$ cmake -DCMAKE_BUILD_TYPE=Debug
```

## Running
After running `make`, you can run the game server from the build tree like so:
```
$ ./bin/morphling-game-server
```

## Testing
To run tests, after you've created the build tree, generate the build files with
testing enabled, and then you can use `make` to build and run the tests.
`ARGS=-VV` is necessary if you want to see exactly what tests failed.
```
$ cmake -DTEST=ON ..
$ make build-tests
$ make test ARGS=-VV
```
