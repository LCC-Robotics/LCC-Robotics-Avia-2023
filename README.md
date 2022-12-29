# LCC Robotics 2023 Repository

This repo contains the code for LCC's robot for AVIA 2023, a robotics competition hosted
by [CRC Robotics](https://robo-crc.ca/).

The project is scaffolded using the `PlatformIO` framework with `megaatmega2560` as its target. We have decided to
use `PlatformIO` because it allows for flexible and platform-agnostic projections with minimal configuration.

## Cloning the Repo 

Make sure you have `git` installed on your machine. Then, clone this repository by command line:

```shell
git clone https://github.com/LCC-Robotics/LCC-Robotics-22-23.git
```

## Build and Upload

### VSCode Extension

Install the `PlatformIO` extension for [VSCode](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide). 

Then, in the `PlatformIO` extension menu, press `Build` to build the project, and then once it's built press `Upload` to upload the firmware to the board.

### PlatformIO Core (CLI)

Install the `PlatformIO` [CLI tool](https://docs.platformio.org/en/stable/core/index.html).

Then, run one of the following commands:

```shell
# build
pio run

# build and upload
pio run --target upload
```

## Notes

If you are using `clangd` or an IDE such as `CLion`, you will need to generate `compile_commands.json` in order to get code completion and error checking working.

Generate `compile_commands.json` using the `PlatoformIO` CLI:

```shell
pio run --target compiledb
```

## Authors

[Kenneth Lew](https://github.com/lew1101)

[Angela Wang](https://github.com/A-Blve)

