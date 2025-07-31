#pragma once

namespace Raindrop::Modules{
    enum class Status{

        // The modules has been referenced but not be declared
        UNREGISTRED,

        // has not yet met dependency requirements
        PENDING,

        // Tryed to initialized yet failed
        FAILED,

        // Successfully initialized and ready to be used
        INITIALIZED,
    };
}