# LCC Robotics 2023 Repository

This repo contains the code for LCC's robot for AVIA 2023, a robotics competition hosted by CRC Robotics. 

## PlatformIO

The repo was orginally scaffolded using `vscode-arduino`, however it quickly proved to be a massive hassle due to fact that each team member has to create a custom configuration (which is time-consuming to make), as well as the difficulty (impossibility) it posed to collaboration. It is an absolute abomination of tool. `arduino-ide` is not any better either. Overall, most of the software tools that `Arduino` have made are pretty trash (perhaps with exception to `arduino-cli`). 

As such, we transitioned to the `PlatformIO` framework because it allows for flexible, scalable, and platform-agnostic projects with minimal configuration. This transition proved to be absolutely worth it, as it has blessed the team with its ease of use, whether it be in collaboration, hosting, building or uploading. It has caused zero frustration (unlike `Arduino` tools), and we do not regret the transition the slightest.

## Installation 

Clone this repository using `git`:

    git clone https://github.com/A-Blve/LCC-Robotics-22-23.git

## Build and Upload

To build and upload this project, we recommend using the `VSCode` extension for `PlatformIO`, as it is tried and true by the team. However, it is also possible to build and upload using the `PlatformIO` [CLI tool](https://docs.platformio.org/en/stable/core/index.html) or one of the other official IDE [extensions](https://docs.platformio.org/en/latest/integration/ide/index.html). 

## Authors

Angela Wang

Kenneth Lew
