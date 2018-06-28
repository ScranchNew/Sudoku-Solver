//
//  main.cpp
//  Sudoku Solver
//
//  Created by Florian von Bertrab on 16.07.15.
//  Copyright (c) 2015 Florian von Bertrab. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>

#include "Sudoko Field.h"

using namespace std;

bool solveSudoku(string filename){
    ifstream inf(filename);
    if (!inf) {
        cerr << "Could not open " + filename + " for reading!" << endl;
        return false;
    } else {
        bool solved;
        SudokuField test(filename);
        solved = test.solve();
        test.print();
        return solved;
    }
}

int main(int argc, const char * argv[]) {
    
    SudokuField test;
	test.solve();
	test.print();
	
    return 0;
}