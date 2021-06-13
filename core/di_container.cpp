# include "di_container.h"

namespace zx
{
	std::unordered_map<std::string, di_container *> di_container::__instances;

	di_container::di_container(const std::string& name) :
		_name(name)
	{
		_singleton_container = new zx::singleton_container(this);
		_nomination_container = new zx::nomination_container(this);
		__instances.insert(std::make_pair(name, this));
	}


	di_container::~di_container()
	{
		delete _singleton_container;
		delete _nomination_container;

		const auto it = __instances.find(this->_name);
		if (it != __instances.end())
		{
			__instances.erase(it);
		}
	}
}
