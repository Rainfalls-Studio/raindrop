#include "Raindrop/Modules/Filesystem/Providers/FolderProvider.hpp"

#include <filesystem>
#include <cstring>
#include <fstream>

namespace Raindrop::Filesystem{
    class File : public IFile{
        public:
            File(const Path& path, OpenFlags flags){

                std::ios_base::openmode openMode{};

                if (flags & OpenFlags::READ) openMode |= std::ios_base::in;
                if (flags & OpenFlags::WRITE) openMode |= std::ios_base::out;
                if (flags & OpenFlags::BINARY) openMode |= std::ios_base::binary;
                if (flags & OpenFlags::APPEND) openMode |= std::ios_base::app;
                if (flags & OpenFlags::TRUCATE) openMode |= std::ios_base::trunc;
                if (flags & OpenFlags::END) openMode |= std::ios_base::ate;
                if (flags & OpenFlags::EXCLUSIVE) openMode |= std::ios_base::noreplace;


                _stream.open(std::string(path.str()), openMode);
                
                if (!_stream.is_open()){
                    throw std::runtime_error("Failed to open file: " + path.str() + " error : " + std::generic_category().message(errno));
                }
            }

            virtual void read(char* buffer, Size size) override{
                _stream.read(buffer, static_cast<std::streamsize>(size));
            }

            virtual void seek(Offset offset, SeekOrigin origin) override{
                std::ios_base::seekdir seekdir;

                switch (origin){
                    case SeekOrigin::BEGIN: seekdir = std::ios_base::beg; break;
                    case SeekOrigin::CURRENT: seekdir = std::ios_base::cur; break;
                    case SeekOrigin::END: seekdir = std::ios_base::end; break;
                }

                _stream.seekg(static_cast<std::fstream::off_type>(offset), seekdir);
            }

            virtual Size tell() override{
                return static_cast<Size>(_stream.tellg());
            }

            virtual Size size() override{
                auto current = _stream.tellg();
                _stream.seekg(0, std::ios_base::end);
                auto end = _stream.tellg();
                _stream.seekg(current);
                return static_cast<Size>(end);
            }

            virtual void close() override {
                _stream.close();
            }
        
        private:
            std::fstream _stream;
    };

    FolderProvider::FolderProvider(const Path& path) : _root{path}{}

    SharedFile FolderProvider::open(const Path& path, OpenFlags flags){
        return std::make_shared<File>(_root / path, flags);
    }

    FolderProvider::Properties FolderProvider::getProperties() const{
        return Properties{
            std::string("FolderProvider:") + _root.str(),
            false
        };
    }

    bool FolderProvider::exists(const Path& path){
        return ::std::filesystem::exists((_root / path).str());
    }
}