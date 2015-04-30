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

#ifndef _CMD_H_
#define _CMD_H_

#include <pthread.h>
#include <stdint.h>

#include "client.h"
#include "mmio.h"
#include "parms.h"
#include "psl_interface.h"

enum cmd_type {
	CMD_READ,
	CMD_WRITE,
	CMD_TOUCH,
	CMD_INTERRUPT,
	CMD_OTHER
};

enum mem_state {
	MEM_IDLE,
	MEM_BUFFER,
	MEM_REQUEST,
	MEM_RECEIVED,
	MEM_DONE
};

struct cmd_event {
	uint64_t addr;
	uint32_t context;
	uint32_t tag;
	uint32_t size;
	uint32_t resp;
	uint8_t unlock;
	uint8_t restart;
	uint8_t *data;
	uint8_t *parity;
	enum cmd_type type;
	enum mem_state state;
	struct cmd_event *_next;
};

struct cmd {
	struct AFU_EVENT *afu_event;
	struct cmd_event *list;
	struct cmd_event *buffer_read;
	struct mmio *mmio;
	struct parms *parms;
	struct client *client;
	volatile enum pslse_state *psl_state;
	pthread_mutex_t *psl_lock;
	pthread_mutex_t lock;
	uint64_t lock_addr;
	uint64_t res_addr;
	uint32_t credits;
	uint16_t irq;
	uint8_t locked;
	uint8_t flushing;
};

struct cmd *cmd_init(struct AFU_EVENT *afu_event, struct parms *parms,
		     struct mmio *mmio, volatile enum pslse_state *state,
		     pthread_mutex_t *lock);

void handle_cmd(struct cmd* cmd, uint32_t parity_enabled, uint32_t latency);

void handle_buffer_read(struct cmd *cmd);

void handle_buffer_data(struct cmd *cmd);

void handle_buffer_write(struct cmd *cmd);

void handle_touch(struct cmd *cmd);

void handle_interrupt(struct cmd *cmd);

void handle_mem_return(struct cmd_event *event, int fd, pthread_mutex_t *lock);

void handle_aerror(struct cmd_event *event);

void handle_response(struct cmd *cmd);

int client_cmd(struct cmd *cmd, uint32_t context);

#endif /* _CMD_H_ */
