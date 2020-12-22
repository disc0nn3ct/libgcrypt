#include "camellia_128_bit.h"
// RFC3713

void bit_printf(gcry_mpi_t a, unsigned int num_of_bits)
{
	for(unsigned int i=0; i<num_of_bits; i++)
	{
		if(i%4==0 && i != 0)
			printf(" ");
		printf("%d", gcry_mpi_test_bit(a, i));
	}
	printf("\n");
}

void bit_xor(gcry_mpi_t *result,const gcry_mpi_t a,const gcry_mpi_t b,const unsigned int num_of_bits)
{
	gcry_mpi_scan(result, GCRYMPI_FMT_HEX, "00", 0, 0);

	// unsigned int lenn;
	// if(gcry_mpi_get_nbits(a) > gcry_mpi_get_nbits(b))
	// {
	// 	lenn = a;
	// }
	// else
	// 	lenn = b;

	for(int i=0; i < num_of_bits; i++)
	{
		if(gcry_mpi_test_bit(a, i) != gcry_mpi_test_bit(b, i))
		{
			gcry_mpi_set_bit(*result, i);
		}
		else
		{
			gcry_mpi_clear_bit(*result, i);
		}
	}

// gcry_mpi_dump(*result);
}

void bit_or(gcry_mpi_t *result,const gcry_mpi_t a,const gcry_mpi_t b,const unsigned int num_of_bits) // да 
{
	gcry_mpi_scan(result, GCRYMPI_FMT_HEX, "00", 0, 0);

	for(int i=0; i < num_of_bits; i++)
	{
		if(gcry_mpi_test_bit(a, i) == 1 || gcry_mpi_test_bit(b, i) == 1)
		{
			gcry_mpi_set_bit(*result, i);
		}
		else
			gcry_mpi_clear_bit(*result, i);

	}
}

void bit_and(gcry_mpi_t *result, const gcry_mpi_t a, const gcry_mpi_t b, const unsigned int num_of_bits) // да 
{
	gcry_mpi_scan(result, GCRYMPI_FMT_HEX, "00", 0, 0);
	for(int i=0; i < num_of_bits; i++)
	{

		if( (gcry_mpi_test_bit(a, i) == 1) && (gcry_mpi_test_bit(b, i) == 1))
		{

			gcry_mpi_set_bit(*result, i);
		}
		else
		{
			gcry_mpi_clear_bit(*result, i);
		}
	}	
}

// битовый сдвиг влево
void bit_lshift(gcry_mpi_t *result, const gcry_mpi_t a, const unsigned int l_bits, const unsigned int num_of_bits)
{
	gcry_mpi_t b = gcry_mpi_new(0);
	b = gcry_mpi_copy(a);
	gcry_mpi_lshift(b, b, l_bits);
	if(gcry_mpi_get_nbits(b) <= num_of_bits)
		*result = gcry_mpi_copy(b);
	else
	{
		gcry_mpi_clear_highbit(b, num_of_bits);
		// bit_printf(a, 12);
		*result = gcry_mpi_copy(b);
	}
	gcry_mpi_release(b);
}




void bit_cyclic_lshift(gcry_mpi_t *result, gcry_mpi_t in,const unsigned int l_bits, const unsigned int num_of_bits)
{
if(l_bits != 0 )
{
	gcry_mpi_t a = gcry_mpi_new(0);
	gcry_mpi_t b = gcry_mpi_new(0);
	a = gcry_mpi_copy(in);
	b = gcry_mpi_copy(in);

	bit_lshift(&a, a, l_bits, num_of_bits);
	
	gcry_mpi_rshift(b, b, (num_of_bits - l_bits)); // поменять с обрезкой больше 128 например
	
	bit_or(&a, a, b, num_of_bits);
	*result = gcry_mpi_copy(a);
	
////////////////////////////////////////////////////

	gcry_mpi_release(a);
	gcry_mpi_release(b);
}
else
{
	*result = gcry_mpi_copy(in);
}

}

// просто возведение в степень 
int Power(int base, int pow){ 
   if (pow == 0) {return 1;}
    if (pow < 0)   {return 0;}  

    int result=1;
         for(int i=0;i<pow;i++){
               result *= base;
         }
    return result;
}

// из mpi в uint (для поиска в массиве) 
unsigned int from_mp_to_uint(gcry_mpi_t num)
{
	unsigned int i=0;
	for(int l=0; l < gcry_mpi_get_nbits(num); l++)
	{
		if(gcry_mpi_test_bit(num, l) == 1 )
		{
			i+= Power(2, l);
		}
	}
	return 	i;
}

