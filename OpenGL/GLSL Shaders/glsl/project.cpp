/**
 * @file project.cpp
 * @brief GLSL project
 *
 * @author H. Q. Bovik (hqbovik)
 * @bug Unimplemented
 */

#include "glsl/project.hpp"
#include <fstream>

// A namespace declaration. All proejct files use this namespace.
// Add this declration (and its closing) to all source/headers you create.
// Note that all #includes should be BEFORE the namespace declaration.
namespace _462 {

GLfloat progTime;
GLuint fbo_color, fbo_depth, fbo_norms, oldColor;
GLuint fbo;
GLuint window_width =1024, window_height = 768;
bool frame1 = true;
// shader loading code

/**
 * Load a file as either a vertex shader or a fragment shader, and attach
 * it to a program.
 * @param file The filename to load
 * @param type Either GL_VERTEX_SHADER_ARB, or GL_FRAGMENT_SHADER_ARB
 * @param program The shader program to which to attach the loaded shader.
 * @return True on success.
 */
static bool load_shader( const char* file, GLint type, GLhandleARB program )
{
    std::ifstream infile;
    char* buffer;
    char error_msg[2048];
    GLhandleARB shader;

    infile.open( file );

    if( infile.fail() ) {
        std::cout << "ERROR: cannot open file: " << file << std::endl;
        infile.close();
        return false;
    }

    // calculate length
    infile.seekg( 0, std::ios::end );
    int length = infile.tellg();
    infile.seekg( 0, std::ios::beg );
    // allocate space for entire program
    buffer = (char *) malloc( (length + 1) * sizeof *buffer );
    if ( !buffer )
        return false;
    // copy entire program into memory
    infile.getline( buffer, length, '\0' );
    infile.close();

    // create shader object
    shader = glCreateShaderObjectARB( type );
    // link shader source
    const char* src = buffer; // workaround for const correctness
    glShaderSourceARB( shader, 1, &src, NULL );
    // compile shaders
    glCompileShaderARB( shader );
    // check success
    GLint result;
    glGetObjectParameterivARB( shader, GL_OBJECT_COMPILE_STATUS_ARB, &result );
    if ( result != GL_TRUE ) {
        glGetInfoLogARB( shader, sizeof error_msg, NULL, error_msg );
        std::cout << "GLSL COMPILE ERROR in " << file << ": " << error_msg << std::endl;
        return false;
    } else {
        std::cout << "Compiled shaders successfully" << std::endl;
    }

    // attach the shader object to the program object
    glAttachObjectARB( program, shader );

    free( buffer );
    return true;
}

/**
 * Loads a vertex and fragment shader from the given files and attaches them to the given
 * shader program object.
 * @param vert_file The filename of the vetex shader.
 * @param frag_file The filename of the fragment shader.
 * @return True on success.
 */
static bool create_shader( GLhandleARB program, const char* vert_file, const char* frag_file )
{
    bool rv = true;

    std::cout
        << "Loading vertex shader " << vert_file
        << "and fragment shader " << frag_file << std::endl;

    // Load vertex shader
    rv = rv && load_shader( vert_file, GL_VERTEX_SHADER_ARB, program );
    // Load fragment shader
    rv = rv && load_shader( frag_file, GL_FRAGMENT_SHADER_ARB, program );

    if ( !rv )
        return false;

    // link
    glLinkProgramARB( program );

    // check for success
    GLint result;
    glGetProgramiv( program, GL_LINK_STATUS, &result );
    if ( result == GL_TRUE ) {
        std::cout << "Successfully linked shader" << std::endl;
        return true;
    } else {
        std::cout << "FAILED to link shader" << std::endl;
        return false;
    }
}


// definitions of functions for the GlslProject class

// constructor, invoked when object is created
GlslProject::GlslProject()
{
    // TODO any basic construction or initialization of members
    // Warning: Although members' constructors are automatically called,
    // ints, floats, pointers, and classes with empty contructors all
    // will have uninitialized data!
}

// destructor, invoked when object is destroyed
GlslProject::~GlslProject()
{
    // Warning: Do not throw exceptions or call virtual functions from deconstructors!
    // They will cause undefined behavior (probably a crash, but perhaps worse).
}

void CheckFramebufferStatus()
{
    GLenum status;
    status = (GLenum) glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    switch(status) {
        case GL_FRAMEBUFFER_COMPLETE_EXT:
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
            printf("Unsupported framebuffer format\n");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
            printf("Framebuffer incomplete, missing attachment\n");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            printf("Framebuffer incomplete, attached images must have same dimensions\n");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
            printf("Framebuffer incomplete, attached images must have same format\n");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
            printf("Framebuffer incomplete, missing draw buffer\n");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
            printf("Framebuffer incomplete, missing read buffer\n");
            break;
        default:
            assert(0);
    }
}


void initFrameBufferDepthBuffer(void) {
    glGenRenderbuffersEXT(1, &oldColor); // Generate one render buffer and store the ID in fbo_depth
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, oldColor); // Bind the fbo_depth render buffer
    // Set the render buffer storage to be a depth component, with a width and height of the window
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, window_width, window_height);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0); // Unbind the render buffer
}

