# UTIL CACHING

This package cache arbitrary values which can be recalled by specifying a key


## Usage

### Caching with number

You could cache a value with arbitrary type (e.g. double) giving number (e.g. double) as key, first value type is for the key.

```cpp
    Cache<double, double> cacheByNumber;
    double key1 = 1.0;
    cacheByNumber.cache(key1, 2.0);
```
Now the Cache stores the value `2.0` with key `1.0`. You could restore the value by matching the exact key. Which will return `true` and you can obtain the value by referencing with `*`

```cpp
    // exact match
    EXPECT_TRUE(cacheByNumber.cached(key1));
    EXPECT_DOUBLE_EQ(*cacheByNumber.cached(key1), 1.);
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
    EXPECT_DOUBLE_EQ(*cacheByNumber.cached(key2, approximateNumberPolicy), 1.);
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
    EXPECT_DOUBLE_EQ(*cacheByTime.cached(time1), 1.);
```

or by specifying one comparison policy and threshold (100ms for example), and recall by approximate match:

```cpp
    Time time2 = time1 + 10ms;
    util_caching::policies::ApproximateTime<std::chrono::milliseconds> approximateTimePolicy{100};
    // approximate match with miliseconds
    EXPECT_TRUE(cacheByTime.cached(time2, approximateTimePolicy));
    EXPECT_DOUBLE_EQ(*cacheByTime.cached(time2, approximateTimePolicy), 1.);
```

More usage please chech the unittest.