const char *SBOX[] = {"70", "82", "2c", "ec", "b3", "27", "c0", "e5", "e4", "85", "57", "35", "ea", "0c", "ae", "41",
                                  "23", "ef", "6b", "93", "45", "19", "a5", "21", "ed", "0e", "4f", "4e", "1d", "65", "92", "bd",
                                  "86", "b8", "af", "8f", "7c", "eb", "1f", "ce", "3e", "30", "dc", "5f", "5e", "c5", "0b", "1a",
                                  "a6", "e1", "39", "ca", "d5", "47", "5d", "3d", "d9", "01", "5a", "d6", "51", "56", "6c", "4d",
                                  "8b", "0d", "9a", "66", "fb", "cc", "b0", "2d", "74", "12", "2b", "20", "f0", "b1", "84", "99",
                                  "df", "4c", "cb", "c2", "34", "7e", "76", "05", "6d", "b7", "a9", "31", "d1", "17", "04", "d7",
                                  "14", "58", "3a", "61", "de", "1b", "11", "1c", "32", "0f", "9c", "16", "53", "18", "f2", "22",
                                  "fe", "44", "cf", "b2", "c3", "b5", "7a", "91", "24", "08", "e8", "a8", "60", "fc", "69", "50",
                                  "aa", "d0", "a0", "7d", "a1", "89", "62", "97", "54", "5b", "1e", "95", "e0", "ff", "64", "d2",
                                  "10", "c4", "00", "48", "a3", "f7", "75", "db", "8a", "03", "e6", "da", "09", "3f", "dd", "94",
                                  "87", "5c", "83", "02", "cd", "4a", "90", "33", "73", "67", "f6", "f3", "9d", "7f", "bf", "e2",
                                  "52", "9b", "d8", "26", "c8", "37", "c6", "3b", "81", "96", "6f", "4b", "13", "be", "63", "2e",
                                  "e9", "79", "a7", "8c", "9f", "6e", "bc", "8e", "29", "f5", "f9", "b6", "2f", "fd", "b4", "59",
                                  "78", "98", "06", "6a", "e7", "46", "71", "ba", "d4", "25", "ab", "42", "88", "a2", "8d", "fa",
                                  "72", "07", "b9", "55", "f8", "ee", "ac", "0a", "36", "49", "2a", "68", "3c", "38", "f1", "a4",
                                  "40", "28", "d3", "7b", "bb", "c9", "43", "c1", "15", "e3", "ad", "f4", "77", "c7", "80", "9e"};


// SBOX1 
void SBOX1(gcry_mpi_t *result, gcry_mpi_t num)
{
	uint numm = from_mp_to_uint(num);
	gcry_mpi_scan(result, GCRYMPI_FMT_HEX, SBOX[numm], 0, 0);
}

// SBOX2[x] = SBOX1[x] <<< 1;
void SBOX2(gcry_mpi_t *result, gcry_mpi_t num)
{
	SBOX1(result, num); 

	// gcry_mpi_dump(*result);
	bit_cyclic_lshift(result, *result, 1, 8); 
}
	
// SBOX3[x] = SBOX1[x] <<< 7;
void SBOX3(gcry_mpi_t *result, gcry_mpi_t num)
{
	SBOX1(result, num);
	bit_cyclic_lshift(result, *result, 7, 8); 
}

// SBOX4[x] = SBOX1[x <<< 1];
void SBOX4(gcry_mpi_t *result, gcry_mpi_t num)
{
	bit_cyclic_lshift(&num, num, 1, 8); // проблемное место? 
	SBOX1(result, num);
}



