# GEA Compiler

A program to convert supported files into the .gea (Galactic Engine Asset) format to be read by Galactic Engine for faster loading times.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Installation

To install and build, run the following commands:
```bash
git clone https://github.com/RomketBoi/GEAcompiler.git
cd GEAcompiler && mkdir build && cd build
cmake .. && cmake --build .
```
You can then add the compiled executable located in `build/bin` to your Galactic Engine project.

## Usage

To use the compiler, simply run the following command:
```bash
gea_compiler path/to/assets/directory/
```
The program will then recursively scan the directory and convert all possible files to the .gea format.

## Contributing

We welcome contributions from everyone. Here are a few ways you can help:

1. **Report bugs**: If you find a bug, please create an issue in our GitHub issue tracker. Describe the problem and include as many details as possible, such as the version of the software you're using, what you did to trigger the bug, and any error messages you saw.

2. **Suggest features**: If you have an idea for a new feature, we'd love to hear about it! Please create an issue in our GitHub issue tracker and describe your idea.

3. **Submit pull requests**: If you've fixed a bug or implemented a new feature, you can submit a pull request. Here's how:

    - Fork the repository and create a new branch for your changes.
    - Make your changes in your branch. Try to keep your changes focused and please follow our coding style.
    - Push your branch to your fork and submit a pull request from your branch to the main repository.
    - We'll review your pull request and merge it if everything looks good.

Please note that this project is released with a [Contributor Code of Conduct](CODE_OF_CONDUCT.md). By participating in this project you agree to abide by its terms.

## License

This project is licensed under the MIT License. This means you can use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the software, under the following conditions:

- You must include the original copyright notice in any copy of the software.
- If you use the software, you must give credit to the original author.

For more details, see the [LICENSE](LICENSE) file.
