#include "utils/architecture.h"
#include "core/mainloop.h"
#include "utils/debug_log.h"

int main(int argc, char *argv[])
{
    DEBUG_LOG("Starting Twisted Fables game");

    // Initialize logging system
    if (!init_debug_log())
    {
        fprintf(stderr, "Failed to initialize debug logging\n");
        return EXIT_FAILURE;
    }

    // Start the main game loop
    int result = mainloop(argc, argv);

    // Cleanup
    cleanup_debug_log();
    DEBUG_LOG("Game terminated with code: %d", result);

    return result;
}