// F функция 
void F_camellia(gcry_mpi_t *result, gcry_mpi_t F_IN, gcry_mpi_t KE)
{

gcry_mpi_scan(result, GCRYMPI_FMT_HEX, "00", 0, 0);

if((gcry_mpi_get_nbits(F_IN) > 64) || (gcry_mpi_get_nbits(KE) > 64))
{
	perror("F_IN or KE to big : ");
}


gcry_mpi_t x;
gcry_mpi_t MASK8;
gcry_mpi_t t[8];
gcry_mpi_t y[8];

x = gcry_mpi_new(64);
MASK8 = gcry_mpi_new(8);
gcry_mpi_scan(&MASK8, GCRYMPI_FMT_HEX, "ff", 0, 0);


for(int i=0; i<8; i++)
{
	t[i] = gcry_mpi_new(8);
	y[i] = gcry_mpi_new(8);
}

bit_xor(&x, F_IN, KE, 64);  	// x  = F_IN ^ KE;
gcry_mpi_rshift(t[0], x, 56);   // t1 =  x >> 56;
gcry_mpi_rshift(t[1], x, 48);   // t2 = (x >> 48)
bit_and(&t[1], t[1], MASK8, 8); // t2 = (x >> 48) & MASK8;
gcry_mpi_rshift(t[2], x, 40);	// t3 = (x >> 40) 
bit_and(&t[2], t[2], MASK8, 8); // t3 = (x >> 40) & MASK8;
gcry_mpi_rshift(t[3], x, 32); // t4 = (x >> 32)
bit_and(&t[3], t[3], MASK8, 8); // t4 = (x >> 32) & MASK8;
gcry_mpi_rshift(t[4], x, 24);  // t5 = (x >> 24)
bit_and(&t[4], t[4], MASK8, 8); // t5 = (x >> 24) & MASK8;
gcry_mpi_rshift(t[5], x, 16); // t6 = (x >> 16)
bit_and(&t[5], t[5], MASK8, 8); // t6 = (x >> 16) & MASK8;
gcry_mpi_rshift(t[6], x, 8); // t7 = (x >>  8)
bit_and(&t[6], t[6], MASK8, 8); // t7 = (x >>  8) & MASK8;
bit_and(&t[7], x, MASK8, 8); // t8 =  x & MASK8;


SBOX1(&t[0], t[0]);	// t1 = SBOX1[t1];
SBOX2(&t[1], t[1]);	// t2 = SBOX2[t2];
SBOX3(&t[2], t[2]);	// t3 = SBOX3[t3];
SBOX4(&t[3], t[3]);	// t4 = SBOX4[t4];
SBOX2(&t[4], t[4]);	// t5 = SBOX2[t5];
SBOX3(&t[5], t[5]);	// t6 = SBOX3[t6];
SBOX4(&t[6], t[6]);	// t7 = SBOX4[t7];
SBOX1(&t[7], t[7]);	// t8 = SBOX1[t8];

// y1 = t1 ^ t3 ^ t4 ^ t6 ^ t7 ^ t8;
bit_xor(&y[0], t[0], t[2], 8);   
bit_xor(&y[0], y[0], t[3], 8);
bit_xor(&y[0], y[0], t[5], 8);
bit_xor(&y[0], y[0], t[6], 8);
bit_xor(&y[0], y[0], t[7], 8);

// y2 = t1 ^ t2 ^ t4 ^ t5 ^ t7 ^ t8;
bit_xor(&y[1], t[0], t[1], 8);   
bit_xor(&y[1], y[1], t[3], 8);
bit_xor(&y[1], y[1], t[4], 8);
bit_xor(&y[1], y[1], t[6], 8);
bit_xor(&y[1], y[1], t[7], 8);


// y3 = t1 ^ t2 ^ t3 ^ t5 ^ t6 ^ t8;
bit_xor(&y[2], t[0], t[1], 8);   
bit_xor(&y[2], y[2], t[2], 8);
bit_xor(&y[2], y[2], t[4], 8);
bit_xor(&y[2], y[2], t[5], 8);
bit_xor(&y[2], y[2], t[7], 8);

// y4 = t2 ^ t3 ^ t4 ^ t5 ^ t6 ^ t7;
bit_xor(&y[3], t[1], t[2], 8);   
bit_xor(&y[3], y[3], t[3], 8);
bit_xor(&y[3], y[3], t[4], 8);
bit_xor(&y[3], y[3], t[5], 8);
bit_xor(&y[3], y[3], t[6], 8);


// y5 = t1 ^ t2 ^ t6 ^ t7 ^ t8;
bit_xor(&y[4], t[0], t[1], 8);   
bit_xor(&y[4], y[4], t[5], 8);
bit_xor(&y[4], y[4], t[6], 8);
bit_xor(&y[4], y[4], t[7], 8);

// y6 = t2 ^ t3 ^ t5 ^ t7 ^ t8;
bit_xor(&y[5], t[1], t[2], 8);   
bit_xor(&y[5], y[5], t[4], 8);
bit_xor(&y[5], y[5], t[6], 8);
bit_xor(&y[5], y[5], t[7], 8);

// y7 = t3 ^ t4 ^ t5 ^ t6 ^ t8;
bit_xor(&y[6], t[2], t[3], 8);   
bit_xor(&y[6], y[6], t[4], 8);
bit_xor(&y[6], y[6], t[5], 8);
bit_xor(&y[6], y[6], t[7], 8);

// y8 = t1 ^ t4 ^ t5 ^ t6 ^ t7;
bit_xor(&y[7], t[0], t[3], 8);   
bit_xor(&y[7], y[7], t[4], 8);
bit_xor(&y[7], y[7], t[5], 8);
bit_xor(&y[7], y[7], t[6], 8);


// F_OUT = (y1 << 56) | (y2 << 48) | (y3 << 40) | (y4 << 32)
// | (y5 << 24) | (y6 << 16) | (y7 <<  8) | y8;
// return FO_OUT;

bit_lshift(&y[0], y[0], 56, 64);
bit_lshift(&y[1], y[1], 48, 64);
bit_lshift(&y[2], y[2], 40, 64);
bit_lshift(&y[3], y[3], 32, 64);
bit_lshift(&y[4], y[4], 24, 64);
bit_lshift(&y[5], y[5], 16, 64);
bit_lshift(&y[6], y[6], 8, 64);
bit_lshift(&y[7], y[7], 0, 64);


bit_or(&x, y[0], y[1], 64);
bit_or(&x, x, y[2], 64);
bit_or(&x, x, y[3], 64);
bit_or(&x, x, y[4], 64);
bit_or(&x, x, y[5], 64);
bit_or(&x, x, y[6], 64);
bit_or(&x, x, y[7], 64);

*result = gcry_mpi_copy(x);

for(int i=0; i<8; i++)
{
	gcry_mpi_release(t[i]);
	gcry_mpi_release(y[i]);
}
gcry_mpi_release(x);
gcry_mpi_release(MASK8);

}

