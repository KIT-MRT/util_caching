# UTIL CACHING

THIS IS A TEST CHANGE AND CAN BE DELETED

This packages provides a utility class to simplify caching of arbitrary values.


## Usage

### Caching with number

You could cache a value with arbitrary type (e.g. double) giving number (e.g. double) as key, first value type is for the key.

```cpp
    Cache<double, double> cacheByNumber;
    double key1 = 1.0;
    cacheByNumber.cache(key1, 2.0);
```
Now the Cache stores the value `2.0` with key `1.0`. You could restore the value by matching the exact key. Which will return `true` and you can obtain the value by referencing with `.value()`

```cpp
    // exact match
    EXPECT_TRUE(cacheByNumber.cached(key1));
    EXPECT_DOUBLE_EQ(cacheByNumber.cached(key1).value(), 1.);
```

You can also do approximate matching by specifying one matching policy 

```cpp
    util_caching::policies::ApproximateNumber<double> approximateNumberPolicy{0.5};
```

with `0.5` threshold that the key should not differ with the stored key. Now you can recall the stored value giving the key:

```cpp
    double key2{1.2};
    // approximate match
    EXPECT_TRUE(cacheByNumber.cached(key2, approximateNumberPolicy));
    EXPECT_DOUBLE_EQ(cacheByNumber.cached(key2, approximateNumberPolicy).value(), 1.);
```
Of course the value can not be recalled when the keys differ more than the threshold:


```cpp
    double key3{1.6};
    // over threshold
    EXPECT_FALSE(cacheByNumber.cached(key3, approximateNumberPolicy));
```
 
### Caching with time point

A more practical usage is to cache values by giving time point as key:

```cpp
    using Time = std::chrono::steady_clock::time_point;
    Cache<Time, double> cacheByTime;
    Time time1 = std::chrono::steady_clock::now();
    cacheByTime.cache(time1, 1.);
```

Now you can recall the value by either do exact match:

```cpp
    // exact match
    EXPECT_TRUE(cacheByTime.cached(time1));
    EXPECT_DOUBLE_EQ(cacheByTime.cached(time1).value(), 1.);
```

or by specifying one comparison policy and threshold (100ms for example), and recall by approximate match:

```cpp
    Time time2 = time1 + 10ms;
    util_caching::policies::ApproximateTime<Time, std::chrono::milliseconds> approximateTimePolicy{100};
    // approximate match with miliseconds
    EXPECT_TRUE(cacheByTime.cached(time2, approximateTimePolicy));
    EXPECT_DOUBLE_EQ(cacheByTime.cached(time2, approximateTimePolicy).value(), 1.);
```

More usage please check the unittest.


## Installation

### Using Docker image

We provide a [`Dockerfile`](./Dockerfile) with the library already installed globally.

Clone or download this repository, then build and run the docker image with `docker compose`:

```bash
cd util_caching
docker compose build
docker compose run --rm util_caching
```

The library is installed in the Docker image under `/usr/local/include/util_caching/` and `/usr/local/lib/cmake/util_caching/`.
So, it can be easily loaded with CMake:

```cmake
find_package(util_caching REQUIRED)
```


### Building from source using CMake

First make sure all dependencies are installed:
- [Googletest](https://github.com/google/googletest) (only if you want to build unit tests)

See also the [`Dockerfile`](./Dockerfile) for how to install these packages under Debian or Ubuntu.

Compile and install the project with CMake:

```bash
mkdir -p util_caching/build
cd util_caching/build
cmake ..
cmake --build .
sudo cmake --install .
```


## Development

### Using Docker image

Follow the steps above to setup the Docker image.
Then, run the development image.

```bash
docker compose run --rm --build util_caching_devel
```

This mounts the source into the container's `/home/blinky/util_caching` folder.
There, you can edit the source code, compile and run the tests etc.


### Compiling unit tests using CMake

In order to compile with tests define `BUILD_TESTS=true`
```bash
mkdir -p util_caching/build
cd util_caching/build
cmake -DBUILD_TESTS=true ..
cmake --build .
```

Run all unit tests:

```bash
cmake --build . --target test
```


### Using ROS 1 and catkin

The [`demo/Dockerfile_ros`](./demo/Dockerfile_ros) shows that how util_caching can be used in a catkin project (it uses CMake under the hood anyways):

```bash
docker compose -f demo/docker-compose.ros.yaml build
docker compose -f demo/docker-compose.ros.yaml run --rm util_caching_ros
```

See [demo/README.md](demo/README.md) for how to run the demo, showcasing the use of `util_caching` in a ROS node.
