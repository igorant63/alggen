## README

## Algorithm Generator (alggen) 1.03

Algorithm Generator is designed for evolutionary modeling of synthesis of program code and algorithms for solving applied problems.
The program is implemented in C++ using MFC library as a Windows application based on a dialog box. 

## Build project 

Open the solution file algen.sln in Visual Studio 2022 and build the project.
Specific tasks are implemented as dynamic DLLs whose files should be placed in the alggen.exe application file folder.
The task of sorting an array of numbers is implemented in the dll.sln solution. After building, the dll.dll file should be copied to the alggen.exe file folder.

## Application Control

The application window displays the menu, program text output panel, control buttons and status bar. 

Menu commands
Population section
"Create" generates the initial program population.
"Save" saves the current population in a file with a .pop extension.
"Load" loads the saved population from a .pop file.
"Merge" merges the current population with the population loaded from the file.
"Sort" arranges the programs according to the optimization settings.
"Selection" removes programs with a quality index lower than the specified one from the population.
"Cut" deletes programs with a sequence number higher than the specified one from the population.
"Reset" clears the program list.
Program section 
"New" adds a new empty program.
"Remove" removes the currently displayed program.
"Add Command" adds a command to the end of the program
"Remove Command" removes the last command in the program.
Settings section
"Select test" displays a panel for selecting a DLL module for a particular task.
"Save settings" saves the settings to a file with the specified name.
"Load settings" loads settings from the selected file.
"Command probabilities" displays the probabilities settings panel for commands groups.
"Mutation probabilities" displays the probabilities settings panel for mutations.
"Population size" displays the settings panel for the population size and length of automatically generated programs.
"Optimization" displays the settings panel for optimization options in the process of generation and selection of programs.

Main window button commands
"List" displays the list of programs with their parameters.
"Prev"/"Next" moves to the previous/next program in the list.
"Start/Stop" starts the cyclic process of mutations and selection.
"Step" performs mutation and selection for one generation of programs.
"View CPU" displays the contents of the first 16 RAM cells and CPU registers before and after execution of the selected program.
"Sample" adds to the list a sample program that implements the task of sorting an array of numbers in memory.
"Test" checks and displays in the status bar the status of the selected program (successful or interrupted by error), its quality index (QUALITY), as well as the quality index of the source array (ORIG.QUAL) and the maximum achievable quality index (MAX.QUAL).
"Test All" checks the operability of all programs in the population and displays in the status bar statistics on the number of formally correct programs and programs containing looping and memory access errors.
"Exit" terminates the application.

## Using the application

After startup, you can select the desired task solving module ("Settings"-"Select test") if additional tasks are installed. Then in the menu section "Settings" you should set the program code generation parameters, mutation parameters and optimization options. 
The sort.set file contains settings that ensure successful synthesis of the sorting algorithm. In order for it to be available through the settings load in the menu, it should be copied to the folder with the executable file alggen.exe.
After setting, the "Start" command starts the evolution process, which can be stopped by the "Stop" command when the required quality indicator is reached. The programs of the final population, their indicators and execution results can be viewed and the current population can be saved to disk. While the evolutionary simulation is running, the status bar displays the time of processing one generation of programs, the total number of mutations and the maximum quality index achieved. 
After stopping the simulation in the program view mode after pressing the "Test" button, the status bar displays the status of the selected program (correct or interrupted by error), its quality index (QUALITY), as well as the quality index of the source array (ORIG.QUAL) and the maximum achievable quality index (MAX.QUAL). 
More detailed information about the parameters of the population programs can be obtained using the "List" command. This command displays the list of programs of the current population with the following characteristics:
Signature - the unique identifier of the automatically generated program of which this program is a descendant.
Age - number of the generation in which this program was generated.
Status - GOOD, if the program completes without looping or memory access errors.
Quality - quality indicator of the program (fitness function value).
Delta Q - increment of the quality of this program in relation to the quality indicator of the initial data array.
Length - program length in operators.
Takts - number of operations before program completion.

## Comments on the application code

The model of virtual computing environment is realized as a set of interacting C++ objects. 
Model objects 
Processor (memory + registers) - struct proc.
Machine command - struct command.
Program - struct program. 
Parameter tables - struct maps.


One global variable processor is created in the program and used for modeling the program execution. The current implementation of the processor uses 256 4-byte RAM cells and 5 general-purpose registers.
The instruction contains two operands used in calculations - op_1, op_2 , and a label. 
With the print method the command forms its listing, with the run method it performs its operation in the processor passed to it as an argument.
A set of commands realizing a specific command system is created as a set of derived classes from the command class - jle_r_r, add_r_c, and so on.


The program includes a specific set of commands - cmds, a command counter - cp, a reference to the processor on which it is executed - prc.
The gen_prog method generates a random set of commands representing a new program.
The gen_sample method generates a template example of a working program.
The run method executes the program.
The listing method outputs the program text to the output device.
Commands are organized into groups, the proportions of the probabilities of falling out are configured in the dialog panel. 
Within a group, each command is equally likely to fall out. You can combine all commands into one group and they become equal probabilities. You can make each command a separate group and customize its probability of falling out individually.
In the settings table, map is an array of one hundred cells containing the number of cells with the index of each command group proportional to the probability of each command group. Randomly selecting one of one hundred of these elements provides the desired probability of a group falling out in a sequence of trials.
The init_map function fills the map array.
The def_prob function sets the probabilities for each command group.
The make_command function generates a separate command for a particular program being formed, taking into account the settings of group probabilities and taking into account the equal probability of commands within the group.
The make_array function initializes the processor before executing the program.

Transition to the new model of processor organization requires reorganization of the proc class, transition to the new command system requires reorganization of the set of classes derived from command, as well as reorganization of the make_command function and coordinated adjustment of the following arrays and variables:
int prob[]; // Distribution of probabilities by types of customizable commands
int n_comm; // Number of customizable command groups
int n_comm_tot; // Total number of different types of commands
int group[]; // Number of commands in each customizable group of commands
Modules of the task solution are loaded from dynamic libraries (*.dll) placed in the application folder. The set of modules can be extended independently. The source code of the array sorting module is available as a DLL project.

The DLL of the task solving module should export the following functions:

int check_alg(base_check *); 
The function gets a pointer to the base_check structure, whose functions get_mem() and get_reg() return pointers to memory and CPU registers. The function must initialize the memory and processor registers, call the run() function of the base_check structure to execute the program, and then compute and return the value of the fitness function. 
int get_max_qual();
The function returns the maximum possible fitness function value for the current task.
int get_base_qual();
The function returns the base value of the fitness function for the initial memory state before the program execution.
int* get_reg_one();
The function returns a pointer to an array containing the initial state of the processor registers.
int * get_mem_one();
The function returns a pointer to the array containing the initial state of the processor's RAM.
void init_array();
The function updates the initial data arrays to solve the task.
char* about();
The function returns a pointer to the text describing this module.

## License

The code for the project is licensed under the MIT license.
* Copyright (C) Igor Antonov igor_a_2000@mail.ru


