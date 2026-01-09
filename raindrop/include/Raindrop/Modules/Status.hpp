#pragma once

namespace Raindrop{
    enum class Status{

        // The modules has been referenced but not be declared
        UNREGISTRED,

        // The module has just been created
        NEW,

        // has not yet met dependency requirements
        PENDING,

        //The module is currently being initialized
        INITIALIZING,

        // Successfully initialized and ready to be used
        INITIALIZED,

        // The module has been requested to stop further resource creation and let go of it's existing resources
        QUIESCING,

        // The module is fully stopped
        TERMINATED,

        // Tryed to initialized yet failed
        FAILED,
    };
}