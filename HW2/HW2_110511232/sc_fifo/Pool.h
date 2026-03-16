#ifndef POOL_H
#define POOL_H
#include <iostream>
#include <vector>
#include <fstream>
#include "systemc.h"
#include "DEBUG.h"

SC_MODULE(MaxPool2d)
{
    const int in_ch;
    const int in_wid;
    const int kernel_size;
    const int stride;
    sc_in_clk iclk;
    string name;

    sc_port<sc_fifo_in_if<double>> ***in_data;
    sc_port<sc_fifo_out_if<double>> ***out_data;

    void run()
    {
        while (true)
        {
            if (DEBUG)
                cout << "run " << name << endl;

            int out_wid = in_wid / stride;

            // a 3D vector to store the input data
            vector<vector<vector<double>>> in_array(in_ch, vector<vector<double>>(in_wid, vector<double>(in_wid, 0)));
            for (int i = 0; i < in_ch; i++)
            {
                for (int j = 0; j < in_wid; j++)
                {
                    for (int k = 0; k < in_wid; k++)
                    {
                        double temp;
                        in_data[i][j][k]->read(temp);
                        in_array[i][j][k] = temp;
                    }
                }
            }

            for (int i = 0; i < out_wid; i++)
            {
                for (int j = 0; j < out_wid; j++)
                {
                    for (int k = 0; k < in_ch; k++)
                    {
                        double max_val = 0;
                        for (int l = 0; l < kernel_size; l++)
                        {
                            for (int m = 0; m < kernel_size; m++)
                            {
                                double temp;
                                temp = in_array[k][i * stride + l][j * stride + m];
                                max_val = max(max_val, temp);
                            }
                        }
                        out_data[k][i][j]->write(max_val);
                    }
                }
            }

            if (DEBUG)
                cout << "write " << name << endl;
            wait();
        }
    }

    SC_CTOR(MaxPool2d) : in_ch(0), in_wid(0), kernel_size(0), stride(0)
    {
        in_data = NULL;
        out_data = NULL;
    }

    MaxPool2d(sc_module_name name, int in_ch, int in_wid, int kernel_size, int stride) : in_ch(in_ch), in_wid(in_wid), kernel_size(kernel_size), stride(stride), name(name)
    {
        in_data = new sc_port<sc_fifo_in_if<double>> **[in_ch];
        out_data = new sc_port<sc_fifo_out_if<double>> **[in_ch];

        for (int i = 0; i < in_ch; i++)
        {
            in_data[i] = new sc_port<sc_fifo_in_if<double>> *[in_wid];
            out_data[i] = new sc_port<sc_fifo_out_if<double>> *[in_wid / stride];
            for (int j = 0; j < in_wid; j++)
            {
                in_data[i][j] = new sc_port<sc_fifo_in_if<double>>[in_wid];
            }
            for (int j = 0; j < in_wid / stride; j++)
            {
                out_data[i][j] = new sc_port<sc_fifo_out_if<double>>[in_wid / stride];
            }
        }

        SC_THREAD(run);
        sensitive << iclk.pos();
        dont_initialize();
    }

    ~MaxPool2d()
    {
        for (int i = 0; i < in_ch; i++)
        {
            for (int j = 0; j < in_wid; j++)
            {
                delete[] in_data[i][j];
            }
            delete[] in_data[i];
        }
        delete[] in_data;

        for (int i = 0; i < in_ch; i++)
        {
            for (int j = 0; j < in_wid / stride; j++)
            {
                delete[] out_data[i][j];
            }
            delete[] out_data[i];
        }
        delete[] out_data;
    }
};

#endif