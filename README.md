# sha3
SHA3: Permutation-Based Hash and Extendable-Output Functions.

## Motivation

SHA3 standard by NIST ( i.e. NIST FIPS PUB 202 ) specifies four ( of different digest length ) permutation-based hash functions and two extendable-output functions, which are built on top of keccak-p[1600, 24] permutation.

These hash functions and extendable output functions are pretty commonly used in various post-quantum cryptography algorithms ( those used for public key encryption, key establishment mechanism & digital signature algorithms ), some of which are already declared as selected candidates ( e.g. Kyber, Falcon, Dilithium, SPHINCS+ etc. ) of NIST PQC standardization effort - waiting to be standardized or some are still competing ( e.g. Bike, Classic McEliece etc. ) in final round of standardization. I decided to implement SHA3 specification as a **header-only C++ library**, so that I can make use of it as a modular dependency ( say pinned to specific commit using git submodule ) in libraries where I implement various PQC schemes.

Few of those places, where I've already used `sha3` as ( git submodule based ) dependency

- [Kyber: Post-Quantum Public-key Encryption & Key-establishment Algorithm](https://github.com/itzmeanjan/kyber)
- [Dilithium: Post-Quantum Digital Signature Algorithm](https://github.com/itzmeanjan/dilithium)
- [SPHINCS+: Stateless Hash-based Digital Signature Algorithm](https://github.com/itzmeanjan/sphincs)
- [Falcon: Fast-Fourier Lattice-based Compact Signatures over NTRU - NIST PQC Digital Signature Algorithm](https://github.com/itzmeanjan/falcon)
- [FrodoKEM: Practical Quantum-secure Key Encapsulation from Generic Lattices](https://github.com/itzmeanjan/frodokem)

> **Warning** Above list may not be up-to-date !

Here I'm maintaining a zero-dependency, header-only C++ library, using modern C++ features ( such as C++{>=11} ), which is fairly easy-to-use in your project, implementing SHA3 [specification](https://dx.doi.org/10.6028/NIST.FIPS.202) i.e. NIST FIPS PUB 202. Following algorithms are implemented in `sha3` library.

Algorithm | Input | Output | Behaviour | Namespace + Header
--- | :-: | :-: | :-: | --:
SHA3-224 | N ( >=0 ) -bytes message | 28 -bytes digest | Given N -bytes input message, this routine computes 28 -bytes sha3-224 digest, while *(incrementally)* consuming message into Keccak[448] sponge. | [`sha3_224::sha3_224`](./include/sha3_224.hpp)
SHA3-256 | N ( >=0 ) -bytes message | 32 -bytes digest | Given N -bytes input message, this routine computes 32 -bytes sha3-256 digest, while *(incrementally)* consuming message into Keccak[512] sponge. | [`sha3_256::sha3_256`](./include/sha3_256.hpp)
SHA3-384 | N ( >=0 ) -bytes message | 48 -bytes digest | Given N -bytes input message, this routine computes 48 -bytes sha3-384 digest, while *(incrementally)* consuming message into Keccak[768] sponge. | [`sha3_384::sha3_384`](./include/sha3_384.hpp)
SHA3-512 | N ( >=0 ) -bytes message | 64 -bytes digest | Given N -bytes input message, this routine computes 64 -bytes sha3-512 digest, while *(incrementally)* consuming message into Keccak[1024] sponge. | [`sha3_512::sha3_512`](./include/sha3_512.hpp)
SHAKE-128 | N ( >=0 ) -bytes message | M ( >=0 ) -bytes output | Given N -bytes input message, this routine squeezes arbitrary ( = M ) number of output bytes from Keccak[256] sponge, which has already *(incrementally)* absorbed input bytes. | [`shake128::shake128`](./include/shake128.hpp)
SHAKE-256 | N ( >=0 ) -bytes message | M ( >=0 ) -bytes digest | Given N -bytes input message, this routine squeezes arbitrary ( = M ) number of output bytes from Keccak[512] sponge, which has already *(incrementally)* absorbed input bytes. | [`shake256::shake256`](./include/shake256.hpp)

> **Note** Learn more about NIST PQC standardization effort [here](https://csrc.nist.gov/projects/post-quantum-cryptography/).

## Prerequisites

- A C++ compiler such as `g++`/ `clang++`, with support for C++20 standard library.

```bash
$ g++ --version
g++ (Ubuntu 12.2.0-17ubuntu1) 12.2.0

$ clang++ --version
Ubuntu clang version 15.0.7
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

- Build tools such as `cmake` and `make`.

```bash
$ make --version
GNU Make 4.3

$ cmake --version
cmake version 3.25.1
```

- For benchmarking SHA3 algorithms, targeting CPU systems, `google-benchmark` library and headers are required to be installed system-wide. Follow [this](https://github.com/google/benchmark#installation) guide if you don't have it installed yet.
- If you are on a machine running GNU/Linux kernel and you want to obtain following (see list below), when benchmarking SHA3 algorithms, you should consider building `google-benchmark` library yourself with libPFM support, following [this](https://gist.github.com/itzmeanjan/05dc3e946f635d00c5e0b21aae6203a7) step-by-step guide. Find more about libPFM @ https://perfmon2.sourceforge.net.
    1) CPU cycle count.
    2) Retired instruction count.
    3) Cycles/ byte ( aka cpb ).
    4) Retired instructions/ cycle ( aka ipc ).

## Testing

For ensuring that SHA3 hash function and extendable output function implementations are correct & conformant to the NIST standard ( see [this](https://dx.doi.org/10.6028/NIST.FIPS.202) ), I make use of K(nown) A(nswer) T(ests), generated following [this](https://gist.github.com/itzmeanjan/448f97f9c49d781a5eb3ddd6ea6e7364) gist.

I also test correctness of incremental message absorption and output squeezing property of SHA3 hash and extendable output functions. 

Issue following command for running all the test cases.

```bash
make

[test] SHA3-{224,256,384,512} incremental absorption
[test] SHAKE{128,256} incremental absorption and squeezing
[test] SHA3-224 K(nown) A(nswer) T(ests)
[test] SHA3-256 K(nown) A(nswer) T(ests)
[test] SHA3-384 K(nown) A(nswer) T(ests)
[test] SHA3-512 K(nown) A(nswer) T(ests)
[test] Shake128 Xof K(nown) A(nswer) T(ests)
[test] Shake256 Xof K(nown) A(nswer) T(ests)
```

## Benchmarking

For benchmarking SHA3 hash and extendable output functions, targeting CPU systems, using `google-benchmark`, issue following command.

> **Warning** You must disable CPU scaling during benchmarking, following [this](https://github.com/google/benchmark/blob/4931aefb51d1e5872b096a97f43e13fa0fc33c8c/docs/reducing_variance.md) guide.

> **Note** When benchmarking extendable output functions ( XOFs ), fixed length output of 32/ 64 -bytes are squeezed from sponge ( s.t. all bytes are read in a single call to `squeeze` function ), for each input byte array of length N | 32 <= N <= 4096 and N = 2^i.

> **Note** Following performance figures were collected by issuing `make perf` - on machines running GNU/Linux kernel, with `google-benchmark` library compiled with *libPFM* support.

```bash
make perf      # You must issue this if you built your google-benchmark library with libPFM support.
make benchmark # Or you can simply use this.
```

### On 12th Gen Intel(R) Core(TM) i7-1260P ( compiled with GCC )

```bash
2023-07-08T15:42:22+04:00
Running ./benchmarks/perf.out
Run on (16 X 4051.77 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 1280 KiB (x8)
  L3 Unified 18432 KiB (x1)
Load Average: 0.96, 0.51, 0.47
***WARNING*** There are 65 benchmarks with threads and 2 performance counters were requested. Beware counters will reflect the combined usage across all threads.
-------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations     CYCLES CYCLES/ BYTE INSTRUCTIONS INSTRUCTIONS/ CYCLE bytes_per_second
-------------------------------------------------------------------------------------------------------------------------------------------------
bench_sha3::keccakf1600             204 ns          204 ns      3439911    952.708      4.76354       5.689k              5.9714       934.652M/s
bench_sha3::sha3_224/32             268 ns          268 ns      2609056   1.25165k      20.8608       5.953k             4.75612       213.357M/s
bench_sha3::sha3_224/64             268 ns          268 ns      2616004    1.2519k      13.6076       5.953k             4.75519       327.098M/s
bench_sha3::sha3_224/128            268 ns          268 ns      2606948   1.25272k      8.03028       5.963k             4.76003       554.829M/s
bench_sha3::sha3_224/256            473 ns          473 ns      1478677   2.21034k      7.78289      11.703k             5.29466       572.377M/s
bench_sha3::sha3_224/512            929 ns          930 ns       751988   4.34973k      8.05505      23.143k             5.32056       554.042M/s
bench_sha3::sha3_224/1024          1849 ns         1849 ns       378555   8.63636k      8.20947      46.012k             5.32771       542.526M/s
bench_sha3::sha3_224/2048          3453 ns         3453 ns       202631   16.1272k      7.76842      86.053k             5.33588       573.297M/s
bench_sha3::sha3_224/4096          6669 ns         6670 ns       104466   31.1458k      7.55234     166.133k             5.33404       589.613M/s
bench_sha3::sha3_256/32             263 ns          263 ns      2650567   1.23211k      19.2517       5.996k             4.86645        231.65M/s
bench_sha3::sha3_256/64             263 ns          263 ns      2653115   1.22958k      12.8081       5.996k             4.87647       348.609M/s
bench_sha3::sha3_256/128            263 ns          263 ns      2646574   1.23145k      7.69656       6.006k             4.87718       579.418M/s
bench_sha3::sha3_256/256            488 ns          488 ns      1432773   2.28049k      7.91838      12.337k              5.4098       562.628M/s
bench_sha3::sha3_256/512            983 ns          983 ns       708733   4.59845k      8.45304      24.973k             5.43074       527.644M/s
bench_sha3::sha3_256/1024          1975 ns         1976 ns       352876   9.22594k      8.73668      50.245k             5.44606       509.748M/s
bench_sha3::sha3_256/2048          3960 ns         3961 ns       176340   18.5124k      8.90021      100.78k             5.44391       500.785M/s
bench_sha3::sha3_256/4096          7683 ns         7684 ns        90578    35.889k      8.69405     195.548k             5.44869       512.316M/s
bench_sha3::sha3_384/32             235 ns          235 ns      2982725    1097.47      13.7184       5.944k             5.41608       325.237M/s
bench_sha3::sha3_384/64             234 ns          234 ns      2991613    1093.01      9.75899       5.944k             5.43821       457.036M/s
bench_sha3::sha3_384/128            427 ns          427 ns      1640165   1.99162k       11.316      11.739k             5.89421       393.229M/s
bench_sha3::sha3_384/256            640 ns          640 ns      1097069   2.98361k      9.81449      17.525k             5.87376       453.017M/s
bench_sha3::sha3_384/512           1067 ns         1067 ns       657046   4.95993k      8.85703      29.105k             5.86802       500.614M/s
bench_sha3::sha3_384/1024          2125 ns         2125 ns       329892   9.89002k      9.22577       58.03k             5.86753       481.036M/s
bench_sha3::sha3_384/2048          4286 ns         4287 ns       163077   19.7671k      9.43088      115.88k             5.86226       466.297M/s
bench_sha3::sha3_384/4096          8580 ns         8581 ns        81876    39.523k      9.53739      231.57k             5.85913       460.545M/s
bench_sha3::sha3_512/32             234 ns          234 ns      2987556    1086.79      11.3208       5.916k             5.44354       391.513M/s
bench_sha3::sha3_512/64             236 ns          236 ns      2989171     1082.7      8.45863       5.916k              5.4641       516.829M/s
bench_sha3::sha3_512/128            436 ns          436 ns      1603019   1.98845k      10.3565      11.717k             5.89252       419.567M/s
bench_sha3::sha3_512/256            869 ns          869 ns       805519   3.96345k      12.3858      23.299k             5.87846        351.27M/s
bench_sha3::sha3_512/512           1735 ns         1735 ns       404000   7.92833k      13.7645      46.464k              5.8605        316.56M/s
bench_sha3::sha3_512/1024          3257 ns         3258 ns       214102   14.8805k       13.677      86.999k              5.8465       318.524M/s
bench_sha3::sha3_512/2048          6276 ns         6277 ns       110904   28.7174k      13.5973     168.074k             5.85268       320.902M/s
bench_sha3::sha3_512/4096         12376 ns        12378 ns        55868   56.4662k      13.5736     330.222k             5.84814       320.506M/s
bench_sha3::shake128/32/32          252 ns          252 ns      2767260   1.17845k      18.4133       5.857k             4.97008       242.204M/s
bench_sha3::shake128/64/32          254 ns          254 ns      2755491   1.18026k      12.2944       5.857k             4.96247       360.749M/s
bench_sha3::shake128/128/32         254 ns          254 ns      2768736   1.18082k      7.38011       5.867k             4.96859       600.413M/s
bench_sha3::shake128/256/32         476 ns          477 ns      1471521   2.18048k       7.5711      11.875k             5.44606       576.356M/s
bench_sha3::shake128/512/32         956 ns          956 ns       729121   4.34911k      7.99468      23.848k             5.48343       542.703M/s
bench_sha3::shake128/1024/32       1678 ns         1679 ns       415485   7.62064k      7.21652      41.819k             5.48759       599.903M/s
bench_sha3::shake128/2048/32       3120 ns         3120 ns       224206   14.1833k      6.81891      77.766k             5.48292       635.698M/s
bench_sha3::shake128/4096/32       6024 ns         6025 ns       115518   27.3013k      6.61368     149.658k             5.48172       653.407M/s
bench_sha3::shake128/32/64          253 ns          253 ns      2760989   1.18037k      12.2955       5.857k             4.96201       361.204M/s
bench_sha3::shake128/64/64          254 ns          254 ns      2754941   1.18295k      9.24177       5.857k              4.9512       480.834M/s
bench_sha3::shake128/128/64         253 ns          253 ns      2759739   1.18263k      6.15954       5.867k             4.96097       722.779M/s
bench_sha3::shake128/256/64         471 ns          471 ns      1485089   2.18466k      6.82706      11.875k             5.43563       648.192M/s
bench_sha3::shake128/512/64         951 ns          951 ns       736512   4.35036k      7.55271      23.848k             5.48184       577.792M/s
bench_sha3::shake128/1024/64       1677 ns         1677 ns       420008   7.62324k      7.00666      41.819k             5.48572       618.593M/s
bench_sha3::shake128/2048/64       3115 ns         3116 ns       223929   14.1815k      6.71472      77.766k             5.48363        646.41M/s
bench_sha3::shake128/4096/64       6015 ns         6016 ns       116819   27.2977k      6.56195     149.658k             5.48244        659.48M/s
bench_sha3::shake256/32/32          260 ns          260 ns      2690296   1.21399k      18.9686       5.948k             4.89955       234.471M/s
bench_sha3::shake256/64/32          263 ns          263 ns      2661225   1.22654k      12.7765       5.948k              4.8494       348.392M/s
bench_sha3::shake256/128/32         260 ns          260 ns      2689856   1.21392k      7.58699       5.958k             4.90807       586.243M/s
bench_sha3::shake256/256/32         481 ns          481 ns      1451859   2.22953k      7.74142      12.056k             5.40742       570.424M/s
bench_sha3::shake256/512/32         980 ns          980 ns       710553   4.44622k       8.1732      24.212k             5.44552        529.45M/s
bench_sha3::shake256/1024/32       1952 ns         1952 ns       357406   8.89541k      8.42369      48.524k             5.45495       515.945M/s
bench_sha3::shake256/2048/32       3901 ns         3901 ns       178423   17.7815k       8.5488      97.139k             5.46293       508.487M/s
bench_sha3::shake256/4096/32       7565 ns         7566 ns        92604    34.432k       8.3411     188.307k             5.46895       520.357M/s
bench_sha3::shake256/32/64          259 ns          260 ns      2688627   1.21247k      12.6299       5.948k             4.90569       352.762M/s
bench_sha3::shake256/64/64          262 ns          262 ns      2665752   1.22605k       9.5785       5.948k             4.85136       465.485M/s
bench_sha3::shake256/128/64         259 ns          259 ns      2667396    1.2141k      6.32343       5.958k             4.90734        705.65M/s
bench_sha3::shake256/256/64         487 ns          487 ns      1469916    2.1609k      6.75282     11.6777k             5.40409        626.93M/s
bench_sha3::shake256/512/64         949 ns          950 ns       735056   4.44382k      7.71497      24.212k             5.44846        578.46M/s
bench_sha3::shake256/1024/64       1902 ns         1902 ns       367509   8.90184k      8.18184      48.524k             5.45101       545.562M/s
bench_sha3::shake256/2048/64       3796 ns         3797 ns       184636   17.7667k      8.41226      97.139k             5.46748       530.459M/s
bench_sha3::shake256/4096/64       7390 ns         7391 ns        94825   34.4484k      8.28087     188.307k             5.46635        536.76M/s
```

### On ARM Cortex-A72 ( compiled with GCC )

```bash
2023-07-08T11:35:19+00:00
Running ./benchmarks/perf.out
Run on (16 X 166.66 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 48 KiB (x16)
  L2 Unified 2048 KiB (x4)
Load Average: 0.34, 0.08, 0.03
***WARNING*** There are 65 benchmarks with threads and 2 performance counters were requested. Beware counters will reflect the combined usage across all threads.
-------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations     CYCLES CYCLES/ BYTE INSTRUCTIONS INSTRUCTIONS/ CYCLE bytes_per_second
-------------------------------------------------------------------------------------------------------------------------------------------------
bench_sha3::keccakf1600             748 ns          748 ns       935763   1.71906k      8.59531       3.918k             2.27915       254.971M/s
bench_sha3::sha3_224/32            1017 ns         1017 ns       688050   2.33817k      38.9695       4.715k             2.01654       56.2473M/s
bench_sha3::sha3_224/64            1017 ns         1017 ns       688394   2.33707k      25.4029       4.719k              2.0192        86.288M/s
bench_sha3::sha3_224/128           1026 ns         1026 ns       682512   2.35717k        15.11       4.729k             2.00622       145.067M/s
bench_sha3::sha3_224/256           1842 ns         1841 ns       380147   4.23244k       14.903       9.257k             2.18715       147.079M/s
bench_sha3::sha3_224/512           3469 ns         3469 ns       201789   7.97336k      14.7655      18.287k             2.29351       148.454M/s
bench_sha3::sha3_224/1024          6720 ns         6720 ns       104148   15.4457k      14.6822      36.345k             2.35309       149.295M/s
bench_sha3::sha3_224/2048         12413 ns        12413 ns        56385   28.5307k      13.7431      67.964k             2.38214       159.496M/s
bench_sha3::sha3_224/4096         23798 ns        23798 ns        29416   54.6972k      13.2631     131.206k             2.39877       165.267M/s
bench_sha3::sha3_256/32             959 ns          959 ns       730221   2.20311k      34.4235       4.466k             2.02714       63.6757M/s
bench_sha3::sha3_256/64             960 ns          960 ns       729215   2.20611k      22.9803        4.47k             2.02619       95.3837M/s
bench_sha3::sha3_256/128            968 ns          968 ns       723345   2.22406k      13.9004        4.48k             2.01434       157.688M/s
bench_sha3::sha3_256/256           1891 ns         1891 ns       370081   4.34731k      15.0948       9.214k             2.11947       145.214M/s
bench_sha3::sha3_256/512           3740 ns         3740 ns       187159   8.59603k      15.8015      18.664k             2.17124       138.718M/s
bench_sha3::sha3_256/1024          7411 ns         7411 ns        94434   17.0343k       16.131       37.56k             2.20496       135.882M/s
bench_sha3::sha3_256/2048         14761 ns        14761 ns        47420   33.9277k      16.3114      75.351k             2.22093       134.382M/s
bench_sha3::sha3_256/4096         28554 ns        28553 ns        24515   65.6282k      15.8983     146.225k             2.22808       137.874M/s
bench_sha3::sha3_384/32             843 ns          843 ns       830339   1.93758k      24.2197       4.332k             2.23578       90.5051M/s
bench_sha3::sha3_384/64             845 ns          845 ns       828545   1.94137k      17.3337       4.336k             2.23348       126.456M/s
bench_sha3::sha3_384/128           1640 ns         1640 ns       426835   3.76945k      21.4173       8.844k             2.34623       102.347M/s
bench_sha3::sha3_384/256           2449 ns         2449 ns       285968   5.62864k      18.5153      13.332k              2.3686       118.388M/s
bench_sha3::sha3_384/512           4038 ns         4037 ns       173364   9.28018k      16.5717      22.306k             2.40362       132.275M/s
bench_sha3::sha3_384/1024          8015 ns         8015 ns        87333   18.4208k      17.1836      44.726k             2.42801       127.558M/s
bench_sha3::sha3_384/2048         15956 ns        15955 ns        43870   36.6728k      17.4966      89.566k              2.4423        125.28M/s
bench_sha3::sha3_384/4096         31848 ns        31848 ns        21980   73.1986k      17.6638      179.24k             2.44868       124.091M/s
bench_sha3::sha3_512/32             858 ns          858 ns       816157   1.97113k      20.5326       4.571k             2.31898       106.757M/s
bench_sha3::sha3_512/64             859 ns          859 ns       814529    1.9751k      15.4305       4.575k             2.31634       142.051M/s
bench_sha3::sha3_512/128           1652 ns         1652 ns       423774   3.79663k      19.7741       8.842k             2.32891       110.849M/s
bench_sha3::sha3_512/256           3237 ns         3237 ns       216278   7.43927k      23.2477      17.316k             2.32765       94.2899M/s
bench_sha3::sha3_512/512           6386 ns         6386 ns       109566   14.6777k      25.4822      34.261k             2.33422       86.0212M/s
bench_sha3::sha3_512/1024         11900 ns        11900 ns        58820   27.3526k      25.1403      63.919k             2.33685       87.1907M/s
bench_sha3::sha3_512/2048         22930 ns        22929 ns        30529   52.7017k      24.9535     123.237k             2.33839       87.8415M/s
bench_sha3::sha3_512/4096         44997 ns        44997 ns        15556   103.426k      24.8621     241.877k             2.33864        88.168M/s
bench_sha3::shake128/32/32         1025 ns         1025 ns       682686   2.35667k      36.8229       4.987k             2.11613       59.5263M/s
bench_sha3::shake128/64/32         1027 ns         1027 ns       681482   2.36069k      24.5905       4.991k             2.11421       89.1394M/s
bench_sha3::shake128/128/32        1035 ns         1035 ns       676557   2.37771k      14.8607       5.001k             2.10329       147.499M/s
bench_sha3::shake128/256/32        2012 ns         2012 ns       347935   4.62409k      16.0559        9.81k              2.1215       136.519M/s
bench_sha3::shake128/512/32        3984 ns         3984 ns       175706   9.15775k      16.8341      19.413k             2.11984       130.209M/s
bench_sha3::shake128/1024/32       6934 ns         6934 ns       100925   15.9384k      15.0932       33.83k             2.12255        145.23M/s
bench_sha3::shake128/2048/32      12838 ns        12838 ns        54523   29.5074k      14.1863      62.666k             2.12374       154.515M/s
bench_sha3::shake128/4096/32      24647 ns        24646 ns        28401   56.6483k      13.7229     120.342k             2.12437        159.73M/s
bench_sha3::shake128/32/64         1027 ns         1027 ns       681459   2.36069k      24.5906       4.991k             2.11421       89.1379M/s
bench_sha3::shake128/64/64         1029 ns         1029 ns       680229   2.36499k      18.4765       4.995k             2.11206        118.63M/s
bench_sha3::shake128/128/64        1036 ns         1036 ns       675381   2.38197k      12.4061       5.005k              2.1012        176.68M/s
bench_sha3::shake128/256/64        2014 ns         2014 ns       347629   4.62821k      14.4632       9.814k             2.12047       151.554M/s
bench_sha3::shake128/512/64        3987 ns         3987 ns       175570   9.16452k      15.9106      19.417k             2.11871        137.77M/s
bench_sha3::shake128/1024/64       6937 ns         6936 ns       100899   15.9425k      14.6531      33.834k             2.12225       149.587M/s
bench_sha3::shake128/2048/64      12841 ns        12840 ns        54517   29.5131k       13.974       62.67k             2.12346        156.86M/s
bench_sha3::shake128/4096/64      24653 ns        24653 ns        28394   56.6638k      13.6211     120.346k             2.12386       160.925M/s
bench_sha3::shake256/32/32          913 ns          913 ns       766582   2.09816k      32.7837       4.513k             2.15093       66.8602M/s
bench_sha3::shake256/64/32          915 ns          915 ns       765288   2.10216k      21.8975       4.517k             2.14875       100.101M/s
bench_sha3::shake256/128/32         922 ns          922 ns       759117   2.11919k      13.2449       4.527k              2.1362       165.492M/s
bench_sha3::shake256/256/32        1826 ns         1826 ns       383400   4.19635k      14.5706       9.216k              2.1962       150.434M/s
bench_sha3::shake256/512/32        3634 ns         3634 ns       192682   8.35169k      15.3524      18.576k             2.22422       142.771M/s
bench_sha3::shake256/1024/32       7225 ns         7224 ns        96880   16.6042k      15.7237      37.292k             2.24593         139.4M/s
bench_sha3::shake256/2048/32      14417 ns        14417 ns        48553   33.1355k      15.9305      74.723k             2.25508       137.592M/s
bench_sha3::shake256/4096/32      27909 ns        27909 ns        25080   64.1458k      15.5392     144.922k             2.25926       141.058M/s
bench_sha3::shake256/32/64          915 ns          915 ns       765298   2.10212k      21.8971       4.517k             2.14878       100.103M/s
bench_sha3::shake256/64/64          916 ns          916 ns       763816   2.10626k      16.4551       4.521k             2.14646       133.203M/s
bench_sha3::shake256/128/64         924 ns          924 ns       757697   2.12317k      11.0582       4.531k             2.13407       198.221M/s
bench_sha3::shake256/256/64        1827 ns         1827 ns       383020   4.20031k       13.126        9.22k             2.19507       166.992M/s
bench_sha3::shake256/512/64        3636 ns         3636 ns       192531   8.35764k      14.5098       18.58k             2.22312       151.064M/s
bench_sha3::shake256/1024/64       7226 ns         7226 ns        96866   16.6075k      15.2642      37.296k             2.24574       143.596M/s
bench_sha3::shake256/2048/64      14419 ns        14419 ns        48547   33.1397k      15.6911      74.727k             2.25491       139.691M/s
bench_sha3::shake256/4096/64      27913 ns        27912 ns        25080   64.1527k      15.4213     144.926k             2.25908       142.134M/s
```

## Usage

`sha3` C++ header-only library is written such that it's fairly easy for one to start using it in their project. All one needs to do

- Include proper header files ( select what you need by name ).
- Use proper struct(s)/ API(s)/ constant(s) ( see [usage examples](./example) or [test cases](./include/tests) ).
- When compiling, let your compiler know where it can find respective header files, which is `./include` directory.

Scheme | Header | Namespace | Example
--- | --- | --- | --:
SHA3-224 | ./include/sha3_224.hpp | `sha3_224::` | [example/sha3_224.cpp](./example/sha3_224.cpp)
SHA3-256 | ./include/sha3_256.hpp | `sha3_256::` | [example/sha3_256.cpp](./example/sha3_256.cpp)
SHA3-384 | ./include/sha3_384.hpp | `sha3_384::` | [example/sha3_384.cpp](./example/sha3_384.cpp)
SHA3-512 | ./include/sha3_512.hpp | `sha3_512::` | [example/sha3_512.cpp](./example/sha3_512.cpp)
SHAKE128 | ./include/shake128.hpp | `shake128::` | [example/shake128.cpp](./example/shake128.cpp)
SHAKE256 | ./include/shake256.hpp | `shake256::` | [example/shake256.cpp](./example/shake256.cpp)

```bash
$ g++ -std=c++20 -Wall -O3 -march=native -I include example/sha3_224.cpp && ./a.out
SHA3-224

Input  : 043f0fa310343b6ca42c3d2ab6f168574fd41774d49c9c1e5922c2cd60b43dbb
Output : 3bfbd5e41e850f29daf9c08dbcaca7c43ca939e7d6c0b6d8993c6af4

# ---

$ g++ -std=c++20 -Wall -O3 -march=native -I include example/sha3_256.cpp && ./a.out
SHA3-256

Input  : 70a3bf382218c7f4ae25775ab1d21f9d48e2f03af70dcdec790a338e982e6fa8
Output : 57be0ef9634da2d94219c53032809f4ffc145df6782279a8059afe607715d675

# ---

$ g++ -std=c++20 -Wall -O3 -march=native -I include example/sha3_384.cpp && ./a.out
SHA3-384

Input  : 314686636dc0499f2ebf0a201fe2d44e2e8888ac1109939998230f2cba5d0e94
Output : 554f4506a1b73724d0ce25cc4a0c0b4fc26478cde43013a59c7e25a22e3e73fbcfa731fe9f5050f77e82530fba7de512

# ---

$ g++ -std=c++20 -Wall -O3 -march=native -I include example/sha3_512.cpp && ./a.out
SHA3-512

Input  : 2c3c0ae485204067f1ecbc69a8fefd19a94c9c1552158a8d57a6612b3202f373
Output : 578386bdd6eb816d6d0cbc984351c889f70675a2661ba605aa65ce204b88a6d6553946c52eeac937647929f99a2ddd7cc6645bc1e89df8ab795faba6dd7d0d45

# ---

$ g++ -std=c++20 -Wall -O3 -march=native -I include example/shake128.cpp && ./a.out
SHAKE-128

Input  : 8814e9f091cd4ee6ac6795be43b25b4d741143f4d7f7e9858731447359eaa1e8
Output : d32991406e38740f9b9b2674e59259891bfd23f9d6ea71a816c3133466163dacb3b1cefc341cbda8

# ---

$ g++ -std=c++20 -Wall -O3 -march=native -I include example/shake256.cpp && ./a.out
SHAKE-256

Input  : a6506638e34127e0a8415241479c968c20422f46497663eaf244f205a756f0b3
Output : ce679163b642380365c3c11dcbca7a36ddd01cefba35b8ec18ad937268f584999c6e8ae061c251dd
```
