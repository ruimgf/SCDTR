#ifndef INT_CIRCULAR_BUFFER_H
#define  INT_CIRCULAR_BUFFER_H
/*classe que implementa um buffer circular de inteiros, isto é implementa um vector
de inteiros em que quando chegamos ao fim começa a escrever por cima no inicio*/

class int_circular_buffer{
private:
  int* buffer;
  int actual_position;
  int max_position;
  int loop_number;
public:
  int_circular_buffer(int buffer_size);
  ~int_circular_buffer();
  /*lê valor na actual_position*/
  int read_actual_value();
  /*lê valor na posição n*/
  int read_n_value(int n);
  /*returna todos os valores no buffer mas apenas os novos por exemplo
  se tiver na volta 1 na posição 10 só retorna os valores de 1 ... 10*/
  int* read_all_values();
  /*insere valor na posição asseguir*/
  void insert_value(int value);


}

#endif
