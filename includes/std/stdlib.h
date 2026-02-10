// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//           /======================\\`
//           | /==================\`||;
//           | |        ||        |;||;
//           | |   ,,   ||   ,,   |;||;
//           | |   ||   ||   ||   |;||;
//           | |   ||   ||   ||   |;||;
//           | |   ||   ``   ||   |;||;
//           | |   ||        ||   |;||;
//           | \––––––––––––––––––/´||;
//           \––––––––––––––––––––––//,
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       COPYRIGHT (c) 2025 MYTHOS PROJECT
//              ALL RIGHTS RESERVED
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef _STD_H_
#define _STD_H_

#include <type.h>

void itoa(uint32_t value, char **buf_ptr_addr, uint8_t base);
void itoa64(uint64_t value, char **buf_ptr_addr, uint8_t base);
int uitoa(uint32_t value, char *buf, uint8_t base);
int uitoa64(uint64_t value, char *buf, uint8_t base);

#endif