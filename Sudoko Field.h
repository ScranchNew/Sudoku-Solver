//
//  Sudoko Field.h
//  Sudoku Solver
//
//  Created by Florian von Bertrab on 16.07.15.
//  Copyright (c) 2015 Florian von Bertrab. All rights reserved.
//

#ifndef __Sudoku_Solver__Sudoko_Field__
#define __Sudoku_Solver__Sudoko_Field__

#include <stdio.h>
#include <string.h>

class SudokuField{
private:
    short       field[9][9] = {0};		// one 16 bit number for each sudoku field. 9 bits are used: 0 1100 0001 for example would mean,
										// that the numbers 8,7 and 1 are possible in this position
										
    std::string sourceFile  = "";		// name of the file, the field is loaded from
public:
    SudokuField();						// constructor
    SudokuField(std::string filename);
	
    short operator()(int x, int y);		// overloading the ()-operator for easy access to the sudoku fields
    void print();						// prints the sudoku field in the current state to console
    void fileOut(std::string filename, bool readable = false);	// writes the current sudoku field into a file, either with full information
																// or with reduced information but human readable
    void fileIn(std::string filename);	// loads the sudoku field from a file
    bool solve();						// tries to solve the sudoku using logical conclusions and brute force, if not possible
    bool checkField();					// checks if the current sudoku field follows the given sudoku rules
    bool solveStep();					// performs on solution step, returns false if nothing changed during this step
};

#endif /* defined(__Sudoku_Solver__Sudoko_Field__) */
