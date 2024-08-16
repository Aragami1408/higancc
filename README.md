<br/>
<p align="center">
  <h2 align="center">higancc</h3>

  <p align="center">
A C compiler implementation by Higanbana, made from scratch using C
    <br/>
    <br/>
  </p>
</p>


## About The Project

The motivation of this project is an educational tool to understand the principles of compiler design and implementation. In particular, higancc is yet another c compiler that translate C source code into machine code. Furthermore, the language has additional features to suit my likings.

## Features

- Lexical analysis
- Syntax parsing
- Semantic analysis
- Intermediate code generation
- Code optimization
- Target code generation

## Getting Started

### Prerequisites

- `gcc` or `clang`
- No build system required

### Building the project

1. Clone the repository:
    `git clone https://github.com/Aragami1408/higancc.git`
2. Build the project:
	- In debug mode: `./build.sh debug`
	- In release mode: `./build.sh release`	

### Usage
To compile a C source file:
```
$ bin/higancc input.c -o output
```

## Project Structure
- `src/` - Source files and Header files for the compiler
- `test/` - Test cases

## Contributing

I am open to contributions from anyone who wants to improve this project. If you want to contribute, please follow the steps below:

- If you have suggestions for adding or removing projects, feel free to  [open an issue](https://github.com/Aragami1408/higancc/issues/new)  to discuss it, or directly create a pull request after you edit the  _README.md_  file with necessary changes.
-   Please make sure you check your spelling and grammar.
-   Create individual PR for each suggestion.

### Creating A Pull Request

1.  Fork the Project
2.  Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3.  Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4.  Push to the Branch (`git push origin feature/AmazingFeature`)
5.  Open a Pull Request
