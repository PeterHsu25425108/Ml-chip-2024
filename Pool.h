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

    sc_in<double> ***in_data;
    sc_out<double> ***out_data;

    void run()
    {
        for (int i = 0; i < in_ch; i++)
        {
            for (int j = 0; j < in_wid; j += stride)
            {
                for (int k = 0; k < in_wid; k += stride)
                {
                    double max = in_data[i][j][k].read();
                    for (int l = 0; l < kernel_size; l++)
                    {
                        for (int m = 0; m < kernel_size; m++)
                        {
                            if (in_data[i][j + l][k + m].read() > max)
                            {
                                max = in_data[i][j + l][k + m].read();
                            }
                        }
                    }
                    out_data[i][j / stride][k / stride].write(max);
                }
            }
        }
    }

    SC_CTOR(MaxPool2d) : in_ch(0), in_wid(0), kernel_size(0), stride(0)
    {
        in_data = NULL;
        out_data = NULL;
        /*SC_METHOD(run);
        for (int i = 0; i < in_ch; i++)
        {
            for (int j = 0; j < in_wid; j += stride)
            {
                for (int k = 0; k < in_wid; k += stride)
                {
                    sensitive << in_data[i][j][k];
                }
            }
        }*/
    }

    MaxPool2d(sc_module_name name, int in_ch, int in_wid, int kernel_size, int stride) : in_ch(in_ch), in_wid(in_wid), kernel_size(kernel_size), stride(stride)
    {
        in_data = new sc_in<double> **[in_ch];
        out_data = new sc_out<double> **[in_ch];
        SC_METHOD(run);
        for (int i = 0; i < in_ch; i++)
        {
            in_data[i] = new sc_in<double> *[in_wid];
            out_data[i] = new sc_out<double> *[in_wid];
            for (int j = 0; j < in_wid; j++)
            {
                in_data[i][j] = new sc_in<double>[in_wid];
                out_data[i][j] = new sc_out<double>[in_wid];
                for (int k = 0; k < in_wid; k++)
                {
                    sensitive << in_data[i][j][k];
                }
            }
        }
    }

    ~MaxPool2d()
    {
        for (int i = 0; i < in_ch; i++)
        {
            for (int j = 0; j < in_wid; j++)
            {
                delete[] in_data[i][j];
                delete[] out_data[i][j];
            }
            delete[] in_data[i];
            delete[] out_data[i];
        }
        delete[] in_data;
        delete[] out_data;
    }
};

#endif