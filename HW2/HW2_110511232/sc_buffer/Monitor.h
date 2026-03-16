#ifndef MONITOR_H
#define MONITOR_H
#include <iostream>
#include <vector>
#include <fstream>
#include "systemc.h"
#include <iomanip>

using namespace std;

SC_MODULE(Monitor)
{
    sc_port<sc_signal_in_if<double>> *prob;
    sc_in_clk iclk;
    vector<string> classes;
    int cycle_cnt = 0;

    void run()
    {
        // pick the top 5 classes with the highest probabilities
        if (cycle_cnt >= 20)
        {
            cout << "Cycle " << cycle_cnt << endl;

            vector<double> prob_vec;
            for (int i = 0; i < 1000; i++)
            {
                prob_vec.push_back(prob[i]->read());
            }

            // sort the probabilities in descending order
            vector<double> prob_vec_sorted = prob_vec;
            sort(prob_vec_sorted.begin(), prob_vec_sorted.end(), greater<double>());

            // print the top 5 classes with the highest probabilities
            for (int i = 0; i < 5; i++)
            {
                int index = find(prob_vec.begin(), prob_vec.end(), prob_vec_sorted[i]) - prob_vec.begin();
                cout << "Class " << i + 1 << ": " << left << setw(30) << classes[index];
                cout << " val = " << left << setw(10) << prob_vec_sorted[i] << endl;
            }

            sc_stop();
        }

        cycle_cnt++;
    }

    SC_CTOR(Monitor)
    {
        prob = new sc_port<sc_signal_in_if<double>>[1000];

        ifstream in_file("../../data/imagenet_classes.txt");
        // parse the class names
        while (!in_file.eof())
        {
            string class_name;
            getline(in_file, class_name);
            classes.push_back(class_name);
        }
        in_file.close();

        SC_METHOD(run);
        sensitive << iclk.pos();
    }

    ~Monitor()
    {
        delete[] prob;
    }
};

#endif