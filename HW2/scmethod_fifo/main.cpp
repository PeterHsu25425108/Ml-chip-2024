#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "systemc.h"
#include "ReLU.h"
#include "Conv2d.h"
#include "Pool.h"
#include "Linear.h"
#include "Monitor.h"
#include <sstream>
#include <cmath>
#include "Pattern.h"
#include <string>
#define DEBUG 1

using namespace std;

/*
Size info for AlexNet:

Conv2d(3, 64, kernel_size=11, stride=4, padding=2): Output size = (55, 55, 64)
ReLU(inplace=True): Output size = (55, 55, 64)
MaxPool2d(kernel_size=3, stride=2): Output size = (27, 27, 64)
Conv2d(64, 192, kernel_size=5, padding=2): Output size = (27, 27, 192)
ReLU(inplace=True): Output size = (27, 27, 192)
MaxPool2d(kernel_size=3, stride=2): Output size = (13, 13, 192)
Conv2d(192, 384, kernel_size=3, padding=1): Output size = (13, 13, 384)
ReLU(inplace=True): Output size = (13, 13, 384)
Conv2d(384, 256, kernel_size=3, padding=1): Output size = (13, 13, 256)
ReLU(inplace=True): Output size = (13, 13, 256)
Conv2d(256, 256, kernel_size=3, padding=1): Output size = (13, 13, 256)
ReLU(inplace=True): Output size = (13, 13, 256)
MaxPool2d(kernel_size=3, stride=2): Output size = (6, 6, 256)
AdaptiveAvgPool2d((6, 6)): Output size = (6, 6, 256)
Flatten: Output size = (9216,)
Linear(256 * 6 * 6, 4096): Output size = (4096,)
ReLU(inplace=True): Output size = (4096,)
Linear(4096, 4096): Output size = (4096,)
ReLU(inplace=True): Output size = (4096,)
Linear(4096, num_classes): Output size = (num_classes,)
*/

