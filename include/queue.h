//
//  queue.h
//  aztec
//
//  Created by Alexander Warsewa on 09.01.16.
//
//

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <avr/io.h>
#include <inttypes.h>
#include <stdio.h>

typedef struct {
    uint8_t *data;
    uint16_t head;
    uint16_t tail;
    uint16_t length;
    uint16_t entrySize;
    uint16_t capacity;
} queue;

void    queue_init(queue *q, uint8_t *data, uint8_t entrySize, uint8_t capacity);
void    queue_push(queue *q, uint8_t *data);
void    queue_pop(queue *q, uint8_t *data);
uint8_t queue_isempty(queue *q);

#endif

