# ifndef THE1_H
# define THE1_H

# include <cmath>
# include <utility>
# include <vector>


// CHANGES MADE TO THIS FILE ARE DO NOT AFFECT VPL!
// IT IS SHARED WITH YOU SO THAT YOU CAN WORK ON YOUR OWN ENVIRONMENT IF YOU WANT.

std::pair<unsigned short, unsigned int> quick_select3(unsigned short *arr,
                                                      unsigned int size,
                                                      unsigned int index,
                                                      long &swap);


std::pair<unsigned short, unsigned int> select_k_with_quick_sort3(unsigned short * arr,
                                                                  unsigned int size,
                                                                  unsigned int index,
                                                                  long &swap);

# endif