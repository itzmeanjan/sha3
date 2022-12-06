## Benchmarking SHA3 on CPU

For benchmarking SHA3 hash functions and extendable output functions, on CPU systems, using `google-benchmark`, issue following command

> **Warning**

> For disabling CPU scaling during benchmarking, follow [this](https://github.com/google/benchmark/blob/60b16f1/docs/user_guide.md#disabling-cpu-frequency-scaling)

> **Note**

> During benchmarking a byte array of length N is used as input | 32 <= N <= 4096 and N = 2^i

> When benchmarking extendable output functions ( XoFs ), output of 32, 64 and 128 -bytes are squeezed from sponge ( s.t. single byte is read at a time to demonstrate performance in worst case scenario ), for each input byte array of length N | 32 <= N <= 4096 and N = 2^i

```fish
make benchmark
```

> **Note**

> On x86_64 architecture average CPU cycle is measured using compiler built-in `__rdtsc`. I've tested it with both Clang and GCC.

### On Intel(R) Core(TM) i5-8279U CPU @ 2.40GHz ( using `clang++` )

```fish
2022-12-05T17:58:52+04:00
Running ./bench/a.out
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB
  L1 Instruction 32 KiB
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB
Load Average: 1.96, 2.35, 2.12
------------------------------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations average_cpu_cycles bytes_per_second
------------------------------------------------------------------------------------------------------------
bench_sha3::keccakf1600              340 ns          340 ns      2065256                798       561.678M/s
bench_sha3::sha3_224/32              358 ns          358 ns      1961389                840       85.2962M/s
bench_sha3::sha3_224/64              353 ns          352 ns      1959034                829       173.213M/s
bench_sha3::sha3_224/128             358 ns          358 ns      1952814                839       341.234M/s
bench_sha3::sha3_224/256             702 ns          701 ns       992753             1.666k       348.264M/s
bench_sha3::sha3_224/512            1382 ns         1381 ns       507474             3.298k        353.63M/s
bench_sha3::sha3_224/1024           2737 ns         2734 ns       251394             6.549k       357.232M/s
bench_sha3::sha3_224/2048           5090 ns         5085 ns       130748            12.197k       384.068M/s
bench_sha3::sha3_224/4096           9869 ns         9855 ns        68754            23.669k       396.366M/s
bench_sha3::sha3_256/32              354 ns          354 ns      1968764                831       86.2062M/s
bench_sha3::sha3_256/64              355 ns          355 ns      1963072                832       171.983M/s
bench_sha3::sha3_256/128             351 ns          351 ns      1999880                825       347.855M/s
bench_sha3::sha3_256/256             693 ns          692 ns      1002305             1.644k       352.655M/s
bench_sha3::sha3_256/512            1374 ns         1367 ns       494637             3.278k       357.173M/s
bench_sha3::sha3_256/1024           2747 ns         2729 ns       254274             6.575k       357.795M/s
bench_sha3::sha3_256/2048           5434 ns         5429 ns       127220            13.022k       359.749M/s
bench_sha3::sha3_256/4096          10471 ns        10465 ns        66371            25.111k       373.258M/s
bench_sha3::sha3_384/32              357 ns          356 ns      1962230                834        85.664M/s
bench_sha3::sha3_384/64              357 ns          357 ns      1969468                835       171.179M/s
bench_sha3::sha3_384/128             704 ns          703 ns       997236             1.668k       173.678M/s
bench_sha3::sha3_384/256            1037 ns         1036 ns       665007             2.467k       235.733M/s
bench_sha3::sha3_384/512            1713 ns         1711 ns       410374             4.091k       285.327M/s
bench_sha3::sha3_384/1024           3430 ns         3426 ns       205157             8.212k       285.004M/s
bench_sha3::sha3_384/2048           6782 ns         6775 ns        99873            16.255k       288.273M/s
bench_sha3::sha3_384/4096          13588 ns        13577 ns        50472             32.59k       287.718M/s
bench_sha3::sha3_512/32              358 ns          357 ns      1936569                836       85.4371M/s
bench_sha3::sha3_512/64              362 ns          360 ns      1978743                847       169.354M/s
bench_sha3::sha3_512/128             755 ns          741 ns       959443             1.789k        164.82M/s
bench_sha3::sha3_512/256            1372 ns         1371 ns       501824             3.271k       178.123M/s
bench_sha3::sha3_512/512            2871 ns         2863 ns       215611             6.869k       170.556M/s
bench_sha3::sha3_512/1024           5333 ns         5323 ns       129694             12.78k       183.469M/s
bench_sha3::sha3_512/2048          10068 ns        10057 ns        64825            24.144k       194.209M/s
bench_sha3::sha3_512/4096          19382 ns        19369 ns        34589            46.496k       201.675M/s
bench_sha3::shake128/32/32           517 ns          516 ns      1356274             1.223k       118.369M/s
bench_sha3::shake128/32/64           695 ns          693 ns       976549             1.651k       132.162M/s
bench_sha3::shake128/32/128          923 ns          921 ns       749722             2.198k       165.618M/s
bench_sha3::shake128/64/32           509 ns          508 ns      1362769             1.206k       180.062M/s
bench_sha3::shake128/64/64           643 ns          642 ns      1053281             1.526k       190.137M/s
bench_sha3::shake128/64/128          918 ns          917 ns       762071             2.186k       199.769M/s
bench_sha3::shake128/128/32          513 ns          512 ns      1384357             1.214k       298.264M/s
bench_sha3::shake128/128/64          659 ns          658 ns      1075566             1.565k       278.272M/s
bench_sha3::shake128/128/128         916 ns          915 ns       743692             2.182k        266.79M/s
bench_sha3::shake128/256/32          844 ns          844 ns       813897             2.011k       325.564M/s
bench_sha3::shake128/256/64          985 ns          984 ns       702896             2.347k       310.169M/s
bench_sha3::shake128/256/128        1249 ns         1248 ns       559396             2.982k       293.374M/s
bench_sha3::shake128/512/32         1524 ns         1523 ns       458445             3.642k       340.699M/s
bench_sha3::shake128/512/64         1690 ns         1684 ns       418908              4.04k       326.175M/s
bench_sha3::shake128/512/128        1943 ns         1941 ns       359170             4.647k       314.518M/s
bench_sha3::shake128/1024/32        2569 ns         2567 ns       272913              6.15k       392.354M/s
bench_sha3::shake128/1024/64        2698 ns         2694 ns       261999             6.459k       385.131M/s
bench_sha3::shake128/1024/128       2956 ns         2953 ns       237119             7.079k       371.985M/s
bench_sha3::shake128/2048/32        4628 ns         4622 ns       151232            11.089k       429.216M/s
bench_sha3::shake128/2048/64        4782 ns         4776 ns       146864             11.46k        421.76M/s
bench_sha3::shake128/2048/128       5097 ns         5088 ns       132754            12.216k       407.834M/s
bench_sha3::shake128/4096/32        8680 ns         8670 ns        80138            20.814k       454.076M/s
bench_sha3::shake128/4096/64        8837 ns         8827 ns        75576            21.191k       449.461M/s
bench_sha3::shake128/4096/128       9321 ns         9274 ns        73512            22.355k       434.373M/s
bench_sha3::shake256/32/32           504 ns          503 ns      1000000             1.192k       121.389M/s
bench_sha3::shake256/32/64           623 ns          622 ns      1117800             1.479k       147.188M/s
bench_sha3::shake256/32/128          868 ns          867 ns       791658             2.067k       176.013M/s
bench_sha3::shake256/64/32           503 ns          502 ns      1382525             1.189k       182.473M/s
bench_sha3::shake256/64/64           638 ns          637 ns      1086332             1.515k       191.777M/s
bench_sha3::shake256/64/128          858 ns          858 ns       815765             2.043k       213.502M/s
bench_sha3::shake256/128/32          495 ns          495 ns      1384220             1.173k       308.487M/s
bench_sha3::shake256/128/64          615 ns          615 ns      1122298              1.46k       297.827M/s
bench_sha3::shake256/128/128         857 ns          857 ns       811491             2.041k       285.042M/s
bench_sha3::shake256/256/32          849 ns          848 ns       818369             2.021k       323.842M/s
bench_sha3::shake256/256/64          968 ns          967 ns       724570             2.308k       315.438M/s
bench_sha3::shake256/256/128        1228 ns         1226 ns       568676             2.931k       298.638M/s
bench_sha3::shake256/512/32         1597 ns         1594 ns       431678             3.817k       325.549M/s
bench_sha3::shake256/512/64         1691 ns         1685 ns       407538             4.041k       325.954M/s
bench_sha3::shake256/512/128        1886 ns         1884 ns       358042             4.511k       323.978M/s
bench_sha3::shake256/1024/32        2913 ns         2909 ns       243776             6.975k       346.178M/s
bench_sha3::shake256/1024/64        3007 ns         3004 ns       230835             7.201k       345.353M/s
bench_sha3::shake256/1024/128       3241 ns         3238 ns       215513             7.761k       339.338M/s
bench_sha3::shake256/2048/32        5610 ns         5603 ns       121701            13.448k       354.006M/s
bench_sha3::shake256/2048/64        5690 ns         5684 ns       121621             13.64k       354.339M/s
bench_sha3::shake256/2048/128       5978 ns         5974 ns       115401             14.33k       347.396M/s
bench_sha3::shake256/4096/32       31062 ns        11662 ns        64059            74.532k        337.58M/s
bench_sha3::shake256/4096/64       11471 ns        11416 ns        62774            27.513k       347.511M/s
bench_sha3::shake256/4096/128      12172 ns        11755 ns        53348            29.197k       342.677M/s
```

### On AWS Graviton2 ( using `g++` )

```fish
2022-12-05T14:02:59+00:00
Running ./bench/a.out
Run on (16 X 166.66 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 48 KiB (x16)
  L2 Unified 2048 KiB (x4)
Load Average: 0.08, 0.02, 0.01
-----------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations bytes_per_second
-----------------------------------------------------------------------------------------
bench_sha3::keccakf1600              701 ns          701 ns       998129       272.237M/s
bench_sha3::sha3_224/32              774 ns          774 ns       905789       39.4228M/s
bench_sha3::sha3_224/64              767 ns          767 ns       912238       79.5407M/s
bench_sha3::sha3_224/128             765 ns          765 ns       915549        159.67M/s
bench_sha3::sha3_224/256            1485 ns         1485 ns       471258       164.379M/s
bench_sha3::sha3_224/512            2944 ns         2944 ns       237831       165.851M/s
bench_sha3::sha3_224/1024           5838 ns         5837 ns       119914       167.294M/s
bench_sha3::sha3_224/2048          10891 ns        10891 ns        64273       179.337M/s
bench_sha3::sha3_224/4096          20995 ns        20994 ns        33344       186.063M/s
bench_sha3::sha3_256/32              786 ns          785 ns       891143       38.8531M/s
bench_sha3::sha3_256/64              780 ns          780 ns       897117       78.2354M/s
bench_sha3::sha3_256/128             784 ns          784 ns       901807       155.715M/s
bench_sha3::sha3_256/256            1500 ns         1499 ns       466889       162.817M/s
bench_sha3::sha3_256/512            2964 ns         2964 ns       236298       164.763M/s
bench_sha3::sha3_256/1024           5867 ns         5867 ns       119318       166.443M/s
bench_sha3::sha3_256/2048          11684 ns        11683 ns        59917       167.172M/s
bench_sha3::sha3_256/4096          22572 ns        22571 ns        31014       173.063M/s
bench_sha3::sha3_384/32              768 ns          768 ns       911442       39.7362M/s
bench_sha3::sha3_384/64              765 ns          765 ns       915024       79.7896M/s
bench_sha3::sha3_384/128            1488 ns         1488 ns       470361       82.0236M/s
bench_sha3::sha3_384/256            2212 ns         2212 ns       316508       110.392M/s
bench_sha3::sha3_384/512            3662 ns         3661 ns       191664       133.358M/s
bench_sha3::sha3_384/1024           7248 ns         7248 ns        96637       134.734M/s
bench_sha3::sha3_384/2048          14441 ns        14441 ns        48470       135.249M/s
bench_sha3::sha3_384/4096          28832 ns        28831 ns        24279       135.489M/s
bench_sha3::sha3_512/32              750 ns          750 ns       933092       40.6803M/s
bench_sha3::sha3_512/64              754 ns          754 ns       928282       80.9409M/s
bench_sha3::sha3_512/128            1454 ns         1454 ns       481469       83.9694M/s
bench_sha3::sha3_512/256            2869 ns         2869 ns       243961       85.0878M/s
bench_sha3::sha3_512/512            5689 ns         5689 ns       123026        85.826M/s
bench_sha3::sha3_512/1024          10618 ns        10617 ns        65926       91.9767M/s
bench_sha3::sha3_512/2048          20475 ns        20474 ns        34190       95.3961M/s
bench_sha3::sha3_512/4096          40183 ns        40181 ns        17420       97.2157M/s
bench_sha3::shake128/32/32           828 ns          828 ns       845234       73.7036M/s
bench_sha3::shake128/32/64           877 ns          877 ns       798334       104.419M/s
bench_sha3::shake128/32/128          975 ns          975 ns       718130       156.551M/s
bench_sha3::shake128/64/32           830 ns          830 ns       843543       110.327M/s
bench_sha3::shake128/64/64           879 ns          879 ns       796684       138.944M/s
bench_sha3::shake128/64/128          976 ns          976 ns       717167       187.602M/s
bench_sha3::shake128/128/32          825 ns          825 ns       848384       184.941M/s
bench_sha3::shake128/128/64          874 ns          874 ns       800900       209.542M/s
bench_sha3::shake128/128/128         971 ns          971 ns       720681       251.356M/s
bench_sha3::shake128/256/32         1548 ns         1548 ns       452232       177.453M/s
bench_sha3::shake128/256/64         1597 ns         1597 ns       438421        191.13M/s
bench_sha3::shake128/256/128        1694 ns         1694 ns       413196       216.158M/s
bench_sha3::shake128/512/32         3015 ns         3015 ns       232238       172.071M/s
bench_sha3::shake128/512/64         3067 ns         3067 ns       228456       179.129M/s
bench_sha3::shake128/512/128        3163 ns         3162 ns       221299       193.001M/s
bench_sha3::shake128/1024/32        5183 ns         5182 ns       135065       194.326M/s
bench_sha3::shake128/1024/64        5231 ns         5231 ns       133810       198.357M/s
bench_sha3::shake128/1024/128       5330 ns         5330 ns       131323       206.114M/s
bench_sha3::shake128/2048/32        9508 ns         9508 ns        73620       208.638M/s
bench_sha3::shake128/2048/64        9557 ns         9557 ns        73248       210.757M/s
bench_sha3::shake128/2048/128       9655 ns         9654 ns        72508       214.952M/s
bench_sha3::shake128/4096/32       18173 ns        18173 ns        38517       216.626M/s
bench_sha3::shake128/4096/64       18224 ns        18224 ns        38412       217.699M/s
bench_sha3::shake128/4096/128      18321 ns        18321 ns        38205       219.875M/s
bench_sha3::shake256/32/32           825 ns          825 ns       848464       73.9826M/s
bench_sha3::shake256/32/64           881 ns          881 ns       794815       103.956M/s
bench_sha3::shake256/32/128          992 ns          992 ns       705551       153.796M/s
bench_sha3::shake256/64/32           822 ns          821 ns       852082       111.447M/s
bench_sha3::shake256/64/64           877 ns          877 ns       797877       139.163M/s
bench_sha3::shake256/64/128          989 ns          989 ns       708029       185.219M/s
bench_sha3::shake256/128/32          816 ns          816 ns       857694       186.954M/s
bench_sha3::shake256/128/64          872 ns          872 ns       803111       210.084M/s
bench_sha3::shake256/128/128         983 ns          983 ns       712161       248.384M/s
bench_sha3::shake256/256/32         1530 ns         1530 ns       457383       179.476M/s
bench_sha3::shake256/256/64         1586 ns         1586 ns       441349       192.412M/s
bench_sha3::shake256/256/128        1697 ns         1697 ns       412386       215.744M/s
bench_sha3::shake256/512/32         2973 ns         2973 ns       235511        174.52M/s
bench_sha3::shake256/512/64         3029 ns         3029 ns       231123       181.339M/s
bench_sha3::shake256/512/128        3140 ns         3140 ns       222963       194.375M/s
bench_sha3::shake256/1024/32        5834 ns         5834 ns       119974        172.62M/s
bench_sha3::shake256/1024/64        5890 ns         5890 ns       118847       176.166M/s
bench_sha3::shake256/1024/128       6002 ns         6001 ns       116629       183.062M/s
bench_sha3::shake256/2048/32       11555 ns        11555 ns        60580       171.676M/s
bench_sha3::shake256/2048/64       11611 ns        11611 ns        60288       173.475M/s
bench_sha3::shake256/2048/128      11722 ns        11722 ns        59714       177.041M/s
bench_sha3::shake256/4096/32       22273 ns        22272 ns        31428       176.757M/s
bench_sha3::shake256/4096/64       22328 ns        22327 ns        31351       177.689M/s
bench_sha3::shake256/4096/128      22440 ns        22439 ns        31196       179.524M/s
```

### On AWS Graviton2 ( using `clang++` )

```fish
2022-12-05T14:04:48+00:00
Running ./bench/a.out
Run on (16 X 166.66 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 48 KiB (x16)
  L2 Unified 2048 KiB (x4)
Load Average: 0.45, 0.21, 0.08
-----------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations bytes_per_second
-----------------------------------------------------------------------------------------
bench_sha3::keccakf1600              752 ns          752 ns       930434       253.675M/s
bench_sha3::sha3_224/32              822 ns          822 ns       851127       37.1269M/s
bench_sha3::sha3_224/64              817 ns          817 ns       856931       74.7184M/s
bench_sha3::sha3_224/128             810 ns          810 ns       864367       150.741M/s
bench_sha3::sha3_224/256            1593 ns         1593 ns       439877        153.26M/s
bench_sha3::sha3_224/512            3155 ns         3155 ns       221702       154.787M/s
bench_sha3::sha3_224/1024           6257 ns         6257 ns       111863       156.068M/s
bench_sha3::sha3_224/2048          11674 ns        11673 ns        59942       167.316M/s
bench_sha3::sha3_224/4096          22495 ns        22495 ns        31118       173.649M/s
bench_sha3::sha3_256/32              821 ns          821 ns       852544       37.1673M/s
bench_sha3::sha3_256/64              815 ns          815 ns       858390       74.8484M/s
bench_sha3::sha3_256/128             808 ns          808 ns       866249       151.078M/s
bench_sha3::sha3_256/256            1599 ns         1599 ns       438184       152.652M/s
bench_sha3::sha3_256/512            3151 ns         3151 ns       222228       154.954M/s
bench_sha3::sha3_256/1024           6248 ns         6248 ns       112027         156.3M/s
bench_sha3::sha3_256/2048          12453 ns        12453 ns        56211        156.84M/s
bench_sha3::sha3_256/4096          24073 ns        24072 ns        29079       162.273M/s
bench_sha3::sha3_384/32              808 ns          808 ns       866155       37.7625M/s
bench_sha3::sha3_384/64              804 ns          804 ns       870884       75.9388M/s
bench_sha3::sha3_384/128            1581 ns         1581 ns       442780       77.2158M/s
bench_sha3::sha3_384/256            2354 ns         2354 ns       298225       103.708M/s
bench_sha3::sha3_384/512            3897 ns         3897 ns       179614       125.294M/s
bench_sha3::sha3_384/1024           7753 ns         7753 ns        90230       125.964M/s
bench_sha3::sha3_384/2048          15473 ns        15473 ns        45246       126.231M/s
bench_sha3::sha3_384/4096          30897 ns        30897 ns        22658        126.43M/s
bench_sha3::sha3_512/32              799 ns          798 ns       876634       38.2188M/s
bench_sha3::sha3_512/64              793 ns          793 ns       882382       76.9419M/s
bench_sha3::sha3_512/128            1564 ns         1564 ns       447609       78.0572M/s
bench_sha3::sha3_512/256            3108 ns         3108 ns       225178       78.5413M/s
bench_sha3::sha3_512/512            6175 ns         6175 ns       113363       79.0768M/s
bench_sha3::sha3_512/1024          11540 ns        11540 ns        60666       84.6254M/s
bench_sha3::sha3_512/2048          22268 ns        22267 ns        31436       87.7127M/s
bench_sha3::sha3_512/4096          43722 ns        43721 ns        16010       89.3441M/s
bench_sha3::shake128/32/32          1204 ns         1204 ns       582024       50.7095M/s
bench_sha3::shake128/32/64          1566 ns         1566 ns       447089       58.4761M/s
bench_sha3::shake128/32/128         2290 ns         2290 ns       305675       66.6406M/s
bench_sha3::shake128/64/32          1207 ns         1207 ns       580068       75.8694M/s
bench_sha3::shake128/64/64          1569 ns         1569 ns       446243       77.8187M/s
bench_sha3::shake128/64/128         2293 ns         2293 ns       305323       79.8652M/s
bench_sha3::shake128/128/32         1204 ns         1204 ns       581400       126.739M/s
bench_sha3::shake128/128/64         1566 ns         1566 ns       446976       116.927M/s
bench_sha3::shake128/128/128        2290 ns         2290 ns       305669       106.611M/s
bench_sha3::shake128/256/32         1980 ns         1980 ns       353556       138.726M/s
bench_sha3::shake128/256/64         2342 ns         2342 ns       298911       130.312M/s
bench_sha3::shake128/256/128        3066 ns         3066 ns       228320       119.443M/s
bench_sha3::shake128/512/32         3561 ns         3561 ns       196578       145.703M/s
bench_sha3::shake128/512/64         3917 ns         3917 ns       178652       140.229M/s
bench_sha3::shake128/512/128        4642 ns         4642 ns       150823       131.495M/s
bench_sha3::shake128/1024/32        5887 ns         5887 ns       118907       171.068M/s
bench_sha3::shake128/1024/64        6249 ns         6249 ns       112010       166.042M/s
bench_sha3::shake128/1024/128       6973 ns         6973 ns       100381       157.554M/s
bench_sha3::shake128/2048/32       10542 ns        10542 ns        66400       188.171M/s
bench_sha3::shake128/2048/64       10907 ns        10907 ns        64182       184.668M/s
bench_sha3::shake128/2048/128      11628 ns        11628 ns        60201        178.47M/s
bench_sha3::shake128/4096/32       19872 ns        19872 ns        35226       198.108M/s
bench_sha3::shake128/4096/64       20234 ns        20234 ns        34595       196.074M/s
bench_sha3::shake128/4096/128      20958 ns        20958 ns        33399       192.209M/s
bench_sha3::shake256/32/32          1200 ns         1200 ns       583488       50.8777M/s
bench_sha3::shake256/32/64          1562 ns         1562 ns       448197       58.6248M/s
bench_sha3::shake256/32/128         2286 ns         2286 ns       306246       66.7563M/s
bench_sha3::shake256/64/32          1194 ns         1194 ns       586453       76.7023M/s
bench_sha3::shake256/64/64          1556 ns         1556 ns       449984        78.473M/s
bench_sha3::shake256/64/128         2280 ns         2280 ns       307056       80.3219M/s
bench_sha3::shake256/128/32         1184 ns         1184 ns       590988       128.829M/s
bench_sha3::shake256/128/64         1547 ns         1546 ns       452636       118.401M/s
bench_sha3::shake256/128/128        2271 ns         2271 ns       308291       107.526M/s
bench_sha3::shake256/256/32         1966 ns         1966 ns       357101       139.739M/s
bench_sha3::shake256/256/64         2327 ns         2327 ns       300591       131.147M/s
bench_sha3::shake256/256/128        3046 ns         3046 ns       229816        120.23M/s
bench_sha3::shake256/512/32         3521 ns         3521 ns       198762       147.338M/s
bench_sha3::shake256/512/64         3889 ns         3889 ns       179978        141.24M/s
bench_sha3::shake256/512/128        4616 ns         4616 ns       151751       132.236M/s
bench_sha3::shake256/1024/32        6619 ns         6619 ns       105749       152.154M/s
bench_sha3::shake256/1024/64        6982 ns         6982 ns       100270       148.609M/s
bench_sha3::shake256/1024/128       7705 ns         7705 ns        90852       142.589M/s
bench_sha3::shake256/2048/32       12824 ns        12823 ns        54587        154.69M/s
bench_sha3::shake256/2048/64       13185 ns        13185 ns        53088       152.759M/s
bench_sha3::shake256/2048/128      13910 ns        13909 ns        50320       149.194M/s
bench_sha3::shake256/4096/32       24441 ns        24441 ns        28639       161.071M/s
bench_sha3::shake256/4096/64       24804 ns        24804 ns        28220       159.948M/s
bench_sha3::shake256/4096/128      25528 ns        25528 ns        27421       157.803M/s
```

### On AWS Graviton3 ( using `g++` )

```fish
2022-12-05T14:07:11+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.00, 0.00, 0.00
-----------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations bytes_per_second
-----------------------------------------------------------------------------------------
bench_sha3::keccakf1600              319 ns          319 ns      2192463       597.205M/s
bench_sha3::sha3_224/32              365 ns          365 ns      1919607       83.5948M/s
bench_sha3::sha3_224/64              356 ns          356 ns      1965801       171.452M/s
bench_sha3::sha3_224/128             350 ns          350 ns      1999622       348.705M/s
bench_sha3::sha3_224/256             687 ns          687 ns      1018915       355.392M/s
bench_sha3::sha3_224/512            1359 ns         1359 ns       514875       359.209M/s
bench_sha3::sha3_224/1024           2700 ns         2700 ns       259308       361.726M/s
bench_sha3::sha3_224/2048           5034 ns         5034 ns       139019        387.98M/s
bench_sha3::sha3_224/4096           9690 ns         9690 ns        72241       403.117M/s
bench_sha3::sha3_256/32              348 ns          348 ns      2009496       87.5822M/s
bench_sha3::sha3_256/64              342 ns          342 ns      2046215       178.432M/s
bench_sha3::sha3_256/128             340 ns          340 ns      2061065       359.398M/s
bench_sha3::sha3_256/256             670 ns          670 ns      1044899       364.361M/s
bench_sha3::sha3_256/512            1331 ns         1331 ns       525928       366.879M/s
bench_sha3::sha3_256/1024           2654 ns         2654 ns       263738       367.924M/s
bench_sha3::sha3_256/2048           5297 ns         5297 ns       132148       368.703M/s
bench_sha3::sha3_256/4096          10247 ns        10247 ns        68309       381.209M/s
bench_sha3::sha3_384/32              336 ns          336 ns      2080661       90.7094M/s
bench_sha3::sha3_384/64              337 ns          337 ns      2078858       181.258M/s
bench_sha3::sha3_384/128             665 ns          665 ns      1053003       183.629M/s
bench_sha3::sha3_384/256             986 ns          986 ns       709867       247.597M/s
bench_sha3::sha3_384/512            1634 ns         1634 ns       428497        298.89M/s
bench_sha3::sha3_384/1024           3264 ns         3264 ns       214478       299.231M/s
bench_sha3::sha3_384/2048           6513 ns         6513 ns       107456       299.867M/s
bench_sha3::sha3_384/4096          13011 ns        13011 ns        53800       300.232M/s
bench_sha3::sha3_512/32              343 ns          343 ns      2040115       88.9462M/s
bench_sha3::sha3_512/64              340 ns          340 ns      2062623       179.416M/s
bench_sha3::sha3_512/128             668 ns          668 ns      1047836       182.735M/s
bench_sha3::sha3_512/256            1323 ns         1323 ns       529103       184.537M/s
bench_sha3::sha3_512/512            2635 ns         2635 ns       265663       185.311M/s
bench_sha3::sha3_512/1024           4926 ns         4926 ns       142117       198.259M/s
bench_sha3::sha3_512/2048           9483 ns         9483 ns        73819       205.962M/s
bench_sha3::sha3_512/4096          18619 ns        18619 ns        37601         209.8M/s
bench_sha3::shake128/32/32           371 ns          371 ns      1888895       164.705M/s
bench_sha3::shake128/32/64           395 ns          395 ns      1771209       231.663M/s
bench_sha3::shake128/32/128          444 ns          444 ns      1574927       343.306M/s
bench_sha3::shake128/64/32           380 ns          380 ns      1841434       240.948M/s
bench_sha3::shake128/64/64           405 ns          405 ns      1727154       301.483M/s
bench_sha3::shake128/64/128          455 ns          455 ns      1540266       402.783M/s
bench_sha3::shake128/128/32          370 ns          370 ns      1893894       412.856M/s
bench_sha3::shake128/128/64          394 ns          394 ns      1776767       464.745M/s
bench_sha3::shake128/128/128         443 ns          443 ns      1578887       550.675M/s
bench_sha3::shake128/256/32          706 ns          706 ns       991583       389.038M/s
bench_sha3::shake128/256/64          731 ns          731 ns       957496       417.483M/s
bench_sha3::shake128/256/128         780 ns          780 ns       896936       469.384M/s
bench_sha3::shake128/512/32         1381 ns         1381 ns       506935       375.666M/s
bench_sha3::shake128/512/64         1405 ns         1405 ns       498107       390.889M/s
bench_sha3::shake128/512/128        1455 ns         1455 ns       481190       419.596M/s
bench_sha3::shake128/1024/32        2396 ns         2396 ns       292183       420.341M/s
bench_sha3::shake128/1024/64        2420 ns         2420 ns       289238       428.721M/s
bench_sha3::shake128/1024/128       2470 ns         2470 ns       283436       444.854M/s
bench_sha3::shake128/2048/32        4407 ns         4407 ns       158828        450.08M/s
bench_sha3::shake128/2048/64        4432 ns         4432 ns       157927       454.455M/s
bench_sha3::shake128/2048/128       4481 ns         4481 ns       156212       463.095M/s
bench_sha3::shake128/4096/32        8438 ns         8438 ns        82940       466.547M/s
bench_sha3::shake128/4096/64        8465 ns         8465 ns        82705       468.683M/s
bench_sha3::shake128/4096/128       8513 ns         8513 ns        82219       473.209M/s
bench_sha3::shake256/32/32           380 ns          380 ns      1843462       160.674M/s
bench_sha3::shake256/32/64           405 ns          405 ns      1727649       225.958M/s
bench_sha3::shake256/32/128          452 ns          452 ns      1546451       337.491M/s
bench_sha3::shake256/64/32           374 ns          374 ns      1872483       244.817M/s
bench_sha3::shake256/64/64           398 ns          398 ns      1756423       306.462M/s
bench_sha3::shake256/64/128          448 ns          448 ns      1563480       408.963M/s
bench_sha3::shake256/128/32          370 ns          370 ns      1892539       412.363M/s
bench_sha3::shake256/128/64          395 ns          395 ns      1773951        463.96M/s
bench_sha3::shake256/128/128         444 ns          444 ns      1576696       549.755M/s
bench_sha3::shake256/256/32          706 ns          706 ns       990915       388.831M/s
bench_sha3::shake256/256/64          731 ns          731 ns       956988       417.214M/s
bench_sha3::shake256/256/128         781 ns          781 ns       896651       469.071M/s
bench_sha3::shake256/512/32         1381 ns         1380 ns       507216       375.809M/s
bench_sha3::shake256/512/64         1406 ns         1406 ns       497985       390.819M/s
bench_sha3::shake256/512/128        1455 ns         1455 ns       481259        419.59M/s
bench_sha3::shake256/1024/32        2724 ns         2724 ns       256919       369.661M/s
bench_sha3::shake256/1024/64        2749 ns         2749 ns       254597       377.414M/s
bench_sha3::shake256/1024/128       2798 ns         2798 ns       250200       392.697M/s
bench_sha3::shake256/2048/32        5418 ns         5418 ns       129192       366.151M/s
bench_sha3::shake256/2048/64        5443 ns         5443 ns       128621       370.057M/s
bench_sha3::shake256/2048/128       5493 ns         5493 ns       127429       377.815M/s
bench_sha3::shake256/4096/32       10451 ns        10451 ns        66971       376.693M/s
bench_sha3::shake256/4096/64       10475 ns        10474 ns        66826       378.762M/s
bench_sha3::shake256/4096/128      10524 ns        10523 ns        66509       382.797M/s
```

### On AWS Graviton3 ( using `clang++` )

```fish
2022-12-05T14:09:25+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.37, 0.21, 0.09
-----------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations bytes_per_second
-----------------------------------------------------------------------------------------
bench_sha3::keccakf1600              326 ns          326 ns      2148549       585.367M/s
bench_sha3::sha3_224/32              352 ns          352 ns      1991179       86.8119M/s
bench_sha3::sha3_224/64              351 ns          351 ns      1995946       174.022M/s
bench_sha3::sha3_224/128             346 ns          346 ns      2023580       352.899M/s
bench_sha3::sha3_224/256             679 ns          679 ns      1030890       359.469M/s
bench_sha3::sha3_224/512            1351 ns         1351 ns       518103       361.499M/s
bench_sha3::sha3_224/1024           2698 ns         2698 ns       259436        361.93M/s
bench_sha3::sha3_224/2048           5035 ns         5035 ns       139035       387.886M/s
bench_sha3::sha3_224/4096           9711 ns         9711 ns        72076       402.243M/s
bench_sha3::sha3_256/32              348 ns          348 ns      2010439       87.6812M/s
bench_sha3::sha3_256/64              348 ns          348 ns      2014004       175.573M/s
bench_sha3::sha3_256/128             342 ns          342 ns      2043948       356.468M/s
bench_sha3::sha3_256/256             676 ns          676 ns      1035220       361.074M/s
bench_sha3::sha3_256/512            1346 ns         1346 ns       520041       362.824M/s
bench_sha3::sha3_256/1024           2693 ns         2693 ns       259954       362.622M/s
bench_sha3::sha3_256/2048           5365 ns         5365 ns       130584       364.026M/s
bench_sha3::sha3_256/4096          10371 ns        10371 ns        67496       376.659M/s
bench_sha3::sha3_384/32              348 ns          348 ns      2009206       87.5781M/s
bench_sha3::sha3_384/64              349 ns          349 ns      2005771        174.92M/s
bench_sha3::sha3_384/128             677 ns          677 ns      1034186        180.34M/s
bench_sha3::sha3_384/256            1010 ns         1010 ns       693297       241.817M/s
bench_sha3::sha3_384/512            1675 ns         1675 ns       418040       291.585M/s
bench_sha3::sha3_384/1024           3351 ns         3351 ns       208967       291.393M/s
bench_sha3::sha3_384/2048           6690 ns         6690 ns       104637        291.95M/s
bench_sha3::sha3_384/4096          13354 ns        13354 ns        52439       292.516M/s
bench_sha3::sha3_512/32              347 ns          347 ns      2016755       87.9357M/s
bench_sha3::sha3_512/64              343 ns          343 ns      2045416       178.187M/s
bench_sha3::sha3_512/128             672 ns          672 ns      1041283       181.555M/s
bench_sha3::sha3_512/256            1338 ns         1338 ns       523325       182.503M/s
bench_sha3::sha3_512/512            2678 ns         2678 ns       261362       182.338M/s
bench_sha3::sha3_512/1024           5005 ns         5005 ns       139863       195.137M/s
bench_sha3::sha3_512/2048           9652 ns         9652 ns        72523        202.35M/s
bench_sha3::sha3_512/4096          18956 ns        18955 ns        36921       206.075M/s
bench_sha3::shake128/32/32           462 ns          462 ns      1516140       132.207M/s
bench_sha3::shake128/32/64           567 ns          567 ns      1233411       161.331M/s
bench_sha3::shake128/32/128          783 ns          783 ns       894200       194.909M/s
bench_sha3::shake128/64/32           463 ns          463 ns      1510364       197.554M/s
bench_sha3::shake128/64/64           571 ns          571 ns      1229931       213.784M/s
bench_sha3::shake128/64/128          785 ns          785 ns       891876       233.305M/s
bench_sha3::shake128/128/32          459 ns          459 ns      1525308       332.484M/s
bench_sha3::shake128/128/64          567 ns          567 ns      1235485       323.196M/s
bench_sha3::shake128/128/128         781 ns          781 ns       896606       312.752M/s
bench_sha3::shake128/256/32          791 ns          791 ns       884359        347.04M/s
bench_sha3::shake128/256/64          900 ns          900 ns       778053       339.222M/s
bench_sha3::shake128/256/128        1114 ns         1114 ns       628255       328.686M/s
bench_sha3::shake128/512/32         1465 ns         1465 ns       477913       354.133M/s
bench_sha3::shake128/512/64         1573 ns         1573 ns       444947       349.219M/s
bench_sha3::shake128/512/128        1788 ns         1788 ns       391584        341.45M/s
bench_sha3::shake128/1024/32        2465 ns         2465 ns       283909       408.484M/s
bench_sha3::shake128/1024/64        2574 ns         2574 ns       271976       403.154M/s
bench_sha3::shake128/1024/128       2789 ns         2788 ns       251034       393.992M/s
bench_sha3::shake128/2048/32        4477 ns         4476 ns       156398        443.13M/s
bench_sha3::shake128/2048/64        4584 ns         4584 ns       152718       439.425M/s
bench_sha3::shake128/2048/128       4800 ns         4799 ns       145851       432.384M/s
bench_sha3::shake128/4096/32        8494 ns         8494 ns        82418       463.478M/s
bench_sha3::shake128/4096/64        8604 ns         8603 ns        81374       461.131M/s
bench_sha3::shake128/4096/128       8817 ns         8817 ns        79393        456.88M/s
bench_sha3::shake256/32/32           465 ns          465 ns      1505648       131.348M/s
bench_sha3::shake256/32/64           573 ns          573 ns      1222103       159.839M/s
bench_sha3::shake256/32/128          786 ns          786 ns       890466       194.119M/s
bench_sha3::shake256/64/32           461 ns          461 ns      1517626       198.484M/s
bench_sha3::shake256/64/64           572 ns          572 ns      1229657       213.463M/s
bench_sha3::shake256/64/128          783 ns          783 ns       894278        233.91M/s
bench_sha3::shake256/128/32          456 ns          456 ns      1536298        334.88M/s
bench_sha3::shake256/128/64          563 ns          563 ns      1244182       325.442M/s
bench_sha3::shake256/128/128         779 ns          779 ns       898890       313.507M/s
bench_sha3::shake256/256/32          789 ns          789 ns       886973        348.06M/s
bench_sha3::shake256/256/64          897 ns          897 ns       780241       340.176M/s
bench_sha3::shake256/256/128        1112 ns         1112 ns       629395       329.241M/s
bench_sha3::shake256/512/32         1457 ns         1457 ns       480328       355.996M/s
bench_sha3::shake256/512/64         1565 ns         1565 ns       447242       350.967M/s
bench_sha3::shake256/512/128        1780 ns         1780 ns       393174       342.814M/s
bench_sha3::shake256/1024/32        2804 ns         2804 ns       249658       359.186M/s
bench_sha3::shake256/1024/64        2911 ns         2911 ns       240468       356.401M/s
bench_sha3::shake256/1024/128       3127 ns         3127 ns       223819        351.31M/s
bench_sha3::shake256/2048/32        5481 ns         5480 ns       127706       361.949M/s
bench_sha3::shake256/2048/64        5587 ns         5587 ns       125290       360.514M/s
bench_sha3::shake256/2048/128       5804 ns         5804 ns       120611       357.574M/s
bench_sha3::shake256/4096/32       10492 ns        10492 ns        66722       375.233M/s
bench_sha3::shake256/4096/64       10598 ns        10598 ns        66045       374.334M/s
bench_sha3::shake256/4096/128      10815 ns        10815 ns        64721       372.474M/s
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( using `g++` )

```fish
2022-12-05T14:11:51+00:00
Running ./bench/a.out
Run on (4 X 2300.25 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.08, 0.02, 0.01
------------------------------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations average_cpu_cycles bytes_per_second
------------------------------------------------------------------------------------------------------------
bench_sha3::keccakf1600              580 ns          580 ns      1205838             1.309k       329.111M/s
bench_sha3::sha3_224/32              635 ns          635 ns      1102375             1.423k        48.078M/s
bench_sha3::sha3_224/64              634 ns          634 ns      1104289             1.421k       96.3255M/s
bench_sha3::sha3_224/128             633 ns          633 ns      1106382             1.418k       192.887M/s
bench_sha3::sha3_224/256            1229 ns         1229 ns       569017             2.792k       198.601M/s
bench_sha3::sha3_224/512            2417 ns         2417 ns       289390             5.523k       202.025M/s
bench_sha3::sha3_224/1024           4794 ns         4794 ns       146030            10.989k       203.708M/s
bench_sha3::sha3_224/2048           8973 ns         8973 ns        78034            20.602k       217.677M/s
bench_sha3::sha3_224/4096          17327 ns        17327 ns        40379            39.809k       225.439M/s
bench_sha3::sha3_256/32              628 ns          628 ns      1117063             1.407k       48.6099M/s
bench_sha3::sha3_256/64              626 ns          626 ns      1114960             1.404k        97.442M/s
bench_sha3::sha3_256/128             622 ns          622 ns      1126533             1.393k       196.388M/s
bench_sha3::sha3_256/256            1214 ns         1214 ns       576945             2.756k       201.062M/s
bench_sha3::sha3_256/512            2400 ns         2400 ns       291505             5.483k       203.462M/s
bench_sha3::sha3_256/1024           4762 ns         4762 ns       146906            10.916k       205.079M/s
bench_sha3::sha3_256/2048           9478 ns         9478 ns        73822            21.763k        206.08M/s
bench_sha3::sha3_256/4096          18378 ns        18377 ns        38105            42.225k       212.556M/s
bench_sha3::sha3_384/32              635 ns          635 ns      1101695             1.426k       48.0423M/s
bench_sha3::sha3_384/64              633 ns          633 ns      1106657             1.421k       96.4342M/s
bench_sha3::sha3_384/128            1223 ns         1223 ns       572559             2.778k       99.7815M/s
bench_sha3::sha3_384/256            1809 ns         1809 ns       387170             4.125k        134.98M/s
bench_sha3::sha3_384/512            2982 ns         2982 ns       234761             6.825k       163.734M/s
bench_sha3::sha3_384/1024           5935 ns         5935 ns       117618            13.617k       164.544M/s
bench_sha3::sha3_384/2048          11822 ns        11821 ns        59227            27.154k       165.227M/s
bench_sha3::sha3_384/4096          23583 ns        23581 ns        29694            54.205k       165.652M/s
bench_sha3::sha3_512/32              604 ns          604 ns      1157715             1.351k       50.5473M/s
bench_sha3::sha3_512/64              604 ns          604 ns      1159672             1.352k       101.082M/s
bench_sha3::sha3_512/128            1164 ns         1164 ns       601628              2.64k       104.878M/s
bench_sha3::sha3_512/256            2281 ns         2281 ns       306718             5.209k       107.047M/s
bench_sha3::sha3_512/512            4527 ns         4527 ns       154606            10.376k       107.853M/s
bench_sha3::sha3_512/1024           8438 ns         8437 ns        83017             19.37k       115.744M/s
bench_sha3::sha3_512/2048          16287 ns        16286 ns        42974            37.421k       119.927M/s
bench_sha3::sha3_512/4096          31946 ns        31944 ns        21911            73.431k       122.283M/s
bench_sha3::shake128/32/32           664 ns          664 ns      1057050             1.504k       91.9465M/s
bench_sha3::shake128/32/64           697 ns          697 ns      1004142             1.581k       131.336M/s
bench_sha3::shake128/32/128          753 ns          753 ns       929504             1.709k       202.674M/s
bench_sha3::shake128/64/32           654 ns          654 ns      1069622             1.481k       140.023M/s
bench_sha3::shake128/64/64           688 ns          688 ns      1019682              1.56k       177.547M/s
bench_sha3::shake128/64/128          743 ns          743 ns       943099             1.687k       246.439M/s
bench_sha3::shake128/128/32          649 ns          649 ns      1077806              1.47k       234.998M/s
bench_sha3::shake128/128/64          686 ns          686 ns      1019411             1.556k       266.896M/s
bench_sha3::shake128/128/128         740 ns          740 ns       944450              1.68k       329.787M/s
bench_sha3::shake128/256/32         1249 ns         1249 ns       560110              2.85k       219.947M/s
bench_sha3::shake128/256/64         1283 ns         1283 ns       545922             2.929k       237.844M/s
bench_sha3::shake128/256/128        1340 ns         1340 ns       522339              3.06k        273.29M/s
bench_sha3::shake128/512/32         2443 ns         2443 ns       286755             5.595k         212.4M/s
bench_sha3::shake128/512/64         2470 ns         2470 ns       283476             5.659k       222.367M/s
bench_sha3::shake128/512/128        2527 ns         2527 ns       277025             5.789k       241.556M/s
bench_sha3::shake128/1024/32        4223 ns         4223 ns       165785             9.689k       238.497M/s
bench_sha3::shake128/1024/64        4253 ns         4253 ns       164550              9.76k       243.981M/s
bench_sha3::shake128/1024/128       4311 ns         4311 ns       162362             9.893k       254.864M/s
bench_sha3::shake128/2048/32        7780 ns         7780 ns        90130            17.871k       254.976M/s
bench_sha3::shake128/2048/64        7807 ns         7806 ns        89682            17.933k       258.021M/s
bench_sha3::shake128/2048/128       7871 ns         7870 ns        88990             18.08k       263.669M/s
bench_sha3::shake128/4096/32       14908 ns        14908 ns        46944            34.255k       264.079M/s
bench_sha3::shake128/4096/64       14929 ns        14929 ns        46887            34.307k       265.752M/s
bench_sha3::shake128/4096/128      14990 ns        14989 ns        46649            34.447k       268.756M/s
bench_sha3::shake256/32/32           653 ns          653 ns      1072085              1.48k       93.4497M/s
bench_sha3::shake256/32/64           687 ns          687 ns      1019952             1.556k       133.361M/s
bench_sha3::shake256/32/128          743 ns          743 ns       942336             1.686k       205.371M/s
bench_sha3::shake256/64/32           652 ns          652 ns      1073999             1.477k       140.448M/s
bench_sha3::shake256/64/64           686 ns          686 ns      1019071             1.556k       177.924M/s
bench_sha3::shake256/64/128          742 ns          742 ns       943990             1.684k       246.754M/s
bench_sha3::shake256/128/32          645 ns          645 ns      1083458              1.46k       236.588M/s
bench_sha3::shake256/128/64          677 ns          677 ns      1033457             1.535k       270.451M/s
bench_sha3::shake256/128/128         734 ns          734 ns       954482             1.665k       332.733M/s
bench_sha3::shake256/256/32         1234 ns         1234 ns       568016             2.815k       222.638M/s
bench_sha3::shake256/256/64         1266 ns         1266 ns       552955             2.889k       241.066M/s
bench_sha3::shake256/256/128        1322 ns         1322 ns       529555             3.018k       277.087M/s
bench_sha3::shake256/512/32         2420 ns         2420 ns       288914             5.542k        214.42M/s
bench_sha3::shake256/512/64         2457 ns         2457 ns       285357             5.629k       223.565M/s
bench_sha3::shake256/512/128        2510 ns         2510 ns       279001             5.749k       243.208M/s
bench_sha3::shake256/1024/32        4783 ns         4782 ns       146385            10.977k       210.577M/s
bench_sha3::shake256/1024/64        4816 ns         4816 ns       145353            11.055k       215.465M/s
bench_sha3::shake256/1024/128       4870 ns         4870 ns       143694             11.18k       225.581M/s
bench_sha3::shake256/2048/32        9498 ns         9497 ns        73718            21.822k       208.874M/s
bench_sha3::shake256/2048/64        9523 ns         9523 ns        73517             21.88k       211.505M/s
bench_sha3::shake256/2048/128       9577 ns         9577 ns        73117            22.004k       216.689M/s
bench_sha3::shake256/4096/32       18365 ns        18363 ns        38115            42.201k       214.383M/s
bench_sha3::shake256/4096/64       18409 ns        18408 ns        38013            42.303k       215.517M/s
bench_sha3::shake256/4096/128      18483 ns        18482 ns        37888            42.472k       217.961M/s
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( using `clang++` )

```fish
2022-12-05T14:14:08+00:00
Running ./bench/a.out
Run on (4 X 2300.25 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.32, 0.20, 0.08
------------------------------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations average_cpu_cycles bytes_per_second
------------------------------------------------------------------------------------------------------------
bench_sha3::keccakf1600              491 ns          491 ns      1425434             1.102k       388.536M/s
bench_sha3::sha3_224/32              514 ns          514 ns      1357307             1.155k        59.358M/s
bench_sha3::sha3_224/64              517 ns          517 ns      1355599             1.162k       117.999M/s
bench_sha3::sha3_224/128             513 ns          513 ns      1364994             1.154k       237.765M/s
bench_sha3::sha3_224/256            1004 ns         1004 ns       696816             2.281k        243.26M/s
bench_sha3::sha3_224/512            1995 ns         1995 ns       350639              4.56k         244.8M/s
bench_sha3::sha3_224/1024           3959 ns         3959 ns       176820             9.079k       246.655M/s
bench_sha3::sha3_224/2048           7388 ns         7388 ns        94381            16.965k       264.356M/s
bench_sha3::sha3_224/4096          14254 ns        14253 ns        49069            32.756k       274.067M/s
bench_sha3::sha3_256/32              517 ns          517 ns      1363253             1.158k       59.0373M/s
bench_sha3::sha3_256/64              515 ns          515 ns      1360303             1.154k       118.521M/s
bench_sha3::sha3_256/128             514 ns          514 ns      1360903             1.152k       237.483M/s
bench_sha3::sha3_256/256            1009 ns         1009 ns       694233              2.29k       242.007M/s
bench_sha3::sha3_256/512            1996 ns         1996 ns       351147              4.56k       244.683M/s
bench_sha3::sha3_256/1024           3951 ns         3951 ns       177147             9.056k       247.181M/s
bench_sha3::sha3_256/2048           7877 ns         7877 ns        88819            18.087k       247.943M/s
bench_sha3::sha3_256/4096          15239 ns        15239 ns        45910            35.023k       256.328M/s
bench_sha3::sha3_384/32              515 ns          515 ns      1359433             1.152k       59.2735M/s
bench_sha3::sha3_384/64              516 ns          516 ns      1358833             1.154k       118.381M/s
bench_sha3::sha3_384/128            1012 ns         1012 ns       691169             2.297k       120.587M/s
bench_sha3::sha3_384/256            1509 ns         1509 ns       464297             3.439k       161.793M/s
bench_sha3::sha3_384/512            2480 ns         2480 ns       282337             5.674k       196.856M/s
bench_sha3::sha3_384/1024           4932 ns         4932 ns       142078            11.311k       198.024M/s
bench_sha3::sha3_384/2048           9836 ns         9836 ns        71263            22.591k       198.567M/s
bench_sha3::sha3_384/4096          19634 ns        19634 ns        35654            45.132k       198.958M/s
bench_sha3::sha3_512/32              513 ns          513 ns      1366094             1.152k       59.5197M/s
bench_sha3::sha3_512/64              512 ns          512 ns      1371809             1.149k       119.301M/s
bench_sha3::sha3_512/128            1006 ns         1005 ns       696759             2.285k       121.412M/s
bench_sha3::sha3_512/256            1994 ns         1994 ns       350855             4.559k       122.428M/s
bench_sha3::sha3_512/512            3952 ns         3952 ns       177033             9.063k       123.553M/s
bench_sha3::sha3_512/1024           7375 ns         7375 ns        94999            16.935k       132.419M/s
bench_sha3::sha3_512/2048          14229 ns        14228 ns        49178            32.698k       137.269M/s
bench_sha3::sha3_512/4096          27934 ns        27934 ns        25057             64.22k       139.836M/s
bench_sha3::shake128/32/32           710 ns          710 ns       988694             1.611k        85.987M/s
bench_sha3::shake128/32/64           900 ns          900 ns       778795             2.048k       101.747M/s
bench_sha3::shake128/32/128         1280 ns         1280 ns       546235             2.923k       119.183M/s
bench_sha3::shake128/64/32           708 ns          708 ns       990747             1.607k       129.304M/s
bench_sha3::shake128/64/64           898 ns          898 ns       779685             2.043k       135.952M/s
bench_sha3::shake128/64/128         1283 ns         1283 ns       546024             2.928k       142.757M/s
bench_sha3::shake128/128/32          708 ns          708 ns       989657             1.606k       215.609M/s
bench_sha3::shake128/128/64          898 ns          898 ns       778971             2.043k       203.947M/s
bench_sha3::shake128/128/128        1278 ns         1278 ns       545935             2.918k       190.999M/s
bench_sha3::shake128/256/32         1202 ns         1202 ns       582985             2.742k         228.5M/s
bench_sha3::shake128/256/64         1392 ns         1392 ns       502642             3.179k       219.275M/s
bench_sha3::shake128/256/128        1772 ns         1772 ns       395130             4.055k       206.619M/s
bench_sha3::shake128/512/32         2190 ns         2190 ns       319606             5.016k       236.858M/s
bench_sha3::shake128/512/64         2381 ns         2381 ns       294106             5.454k       230.727M/s
bench_sha3::shake128/512/128        2759 ns         2759 ns       253798             6.323k       221.253M/s
bench_sha3::shake128/1024/32        3664 ns         3664 ns       190975             8.405k       274.874M/s
bench_sha3::shake128/1024/64        3854 ns         3854 ns       181562             8.842k       269.223M/s
bench_sha3::shake128/1024/128       4235 ns         4235 ns       165245             9.719k       259.408M/s
bench_sha3::shake128/2048/32        6603 ns         6603 ns       106043            15.165k       300.414M/s
bench_sha3::shake128/2048/64        6790 ns         6790 ns       103132            15.594k       296.653M/s
bench_sha3::shake128/2048/128       7171 ns         7171 ns        97556            16.471k       289.405M/s
bench_sha3::shake128/4096/32       12471 ns        12470 ns        56138             28.66k       315.696M/s
bench_sha3::shake128/4096/64       12662 ns        12662 ns        55294            29.101k        313.31M/s
bench_sha3::shake128/4096/128      13052 ns        13052 ns        53620            29.997k       308.633M/s
bench_sha3::shake256/32/32           705 ns          705 ns       993390             1.599k       86.5983M/s
bench_sha3::shake256/32/64           892 ns          892 ns       784921              2.03k       102.616M/s
bench_sha3::shake256/32/128         1275 ns         1275 ns       548842             2.911k        119.67M/s
bench_sha3::shake256/64/32           706 ns          706 ns       993132             1.602k       129.692M/s
bench_sha3::shake256/64/64           900 ns          900 ns       781725             2.048k       135.656M/s
bench_sha3::shake256/64/128         1276 ns         1276 ns       548214             2.914k       143.464M/s
bench_sha3::shake256/128/32          706 ns          706 ns       991871             1.602k        216.15M/s
bench_sha3::shake256/128/64          895 ns          895 ns       781711             2.038k       204.474M/s
bench_sha3::shake256/128/128        1277 ns         1277 ns       550753             2.915k       191.177M/s
bench_sha3::shake256/256/32         1194 ns         1194 ns       586212             2.724k       230.074M/s
bench_sha3::shake256/256/64         1384 ns         1384 ns       505824             3.162k       220.478M/s
bench_sha3::shake256/256/128        1766 ns         1766 ns       396955             4.039k       207.403M/s
bench_sha3::shake256/512/32         2183 ns         2183 ns       320608             4.999k        237.63M/s
bench_sha3::shake256/512/64         2369 ns         2369 ns       295536             5.428k       231.844M/s
bench_sha3::shake256/512/128        2752 ns         2752 ns       254210             6.308k       221.783M/s
bench_sha3::shake256/1024/32        4138 ns         4138 ns       169095             9.495k       243.383M/s
bench_sha3::shake256/1024/64        4328 ns         4328 ns       161729             9.933k       239.726M/s
bench_sha3::shake256/1024/128       4712 ns         4712 ns       148624            10.815k       233.172M/s
bench_sha3::shake256/2048/32        8067 ns         8067 ns        86724            18.533k       245.882M/s
bench_sha3::shake256/2048/64        8256 ns         8256 ns        84769            18.968k       243.956M/s
bench_sha3::shake256/2048/128       8637 ns         8637 ns        81037            19.843k       240.262M/s
bench_sha3::shake256/4096/32       15438 ns        15438 ns        45302            35.488k       255.002M/s
bench_sha3::shake256/4096/64       15631 ns        15631 ns        44730            35.931k       253.816M/s
bench_sha3::shake256/4096/128      16033 ns        16033 ns        43691            36.856k       251.253M/s
```

---

For comparing performance of Python `hashlib` module & Python wrapper interface built on top of this `sha3` C++ library, issue following command

```fish
make benchpy
```

On Intel(R) Core(TM) i5-8279U CPU @ 2.40GHz, when wrapper C interface is compiled to shared library object using `clang++`, performance comparison looks like

```fish
cd wrapper/python; python3 -m pytest -k bench -v; cd ..
=========================================================================================== test session starts ============================================================================================
platform darwin -- Python 3.10.6, pytest-7.1.3, pluggy-1.0.0 -- /usr/local/opt/python@3.10/bin/python3.10
cachedir: .pytest_cache
benchmark: 3.4.1 (defaults: timer=time.perf_counter disable_gc=False min_rounds=5 min_time=0.000005 max_time=1.0 calibration_precision=10 warmup=False warmup_iterations=100000)
rootdir: /Users/anjan/Documents/work/sha3/wrapper/python
plugins: benchmark-3.4.1
collected 12 items / 4 deselected / 8 selected

test_sha3.py::test_bench_sha3_224_self PASSED                                                                                                                                                        [ 12%]
test_sha3.py::test_bench_sha3_224_hashlib PASSED                                                                                                                                                     [ 25%]
test_sha3.py::test_bench_sha3_256_self PASSED                                                                                                                                                        [ 37%]
test_sha3.py::test_bench_sha3_256_hashlib PASSED                                                                                                                                                     [ 50%]
test_sha3.py::test_bench_sha3_384_self PASSED                                                                                                                                                        [ 62%]
test_sha3.py::test_bench_sha3_384_hashlib PASSED                                                                                                                                                     [ 75%]
test_sha3.py::test_bench_sha3_512_self PASSED                                                                                                                                                        [ 87%]
test_sha3.py::test_bench_sha3_512_hashlib PASSED                                                                                                                                                     [100%]


--------------------------------------------------------------------------------- benchmark 'sha3-224': 2 tests ---------------------------------------------------------------------------------
Name (time in us)                  Min                Max              Mean            StdDev            Median               IQR            Outliers  OPS (Kops/s)            Rounds  Iterations
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
test_bench_sha3_224_hashlib     1.1708 (1.0)      11.6266 (1.0)      1.2292 (1.0)      0.2171 (1.0)      1.2187 (1.0)      0.0126 (1.0)     536;21490      813.5219 (1.0)       85419          10
test_bench_sha3_224_self        1.9797 (1.69)     31.0785 (2.67)     2.0877 (1.70)     0.5655 (2.61)     2.0326 (1.67)     0.0280 (2.22)     690;2973      479.0036 (0.59)      50536          10
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------- benchmark 'sha3-256': 2 tests ---------------------------------------------------------------------------------
Name (time in us)                  Min                Max              Mean            StdDev            Median               IQR            Outliers  OPS (Kops/s)            Rounds  Iterations
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
test_bench_sha3_256_hashlib     1.1701 (1.0)      14.6670 (1.0)      1.2260 (1.0)      0.2296 (1.0)      1.2156 (1.0)      0.0441 (1.51)      577;794      815.6753 (1.0)       83209          10
test_bench_sha3_256_self        1.9464 (1.66)     26.8438 (1.83)     2.0467 (1.67)     0.5406 (2.35)     2.0047 (1.65)     0.0293 (1.0)      532;2165      488.5953 (0.60)      50662          10
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------- benchmark 'sha3-384': 2 tests ---------------------------------------------------------------------------------
Name (time in us)                  Min                Max              Mean            StdDev            Median               IQR            Outliers  OPS (Kops/s)            Rounds  Iterations
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
test_bench_sha3_384_hashlib     1.1554 (1.0)      17.1553 (1.0)      1.2257 (1.0)      0.2638 (1.0)      1.2055 (1.0)      0.0597 (1.79)      682;852      815.8825 (1.0)       84367          10
test_bench_sha3_384_self        1.9566 (1.69)     32.4185 (1.89)     2.0766 (1.69)     0.5168 (1.96)     2.0375 (1.69)     0.0333 (1.0)      430;3212      481.5450 (0.59)      51196          10
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------- benchmark 'sha3-512': 2 tests ----------------------------------------------------------------------------------
Name (time in us)                  Min                Max              Mean            StdDev            Median               IQR             Outliers  OPS (Kops/s)            Rounds  Iterations
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
test_bench_sha3_512_hashlib     1.4670 (1.0)      13.7156 (1.0)      1.5416 (1.0)      0.2780 (1.0)      1.5201 (1.0)      0.0161 (1.0)      680;10144      648.6760 (1.0)       68297          10
test_bench_sha3_512_self        2.3273 (1.59)     30.9500 (2.26)     2.4547 (1.59)     0.5565 (2.00)     2.4073 (1.58)     0.0507 (3.15)       506;911      407.3823 (0.63)      43663          10
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Legend:
  Outliers: 1 Standard Deviation from Mean; 1.5 IQR (InterQuartile Range) from 1st Quartile and 3rd Quartile.
  OPS: Operations Per Second, computed as 1 / Mean
===================================================================================== 8 passed, 4 deselected in 16.20s =====================================================================================
```
