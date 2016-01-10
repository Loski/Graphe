/* Very simple queue
 * These are FIFO queues which discard the new data when full.
 *
 * Queue is empty when in == out.
 * If in != out, then
 *  - items are placed into in before incrementing in
 *  - items are removed from out before incrementing out
 * Queue is full when in == (out-1 + QUEUE_SIZE) % QUEUE_SIZE;
 *
 * The queue will hold QUEUE_ELEMENTS number of items before the
 * calls to QueuePut fail.
 */

/* Queue structure */


typedef struct fifo{
    int * Queue;
    int QueueIn, QueueOut, QUEUE_ELEMENTS, QUEUE_SIZE;
}fifo;


fifo QueueInit(int nb)
{
    fifo file;
    file.Queue = malloc(size_t(int) * nb);
    file.QueueIn = file.QueueOut = 0;
    file.Q
}

int QueuePut(int new_o)
{
    if(QueueIn == (( QueueOut - 1 + QUEUE_SIZE) % QUEUE_SIZE))
    {
        return -1; /* Queue Full*/
    }
    Queue[QueueIn] = new_o;
    QueueIn = (QueueIn + 1) % QUEUE_SIZE;
    return 0; // No errors
}

int QueueGet(int *old)
{
    if(QueueIn == QueueOut)
    {
        return -1; /* Queue Empty - nothing to get*/
    }
    *old = Queue[QueueOut];
    QueueOut = (QueueOut + 1) % QUEUE_SIZE;
    return 0; // No errors
}
