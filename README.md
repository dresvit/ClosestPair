## Introduction
This is an **approximation algorithm** with random projection to find the closest pair of a set of objects in high-dimensional space.


## Dataset
* Please download from http://yann.lecun.com/exdb/mnist/
* Use **training set images (train-images-idx3-ubyte.gz)** as dataset `Mnist.ds` (need to extract)
* Each object is an image with pixels 28×28


## How to run
* Compile: `g++ cp.cpp -o cp`
* Run: `./cp -n 60000 -d 784 -f Mnist.ds`