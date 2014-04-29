#version 130

// Old - but no longer has to be between -1 and +1 because mM (below) can scale/rotate/translate it!
in vec4 s_vPosition;
in vec4 s_vColor;
in vec2 texcoord;

// New matrix stuff! Note that "uniform" means "for all vertices" - they share this
uniform mat4 mM; // The matrix to convert into the world coordinate system
uniform mat4 mV; // The matrix to convert into the camera coordinate system
uniform mat4 mP; // The perspective matrix for depth

out vec4 color;
out vec2 texCoord;

void main () {
color = s_vColor;
//texcoord=texCoord;//Uncomment is death
// New way using matrix multiplication. From local, to world, to camera, to NDCs
gl_Position = mP*mV*mM*s_vPosition;

}
