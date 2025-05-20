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

  unsigned int input           = nn->connections[connectionIndex].input;
  unsigned int output          = nn->connections[connectionIndex].output;
  layer_descriptor inputLayer  = findLayer(nn, input);
  layer_descriptor outputLayer = findLayer(nn, output);

  unsigned int newLayer;
  unsigned int newLayerIndex;
  int minRange;
  int maxRange;
  printf("layers %s %s\n", layer_str(inputLayer), layer_str(outputLayer));

  // Not checking layer compatibility because this check is made at the creation
  // of a connection

  if (inputLayer.layer_t == INPUT) minRange = 0;
  else
    minRange = inputLayer.id + 1;

  if (outputLayer.layer_t == OUTPUT) maxRange = nn->layers_n - 2;
  else
    maxRange = outputLayer.id - 1;

  printf("range %d %d\n", minRange, maxRange);
  if (maxRange - minRange >= 0)
  {
    newLayer = minRange + rand() % (maxRange - minRange + 1);
  }
  else
  {
    REALLOC(nn->layers_n, nn->layers_c, sizeof(layer), nn->layers);
    nn->layers[nn->layers_n++].descriptor =
        (layer_descriptor){HIDDEN, nn->layers_n - 2};
  }

  for (unsigned int i = 0; i < nn->layers_n; i++)
  {
    printf("%d ? %d\n", nn->layers[i].descriptor.id, newLayer);
    if (nn->layers[i].descriptor.layer_t == HIDDEN &&
        nn->layers[i].descriptor.id == newLayer)
      newLayerIndex = i;
  }

  printf("uwu %d\n", nn->layers_n);
  printf("new layer index %d\n", newLayerIndex);
  REALLOC(nn->layers[newLayerIndex].ids_n, nn->layers[newLayerIndex].ids_c,
          sizeof(unsigned int), nn->layers[newLayerIndex].ids);
  unsigned int newNode = nn->nodes_n++;
  nn->layers[newLayerIndex].ids[nn->layers[newLayerIndex].ids_n++] = newNode;

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
