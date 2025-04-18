#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "fmod_errors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define EVENT_PATH_MAX_LEN 1024

static void panic_on_err(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        fprintf(stderr, "panic! error code: %d (%s)\n", result, FMOD_ErrorString(result));
        exit(1);
    }
}

static void help(FILE* f) {
    fprintf(f, "Usage: fevlist [--verbose|-v] [--fmod-debug|-d] <BANK_PATH>\n");
    fprintf(f, "Example: fevlist MyBank.bank\n");
}

int main(int argc, const char** argv) {
    if (argc < 2) {
        help(stderr);
        return 1;
    }

    int verbose = 0;
    int fmod_debug = 0;
    const char* bank_path = NULL;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = 1;
        } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--fmod-debug") == 0) {
            fmod_debug = 1;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            help(stdout);
            return 0;
        } else {
            bank_path = argv[i];
        }
    }

    if (!bank_path) {
        fprintf(stderr, "Error: No bank path provided\n");
        help(stderr);
        return 1;
    }

#ifdef DEBUG
    if (!fmod_debug) {
        panic_on_err(FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_NONE, FMOD_DEBUG_MODE_TTY, nullptr, nullptr));
    }
#endif

    if (verbose) printf("Loading bank: %s\n", bank_path);

    FMOD::Studio::System* system = nullptr;
    panic_on_err(FMOD::Studio::System::create(&system));

    panic_on_err(system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr));

    FMOD::Studio::Bank* bank = nullptr;
    panic_on_err(system->loadBankFile(bank_path, FMOD_STUDIO_LOAD_BANK_NORMAL, &bank));
    panic_on_err(bank->loadSampleData());
    panic_on_err(system->flushSampleLoading());

    int event_count = 0;
    panic_on_err(bank->getEventCount(&event_count));

    if (verbose) printf("Found %d events in bank\n", event_count);

    FMOD::Studio::EventDescription** events = new FMOD::Studio::EventDescription*[event_count];
    panic_on_err(bank->getEventList(events, event_count, nullptr));

    for (int i = 0; i < event_count; ++i) {
        char path[EVENT_PATH_MAX_LEN] = {0};
        FMOD_GUID id;

        FMOD::Studio::EventDescription* event = events[i];
        FMOD_RESULT path_result = event->getPath(path, EVENT_PATH_MAX_LEN, nullptr);
        event->getID(&id); // ignore errors here; still useful to try

        if (path_result == FMOD_OK) {
            printf("%s\n", path);
        } else {
            // fallback if no string table present
            printf("{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}\n",
                id.Data1, id.Data2, id.Data3,
                id.Data4[0], id.Data4[1],
                id.Data4[2], id.Data4[3], id.Data4[4], id.Data4[5], id.Data4[6], id.Data4[7]);
        }
    }

    delete[] events;

    panic_on_err(system->unloadAll());
    panic_on_err(system->flushCommands());
    panic_on_err(system->release());

    if (verbose) printf("Done.\n");
    return 0;
}

