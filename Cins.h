//
//  cIns.h
//  Matrize
//
//  Created by Florian von Bertrab on 14.05.15.
//  Copyright (c) 2015 Florian von Bertrab. All rights reserved.
//

#ifndef __Matrize__cIns__
#define __Matrize__cIns__

#include <stdio.h>
#include <iostream>

template <typename Type>

// Lets you type in chars, when ints are requested without causing infinite loops

void numCin(Type & fill) {
    using namespace std;
    while (true) {
        if (cin >> fill) {
            break;
        }
        cout << "ERROR! Try again: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

#endif /* defined(__Matrize__cIns__) */
