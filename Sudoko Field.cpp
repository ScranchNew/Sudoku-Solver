//
//  Sudoko Field.cpp
//  Sudoku Solver
//
//  Created by Florian von Bertrab on 16.07.15.
//  Copyright (c) 2015 Florian von Bertrab. All rights reserved.
//
#include <fstream>
#include <iostream>

#include "Sudoko Field.h"
#include "Cins.h"

SudokuField::SudokuField()	// if no file is given, the sudoku field has to be filled in manually
{
    using namespace std;
    cout << "Please type in all known fields in order: ";
    short temp(0);
    for (int y = 0 ; y < 9; y++) {
        for (int x = 0 ; x < 9; x++) {
            numCin(temp);
            temp = temp > 9 ? 9 : temp;
            if (temp < 1) {
                field[x][y] = 0b111111111;
            } else {
                field[x][y] = 0b1 << (temp-1);
            }
        }
    }
    print();
}

SudokuField::SudokuField(std::string filename)	// fills in the sudoku field from a file.
{
    fileIn(filename);
}

short SudokuField::operator()(int x, int y)		// returns the value of the sudoku field at position (x,y)
{
    short nOut = 0;
    for (int i = 0; i < 9; i++) {
        if (field[x][y] & 0b1 << (i)) {
            if (!nOut) {
                nOut = i+1;
            } else if (nOut > 0) {
                nOut = -2;
            } else {
                nOut--;
            }
        }
    }
    return nOut;
}

void SudokuField::print()						// prints the current state in a readable manner to console
{
    using namespace std;
    cout << endl;
    for (int y = 0; y < 9; y++) {
        cout << " ";
        for (int x = 0; x < 9; x++) {
            if (operator()(x,y) > 0) {
                cout << operator()(x,y);
            } else {
                cout << " ";
            }
            if ((x+1)%3 || x==8) {
                cout << " ";
            } else {
                cout << " | ";
            }
        }
        if (!((y+1)%3  || y == 8)) {
            cout << endl << " ------+-------+------";
        }
        cout << endl;
    }
    cout << endl;
}

void SudokuField::fileOut(std::string filename, bool readable)
{
    using namespace std;
    ofstream outf(filename);
    
    if (!outf) {
        cerr << "Could not open " + filename + " for writing!" << endl;
        return;
    }
    outf << (readable ? "r" : "d") << endl;
    if (!readable) {
        outf << hex;
    }
    for (int y = 0; y < 9; y++) {
        outf << " ";
        for (int x = 0; x < 9; x++) {
            if (readable) {
                outf << (operator()(x,y) > 0 ? operator()(x,y) : 0) << " ";
            } else {
                outf << field[x][y] << "\t";
            }
            if (x%3 == 2) {
                outf << "  ";
            }
        }
        outf << endl;
        if (y%3 == 2) {
            outf << endl;
        }
    }
    cout << "Writing to file: " + filename << endl;
}

void SudokuField::fileIn(std::string filename)
{
    using namespace std;
    if (filename == "NULL") {
        print();
        return;
    }
    ifstream inf(filename);
    
    if (!inf) {
        cerr << "Could not open " + filename + " for reading!" << endl;
        return;
    }
    char read;
    inf >> read;
    bool readable = false;
    if (read == 'r') {
        readable = true;
    }
    short temp;
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            inf >> hex >> temp;
            if (readable) {
                temp = temp > 9 ? 9 : temp;
                if (temp < 1) {
                    field[x][y] = 0b111111111;
                } else {
                    field[x][y] = 0b1 << (temp-1);
                }
            } else {
                temp = ((temp > 511 ? 511 : temp) < 0 ? 0 : temp);
                field[x][y] = temp;
            }
        }
    }
    cout << "Reading from file: " + filename << endl;
    sourceFile = filename;
    print();
}

