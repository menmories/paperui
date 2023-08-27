#include "paper_builder.h"
#include "pugiconfig.hpp"
#include "pugixml.hpp"
#include <string>
struct paper_builder* paper_builder_load(struct paper_render* render, const char* filename, ELoadType type)
{
    if (type == Load_XML)
    {
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(filename);
		FILE* pFIle = fopen(filename, "r");
		if (result.status != pugi::xml_parse_status::status_ok)
		{
			return nullptr;
		}
		struct paper_builder* builder = (struct paper_builder*)malloc(sizeof(struct paper_builder));
		if (!builder)
		{
			return nullptr;
		}

		pugi::xml_node node_ui = doc.root().child("UI");
		pugi::xml_node node_head = node_ui.child("Head");
		pugi::xml_node node_body = node_ui.child("Body");
		builder->x = node_body.attribute("x").as_uint();
		builder->y = node_body.attribute("y").as_uint();
		builder->width = node_body.attribute("width").as_uint();
		builder->height = node_body.attribute("height").as_uint();

		pugi::xml_node node_image = node_body.child("Img");
		std::string src = node_image.attribute("src").as_string();


		builder->image = paper_image_load_from_file(render, src.c_str());
		if (!builder->image)
		{
			fprintf(stderr, "加载图片文件失败，文件名：%s\n", src.c_str());
		}
		builder->image_rect.left = node_image.attribute("x").as_int();
		builder->image_rect.top = node_image.attribute("y").as_int();
		builder->image_rect.right = node_image.attribute("width").as_int();
		builder->image_rect.bottom = node_image.attribute("height").as_int();
		builder->image_rect.right += builder->image_rect.left;
		builder->image_rect.bottom += builder->image_rect.top;
		return builder;
    }
	return nullptr;
}

void paper_builder_free(struct paper_builder* builder)
{
    free(builder);
}
