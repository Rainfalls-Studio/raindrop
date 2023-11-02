#include <Raindrop/Core/Registry/Node.hpp>
#include <Raindrop/Core/Registry/Context.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

namespace Raindrop::Core::Registry{
	struct Node::Impl{
		pt::ptree& tree;

		Impl(pt::ptree& tree) : tree{tree}{}
	};

	Node::Node(const Node& node) : _context{node._context}{
		_impl = std::make_unique<Impl>(node._impl->tree); 
	}

	Node::Node(Context& context, Impl impl) : _context{context}{
		_impl = std::make_unique<Impl>(impl);
	}

	Node::Node(Context& context) : _context{context}{}

	Node::~Node(){
		_impl.reset();
	}
	
	bool Node::same(const Node& other) const{
		return _impl->tree == other._impl->tree;
	}

	bool Node::different(const Node& other) const{
		return _impl->tree != other._impl->tree;
	}

	void Node::load(const std::filesystem::path& path){
		_context.logger().info("Loading registry file from %s", path.string().c_str());

		try{
			boost::property_tree::read_json(path.string(), _impl->tree);
		} catch (const boost::property_tree::json_parser_error& e){
			_context.logger().error("Failed to load registry from %s : %s", path.string().c_str(), e.what());
			return;
		}

		_context.logger().info("Loaded %s with success !", path.string().c_str());
	}

	void Node::save(const std::filesystem::path& path){
		_context.logger().info("Saving registry file at %s", path.string().c_str());

		try{
			boost::property_tree::write_json(path.string(), _impl->tree);
		} catch (const boost::property_tree::json_parser_error& e){
			_context.logger().error("Failed to save registry at %s : %s", path.string().c_str(), e.what());
			return;
		}
		
		_context.logger().info("Saved %s with success !", path.string().c_str());
	}

	std::string Node::format(const std::string& str){
		std::string result = str;
		size_t pos = 0;
		while ((pos = result.find("${", pos)) != std::string::npos) {
			size_t endPos = result.find("}", pos);
			if (endPos != std::string::npos) {
				std::string placeholder = result.substr(pos + 2, endPos - pos - 2);
				try {
					std::string path = placeholder;
					std::replace(path.begin(), path.end(), '.', '/');
					int value = _impl->tree.get<int>(path);
					result.replace(pos, endPos - pos + 1, std::to_string(value));
					pos += std::to_string(value).length();

				} catch (const pt::ptree_error&) {
					pos = endPos + 1;
				}
			} else {
				pos += 2;
			}
		}
		return result;
	}

	Node Node::get(const std::string& path){
		return Node(_context, _impl->tree.get_child(path));
	}

	const Node Node::get(const std::string& path) const{
		return Node(_context, _impl->tree.get_child(path));
	}

	void Node::remove(const std::string& path){
		_impl->tree.erase(path);
	}
	
	template<typename T>
	T& Node::as() const{
		return _impl->tree.get<T&>();
	}

	void Node::sort(){
		_impl->tree.sort();
	}

	void Node::revers(){
		_impl->tree.reverse();
	}

	void Node::clear(){
		_impl->tree.clear();
	}

	uint32_t Node::size() const{
		return static_cast<uint32_t>(_impl->tree.size());
	}

	bool Node::empty() const{
		return _impl->tree.empty();
	}
}