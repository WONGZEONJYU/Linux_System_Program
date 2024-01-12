#include <thread>
#include <exception>
#include <iostream>
#include <atomic>
#include "queue.hpp"

struct TQueue{
    ListNode head{};
    pthread_mutex_t mutex{};
    std::atomic<int> length{};
};

Queue Queue_Create()
{
    TQueue *ret{};
    try{
        ret = new TQueue;
        pthread_mutexattr_t attr {};
        pthread_mutexattr_init(&attr);
        pthread_mutex_init(&ret->mutex, &attr);
        List_Init(reinterpret_cast<List*>(ret));
        ret->length.store(0);
    }catch(const std::bad_alloc& e){
        std::cerr << e.what() << '\n';
        ret = nullptr;
    }

    return ret;
}

void Queue_Destroy(Queue queue)
{
    auto q {static_cast<TQueue*>(queue)};
    
    pthread_mutex_destroy(&q->mutex);
    
    delete q;
}

int Queue_IsEmpty(Queue queue)
{
    auto q {static_cast<TQueue*>(queue)};
    
    pthread_mutex_lock(&q->mutex);
    
    const auto ret {List_IsEmpty(reinterpret_cast<List*>(q))};
    
    pthread_mutex_unlock(&q->mutex);
    
    return ret;
}

int Queue_IsContained(Queue queue, QueueNode* node)
{
    auto q {static_cast<TQueue*>(queue)};
    
    pthread_mutex_lock(&q->mutex);

    int ret {};
    ListNode* pos {};
    List_ForEach(reinterpret_cast<List*>(q), pos) {
        if( ret = IsEqual(pos, node) ){
            break;
        }
    }

    pthread_mutex_unlock(&q->mutex);
    return ret;
}

void Queue_Add(Queue queue, QueueNode* node)
{
    auto q {static_cast<TQueue*>(queue)};

    pthread_mutex_lock(&q->mutex);
    
    List_AddTail(reinterpret_cast<List*>(q), node);
    
    pthread_mutex_unlock(&q->mutex);

    ++q->length;
}

QueueNode* Queue_Front(Queue queue)
{
    auto q {static_cast<TQueue*>(queue)};

    pthread_mutex_lock(&q->mutex);
    
    auto ret {q->head.next};

    pthread_mutex_unlock(&q->mutex);
    
    return ret;
}

QueueNode* Queue_Remove(Queue queue)
{
    auto q {static_cast<TQueue*>(queue)};
    QueueNode* ret {};
 
    if( q->length > 0 ){
        pthread_mutex_lock(&q->mutex);
        ret = q->head.next;
        List_DelNode(ret);
        pthread_mutex_unlock(&q->mutex);
        --q->length;
    }

    return ret;
}

int Queue_Length(Queue queue)
{
    auto q {static_cast<TQueue*>(queue)};
    
    //pthread_mutex_lock(&q->mutex);
    
    const auto ret {q->length.load()};
    
    //pthread_mutex_unlock(&q->mutex);
    
    return ret;
}

void Queue_Rotate(Queue queue)
{
    auto q {static_cast<TQueue*>(queue)};
    
    //pthread_mutex_lock(&q->mutex);
    
    if( q->length > 0 ){
        auto node {Queue_Remove(q)};
        Queue_Add(q, node);
    }

    //pthread_mutex_unlock(&q->mutex);
}

