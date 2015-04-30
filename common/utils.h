/*
 * Copyright 2014,2015 International Business Machines
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>
#include <stdio.h>

#ifdef DEBUG
#define DPRINTF(...) printf(__VA_ARGS__)
#else
#define DPRINTF(...)
#endif

#define MAX_LINE_CHARS 1024

#define ODD_PARITY 1
#define BYTES_PER_DWORD 8
#define DWORDS_PER_CACHELINE 16
#define CACHELINE_BYTES 128

#define PSLSE_ATTACH       0x01
#define PSLSE_DETACH       0x02
#define PSLSE_MEMORY_READ  0x03
#define PSLSE_MEMORY_WRITE 0x04
#define PSLSE_MEMORY_TOUCH 0x05
#define PSLSE_MEM_SUCCESS  0x06
#define PSLSE_MEM_FAILURE  0x07
#define PSLSE_MMIO_MAP     0x08
#define PSLSE_MMIO_READ64  0x09
#define PSLSE_MMIO_WRITE64 0x0A
#define PSLSE_MMIO_READ32  0x0B
#define PSLSE_MMIO_WRITE32 0x0C
#define PSLSE_MMIO_ACK     0x0D
#define PSLSE_MMIO_FAIL    0x0E
#define PSLSE_INTERRUPT    0x0F

// PSLSE states
enum pslse_state {
	PSLSE_IDLE,
	PSLSE_RESET,
	PSLSE_DESC,
	PSLSE_PENDING,
	PSLSE_RUNNING,
	PSLSE_DONE
};

// Usage message
void usage(int argc, char **argv);

// Display fatal message (For catching code bugs, not AFU bugs)
void fatal_msg(char *format, ...);

// Display error message
void error_msg(char *format, ...);

// Display error message
void warn_msg(char *format, ...);

// Display error message
void info_msg(char *format, ...);

// Delay for up to ns nanoseconds
void ns_delay(long ns);

// Allocate memory for data and get size bytes from fd
uint8_t * get_bytes(int fd, unsigned size, int timeout);

// Put bytes on socket and return number of bytes successfully written
int put_bytes(int fd, unsigned size, uint8_t *data, int timeout);

// Generate parity for outbound data and checking inbound data
// 1 bit of parity for up to 64 bits of data
uint8_t generate_parity(uint64_t data, uint8_t odd);

// Generate parity for entire cacheline
// 1 bit of parity for each 64 bits of data
void generate_cl_parity(uint8_t * data, uint8_t * parity);

#endif /* _UTILS_H_ */
