# Projet_Cy

The C-Wire project aims to provide a comprehensive analysis of energy data within a complex network structured around a hierarchy of power plants, substations, and distribution posts. The goal is to identify instances of overproduction and underproduction while calculating the distribution of energy consumption between businesses and individual consumers.

## Table of content 
- [Project Description](#project-description)
- [Features](#features)
- [Shell mode](#Shell-mode)
- [C mode](#c-mode)
- [Installation](#installation)
- [Usage](#usage)
- [Script Shell execution](#script-shell-execution)
- [C execution program](#c-execution-program)
- [File structure](#file-structure)
- [Authors](#authors)

## Project Description
The C-Wire project aims to provide a synthesis of energy data from a complex network based on a hierarchical system of power plants, substations, and distribution stations. The goal is to identify situations of overproduction, underproduction, and to calculate the distribution of energy consumed by businesses and individuals.

## Features
Script Shell Mode
	•	Data Filtering: Selection of relevant information from the input CSV file.
	•	Option Validation: Verification of passed arguments (stations, consumers, etc.).
	•	Output File Creation: Generation of CSV files containing the results of the analysis.

C Program Mode
	•	Data Processing: Calculation of energy consumption using an AVL to optimize performance.
	•	Generality: Management of HV-B, HV-A, and LV stations with a single program.
	•	Memory Management: Dynamic allocation and deallocation to minimize memory footprint.
	•	Data Validation: Checking input consistency and returning explicit errors in case of issues.

## Installation 
	Clone the repository from GitHub:
git clone lien
	Navigate to the project directory:
cd C-Wire
	Compile the project using the provided Makefile:
make -C codeC
## Usage


## File structure
Project_C-Wire/
├── c-wire.sh               # Main Shell script
├── input/                  # Directory containing the input CSV file
│   └── data.csv
├── codeC/                  # Directory containing the C program
│   ├── main.c              # Main program
│   ├── avl.c               # AVL management
│   ├── avl.h               # Header for AVL
│   ├── utils.c             # Utility functions
│   ├── utils.h             # Utility functions header
│   ├── Makefile            # Makefile for compilation
├── tmp/                    # Temporary directory for intermediate files
├── tests/                  # Test results and files
└── graphs/                 # Generated graphs (if bonus implemented)






 ## Authors
 El dana Aida
Nguyen Thuy Tran
Sanchez Elsa






 