void ma_printf(gcry_mpi_t m)
{
	printf("========\n");
	gcry_mpi_dump(m);
	printf("\n========\n");
}


void delete_round_keys(gcry_mpi_t subkeys[])
{
	for(int i = 0; i < 26; i++)
	{
		gcry_mpi_release(subkeys[i]);
	}
}

void round_key(gcry_mpi_t key, gcry_mpi_t end_round_key[])
{

	
	for(int i=0; i<26; i++)
	{
		end_round_key[i] = gcry_mpi_new(0);
	}
	gcry_mpi_t buf1 = gcry_mpi_new(0);	

	gcry_mpi_t KL = gcry_mpi_new(0);	
	gcry_mpi_t KR = gcry_mpi_new(0);
	gcry_mpi_t KA = gcry_mpi_new(0);	
	gcry_mpi_t D1 = gcry_mpi_new(0);	
	gcry_mpi_t D2 = gcry_mpi_new(0);	
	gcry_mpi_t MASK64 = gcry_mpi_new(0);
	gcry_mpi_t SIGMA[6];

	for(int i=0; i<6; i++)
	{
		SIGMA[i] = gcry_mpi_new(0);
	}

	gcry_mpi_scan(&SIGMA[0], GCRYMPI_FMT_HEX, "A09E667F3BCC908B", 0, 0);
	gcry_mpi_scan(&SIGMA[1], GCRYMPI_FMT_HEX, "B67AE8584CAA73B2", 0, 0);
	gcry_mpi_scan(&SIGMA[2], GCRYMPI_FMT_HEX, "C6EF372FE94F82BE", 0, 0);
	gcry_mpi_scan(&SIGMA[3], GCRYMPI_FMT_HEX, "54FF53A5F1D36F1C", 0, 0);
	gcry_mpi_scan(&SIGMA[4], GCRYMPI_FMT_HEX, "10E527FADE682D1D", 0, 0);
	gcry_mpi_scan(&SIGMA[5], GCRYMPI_FMT_HEX, "B05688C2B3E6C1FD", 0, 0);


	KL = gcry_mpi_copy(key);
	gcry_mpi_scan(&KR, GCRYMPI_FMT_HEX, "00", 0, 0);
	gcry_mpi_scan(&MASK64, GCRYMPI_FMT_HEX, "ffffffffffffffff", 0, 0);



	bit_xor(&D1, KL, KR, 128);   // (KL ^ KR)
	gcry_mpi_rshift(D1, D1, 64); // D1 = (KL ^ KR) >> 64
	bit_xor(&D2, KL, KR, 64);   // (KL ^ KR)
	bit_and(&D2, D2, MASK64, 64); 	// (KL ^ KR) & MASK64;
	F_camellia(&buf1, D1, SIGMA[0]); // F(D1, Sigma1)
	bit_xor(&D2, D2, buf1, 64); // D2 = D2 ^ F(D1, Sigma1);
	F_camellia(&buf1, D2, SIGMA[1]); // F(D2, Sigma2);
	bit_xor(&D1, D1, buf1, 64); //    D1 = D1 ^ F(D2, Sigma2);
	gcry_mpi_rshift(buf1, KL, 64);	 // (KL >> 64)
	bit_xor(&D1, D1, buf1, 64);	// D1 = D1 ^ (KL >> 64);
	bit_and(&buf1, KL, MASK64, 64); 	// (KL & MASK64)
	bit_xor(&D2, D2, buf1, 64);  // D2 = D2 ^ (KL & MASK64);
	F_camellia(&buf1, D1, SIGMA[2]); // F(D1, Sigma3);
	bit_xor(&D2, D2, buf1, 64); // D2 = D2 ^ F(D1, Sigma3);
	F_camellia(&buf1, D2, SIGMA[3]); // F(D2, Sigma4);
	bit_xor(&D1, D1, buf1, 64); // D1 = D1 ^ F(D2, Sigma4);
	bit_lshift(&buf1, D1, 64, 128); // (D1 << 64)
	bit_or(&KA, buf1, D2, 128); // KA = (D1 << 64) | D2;

	// теперь раундовые ключи 

	bit_cyclic_lshift(&end_round_key[0], KL, 0, 64); // (KL <<<  0)
	gcry_mpi_rshift(end_round_key[0], end_round_key[0], 64); // kw1 = (KL <<<   0) >> 64;

	bit_cyclic_lshift(&end_round_key[1], KL, 0, 64); // (KL <<<  0)
	bit_and(&end_round_key[1], end_round_key[1], MASK64, 64); // kw2 = (KL <<<   0) & MASK64;

	bit_cyclic_lshift(&end_round_key[2], KA, 0, 128); // (KA <<<  0)
	gcry_mpi_rshift(end_round_key[2], end_round_key[2], 64); // k1  = (KA <<<   0) >> 64;

	bit_cyclic_lshift(&end_round_key[3], KA, 0, 128); // (KA <<<  0)
	bit_and(&end_round_key[3], end_round_key[3], MASK64, 128);  //  k2  = (KA <<<   0) & MASK64;

	bit_cyclic_lshift(&end_round_key[4], KL, 15, 128); // (KL <<<  15)
	gcry_mpi_rshift(end_round_key[4], end_round_key[4], 64); //  k3  = (KL <<<  15) >> 64;

	bit_cyclic_lshift(&end_round_key[5], KL, 15, 128);  // (KL <<<  15)
	bit_and(&end_round_key[5], end_round_key[5], MASK64, 128); // k4  = (KL <<<  15) & MASK64;

	bit_cyclic_lshift(&end_round_key[6], KA, 15, 128); // (KA <<<  15)
	gcry_mpi_rshift(end_round_key[6], end_round_key[6], 64); // k5  = (KA <<<  15) >> 64;

	bit_cyclic_lshift(&end_round_key[7], KA, 15, 128); // (KA <<<  15)
	bit_and(&end_round_key[7], end_round_key[7], MASK64, 128); //  k6  = (KA <<<  15) & MASK64;

	bit_cyclic_lshift(&end_round_key[8], KA, 30, 128); // (KA <<<  30)
	gcry_mpi_rshift(end_round_key[8], end_round_key[8], 64); //  ke1 = (KA <<<  30) >> 64;

	bit_cyclic_lshift(&end_round_key[9], KA, 30, 128); // (KA <<<  30)
	bit_and(&end_round_key[9], end_round_key[9], MASK64, 128); //  ke2 = (KA <<<  30) & MASK64;

	bit_cyclic_lshift(&end_round_key[10], KL, 45, 128); // (KL <<<  45)
	gcry_mpi_rshift(end_round_key[10], end_round_key[10], 64); //  k7  = (KL <<<  45) >> 64;

	bit_cyclic_lshift(&end_round_key[11], KL, 45, 128);         // (KL <<<  45)
	bit_and(&end_round_key[11], end_round_key[11], MASK64, 128); //  k8  = (KL <<<  45) & MASK64;

	bit_cyclic_lshift(&end_round_key[12], KA, 45, 128); // (KA <<<  45)
	gcry_mpi_rshift(end_round_key[12], end_round_key[12], 64); // k9  = (KA <<<  45) >> 64;

	bit_cyclic_lshift(&end_round_key[13], KL, 60, 128); // (KL <<<  60)
	bit_and(&end_round_key[13], end_round_key[13], MASK64, 64); // k10 = (KL <<<  60) & MASK64;

	bit_cyclic_lshift(&end_round_key[14], KA, 60, 128); // (KA <<<  60)
	gcry_mpi_rshift(end_round_key[14], end_round_key[14], 64); // k11 = (KA <<<  60) >> 64;

	bit_cyclic_lshift(&end_round_key[15], KA, 60, 128); // (KA <<<  60)
	bit_and(&end_round_key[15], end_round_key[15], MASK64, 128); // k12 = (KA <<<  60) & MASK64;

	bit_cyclic_lshift(&end_round_key[16], KL, 77, 128);  // (KL <<<  77)
	gcry_mpi_rshift(end_round_key[16], end_round_key[16], 64);//  ke3 = (KL <<<  77) >> 64;

	bit_cyclic_lshift(&end_round_key[17], KL, 77, 128); // (KL <<<  77)
	bit_and(&end_round_key[17], end_round_key[17], MASK64, 128);  //  ke4 = (KL <<<  77) & MASK64;

	bit_cyclic_lshift(&end_round_key[18], KL, 94, 128); // (KL <<<  94)
	gcry_mpi_rshift(end_round_key[18], end_round_key[18], 64);//  k13 = (KL <<<  94) >> 64;

	bit_cyclic_lshift(&end_round_key[19], KL, 94, 128); // (KL <<<  94)
	bit_and(&end_round_key[19], end_round_key[19], MASK64, 128); // k14 = (KL <<<  94) & MASK64;

	bit_cyclic_lshift(&end_round_key[20], KA, 94, 128); // (KA <<<  94)
	gcry_mpi_rshift(end_round_key[20], end_round_key[20], 64); // k15 = (KA <<<  94) >> 64;

	bit_cyclic_lshift(&end_round_key[21], KA, 94, 128); // (KA <<<  94)
	bit_and(&end_round_key[21], end_round_key[21], MASK64, 128); // k16 = (KA <<<  94) & MASK64;

	bit_cyclic_lshift(&end_round_key[22], KL, 111, 128); // (KL <<< 111)
	gcry_mpi_rshift(end_round_key[22], end_round_key[22], 64); // k17 = (KL <<< 111) >> 64;

	bit_cyclic_lshift(&end_round_key[23], KL, 111, 128); // (KL <<< 111)
	bit_and(&end_round_key[23], end_round_key[23], MASK64, 128); // k18 = (KL <<< 111) & MASK64;

	bit_cyclic_lshift(&end_round_key[24], KA, 111, 128); // (KA <<< 111)
	gcry_mpi_rshift(end_round_key[24], end_round_key[24], 64); // kw3 = (KA <<< 111) >> 64;

	bit_cyclic_lshift(&end_round_key[25], KA, 111, 128); // (KA <<< 111)
	bit_and(&end_round_key[25], end_round_key[25], MASK64, 128); // kw4 = (KA <<< 111) & MASK64;


	for(int i=0; i<6; i++)
	{
		gcry_mpi_release(SIGMA[i]);
	}

	gcry_mpi_release(KL);
	gcry_mpi_release(KA);
	gcry_mpi_release(KR);
	gcry_mpi_release(D1);
	gcry_mpi_release(D2);
	gcry_mpi_release(MASK64);

}

