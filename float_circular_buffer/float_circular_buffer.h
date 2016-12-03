#ifndef float_CIRCULAR_BUFFER_H
#define  float_CIRCULAR_BUFFER_H
/*classe que implementa um buffer circular de floateiros, isto é implementa um vector
de floateiros em que quando chegamos ao fim começa a escrever por cima no inicio*/
#include <vector>


class float_circular_buffer{
private:
  float* buffer;
  float actual_position;
  float max_position;
  float loop_number;
public:
  float_circular_buffer(float buffer_size);
  ~float_circular_buffer();
  /*lê valor na actual_position*/
  float read_actual_value();
  /*lê valor na posição n*/
  float read_n_value(float n);
  /*returna todos os valores no buffer mas apenas os novos por exemplo
  se tiver na volta 1 na posição 10 só retorna os valores de 1 ... 10*/
  std::vector<float> read_all_values();
  /*insere valor na posição asseguir*/
  void insert_value(float value);


};

#endif
