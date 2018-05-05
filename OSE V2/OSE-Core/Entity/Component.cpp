#include "../../stdafx.h"
#include "Component.h"

using namespace ose::entity;

const std::size_t Component::Type = std::hash<std::string>()(TO_STRING(Component));

COMPONENT_DEFINITION(Component, TextureFilter)
COMPONENT_DEFINITION(Component, MeshFilter)
COMPONENT_DEFINITION(Component, MeshRenderer)
COMPONENT_DEFINITION(Component, Material)
