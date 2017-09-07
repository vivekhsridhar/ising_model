//
//  main.cpp
//  ising_model
//
//  Created by Vivek Sridhar on 07/09/17.
//  Copyright © 2017 Vivek Sridhar. All rights reserved.
//

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "parameteres.h"

using namespace rnd;
using namespace cv;

//**************************************************************************************************
//**	MAIN	************************************************************************************
//**************************************************************************************************

int main()
{
    // Random generator engine from a time-based seed
    unsigned seed = static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    rnd::set_seed(seed);
    
    // Set parameters
    grid_size = 200;
    temperature = 2.0;
    system_energy = 0.0;
    spin_energy = 0.0;
    system_magnetisation = 0;
    spin_magnetisation = 0;
    beta = 1 / (boltzmann_constant * temperature);
    
    agent = new spin[grid_size*grid_size];
    
    int num_timesteps = 100000;
    
    SetupSimulation();
    for (int i = 0; i != num_timesteps; ++i)
    {
        FlipSpins();
        if (i % 100 == 0) Graphics();
        
        ++timestep_number;
    }
    
    return 0;
}

//**************************************************************************************************
//**    OTHER GROUP LEVEL FUNCTIONS ****************************************************************
//**************************************************************************************************

void FlipSpins()
{
    for (int i = 0; i != grid_size*grid_size; ++i)
    {
        int id = rnd::integer(grid_size*grid_size);
        CalculateSystemProperties(id);
        double before = spin_energy;
        agent[id].state *= -1;
        CalculateSystemProperties(id);
        double after = spin_energy;
        
        double p_accept = 0.0;
        if (before < after)
        {
            p_accept = exp(-(after - before) * beta);
        }
        else p_accept = 1.0;
        
        if (rnd::uniform() >= p_accept) agent[id].state *= -1;
    }
}

void CalculateSystemProperties(int spin_id)
{
    if (spin_id == -1)
    {
        system_energy = 0.0;
        system_magnetisation = 0;
        for (int i = 0; i != grid_size*grid_size; ++i) CalculateSpinProperties(system_energy, system_magnetisation, i);
    }
    else
    {
        spin_energy = 0.0;
        spin_magnetisation = 0;
        CalculateSpinProperties(spin_energy, spin_magnetisation, spin_id);
    }
}

void CalculateSpinProperties(double& energy, int& magnetisation, int spin_id)
{
    int x = spin_id / grid_size;
    int y = spin_id % grid_size;
    
    CVec2D l, r, u, d;
    
    l = CVec2D(x - 1, y);
    r = CVec2D(x + 1, y);
    u = CVec2D(x, y - 1);
    d = CVec2D(x, y + 1);
    if (l.x < 0 || r.x >= grid_size || u.y < 0 || d.y >= grid_size)
    {
        if (l.x < 0) l.x += grid_size;
        if (r.x >= grid_size) r.x -= grid_size;
        if (u.y < 0) u.y += grid_size;
        if (u.y >= grid_size) u.y -= grid_size;
    }
    
    int lid = l.x * grid_size + l.y;
    int rid = r.x * grid_size + r.y;
    int uid = u.x * grid_size + u.y;
    int did = d.x * grid_size + d.y;
    
    energy -= agent[spin_id].state * agent[lid].state;
    energy -= agent[spin_id].state * agent[rid].state;
    energy -= agent[spin_id].state * agent[uid].state;
    energy -= agent[spin_id].state * agent[did].state;
    
    magnetisation += agent[spin_id].state;
}

void SetupSimulation()
{
    timestep_number = 0;
    SetupSpins();
}

void SetupSpins()
{
    CVec2D set_position;
    int set_state;
    
    for(int i = 0; i != grid_size*grid_size; ++i)
    {
        int x = i / grid_size;
        int y = i % grid_size;
        set_position = CVec2D(x, y);
        
        if (rnd::uniform() < 0.5) set_state = -1;
        else set_state = 1;
        
        agent[i].Setup(set_position, set_state);
    }
}

//**************************************************************************************************
//**	GRAPHICS   *********************************************************************************
//**************************************************************************************************

void Graphics()
{
    // Colours vector
    Scalar colours[2] = {Scalar(240, 255, 255), Scalar(82, 56, 6)};
    
    // Draw area
    Mat visualisation = Mat::zeros(grid_size, grid_size, CV_8UC3);
    
    // Draw spins
    for (int i = 0; i != grid_size*grid_size; ++i)
    {
        int colour = -1;
        if (agent[i].state == -1) colour = 0;
        else colour = 1;
        circle(visualisation, Point(agent[i].position.x, agent[i].position.y), 1, colours[colour], -1);
    }
    
    // Display timestep number & cue counter on screen
    putText(visualisation, std::to_string(timestep_number), cvPoint(10,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
    
    imshow("ising_model", visualisation);
    waitKey(1);
}
