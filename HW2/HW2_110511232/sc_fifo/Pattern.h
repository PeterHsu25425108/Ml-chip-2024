#ifndef PATTERN_H
#define PATTERN_H
#include <iostream>
#include <vector>
#include <fstream>
#include "systemc.h"
#include "DEBUG.h"

SC_MODULE(Pattern)
{
    string infile_name;
    vector<vector<vector<double>>> in_data;
    sc_port<sc_fifo_out_if<double>> ***out_data;
    sc_in_clk iclk;

    void run()
    {
        while (true)
        {
            if (DEBUG)
                cout << "run Pattern" << endl;
            // write input data to out_data
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 224; j++)
                {
                    for (int k = 0; k < 224; k++)
                    {
                        out_data[i][j][k]->write(in_data[i][j][k]);
                    }
                }
            }

            if (DEBUG)
                cout << "write Pattern" << endl;
            wait();
        }
    }

    SC_CTOR(Pattern)
    {
        out_data = NULL;
    }

    Pattern(sc_module_name name, string infile_name) : infile_name(infile_name)
    {
        in_data = vector<vector<vector<double>>>(3, vector<vector<double>>(224, vector<double>(224, 0)));
        // read input data from file
        ifstream in_file(infile_name.c_str());
        string temp, str;

        double val;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 224; j++)
            {
                for (int k = 0; k < 224; k++)
                {
                    in_file >> str;
                    stringstream ss(str);
                    getline(ss, temp, 'e');
                    val = stod(temp);
                    getline(ss, temp);
                    val *= pow(10, stod(temp));

                    in_data[i][j][k] = val;
                }
            }
        }

        in_file.close();

        out_data = new sc_port<sc_fifo_out_if<double>> **[3];
        for (int i = 0; i < 3; i++)
        {
            out_data[i] = new sc_port<sc_fifo_out_if<double>> *[224];
            for (int j = 0; j < 224; j++)
            {
                out_data[i][j] = new sc_port<sc_fifo_out_if<double>>[224];
            }
        }
        SC_THREAD(run);
        sensitive << iclk.pos();
        dont_initialize();
    }

    ~Pattern()
    {
    }
};

#endif