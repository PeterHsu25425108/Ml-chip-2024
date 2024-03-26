#ifndef RELU_H
#define RELU_H
#include <iostream>
#include <vector>
#include <fstream>
#include "systemc.h"

SC_MODULE(ReLU_1D)
{
    const int in_len;
    sc_in<double> *in_data;
    sc_out<double> *out_data;

    void run()
    {
        for (int i = 0; i < in_len; i++)
        {
            out_data[i].write(in_data[i].read() > 0 ? in_data[i].read() : 0);
        }
    }

    SC_CTOR(ReLU_1D) : in_len(0)
    {
        in_data = NULL;
        out_data = NULL;
        /*SC_METHOD(run);
        for (int i = 0; i < in_len; i++)
        {
            sensitive << in_data[i];
        }*/
    }

    ReLU_1D(sc_module_name name, int in_len) : in_len(in_len)
    {
        in_data = new sc_in<double>[in_len];
        out_data = new sc_out<double>[in_len];
        SC_METHOD(run);
        for (int i = 0; i < in_len; i++)
        {
            sensitive << in_data[i];
        }
    }

    ~ReLU_1D()
    {
        delete[] in_data;
        delete[] out_data;
    }
};

SC_MODULE(ReLU_3D)
{
    const int in_ch;
    const int in_wid;
    sc_in<double> ***in_data;
    sc_out<double> ***out_data;

    void run()
    {
        for (int i = 0; i < in_ch; i++)
        {
            for (int j = 0; j < in_wid; j++)
            {
                for (int k = 0; k < in_wid; k++)
                {
                    out_data[i][j][k].write(in_data[i][j][k].read() > 0 ? in_data[i][j][k].read() : 0);
                }
            }
        }
    }

    SC_CTOR(ReLU_3D) : in_ch(0), in_wid(0)
    {
        in_data = NULL;
        out_data = NULL;
        /*SC_METHOD(run);
        for (int i = 0; i < in_ch; i++)
        {
            for (int j = 0; j < in_wid; j++)
            {
                for (int k = 0; k < in_wid; k++)
                {
                    sensitive << in_data[i][j][k];
                }
            }
        }*/
    }

    ReLU_3D(sc_module_name name, int in_ch, int in_wid) : in_ch(in_ch), in_wid(in_wid)
    {
        in_data = new sc_in<double> **[in_ch];
        for (int i = 0; i < in_ch; i++)
        {
            in_data[i] = new sc_in<double> *[in_wid];
            for (int j = 0; j < in_wid; j++)
            {
                in_data[i][j] = new sc_in<double>[in_wid];
            }
        }

        out_data = new sc_out<double> **[in_ch];
        for (int i = 0; i < in_ch; i++)
        {
            out_data[i] = new sc_out<double> *[in_wid];
            for (int j = 0; j < in_wid; j++)
            {
                out_data[i][j] = new sc_out<double>[in_wid];
            }
        }

        SC_METHOD(run);
        for (int i = 0; i < in_ch; i++)
        {
            for (int j = 0; j < in_wid; j++)
            {
                for (int k = 0; k < in_wid; k++)
                {
                    sensitive << in_data[i][j][k];
                }
            }
        }
    }

    ~ReLU_3D()
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