/**
 * @file queue.c
 * @brief Implementation of a queue that supports FIFO and LIFO operations.
 *
 * This queue implementation uses a singly-linked list to represent the
 * queue elements. Each queue element stores a string value.
 *
 * Assignment for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Extended to store strings, 2018
 *
 * TODO: fill in your name and Andrew ID
 * @author XXX <XXX@andrew.cmu.edu>
 */

#include "queue.h"
#include "harness.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Allocates a new queue
 * @return The new queue, or NULL if memory allocation failed
 */
queue_t *queue_new(void) {
    queue_t *q = malloc(sizeof(queue_t));
    /* What if malloc returned NULL? */
    if (q == NULL) {
        free(q);
        return NULL;
    }

    q->size = (size_t)0;
    q->head = NULL;
    q->tail = NULL;
    return q;
}

/**
 * @brief Frees all memory used by a queue
 * @param[in] q The queue to free
 */
void queue_free(queue_t *q) {
    /* How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (q == NULL) {
        free(q);
        return;
    }
    list_ele_t *current = q->head;
    while (current != NULL) {
        free(current->value);
        current = current->next;
        free(q->head);
        q->head = current;
    }
    free(q);
    return;
}

/**
 * @brief Attempts to insert an element at head of a queue
 *
 * This function explicitly allocates space to create a copy of `s`.
 * The inserted element points to a copy of `s`, instead of `s` itself.
 *
 * @param[in] q The queue to insert into
 * @param[in] s String to be copied and inserted into the queue
 *
 * @return true if insertion was successful
 * @return false if q is NULL, or memory allocation failed check
 */
bool queue_insert_head(queue_t *q, const char *s) {
    if (q == NULL)
        return false; // failsafe for NULL q
    if (s == NULL)
        return false; // trace 06?
    // make new elem
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL) { // malloc failsafe
        free(newh);
        return false;
    }
    // alloc + copy string
    size_t len = strlen(s); // O(n)
    char *copy = (char *)(malloc(sizeof(char) * (len + 1)));
    if (copy == NULL) { // malloc failsafe
        free(copy);
        free(newh);
        return false;
    }
    strcpy(copy, s);
    newh->value = (char *)copy; // set newh value to copied string
    // If q is empty, make the head also the tail
    if ((q->head == NULL) && (q->tail == NULL) && (q->size == 0)) {
        q->tail = newh;
        q->head = newh;
        newh->next = NULL;
        q->size += (size_t)1;
        return true;
    }
    // filled q case
    newh->next = q->head;
    q->head = newh;
    q->size += (size_t)1;
    return true;
}

/**
 * @brief Attempts to insert an element at tail of a queue
 *
 * This function explicitly allocates space to create a copy of `s`.
 * The inserted element points to a copy of `s`, instead of `s` itself.
 *
 * @param[in] q The queue to insert into
 * @param[in] s String to be copied and inserted into the queue
 *
 * @return true if insertion was successful
 * @return false if q is NULL, or memory allocation failed
 */
bool queue_insert_tail(queue_t *q, const char *s) {
    if (q == NULL)
        return false; // NULL q failsafe
    if (s == NULL)
        return false; // trace 06?
    // create new elem
    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));
    if (newt == NULL) { // malloc failsafe
        free(newt);
        return false;
    }
    // alloc + copy string
    size_t len = strlen(s); // O(n)
    char *copy = (char *)(malloc(sizeof(char) * (len + 1)));
    if (copy == NULL) { // malloc failsafe
        free(copy);
        free(newt);
        return false;
    }
    strcpy(copy, s);
    newt->value = (char *)copy;
    // Empty q case
    if ((q->head == NULL) && (q->tail == NULL) && (q->size == 0)) {
        q->head = newt;
        q->tail = newt;
        q->size += (size_t)1;
        newt->next = NULL;
        return true;
    }
    // move tail and incr size
    q->tail->next = newt;
    q->tail = newt;
    newt->next = NULL;
    q->size += (size_t)1;
    /* Remember: It should operate in O(1) time */
    return true;
}

/**
 * @brief Attempts to remove an element from head of a queue
 *
 * If removal succeeds, this function frees all memory used by the
 * removed list element and its string value before returning.
 *
 * If removal succeeds and `buf` is non-NULL, this function copies up to
 * `bufsize - 1` characters from the removed string into `buf`, and writes
 * a null terminator '\0' after the copied string.
 *
 * @param[in]  q       The queue to remove from
 * @param[out] buf     Output buffer to write a string value into
 * @param[in]  bufsize Size of the buffer `buf` points to
 *
 * @return true if removal succeeded
 * @return false if q is NULL or empty
 */
bool queue_remove_head(queue_t *q, char *buf, size_t bufsize) {
    // check q reqs
    if (q == NULL)
        return false;
    if (q->head == NULL)
        return false;
    // Buff things--only if buf is non-NULL
    if (buf != NULL) {
        size_t len = (bufsize / (sizeof(char)));
        strncpy(buf, q->head->value, len - 1);
        buf[len - 1] = '\0';
    }
    // removal + freeing
    if (q->size == 1) { // 1 elem case
        free(q->head->value);
        free(q->head);
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
        return true;
    }
    list_ele_t *storelocation = q->head->next;
    free(q->head->value);
    free(q->head);
    q->head = storelocation;
    q->size -= (size_t)1;
    return true;
}

/**
 * @brief Returns the number of elements in a queue
 *
 * This function runs in O(1) time.
 *
 * @param[in] q The queue to examine
 *
 * @return the number of elements in the queue, or
 *         0 if q is NULL or empty
 */
size_t queue_size(queue_t *q) {
    /* You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    if (q == NULL)
        return 0;
    return q->size;
}

/**
 * @brief Reverse the elements in a queue
 *
 * This function does not allocate or free any list elements, i.e. it does
 * not call malloc or free, including inside helper functions. Instead, it
 * rearranges the existing elements of the queue.
 *
 * @param[in] q The queue to reverse
 */
void queue_reverse(queue_t *q) {
    // if q empty or NULL there is nothing to reverse
    if (q == NULL)
        return;
    else if (q->head == NULL)
        return;
    list_ele_t *current = q->head;
    list_ele_t *prev = NULL;
    list_ele_t *nextStep; // named to differentiate from ->next
    // store old head n tail locations to switch later
    list_ele_t *oldHead = q->head;
    list_ele_t *oldTail = q->tail;
    while (current != NULL) {
        nextStep = current->next; // store nextStep
        current->next = prev;     // reverse ptr
        // set vals for next iteration
        prev = current;
        current = nextStep;
    }
    q->tail = oldHead;
    q->head = oldTail;
    return;
}
