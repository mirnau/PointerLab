//Inkludera f�rst i alla cpp filer 
#pragma once

//Memory leaks
#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#ifndef DBG_NEW
#include <stdlib.h>
#include <crtdbg.h>
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#define LEVEL 4
//#define G_BETYG
//#define VG_BETYG

//B�rja med att ha G_BETYG och VG_BETYG undefined och s�tt LEVEL till 1.
// Stega sedan upp LEVEL till 8
// Definiera G_BETYG, d� k�rs �ven TestSharedG()
// Om du satsar p� G s� �r det klart nu annars
// Stega sedan upp LEVEL till ungef�r 15
// s�tt LEVEL till 99
// Definiera VG_BETYG, nu d� k�rs alla tester.
// 
// 
//Observera att LEVEL testerna �r mycket enkla
//det �r f�rst G_BETYG resp. VG_BETYG som blir en ordentlig test.

/* Niv�erna man kan s�tta p� LEVEL
#if false
level 1
SharedPtr();
SharedPtr(T*);
get()
used_count()
Invariant();

level 2
SharedPtr(nullptr);
SharedPtr(const SharedPtr&);


level 3
operator*
operator->
operator bool


level 4
    SharedPtr(const SharedPtr&&)
    operator=(const SharedPtr&&)
    reset
Level 5
==, <  allihop

level 6
swap

level 7
//This is not part of the requirements
//MakeShared


//Nu b�rjar VG
level 11
WeakPtr();
~WeakPtr();
expired();
Invariant();
    
level 12
WeakPtr(const SharedPtr&);
Lock()

level 13
WeakPtr(const WeakPtr&);
operator=(const WeakPtr&);
operator=(const SharedPtr&);

level 14
    reset
    swap

#endif


*/