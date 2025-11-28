project(Asset
    LANGUAGES CXX
    VERSION 0.1.0
    DESCRIPTION "Simple asset manager with factories and async friendly loading" 
)

module_dependencies(
    SOFT
        Event
)

# create library
add_raindrop_module()