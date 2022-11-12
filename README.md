# CacheSimulator

This cache simulator mimics the behavior of a hardware cache subsystem. A cache is used to reduce the average cost of accessing main mamory from the processor.

## Inputs
There are 4 inputs

```bash
1. nk: the capacity of the cache in kilobytes (an int)
2. assoc: the associativity of the cache (an int)
3. blocksize: the size of a single cache block in bytes (an int)
4. repl: the replacement policy (a char); 'l' means LRU, 'r' means random.
```

## Outputs
There are 6 outputs
```bash
1. The total number of misses,
2. The percentage of misses (i.e. total misses divided by total accesses),
3. The total number of read misses,
4. The percentage of read misses (i.e. total read misses divided by total read accesses),
5. The total number of write misses,
6. The percentage of write misses (i.e. total write misses divided by total write accesses).
```

<!-- ## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

Please make sure to update tests as appropriate. -->

<!-- ## License

-->