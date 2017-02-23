#include <cstdlib>
#include <iostream>
#include <fstream>
#include "lipTracking.hpp"
#include "eye_detect.hpp"
//#include "initializeNN.hpp"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) 
{
    //initializeNN();
    //cout << "Creazione rete neurale completata.\n";
    //cout << "Premere un tasto per iniziare il tracking...";
    //getchar();
    //cout << "OK\n";
    lipTracking();
    return 0;
}

