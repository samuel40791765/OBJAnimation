//////////////////////////////////////////////////////////////////////////////
//
//  --- LoadShaders.h ---
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __LOAD_SHADERS_H__
#define __LOAD_SHADERS_H__

#include <GL/gl.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

//----------------------------------------------------------------------------
//
//  LoadShaders() takes an array of ShaderFile structures, each of which
//    contains the type of the shader, and a pointer a C-style character
//    string (i.e., a NULL-terminated array of characters) containing the
//    entire shader source.
//
//  The array of structures is terminated by a final Shader with the
//    "type" field set to GL_NONE.
//
//  LoadShaders() returns the shader program value (as returned by
//    glCreateProgram()) on success, or zero on failure. 
//

typedef struct {
    GLenum       type;
    const char*  filename;
    GLuint       shader;
} ShaderInfo;

static const GLchar*
	ReadShader( const char* filename )
{
#ifdef WIN32
	FILE* infile;
	fopen_s( &infile, filename, "rb" );
#else
	FILE* infile = fopen( filename, "rb" );
#endif // WIN32

	if ( !infile ) {
#ifdef _DEBUG
		std::cerr << "Unable to open file '" << filename << "'" << std::endl;
#endif /* DEBUG */
		return NULL;
	}

	fseek( infile, 0, SEEK_END );
	int len = ftell( infile );
	fseek( infile, 0, SEEK_SET );

	GLchar* source = new GLchar[len+1];

	fread( source, 1, len, infile );
	fclose( infile );

	source[len] = 0;

	return const_cast<const GLchar*>(source);
}

//----------------------------------------------------------------------------

GLuint
	LoadShaders( ShaderInfo* shaders )
{
	if ( shaders == NULL ) { return 0; }

	GLuint program = glCreateProgram();

	ShaderInfo* entry = shaders;
	while ( entry->type != GL_NONE ) {
		GLuint shader = glCreateShader( entry->type );

		entry->shader = shader;

		const GLchar* source = ReadShader( entry->filename );
		if ( source == NULL ) {
			for ( entry = shaders; entry->type != GL_NONE; ++entry ) {
				glDeleteShader( entry->shader );
				entry->shader = 0;
			}

			return 0;
		}

		glShaderSource( shader, 1, &source, NULL );
		delete [] source;

		glCompileShader( shader );

		GLint compiled;
		glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled ) {
#ifdef _DEBUG
			GLsizei len;
			glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );

			GLchar* log = new GLchar[len+1];
			glGetShaderInfoLog( shader, len, &len, log );
			std::cerr << "Shader compilation failed: " << log << std::endl;
			delete [] log;
#endif /* DEBUG */

			return 0;
		}

		glAttachShader( program, shader );

		++entry;
	}

#ifdef GL_VERSION_4_1
	if ( GLEW_VERSION_4_1 ) {
		// glProgramParameteri( program, GL_PROGRAM_SEPARABLE, GL_TRUE );
	}
#endif /* GL_VERSION_4_1 */

	glLinkProgram( program );

	GLint linked;
	glGetProgramiv( program, GL_LINK_STATUS, &linked );
	if ( !linked ) {
#ifdef _DEBUG
		GLsizei len;
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &len );

		GLchar* log = new GLchar[len+1];
		glGetProgramInfoLog( program, len, &len, log );
		std::cerr << "Shader linking failed: " << log << std::endl;
		delete [] log;
#endif /* DEBUG */

		for ( entry = shaders; entry->type != GL_NONE; ++entry ) {
			glDeleteShader( entry->shader );
			entry->shader = 0;
		}

		return 0;
	}

	return program;
}

//----------------------------------------------------------------------------

#ifdef __cplusplus
};
#endif // __cplusplus

#endif // __LOAD_SHADERS_H__
