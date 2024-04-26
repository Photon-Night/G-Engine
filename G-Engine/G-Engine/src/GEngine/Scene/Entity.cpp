#include "gepch.h"
#include "Entity.h"

namespace GEngine {

	Entity::Entity(const std::string& name)
		: m_Name(name), m_Transform(1.0f)
	{

	}

	Entity::~Entity()
	{

	}

}