int sc_main(int argc, char *argv[])
{

    if (DEBUG)
    {
        cout << "sc_main: start\n";
    }

    // declaring the clock
    sc_clock clk("clk", 4, SC_NS, 0.5, 1, SC_NS, false);

    // declaring the signals
    sc_fifo<double> ***conv1_out = new sc_fifo<double> **[64];
    for (int i = 0; i < 64; i++)
    {
        conv1_out[i] = new sc_fifo<double> *[55];
        for (int j = 0; j < 55; j++)
        {
            conv1_out[i][j] = new sc_fifo<double>[55];
        }
    }

    sc_fifo<double> ***relu1_out = new sc_fifo<double> **[64];
    for (int i = 0; i < 64; i++)
    {
        relu1_out[i] = new sc_fifo<double> *[55];
        for (int j = 0; j < 55; j++)
        {
            relu1_out[i][j] = new sc_fifo<double>[55];
        }
    }

    sc_fifo<double> ***pool1_out = new sc_fifo<double> **[64];
    for (int i = 0; i < 64; i++)
    {
        pool1_out[i] = new sc_fifo<double> *[27];
        for (int j = 0; j < 27; j++)
        {
            pool1_out[i][j] = new sc_fifo<double>[27];
        }
    }

    sc_fifo<double> ***conv2_out = new sc_fifo<double> **[192];
    for (int i = 0; i < 192; i++)
    {
        conv2_out[i] = new sc_fifo<double> *[27];
        for (int j = 0; j < 27; j++)
        {
            conv2_out[i][j] = new sc_fifo<double>[27];
        }
    }

    sc_fifo<double> ***relu2_out = new sc_fifo<double> **[192];
    for (int i = 0; i < 192; i++)
    {
        relu2_out[i] = new sc_fifo<double> *[27];
        for (int j = 0; j < 27; j++)
        {
            relu2_out[i][j] = new sc_fifo<double>[27];
        }
    }

    sc_fifo<double> ***pool2_out = new sc_fifo<double> **[192];
    for (int i = 0; i < 192; i++)
    {
        pool2_out[i] = new sc_fifo<double> *[13];
        for (int j = 0; j < 13; j++)
        {
            pool2_out[i][j] = new sc_fifo<double>[13];
        }
    }

    sc_fifo<double> ***conv3_out = new sc_fifo<double> **[384];
    for (int i = 0; i < 384; i++)
    {
        conv3_out[i] = new sc_fifo<double> *[13];
        for (int j = 0; j < 13; j++)
        {
            conv3_out[i][j] = new sc_fifo<double>[13];
        }
    }

    sc_fifo<double> ***relu3_out = new sc_fifo<double> **[384];
    for (int i = 0; i < 384; i++)
    {
        relu3_out[i] = new sc_fifo<double> *[13];
        for (int j = 0; j < 13; j++)
        {
            relu3_out[i][j] = new sc_fifo<double>[13];
        }
    }

    sc_fifo<double> ***conv4_out = new sc_fifo<double> **[256];
    for (int i = 0; i < 256; i++)
    {
        conv4_out[i] = new sc_fifo<double> *[13];
        for (int j = 0; j < 13; j++)
        {
            conv4_out[i][j] = new sc_fifo<double>[13];
        }
    }

    sc_fifo<double> ***relu4_out = new sc_fifo<double> **[256];
    for (int i = 0; i < 256; i++)
    {
        relu4_out[i] = new sc_fifo<double> *[13];
        for (int j = 0; j < 13; j++)
        {
            relu4_out[i][j] = new sc_fifo<double>[13];
        }
    }

    sc_fifo<double> ***conv5_out = new sc_fifo<double> **[256];
    for (int i = 0; i < 256; i++)
    {
        conv5_out[i] = new sc_fifo<double> *[13];
        for (int j = 0; j < 13; j++)
        {
            conv5_out[i][j] = new sc_fifo<double>[13];
        }
    }

    sc_fifo<double> ***relu5_out = new sc_fifo<double> **[256];
    for (int i = 0; i < 256; i++)
    {
        relu5_out[i] = new sc_fifo<double> *[13];
        for (int j = 0; j < 13; j++)
        {
            relu5_out[i][j] = new sc_fifo<double>[13];
        }
    }

    sc_fifo<double> ***pool5_out = new sc_fifo<double> **[256];
    for (int i = 0; i < 256; i++)
    {
        pool5_out[i] = new sc_fifo<double> *[6];
        for (int j = 0; j < 6; j++)
        {
            pool5_out[i][j] = new sc_fifo<double>[6];
        }
    }

    sc_fifo<double> *flatten_out = new sc_fifo<double>[9216];
    sc_fifo<double> *fc1_out = new sc_fifo<double>[4096];
    sc_fifo<double> *relu6_out = new sc_fifo<double>[4096];
    sc_fifo<double> *fc2_out = new sc_fifo<double>[4096];
    sc_fifo<double> *relu7_out = new sc_fifo<double>[4096];
    sc_fifo<double> *fc3_out = new sc_fifo<double>[1000];

    if (DEBUG)
    {
        cout << "sc_main: start creating modules\n";
    }

    // module instaitation
    string input_file = argv[1];
    Pattern pattern("pattern", input_file);
    Conv2d conv1("conv1", 224, 55, 3, 64, 2, 4, 11, "conv1");
    ReLU_3D relu1("relu1", 64, 55);
    MaxPool2d pool1("pool1", 64, 55, 3, 2);
    Conv2d conv2("conv2", 27, 27, 64, 192, 2, 1, 5, "conv2");
    ReLU_3D relu2("relu2", 192, 27);
    MaxPool2d pool2("pool2", 192, 27, 3, 2);
    Conv2d conv3("conv3", 13, 13, 192, 384, 1, 1, 3, "conv3");
    ReLU_3D relu3("relu3", 384, 13);
    Conv2d conv4("conv4", 13, 13, 384, 256, 1, 1, 3, "conv4");
    ReLU_3D relu4("relu4", 256, 13);
    Conv2d conv5("conv5", 13, 13, 256, 256, 1, 1, 3, "conv5");
    ReLU_3D relu5("relu5", 256, 13);
    MaxPool2d pool5("pool5", 256, 13, 3, 2);
    Flatten flatten("flatten");
    Linear fc1("fc6", 9216, 4096, "fc6");
    ReLU_1D relu6("relu6", 4096);
    Linear fc2("fc7", 4096, 4096, "fc7");
    ReLU_1D relu7("relu7", 4096);
    Linear fc3("fc8", 4096, 1000, "fc8");

    Monitor monitor("monitor");

    // connect the clk
    pattern.iclk(clk);
    conv1.iclk(clk);
    relu1.iclk(clk);
    pool1.iclk(clk);
    conv2.iclk(clk);
    relu2.iclk(clk);
    pool2.iclk(clk);
    conv3.iclk(clk);
    relu3.iclk(clk);
    conv4.iclk(clk);
    relu4.iclk(clk);
    conv5.iclk(clk);
    relu5.iclk(clk);
    pool5.iclk(clk);
    flatten.iclk(clk);
    fc1.iclk(clk);
    relu6.iclk(clk);
    fc2.iclk(clk);
    relu7.iclk(clk);
    fc3.iclk(clk);
    monitor.iclk(clk);

    if (DEBUG)
    {
        cout << "sc_main: modules created\n";
    }

    // signal for input data
    sc_fifo<double> ***in_data = new sc_fifo<double> **[3];
    for (int i = 0; i < 3; i++)
    {
        in_data[i] = new sc_fifo<double> *[224];
        for (int j = 0; j < 224; j++)
        {
            in_data[i][j] = new sc_fifo<double>[224];
        }
    }

    if (DEBUG)
    {
        cout << "start reading input: \n";
    }

    // connect the in_data to pattern.out_data
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 224; j++)
        {
            for (int k = 0; k < 224; k++)
            {
                pattern.out_data[i][j][k](in_data[i][j][k]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Input data read from file: \n";
    }

    // connecting the modules
    for (int i = 0; i < 224; i++)
    {
        for (int j = 0; j < 224; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                conv1.in_data[k][i][j](in_data[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Input data sent to conv1: \n";
    }

    for (int i = 0; i < 55; i++)
    {
        for (int j = 0; j < 55; j++)
        {
            for (int k = 0; k < 64; k++)
            {
                conv1.out_data[k][i][j](conv1_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Output data from conv1: \n";
    }

    for (int i = 0; i < 55; i++)
    {
        for (int j = 0; j < 55; j++)
        {
            for (int k = 0; k < 64; k++)
            {
                relu1.in_data[k][i][j](conv1_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Input data sent to relu1: \n";
    }

    for (int i = 0; i < 55; i++)
    {
        for (int j = 0; j < 55; j++)
        {
            for (int k = 0; k < 64; k++)
            {
                relu1.out_data[k][i][j](relu1_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Output data from relu1: \n";
    }

    for (int i = 0; i < 55; i++)
    {
        for (int j = 0; j < 55; j++)
        {
            for (int k = 0; k < 64; k++)
            {
                pool1.in_data[k][i][j](relu1_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Input data sent to pool1: \n";
    }

    for (int i = 0; i < 27; i++)
    {
        for (int j = 0; j < 27; j++)
        {
            for (int k = 0; k < 64; k++)
            {
                pool1.out_data[k][i][j](pool1_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Output data from pool1: \n";
    }

    for (int i = 0; i < 27; i++)
    {
        for (int j = 0; j < 27; j++)
        {
            for (int k = 0; k < 64; k++)
            {
                conv2.in_data[k][i][j](pool1_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Input data sent to conv2: \n";
    }

    for (int i = 0; i < 27; i++)
    {
        for (int j = 0; j < 27; j++)
        {
            for (int k = 0; k < 192; k++)
            {
                conv2.out_data[k][i][j](conv2_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Output data from conv2: \n";
    }

    for (int i = 0; i < 27; i++)
    {
        for (int j = 0; j < 27; j++)
        {
            for (int k = 0; k < 192; k++)
            {
                relu2.in_data[k][i][j](conv2_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Input data sent to relu2: \n";
    }

    for (int i = 0; i < 27; i++)
    {
        for (int j = 0; j < 27; j++)
        {
            for (int k = 0; k < 192; k++)
            {
                relu2.out_data[k][i][j](relu2_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Output data from relu2: \n";
    }

    for (int i = 0; i < 27; i++)
    {
        for (int j = 0; j < 27; j++)
        {
            for (int k = 0; k < 192; k++)
            {
                pool2.in_data[k][i][j](relu2_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Input data sent to pool2: \n";
    }

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            for (int k = 0; k < 192; k++)
            {
                pool2.out_data[k][i][j](pool2_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Output data from pool2: \n";
    }

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            for (int k = 0; k < 192; k++)
            {
                conv3.in_data[k][i][j](pool2_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Input data sent to conv3: \n";
    }

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            for (int k = 0; k < 384; k++)
            {
                conv3.out_data[k][i][j](conv3_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Output data from conv3: \n";
    }

    // avgpool.in_data(pool5_out);

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            for (int k = 0; k < 384; k++)
            {
                relu3.in_data[k][i][j](conv3_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Input data sent to relu3: \n";
    }

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            for (int k = 0; k < 384; k++)
            {
                relu3.out_data[k][i][j](relu3_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Output data from relu3: \n";
    }

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            for (int k = 0; k < 384; k++)
            {
                conv4.in_data[k][i][j](relu3_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Input data sent to conv4: \n";
    }

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            for (int k = 0; k < 256; k++)
            {
                conv4.out_data[k][i][j](conv4_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Output data from conv4: \n";
    }

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            for (int k = 0; k < 256; k++)
            {
                relu4.in_data[k][i][j](conv4_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Input data sent to relu4: \n";
    }

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            for (int k = 0; k < 256; k++)
            {
                relu4.out_data[k][i][j](relu4_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Output data from relu4: \n";
    }

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            for (int k = 0; k < 256; k++)
            {
                conv5.in_data[k][i][j](relu4_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Input data sent to conv5: \n";
    }

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            for (int k = 0; k < 256; k++)
            {
                conv5.out_data[k][i][j](conv5_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Output data from conv5: \n";
    }

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            for (int k = 0; k < 256; k++)
            {
                relu5.in_data[k][i][j](conv5_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Input data sent to relu5: \n";
    }

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            for (int k = 0; k < 256; k++)
            {
                relu5.out_data[k][i][j](relu5_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Output data from relu5: \n";
    }

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            for (int k = 0; k < 256; k++)
            {
                pool5.in_data[k][i][j](relu5_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Input data sent to pool5: \n";
    }

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            for (int k = 0; k < 256; k++)
            {
                pool5.out_data[k][i][j](pool5_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Output data from pool5: \n";
    }

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            for (int k = 0; k < 256; k++)
            {
                flatten.in_data[k][i][j](pool5_out[k][i][j]);
            }
        }
    }

    if (DEBUG)
    {
        cout << "Input data sent to flatten: \n";
    }

    for (int i = 0; i < 9216; i++)
    {
        flatten.out_data[i](flatten_out[i]);
    }

    for (int i = 0; i < 9216; i++)
    {
        fc1.in_data[i](flatten_out[i]);
    }

    for (int i = 0; i < 4096; i++)
    {
        fc1.out_data[i](fc1_out[i]);
    }

    for (int i = 0; i < 4096; i++)
    {
        relu6.in_data[i](fc1_out[i]);
    }

    for (int i = 0; i < 4096; i++)
    {
        relu6.out_data[i](relu6_out[i]);
    }

    for (int i = 0; i < 4096; i++)
    {
        fc2.in_data[i](relu6_out[i]);
    }

    for (int i = 0; i < 4096; i++)
    {
        fc2.out_data[i](fc2_out[i]);
    }

    for (int i = 0; i < 4096; i++)
    {
        relu7.in_data[i](fc2_out[i]);
    }

    for (int i = 0; i < 4096; i++)
    {
        relu7.out_data[i](relu7_out[i]);
    }

    for (int i = 0; i < 4096; i++)
    {
        fc3.in_data[i](relu7_out[i]);
    }

    for (int i = 0; i < 1000; i++)
    {
        fc3.out_data[i](fc3_out[i]);
    }

    for (int i = 0; i < 1000; i++)
    {
        monitor.prob[i](fc3_out[i]);
    }

    // cout << "start simulating\n";
    sc_start(500, SC_NS);
    // cout << "end simulating\n";

    if (DEBUG)
    {
        cout << "start deleting\n";
    }

    // delete the dynamically allocated memory
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 224; j++)
        {
            delete[] in_data[i][j];
        }
        delete[] in_data[i];
    }
    delete[] in_data;

    if (DEBUG)
    {
        cout << "in_data deleted\n";
    }

    // Delete conv1_out
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 55; j++)
        {
            delete[] conv1_out[i][j];
        }
        delete[] conv1_out[i];
    }
    delete[] conv1_out;

    if (DEBUG)
    {
        cout << "conv1_out deleted\n";
    }

    // Delete relu1_out
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 55; j++)
        {
            delete[] relu1_out[i][j];
        }
        delete[] relu1_out[i];
    }
    delete[] relu1_out;

    if (DEBUG)
    {
        cout << "relu1_out deleted\n";
    }

    // Delete pool1_out
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 27; j++)
        {
            delete[] pool1_out[i][j];
        }
        delete[] pool1_out[i];
    }
    delete[] pool1_out;

    if (DEBUG)
    {
        cout << "pool1_out deleted\n";
    }

    // Delete conv2_out
    for (int i = 0; i < 192; i++)
    {
        for (int j = 0; j < 27; j++)
        {
            delete[] conv2_out[i][j];
        }
        delete[] conv2_out[i];
    }
    delete[] conv2_out;

    if (DEBUG)
    {
        cout << "conv2_out deleted\n";
    }

    // Delete relu2_out
    for (int i = 0; i < 192; i++)
    {
        for (int j = 0; j < 27; j++)
        {
            delete[] relu2_out[i][j];
        }
        delete[] relu2_out[i];
    }
    delete[] relu2_out;

    if (DEBUG)
    {
        cout << "relu2_out deleted\n";
    }

    // Delete pool2_out
    for (int i = 0; i < 192; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            delete[] pool2_out[i][j];
        }
        delete[] pool2_out[i];
    }
    delete[] pool2_out;

    if (DEBUG)
    {
        cout << "pool2_out deleted\n";
    }

    // Delete conv3_out
    for (int i = 0; i < 384; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            delete[] conv3_out[i][j];
        }
        delete[] conv3_out[i];
    }
    delete[] conv3_out;

    if (DEBUG)
    {
        cout << "conv3_out deleted\n";
    }

    // Delete relu3_out
    for (int i = 0; i < 384; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            delete[] relu3_out[i][j];
        }
        delete[] relu3_out[i];
    }
    delete[] relu3_out;

    if (DEBUG)
    {
        cout << "relu3_out deleted\n";
    }

    // Delete conv4_out
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            delete[] conv4_out[i][j];
        }
        delete[] conv4_out[i];
    }
    delete[] conv4_out;

    if (DEBUG)
    {
        cout << "conv4_out deleted\n";
    }

    // Delete relu4_out
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            delete[] relu4_out[i][j];
        }
        delete[] relu4_out[i];
    }
    delete[] relu4_out;

    if (DEBUG)
    {
        cout << "relu4_out deleted\n";
    }

    // Delete conv5_out
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            delete[] conv5_out[i][j];
        }
        delete[] conv5_out[i];
    }
    delete[] conv5_out;

    if (DEBUG)
    {
        cout << "conv5_out deleted\n";
    }

    // Delete relu5_out
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            delete[] relu5_out[i][j];
        }
        delete[] relu5_out[i];
    }
    delete[] relu5_out;

    if (DEBUG)
    {
        cout << "relu5_out deleted\n";
    }

    // Delete pool5_out
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            delete[] pool5_out[i][j];
        }
        delete[] pool5_out[i];
    }
    delete[] pool5_out;

    if (DEBUG)
    {
        cout << "pool5_out deleted\n";
    }

    // Delete flatten_out
    delete[] flatten_out;

    if (DEBUG)
    {
        cout << "flatten_out deleted\n";
    }

    // Delete fc1_out
    delete[] fc1_out;

    if (DEBUG)
    {
        cout << "fc1_out deleted\n";
    }

    // Delete relu6_out
    delete[] relu6_out;

    if (DEBUG)
    {
        cout << "relu6_out deleted\n";
    }

    // Delete fc2_out
    delete[] fc2_out;

    if (DEBUG)
    {
        cout << "fc2_out deleted\n";
    }

    // Delete relu7_out
    delete[] relu7_out;

    if (DEBUG)
    {
        cout << "relu7_out deleted\n";
    }

    // Delete fc3_out
    delete[] fc3_out;

    if (DEBUG)
    {
        cout << "fc3_out deleted\n";
    }

    return 0;
}