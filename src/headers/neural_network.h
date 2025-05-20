#ifndef NEURAL_NETWORK
#define NEURAL_NETWORK

#include <limits.h>
#include <stdlib.h>

enum type
{
  INPUT,
  OUTPUT,
  HIDDEN,
  EMPTY
};

typedef struct
{
  enum type layer_t;
  unsigned int id;
} layer_descriptor;

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
  layer_descriptor descriptor;
  unsigned int* ids;
  unsigned int ids_n;
  unsigned int ids_c;
} layer;

typedef struct
{
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
layer_descriptor findLayer(nn* nn, unsigned int node);
void freeNN(nn* nn);
void printNN(nn* nn);
char* layer_str(layer_descriptor lyr);

// Genotype
void insertNode(nn* nn, unsigned int connectionIndex);
void mutate(nn* nn, unsigned int mutationProbability,
            unsigned int nodeInsertionProbability);
#endif
