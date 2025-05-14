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
