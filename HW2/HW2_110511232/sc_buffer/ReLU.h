#ifndef RELU_H
#define RELU_H
#include <iostream>
#include <vector>
#include <fstream>
#include "systemc.h"

SC_MODULE(ReLU_1D)
{
    const int in_len;
    sc_port<sc_signal_in_if<double>> *in_data;
    sc_port<sc_signal_out_if<double>> *out_data;
    sc_in_clk iclk;

    void run()
    {
        // Execute ReLU activation function
        for (int i = 0; i < in_len; i++)
        {
            out_data[i]->write(in_data[i]->read() > 0 ? in_data[i]->read() : 0);
        }
    }

    SC_CTOR(ReLU_1D) : in_len(0)
    {
        in_data = NULL;
        out_data = NULL;
    }

    ReLU_1D(sc_module_name name, int in_len) : in_len(in_len)
    {
        // Initialize in_data and out_data of size in_len
        in_data = new sc_port<sc_signal_in_if<double>>[in_len];
        out_data = new sc_port<sc_signal_out_if<double>>[in_len];
        SC_METHOD(run);
        sensitive << iclk.pos();
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
    sc_port<sc_signal_in_if<double>> ***in_data;
    sc_port<sc_signal_out_if<double>> ***out_data;
    sc_in_clk iclk;

    void run()
    {
        // Execute ReLU activation function
        for (int i = 0; i < in_ch; i++)
        {
            for (int j = 0; j < in_wid; j++)
            {
                for (int k = 0; k < in_wid; k++)
                {
                    out_data[i][j][k]->write(in_data[i][j][k]->read() > 0 ? in_data[i][j][k]->read() : 0);
                }
            }
        }
    }

    SC_CTOR(ReLU_3D) : in_ch(0), in_wid(0)
    {
        in_data = NULL;
        out_data = NULL;
    }

    ReLU_3D(sc_module_name name, int in_ch, int in_wid) : in_ch(in_ch), in_wid(in_wid)
    {
        // Initialize in_data and out_data of size in_ch * in_wid * in_wid
        in_data = new sc_port<sc_signal_in_if<double>> **[in_ch];
        for (int i = 0; i < in_ch; i++)
        {
            in_data[i] = new sc_port<sc_signal_in_if<double>> *[in_wid];
            for (int j = 0; j < in_wid; j++)
            {
                in_data[i][j] = new sc_port<sc_signal_in_if<double>>[in_wid];
            }
        }

        out_data = new sc_port<sc_signal_out_if<double>> **[in_ch];
        for (int i = 0; i < in_ch; i++)
        {
            out_data[i] = new sc_port<sc_signal_out_if<double>> *[in_wid];
            for (int j = 0; j < in_wid; j++)
            {
                out_data[i][j] = new sc_port<sc_signal_out_if<double>>[in_wid];
            }
        }

        SC_METHOD(run);
        sensitive << iclk.pos();
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