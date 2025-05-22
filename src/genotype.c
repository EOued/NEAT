#include "macros.h"
#include "neural_network.h"
#include "utils.h"

#include <string.h>

// mutations

// Adding a new node

void insertNode(nn* nn, unsigned int connectionIndex)
{
  if (!nn) ERR("Given empty nn");
  if (connectionIndex >= nn->connections_n)
    ERR("Given connection does not exist");
  if (!nn->connections[connectionIndex].enabled) return;

  // Let a, b be the layers of each node of the connection (with b > a)
  // if b-a > 1, then the layer of the inserted node is a random layer r such as
  // a < r < b Otherwise, we need to insert a new layer and propagate the
  // insertion

  unsigned int input  = nn->connections[connectionIndex].input;
  unsigned int output = nn->connections[connectionIndex].output;
  int inputLayer      = findLayer(nn, input);
  int outputLayer     = findLayer(nn, output);

  unsigned int newLayer;
  unsigned int newLayerIndex;
  int minRange = inputLayer + 1;
  int maxRange = outputLayer - 1;

  // Not checking layer compatibility because this check is made at the creation
  // of a connection
  if (maxRange - minRange > 0)
    newLayer = random_between_range(minRange, maxRange);
  else
  {
    // Creation of a new layer
    newLayer = inputLayer + 1;
  }

  // Updating old layers descriptor if current layer exists
  for (unsigned int i = 0; i < nn->layers_n; i++)
  {
    if (maxRange - minRange > 0 && nn->layers[i].descriptor == newLayer)
      newLayerIndex = i;
    if (maxRange - minRange <= 0 && nn->layers[i].descriptor >= newLayer)
      nn->layers[i].descriptor++;
  }
  if (maxRange - minRange <= 0)
  {
    REALLOC(nn->layers_n, nn->layers_c, sizeof(layer), nn->layers);
    nn->layers[nn->layers_n].descriptor = newLayer;
    nn->layers[nn->layers_n].ids        = 0;
    nn->layers[nn->layers_n].ids_n      = 0;
    nn->layers[nn->layers_n].ids_c      = 0;
    newLayerIndex                       = nn->layers_n++;
  }

  REALLOC(nn->layers[newLayerIndex].ids_n, nn->layers[newLayerIndex].ids_c,
          sizeof(unsigned int), nn->layers[newLayerIndex].ids);
  unsigned int newNode = nn->nodes_n++;
  nn->layers[newLayerIndex].ids[nn->layers[newLayerIndex].ids_n++] = newNode;

  // Modifiying connections
  nn->connections[connectionIndex].enabled = 0;
  double weight = nn->connections[connectionIndex].weight;
  // TODO: ADDING CORRECT INNOV NUMBER
  addConnection(nn, (connection){input, newNode, nn->connections_n, 1, 1});
  addConnection(nn,
                (connection){newNode, output, nn->connections_n, 1, weight});
  return;
}

void mutate(nn* nn, unsigned int mutationProbability,
            unsigned int nodeInsertionProbability)
{
  if (mutationProbability > 100)
    ERR("Mutation probability must be in rang 0, 100");
  if (nodeInsertionProbability > 100)
    ERR("Node insertion probability must be in range 0, 100");

  if (!nn || (unsigned int)rand() % 101 > mutationProbability) return;
  if (rand() % 100 < (int)nodeInsertionProbability)
  {
    if (!nn->connections_n) return;
    int* indexes = malloc(nn->connections_n * sizeof(int));
    int size     = 0;
    for (unsigned int i = 0; i < nn->connections_n; i++)
      if (nn->connections[i].enabled) indexes[size++] = i;
    insertNode(nn, indexes[rand() % size]);
    free(indexes);
    return;
  }

  // Add connection
  unsigned int inputLayer = random_between_range(0, nn->layers_n - 2);
  unsigned int outputLayer =
      random_between_range(inputLayer + 1, nn->layers_n - 1);

  unsigned int inputNeuron =
      nn->layers[inputLayer].ids[rand() % (nn->layers[inputLayer].ids_n)];
  unsigned int outputNeuron =
      nn->layers[outputLayer].ids[rand() % (nn->layers[outputLayer].ids_n)];

  double weight = ((double)rand() / (double)RAND_MAX) * 10.0;
  addConnection(nn, (connection){inputNeuron, outputNeuron, nn->connections_n,
                                 1, weight});
  return;
}
