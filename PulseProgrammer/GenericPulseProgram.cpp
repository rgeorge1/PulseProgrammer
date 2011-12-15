//
//  GenericPulseProgram.cpp
//  PulseProgrammer
//
//  Created by Richard George on 09/12/2011.
//  Copyright 2011 University of Oxford. All rights reserved.
//

#include <iostream>

#include "AWG.h"
#include "PlaylistEntry.h"
#include "GenericPulseProgram.h"
#include "Sequence.h"

extern bool verbose;

GenericPulseProgram::GenericPulseProgram()
{
    current_point = 0;
    current_phase_cycle_step = 0;
    max_points = 0;
    max_phase_cycle_steps = 0;
}

int GenericPulseProgram::getPointCount() const
{
    return max_points+1;
}

int GenericPulseProgram::getPhaseCycleStepCount() const
{
    return max_phase_cycle_steps;
}

void GenericPulseProgram::setPointNumber(int n)
{
    current_point = n;
    // Update the maximum
    max_points = ( n > max_points) ? n : max_points;
}

void GenericPulseProgram::setPhaseCycleStep(int n)
{
    current_phase_cycle_step = n;
    // Update the maximum
    max_phase_cycle_steps = ( n > max_phase_cycle_steps ) ? n : max_phase_cycle_steps;
}

void GenericPulseProgram::addPulse(const char *envelope, double start_time, double duration, double fq, double amp, double ph)
{
    PulseData *pd = new PulseData(envelope,start_time,duration,fq,amp,ph);
    pulses[std::pair<int,int>(current_point,current_phase_cycle_step)].push_back(pd);
}

void GenericPulseProgram::getPulses(int p,int s,std::vector<PulseData *> &ps)
{
    if (verbose) std::cout << "Retrieving pulse point #" << p << " phase cycle step #" << s << std::endl;
    
    int n_pulses = (int) pulses[std::pair<int,int>(p,s)].size();
    
    if (verbose) std::cout << "This point has " << n_pulses << " pulse(s) defined" << std::endl;
    
    // This looks like a bug! We should be copying here.
    
    ps.clear();
    std::vector<PulseData *> xx(pulses[std::pair<int,int>(p,s)]);
    ps.swap(xx);
    
}

GenericPulseProgram::~GenericPulseProgram()
{
    std::map<std::pair<int,int>,std::vector<PulseData * > >::iterator it;
    
    for (it = pulses.begin();it!=pulses.end();it++)
    {
        for (std::vector<PulseData *>::iterator it2 = it->second.begin();it2 != it->second.end() ; it2++)
        {
            delete *it2;
        }
    }
    pulses.clear();
}

void GenericPulseProgram::generatePlaylist(const AWG &a,int point,int step,std::vector<PlaylistEntry> &list) 
{

   // Get the list of pulses

   std::vector<PulseData *> d;
   getPulses(point,step,d);

   // Calculate a list of times during which we need to synthesise

   Sequence s;

   // Iterate over pulses
   for (std::vector<PulseData *>::iterator it = d.begin();it!=d.end();it++)
   {
      if (verbose) (*it)->describePulse();
    
      // Convert into sample number, from time in nanoseconds
      s.addRisingEdge(a.getSampleBeforeTime((*it)->getFirstSynthesisTime()));
      s.addFallingEdge(a.getSampleAfterTime((*it)->getLastSynthesisTime()));
   }

   // Combine overlapping pulses
   s.flatten();

   if (verbose) std::cout << "Rounded sequence reads:" << std::endl;

   // Coarsen the synthesis times out to ChunkyBlockSize (~ 256 samples)
   Sequence rs(s,a.getChunkyBlockSize());
   
   if (verbose) rs.describeSequence();

   // Now we have a list of sample numbers
   // that represent when we must generate non-zero output from the AWG
    
   std::map<double,int>::const_iterator it = rs.edges.begin();
   std::vector<PulseBuffer *> buffers;

   double start_time = 0.0, stop_time = 0.0;

   if (verbose) std::cout << "Playback list for point " << point << ", cycle step " << step << " consists of:" << std::endl << std::endl;

   // For each period when non-zero output is present
   for (;it!=rs.edges.end();it++)
   {
      if ((*it).second == 1)
      {
         start_time = (*it).first;
         
          if (stop_time<start_time) 
         {
             // We have a pause
             if (verbose) std::cout << " --> (A pause from sample " << stop_time << " to sample " << start_time << ")" << std::endl << std::endl;
             // Store a placeholder for an empty segment
             list.push_back(PlaylistEntry(start_time - stop_time));
         }
          
         continue;
      }
    
      if ((*it).second == -1)
      {
          stop_time = (*it).first;
          if (verbose) std::cout << "Creating a buffer to cover samples " << start_time << " to " << stop_time << std::endl;
        
          // Calculate the output during this interval
          PulseBuffer *pb = new PulseBuffer(start_time,stop_time);
        
          std::vector<PulseData *>::const_iterator it2 = d.begin();
        
          for (;it2!=d.end();it2++)
          {
             if (verbose) std::cout << "Rendering pulse " << (*it2)->this_pulse_number << " to buffer " << pb->this_buffer_number << std::endl;
             (*it2)->renderToBuffer(a,pb);
          }
        
          if (verbose) std::cout << std::endl;
          
          // Make sure amplitude is between +/- 1 Volt
          pb->clipSamples();
          // Calculate a hash for the buffer
          pb->finalise();
          // Print out the checksum
          if (verbose) pb->reportMD5();
          
          buffers.push_back(pb);
          
          list.push_back(PlaylistEntry(pb));
          continue;
      } 
   }
}

