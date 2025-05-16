#include "macros.h"
#include "neural_network.h"

#include <string.h>

// mutations

// Adding a new node

void insertNode(nn* nn, unsigned int connectionIndex)
{
  if (!nn) ERR("Given empty nn");
  if (connectionIndex >= nn->connections_n)
    ERR("Given connection does not exist");

  // Let a, b be the layers of each node of the connection (with b > a)
  // if b-a > 1, then the layer of the inserted node is a random layer r such as
  // a < r < b Otherwise, we need to insert a new layer and propagate the
  // insertion

  unsigned int input       = nn->connections[connectionIndex].input;
  unsigned int output      = nn->connections[connectionIndex].output;
  unsigned int inputLayer  = findLayer(nn, input);
  unsigned int outputLayer = findLayer(nn, output);

  unsigned int newLayer;
  if (outputLayer - inputLayer > 1)
  {
    newLayer = rand() % (outputLayer - inputLayer - 2) + inputLayer + 1;
  }
  else
  {
    newLayer = outputLayer;
    REALLOC(nn->layers_n++, nn->layers_c, sizeof(layer), nn->layers);

    for (int i = nn->layers_n - 1; i >= (int)outputLayer; i--)
      memcpy(&nn->layers[i + 1], &nn->layers[i], sizeof(layer));

    nn->layers[outputLayer].ids   = malloc(sizeof(unsigned int));
    nn->layers[outputLayer].ids_n = 0;
    nn->layers[outputLayer].ids_c = 1;
  }

  // Adding node

  REALLOC(nn->layers[newLayer].ids_n, nn->layers[newLayer].ids_c,
          sizeof(unsigned int), nn->layers[newLayer].ids);
  unsigned int newNode                                   = nn->nodes_n++;
  nn->layers[newLayer].ids[nn->layers[newLayer].ids_n++] = newNode;

  // Modifiying connections

  nn->connections[connectionIndex].enabled = 0;
  double weight = nn->connections[connectionIndex].weight;
  addConnection(nn, (connection){input, newNode, 1, 1, 1});
  addConnection(nn, (connection){newNode, output, 1, 1, weight});
  return;
}

void mutate(nn* nn, unsigned int mutationProbability,
            unsigned int nodeInsertionProbability)
{
  if (mutationProbability > 100)
    ERR("Mutation probability must be in rang 0, 100");
  if (nodeInsertionProbability > 100)
    ERR("Node insertion probability must be in range 0, 100");

  if (!nn || (unsigned int)rand() % 100 > mutationProbability) return;
  unsigned int mutation = rand() % 100;
  if (mutation < nodeInsertionProbability)
  {
    insertNode(nn, rand() % nn->connections_n);
    return;
  }

  // Add connection
  unsigned int inputLayer = rand() % (nn->layers_n - 1);
  if (inputLayer > 0) inputLayer++;
  unsigned int outputLayer = inputLayer + rand() % (nn->layers_n - inputLayer);
  if (inputLayer == outputLayer) outputLayer = 1;
  printf("%d %d\n", inputLayer, outputLayer);

  unsigned int inputNeuron  = rand() % (nn->layers[inputLayer].ids_n);
  unsigned int outputNeuron = rand() % (nn->layers[outputLayer].ids_n);

  double weight = ((double)rand() / (double)RAND_MAX) * 10.0;
  addConnection(nn, (connection){inputNeuron, outputNeuron, nn->connections_n,
                                 1, weight});
  return;
}
