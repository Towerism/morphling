# Morphling Game Server
## Building
To build the game server, you must create the build tree, generate the build
files, and then run `make`.
```
$ mkdir build && cd build
$ cmake ..
$ make
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