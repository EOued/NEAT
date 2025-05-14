#ifndef NEURAL_NETWORK
#define NEURAL_NETWORK

#include <stdlib.h>

typedef struct {
  unsigned int input;
  unsigned int output;
  unsigned int innov_number;
  unsigned int enabled;
  double weight;
} connection;

typedef struct {
  unsigned int* ids; 
  unsigned int ids_n;
  unsigned int ids_c;
  // 0 : sensor, 1  : output, 2-n : hidden (i - 2);
  unsigned int type;
} layer;

typedef struct {
  layer* layers;
  unsigned int layers_n;
  unsigned int layers_c;
  
  unsigned int connections_n;
  unsigned int connections_c;
  connection* connections;

  unsigned int nodes_n;
} nn;

// Basic functions
nn* createEmpty(unsigned int input_n, unsigned int output_n);
void addConnection(nn* nn, connection connection);
int findLayer(nn* nn, unsigned int node);
void freeNN(nn* nn);
void printNN(nn* nn);

// Genotype
void insertNode(nn* nn, unsigned int connectionIndex);

#endif
