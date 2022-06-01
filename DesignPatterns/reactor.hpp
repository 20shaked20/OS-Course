/**
 * @file reactor.hpp
 * @author shaked levi and lior nagar
 * @brief reactor head file.
 * @version 0.1
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 */

/**
 * @brief implemnetd using select or poll -> our choice
 * using one thread or multiple -> user choice.
 *
 */

#include <vector>
#include <stdlib.h>

struct fd_handlers{

    int fd;
    void*func;
    pthread_t temp;

};

class Reactor
{

public:
    
    std::vector<fd_handlers*> _fds;
    
    /**
     * @brief creates a new reactor.
     *
     * @return void*
     */
    friend void *newReactor();

    /**
     * @brief install func to handle input coming through the file descriptor.
     *
     * @param rec -> reactor.
     * @param func -> a function.
     * @param fd -> file descriptor.
     * @return void*
     */
    friend void InstallHandler(Reactor *rec, void *func(void *), int *fd);

    /**
     * @brief removes the func operating over the file descriptor (basically free the fd).
     *
     * @param rec -> reactor.
     * @return void*
     */
    void RemoveHandler(Reactor *rec,int *fd);
};