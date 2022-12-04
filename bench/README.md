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
2022-12-04T12:21:12+04:00
Running ./bench/a.out
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB
  L1 Instruction 32 KiB
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB
Load Average: 1.21, 1.91, 1.86
------------------------------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations average_cpu_cycles bytes_per_second
------------------------------------------------------------------------------------------------------------
bench_sha3::keccakf1600              337 ns          337 ns      2078409                790       566.717M/s
bench_sha3::sha3_224/32              350 ns          350 ns      1984532                821       87.1679M/s
bench_sha3::sha3_224/64              350 ns          349 ns      2006892                822       174.645M/s
bench_sha3::sha3_224/128             350 ns          350 ns      1985782                819       349.211M/s
bench_sha3::sha3_224/256             685 ns          685 ns      1007484             1.625k       356.593M/s
bench_sha3::sha3_224/512            1360 ns         1358 ns       504432             3.244k       359.445M/s
bench_sha3::sha3_224/1024           2710 ns         2708 ns       259816             6.486k       360.621M/s
bench_sha3::sha3_224/2048           5079 ns         5073 ns       136791            12.173k        384.99M/s
bench_sha3::sha3_224/4096           9758 ns         9754 ns        70876            23.403k       400.486M/s
bench_sha3::sha3_256/32              351 ns          351 ns      1995393                824       86.9105M/s
bench_sha3::sha3_256/64              351 ns          350 ns      1978446                822       174.229M/s
bench_sha3::sha3_256/128             348 ns          347 ns      2006806                816       351.317M/s
bench_sha3::sha3_256/256             683 ns          683 ns      1011561             1.619k        357.71M/s
bench_sha3::sha3_256/512            1359 ns         1359 ns       502531             3.243k       359.406M/s
bench_sha3::sha3_256/1024           2853 ns         2828 ns       257639             6.828k       345.368M/s
bench_sha3::sha3_256/2048           7075 ns         6474 ns       124365            16.958k       301.689M/s
bench_sha3::sha3_256/4096          13544 ns        12217 ns        59434             32.48k       319.741M/s
bench_sha3::sha3_384/32              610 ns          520 ns      1000000             1.441k       58.7114M/s
bench_sha3::sha3_384/64              390 ns          385 ns      1699902                912       158.718M/s
bench_sha3::sha3_384/128            1310 ns         1144 ns       953717             3.122k       106.745M/s
bench_sha3::sha3_384/256            1105 ns         1091 ns       645031              2.63k       223.697M/s
bench_sha3::sha3_384/512            1963 ns         1826 ns       350233              4.69k       267.442M/s
bench_sha3::sha3_384/1024           3685 ns         3667 ns       192632             8.823k       266.275M/s
bench_sha3::sha3_384/2048           7286 ns         7247 ns        95961            17.463k        269.52M/s
bench_sha3::sha3_384/4096          14435 ns        14325 ns        45709             34.62k        272.69M/s
bench_sha3::sha3_512/32              399 ns          394 ns      1889497                934       77.5442M/s
bench_sha3::sha3_512/64              376 ns          374 ns      1786106                880       163.356M/s
bench_sha3::sha3_512/128             784 ns          776 ns       939787             1.858k       157.314M/s
bench_sha3::sha3_512/256            1432 ns         1430 ns       446081             3.415k       170.727M/s
bench_sha3::sha3_512/512            2861 ns         2857 ns       242524             6.844k       170.924M/s
bench_sha3::sha3_512/1024           5296 ns         5287 ns       127667            12.689k       184.702M/s
bench_sha3::sha3_512/2048          10303 ns        10288 ns        67643             24.71k       189.846M/s
bench_sha3::sha3_512/4096          20287 ns        20246 ns        35088            48.669k       192.938M/s
bench_sha3::shake128/32/32           528 ns          527 ns      1317449              1.25k       115.717M/s
bench_sha3::shake128/32/64           679 ns          677 ns      1036484             1.612k       135.244M/s
bench_sha3::shake128/32/128          961 ns          960 ns       721687             2.288k       158.964M/s
bench_sha3::shake128/64/32           532 ns          532 ns      1319037             1.261k       172.238M/s
bench_sha3::shake128/64/64           680 ns          678 ns      1014478             1.614k        179.96M/s
bench_sha3::shake128/64/128          957 ns          955 ns       734469              2.28k       191.673M/s
bench_sha3::shake128/128/32          535 ns          534 ns      1314727             1.267k       285.559M/s
bench_sha3::shake128/128/64          678 ns          677 ns      1038345              1.61k       270.567M/s
bench_sha3::shake128/128/128         962 ns          960 ns       728977             2.292k       254.251M/s
bench_sha3::shake128/256/32          893 ns          892 ns       796613             2.126k       307.952M/s
bench_sha3::shake128/256/64         1037 ns         1035 ns       681458             2.472k       294.804M/s
bench_sha3::shake128/256/128        1321 ns         1319 ns       532583             3.152k       277.748M/s
bench_sha3::shake128/512/32         1617 ns         1615 ns       432029             3.863k       321.218M/s
bench_sha3::shake128/512/64         1801 ns         1790 ns       391913             4.305k       306.923M/s
bench_sha3::shake128/512/128        2077 ns         2065 ns       341038             4.968k       295.579M/s
bench_sha3::shake128/1024/32        2727 ns         2722 ns       259685             6.527k       369.987M/s
bench_sha3::shake128/1024/64        2845 ns         2841 ns       246746             6.811k       365.182M/s
bench_sha3::shake128/1024/128       3167 ns         3156 ns       221853             7.582k        348.11M/s
bench_sha3::shake128/2048/32        4923 ns         4917 ns       142239            11.798k       403.437M/s
bench_sha3::shake128/2048/64        5022 ns         5016 ns       139137            12.035k       401.571M/s
bench_sha3::shake128/2048/128       5334 ns         5322 ns       131797            12.783k       389.914M/s
bench_sha3::shake128/4096/32        9205 ns         9195 ns        76225            22.075k       428.135M/s
bench_sha3::shake128/4096/64        9443 ns         9431 ns        74378            22.645k       420.668M/s
bench_sha3::shake128/4096/128       9889 ns         9879 ns        74104            23.715k       407.778M/s
bench_sha3::shake256/32/32           532 ns          531 ns      1323902             1.259k       114.949M/s
bench_sha3::shake256/32/64           658 ns          657 ns      1061362             1.561k       139.402M/s
bench_sha3::shake256/32/128          927 ns          926 ns       745943             2.207k       164.864M/s
bench_sha3::shake256/64/32           530 ns          529 ns      1359910             1.255k       173.069M/s
bench_sha3::shake256/64/64           661 ns          660 ns      1052885              1.57k        185.02M/s
bench_sha3::shake256/64/128          923 ns          920 ns       768184             2.198k       198.976M/s
bench_sha3::shake256/128/32          525 ns          524 ns      1324754             1.242k       291.207M/s
bench_sha3::shake256/128/64          662 ns          660 ns      1074048             1.571k       277.423M/s
bench_sha3::shake256/128/128         915 ns          914 ns       771613             2.179k       267.171M/s
bench_sha3::shake256/256/32          916 ns          912 ns       782709             2.179k       301.158M/s
bench_sha3::shake256/256/64         1029 ns         1027 ns       609278             2.452k       297.235M/s
bench_sha3::shake256/256/128        1324 ns         1313 ns       538386             3.158k        278.88M/s
bench_sha3::shake256/512/32         1743 ns         1711 ns       412378             4.163k       303.294M/s
bench_sha3::shake256/512/64         1841 ns         1814 ns       309400               4.4k       302.778M/s
bench_sha3::shake256/512/128        2040 ns         2022 ns       349883             4.878k       301.866M/s
bench_sha3::shake256/1024/32        3076 ns         3041 ns       229573             7.366k       331.181M/s
bench_sha3::shake256/1024/64        3163 ns         3148 ns       221016             7.574k       329.635M/s
bench_sha3::shake256/1024/128       3374 ns         3366 ns       205599              8.08k       326.379M/s
bench_sha3::shake256/2048/32        5832 ns         5824 ns       121143            13.979k       340.625M/s
bench_sha3::shake256/2048/64        5846 ns         5840 ns       119806            14.013k       344.886M/s
bench_sha3::shake256/2048/128       6077 ns         6073 ns       113710            14.569k       341.734M/s
bench_sha3::shake256/4096/32       10894 ns        10883 ns        63854            26.128k       361.735M/s
bench_sha3::shake256/4096/64       11004 ns        10993 ns        63002            26.393k       360.876M/s
bench_sha3::shake256/4096/128      11268 ns        11257 ns        62091            27.025k       357.843M/s
```

### On AWS Graviton2 ( using `g++` )

```fish
2022-12-04T08:25:39+00:00
Running ./bench/a.out
Run on (16 X 166.66 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 48 KiB (x16)
  L2 Unified 2048 KiB (x4)
Load Average: 0.15, 0.03, 0.01
-----------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations bytes_per_second
-----------------------------------------------------------------------------------------
bench_sha3::keccakf1600             1500 ns         1500 ns       466444       127.135M/s
bench_sha3::sha3_224/32             1635 ns         1635 ns       428101       18.6681M/s
bench_sha3::sha3_224/64             1628 ns         1628 ns       430004       37.4943M/s
bench_sha3::sha3_224/128            1626 ns         1626 ns       430405       75.0555M/s
bench_sha3::sha3_224/256            3216 ns         3216 ns       217680       75.9224M/s
bench_sha3::sha3_224/512            6407 ns         6407 ns       109224       76.2132M/s
bench_sha3::sha3_224/1024          12766 ns        12766 ns        54832       76.4976M/s
bench_sha3::sha3_224/2048          23889 ns        23888 ns        29302       81.7601M/s
bench_sha3::sha3_224/4096          46132 ns        46131 ns        15175       84.6774M/s
bench_sha3::sha3_256/32             1805 ns         1805 ns       387825       16.9084M/s
bench_sha3::sha3_256/64             1800 ns         1800 ns       388855       33.9065M/s
bench_sha3::sha3_256/128            1796 ns         1796 ns       389810       67.9789M/s
bench_sha3::sha3_256/256            3553 ns         3553 ns       197000       68.7079M/s
bench_sha3::sha3_256/512            7078 ns         7078 ns        98871       68.9847M/s
bench_sha3::sha3_256/1024          14101 ns        14101 ns        49642       69.2559M/s
bench_sha3::sha3_256/2048          28160 ns        28159 ns        24859       69.3596M/s
bench_sha3::sha3_256/4096          54503 ns        54502 ns        12843       71.6714M/s
bench_sha3::sha3_384/32             1591 ns         1591 ns       440055        19.185M/s
bench_sha3::sha3_384/64             1589 ns         1589 ns       440530       38.4125M/s
bench_sha3::sha3_384/128            3131 ns         3131 ns       223592       38.9912M/s
bench_sha3::sha3_384/256            4681 ns         4681 ns       149779       52.1593M/s
bench_sha3::sha3_384/512            7768 ns         7768 ns        90109       62.8584M/s
bench_sha3::sha3_384/1024          15468 ns        15468 ns        45255       63.1363M/s
bench_sha3::sha3_384/2048          30881 ns        30881 ns        22667       63.2475M/s
bench_sha3::sha3_384/4096          61715 ns        61714 ns        11341       63.2957M/s
bench_sha3::sha3_512/32             1653 ns         1653 ns       423477       18.4631M/s
bench_sha3::sha3_512/64             1647 ns         1647 ns       425080       37.0654M/s
bench_sha3::sha3_512/128            3254 ns         3254 ns       215150       37.5197M/s
bench_sha3::sha3_512/256            6473 ns         6473 ns       108121       37.7176M/s
bench_sha3::sha3_512/512           12892 ns        12891 ns        54296       37.8763M/s
bench_sha3::sha3_512/1024          24128 ns        24127 ns        29013       40.4762M/s
bench_sha3::sha3_512/2048          46602 ns        46601 ns        15021       41.9117M/s
bench_sha3::sha3_512/4096          91539 ns        91537 ns         7647        42.674M/s
bench_sha3::shake128/32/32          1732 ns         1732 ns       404234       35.2481M/s
bench_sha3::shake128/32/64          1787 ns         1787 ns       391652       51.2245M/s
bench_sha3::shake128/32/128         1899 ns         1899 ns       368665       80.3652M/s
bench_sha3::shake128/64/32          1734 ns         1734 ns       403732       52.8043M/s
bench_sha3::shake128/64/64          1789 ns         1789 ns       391178       68.2173M/s
bench_sha3::shake128/64/128         1901 ns         1901 ns       368249       96.3289M/s
bench_sha3::shake128/128/32         1728 ns         1728 ns       405057       88.2951M/s
bench_sha3::shake128/128/64         1784 ns         1784 ns       392404       102.647M/s
bench_sha3::shake128/128/128        1895 ns         1895 ns       369362       128.821M/s
bench_sha3::shake128/256/32         3342 ns         3342 ns       209471       82.1924M/s
bench_sha3::shake128/256/64         3398 ns         3397 ns       206038       89.8238M/s
bench_sha3::shake128/256/128        3509 ns         3509 ns       199497       104.371M/s
bench_sha3::shake128/512/32         6598 ns         6597 ns       106063       78.6369M/s
bench_sha3::shake128/512/64         6652 ns         6652 ns       105239       82.5816M/s
bench_sha3::shake128/512/128        6765 ns         6765 ns       103472       90.2215M/s
bench_sha3::shake128/1024/32       11441 ns        11441 ns        61175       88.0276M/s
bench_sha3::shake128/1024/64       11496 ns        11496 ns        60891       90.2572M/s
bench_sha3::shake128/1024/128      11610 ns        11609 ns        60297       94.6325M/s
bench_sha3::shake128/2048/32       21118 ns        21118 ns        33146        93.932M/s
bench_sha3::shake128/2048/64       21176 ns        21175 ns        33058       95.1201M/s
bench_sha3::shake128/2048/128      21286 ns        21285 ns        32884       97.4935M/s
bench_sha3::shake128/4096/32       40496 ns        40495 ns        17286       97.2153M/s
bench_sha3::shake128/4096/64       40551 ns        40551 ns        17262       97.8341M/s
bench_sha3::shake128/4096/128      40664 ns        40662 ns        17214       99.0674M/s
bench_sha3::shake256/32/32          1629 ns         1629 ns       429762       37.4735M/s
bench_sha3::shake256/32/64          1685 ns         1685 ns       415546       54.3499M/s
bench_sha3::shake256/32/128         1796 ns         1796 ns       389788        84.969M/s
bench_sha3::shake256/64/32          1624 ns         1624 ns       431013       56.3726M/s
bench_sha3::shake256/64/64          1680 ns         1680 ns       416737       72.6771M/s
bench_sha3::shake256/64/128         1791 ns         1791 ns       390834       102.235M/s
bench_sha3::shake256/128/32         1620 ns         1620 ns       432202       94.2104M/s
bench_sha3::shake256/128/64         1675 ns         1675 ns       417816       109.297M/s
bench_sha3::shake256/128/128        1787 ns         1787 ns       391714       136.634M/s
bench_sha3::shake256/256/32         3130 ns         3130 ns       223615       87.7431M/s
bench_sha3::shake256/256/64         3186 ns         3186 ns       219701       95.7824M/s
bench_sha3::shake256/256/128        3297 ns         3297 ns       212278       111.059M/s
bench_sha3::shake256/512/32         6165 ns         6164 ns       113503       84.1613M/s
bench_sha3::shake256/512/64         6222 ns         6222 ns       112500       88.2927M/s
bench_sha3::shake256/512/128        6334 ns         6334 ns       110480       96.3609M/s
bench_sha3::shake256/1024/32       12216 ns        12216 ns        57299       82.4405M/s
bench_sha3::shake256/1024/64       12272 ns        12271 ns        57040       84.5536M/s
bench_sha3::shake256/1024/128      12384 ns        12383 ns        56518       88.7196M/s
bench_sha3::shake256/2048/32       24317 ns        24317 ns        28786       81.5744M/s
bench_sha3::shake256/2048/64       24373 ns        24373 ns        28721       82.6398M/s
bench_sha3::shake256/2048/128      24484 ns        24484 ns        28590        84.757M/s
bench_sha3::shake256/4096/32       46995 ns        46994 ns        14895       83.7723M/s
bench_sha3::shake256/4096/64       47050 ns        47049 ns        14878       84.3218M/s
bench_sha3::shake256/4096/128      47160 ns        47160 ns        14842       85.4183M/s
```

### On AWS Graviton2 ( using `clang++` )

```fish
2022-12-04T08:29:25+00:00
Running ./bench/a.out
Run on (16 X 166.66 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 48 KiB (x16)
  L2 Unified 2048 KiB (x4)
Load Average: 0.14, 0.16, 0.08
-----------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations bytes_per_second
-----------------------------------------------------------------------------------------
bench_sha3::keccakf1600             1399 ns         1399 ns       500352       136.378M/s
bench_sha3::sha3_224/32             1467 ns         1467 ns       476901       20.7973M/s
bench_sha3::sha3_224/64             1463 ns         1463 ns       478473       41.7203M/s
bench_sha3::sha3_224/128            1455 ns         1455 ns       481031       83.8845M/s
bench_sha3::sha3_224/256            2875 ns         2875 ns       243562       84.9254M/s
bench_sha3::sha3_224/512            5729 ns         5729 ns       122145       85.2279M/s
bench_sha3::sha3_224/1024          11413 ns        11412 ns        61340       85.5705M/s
bench_sha3::sha3_224/2048          21349 ns        21348 ns        32793        91.488M/s
bench_sha3::sha3_224/4096          41216 ns        41216 ns        16984       94.7758M/s
bench_sha3::sha3_256/32             1466 ns         1466 ns       477362       20.8112M/s
bench_sha3::sha3_256/64             1461 ns         1461 ns       479153       41.7786M/s
bench_sha3::sha3_256/128            1453 ns         1453 ns       481630        83.991M/s
bench_sha3::sha3_256/256            2871 ns         2871 ns       243809       85.0324M/s
bench_sha3::sha3_256/512            5720 ns         5720 ns       122249       85.3693M/s
bench_sha3::sha3_256/1024          11400 ns        11400 ns        61404       85.6666M/s
bench_sha3::sha3_256/2048          22757 ns        22757 ns        30760       85.8263M/s
bench_sha3::sha3_256/4096          44037 ns        44037 ns        15893       88.7033M/s
bench_sha3::sha3_384/32             1454 ns         1454 ns       481433       20.9893M/s
bench_sha3::sha3_384/64             1450 ns         1450 ns       482857       42.1036M/s
bench_sha3::sha3_384/128            2871 ns         2871 ns       243832       42.5207M/s
bench_sha3::sha3_384/256            4281 ns         4281 ns       163524        57.034M/s
bench_sha3::sha3_384/512            7120 ns         7120 ns        98312       68.5776M/s
bench_sha3::sha3_384/1024          14191 ns        14191 ns        49326       68.8145M/s
bench_sha3::sha3_384/2048          28341 ns        28340 ns        24700       68.9176M/s
bench_sha3::sha3_384/4096          56642 ns        56641 ns        12358       68.9649M/s
bench_sha3::sha3_512/32             1445 ns         1445 ns       484534       21.1241M/s
bench_sha3::sha3_512/64             1440 ns         1440 ns       486259       42.3996M/s
bench_sha3::sha3_512/128            2853 ns         2853 ns       245325       42.7822M/s
bench_sha3::sha3_512/256            5689 ns         5689 ns       122993       42.9125M/s
bench_sha3::sha3_512/512           11336 ns        11336 ns        61747       43.0732M/s
bench_sha3::sha3_512/1024          21217 ns        21216 ns        32995       46.0291M/s
bench_sha3::sha3_512/2048          40972 ns        40972 ns        17084       47.6698M/s
bench_sha3::sha3_512/4096          80487 ns        80486 ns         8697       48.5331M/s
bench_sha3::shake128/32/32          1850 ns         1850 ns       378416       32.9956M/s
bench_sha3::shake128/32/64          2212 ns         2212 ns       316465       41.3904M/s
bench_sha3::shake128/32/128         2936 ns         2936 ns       238428       51.9732M/s
bench_sha3::shake128/64/32          1852 ns         1852 ns       378117       49.4368M/s
bench_sha3::shake128/64/64          2214 ns         2214 ns       316325       55.1377M/s
bench_sha3::shake128/64/128         2938 ns         2938 ns       238261        62.324M/s
bench_sha3::shake128/128/32         1846 ns         1846 ns       379108       82.6389M/s
bench_sha3::shake128/128/64         2208 ns         2208 ns       316967       82.9158M/s
bench_sha3::shake128/128/128        2933 ns         2933 ns       238703       83.2525M/s
bench_sha3::shake128/256/32         3268 ns         3268 ns       214229       84.0575M/s
bench_sha3::shake128/256/64         3630 ns         3630 ns       192891       84.0811M/s
bench_sha3::shake128/256/128        4354 ns         4354 ns       160788       84.1146M/s
bench_sha3::shake128/512/32         6136 ns         6136 ns       114136       84.5556M/s
bench_sha3::shake128/512/64         6497 ns         6497 ns       107717       84.5502M/s
bench_sha3::shake128/512/128        7221 ns         7221 ns        96938       84.5275M/s
bench_sha3::shake128/1024/32       10400 ns        10400 ns        67304       96.8346M/s
bench_sha3::shake128/1024/64       10762 ns        10762 ns        65052       96.4102M/s
bench_sha3::shake128/1024/128      11487 ns        11487 ns        60960        95.645M/s
bench_sha3::shake128/2048/32       18927 ns        18927 ns        36985       104.807M/s
bench_sha3::shake128/2048/64       19288 ns        19288 ns        36291       104.425M/s
bench_sha3::shake128/2048/128      20014 ns        20014 ns        34976       103.689M/s
bench_sha3::shake128/4096/32       35993 ns        35993 ns        19448       109.377M/s
bench_sha3::shake128/4096/64       36354 ns        36354 ns        19255        109.13M/s
bench_sha3::shake128/4096/128      37079 ns        37078 ns        18880       108.643M/s
bench_sha3::shake256/32/32          1846 ns         1846 ns       379422       33.0721M/s
bench_sha3::shake256/32/64          2208 ns         2208 ns       317243       41.4731M/s
bench_sha3::shake256/32/128         2932 ns         2932 ns       238780       52.0511M/s
bench_sha3::shake256/64/32          1839 ns         1839 ns       380566       49.7753M/s
bench_sha3::shake256/64/64          2202 ns         2202 ns       317950       55.4479M/s
bench_sha3::shake256/64/128         2926 ns         2926 ns       239268       62.5877M/s
bench_sha3::shake256/128/32         1829 ns         1829 ns       382760       83.4085M/s
bench_sha3::shake256/128/64         2192 ns         2192 ns       319558       83.5512M/s
bench_sha3::shake256/128/128        2916 ns         2916 ns       240085       83.7388M/s
bench_sha3::shake256/256/32         3252 ns         3251 ns       215593       84.4725M/s
bench_sha3::shake256/256/64         3622 ns         3622 ns       193242       84.2555M/s
bench_sha3::shake256/256/128        4333 ns         4333 ns       161443       84.5103M/s
bench_sha3::shake256/512/32         6096 ns         6096 ns       114871       85.1034M/s
bench_sha3::shake256/512/64         6457 ns         6457 ns       108388       85.0709M/s
bench_sha3::shake256/512/128        7183 ns         7183 ns        97472       84.9694M/s
bench_sha3::shake256/1024/32       11776 ns        11776 ns        59442       85.5199M/s
bench_sha3::shake256/1024/64       12137 ns        12137 ns        57673       85.4906M/s
bench_sha3::shake256/1024/128      12861 ns        12860 ns        54426       85.4272M/s
bench_sha3::shake256/2048/32       23134 ns        23134 ns        30258       85.7459M/s
bench_sha3::shake256/2048/64       23495 ns        23494 ns        29795       85.7297M/s
bench_sha3::shake256/2048/128      24221 ns        24221 ns        28900       85.6789M/s
bench_sha3::shake256/4096/32       44419 ns        44418 ns        15759       88.6293M/s
bench_sha3::shake256/4096/64       44785 ns        44782 ns        15632       88.5902M/s
bench_sha3::shake256/4096/128      45503 ns        45503 ns        15384       88.5295M/s
```

### On AWS Graviton3 ( using `g++` )

```fish
2022-12-04T08:32:30+00:00
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
bench_sha3::keccakf1600              629 ns          629 ns      1111085        303.01M/s
bench_sha3::sha3_224/32              725 ns          725 ns       965070       42.0704M/s
bench_sha3::sha3_224/64              722 ns          722 ns       969306       84.5744M/s
bench_sha3::sha3_224/128             718 ns          718 ns       975150       170.003M/s
bench_sha3::sha3_224/256            1426 ns         1426 ns       491069        171.21M/s
bench_sha3::sha3_224/512            2843 ns         2843 ns       245956       171.739M/s
bench_sha3::sha3_224/1024           5671 ns         5671 ns       123491       172.192M/s
bench_sha3::sha3_224/2048          10607 ns        10607 ns        65995       184.143M/s
bench_sha3::sha3_224/4096          20472 ns        20472 ns        34168       190.814M/s
bench_sha3::sha3_256/32              699 ns          699 ns      1005574       43.6589M/s
bench_sha3::sha3_256/64              691 ns          691 ns      1008578       88.2664M/s
bench_sha3::sha3_256/128             691 ns          691 ns      1011001       176.649M/s
bench_sha3::sha3_256/256            1373 ns         1373 ns       511324       177.772M/s
bench_sha3::sha3_256/512            2745 ns         2744 ns       254866       177.913M/s
bench_sha3::sha3_256/1024           5472 ns         5472 ns       127864       178.478M/s
bench_sha3::sha3_256/2048          10957 ns        10957 ns        63806       178.253M/s
bench_sha3::sha3_256/4096          21229 ns        21228 ns        33011       184.013M/s
bench_sha3::sha3_384/32              754 ns          754 ns       930264       40.4719M/s
bench_sha3::sha3_384/64              747 ns          747 ns       936660       81.6634M/s
bench_sha3::sha3_384/128            1480 ns         1480 ns       472378       82.4587M/s
bench_sha3::sha3_384/256            2232 ns         2232 ns       313351       109.363M/s
bench_sha3::sha3_384/512            3689 ns         3689 ns       189499       132.356M/s
bench_sha3::sha3_384/1024           7363 ns         7363 ns        95177       132.639M/s
bench_sha3::sha3_384/2048          14690 ns        14689 ns        47575       132.961M/s
bench_sha3::sha3_384/4096          29361 ns        29360 ns        23854       133.047M/s
bench_sha3::sha3_512/32              726 ns          726 ns       965443       42.0351M/s
bench_sha3::sha3_512/64              717 ns          717 ns       980229       85.1595M/s
bench_sha3::sha3_512/128            1434 ns         1434 ns       488722       85.1051M/s
bench_sha3::sha3_512/256            2851 ns         2851 ns       246483       85.6222M/s
bench_sha3::sha3_512/512            5666 ns         5666 ns       123546       86.1784M/s
bench_sha3::sha3_512/1024          10588 ns        10588 ns        66126       92.2365M/s
bench_sha3::sha3_512/2048          20433 ns        20433 ns        34251       95.5874M/s
bench_sha3::sha3_512/4096          40147 ns        40146 ns        17442       97.3012M/s
bench_sha3::shake128/32/32           761 ns          761 ns       921827       80.2175M/s
bench_sha3::shake128/32/64           787 ns          787 ns       890315       116.393M/s
bench_sha3::shake128/32/128          836 ns          836 ns       837504       182.491M/s
bench_sha3::shake128/64/32           767 ns          767 ns       911940       119.369M/s
bench_sha3::shake128/64/64           793 ns          793 ns       885345       154.017M/s
bench_sha3::shake128/64/128          841 ns          841 ns       832707       217.823M/s
bench_sha3::shake128/128/32          764 ns          764 ns       915741       199.623M/s
bench_sha3::shake128/128/64          786 ns          786 ns       886538       232.971M/s
bench_sha3::shake128/128/128         836 ns          836 ns       839830       292.203M/s
bench_sha3::shake128/256/32         1482 ns         1482 ns       472423       185.376M/s
bench_sha3::shake128/256/64         1505 ns         1505 ns       464821       202.751M/s
bench_sha3::shake128/256/128        1555 ns         1555 ns       450119       235.507M/s
bench_sha3::shake128/512/32         2941 ns         2941 ns       238061       176.376M/s
bench_sha3::shake128/512/64         2965 ns         2965 ns       236211       185.297M/s
bench_sha3::shake128/512/128        3014 ns         3014 ns       232261       202.497M/s
bench_sha3::shake128/1024/32        5110 ns         5110 ns       136957       197.077M/s
bench_sha3::shake128/1024/64        5138 ns         5138 ns       136120       201.933M/s
bench_sha3::shake128/1024/128       5200 ns         5200 ns       135071       211.283M/s
bench_sha3::shake128/2048/32        9453 ns         9452 ns        74035       209.857M/s
bench_sha3::shake128/2048/64        9476 ns         9476 ns        73829        212.55M/s
bench_sha3::shake128/2048/128       9527 ns         9527 ns        73492       217.821M/s
bench_sha3::shake128/4096/32       18129 ns        18129 ns        38613       217.156M/s
bench_sha3::shake128/4096/64       18149 ns        18148 ns        38563       218.602M/s
bench_sha3::shake128/4096/128      18200 ns        18199 ns        38465       221.347M/s
bench_sha3::shake256/32/32           758 ns          758 ns       923834        80.552M/s
bench_sha3::shake256/32/64           782 ns          782 ns       895611        117.12M/s
bench_sha3::shake256/32/128          831 ns          831 ns       842526       183.647M/s
bench_sha3::shake256/64/32           755 ns          755 ns       926962       121.234M/s
bench_sha3::shake256/64/64           780 ns          780 ns       897175       156.527M/s
bench_sha3::shake256/64/128          830 ns          830 ns       843818       220.724M/s
bench_sha3::shake256/128/32          751 ns          751 ns       931685       203.091M/s
bench_sha3::shake256/128/64          779 ns          779 ns       898261       234.963M/s
bench_sha3::shake256/128/128         829 ns          829 ns       844759       294.542M/s
bench_sha3::shake256/256/32         1473 ns         1473 ns       475036       186.438M/s
bench_sha3::shake256/256/64         1498 ns         1498 ns       467360       203.758M/s
bench_sha3::shake256/256/128        1547 ns         1547 ns       452450       236.666M/s
bench_sha3::shake256/512/32         2913 ns         2913 ns       240267       178.076M/s
bench_sha3::shake256/512/64         2938 ns         2938 ns       238258       186.968M/s
bench_sha3::shake256/512/128        2988 ns         2987 ns       234230       204.306M/s
bench_sha3::shake256/1024/32        5788 ns         5788 ns       120922       173.997M/s
bench_sha3::shake256/1024/64        5813 ns         5813 ns       120399       178.488M/s
bench_sha3::shake256/1024/128       5862 ns         5862 ns       119386       187.413M/s
bench_sha3::shake256/2048/32       11519 ns        11518 ns        60778       172.214M/s
bench_sha3::shake256/2048/64       11544 ns        11544 ns        60638       174.476M/s
bench_sha3::shake256/2048/128      11595 ns        11595 ns        60371        178.98M/s
bench_sha3::shake256/4096/32       22261 ns        22261 ns        31445       176.847M/s
bench_sha3::shake256/4096/64       22290 ns        22290 ns        31403       177.989M/s
bench_sha3::shake256/4096/128      22339 ns        22339 ns        31334       180.328M/s
```

### On AWS Graviton3 ( using `clang++` )

```fish
2022-12-04T08:34:57+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.26, 0.18, 0.07
-----------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations bytes_per_second
-----------------------------------------------------------------------------------------
bench_sha3::keccakf1600              459 ns          459 ns      1525865       415.789M/s
bench_sha3::sha3_224/32              474 ns          474 ns      1477756       64.4352M/s
bench_sha3::sha3_224/64              470 ns          470 ns      1489834        129.92M/s
bench_sha3::sha3_224/128             466 ns          466 ns      1503524       262.215M/s
bench_sha3::sha3_224/256             924 ns          924 ns       757322       264.152M/s
bench_sha3::sha3_224/512            1843 ns         1843 ns       379801       264.898M/s
bench_sha3::sha3_224/1024           3689 ns         3688 ns       189787       264.761M/s
bench_sha3::sha3_224/2048           6899 ns         6899 ns       101456       283.097M/s
bench_sha3::sha3_224/4096          13315 ns        13315 ns        52575       293.375M/s
bench_sha3::sha3_256/32              475 ns          475 ns      1474436       64.2738M/s
bench_sha3::sha3_256/64              471 ns          471 ns      1486340       129.582M/s
bench_sha3::sha3_256/128             465 ns          465 ns      1503830       262.243M/s
bench_sha3::sha3_256/256             925 ns          925 ns       756859       264.026M/s
bench_sha3::sha3_256/512            1842 ns         1842 ns       379990       265.056M/s
bench_sha3::sha3_256/1024           3688 ns         3688 ns       189823       264.785M/s
bench_sha3::sha3_256/2048           7362 ns         7362 ns        95081       265.309M/s
bench_sha3::sha3_256/4096          14240 ns        14240 ns        49158       274.313M/s
bench_sha3::sha3_384/32              472 ns          472 ns      1482101        64.606M/s
bench_sha3::sha3_384/64              470 ns          470 ns      1489406        129.87M/s
bench_sha3::sha3_384/128             927 ns          927 ns       754747       131.638M/s
bench_sha3::sha3_384/256            1384 ns         1384 ns       505991       176.453M/s
bench_sha3::sha3_384/512            2298 ns         2298 ns       304661       212.523M/s
bench_sha3::sha3_384/1024           4601 ns         4601 ns       152147       212.243M/s
bench_sha3::sha3_384/2048           9191 ns         9191 ns        76168       212.511M/s
bench_sha3::sha3_384/4096          18354 ns        18354 ns        38140       212.829M/s
bench_sha3::sha3_512/32              471 ns          471 ns      1486521       64.8079M/s
bench_sha3::sha3_512/64              466 ns          466 ns      1506053       130.896M/s
bench_sha3::sha3_512/128             923 ns          923 ns       758492       132.279M/s
bench_sha3::sha3_512/256            1836 ns         1836 ns       381251       132.974M/s
bench_sha3::sha3_512/512            3677 ns         3677 ns       190378       132.804M/s
bench_sha3::sha3_512/1024           6878 ns         6878 ns       101765       141.981M/s
bench_sha3::sha3_512/2048          13279 ns        13278 ns        52716       147.091M/s
bench_sha3::sha3_512/4096          26090 ns        26090 ns        26829       149.724M/s
bench_sha3::shake128/32/32           585 ns          585 ns      1195968       104.309M/s
bench_sha3::shake128/32/64           694 ns          694 ns      1008351       131.876M/s
bench_sha3::shake128/32/128          909 ns          909 ns       769753       167.797M/s
bench_sha3::shake128/64/32           586 ns          586 ns      1194395        156.21M/s
bench_sha3::shake128/64/64           694 ns          694 ns      1007178       175.779M/s
bench_sha3::shake128/64/128          911 ns          911 ns       768116       200.977M/s
bench_sha3::shake128/128/32          582 ns          582 ns      1203460       262.267M/s
bench_sha3::shake128/128/64          690 ns          690 ns      1014076       265.298M/s
bench_sha3::shake128/128/128         907 ns          907 ns       771572       269.138M/s
bench_sha3::shake128/256/32         1040 ns         1040 ns       673404       264.206M/s
bench_sha3::shake128/256/64         1150 ns         1150 ns       609600       265.466M/s
bench_sha3::shake128/256/128        1372 ns         1372 ns       510024       266.851M/s
bench_sha3::shake128/512/32         1966 ns         1966 ns       355943       263.835M/s
bench_sha3::shake128/512/64         2077 ns         2077 ns       336976       264.465M/s
bench_sha3::shake128/512/128        2297 ns         2297 ns       304739       265.677M/s
bench_sha3::shake128/1024/32        3343 ns         3342 ns       209432       301.305M/s
bench_sha3::shake128/1024/64        3453 ns         3453 ns       202721         300.5M/s
bench_sha3::shake128/1024/128       3673 ns         3673 ns       190580        299.11M/s
bench_sha3::shake128/2048/32        6103 ns         6103 ns       114711       325.052M/s
bench_sha3::shake128/2048/64        6211 ns         6211 ns       112707       324.294M/s
bench_sha3::shake128/2048/128       6428 ns         6427 ns       108911       322.866M/s
bench_sha3::shake128/4096/32       11612 ns        11611 ns        60281       339.046M/s
bench_sha3::shake128/4096/64       11721 ns        11721 ns        59724       338.486M/s
bench_sha3::shake128/4096/128      11937 ns        11936 ns        58641       337.484M/s
bench_sha3::shake256/32/32           589 ns          589 ns      1187791       103.588M/s
bench_sha3::shake256/32/64           698 ns          698 ns      1003403       131.244M/s
bench_sha3::shake256/32/128          914 ns          914 ns       766112       167.002M/s
bench_sha3::shake256/64/32           584 ns          584 ns      1198856       156.801M/s
bench_sha3::shake256/64/64           693 ns          693 ns      1011252       176.192M/s
bench_sha3::shake256/64/128          908 ns          908 ns       770610       201.562M/s
bench_sha3::shake256/128/32          579 ns          579 ns      1209134       263.564M/s
bench_sha3::shake256/128/64          688 ns          688 ns      1018046       266.262M/s
bench_sha3::shake256/128/128         904 ns          904 ns       774426       270.117M/s
bench_sha3::shake256/256/32         1038 ns         1038 ns       674437       264.577M/s
bench_sha3::shake256/256/64         1147 ns         1147 ns       610512       266.168M/s
bench_sha3::shake256/256/128        1362 ns         1362 ns       513831       268.802M/s
bench_sha3::shake256/512/32         1955 ns         1955 ns       358076       265.388M/s
bench_sha3::shake256/512/64         2063 ns         2063 ns       339251       266.239M/s
bench_sha3::shake256/512/128        2279 ns         2279 ns       307130       267.789M/s
bench_sha3::shake256/1024/32        3798 ns         3798 ns       184282       265.159M/s
bench_sha3::shake256/1024/64        3907 ns         3907 ns       179223       265.583M/s
bench_sha3::shake256/1024/128       4121 ns         4121 ns       169847        266.57M/s
bench_sha3::shake256/2048/32        7471 ns         7471 ns        93699       265.521M/s
bench_sha3::shake256/2048/64        7579 ns         7579 ns        92356       265.751M/s
bench_sha3::shake256/2048/128       7795 ns         7795 ns        89782       266.235M/s
bench_sha3::shake256/4096/32       14343 ns        14342 ns        48816       274.485M/s
bench_sha3::shake256/4096/64       14450 ns        14450 ns        48441       274.557M/s
bench_sha3::shake256/4096/128      14664 ns        14664 ns        47736       274.707M/s
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( using `g++` )

```fish
2022-12-04T08:37:09+00:00
Running ./bench/a.out
Run on (4 X 2300.25 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.13, 0.03, 0.01
------------------------------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations average_cpu_cycles bytes_per_second
------------------------------------------------------------------------------------------------------------
bench_sha3::keccakf1600             1336 ns         1336 ns       524560             3.047k       142.794M/s
bench_sha3::sha3_224/32             1109 ns         1109 ns       628047             2.513k       27.5214M/s
bench_sha3::sha3_224/64             1109 ns         1109 ns       631883             2.513k       55.0437M/s
bench_sha3::sha3_224/128            1106 ns         1106 ns       633912             2.505k       110.408M/s
bench_sha3::sha3_224/256            2175 ns         2175 ns       321854             4.965k       112.245M/s
bench_sha3::sha3_224/512            4320 ns         4319 ns       162196             9.897k       113.042M/s
bench_sha3::sha3_224/1024           8573 ns         8573 ns        81642            19.682k        113.91M/s
bench_sha3::sha3_224/2048          16068 ns        16068 ns        43450             36.92k       121.557M/s
bench_sha3::sha3_224/4096          31017 ns        31017 ns        22534            71.304k        125.94M/s
bench_sha3::sha3_256/32             1155 ns         1155 ns       606164             2.618k         26.42M/s
bench_sha3::sha3_256/64             1156 ns         1156 ns       605897             2.619k       52.8112M/s
bench_sha3::sha3_256/128            1162 ns         1162 ns       602718             2.633k       105.071M/s
bench_sha3::sha3_256/256            2275 ns         2274 ns       307624             5.192k       107.339M/s
bench_sha3::sha3_256/512            4573 ns         4573 ns       153275            10.479k       106.772M/s
bench_sha3::sha3_256/1024           9125 ns         9125 ns        76748             20.95k       107.024M/s
bench_sha3::sha3_256/2048          18241 ns        18241 ns        38394            41.915k       107.074M/s
bench_sha3::sha3_256/4096          35275 ns        35273 ns        19833            81.087k       110.742M/s
bench_sha3::sha3_384/32             1102 ns         1102 ns       635856             2.496k       27.6952M/s
bench_sha3::sha3_384/64             1102 ns         1102 ns       635710             2.494k       55.4066M/s
bench_sha3::sha3_384/128            2157 ns         2157 ns       324392             4.924k       56.5833M/s
bench_sha3::sha3_384/256            3202 ns         3202 ns       218457             7.327k        76.254M/s
bench_sha3::sha3_384/512            5311 ns         5310 ns       131496            12.176k       91.9464M/s
bench_sha3::sha3_384/1024          10579 ns        10579 ns        66243            24.293k        92.315M/s
bench_sha3::sha3_384/2048          21119 ns        21116 ns        33175            48.523k        92.495M/s
bench_sha3::sha3_384/4096          42108 ns        42106 ns        16634            96.799k       92.7719M/s
bench_sha3::sha3_512/32             1105 ns         1105 ns       633035             2.502k       27.6183M/s
bench_sha3::sha3_512/64             1101 ns         1101 ns       635885             2.492k       55.4409M/s
bench_sha3::sha3_512/128            2176 ns         2176 ns       321695             4.967k       56.0924M/s
bench_sha3::sha3_512/256            4313 ns         4313 ns       162701              9.88k       56.6082M/s
bench_sha3::sha3_512/512            8569 ns         8568 ns        81722            19.668k       56.9878M/s
bench_sha3::sha3_512/1024          15992 ns        15992 ns        43767            36.738k       61.0639M/s
bench_sha3::sha3_512/2048          30862 ns        30859 ns        22666            70.931k       63.2918M/s
bench_sha3::sha3_512/4096          60556 ns        60555 ns        11559           139.236k       64.5078M/s
bench_sha3::shake128/32/32          1478 ns         1478 ns       472524             3.373k       41.2972M/s
bench_sha3::shake128/32/64          1513 ns         1513 ns       462952             3.458k       60.5157M/s
bench_sha3::shake128/32/128         1568 ns         1568 ns       446404             3.585k       97.3034M/s
bench_sha3::shake128/64/32          1478 ns         1478 ns       472079             3.372k       61.9487M/s
bench_sha3::shake128/64/64          1507 ns         1506 ns       464019             3.443k       81.0328M/s
bench_sha3::shake128/64/128         1562 ns         1562 ns       448113             3.572k       117.198M/s
bench_sha3::shake128/128/32         1472 ns         1472 ns       475581             3.359k       103.679M/s
bench_sha3::shake128/128/64         1512 ns         1512 ns       462651             3.456k       121.092M/s
bench_sha3::shake128/128/128        1559 ns         1559 ns       448800             3.564k       156.593M/s
bench_sha3::shake128/256/32         2866 ns         2866 ns       244420             6.565k       95.8469M/s
bench_sha3::shake128/256/64         2908 ns         2908 ns       238361             6.668k        104.93M/s
bench_sha3::shake128/256/128        2961 ns         2961 ns       236320             6.788k       123.694M/s
bench_sha3::shake128/512/32         5742 ns         5742 ns       121997             13.18k       90.3489M/s
bench_sha3::shake128/512/64         5771 ns         5771 ns       121140            13.252k       95.1813M/s
bench_sha3::shake128/512/128        5821 ns         5821 ns       120349            13.366k       104.861M/s
bench_sha3::shake128/1024/32        9974 ns         9974 ns        69978            22.914k       100.975M/s
bench_sha3::shake128/1024/64       10031 ns        10031 ns        69813            23.048k       103.444M/s
bench_sha3::shake128/1024/128      10061 ns        10061 ns        69496            23.119k       109.196M/s
bench_sha3::shake128/2048/32       18525 ns        18524 ns        37774             42.58k       107.084M/s
bench_sha3::shake128/2048/64       18538 ns        18537 ns        37747            42.613k       108.656M/s
bench_sha3::shake128/2048/128      18594 ns        18594 ns        37591            42.742k       111.608M/s
bench_sha3::shake128/4096/32       35530 ns        35529 ns        19690            81.693k       110.806M/s
bench_sha3::shake128/4096/64       35559 ns        35558 ns        19664            81.759k       111.574M/s
bench_sha3::shake128/4096/128      35594 ns        35593 ns        19625             81.84k       113.177M/s
bench_sha3::shake256/32/32          1330 ns         1330 ns       526433             3.037k       45.8975M/s
bench_sha3::shake256/32/64          1371 ns         1370 ns       510515              3.13k       66.8034M/s
bench_sha3::shake256/32/128         1427 ns         1427 ns       490794              3.26k       106.941M/s
bench_sha3::shake256/64/32          1334 ns         1334 ns       525015             3.046k       68.6385M/s
bench_sha3::shake256/64/64          1368 ns         1368 ns       510377             3.125k       89.2164M/s
bench_sha3::shake256/64/128         1424 ns         1424 ns       491717             3.253k        128.59M/s
bench_sha3::shake256/128/32         1325 ns         1325 ns       528523             3.025k       115.171M/s
bench_sha3::shake256/128/64         1359 ns         1359 ns       515073             3.103k       134.763M/s
bench_sha3::shake256/128/128        1415 ns         1415 ns       494772             3.234k       172.502M/s
bench_sha3::shake256/256/32         2592 ns         2592 ns       269991             5.939k       105.979M/s
bench_sha3::shake256/256/64         2630 ns         2630 ns       266230             6.026k       116.053M/s
bench_sha3::shake256/256/128        2676 ns         2676 ns       261593             6.134k       136.829M/s
bench_sha3::shake256/512/32         5133 ns         5132 ns       136469            11.784k       101.086M/s
bench_sha3::shake256/512/64         5174 ns         5174 ns       135408            11.878k       106.174M/s
bench_sha3::shake256/512/128        5226 ns         5226 ns       133896            11.999k       116.784M/s
bench_sha3::shake256/1024/32       10257 ns        10256 ns        68298            23.569k       98.1905M/s
bench_sha3::shake256/1024/64       10280 ns        10280 ns        68108            23.622k       100.932M/s
bench_sha3::shake256/1024/128      10342 ns        10341 ns        67692            23.766k       106.237M/s
bench_sha3::shake256/2048/32       20472 ns        20472 ns        34182            47.064k        96.895M/s
bench_sha3::shake256/2048/64       20502 ns        20501 ns        34136            47.133k       98.2454M/s
bench_sha3::shake256/2048/128      20571 ns        20571 ns        34031            47.288k        100.88M/s
bench_sha3::shake256/4096/32       39638 ns        39637 ns        17654            91.144k       99.3214M/s
bench_sha3::shake256/4096/64       39660 ns        39659 ns        17647            91.194k       100.035M/s
bench_sha3::shake256/4096/128      39725 ns        39723 ns        17623             91.34k        101.41M/s
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( using `clang++` )

```fish
2022-12-04T08:39:47+00:00
Running ./bench/a.out
Run on (4 X 2300.25 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.26, 0.20, 0.09
------------------------------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations average_cpu_cycles bytes_per_second
------------------------------------------------------------------------------------------------------------
bench_sha3::keccakf1600              491 ns          491 ns      1425023             1.101k       388.771M/s
bench_sha3::sha3_224/32              516 ns          516 ns      1355114             1.159k       59.1475M/s
bench_sha3::sha3_224/64              515 ns          515 ns      1360671             1.157k       118.477M/s
bench_sha3::sha3_224/128             511 ns          511 ns      1371388             1.148k       238.938M/s
bench_sha3::sha3_224/256            1002 ns         1002 ns       696561             2.278k       243.606M/s
bench_sha3::sha3_224/512            1990 ns         1989 ns       351893             4.549k       245.432M/s
bench_sha3::sha3_224/1024           3952 ns         3952 ns       177010             9.063k       247.083M/s
bench_sha3::sha3_224/2048           7365 ns         7365 ns        95139            16.912k       265.196M/s
bench_sha3::sha3_224/4096          14237 ns        14237 ns        49222            32.718k       274.376M/s
bench_sha3::sha3_256/32              514 ns          514 ns      1361288             1.152k       59.3709M/s
bench_sha3::sha3_256/64              516 ns          516 ns      1358727             1.156k       118.313M/s
bench_sha3::sha3_256/128             513 ns          513 ns      1361566              1.15k       237.843M/s
bench_sha3::sha3_256/256            1007 ns         1007 ns       695253             2.286k       242.377M/s
bench_sha3::sha3_256/512            1989 ns         1989 ns       351682             4.545k       245.435M/s
bench_sha3::sha3_256/1024           3948 ns         3948 ns       177203              9.05k       247.379M/s
bench_sha3::sha3_256/2048           7856 ns         7856 ns        88918            18.039k       248.618M/s
bench_sha3::sha3_256/4096          15238 ns        15236 ns        45975            35.019k       256.381M/s
bench_sha3::sha3_384/32              518 ns          518 ns      1353909             1.159k       58.9091M/s
bench_sha3::sha3_384/64              518 ns          518 ns      1353049             1.158k       117.908M/s
bench_sha3::sha3_384/128            1012 ns         1012 ns       690020             2.296k       120.595M/s
bench_sha3::sha3_384/256            1507 ns         1507 ns       464767             3.433k       162.047M/s
bench_sha3::sha3_384/512            2472 ns         2472 ns       283207             5.653k       197.538M/s
bench_sha3::sha3_384/1024           4927 ns         4927 ns       142239              11.3k       198.226M/s
bench_sha3::sha3_384/2048           9807 ns         9806 ns        71394            22.524k        199.17M/s
bench_sha3::sha3_384/4096          19624 ns        19624 ns        35677            45.109k       199.057M/s
bench_sha3::sha3_512/32              513 ns          513 ns      1363676             1.152k       59.4614M/s
bench_sha3::sha3_512/64              509 ns          509 ns      1374820             1.144k       119.831M/s
bench_sha3::sha3_512/128            1001 ns         1001 ns       699436             2.274k       122.008M/s
bench_sha3::sha3_512/256            1988 ns         1987 ns       352385             4.543k       122.845M/s
bench_sha3::sha3_512/512            3941 ns         3941 ns       177803             9.036k       123.908M/s
bench_sha3::sha3_512/1024           7359 ns         7359 ns        95078            16.899k         132.7M/s
bench_sha3::sha3_512/2048          14179 ns        14179 ns        49332            32.584k       137.747M/s
bench_sha3::sha3_512/4096          27844 ns        27843 ns        25099            64.013k       140.294M/s
bench_sha3::shake128/32/32           708 ns          708 ns       988364             1.606k       86.2108M/s
bench_sha3::shake128/32/64           900 ns          900 ns       778236             2.047k       101.766M/s
bench_sha3::shake128/32/128         1281 ns         1281 ns       545164             2.925k       119.096M/s
bench_sha3::shake128/64/32           709 ns          709 ns       987615             1.608k        129.16M/s
bench_sha3::shake128/64/64           899 ns          899 ns       778721             2.046k       135.783M/s
bench_sha3::shake128/64/128         1278 ns         1278 ns       547808             2.918k       143.271M/s
bench_sha3::shake128/128/32          706 ns          706 ns       991560             1.602k       216.113M/s
bench_sha3::shake128/128/64          896 ns          896 ns       781141              2.04k       204.276M/s
bench_sha3::shake128/128/128        1278 ns         1278 ns       548169             2.917k       191.092M/s
bench_sha3::shake128/256/32         1199 ns         1199 ns       584046             2.737k       229.008M/s
bench_sha3::shake128/256/64         1386 ns         1386 ns       504875             3.167k       220.118M/s
bench_sha3::shake128/256/128        1768 ns         1768 ns       396150             4.045k       207.134M/s
bench_sha3::shake128/512/32         2190 ns         2190 ns       319939             5.015k       236.898M/s
bench_sha3::shake128/512/64         2378 ns         2378 ns       294320             5.448k       230.975M/s
bench_sha3::shake128/512/128        2758 ns         2757 ns       253717              6.32k       221.349M/s
bench_sha3::shake128/1024/32        3660 ns         3659 ns       191306             8.395k       275.205M/s
bench_sha3::shake128/1024/64        3851 ns         3851 ns       181728             8.835k       269.458M/s
bench_sha3::shake128/1024/128       4232 ns         4232 ns       165367             9.712k       259.625M/s
bench_sha3::shake128/2048/32        6596 ns         6596 ns       106260            15.148k       300.751M/s
bench_sha3::shake128/2048/64        6786 ns         6786 ns       103065            15.586k       296.821M/s
bench_sha3::shake128/2048/128       7170 ns         7170 ns        97689            16.469k       289.435M/s
bench_sha3::shake128/4096/32       12486 ns        12486 ns        56027            28.698k       315.288M/s
bench_sha3::shake128/4096/64       12684 ns        12683 ns        55229            29.151k       312.798M/s
bench_sha3::shake128/4096/128      13067 ns        13066 ns        52763            30.031k         308.3M/s
bench_sha3::shake256/32/32           705 ns          705 ns       994366             1.599k       86.6117M/s
bench_sha3::shake256/32/64           895 ns          895 ns       782353             2.036k       102.309M/s
bench_sha3::shake256/32/128         1276 ns         1276 ns       548923             2.912k       119.607M/s
bench_sha3::shake256/64/32           705 ns          705 ns       994003               1.6k       129.865M/s
bench_sha3::shake256/64/64           896 ns          896 ns       782167             2.038k       136.313M/s
bench_sha3::shake256/64/128         1276 ns         1276 ns       548018             2.913k       143.493M/s
bench_sha3::shake256/128/32          706 ns          706 ns       992663             1.601k       216.221M/s
bench_sha3::shake256/128/64          894 ns          894 ns       782400             2.035k       204.827M/s
bench_sha3::shake256/128/128        1274 ns         1274 ns       548811             2.908k       191.659M/s
bench_sha3::shake256/256/32         1193 ns         1193 ns       586701             2.723k       230.157M/s
bench_sha3::shake256/256/64         1386 ns         1386 ns       505510             3.165k       220.241M/s
bench_sha3::shake256/256/128        1764 ns         1764 ns       396874             4.036k       207.585M/s
bench_sha3::shake256/512/32         2179 ns         2179 ns       321214             4.989k       238.127M/s
bench_sha3::shake256/512/64         2363 ns         2363 ns       296215             5.414k       232.431M/s
bench_sha3::shake256/512/128        2752 ns         2752 ns       254621             6.308k       221.768M/s
bench_sha3::shake256/1024/32        4138 ns         4138 ns       169193             9.496k       243.375M/s
bench_sha3::shake256/1024/64        4329 ns         4329 ns       161754             9.935k       239.708M/s
bench_sha3::shake256/1024/128       4707 ns         4707 ns       148728            10.805k       233.402M/s
bench_sha3::shake256/2048/32        8038 ns         8037 ns        87040            18.463k       246.809M/s
bench_sha3::shake256/2048/64        8227 ns         8227 ns        85045            18.899k       244.825M/s
bench_sha3::shake256/2048/128       8607 ns         8606 ns        81401            19.773k       241.125M/s
bench_sha3::shake256/4096/32       15412 ns        15412 ns        45424            35.425k       255.443M/s
bench_sha3::shake256/4096/64       15605 ns        15605 ns        44860            35.872k       254.227M/s
bench_sha3::shake256/4096/128      15990 ns        15990 ns        43768            36.756k       251.931M/s
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