void FL(gcry_mpi_t *result, gcry_mpi_t FL_IN, gcry_mpi_t KE)
{
	if(gcry_mpi_get_nbits(FL_IN) > 64 || gcry_mpi_get_nbits(KE) > 64)
	{
		perror("TO BIG FL_IN or KE in Fl funktion ");
	}
	gcry_mpi_t x1, x2, k1, k2, MASK32, buf1;
	x1 = gcry_mpi_new(0);
	x2 = gcry_mpi_new(0);
	k1 = gcry_mpi_new(0);
	k2 = gcry_mpi_new(0);
	buf1 = gcry_mpi_new(0);
	MASK32 = gcry_mpi_new(0);
	gcry_mpi_scan(&MASK32, GCRYMPI_FMT_HEX, "ffffffff", 0, 0);




	gcry_mpi_rshift(x1, FL_IN, 32); 	// x1 = FL_IN >> 32;
	bit_and(&x2, FL_IN, MASK32, 32); // x2 = FL_IN & MASK32;

	gcry_mpi_rshift(k1, KE, 32); 		// k1 = KE >> 32;
	bit_and(&k2, KE, MASK32, 32); 		// k2 = KE & MASK32;
	bit_and(&buf1, x1, k1, 32); 		// (x1 & k1)
	bit_cyclic_lshift(&buf1, buf1, 1, 32); // ((x1 & k1) <<< 1);
	bit_xor(&x2, x2, buf1, 32); // x2 = x2 ^ ((x1 & k1) <<< 1);


	bit_or(&buf1, x2, k2, 32); // (x2 | k2)
	bit_xor(&x1, x1, buf1, 32); // x1 = x1 ^ (x2 | k2);

	bit_lshift(&buf1, x1, 32, 64); //(x1 << 32)
	bit_or(result, buf1, x2, 64);  // FL_OUT = (x1 << 32) | x2;

	gcry_mpi_release(x1);
	gcry_mpi_release(x2);
	gcry_mpi_release(k1);
	gcry_mpi_release(k2);
	gcry_mpi_release(MASK32);
	gcry_mpi_release(buf1);
}


