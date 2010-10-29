#ifndef GLSL_PROGRAM_H__
#define GLSL_PROGRAM_H__

//#include <GL/glew.h>
namespace glsl
{
	class Shader;
	class Program
	{
	public:
		Program();
		~Program();
		bool attachShader(const Shader& s);
		bool link();
		void bind() const;
		void unbind() const;
		GLint location(const char* c) const;
		void setUniform1i(const char* name, GLint v0) const;
		void setUniform2i(const char* name, GLint v0, GLint v1) const;
		void setUniform3i(const char* name, GLint v0, GLint v1, GLint v2) const;
		void setUniform4i(const char* name, GLint v0, GLint v1, GLint v2, GLint v3) const;

		void setUniform1f(const char* name, GLfloat v0) const;
		void setUniform2f(const char* name, GLfloat v0, GLfloat v1) const;
		void setUniform3f(const char* name, GLfloat v0, GLfloat v1, GLfloat v2) const;
		void setUniform4f(const char* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const;
	private:
		GLuint          prog_;
	};

} //namespace glsl

#endif /* GLSL_PROGRAM_H__ */