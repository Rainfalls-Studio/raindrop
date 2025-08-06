#include "Raindrop/Modules/Filesystem/MountTable.hpp"

namespace Raindrop::Filesystem{
    MountTable::MountTable(){}
    MountTable::~MountTable(){}

    struct MountComparator {
        bool operator()(const MountTable::Entry& a, const MountTable::Entry& b) const {
            if (a.root.size() != b.root.size())
                return a.root.size() > b.root.size(); // longer roots first
            return a.priority > b.priority;               // higher priorities first
        }
    };

    void MountTable::mount(const Path& root, SharedProvider provider, int priority){
        IProvider::Properties properties = provider->getProperties();
        
        Entry entry{
            .root = root,
            .provider = provider,
            .priority = priority,
            .readOnly = properties.readOnly
        };

        auto it = std::lower_bound(_mounts.begin(), _mounts.end(), entry, MountComparator{});
        _mounts.insert(it, entry);
    }

    bool MountTable::exists(const Path& path) const{
        auto mounts = findMatchingMounts(path);
        if (mounts.empty()) return false;

        for (auto* mount : mounts) {
            Path relativePath = path.relativeTo(mount->root);
            if (mount->provider->exists(relativePath)){
                return true;
            }
        }

        return false;
    }

    SharedFile MountTable::open(const Path& path, OpenFlags flags) const{
        auto mounts = findMatchingMounts(path);
        for (auto* mount : mounts) {
            Path relativePath = path.relativeTo(mount->root);
            if (mount->provider->exists(relativePath)) {
                auto file = mount->provider->open(relativePath, flags);
                if (file) return file;
            }
        }
        return nullptr;
    }

    const MountTable::MountList& MountTable::mounts(){
        return _mounts;
    }

    std::vector<const MountTable::Entry*> MountTable::findMatchingMounts(const Path& virtualPath) const{
        std::vector<const Entry*> result;

        for (const auto& mount : _mounts) {
            if (mount.root.isPrefixOf(virtualPath)) {
                result.push_back(&mount);
            }
        }

        return result; // Already sorted by design
    }
}