// это обратная функция FL
void FLINV(gcry_mpi_t *result, gcry_mpi_t FLINV_IN, gcry_mpi_t KE)
{
	if(gcry_mpi_get_nbits(FLINV_IN) > 64 || gcry_mpi_get_nbits(KE) > 64)
	{
		perror("TO BIG FL_IN or KE in FLINV funktion ");
	}
	gcry_mpi_t y1, y2, k1, k2, MASK32, buf1;
	y1 = gcry_mpi_new(0);
	y2 = gcry_mpi_new(0);
	k1 = gcry_mpi_new(0);
	k2 = gcry_mpi_new(0);
	buf1 = gcry_mpi_new(0);
	MASK32 = gcry_mpi_new(0);
	gcry_mpi_scan(&MASK32, GCRYMPI_FMT_HEX, "ffffffff", 0, 0);

	gcry_mpi_rshift(y1, FLINV_IN, 32); 	//  y1 = FLINV_IN >> 32;
	bit_and(&y2, FLINV_IN, MASK32, 32); // y2 = FLINV_IN & MASK32;

	gcry_mpi_rshift(k1, KE, 32); // k1 = KE >> 32;
    bit_and(&k2, KE, MASK32, 32); // k2 = KE & MASK32;
 	bit_or(&buf1, y2, k2, 32);   // (y2 | k2)
    bit_xor(&y1, y1, buf1, 32);  // y1 = y1 ^ (y2 | k2);
    
    bit_and(&buf1, y1, k1, 32); // (y1 & k1)
    bit_cyclic_lshift(&buf1, buf1, 1, 32); //  (y1 & k1) <<< 1
    bit_xor(&y2, y2, buf1, 32);  // y2 = y2 ^ ((y1 & k1) <<< 1);	

    bit_lshift(&buf1, y1, 32, 64); // (y1 << 32)
    bit_or(result, buf1, y2, 64); // result = (y1 << 32) | y2;

	gcry_mpi_release(y1);
	gcry_mpi_release(y2);
	gcry_mpi_release(k1);
	gcry_mpi_release(k2);
	gcry_mpi_release(MASK32);
	gcry_mpi_release(buf1);

}





