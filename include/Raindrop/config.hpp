#pragma once

#ifdef RAINDROP_MMOCK
    #define RDP_MOCK_VIRTUAL virtual
    #define RDP_MOCK_OVERRIDE override
#else
    #define RDP_MOCK_VIRTUAL
    #define RDP_MOCK_OVERRIDE
#endif