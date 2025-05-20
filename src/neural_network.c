#include "neural_network.h"
#include "macros.h"
#include "testing.h"
#include "utils.h"

#include <time.h>

int layer_id(layer_descriptor l)
{
  return l.layer_t == INPUT ? -1 : (l.layer_t == OUTPUT ? INT_MAX : (int)l.id);
}

nn* createEmpty(unsigned int input_n, unsigned int output_n)
{

  if (input_n == 0) ERR("Sensor layer size must be strictly positive");
  if (output_n == 0) ERR("Output layer size must be strictly positive");

  nn* network;
  MEMCHK(network = malloc(sizeof(nn)));
  network->connections   = 0;
  network->connections_n = 0;
  network->connections_c = 0;

  MEMCHK(network->layers = malloc(2 * sizeof(layer)));
  network->layers_n = 2;
  network->layers_c = 2;

  network->layers[0].ids_n = input_n;
  network->layers[0].ids_c = next_power_of_two(input_n);
  MEMCHK(network->layers[0].ids =
             malloc(network->layers[0].ids_c * sizeof(unsigned int)));
  for (unsigned int i = 0; i < input_n; i++) network->layers[0].ids[i] = i;
  network->layers[0].descriptor = (layer_descriptor){INPUT, 0};

  network->layers[1].ids_n = output_n;
  network->layers[1].ids_c = next_power_of_two(output_n);
  MEMCHK(network->layers[1].ids =
             malloc(network->layers[1].ids_c * sizeof(unsigned int)));
  for (unsigned int i = 0; i < output_n; i++)
    network->layers[1].ids[i] = input_n + i;
  network->layers[1].descriptor = (layer_descriptor){OUTPUT, 0};

  network->nodes_n = input_n + output_n;

  return network;
}

layer_descriptor findLayer(nn* nn, unsigned int node)
{
  layer layer;
  for (unsigned int i = 0; i < nn->layers_n; i++)
  {
    layer = nn->layers[i];
    for (unsigned int j = 0; j < layer.ids_n; j++)
      if (layer.ids[j] == node) return layer.descriptor;
  }
  return NULL_LD;
}

void addConnection(nn* nn, connection connection)
{
  if (!nn) return;

  // Layer checking
  layer_descriptor inputLayer, outputLayer;
  LYRERRCHK(inputLayer = findLayer(nn, connection.input));
  LYRERRCHK(outputLayer = findLayer(nn, connection.output));
  if (layer_id(inputLayer) >= layer_id(outputLayer))
    ERR("inputLayer >= outputLayer");

  REALLOC(nn->connections_n, nn->connections_c, sizeof(connection),
          nn->connections);
  nn->connections[nn->connections_n++] = connection;
  return;
}

char* layer_str(layer_descriptor lyr)
{
  if (lyr.layer_t == EMPTY) return "";
  int id;
  switch (id = layer_id(lyr))
  {
  case -1: return "INPUT";
  case INT_MAX: return "OUTPUT";
  default:
  {
    static char str[50];
    sprintf(str, "%d", id);
    return str;
  }
  }
}

void printNN(nn* nn)
{
  if (!nn) return;
  unsigned int inputLayerIndex, outputLayerIndex;
  for (unsigned int i = 0; i < nn->layers_n; i++)
  {
    if (nn->layers[i].descriptor.layer_t == INPUT) inputLayerIndex = i;
    if (nn->layers[i].descriptor.layer_t == OUTPUT) outputLayerIndex = i;
  }
  printf("Input nodes : ");
  for (unsigned int i = 0; i < nn->layers[inputLayerIndex].ids_n; i++)
    printf("%d ", nn->layers[inputLayerIndex].ids[i]);
  printf("\nOutput nodes : ");
  for (unsigned int i = 0; i < nn->layers[outputLayerIndex].ids_n; i++)
    printf("%d ", nn->layers[outputLayerIndex].ids[i]);
  printf("\nConnections:");

  for (unsigned int connection = 0; connection < nn->connections_n;
       connection++)
  {
    if (nn->connections[connection].enabled)
      printf("\t[%d(%s)--%f-->%d(%s)] : %d\n",
             nn->connections[connection].input,
             layer_str(findLayer(nn, nn->connections[connection].input)),
             nn->connections[connection].weight,
             nn->connections[connection].output,
             layer_str(findLayer(nn, nn->connections[connection].output)),
             nn->connections[connection].innov_number);
  }
}

void freeNN(nn* nn)
{
  if (!nn) return;
  if (nn->layers)
  {
    for (unsigned int i = 0; i < nn->layers_n; i++)
      if (nn->layers[i].ids) free(nn->layers[i].ids);
    free(nn->layers);
  }
  if (nn->connections) free(nn->connections);
  free(nn);
  return;
}

int main(void)
{
  srand(time(NULL));
#ifdef TEST
  nn_testing_init();
#else
  nn* nn = createEmpty(3, 2);
  printNN(nn);
  printf("\n");
  addConnection(nn, (connection){0, 3, 1, 1, 1.0f});
  printNN(nn);
  printf("\n");
  insertNode(nn, 0);
  printNN(nn);
  printf("\n");
  insertNode(nn, 0);
  printNN(nn);
  printf("\n");
  mutate(nn, 100, 0);
  printNN(nn);
  printf("\n");
  freeNN(nn);
#endif
  return 0;
}