void camellia_encryption(gcry_mpi_t *encypted_tex,const gcry_mpi_t M, const gcry_mpi_t subkeys[])
{
	gcry_mpi_t D1, D2, buf1, MASK64;
	D1 = gcry_mpi_new(0);
	D2 = gcry_mpi_new(0);
	buf1 = gcry_mpi_new(0);
	MASK64 = gcry_mpi_new(0);
	gcry_mpi_scan(&MASK64, GCRYMPI_FMT_HEX, "ffffffffffffffff", 0, 0);

	gcry_mpi_rshift(D1, M, 64); // D1 = M >> 64; это будет Левая часть
	bit_and(&D2, M, MASK64, 64); // D2 = M & MASK64; Это будет Правая часть 




	bit_xor(&D1, D1, subkeys[0], 64); // !!! предварительное отбеливание !!! D1 = D1 ^ kw1;
	bit_xor(&D2, D2, subkeys[1], 64);  // D2 = D2 ^ kw2;

	F_camellia(&buf1, D1, subkeys[2]); 	  // F(D1, k1)
	bit_xor(&D2, D2, buf1, 64); // D2 = D2 ^ F(D1, k1);     // Раунд 1


	F_camellia(&buf1, D2, subkeys[3]); // F(D2, k2)
	bit_xor(&D1, D1, buf1, 64); // D1 = D1 ^ F(D2, k2);     // Раунд 2
	F_camellia(&buf1, D1, subkeys[4]); // F(D1, k3);
	bit_xor(&D2, D2, buf1, 64); // D2 = D2 ^ F(D1, k3);     // Раунд 3
	F_camellia(&buf1, D2, subkeys[5]);
	bit_xor(&D1, D1, buf1, 64);// D1 = D1 ^ F(D2, k4);     // Раунд 4
	F_camellia(&buf1, D1, subkeys[6]);	
	bit_xor(&D2, D2, buf1, 64);// D2 = D2 ^ F(D1, k5);     // Раунд 5
	F_camellia(&buf1, D2, subkeys[7]); 
	bit_xor(&D1, D1, buf1, 64); // D1 = D1 ^ F(D2, k6);     // Раунд 6
	FL(&D1, D1, subkeys[8]); // D1 = FL   (D1, ke1);     // FL    
	FLINV(&D2, D2, subkeys[9]); // D2 = FLINV(D2, ke2);     // FLINV

	F_camellia(&buf1, D1, subkeys[10]); // F(D1, k7);
	bit_xor(&D2, D2, buf1, 64); 	// D2 = D2 ^ F(D1, k7);     // Раунд 7
	F_camellia(&buf1, D2, subkeys[11]); // F(D2, k8)
	bit_xor(&D1, D1, buf1, 64); // D1 = D1 ^ F(D2, k8);     // Раунд 8
	F_camellia(&buf1, D1, subkeys[12]); // F(D1, k9);
	bit_xor(&D2, D2, buf1, 64); // 	// D2 = D2 ^ F(D1, k9);     // Раунд 9
	F_camellia(&buf1, D2, subkeys[13]); // F(D2, k10);
	bit_xor(&D1, D1, buf1, 64); //  D1 = D1 ^ F(D2, k10);    // Раунд 10
	F_camellia(&buf1, D1, subkeys[14]); //  F(D1, k11); 
	bit_xor(&D2, D2, buf1, 64); //  D2 = D2 ^ F(D1, k11);    // Раунд 11
	F_camellia(&buf1, D2, subkeys[15]); // F(D2, k12); 
	bit_xor(&D1, D1, buf1, 64); // D1 = D1 ^ F(D2, k12);    // Раунд 12
	FL(&D1, D1, subkeys[16]);  //  D1 = FL   (D1, ke3);     // FL
	FLINV(&D2, D2, subkeys[17]); // D2 = FLINV(D2, ke4);     // FLINV


	F_camellia(&buf1, D1, subkeys[18]); // F(D1, k13); 
	bit_xor(&D2, D2, buf1, 64); 	// D2 = D2 ^ F(D1, k13);    // Раунд 13
	F_camellia(&buf1, D2, subkeys[19]); // F(D2, k14); 
	bit_xor(&D1, D1, buf1, 64); // D1 = D1 ^ F(D2, k14);    // Раунд 14
	F_camellia(&buf1, D1, subkeys[20]); // F(D1, k15);
	bit_xor(&D2, D2, buf1, 64); // D2 = D2 ^ F(D1, k15);    // Раунд 15
	F_camellia(&buf1, D2, subkeys[21]); // F(D2, k16);
	bit_xor(&D1, D1, buf1, 64); 	// D1 = D1 ^ F(D2, k16);    // Раунд 16
	F_camellia(&buf1, D1, subkeys[22]); // F(D1, k17); 
	bit_xor(&D2, D2, buf1, 64); // D2 = D2 ^ F(D1, k17);    // Раунд 17
	F_camellia(&buf1, D2, subkeys[23]); // F(D2, k18); 
	bit_xor(&D1, D1, buf1, 64); // D1 = D1 ^ F(D2, k18);    // Раунд 18

	bit_xor(&D2, D2, subkeys[24], 64); // D2 = D2 ^ kw3;           // Постотбеливание 
	bit_xor(&D1, D1, subkeys[25], 64); // D1 = D1 ^ kw4;

	bit_lshift(&D2, D2, 64, 129); // (D2 << 64) 
	bit_or(encypted_tex, D1, D2, 128); 	// C = (D2 << 64) | D1; состоит из 128 бит ;D 

	gcry_mpi_release(D1);
	gcry_mpi_release(D2);
	gcry_mpi_release(buf1);
	gcry_mpi_release(MASK64);
}


