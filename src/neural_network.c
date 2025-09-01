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

connection* find_connection(nn* nn, unsigned int sensor, unsigned int output)
{
  for (unsigned int i = 0; i < nn->connections_n; i++)
    if (nn->connections[i].input == sensor &&
        nn->connections[i].output == output)
      return &nn->connections[i];
  return NULL;
}

// Get all connections leading to specific connection, return the number of them
unsigned int leading_connections(nn* nn, unsigned int neuron,
                                 connection** connection_list)
{
  unsigned int capacity = 4;
  unsigned int size     = 0;
  *connection_list      = calloc(capacity, sizeof(connection));
  for (unsigned int i = 0; i < nn->connections_n; i++)
  {
    if (nn->connections[i].output != neuron || !nn->connections[i].enabled)
      continue;
    REALLOC(size, capacity, sizeof(connection), *connection_list);
    (*connection_list)[size++] = nn->connections[i];
  }
  return size;
}

double neuron_value(nn* nn, unsigned int neuron, double* input_values)
{
  if (!nn) return -1;
  connection* l_connections  = NULL;
  unsigned int l_conn_number = leading_connections(nn, neuron, &l_connections);
  connection c;
  unsigned int _neuron;
  double value;
  double total_value = 0;
  for (unsigned int i = 0; i < l_conn_number; i++)
  {
    c       = l_connections[i];
    _neuron = c.input;
    // Check if neuron on input layer
    if (_neuron < nn->layers[0].ids_n) value = input_values[_neuron];
    else
      // Getting value through recursion
      // Improve it with a list to store already computed values
      value = neuron_value(nn, _neuron, input_values);
    total_value += c.weight * value;
  }
  free(l_connections);
  return total_value;
}

double* propagate(nn* nn, double* input_values,
                  double (*transfer_function)(double))
{
  if (!nn) return 0;

  layer last_layer      = nn->layers[1];
  double* output_values = calloc(last_layer.ids_n, sizeof(double));
  for (unsigned int neuron = 0; neuron < last_layer.ids_n; neuron++)
  {
    output_values[neuron] = transfer_function(
        neuron_value(nn, last_layer.ids[neuron], input_values));
  }
  return output_values;
}

double void_tf(double _) { return _; }

int main(void)
{
  srand(time(NULL));
#ifdef TEST
  nn_testing_init();
#else
  nn* nn        = createEmpty(2, 2);
  connection c1 = {
      .input = 0, .output = 2, .innov_number = 0, .enabled = 1, .weight = 0.5};
  connection c2 = {
      .input = 1, .output = 2, .innov_number = 0, .enabled = 1, .weight = 0.25};
  connection c3 = {
      .input = 0, .output = 3, .innov_number = 0, .enabled = 1, .weight = 0.25};
  connection c4 = {
      .input = 1, .output = 3, .innov_number = 0, .enabled = 1, .weight = 0.5};

  addConnection(nn, c1);
  addConnection(nn, c2);
  addConnection(nn, c3);
  addConnection(nn, c4);

  insertNode(nn, 0);
  printNN(nn);

  double input_values[2] = {1, 0.5};

  double* output_values = propagate(nn, input_values, void_tf);
  printf("Propagation result: %f - %f\n", output_values[0], output_values[1]);
  free(output_values);
  freeNN(nn);
#endif
  return 0;
}
