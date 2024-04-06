# Base Client for C++

## Features
 - socket client

<br/>

## Supported Platforms
 - requires a compiler that conforms to the C++20 standard or higher.

### Operating Systems
 - CentOS Stream release 9
 - Ubuntu 22.04.3 LTS

### Compiler
 - gcc version 13.1.0

### Build System
 - cmake version 3.20.2

<br/>

## Build
 - basic
   - `mkdir build && cd build`
   - `cmake ../ && make -j4 install`
   - output
     - `./install/socket-client`
 - test
   - `mkdir build && cd build`
   - `cmake ../ -DENABLE_TEST=on && make -j4 install && make test`
 - code coverage
   - prepare
     - CentOS Stream release 9
       - `dnf install perl-FindBin`
       - `dnf install perl-Time-HiRes`
       - `dnf install perl-CPAN`
       - `perl -MCPAN -e 'install Capture::Tiny'`
       - `perl -MCPAN -e 'install DateTime'`
       - `perl -MCPAN -e 'install Date::Parse'`
     - Ubuntu 22.04.3 LTS
       - `sudo perl -MCPAN -e 'install Capture::Tiny'`
       - `sudo perl -MCPAN -e 'install DateTime'`
   - `mkdir build && cd build`
   - `cmake ../ -DENABLE_TEST=on && make -j4 install && make test && make lcov_run`
   - run `./coverage/html/index.html` in your browser

<br/>

## Operation
### socket-client
 - run
   - `./build/install/socket-client -c ./test_data/config/`
 - log
   - `/tmp/test/log/socket_client_YYYYMMDD.log`
 - config
   - `./test_data/config/`
 - modify the behavior
   - modify the Job function of Main.cpp
