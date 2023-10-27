# CSP-Pi-GPIO

A minimal viable implementation of a [CSP](https://github.com/spaceinventor/libcsp) program for reading and writing to the Raspberry Pi GPIO pins.

Used to prototype how to create an interface to the CSP network. 

For testing, a coffee pot is connected with a relay to the GPIO pins, and can as such be turned on and off by CSP commands.

## Features

- Creates a CSP node and joins ZMQ network on localhost
- Set internal pull-up on input pins
- Change pin direction and set ouput value by setting CSP parameters
- Polls input pins to keep track of current state

## Usage

### Prerequisites
Partly taken from the [CSH readme](https://github.com/spaceinventor/csh#build):
Requirements: libcurl4-openssl-dev build-essential, libsocketcan-dev, can-utils, libzmq3-dev, libyaml-dev, meson, ninja, pkg-config, fonts-powerline, python3-pip, libelf-dev, libbsd-dev

```bash
sudo apt install libcurl4-openssl-dev git build-essential libsocketcan-dev can-utils libzmq3-dev libyaml-dev pkg-config fonts-powerline python3-pip libelf-dev libbsd-dev
sudo pip3 install meson ninja
```
Sometimes needed (replace 3.5 with your python version):
```bash
link /usr/sbin/ninja /usr/local/lib/python3.5/dist-packages/ninja
export PATH=~/.local/bin:$PATH
```

### Local git repository
When cloning the repo, make sure to clone the submodules as well:
```bash
git clone --recurse-submodules https://github.com/spaceinventor/csh.git
```

When pulling, do so with the submodules aswell:
```bash
git pull --recurse-submodules
```

### Building
Inside the cloned repository, run:

```bash
./configure
./build
```

### Run
run zmqproxy built from the csh repo

```
./builddir/app
```

## Example usage
Assuming zmqproxy and the compiled app has been started, now open a CSH terminal, connecting to the zmq network.

The following shows an example of first checking that there is a connection to the CSP-GPIO node, then setting pin 19 as an output, and finally setting the pin high.

```
$ ./builddir/csh -i init/zmq.csh

  ***********************
  **     CSP   Shell   **
  ***********************

  Copyright (c) 2016-2023 Space Inventor A/S <info@space-inventor.com>
  Compiled: Oct  9 2023 git: v2-239-gc8324cb

  File /home/pi/csh_hosts not found
  Init file: init/zmq.csh
  Version 2
  Hostname: raspberrypi
  Model: #1579 SMP Fri Aug 26 11:13:03 BST 2022
  Revision: 5.15.61-v7l+
  Deduplication: 3
  ZMQ init ZMQ0: addr: 19, pub(tx): [tcp://localhost:6000], sub(rx): [tcp://localhost:7000]

raspberrypi  csp scan
CSP SCAN  [0:16382]
Found something on addr 0...
raspberrypi
#1579 SMP Fri Aug 26 11:13:03
5.15.61-v7l+
Oct  9 2023 21:34:08

Found something on addr 12...
CSP-GPIO
RPi3
1
Oct 27 2023 22:06:06

Found something on addr 19...
raspberrypi
#1579 SMP Fri Aug 26 11:13:03
5.15.61-v7l+
Oct  9 2023 21:34:08

raspberrypi  node 12

raspberrypi  12  list download
Got param: csp_addr:12[1]
Got param: csp_mask:12[1]
Got param: gpio_modes:12[32]
Got param: gpio_outputs:12[32]
Got param: gpio_inputs:12[32]
Got param: serial0:12[1]
Received 6 parameters, of which 0 remote parameters were skipped

strawberrypi  12  set gpio_modes[19] 1 @ 22:12:19 d. 27/10/23
  3:12  gpio_modes           = [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0] u08[32]        Rc

strawberrypi  12  set gpio_outputs[19] 1 @ 22:12:40 d. 27/10/23
  5:12  gpio_outputs         = [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0] u08[32]        Rc

```

## Todo
- [ ] Add zmqproxy from CSH repo
- [ ] Add slash commands 
  - [ ] for setting pin direction 
  - [ ] for setting pin value
  - [ ] for setting pull-up/down
  - [ ] for reading pin value
- [ ] Some kind of init functionality, like in CSH, so we are not limited to the hardcoded ZMQ interface


## How it works
### Main
The main entry point is the `main.c` file. Here, the program is initialized, and the CSP node is created. 
Everything that will run is either called in a loop, or in a callback function. There are multiple loops running in parallel pthreads (POSIX threads). A few are made to loop specific frequencies, such as the `onehz` and `tenhz` loops. Hooking into the looping tasks happen in the `hooks.c` file.

### GPIO
For the GPIO parts (`gpio.c`), we make use of the memory access to the GPIO pins on the Raspberry Pi, using the `/dev/gpiomem` file. This memory can then be directly accessed to either read or configure/set the GPIO pins. The exact addresses for the different GPIO registers are found in the datasheets for the BCM SoC. 
- [BCM2837 / Raspberry Pi 3](https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf)
- [BCM2711 / Raspberry Pi 4](https://datasheets.raspberrypi.org/bcm2711/bcm2711-peripherals.pdf)

### CSP Params
The system parameters are used to set and get the GPIO status. It consists of vmem, which is where the parameter  values are stored, and the definitions of the parameters themselves. 

The VMEM is defined in the header `vmem_config.h` and initialized in `main.c`. In the header, the VMEM address for the different parameters are defined based on their sizes. The VMEM is then initialized in `main.c` as a vmem_file, where it is stored on the filesystem, making it non-volatile.

The parameters are defined in `param_config.c` using the corresponding macro, with all the details, such as size, array length, callback, VMEM location, etc.

### Slash commands (TODO)