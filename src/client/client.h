#ifndef CLIENT_H
#define CLIENT_H

#include "core/types.h"

// --- type definitions -------------------------------------------------------

/**
 * Exit codes for ``main` to return depending on the state of the application
 */
typedef enum Chip8ExitCode
{
    C8_SUCCESS,
    C8_FAILURE, 
} Chip8ExitCode;

/**
 * This struct is a handle to the client
 */
typedef struct Chip8Client Chip8Client;

// --- client interface -------------------------------------------------------

/**
 * Initializes the window, sound and emulator and returns a handle to the
 * initialized client
 *
 * @param[in] argc
 *      The number of arguments passed via the command-line
 * @param[in] argv
 *      The arguments passed via the command-line
 *
 * @return
 *      A handle to the initialized client or `NULL` if something went wrong
 */
Chip8Client *c8InitClient(i32 argc, char **argv);

/**
 * Runs the client and returns once the client has been prompted to close by
 * the user
 *
 * @param[in] client
 *      The handle to the client
 */
void c8RunClient(Chip8Client *client);

/**
 * Cleans up all resources used by the client and closes the application
 *
 * @param[in] client
 *      The handle to the client
 */
void c8CloseClient(Chip8Client *client);

#endif /* CLIENT_H */
