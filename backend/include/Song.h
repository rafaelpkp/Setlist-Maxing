#pragma once

#include<string>
using namespace std;

struct Song {
    string id;
    string title;
    string artists;
    
    int durationMs = 0;
    double bpm = 0.0;
    int key = -1;
    int mode = -1;
    double energy = -1.0;
};