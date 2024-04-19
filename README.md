# FeedForwardNeuralNetwork
Implementation of a basic multilayer perceptron on C++ language

The program must is built with Makefile which contains standard set of targets for GNU-programs: all, install, uninstall, clean, dvi, dist, tests.
- The program provides the ability to form and train neural network models to classify handwritten Latin letters
- The perceptron:
  - classifies images with handwritten letters of the Latin alphabet
  - has **from 2 to 5** hidden layers
  - uses a sigmoid activation function for each hidden layer
  - is able to learn on an open dataset (e.g. EMNIST-letters presented in the datasets directory). 
  - shows accuracy on a test sample *over 70 percent*
  - is trained using the backpropagation method
