/**
 * @file reactor.cpp
 * @author shaked levi & lior nagar
 * @brief implemnation of the class for the reactor
 * @version 0.1
 * @date 2022-05-27
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "reactor.hpp"
#include <cstdlib>
#include <pthread.h>


void InstallHandler(Reactor *rec, void *func(void *), int *fd)
{

    for (size_t i = 0; i < rec->_fds.size(); ++i)
    {
        if (rec->_fds.at(i)->fd == *fd)
        {
            /*case where we already have this fd,
             * we will simply change the handler.
             */

            rec->_fds.at(i)->func = &func;
        }
    }
    /*else: create a new fd handler and insert to vector*/
    struct fd_handlers *assign = (struct fd_handlers *)malloc(sizeof(struct fd_handlers));
    assign->fd = *fd;
    assign->func = &func;
    assign->temp = 0;
    pthread_create(&assign->temp,NULL,func,(void*)fd); /*creating the thread to handle the function*/
    rec->_fds.push_back(assign);
    
}

void Reactor::RemoveHandler(Reactor *rec, int *fd)
{

    for (size_t i = 0; i < rec->_fds.size(); ++i)
    {
        if (rec->_fds.at(i)->fd == *fd)
        {
            /*case where we already have this fd,
             * we will simply NULL the func.
             */

            rec->_fds.at(i)->func = NULL; /*set func to null*/
            pthread_cancel(rec->_fds.at(i)->temp);/*killing the thread*/
        }
    }
}