#pragma once
#include "vector"
#include "myve_vertex_data.hpp"
#include "myve_texture.hpp"

namespace myve
{
	struct ModelData {

		std::vector<VertexData> vertices;
		std::vector<uint32_t> indices;

	};

	class ResourceLoader
	{
	public:
		static ModelData loadOBJ(const std::string& path);
	private:
	};
}