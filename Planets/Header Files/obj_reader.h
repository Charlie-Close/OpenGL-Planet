//
//  obj_reader.h
//  Planets
//
//  Created by Charlie Close on 26/04/2023.
//

#ifndef obj_reader_h
#define obj_reader_h

#include"VBO.h"
#include<glm/glm.hpp>
#include <vector>
#include <string>
#include <istream>
#include <fstream>
#include <sstream>

struct VertRef
{
    VertRef( int v, int vt, int vn ) : v(v), vt(vt), vn(vn) { }
    int v, vt, vn;
};

std::vector< Vertex > LoadOBJ(std::string pathfile);

#endif /* obj_reader_h */
