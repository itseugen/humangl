#include "opengl.hpp"

PFNGLCREATESHADERPROC				glCreateShader_ptr = nullptr;
PFNGLSHADERSOURCEPROC				glShaderSource_ptr = nullptr;
PFNGLCOMPILESHADERPROC				glCompileShader_ptr = nullptr;
PFNGLGETSHADERIVPROC				glGetShaderiv_ptr = nullptr;
PFNGLGETSHADERINFOLOGPROC			glGetShaderInfoLog_ptr = nullptr;
PFNGLCREATEPROGRAMPROC				glCreateProgram_ptr = nullptr;
PFNGLATTACHSHADERPROC				glAttachShader_ptr = nullptr;
PFNGLLINKPROGRAMPROC				glLinkProgram_ptr = nullptr;
PFNGLGETPROGRAMIVPROC				glGetProgramiv_ptr = nullptr;
PFNGLGETPROGRAMINFOLOGPROC			glGetProgramInfoLog_ptr = nullptr;
PFNGLUSEPROGRAMPROC					glUseProgram_ptr = nullptr;
PFNGLDELETESHADERPROC				glDeleteShader_ptr = nullptr;
PFNGLGETUNIFORMLOCATIONPROC			glGetUniformLocation_ptr = nullptr;
PFNGLUNIFORMMATRIX4FVPROC			glUniformMatrix4fv_ptr = nullptr;
PFNGLGENBUFFERSPROC					glGenBuffers_ptr = nullptr;
PFNGLBINDBUFFERPROC					glBindBuffer_ptr = nullptr;
PFNGLBUFFERDATAPROC					glBufferData_ptr = nullptr;
PFNGLGENVERTEXARRAYSPROC			glGenVertexArrays_ptr = nullptr;
PFNGLBINDVERTEXARRAYPROC			glBindVertexArray_ptr = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC	glEnableVertexAttribArray_ptr = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC		glVertexAttribPointer_ptr = nullptr;
PFNGLDELETEBUFFERSPROC				glDeleteBuffers_ptr = nullptr;
PFNGLDELETEVERTEXARRAYSPROC			glDeleteVertexArrays_ptr = nullptr;

PFNGLGENTEXTURESPROC				glGenTextures_ptr = nullptr;
PFNGLBINDTEXTUREPROC				glBindTexture_ptr = nullptr;
PFNGLTEXPARAMETERIPROC				glTexParameteri_ptr = nullptr;
PFNGLTEXIMAGE2DPROC					glTexImage2D_ptr = nullptr;
PFNGLGENERATEMIPMAPPROC				glGenerateMipmap_ptr = nullptr;

PFNGLUNIFORM3FPROC					glUniform3f_ptr = nullptr;

PFNGLUNIFORM1IPROC					glUniform1i_ptr = nullptr;

bool loadGLFunctions()
{
	// load pointers we declared
	if(!loadProc(glCreateShader_ptr, "glCreateShader")) return false;
	if(!loadProc(glShaderSource_ptr, "glShaderSource")) return false;
	if(!loadProc(glCompileShader_ptr, "glCompileShader")) return false;
	if(!loadProc(glGetShaderiv_ptr, "glGetShaderiv")) return false;
	if(!loadProc(glGetShaderInfoLog_ptr, "glGetShaderInfoLog")) return false;
	if(!loadProc(glCreateProgram_ptr, "glCreateProgram")) return false;
	if(!loadProc(glAttachShader_ptr, "glAttachShader")) return false;
	if(!loadProc(glLinkProgram_ptr, "glLinkProgram")) return false;
	if(!loadProc(glGetProgramiv_ptr, "glGetProgramiv")) return false;
	if(!loadProc(glGetProgramInfoLog_ptr, "glGetProgramInfoLog")) return false;
	if(!loadProc(glUseProgram_ptr, "glUseProgram")) return false;
	if(!loadProc(glDeleteShader_ptr, "glDeleteShader")) return false;
	if(!loadProc(glGetUniformLocation_ptr, "glGetUniformLocation")) return false;
	if(!loadProc(glUniformMatrix4fv_ptr, "glUniformMatrix4fv")) return false;
	if(!loadProc(glGenBuffers_ptr, "glGenBuffers")) return false;
	if(!loadProc(glBindBuffer_ptr, "glBindBuffer")) return false;
	if(!loadProc(glBufferData_ptr, "glBufferData")) return false;
	if(!loadProc(glGenVertexArrays_ptr, "glGenVertexArrays")) return false;
	if(!loadProc(glBindVertexArray_ptr, "glBindVertexArray")) return false;
	if(!loadProc(glEnableVertexAttribArray_ptr, "glEnableVertexAttribArray")) return false;
	if(!loadProc(glVertexAttribPointer_ptr, "glVertexAttribPointer")) return false;
	if(!loadProc(glDeleteBuffers_ptr, "glDeleteBuffers")) return false;
	if(!loadProc(glDeleteVertexArrays_ptr, "glDeleteVertexArrays")) return false;

	if(!loadProc(glGenTextures, "glGenTextures")) return false;
	if(!loadProc(glBindTexture, "glBindTexture")) return false;
	if(!loadProc(glTexParameteri, "glTexParameteri")) return false;
	if(!loadProc(glTexImage2D, "glTexImage2D")) return false;
	if(!loadProc(glGenerateMipmap, "glGenerateMipmap")) return false;

	if(!loadProc(glUniform3f, "glUniform3f")) return false;

	if(!loadProc(glUniform1i_ptr, "glUniform1i")) return false;

	return true;
}
