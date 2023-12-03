#version 430 core
layout (points) in;
layout (triangle_strip, max_vertices = 16) out;

layout(std430,  binding = 0) buffer edgeMaskBuffer
{
  int edgeMask[];
};

layout(std430, binding = 1) buffer triangleTableBuffer
{
  int triangleTable[];
};


in VS_OUT {
    mat4 model;
    mat4 view;
    mat4 projection;
}gs_in[];


vec4 cubePos[8] = vec4[8](
        vec4(-0.5,-0.5,-0.5, 0.0),    //v0
        vec4( 0.5,-0.5, -0.5, 0.0),   //v1
        vec4( 0.5,-0.5, 0.5, 0.0),    //v2
        vec4(-0.5,-0.5, 0.5, 0.0),    //v3

        vec4(-0.5, 0.5,-0.5, 0.0),    //v4
        vec4( 0.5, 0.5, -0.5, 0.0),   //v5
        vec4( 0.5, 0.5, 0.5, 0.0),    //v6
        vec4(-0.5, 0.5, 0.5, 0.0)     //v7
);

   

 ivec2 edgeIndices[12] = ivec2[12](
        ivec2(0, 1),   //edge0
        ivec2(1, 2),   //edge1
        ivec2(2, 3),   //edge2
        ivec2(3, 0),   //edge3
        ivec2(4, 5),   //edge4
        ivec2(5, 6),   //edge5
        ivec2(6, 7),   //edge6
        ivec2(7, 4),   //edge7
        ivec2(4, 0),   //edge8
        ivec2(1, 5),   //edge9
        ivec2(2, 6),   //edge10
        ivec2(3, 7)    //edge11
);

float densityFunction(vec4 ws){
    return ws.x *ws.x +  ws.y *ws.y +  ws.z *ws.z -4096.0 ;
}

int GetCubeIndex(){
    int index = 0;
    for(int i = 0; i<8;i++){
        if(densityFunction(gl_in[0].gl_Position + cubePos[i])>0.0)
            index = index | int(pow(2,i)) ;
    }
    return index;
}

vec4 interpolateVerticesFromEdgeIndices(int index){
    vec4 result;
    vec4 v1= cubePos[edgeIndices[index].x] ;
    vec4 v2= cubePos[edgeIndices[index].y] ;

    result = (v1 + v2) /2.0;
    return gl_in[0].gl_Position+result;
}

void DrawTriangles(){
    
    int cubeIndex = GetCubeIndex();
    /*
    vec4 vertices[12];
    for(int i =0;i<12;i++){
        int bitmask = int(pow(2, i));
        if((edgeMask[cubeIndex] & bitmask) !=0 ){
            vertices[i] =interpolateVerticesFromEdgeIndices(i);
        }
    }
    
    for(int i = 0; triangleTable[cubeIndex*16 + i]!=-1;i+=3){
        gl_Position =gs_in[0].projection*gs_in[0].view*gs_in[0].model* vertices[triangleTable[cubeIndex*16 + i]];
        EmitVertex();  
        gl_Position =gs_in[0].projection*gs_in[0].view*gs_in[0].model* vertices[triangleTable[cubeIndex*16 + i +1]];
        EmitVertex();                                                  
        gl_Position =gs_in[0].projection*gs_in[0].view*gs_in[0].model* vertices[triangleTable[cubeIndex*16 + i +2]];
        EmitVertex();  
        EndPrimitive();
    }
    */
    
     for(int i = 0; triangleTable[cubeIndex*16 + i]!=-1;i+=3){
        gl_Position =gs_in[0].projection*gs_in[0].view*gs_in[0].model* interpolateVerticesFromEdgeIndices(triangleTable[cubeIndex*16 + i]);
        EmitVertex();  
        gl_Position =gs_in[0].projection*gs_in[0].view*gs_in[0].model* interpolateVerticesFromEdgeIndices(triangleTable[cubeIndex*16 + i +1]);
        EmitVertex();                                                  
        gl_Position =gs_in[0].projection*gs_in[0].view*gs_in[0].model* interpolateVerticesFromEdgeIndices(triangleTable[cubeIndex*16 + i +2]);
        EmitVertex();  
        EndPrimitive();
    }
    
}


void main(){  
  DrawTriangles();

}







