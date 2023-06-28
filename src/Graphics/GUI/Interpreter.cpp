#include <Raindrop/Graphics/GUI/Interpreter.hpp>
#include <Raindrop/Graphics/GUI/Item.hpp>

namespace Raindrop::Graphics::GUI{
	Interpreter::Interpreter(Core::EngineContext &context) : _context{context}{
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.GUI");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");
		subscribeEvents();
	}

	Interpreter::~Interpreter(){}

	void Interpreter::update(){
		auto it = _items.begin();
		while (it != _items.end()){
			if (it->get()->update()){
				_items.erase(it);
			}
			it++;
		}
	}

	void Interpreter::clear(){
		_items.clear();
	}

	void Interpreter::parse(const std::filesystem::path& path, Item* root){
		auto& registry = _context.registry;
		auto& eventManager = _context.eventManager;

		CLOG(INFO, "Engine.Graphics.GUI") << "Loading GUI... : " << path;
		tinyxml2::XMLDocument document;

		if (document.LoadFile(path.u8string().c_str()) != tinyxml2::XML_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.GUI") << "Failed to open " << path << ". Reason : " << document.ErrorStr();
			throw std::runtime_error("failed to open xml file");
		}
		
		if (root == nullptr){
			for (tinyxml2::XMLElement* child = document.FirstChildElement(); child != nullptr; child = child->NextSiblingElement()){
				_items.push_back(Item::create(child, registry, eventManager));
			}
		} else {
			for (tinyxml2::XMLElement* child = document.FirstChildElement(); child != nullptr; child = child->NextSiblingElement()){
				root->_childs.push_back(Item::create(child, registry, eventManager));
			}
		}
		CLOG(INFO, "Engine.Graphics.GUI") << "GUI Loaded with success !";
	}

	void Interpreter::subscribeEvents(){
		auto& registry = _context.registry;
		auto& eventManager = _context.eventManager;

		eventManager.subscribe("Engine.GUI.Open", [&](){
			parse(registry["Engine.GUI.OpenPath"].as<std::string>(), reinterpret_cast<Item*>(registry["Engine.GUI.OpenRoot"].as<void*>()));
		});
	}
}