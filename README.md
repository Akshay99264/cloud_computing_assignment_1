# Assignment 1

This repository contains code assignment related to Linux Kernel Modules.

## 1. Folder `1`

### Contents:

- **lkm1.c**: Code for the first Linux Kernel Module.
- **lkm2.c**: Code for the second Linux Kernel Module.
- **lkm3.c**: Code for the third Linux Kernel Module.
- **lkm4.c**: Code for the fourth Linux Kernel Module.
- **Makefile**: Makefile for compiling the modules.
- **test_lkm2.c**: Test file for lkm2.
- **test_lkm3.c**: Test file for lkm3.
- **test_lkm4.c**: Test file for lkm4.

### Compilation and Usage:

1. Run the following command to compile all the modules:

    ```bash
    make
    ```

2. After compilation, the following kernel modules will be generated:
   - lkm1.ko
   - lkm2.ko
   - lkm3.ko
   - lkm4.ko

3. To insert any module, run the corresponding command:

   ```bash
   sudo insmod lkm1.ko
   sudo insmod lkm2.ko PID=<pid after running test_lkm2.c>
   sudo insmod lkm3.ko PID=<pid after running test_lkm3.c> virtual_address=<virtual_address after running test_lkm3.c>
   sudo insmod lkm4.ko PID=<pid after running test_lkm4.c>

4. To view message/output
   ```bash
   sudo dmesg

5. To remove any module
   ```bash
   sudo rmmod <module name>
   
# 2_I Folder

This folder contains code and scripts related to assignment 2_I.

## Contents:

- **Makefile**: Makefile for compilation.
- **solution_2_I.c**: Code for assignment 2_I.
- **spock.sh**: Shell script for running `solution_2_I.c`.
- **test.c**: Test file.

## Running Assignment 2_I:

To run the solution for assignment 2_I, follow these steps:


1. Run the provided shell script `spock.sh`:

    ```bash
    ./spock.sh
    ```

This script will execute `solution_2_I.c` and provide the necessary environment for testing.

## Additional Information:

- The `test.c` file may be used for additional testing or as a reference for understanding the behavior of the solution.

# 3 Folder

This folder contains code and files related to assignment 3.

## Contents:

- **hello_procfs.c**: Code for a module.
- **get_pgfaults.c**: Code for another module.
- **Makefile**: Makefile for compilation.

## Compilation and Usage:

1. Run the following command to compile the modules:

    ```bash
    make
    ```

2. To insert a module, run:

    ```bash
    sudo insmod <module_name>
    ```

   Replace `<module_name>` with either `hello_procfs` or `get_pgfaults` based on the module you want to insert.

3. To remove a module, run:

    ```bash
    sudo rmmod <module_name>
    ```

   Again, replace `<module_name>` with the appropriate module name.

## Additional Information:

- The provided Makefile is used for compiling the modules.
- Each module (`hello_procfs` and `get_pgfaults`) serves a specific purpose. Use the appropriate module based on your requirements.

## License:

This code is part of Assignment 1 and is licensed under the [MIT License](LICENSE).
