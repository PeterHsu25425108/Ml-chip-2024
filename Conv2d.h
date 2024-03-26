#ifndef CONV2D_H
#define CONV2D_H

#include <iostream>
#include <vector>
#include <fstream>
#include "systemc.h"

using namespace std;

SC_MODULE(Conv2d)
{
    const int in_wid, out_wid, in_ch, out_ch;
    const int padding, stride, kernel_size;
    string file_name;
    sc_in<double> ***in_data;
    sc_out<double> ***out_data;

    void run()
    {
        cout << "run conv2d" << endl;
        // Implement the functionality of the module here
        // a 3D vector to store the padded input with size (in_ch, in_wid + 2 * padding, in_wid + 2 * padding)
        // initialize the vector with zeros
        vector<vector<vector<double>>> padded_in(in_ch, vector<vector<double>>(in_wid + 2 * padding, vector<double>(in_wid + 2 * padding, 0)));

        // a 3D vector to store the output with size (out_ch, out_wid, out_wid)
        // initialize the vector with zeros
        vector<vector<vector<double>>> out_vec(out_ch, vector<vector<double>>(out_wid, vector<double>(out_wid, 0)));

        // weight matrix
        vector<vector<vector<vector<double>>>> weight(out_ch, vector<vector<vector<double>>>(in_ch, vector<vector<double>>(kernel_size, vector<double>(kernel_size, 0))));

        // bias vector
        vector<double> bias(out_ch, 0);

        // read the weight matrix from the file "data/file_name_weight.txt"
        ifstream weight_file("data/" + file_name + "_weight.txt");
        for (int i = 0; i < out_ch; i++)
        {
            for (int j = 0; j < in_ch; j++)
            {
                for (int k = 0; k < kernel_size; k++)
                {
                    for (int l = 0; l < kernel_size; l++)
                    {
                        weight_file >> weight[i][j][k][l];
                    }
                }
            }
        }
        weight_file.close();

        // read the bias vector from the file "data/file_name_bias.txt"
        ifstream bias_file("data/" + file_name + "_bias.txt");
        for (int i = 0; i < out_ch; i++)
        {
            bias_file >> bias[i];
        }
        bias_file.close();

        // padding the input
        for (int i = 0; i < in_ch; i++)
        {
            for (int j = 0; j < in_wid; j++)
            {
                for (int k = 0; k < in_wid; k++)
                {
                    // zero padding
                    padded_in[i][j + padding][k + padding] = in_data[i][j][k].read();
                }
            }
        }

        // convolution operation
        for (int i = 0; i < out_ch; i++)
        {
            for (int j = 0; j < out_wid; j++)
            {
                for (int k = 0; k < out_wid; k++)
                {
                    // initialize the output with zeros
                    out_vec[i][j][k] = 0;
                    for (int l = 0; l < in_ch; l++)
                    {
                        for (int m = 0; m < kernel_size; m++)
                        {
                            for (int n = 0; n < kernel_size; n++)
                            {
                                out_vec[i][j][k] += padded_in[l][j * stride + m][k * stride + n] * weight[i][l][m][n];
                            }
                        }
                    }
                }
            }
        }

        // add the bias
        for (int i = 0; i < out_ch; i++)
        {
            for (int j = 0; j < out_wid; j++)
            {
                for (int k = 0; k < out_wid; k++)
                {
                    out_vec[i][j][k] += bias[i];
                }
            }
        }

        // write the output to the out_data port
        for (int i = 0; i < out_ch; i++)
        {
            for (int j = 0; j < out_wid; j++)
            {
                for (int k = 0; k < out_wid; k++)
                {
                    out_data[i][j][k].write(out_vec[i][j][k]);
                }
            }
        }
    }

    SC_CTOR(Conv2d) : in_wid(0), out_wid(0), in_ch(0), out_ch(0), padding(0), stride(0), kernel_size(0), file_name("")
    {
        cout << "SC_CTOR(Conv2d)" << endl;
        // define the input ports with size (in_ch, in_wid, in_wid)
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

    Conv2d(sc_module_name name, int in_wid, int out_wid, int in_ch, int out_ch, int padding, int stride, int kernel_size, string file_name) : in_wid(in_wid), out_wid(out_wid), in_ch(in_ch), out_ch(out_ch), padding(padding), stride(stride), kernel_size(kernel_size), file_name(file_name)
    {
        // define the input ports with size (in_ch, in_wid, in_wid)
        // vector<vector<vector<sc_in<double>>>> in_data(in_ch /*, vector < vector<sc_in<double>>(in_wid, sc_in<double>())*/);
        // this->in_data = &in_data;
        cout << "constructor for " << name << endl;
        in_data = new sc_in<double> **[in_ch];
        for (int i = 0; i < in_ch; i++)
        {
            in_data[i] = new sc_in<double> *[in_wid];
            for (int j = 0; j < in_wid; j++)
            {
                in_data[i][j] = new sc_in<double>[in_wid];
            }
        }

        // define the output ports with size (out_ch, out_wid, out_wid)
        // vector<vector<vector<sc_out<double>>>> out_data(out_ch /*, vector < vector<sc_out<double>>(out_wid, sc_out<double>()) */);
        // this->out_data = &out_data;
        out_data = new sc_out<double> **[out_ch];
        for (int i = 0; i < out_ch; i++)
        {
            out_data[i] = new sc_out<double> *[out_wid];
            for (int j = 0; j < out_wid; j++)
            {
                out_data[i][j] = new sc_out<double>[out_wid];
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

    ~Conv2d()
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

        for (int i = 0; i < out_ch; i++)
        {
            for (int j = 0; j < out_wid; j++)
            {
                delete[] out_data[i][j];
            }
            delete[] out_data[i];
        }
        delete[] out_data;
    }
};

#endif