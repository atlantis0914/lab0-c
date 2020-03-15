#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* TODO: What if malloc returned NULL? */
    if (q) {
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
    }
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (!q) {
        return;
    }

    while (q->head) {
        list_ele_t *tmp = q->head->next;
        free(q->head->value);
        free(q->head);
        q->head = tmp;
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    /* TODO: What should you do if the q is NULL? */
    if (!q) {
        return false;
    }

    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        return false;
    }

    size_t len = strlen(s);
    newh->value = (char *) malloc(len + 1);
    if (!newh->value) {
        free(newh);
        return false;
    }

    strncpy(newh->value, s, len);
    newh->value[len] = '\0';
    newh->next = q->head;
    q->head = newh;
    q->size++;
    /* If this is the first insert head operation, initialize q->tail as well */
    if (!q->tail) {
        q->tail = newh;
    }
    return true;
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q) {
        return false;
    }

    list_ele_t *newt = malloc(sizeof(list_ele_t));
    if (!newt) {
        return false;
    }

    size_t len = strlen(s);
    newt->value = (char *) malloc(len + 1);
    if (!newt->value) {
        free(newt);
        return false;
    }

    strncpy(newt->value, s, len);
    newt->value[len] = '\0';
    newt->next = NULL;
    q->size++;
    /* If this is the first it operation, initialize q->head as well */
    if (q->head == NULL) {
        q->head = newt;
    } else {
        q->tail->next = newt;
    }
    q->tail = newt;
    return true;
    /* TODO: Remove the above comment when you are about to implement. */
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head) {
        return false;
    }

    list_ele_t *tmp = q->head;
    q->head = tmp->next;
    tmp->next = NULL;

    if (sp) {
        size_t len = strlen(tmp->value);
        if (len > bufsize) {
            len = bufsize - 1;
        }
        strncpy(sp, tmp->value, len);
        sp[len] = '\0';
    }
    free(tmp->value);
    free(tmp);
    q->size--;
    return true;
    /* TODO: Remove the above comment when you are about to implement. */
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return q ? q->size : 0;
    /* TODO: Remove the above comment when you are about to implement. */
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q == NULL || q->head == NULL) {
        return;
    }

    list_ele_t *tmp, *cur;
    q->tail = q->head;
    tmp = q->head->next;
    while (tmp) {
        cur = tmp->next;
        tmp->next = q->head;
        q->head = tmp;
        tmp = cur;
    }
    q->tail->next = NULL;
    /* TODO: Remove the above comment when you are about to implement. */
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
list_ele_t *q_sort_merge(list_ele_t *a, list_ele_t *b)
{
    list_ele_t *head = NULL, **tail;
    tail = &head;

    while (1) {
        if (a == NULL) {
            (*tail) = b;
            break;
        }
        if (b == NULL) {
            (*tail) = a;
            break;
        }
        if (strcmp(a->value, b->value) < 0) {
            (*tail) = a;
            a = a->next;
        } else {
            (*tail) = b;
            b = b->next;
        }
        tail = &((*tail)->next);
    }
    return head;
}

void q_sort(queue_t *q)
{
    if (q == NULL || q->head == NULL || q->head->next == NULL) {
        return;
    }

    list_ele_t *fast = q->head->next;
    list_ele_t *slow = q->head;

    queue_t f, s;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    fast = slow->next;
    slow->next = NULL;
    f.head = fast;
    s.head = q->head;

    q_sort(&s);
    q_sort(&f);

    q->head = q_sort_merge(s.head, f.head);
    /* TODO: Remove the above comment when you are about to implement. */
}
