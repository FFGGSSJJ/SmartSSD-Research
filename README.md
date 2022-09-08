

# SmartSSD Research

## Intro

This research lasts for 3 stages:

1. **2022.05 - 2022.06**: explore the usage of SmartSSD with `Vitis_Acce_Example`, including `Data Transfer`, `Vector Addition`, `Matrix Multiplication`. Implemented programs (`./transfertest`) to test the bandwidth of data transfer in different directions.
2. **2022.06 - 2022.08**: implemented an encoding algorithm, `Run Length Encoding (RLE)`, on the SmartSSD (`./compression`). 
3. **2022.08 - Present**: optimizing the `RLE` encoding program, measure power consumption, ...



## Repo Structure

- `./datatransfer`: program codes implemented for data transfer bandwidth tests
- `./simple_vadd`: Vitis Example of vector addition
- `./matrixmul`: simple matrix multiplication implmented
- `./compression`: rle kernel algorithms implemented (**in-update**)
- `./data`: dataset used to test transfer/encoding programs

> Refer to `docs` if you want to try programs in this project

- `./docs/build.md`: instructions on how to build and simulate/run the program.

- `./docs/issues.md`: issues encountered during development



