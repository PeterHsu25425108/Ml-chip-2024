#ifndef POOL_H
#define POOL_H
#include <iostream>
#include <vector>
#include <fstream>
#include "systemc.h"

SC_MODULE(MaxPool2d)
{
    const int in_ch;
    const int in_wid;
    const int kernel_size;
    const int stride;
    sc_in_clk iclk;

    sc_port<sc_signal_in_if<double>> ***in_data;
    sc_port<sc_signal_out_if<double>> ***out_data;

    void run()
    {

        int out_wid = in_wid / stride;

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
                            // find the maximum value in the kernel
                            max_val = max(max_val, in_data[k][i * stride + l][j * stride + m]->read());
                        }
                    }
                    out_data[k][i][j]->write(max_val);
                }
            }
        }
    }

    SC_CTOR(MaxPool2d) : in_ch(0), in_wid(0), kernel_size(0), stride(0)
    {
        in_data = NULL;
        out_data = NULL;
    }

    MaxPool2d(sc_module_name name, int in_ch, int in_wid, int kernel_size, int stride) : in_ch(in_ch), in_wid(in_wid), kernel_size(kernel_size), stride(stride)
    {
        // Initialize the input and output ports of size in_ch x in_wid x in_wid
        in_data = new sc_port<sc_signal_in_if<double>> **[in_ch];
        out_data = new sc_port<sc_signal_out_if<double>> **[in_ch];

        for (int i = 0; i < in_ch; i++)
        {
            in_data[i] = new sc_port<sc_signal_in_if<double>> *[in_wid];
            out_data[i] = new sc_port<sc_signal_out_if<double>> *[in_wid / stride];
            for (int j = 0; j < in_wid; j++)
            {
                in_data[i][j] = new sc_port<sc_signal_in_if<double>>[in_wid];
            }
            for (int j = 0; j < in_wid / stride; j++)
            {
                out_data[i][j] = new sc_port<sc_signal_out_if<double>>[in_wid / stride];
            }
        }

        SC_METHOD(run);
        sensitive << iclk.pos();
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