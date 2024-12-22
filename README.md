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
- Script Shell Mode
	•	Data Filtering: Selection of relevant information from the input CSV file.
	•	Option Validation: Verification of passed arguments (stations, consumers, etc.).
	•	Output File Creation: Generation of CSV files containing the results of the analysis.

- C Program Mode
	•	Data Processing: Calculation of energy consumption using an AVL to optimize performance.
	•	Generality: Management of HV-B, HV-A, and LV stations with a single program.
	•	Memory Management: Dynamic allocation and deallocation to minimize memory footprint.
	•	Data Validation: Checking input consistency and returning explicit errors in case of issues.

## Installation 
	- Clone the repository from GitHub:
- git clone https://github.com/thfennesty
	- Navigate to the project directory:
- cd C-Wire
	- Compile the project using the provided Makefile:
- make -C codeC


## Usage
- Running the Application

- To run the application, use the following command:

- ./c-wire.sh <input_file_path> <station_type> <consumer_type> [<plant_id>]

- Shell Script Usage

1. Filter and Process Data
	•	Specify the input CSV file path, station type, and consumer type as required arguments.
	•	Optionally, add a power plant ID to filter results for a specific plant.

2. Validate Input Options
	•	The script checks the validity of provided arguments. If invalid, an error message is displayed, and the help menu is shown.

3. Compile the C Program Automatically
	•	The script ensures the main executable is present. If not, it compiles the C program using the Makefile before proceeding.

4. Output Results
	•	Generates CSV files summarizing station capacities and energy consumption based on the specified parameters.

Program C Usage

1. Data Processing
	•	The program reads filtered data and calculates energy consumption using an AVL tree.
	•	Handles all three station types (hvb, hva, lv) with optimized memory management.

2. Error Handling
	•	Ensures data integrity and returns error codes for invalid inputs or system errors.

3. Output Summary
	•	Provides structured results, including total capacity and consumption for analyzed stations.

## File structure
- Projet_C-Wire/
- ├── c-wire.sh          # Main Shell script to run the project
- ├── Makefile           # Makefile to compile the C program
- ├── codeC/             # Directory containing source files and the executable for the C program
- │   ├── fonction.c     # Source file with function implementations
- │   ├── headers.h      # Header file with function and structure declarations
- │   ├── main.c         # Main source file of the program
- │   ├── read.c         # Source file for input data handling
- │   └── main           # Compiled executable for normal use
- └── README.md          # Documentation explaining installation and usage


 ## Authors
- El dana Aida
- Nguyen Thuy Tran
- Sanchez Elsa






 
