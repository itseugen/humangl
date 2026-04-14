#pragma once

#include "humangl.h"
#define GLFW_INCLUDE_NONE

// ------------------------------
// Manual loader: declarations (function pointer types + variables)
// ------------------------------
typedef GLuint	(*PFNGLCREATESHADERPROC)(GLenum);
typedef void	(*PFNGLSHADERSOURCEPROC)(GLuint, GLsizei, const GLchar* const*, const GLint*);
typedef void	(*PFNGLCOMPILESHADERPROC)(GLuint);
typedef void	(*PFNGLGETSHADERIVPROC)(GLuint, GLenum, GLint*);
typedef void	(*PFNGLGETSHADERINFOLOGPROC)(GLuint, GLsizei, GLsizei*, GLchar*);
typedef GLuint	(*PFNGLCREATEPROGRAMPROC)(void);
typedef void	(*PFNGLATTACHSHADERPROC)(GLuint, GLuint);
typedef void	(*PFNGLLINKPROGRAMPROC)(GLuint);
typedef void	(*PFNGLGETPROGRAMIVPROC)(GLuint, GLenum, GLint*);
typedef void	(*PFNGLGETPROGRAMINFOLOGPROC)(GLuint, GLsizei, GLsizei*, GLchar*);
typedef void	(*PFNGLUSEPROGRAMPROC)(GLuint);
typedef void	(*PFNGLDELETESHADERPROC)(GLuint);
typedef GLint	(*PFNGLGETUNIFORMLOCATIONPROC)(GLuint, const GLchar*);
typedef void	(*PFNGLUNIFORMMATRIX4FVPROC)(GLint, GLsizei, GLboolean, const GLfloat*);
typedef void	(*PFNGLGENBUFFERSPROC)(GLsizei, GLuint*);
typedef void	(*PFNGLBINDBUFFERPROC)(GLenum, GLuint);
typedef void	(*PFNGLBUFFERDATAPROC)(GLenum, ptrdiff_t, const void*, GLenum);
typedef void	(*PFNGLGENVERTEXARRAYSPROC)(GLsizei, GLuint*);
typedef void	(*PFNGLBINDVERTEXARRAYPROC)(GLuint);
typedef void	(*PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint);
typedef void	(*PFNGLVERTEXATTRIBPOINTERPROC)(GLuint, GLint, GLenum, GLboolean, GLsizei, const void*);
typedef void	(*PFNGLDELETEBUFFERSPROC)(GLsizei, const GLuint*);
typedef void	(*PFNGLDELETEVERTEXARRAYSPROC)(GLsizei, const GLuint*);

typedef void	(*PFNGLGENTEXTURESPROC)(GLsizei, GLuint*);
typedef void	(*PFNGLBINDTEXTUREPROC)(GLenum, GLuint);
typedef void	(*PFNGLTEXPARAMETERIPROC)(GLenum, GLenum, GLint);
typedef void	(*PFNGLTEXIMAGE2DPROC)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void*);
typedef void	(*PFNGLGENERATEMIPMAPPROC)(GLenum);

// function pointers
extern PFNGLCREATESHADERPROC				glCreateShader_ptr;
extern PFNGLSHADERSOURCEPROC				glShaderSource_ptr;
extern PFNGLCOMPILESHADERPROC				glCompileShader_ptr;
extern PFNGLGETSHADERIVPROC					glGetShaderiv_ptr;
extern PFNGLGETSHADERINFOLOGPROC			glGetShaderInfoLog_ptr;
extern PFNGLCREATEPROGRAMPROC				glCreateProgram_ptr;
extern PFNGLATTACHSHADERPROC				glAttachShader_ptr;
extern PFNGLLINKPROGRAMPROC					glLinkProgram_ptr;
extern PFNGLGETPROGRAMIVPROC				glGetProgramiv_ptr;
extern PFNGLGETPROGRAMINFOLOGPROC			glGetProgramInfoLog_ptr;
extern PFNGLUSEPROGRAMPROC					glUseProgram_ptr;
extern PFNGLDELETESHADERPROC				glDeleteShader_ptr;
extern PFNGLGETUNIFORMLOCATIONPROC			glGetUniformLocation_ptr;
extern PFNGLUNIFORMMATRIX4FVPROC			glUniformMatrix4fv_ptr;
extern PFNGLGENBUFFERSPROC					glGenBuffers_ptr;
extern PFNGLBINDBUFFERPROC					glBindBuffer_ptr;
extern PFNGLBUFFERDATAPROC					glBufferData_ptr;
extern PFNGLGENVERTEXARRAYSPROC				glGenVertexArrays_ptr;
extern PFNGLBINDVERTEXARRAYPROC				glBindVertexArray_ptr;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC		glEnableVertexAttribArray_ptr;
extern PFNGLVERTEXATTRIBPOINTERPROC			glVertexAttribPointer_ptr;
extern PFNGLDELETEBUFFERSPROC				glDeleteBuffers_ptr;
extern PFNGLDELETEVERTEXARRAYSPROC			glDeleteVertexArrays_ptr;

extern PFNGLGENTEXTURESPROC					glGenTextures_ptr;
extern PFNGLBINDTEXTUREPROC					glBindTexture_ptr;
extern PFNGLTEXPARAMETERIPROC				glTexParameteri_ptr;
extern PFNGLTEXIMAGE2DPROC					glTexImage2D_ptr;
extern PFNGLGENERATEMIPMAPPROC				glGenerateMipmap_ptr;

// helper to load a symbol
template<typename T>
bool loadProc(T &fn, const char* name)
{
		fn = (T)glfwGetProcAddress(name);
		if (!fn) {
			std::cerr << "Failed to load GL function: " << name << "\n";
			return false;
		}
		return true;
}

bool loadGLFunctions();

#define glCreateShader  glCreateShader_ptr
#define glShaderSource  glShaderSource_ptr
#define glCompileShader glCompileShader_ptr
#define glGetShaderiv   glGetShaderiv_ptr
#define glGetShaderInfoLog glGetShaderInfoLog_ptr
#define glCreateProgram glCreateProgram_ptr
#define glAttachShader  glAttachShader_ptr
#define glLinkProgram   glLinkProgram_ptr
#define glGetProgramiv  glGetProgramiv_ptr
#define glGetProgramInfoLog glGetProgramInfoLog_ptr
#define glUseProgram    glUseProgram_ptr
#define glDeleteShader  glDeleteShader_ptr
#define glGetUniformLocation glGetUniformLocation_ptr
#define glUniformMatrix4fv glUniformMatrix4fv_ptr
#define glGenBuffers glGenBuffers_ptr
#define glBindBuffer glBindBuffer_ptr
#define glBufferData glBufferData_ptr
#define glGenVertexArrays glGenVertexArrays_ptr
#define glBindVertexArray glBindVertexArray_ptr
#define glEnableVertexAttribArray glEnableVertexAttribArray_ptr
#define glVertexAttribPointer glVertexAttribPointer_ptr
#define glDeleteBuffers glDeleteBuffers_ptr
#define glDeleteVertexArrays glDeleteVertexArrays_ptr

#define glGenTextures glGenTextures_ptr
#define glBindTexture glBindTexture_ptr
#define glTexParameteri glTexParameteri_ptr
#define glTexImage2D glTexImage2D_ptr
#define glGenerateMipmap glGenerateMipmap_ptr