void initFrameBufferImages(void) {
    
    // Color Buffer
    glGenTextures(1, &fbo_color); // Generate one texture
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, fbo_color); // Bind the texture fbo_color
    // Create a standard texture with the width and height of our window
    glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, window_width, window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    // Setup the basic texture parameters
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_NEAREST);
    // Unbind the texture
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);

    // Normals Buffer
    glGenTextures(1, &fbo_norms); // Generate one texture
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, fbo_norms); // Bind the texture fbo_norms
    // Create a standard texture with the width and height of our window
    glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, window_width, window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    // Setup the basic texture parameters
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_NEAREST);
    // Unbind the texture
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);

    // Depth Buffer
    glGenTextures(1, &fbo_depth); // Generate one texture
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, fbo_depth); // Bind the texture fbo_depth
    // Create a standard texture with the width and height of our window
    glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_DEPTH_COMPONENT, window_width, window_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    // Setup the basic texture parameters
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_NEAREST);
    // Unbind the texture
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);

    //initFrameBufferDepthBuffer();

    
    // Old Color Buffer
    glGenTextures(1, &oldColor); // Generate one texture
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, oldColor); // Bind the texture fbo_norms
    // Create a standard texture with the width and height of our window
    glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, window_width, window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    // Setup the basic texture parameters
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_NEAREST);
    // Unbind the texture
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);
    
}

/**
 * Initialize the project, loading the mesh from the given filename.
 * Also do any necessary opengl initialization.
 * @param renderer Object that renders the scene.
 * @param width The width in pixels of the framebuffer.
 * @param height The height in pixels of the framebuffer.
 * @return true on success, false on error.
 * @see scene/mesh.hpp
 */
bool GlslProject::initialize( const SceneRenderer* renderer, int width, int height )
{
    bool rv = true;
    glEnable(GL_ARB_texture_rectangle);

    // copy renderer for later use
    this->renderer = renderer;

    // create a test shader
    program  = glCreateProgramObjectARB();
    rv = rv && create_shader( program, "shaders/example_vert.glsl", "shaders/example_frag.glsl" );

    toon  = glCreateProgramObjectARB();
    rv = rv && create_shader( toon, "shaders/toon_vert.glsl", "shaders/toon_frag.glsl" );

    toon_render  = glCreateProgramObjectARB();
    rv = rv && create_shader( toon_render, "shaders/toon_render_vert.glsl", "shaders/toon_render_frag.glsl" );
    
    // TODO opengl initialization code
	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
		printf("Ready for GLSL\n");
	else {
		printf("Not totally ready \n");
		exit(1);
	}
	printInfoLog(program);

    
    initFrameBufferImages();

    glGenFramebuffersEXT(1, &fbo); // Generate one frame buffer and store the ID in fbo

    //CheckFramebufferStatus(); // Check that status of our generated frame buffer
    return rv;
}

/**
 * Clean up the project. Free any memory, etc.
 */
