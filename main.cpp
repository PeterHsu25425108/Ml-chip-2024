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
#define DEBUG 1

using namespace std;

void input_image(vector<vector<vector<double>>> &in_image, char *argv[])
{
    ifstream in_file(argv[1]);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 224; j++)
        {
            for (int k = 0; k < 224; k++)
            {
                in_file >> in_image[i][j][k];
            }
        }
    }
}

/*void checkClassification(const vector<double> &fc3_out)
{
    ifstream class_file("data/imagenet_classes.txt");
    string class_name;

    // find out and print the class name that has the highest probability
    // sort fc3_out in descending order
    vector<double> fc3_out_sorted = fc3_out;
    sort(fc3_out_sorted.begin(), fc3_out_sorted.end(), greater<double>());

    for (int i = 0; i < 5; i++)
    {
        int index = find(fc3_out.begin(), fc3_out.end(), fc3_out_sorted[i]) - fc3_out.begin();
        for (int j = 0; j < index; j++)
        {
            class_file >> class_name;
        }
        cout << "Class " << i + 1 << ": " << class_name << " with probability " << fc3_out_sorted[i] << endl;
    }
}*/

/*void init3DArr(sc_signal<double> ***arr, int x, int y, int z)
{
    arr = new sc_signal<double> **[x];
    for (int i = 0; i < x; i++)
    {
        arr[i] = new sc_signal<double> *[y];
        for (int j = 0; j < y; j++)
        {
            arr[i][j] = new sc_signal<double>[z];
        }
    }
}

void delete3DArr(sc_signal<double> ***arr, int x, int y)
{
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            delete[] arr[i][j];
        }
        delete[] arr[i];
    }
    delete[] arr;
}*/

