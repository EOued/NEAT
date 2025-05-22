#include "neural_network.h"
#include "macros.h"
#include "testing.h"
#include "utils.h"

#include <time.h>

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
  network->layers[0].descriptor = 0;

  network->layers[1].ids_n = output_n;
  network->layers[1].ids_c = next_power_of_two(output_n);
  MEMCHK(network->layers[1].ids =
             malloc(network->layers[1].ids_c * sizeof(unsigned int)));
  for (unsigned int i = 0; i < output_n; i++)
    network->layers[1].ids[i] = input_n + i;
  network->layers[1].descriptor = 1;

  network->nodes_n = input_n + output_n;

  return network;
}

int findLayer(nn* nn, unsigned int node)
{
  layer layer;
  for (unsigned int i = 0; i < nn->layers_n; i++)
  {
    layer = nn->layers[i];
    for (unsigned int j = 0; j < layer.ids_n; j++)
      if (layer.ids[j] == node) return layer.descriptor;
  }
  return -1;
}

void addConnection(nn* nn, connection connection)
{
  if (!nn) return;

  // Layer checking
  int inputLayer, outputLayer;
  ERRCHK(inputLayer = findLayer(nn, connection.input));
  ERRCHK(outputLayer = findLayer(nn, connection.output));
  if (inputLayer >= outputLayer)
  {
    printf("ERR %d %d\n", inputLayer, outputLayer);
    ERR("inputLayer >= outputLayer");
  }

  REALLOC(nn->connections_n, nn->connections_c, sizeof(connection),
          nn->connections);
  nn->connections[nn->connections_n++] = connection;
  return;
}

void printNN(nn* nn)
{
  if (!nn) return;
  unsigned int inputLayerIndex, outputLayerIndex;
  for (unsigned int i = 0; i < nn->layers_n; i++)
  {
    if (nn->layers[i].descriptor == 0) inputLayerIndex = i;
    if (nn->layers[i].descriptor == nn->layers_n - 1) outputLayerIndex = i;
  }
  printf("Input nodes : ");
  for (unsigned int i = 0; i < nn->layers[inputLayerIndex].ids_n; i++)
    printf("%d ", nn->layers[inputLayerIndex].ids[i]);
  printf("\nOutput nodes : ");
  for (unsigned int i = 0; i < nn->layers[outputLayerIndex].ids_n; i++)
    printf("%d ", nn->layers[outputLayerIndex].ids[i]);
  printf("\nConnections:\n");

  connection c;
  for (unsigned int connection = 0; connection < nn->connections_n;
       connection++)
  {

    c = nn->connections[connection];
    if (c.enabled)
      printf("\t[%d(%d)--%f-->%d(%d): %d\n", c.input, findLayer(nn, c.input),
             c.weight, c.output, findLayer(nn, c.output), c.innov_number);
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

  for (int _ = 0; _ < 2; _++)
  {
    mutate(nn, 100, 50);
    printNN(nn);
    printf("\n");
  }
  freeNN(nn);
#endif
  return 0;
}