void GlslProject::destroy()
{
    //Delete resources
    
    glDeleteObjectARB( program );
    glDeleteObjectARB( toon );
    glDeleteObjectARB( toon_render );

    glDeleteTextures(1, &fbo_color);
    glDeleteTextures(1, &fbo_depth);
    glDeleteTextures(1, &fbo_norms);
    glDeleteTextures(1, &oldColor);

    //glDeleteRenderbuffersEXT(1, &fbo_depth);
    //Bind 0, which means render to back buffer, as a result, fb is unbound
    //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    glDeleteFramebuffersEXT(1, &fbo);
    // TODO any cleanup code
}

/**
 * Render the scene with outlines using shaders.
 */
void GlslProject::render()
{
    if (frame1) {
        /* PASS 0 : Storing color and depth*/
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo); // Bind our frame buffer
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_RECTANGLE_ARB, fbo_depth, 0);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_RECTANGLE_ARB, fbo_color, 0);
        renderer->render_scene();
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        /* PASS 0 END */


        /* PASS 1 : Storing normals */
        glUseProgramObjectARB( program ); // you bind the current shader, just like with textures
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo); // Bind our frame buffer
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_RECTANGLE_ARB, fbo_norms, 0);
        renderer->render_scene();
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // Unbind our frame buffer
        glUseProgramObjectARB( 0 );
        /* PASS 1 END */


        /* PASS 2 : Calculating outline and rendering to an FBO for storage in frame (i-1) texture : texture named : oldColor */
        glUseProgramObjectARB( toon ); // you bind the current shader, just like with textures
        
            /* Passing textures to shader */
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_RECTANGLE_ARB, fbo_color); // Bind our frame buffer color texture
            GLint texture_loc = glGetUniformLocationARB(toon, "scenetex");
            glUniform1iARB( texture_loc, 0 );

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_RECTANGLE_ARB, fbo_norms); // Bind our frame buffer normal texture
            GLint norm_texture_loc = glGetUniformLocationARB(toon, "normtex");
            glUniform1iARB( norm_texture_loc, 1 );

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_RECTANGLE_ARB, fbo_depth); // Bind our frame buffer depth texture
            GLint depth_texture_loc = glGetUniformLocationARB(toon, "depthtex");
            glUniform1iARB( depth_texture_loc, 2 );

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glPushMatrix();

            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f);
                glVertex3f(-1.0f, -1.0f, -1.0f); // The bottom left corner

                glTexCoord2f(window_width, 0.0f);
                glVertex3f(1.0f, -1.0f, -1.0f); // The top left corner

                glTexCoord2f(window_width, window_height);
                glVertex3f(1.0f, 1.0f, -1.0f); // The top right corner

                glTexCoord2f(0.0f, window_height);
                glVertex3f(-1.0f, 1.0f, -1.0f); // The bottom right corner
            glEnd();

            glPopMatrix();

        glUseProgramObjectARB( 0 );

        /*
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo); // Bind our frame buffer
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_RECTANGLE_ARB, oldColor, 0);
        */

        //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // Unbind our frame buffer
        
        /* PASS 2 END */

        /* PASS 3 : Rendering the above stored FBO texture to screen sized quad by sampling the oldColor texture */
        /*
        glUseProgramObjectARB( toon_render ); // you bind the current shader, just like with textures
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_RECTANGLE_ARB, oldColor); // Bind our frame buffer color texture
        GLint toon_texture_loc = glGetUniformLocationARB(toon_render, "toontex");
        glUniform1iARB( toon_texture_loc, 0 );
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glPushMatrix();

        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f, -1.0f); // The bottom left corner

            glTexCoord2f(window_width, 0.0f);
            glVertex3f(1.0f, -1.0f, -1.0f); // The top left corner
            
            glTexCoord2f(window_width, window_height);
            glVertex3f(1.0f, 1.0f, -1.0f); // The top right corner

            glTexCoord2f(0.0f, window_height);
            glVertex3f(-1.0f, 1.0f, -1.0f); // The bottom right corner
        glEnd();

        glPopMatrix();
        
        glUseProgramObjectARB( 0 );
        */
        /* PASS 3 END */
        
        /* FRAME 1 END */
        //frame1 = false;
    }



}

void GlslProject::printInfoLog(GLhandleARB obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB,
					 &infologLength);

    if (infologLength > 0)
    {
	infoLog = (char *)malloc(infologLength);
	glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
	printf("%s\n",infoLog);
	free(infoLog);
    }
}

} /* _462 */