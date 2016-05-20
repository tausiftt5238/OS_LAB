/*
 * **********************************************************************
 * You are free to add anything you think you require to this file
 */
#include "paintshop_driver.h"


int no_of_customers;
static struct semaphore *QSem;
static struct semaphore *bufferBlocker;
static struct semaphore *tintSem[NCOLOURS];
//static struct semaphore *cusSem[NCOLOURS];
static struct queue *buffer;

struct orderCan {
    struct paintcan *can;
    struct semaphore *orderSem;
};
