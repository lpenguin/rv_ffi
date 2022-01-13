#include "vange_rs.h"
#ifdef USE_VANGE_RS_STUB
rv_context rv_init(struct rv_init_descriptor desc)
{

}

void rv_exit(rv_context context)
{

}

void rv_camera_init(rv_context context, struct  rv_camera_description desc)
{

}

void rv_camera_set_transform(rv_context context, struct rv_transform transform)
{

}

void rv_map_init(rv_context context, rv_map_description map_description)
{

}

void rv_map_exit(rv_context context)
{

}

void rv_map_update_data(rv_context context,struct  rv_rect region)
{

}

void rv_map_update_palette(rv_context context, int32_t first_entry, int32_t entry_count, uint8_t *palette)
{

}

void rv_render(rv_context context,struct  rv_rect viewport)
{

}
#endif // USE_VANGE_RS_STUB
