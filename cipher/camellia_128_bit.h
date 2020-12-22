#ifndef CAMELLIA_128_BIT_H  
#define CAMELLIA_128_BIT_H
#include <gcrypt.h>
#include <stdlib.h> // для exit
#include <stdio.h>




// gcry_mpi_t camellia_n(gcry_mpi_t text_in, gcry_mpi_t key);

void bit_printf(gcry_mpi_t a, unsigned int num_of_bits); // побитовый вывод числа 


void bit_xor(gcry_mpi_t *result,const gcry_mpi_t a,const gcry_mpi_t b,const unsigned int num_of_bits); // побитовый XOR
 

void bit_or(gcry_mpi_t *result,const gcry_mpi_t a,const gcry_mpi_t b,const unsigned int num_of_bits); // битовая операция OR   ||


void bit_and(gcry_mpi_t *result,const gcry_mpi_t a,const gcry_mpi_t b,const unsigned int num_of_bits); // операция AND &&


void bit_lshift(gcry_mpi_t *result, const gcry_mpi_t a, const unsigned int l_bits, const unsigned int num_of_bits); // операция << 


void bit_cyclic_lshift(gcry_mpi_t *result, gcry_mpi_t in,const unsigned int l_bits, const unsigned int num_of_bits); // операция <<< 


int Power(int base, int pow); // возведение в степень ^


unsigned int from_mp_to_uint(gcry_mpi_t num); // из 16 ричной в 10 ричную


void SBOX1(gcry_mpi_t *result, gcry_mpi_t num); // взятие из таблички SBOX методом SBOX1 


void SBOX2(gcry_mpi_t *result, gcry_mpi_t num); // взятие из таблички SBOX методом SBOX2


void F_camellia(gcry_mpi_t *result, gcry_mpi_t F_IN, gcry_mpi_t KE); // Функция F 


void delete_round_keys(gcry_mpi_t subkeys[]); // удаление раундовых ключей 


void round_key(gcry_mpi_t key, gcry_mpi_t end_key[]); // развертка раундовых ключей 


void FL(gcry_mpi_t *result, gcry_mpi_t FL_IN, gcry_mpi_t KE); // функция FL (из алгоритма)


void FLINV(gcry_mpi_t *result, gcry_mpi_t FLINV_IN, gcry_mpi_t KE); // обратная функция FL (из алгоритма) 


void camellia_encryption(gcry_mpi_t *encypted_tex, const gcry_mpi_t M, const gcry_mpi_t subkeys[]); // Функция зашифрования одного блока информации алгоритмом Камеллия 


void camellia_decryption(gcry_mpi_t *text, const gcry_mpi_t M, const gcry_mpi_t subkeys[]); // Функция расшифрования одного блока информации


int test();// проверка 


#endif 

