# base-client-cpp

## Features
 - socket client

<br/>

## Supported Platforms
 - requires a compiler that conforms to the C++20 standard or higher.

### Operating Systems
 - CentOS Stream release 9

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
   - `dnf install perl-FindBin`
   - `dnf install perl-Time-HiRes`
   - `dnf install perl-CPAN`
   - `perl -MCPAN -e 'install Capture::Tiny'`
   - `perl -MCPAN -e 'install DateTime'`
   - `perl -MCPAN -e 'install Date::Parse'`
   - `mkdir build && cd build`
   - `cmake ../ -DENABLE_TEST=on && make -j4 install && make test && make lcov_run`
   - run `./coverage/html/index.html` in your browser

<br/>

## Operation
### socket-client
 - start
   - standalone
     - `./build/install/socket-client -c ./test_data/config/ -s`
   - non standalone
     - `./build/install/socket-client -c ./test_data/config/`
 - stop
```bash
$ telnet 0 10002
Trying 0.0.0.0...
Connected to 0.
Escape character is '^]'.
=== greeting ===
stop
200 ok
```
 - admin
```bash
$ telnet 0 10002
Trying 0.0.0.0...
Connected to 0.
Escape character is '^]'.
=== greeting ===
help
help : print command
log_reconfigure : log reconfigure
quit : connect end
stop : process stop
200 ok
quit
200 ok
```
 - log
   - `/tmp/test/log/socket_client_YYYYMMDD.log`
 - config
   - `./test_data/config/`
 - modify the behavior
   - modify the Job function of Main.cpp
