#ifndef NEURAL_NETWORK
#define NEURAL_NETWORK

#include <limits.h>
#include <stdlib.h>

typedef struct
{
  unsigned int input;
  unsigned int output;
  unsigned int innov_number;
  unsigned int enabled;
  double weight;
} connection;

typedef struct
{
  // UUID for identification
  unsigned int descriptor;
  unsigned int* ids;
  // Numbers of IDs
  unsigned int ids_n;
  // List capacity
  unsigned int ids_c;
} layer;

typedef struct
{
  // List of layers
  layer* layers;
  // Number of layers
  unsigned int layers_n;
  // List capacity
  unsigned int layers_c;

  // List of connections
  connection* connections;
  // Number of connections
  unsigned int connections_n;
  // List capacity
  unsigned int connections_c;

  // Total number of nodes
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
void mutate(nn* nn, unsigned int mutationProbability,
            unsigned int nodeInsertionProbability);
#endif
