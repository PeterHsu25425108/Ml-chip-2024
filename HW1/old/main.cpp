#include <iostream>
#include "ReLU.h"
#include <vector>
#include <fstream>
#include "conv2d.h"
#include "pool.h"
#include "fc.h"
#include <algorithm>
#include"systemc.h"

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

void checkClassification(const vector<double> &fc3_out)
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
}

int main(int argc, char *argv[])
{
    vector<vector<vector<double>>> in_image(3, vector<vector<double>>(224, vector<double>(224, 0)));

    cout << "start Input reading" << endl;

    input_image(in_image, argv);

    cout << "Input image read" << endl;

    vector<vector<vector<double>>> conv1_out(64, vector<vector<double>>(55, vector<double>(55, 0)));
    // apply conv1
    // Conv2d(int out_ch, int in_ch, int zero_pad, int in_wid, int out_wid, int ker_wid, int stride, vector<vector<vector<int>>> &input, vector<vector<vector<int>>> &output, string filename)
    // Meaning of each input of Conv2d: out_ch, in_ch, zero_pad, in_wid, out_wid, ker_wid, stride, input, output, filename
    Conv2d(64, 3, 2, 224, 55, 11, 4, in_image, conv1_out, "conv1");

    cout << "Conv1 applied" << endl;

    ReLU(conv1_out);

    vector<vector<vector<double>>> pool1_out(64, vector<vector<double>>(27, vector<double>(27, 0)));
    // apply pool1
    MaxPool2d(conv1_out, pool1_out, 3, 2);

    vector<vector<vector<double>>> conv2_out(192, vector<vector<double>>(27, vector<double>(27, 0)));
    // apply conv2
    Conv2d(192, 64, 2, 55, 27, 5, 1, pool1_out, conv2_out, "conv2");

    ReLU(conv2_out);

    vector<vector<vector<double>>> pool2_out(192, vector<vector<double>>(13, vector<double>(13, 0)));
    // apply pool2
    MaxPool2d(conv2_out, pool2_out, 3, 2);

    vector<vector<vector<double>>> conv3_out(384, vector<vector<double>>(13, vector<double>(13, 0)));
    // apply conv3
    Conv2d(384, 192, 1, 27, 13, 3, 1, pool2_out, conv3_out, "conv3");

    ReLU(conv3_out);

    vector<vector<vector<double>>> conv4_out(256, vector<vector<double>>(13, vector<double>(13, 0)));
    // apply conv4
    Conv2d(256, 384, 1, 13, 13, 3, 1, conv3_out, conv4_out, "conv4");

    ReLU(conv4_out);

    vector<vector<vector<double>>> conv5_out(256, vector<vector<double>>(13, vector<double>(13, 0)));
    // apply conv5
    Conv2d(256, 256, 1, 13, 13, 3, 1, conv4_out, conv5_out, "conv5");

    ReLU(conv5_out);

    vector<vector<vector<double>>> pool5_out(256, vector<vector<double>>(6, vector<double>(6, 0)));
    // apply pool5
    MaxPool2d(conv5_out, pool5_out, 3, 2);

    vector<vector<vector<double>>> avgpool_out(256, vector<vector<double>>(6, vector<double>(6, 0)));
    // apply adaptive avg pool
    AvgPool2d(pool5_out, avgpool_out, 6, 1);

    // flatten
    vector<double> fc1_in;
    flatten(fc1_in, avgpool_out);

    // apply fc1
    vector<double> fc1_out(4096, 0);
    fc(fc1_out, fc1_in, "fc1");
    ReLU(fc1_out);

    // apply fc2
    vector<double> fc2_out(4096, 0);
    fc(fc2_out, fc1_out, "fc2");
    ReLU(fc2_out);

    // apply fc3
    vector<double> fc3_out(1000, 0);
    fc(fc3_out, fc2_out, "fc3");

    // print output
    checkClassification(fc3_out);

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