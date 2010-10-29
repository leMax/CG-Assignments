#ifndef GLSL_SHADER_H__
#define GLSL_SHADER_H__
	
namespace glsl
{
	class Program;

	class Shader
	{
	public:
		Shader(GLenum shaderType);
		~Shader();
		bool compile(const char* source);       
		bool compileFromFile(const char* path);

	private:
		GLuint          shader_;        /// the shader object
		friend class Program;
 	};
 	
} //namespace glsl
#endif /* GLSL_SHADER_H__ */