int sc_main(int argc, char *argv[])
{
    // vector<vector<vector<double>>> in_image(3, vector<vector<double>>(224, vector<double>(224)));
    // input_image(in_image, argv);

    /*sc_signal<double> ***conv1_out;
    sc_signal<double> ***relu1_out;
    sc_signal<double> ***pool1_out;
    sc_signal<double> ***conv2_out;
    sc_signal<double> ***relu2_out;
    sc_signal<double> ***pool2_out;
    sc_signal<double> ***conv3_out;
    sc_signal<double> ***relu3_out;
    sc_signal<double> ***conv4_out;
    sc_signal<double> ***relu4_out;
    sc_signal<double> ***conv5_out;
    sc_signal<double> ***relu5_out;
    sc_signal<double> ***pool5_out;

    init3DArr(conv1_out, 55, 55, 64);
    init3DArr(relu1_out, 55, 55, 64);
    init3DArr(pool1_out, 27, 27, 64);
    init3DArr(conv2_out, 27, 27, 192);
    init3DArr(relu2_out, 27, 27, 192);
    init3DArr(pool2_out, 13, 13, 192);
    init3DArr(conv3_out, 13, 13, 384);
    init3DArr(relu3_out, 13, 13, 384);
    init3DArr(conv4_out, 13, 13, 256);
    init3DArr(relu4_out, 13, 13, 256);
    init3DArr(conv5_out, 13, 13, 256);
    init3DArr(relu5_out, 13, 13, 256);
    init3DArr(pool5_out, 6, 6, 256);
    */

    if (DEBUG)
    {
        cout << "sc_main: start\n";
    }
    /*
        sc_signal<double> conv1_out[55][55][64];
        sc_signal<double> relu1_out[55][55][64];
        sc_signal<double> pool1_out[27][27][64];
        sc_signal<double> conv2_out[27][27][192];
        sc_signal<double> relu2_out[27][27][192];
        sc_signal<double> pool2_out[13][13][192];
        sc_signal<double> conv3_out[13][13][384];
        sc_signal<double> relu3_out[13][13][384];
        sc_signal<double> conv4_out[13][13][256];
        sc_signal<double> relu4_out[13][13][256];
        sc_signal<double> conv5_out[13][13][256];
        sc_signal<double> relu5_out[13][13][256];
        sc_signal<double> pool5_out[6][6][256];

        // sc_signal<double> avgpool_out[6][6][256];

        sc_signal<double> flatten_out[9216];
        sc_signal<double> fc1_out[4096];
        sc_signal<double> relu6_out[4096];
        sc_signal<double> fc2_out[4096];
        sc_signal<double> relu7_out[4096];
        sc_signal<double> fc3_out[1000];*/

    sc_signal<double> ***conv1_out = new sc_signal<double> **[55];
    for (int i = 0; i < 55; i++)
    {
        conv1_out[i] = new sc_signal<double> *[55];
        for (int j = 0; j < 55; j++)
        {
            conv1_out[i][j] = new sc_signal<double>[64];
        }
    }

    sc_signal<double> ***relu1_out = new sc_signal<double> **[55];
    for (int i = 0; i < 55; i++)
    {
        relu1_out[i] = new sc_signal<double> *[55];
        for (int j = 0; j < 55; j++)
        {
            relu1_out[i][j] = new sc_signal<double>[64];
        }
    }

    sc_signal<double> ***pool1_out = new sc_signal<double> **[27];
    for (int i = 0; i < 27; i++)
    {
        pool1_out[i] = new sc_signal<double> *[27];
        for (int j = 0; j < 27; j++)
        {
            pool1_out[i][j] = new sc_signal<double>[64];
        }
    }

    sc_signal<double> ***conv2_out = new sc_signal<double> **[27];
    for (int i = 0; i < 27; i++)
    {
        conv2_out[i] = new sc_signal<double> *[27];
        for (int j = 0; j < 27; j++)
        {
            conv2_out[i][j] = new sc_signal<double>[192];
        }
    }

    sc_signal<double> ***relu2_out = new sc_signal<double> **[27];
    for (int i = 0; i < 27; i++)
    {
        relu2_out[i] = new sc_signal<double> *[27];
        for (int j = 0; j < 27; j++)
        {
            relu2_out[i][j] = new sc_signal<double>[192];
        }
    }

    sc_signal<double> ***pool2_out = new sc_signal<double> **[13];
    for (int i = 0; i < 13; i++)
    {
        pool2_out[i] = new sc_signal<double> *[13];
        for (int j = 0; j < 13; j++)
        {
            pool2_out[i][j] = new sc_signal<double>[192];
        }
    }

    sc_signal<double> ***conv3_out = new sc_signal<double> **[13];
    for (int i = 0; i < 13; i++)
    {
        conv3_out[i] = new sc_signal<double> *[13];
        for (int j = 0; j < 13; j++)
        {
            conv3_out[i][j] = new sc_signal<double>[384];
        }
    }

    sc_signal<double> ***relu3_out = new sc_signal<double> **[13];
    for (int i = 0; i < 13; i++)
    {
        relu3_out[i] = new sc_signal<double> *[13];
        for (int j = 0; j < 13; j++)
        {
            relu3_out[i][j] = new sc_signal<double>[384];
        }
    }

    sc_signal<double> ***conv4_out = new sc_signal<double> **[13];
    for (int i = 0; i < 13; i++)
    {
        conv4_out[i] = new sc_signal<double> *[13];
        for (int j = 0; j < 13; j++)
        {
            conv4_out[i][j] = new sc_signal<double>[256];
        }
    }

    sc_signal<double> ***relu4_out = new sc_signal<double> **[13];
    for (int i = 0; i < 13; i++)
    {
        relu4_out[i] = new sc_signal<double> *[13];
        for (int j = 0; j < 13; j++)
        {
            relu4_out[i][j] = new sc_signal<double>[256];
        }
    }

    sc_signal<double> ***conv5_out = new sc_signal<double> **[13];
    for (int i = 0; i < 13; i++)
    {
        conv5_out[i] = new sc_signal<double> *[13];
        for (int j = 0; j < 13; j++)
        {
            conv5_out[i][j] = new sc_signal<double>[256];
        }
    }

    sc_signal<double> ***relu5_out = new sc_signal<double> **[13];
    for (int i = 0; i < 13; i++)
    {
        relu5_out[i] = new sc_signal<double> *[13];
        for (int j = 0; j < 13; j++)
        {
            relu5_out[i][j] = new sc_signal<double>[256];
        }
    }

    sc_signal<double> ***pool5_out = new sc_signal<double> **[6];
    for (int i = 0; i < 6; i++)
    {
        pool5_out[i] = new sc_signal<double> *[6];
        for (int j = 0; j < 6; j++)
        {
            pool5_out[i][j] = new sc_signal<double>[256];
        }
    }

    sc_signal<double> *flatten_out = new sc_signal<double>[9216];
    sc_signal<double> *fc1_out = new sc_signal<double>[4096];
    sc_signal<double> *relu6_out = new sc_signal<double>[4096];
    sc_signal<double> *fc2_out = new sc_signal<double>[4096];
    sc_signal<double> *relu7_out = new sc_signal<double>[4096];
    sc_signal<double> *fc3_out = new sc_signal<double>[1000];

    if (DEBUG)
    {
        cout << "sc_main: start creating modules\n";
    }

    // sc_module_name name, int in_wid, int out_wid, int in_ch, int out_ch, int padding, int stride, int kernel_size, string file_name) : in_wid(in_wid), out_wid(out_wid), in_ch(in_ch), out_ch(out_ch), padding(padding), stride(stride), kernel_size(kernel_size), file_name(file_name)
    Conv2d conv1("conv1", 224, 55, 3, 64, 2, 4, 11, "conv1");
    ReLU_3D relu1("relu1", 55, 64);
    // MaxPool2d(sc_module_name name, int in_ch, int in_wid, int kernel_size, int stride)
    MaxPool2d pool1("pool1", 64, 55, 3, 2);
    Conv2d conv2("conv2", 27, 27, 64, 192, 2, 1, 5, "conv2");
    ReLU_3D relu2("relu2", 27, 192);
    MaxPool2d pool2("pool2", 192, 27, 3, 2);
    Conv2d conv3("conv3", 13, 13, 192, 384, 1, 1, 3, "conv3");
    ReLU_3D relu3("relu3", 13, 384);
    Conv2d conv4("conv4", 13, 13, 384, 256, 1, 1, 3, "conv4");
    ReLU_3D relu4("relu4", 13, 256);
    Conv2d conv5("conv5", 13, 13, 256, 256, 1, 1, 3, "conv5");
    ReLU_3D relu5("relu5", 13, 256);
    MaxPool2d pool5("pool5", 256, 13, 3, 2);
    // AdaptiveAvgPool2d((6, 6)): Output size = (6, 6, 256)
    Flatten flatten("flatten");
    Linear fc1("fc1", 9216, 4096, "fc1");
    ReLU_1D relu6("relu6", 4096);
    Linear fc2("fc2", 4096, 4096, "fc2");
    ReLU_1D relu7("relu7", 4096);
    Linear fc3("fc3", 4096, 1000, "fc3");

    Monitor monitor("monitor");

    if (DEBUG)
    {
        cout << "sc_main: modules created\n";
    }

    sc_signal<double> in_data[3][224][224];

    if (DEBUG)
    {
        cout << "start reading input: \n";
    }

    ifstream in_file(argv[1]);
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

    if (DEBUG)
    {
        cout << "Input data read from file: \n";
    }

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

    for (int i = 0; i < 27; i++)
    {
        for (int j = 0; j < 27; j++)
        {
            for (int k = 0; k < 64; k++)
            {
                pool1.in_data[k][i][j](relu1_out[k][i][j]);
            }
        }
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

    for (int i = 0; i < 27; i++)
    {
        for (int j = 0; j < 27; j++)
        {
            for (int k = 0; k < 192; k++)
            {
                conv2.in_data[k][i][j](pool1_out[k][i][j]);
            }
        }
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

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            for (int k = 0; k < 192; k++)
            {
                pool2.in_data[k][i][j](relu2_out[k][i][j]);
            }
        }
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

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            for (int k = 0; k < 384; k++)
            {
                conv3.in_data[k][i][j](pool2_out[k][i][j]);
            }
        }
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

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            for (int k = 0; k < 256; k++)
            {
                conv4.in_data[k][i][j](relu3_out[k][i][j]);
            }
        }
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

    cout << "start simulating\n";
    sc_start(500, SC_NS);
    cout << "end simulating\n";

    // delete the dynamically allocated memory
    for (int i = 0; i < 55; i++)
    {
        for (int j = 0; j < 55; j++)
        {
            delete[] conv1_out[i][j];
        }
        delete[] conv1_out[i];
    }
    delete[] conv1_out;

    for (int i = 0; i < 55; i++)
    {
        for (int j = 0; j < 55; j++)
        {
            delete[] relu1_out[i][j];
        }
        delete[] relu1_out[i];
    }
    delete[] relu1_out;

    for (int i = 0; i < 27; i++)
    {
        for (int j = 0; j < 27; j++)
        {
            delete[] pool1_out[i][j];
        }
        delete[] pool1_out[i];
    }
    delete[] pool1_out;

    for (int i = 0; i < 27; i++)
    {
        for (int j = 0; j < 27; j++)
        {
            delete[] conv2_out[i][j];
        }
        delete[] conv2_out[i];
    }
    delete[] conv2_out;

    for (int i = 0; i < 27; i++)
    {
        for (int j = 0; j < 27; j++)
        {
            delete[] relu2_out[i][j];
        }
        delete[] relu2_out[i];
    }
    delete[] relu2_out;

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            delete[] pool2_out[i][j];
        }
        delete[] pool2_out[i];
    }
    delete[] pool2_out;

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            delete[] conv3_out[i][j];
        }
        delete[] conv3_out[i];
    }
    delete[] conv3_out;

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            delete[] relu3_out[i][j];
        }
        delete[] relu3_out[i];
    }
    delete[] relu3_out;

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            delete[] conv4_out[i][j];
        }
        delete[] conv4_out[i];
    }
    delete[] conv4_out;

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            delete[] relu4_out[i][j];
        }
        delete[] relu4_out[i];
    }
    delete[] relu4_out;

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            delete[] conv5_out[i][j];
        }
        delete[] conv5_out[i];
    }
    delete[] conv5_out;

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            delete[] relu5_out[i][j];
        }
        delete[] relu5_out[i];
    }
    delete[] relu5_out;

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            delete[] pool5_out[i][j];
        }
        delete[] pool5_out[i];
    }
    delete[] pool5_out;

    delete[] flatten_out;
    delete[] fc1_out;
    delete[] relu6_out;
    delete[] fc2_out;
    delete[] relu7_out;
    delete[] fc3_out;

    return 0;
}

/*Conv2d(3, 64, kernel_size=11, stride=4, padding=2): Output size = (55, 55, 64)
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
Linear(4096, num_classes): Output size = (num_classes,)*/