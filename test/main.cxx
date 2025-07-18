#include <iostream>

#include "box2d/box2d.h"

int main()
{
    printf("Hello world!");

    b2WorldDef world_def { b2DefaultWorldDef() };

    b2SetLengthUnitsPerMeter(32.0f);
    world_def.gravity.y = -9.81f * 32.0f;

    b2WorldId world_id = b2CreateWorld(&world_def);

    // creating physics ground
    b2BodyDef ground_def = b2DefaultBodyDef();

    ground_def.position = { .0f, -2.0f };

    b2BodyId ground_id = b2CreateBody(world_id, &ground_def);

    // creating shape for physics ground
    b2ShapeDef shape_def = b2DefaultShapeDef();

    b2Polygon polygon_box = b2MakeBox(100.0f, 2.0f);

    b2CreatePolygonShape(ground_id, &shape_def, &polygon_box);
}