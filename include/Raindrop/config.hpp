#pragma once

#ifdef RAINDROP_MMOCK
    #define RAINDROP_MOCK_VIRTUAL virtual
    #define RAINDROP_MOCK_OVERRIDE override
#else
    #define RAINDROP_MOCK_VIRTUAL
    #define RAINDROP_MOCK_OVERRIDE
#endif