bool SudokuField::solveStep()
{
    bool keepSolving = false;
    // Ausschluss-Verfahren
    for (int row = 0; row < 9; row++) {		// for each row
        short mask = 0b0;
        for (int x = 0; x < 9; x++) {       // lists all already filled Numbers
            mask += operator()(x,row) > 0 ? field[x][row] : 0;
        }
        for (int x = 0; x < 9; x++) {       // deletes those possibilities from the other fields
            if (operator()(x,row) < 1) {
                short t = field[x][row];
                field[x][row] = field[x][row] ^ (field[x][row] & mask);
                if (field[x][row] != t) {
                    keepSolving = true;
                }
            }
        }
    }
    for (int column = 0; column < 9; column++) {	// for each column
        short mask = 0b0;
        for (int y = 0; y < 9; y++) {       // lists all already filled Numbers
            mask += operator()(column,y) > 0 ? field[column][y] : 0;
        }
        for (int y = 0; y < 9; y++) {       // deletes those possibilities from the other fields
            if (operator()(column,y) < 1) {
                short t = field[column][y];
                field[column][y] = field[column][y] ^ (field[column][y] & mask);
                if (field[column][y] != t) {
                    keepSolving = true;
                }
            }
        }
    }
    for (int box = 0; box < 9; box ++) {		// for each 3x3 box
        short mask = 0b0;
        for (int y = 3.0*(box/3); y < 3.0*(box/3)+3; y++) {       // lists already all filled Numbers
            for (int x = 3*(box%3); x < 3*(box%3)+3; x++) {
                mask += operator()(x,y) > 0 ? field[x][y] : 0;
            }
        }
        for (int y = 3.0*(box/3); y < 3.0*(box/3)+3; y++) {       // deletes those possibilities from the other fields
            for (int x = 3*(box%3); x < 3*(box%3)+3; x++) {
                if (operator()(x,y) < 1) {
                    short t = field[x][y];
                    field[x][y] = field[x][y] ^ (field[x][y] & mask);
                    if (field[x][y] != t) {
                        keepSolving = true;
                    }
                }
            }
        }
    }
    
    // Einschluss-Verfahren
    for (int row = 0; row < 9; row++) {				// for each row
        for (int x = 0; x < 9; x++) {
            short temp = field[x][row];             // checks if field is the only one with one possible number
            for (int x2 = 0; x2 < 8; x2++) {
                int x_ = x2 < x ? x2 : x2+1;
                temp = temp ^ (temp & field[x_][row]);
            }
            if (temp && temp != field[x][row]) {    // if true: writes that number into the field
                field[x][row] = temp;
                keepSolving = true;
            }
        }
    }
    for (int column = 0; column < 9; column++) {	// for each column
        for (int y = 0; y < 9; y++) {
            short temp = field[column][y];          // checks if field is the only one with one possible number
            for (int y2 = 0; y2 < 8; y2++) {
                int y_ = y2 < y ? y2 : y2+1;
                temp = temp ^ (temp & field[column][y_]);
            }
            if (temp && temp != field[column][y]) { // if true: writes that number into the field
                field[column][y] = temp;
                keepSolving = true;
            }
        }
    }
    for (int box = 0; box < 9; box ++) {			// for each 3x3 box
        for (int place = 0; place < 9; place++) {
            int x = 3*(box%3) + place%3;
            int y = 3.0*(box/3) + place/3;
            short temp = field[x][y];               // checks if field is the only one with one possible number
            for (int place2 = 0; place < 8; place++) {
                int place_ = place2 < place ? place2 : place2+1;
                int x2 = 3*(box%3) + place_%3;
                int y2 = 3.0*(box/3) + place_/3;
                temp = temp ^ (temp & field[x2][y2]);
            }
            if (temp && temp != field[x][y]) {      // if true: writes that number into the field
                field[x][y] = temp;
                keepSolving = true;
            }
        }
    }
    return keepSolving;	// returns false if there was no change during this solution step
}

bool SudokuField::checkField()
{
    using namespace std;
    for (int y = 0; y < 9; y++) {		// checks rows for numbers appearing more than once
        short temp = 0b0;
        for (int x = 0; x < 9; x++) {
            if (field[x][y] == 0) {
                cout << "Field " << x+1 << "/" << y+1 << "not solvable!" << endl;
                return false;
            }
            temp = temp | field[x][y];
        }
        if (temp != 0b111111111) {
            cout << "Error in row " << y+1 << endl;
            return false;
        }
    }
    for (int x = 0; x < 9; x++) {		// checks columns for numbers appearing more than once
        short temp = 0b0;
        for (int y = 0; y < 9; y++) {
            temp = temp | field[x][y];
        }
        if (temp != 0b111111111) {
            cout << "Error in column " << x+1 << endl;
            return false;
        }
    }
    for (int box = 0; box < 9; box++) {	// checks 3x3 boxes for numbers appearing more than once
        short temp = 0b0;
        for (int square = 0; square < 9; square++) {
            short x = 3*(box%3) + square%3;
            short y = 3.0*(box/3) + square/3;
            temp = temp | field[x][y];
        }
        if (temp != 0b111111111) {
            cout << "Error in box " << box+1 << endl;
            return false;
        }
    }
    return true;
}

bool SudokuField::solve()
{
    using namespace std;
    while (solveStep());        	// normal procedure until it doesn't work anymore
    if (!checkField()) {        	// check for double numbers etc
        return false;
    }
    int xTest = 0;                 // Coordinates of field to try out;
    int yTest = 0;
	
    int mostDet = -10;          	// field with least possible Options
    for (int y = 0; y < 9; y++) {       // finding this field
        for (int x = 0; x < 9; x++) {
            int num = operator()(x,y);
            if (num < 0 && num > mostDet) {
                xTest = x;
                yTest = y;
                mostDet = num;
            }
            if (mostDet == -2) {break;}
        }
        if (mostDet == -2) {break;}
    }
    if (mostDet > -10) {        	// if there are not fully determined fields the algorythm tries all possible options in order
        short * choices = new short[-mostDet];
        int chCount = 0;
        for (int i = 0; i < 9; i++) {
            if (field[xTest][yTest] & 0b1 << i) {
                choices[chCount++] = 0b1 << i;  // lists options in choices[]
            }
        }
        for (int i = 0; i < chCount; i++) {
            short buffer[9][9];
            for (int y = 0; y < 9; y++) {       // buffers field if choice leads to garbage
                for (int x = 0; x < 9; x++) {
                    buffer[x][y] = field[x][y];
                }
            }
            field[xTest][yTest] = choices[i];   // replaces field
            
            if (solve()) {                      // tries to solve resulting field
                return true;
            }
            for (int y = 0; y < 9; y++) {       // if this doesn't work it restores the old one
                for (int x = 0; x < 9; x++) {
                    field[x][y] = buffer[x][y];
                }
            }
        }
        return false;
    }
    return true;
}