// для расшифрования надо только поменять некоторые раундовые ключи местами => 
void camellia_decryption(gcry_mpi_t *text, const gcry_mpi_t M, const gcry_mpi_t subkeys[])
{
	gcry_mpi_t for_decryption[26];
	// gcry_mpi_t buf1;
	// buf1 = gcry_mpi_new(0);
	
	for(int i = 0; i < 26; i++)
	{
		for_decryption[i] = gcry_mpi_new(0);
		for_decryption[i] = gcry_mpi_copy(subkeys[i]);
	}
	gcry_mpi_swap(for_decryption[0], for_decryption[24]);
	gcry_mpi_swap(for_decryption[1], for_decryption[25]);
	gcry_mpi_swap(for_decryption[2], for_decryption[23]);
	gcry_mpi_swap(for_decryption[3], for_decryption[22]);
	gcry_mpi_swap(for_decryption[4], for_decryption[21]);
	gcry_mpi_swap(for_decryption[5], for_decryption[20]);
	gcry_mpi_swap(for_decryption[6], for_decryption[19]);
	gcry_mpi_swap(for_decryption[7], for_decryption[18]);
	gcry_mpi_swap(for_decryption[8], for_decryption[17]);
	gcry_mpi_swap(for_decryption[9], for_decryption[16]);
	gcry_mpi_swap(for_decryption[10], for_decryption[15]);
	gcry_mpi_swap(for_decryption[11], for_decryption[14]);
	gcry_mpi_swap(for_decryption[12], for_decryption[13]);


	camellia_encryption(text, M, for_decryption);

	delete_round_keys(for_decryption);

}

int test()// проверка 
{
	printf("Test\n");
	gcry_mpi_t key_1_128 = gcry_mpi_new(0); // 1 test из https://tools.ietf.org/html/rfc3713
	gcry_mpi_t Plaintext_1_128 = gcry_mpi_new(0);
	gcry_mpi_t Ciphertext_1_128 = gcry_mpi_new(0);
	gcry_mpi_scan(&key_1_128, GCRYMPI_FMT_HEX, "0123456789abcdeffedcba9876543210", 0, 0);
	gcry_mpi_scan(&Plaintext_1_128, GCRYMPI_FMT_HEX, "0123456789abcdeffedcba9876543210", 0, 0);
	gcry_mpi_scan(&Ciphertext_1_128, GCRYMPI_FMT_HEX, "67673138549669730857065648eabe43", 0, 0);


	printf("https://tools.ietf.org/html/rfc3713\n");
	gcry_mpi_t a = gcry_mpi_new(0); // 1 test из https://tools.ietf.org/html/rfc3713

	gcry_mpi_t keys_for_round[26];

	round_key(key_1_128, keys_for_round);


	gcry_mpi_t test_chip = gcry_mpi_new(0);

	camellia_encryption(&test_chip, Plaintext_1_128,  keys_for_round);

	printf("Открытый текст: \n");
	gcry_mpi_dump(Plaintext_1_128);
	printf("\n");


	if(gcry_mpi_cmp(Ciphertext_1_128 ,test_chip) == 0 ) // если одинаковые, то тест пройден 
	{
		printf("Test passed\n");

		printf("1 эталон, 2 это что вышло\n");
		gcry_mpi_dump(Ciphertext_1_128);
		printf("\n");
		gcry_mpi_dump(test_chip);
		printf("\n");
	}
	else
	{
		perror("Test failed: ");
	}


	camellia_decryption( &a, test_chip,  keys_for_round);

	if(gcry_mpi_cmp(Plaintext_1_128 ,a) == 0 ) // если одинаковые, то тест пройден 
	{
		printf("Test passed\n");

		printf("Получился начальный тект\n");
		gcry_mpi_dump(a);
		printf("\n");
	}
	else
	{
		perror("Test failed not same: ");
	}




	delete_round_keys(keys_for_round);

	return 0; 
}