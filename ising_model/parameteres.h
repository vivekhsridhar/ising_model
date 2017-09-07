//
//  parameteres.h
//  ising_model
//
//  Created by Vivek Hari Sridhar on 07/09/17.
//  Copyright © 2017 Vivek Hari Sridhar. All rights reserved.
//

#ifndef parameteres_h
#define parameteres_h

#include "spin.h"
#include <fstream>

const double boltzmann_constant = 1.0;

int     timestep_number;    // timestep number
int     grid_size;
int     system_magnetisation;
int     spin_magnetisation;

double  system_energy;
double  spin_energy;
double  temperature;
double  beta;

spin* agent;

int main();
void FlipSpins();
void CalculateSystemProperties(int spin_id);
void CalculateSpinProperties(double& energy, int& magnetisation, int spin_id);
void SetupSimulation();
void SetupSpins();

void Graphics();

#endif /* parameteres_h */
