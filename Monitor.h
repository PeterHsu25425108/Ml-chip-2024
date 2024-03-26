#ifndef MONITOR_H
#define MONITOR_H
#include <iostream>
#include <vector>
#include <fstream>
#include "systemc.h"

SC_MODULE(Monitor)
{
    sc_in<double> *prob;

    void run()
    {
        ifstream in_file("data/imagenet_classes.txt");
        vector<string> classes;
        // parse the class names
        while (!in_file.eof())
        {
            string class_name;
            getline(in_file, class_name);
            classes.push_back(class_name);
        }
        in_file.close();

        // pick the top 5 classes with the highest probabilities
        vector<double> prob_vec;
        for (int i = 0; i < 1000; i++)
        {
            prob_vec.push_back(prob[i].read());
        }

        vector<double> prob_vec_sorted = prob_vec;
        sort(prob_vec_sorted.begin(), prob_vec_sorted.end(), greater<double>());

        for (int i = 0; i < 5; i++)
        {
            int index = find(prob_vec.begin(), prob_vec.end(), prob_vec_sorted[i]) - prob_vec.begin();
            cout << "Class " << i + 1 << ": " << classes[index] << " with probability " << prob_vec_sorted[i] << endl;
        }
    }

    SC_CTOR(Monitor)
    {
        cout << "Monitor is constructing" << endl;
        prob = new sc_in<double>[1000];
        SC_METHOD(run);
        for (int i = 0; i < 1000; i++)
        {
            sensitive << prob[i];
        }
        cout << "Monitor is constructed" << endl;
    }

    ~Monitor()
    {
        delete[] prob;
    }
};

#endif