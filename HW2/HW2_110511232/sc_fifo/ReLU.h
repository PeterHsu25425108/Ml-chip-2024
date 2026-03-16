#ifndef RELU_H
#define RELU_H
#include <iostream>
#include <vector>
#include <fstream>
#include "systemc.h"
#include "DEBUG.h"

using namespace std;

SC_MODULE(ReLU_1D)
{
    const int in_len;
    sc_port<sc_fifo_in_if<double>> *in_data;
    sc_port<sc_fifo_out_if<double>> *out_data;
    sc_in_clk iclk;
    string name;

    void run()
    {
        while (true)
        {
            if (DEBUG)
                cout << "run ReLU_1D" << endl;
            for (int i = 0; i < in_len; i++)
            {
                double temp;
                in_data[i]->read(temp);
                out_data[i]->write(temp > 0 ? temp : 0);
            }

            wait();
        }
    }

    SC_CTOR(ReLU_1D) : in_len(0)
    {
        in_data = NULL;
        out_data = NULL;
    }

    ReLU_1D(sc_module_name name, int in_len) : in_len(in_len), name(name)
    {
        // cout << "constructor for " << name << endl;
        in_data = new sc_port<sc_fifo_in_if<double>>[in_len];
        out_data = new sc_port<sc_fifo_out_if<double>>[in_len];
        SC_THREAD(run);
        sensitive << iclk.pos();
        dont_initialize();
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
    sc_port<sc_fifo_in_if<double>> ***in_data;
    sc_port<sc_fifo_out_if<double>> ***out_data;
    sc_in_clk iclk;
    string name;

    void run()
    {
        while (true)
        {
            if (DEBUG)
                cout << "run ReLU_3D" << endl;
            for (int i = 0; i < in_ch; i++)
            {
                for (int j = 0; j < in_wid; j++)
                {
                    for (int k = 0; k < in_wid; k++)
                    {
                        double temp;
                        /*bool readable = in_data[i][j][k]->nb_read(temp);
                        temp = readable ? temp : 0;*/
                        in_data[i][j][k]->read(temp);
                        out_data[i][j][k]->write(temp > 0 ? temp : 0);
                    }
                }
            }
            if(DEBUG) cout << "write " << name << endl;
            wait();
        }
    }

    SC_CTOR(ReLU_3D) : in_ch(0), in_wid(0)
    {
        in_data = NULL;
        out_data = NULL;
    }

    ReLU_3D(sc_module_name name, int in_ch, int in_wid) : in_ch(in_ch), in_wid(in_wid), name(name)
    {
        // cout << "constructor for " << name << endl;
        in_data = new sc_port<sc_fifo_in_if<double>> **[in_ch];
        for (int i = 0; i < in_ch; i++)
        {
            in_data[i] = new sc_port<sc_fifo_in_if<double>> *[in_wid];
            for (int j = 0; j < in_wid; j++)
            {
                in_data[i][j] = new sc_port<sc_fifo_in_if<double>>[in_wid];
            }
        }

        out_data = new sc_port<sc_fifo_out_if<double>> **[in_ch];
        for (int i = 0; i < in_ch; i++)
        {
            out_data[i] = new sc_port<sc_fifo_out_if<double>> *[in_wid];
            for (int j = 0; j < in_wid; j++)
            {
                out_data[i][j] = new sc_port<sc_fifo_out_if<double>>[in_wid];
            }
        }

        SC_THREAD(run);
        sensitive << iclk.pos();
        dont_initialize();
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