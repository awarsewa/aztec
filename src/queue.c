//
//  queue.c
//  aztec
//
//  Created by Alexander Warsewa on 09.01.16.
//
//

#include "queue.h"

void queue_init(queue *q, uint8_t *data, uint8_t entrySize, uint8_t capacity)
{
    q->data = data;
    q->head = q->tail = 0;
    q->entrySize = entrySize;
    q->capacity = capacity;
    q->length = 0;
}

void queue_push(queue *q, uint8_t *data)
{
    uint16_t i, j = 0;
    
    if(q->length < q->capacity){
        for(i = q->tail; i < (q->tail + q->entrySize); i++){
            q->data[i] = data[j];
            j++;
        }

        q->tail = (q->tail + q->entrySize);
        if(q->tail >= (q->capacity * q->entrySize))
            q->tail = 0; 

        ++(q->length);
    }
}

void queue_pop(queue *q, uint8_t *data)
{
    uint16_t i, j = 0;

    if(q->length){
        for(i = q->head; i < (q->head + q->entrySize); i++){
            data[j] = q->data[i];
            j++;
        }
        q->head = (q->head + q->entrySize);
        if(q->head >= (q->capacity * q->entrySize))
            q->head = 0;
        --(q->length);

    }    
}

uint8_t queue_isempty(queue *q)
{
    if(q->length)
        return 0;
    else return 1;
}

