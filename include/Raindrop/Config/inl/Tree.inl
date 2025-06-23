#pragma once

#include "../Tree.hpp"
#include "Raindrop/Core/Internal/Config/Tree.hpp"

namespace Raindrop::Core::Config{
    inline Tree::Tree() : _impl{}{}
    inline Tree::Tree(std::shared_ptr<Core::Internal::Config::Tree> impl) : _impl{impl}{}

    inline bool Tree::isValid() const noexcept{
        return _impl != nullptr;
    }

    inline void Tree::parseFile(const std::string& path){
        assertGet().parseFile(path);
    }

    inline void Tree::parseStr(const std::string& string){
        assertGet().parseStr(string);
    }

    inline Node Tree::getRoot() noexcept{
        return assertGet().getRoot();
    }

    inline const Node Tree::getRoot() const noexcept{
        return assertGet().getRoot();
    }

    inline Node Tree::operator[](const Tag& tag){
        return assertGet().getNode(tag);
    }

    inline const Node Tree::operator[](const Tag& tag) const{
        return assertGet().getNode(tag);
    }

    inline Node Tree::getNode(const Tag& tag){
        return assertGet().getNode(tag);
    }

    inline const Node Tree::getNode(const Tag& tag) const{
        return assertGet().getNode(tag);
    }

    inline std::shared_ptr<Core::Internal::Config::Tree> Tree::get(){
        return _impl;
    }

    inline Core::Internal::Config::Tree& Tree::assertGet(){
        Core::Internal::Config::Tree* ptr = _impl.get();
        assert(ptr != nullptr);
        return *ptr;
    }

    inline const Core::Internal::Config::Tree& Tree::assertGet() const{
        Core::Internal::Config::Tree* ptr = _impl.get();
        assert(ptr != nullptr);
        return *ptr;
    }
}