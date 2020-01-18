/*******************************************************************
 * Shader:  A class to encapsulate the creation and use of a set of
 * shaders. Note this class requires a seperate "shaders" directory
 *  to store the shaders in. Further this class creates a 
 * binary file that is used instead of recompiling the code.
 * if any changes are made to the shaders the compiled binary
 * must be deleted for them to take effect.
 * Edward C. Eberle <eberdeed@eberdeed.net>
 * December 2019 San Diego, California USA
 * ****************************************************************/

#include "shader.h"


Shader::Shader()
{
    cout << "\n\n\tCreating Shader.\n\n";
}

Shader::~Shader()
{
    cout << "\n\n\tDestroying Shader.\n\n";
}

void Shader::initShader(string vertexPath, string fragmentPath, 
    string outputFile)
{
    //! Where the program is created.
    outputFile = "/usr/share/openglresources/shaders/" + outputFile;
    vertexPath = "/usr/share/openglresources/shaders/" + vertexPath;
    fragmentPath = "/usr/share/openglresources/shaders/" + fragmentPath;
    this->outputFile = outputFile;
    int numFormats = 0;
    GLenum *valFormats;
    /** Before a binary can be loaded a binary format has
     * to be selected and before a binary format can be
     * found an amount of the number of binary formats 
     * has to be obtained.
     */
    glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &numFormats);
    Program = glCreateProgram();
    if (numFormats > 1)
    {
        valFormats = new GLenum[numFormats];
        glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, (int*)valFormats);
        for (int x = 0; x < numFormats; x++)
        {
            cout << "\n\nFormat " << x << " : " << valFormats[x];
        }
        format = valFormats[0];
        cout << "\n\n";
    }
    else
    {
        valFormats = new GLenum(0);
        glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, (int*) valFormats);
        format = *valFormats;
    }
    FILE *shaderFile;
    unsigned char c;
    shaderFile = fopen(outputFile.c_str(), "rb");
    if (!shaderFile)
    {
        cout << "\n\n\tError opening file " << outputFile << ".\n\n";
        response = false;
    }
    else
    {
        GLsizei fileSize = file_size(path(outputFile.c_str()));
        unsigned char shaderBinary[fileSize];
        for (int x = 0; x < fileSize; x++)
        { //! standard C I/O file reading loop
            shaderBinary[x] = fgetc(shaderFile);
        }
        /** This is the spot where the format is used to turn
         * the loaded binary into an OpenGL shader program.
         */
        glProgramBinary(Program, format, (GLvoid*) shaderBinary, fileSize);
        cout << "\n\n\tSuccessfully loaded pre-compiled agregate "
        << "program binary.\n\tThe program has binary format " 
        << format << " and size " << fileSize << " bytes.\n\n";
        glValidateProgram(Program);
        /** If the shader program does not load, the Shader
         * class will go through the usual process of 
         * obtaining the shader code, compling it, linking 
         * it and using it as a shader program.
         */
        glGetProgramiv(Program, GL_VALIDATE_STATUS, &response);
        if (!response)
        {
            cout << "\n\n\tInvalid program, recompile initiated.\n\n";
        }
    }
    if (!response)
    {
        glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &response);
        if (response > 1)
        {
            char infoLog[response];
            glGetProgramInfoLog(Program, response, NULL, infoLog);
            cout << "\n\n\tError loading shader program binary:  " << infoLog << "\n\n";
        }
        vertex = createShader(GL_VERTEX_SHADER, vertexPath);
        fragment = createShader(GL_FRAGMENT_SHADER, fragmentPath);
        if ((!vertex) || (!fragment))
        {
            cout << "\n\n\tError compiling shaders.\n\n";
            exit(1);
        }
        glProgramParameteri(Program, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
        glAttachShader(Program, vertex);
        glAttachShader(Program, fragment);
        glLinkProgram(Program);
        glGetProgramiv(Program, GL_PROGRAM_BINARY_LENGTH, &progLength);
        //! Print linking errors if any
        glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &infoLength);
        if(infoLength > 0)
        {
            char infoLog[infoLength];
            glGetProgramInfoLog(Program, infoLength, NULL, infoLog);
            cout << "\n\nShader Program Link Error\n" << infoLog << endl;
        }
        else
        {
            cout << "\n\n\tShader agregate binary program created "
            << "and the program has length " << progLength 
            << " bytes.\n\n";
        }
        //! Delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if(createBinary())
        {
            cout << "\n\n\tShader program binary " << outputFile 
            << " compiled and saved.\n\n";
        }
        else
        {
            cout << "\n\n\tShader program binary " << outputFile 
            << " failed to compile and save.\n\n";
        }
    }
}

unsigned int Shader::createShader(unsigned int type, string fpath)
{
    //! Where the individual shaders are compiled.
    char c;
    string shaderCode;
    unsigned int shaderobj;
    FILE *shaderFile = fopen(fpath.c_str(), "r");
    if (!shaderFile)
    {
        cout << "\n\n\tError opening file " << fpath << ".\n\n";
        return 0;
    }
    //! Read file's buffer contents into streams
    while ((c = std::fgetc(shaderFile)) != EOF) 
    { //! standard C I/O file reading loop
        shaderCode += c;
    }
    fclose(shaderFile);
    const GLchar* glShaderCode = shaderCode.c_str();
    //! Vertex Shader
    try
    {
        shaderobj = glCreateShader(type);
        if (!shaderobj)
        {
            cout << "\n\n\tUnable to create the shader object.\n\n";
            return 0;
        }
        else
        {
            cout << "\n\n\tCreated the shader object.\n\n";
        }
        glShaderSource(shaderobj, 1, &glShaderCode, nullptr);
        glCompileShader(shaderobj);
        //! Print compile errors if any
        glGetShaderiv(shaderobj, GL_INFO_LOG_LENGTH, &infoLength);
        if(infoLength > 0)
        {
            char infoLog[infoLength];
            glGetShaderInfoLog(shaderobj, infoLength, NULL, infoLog);
            cout << "\n\nShader compilation error: \n" << infoLog << endl;
            glDeleteShader(shaderobj);
            return 0;
        }
        else
        {
             cout << "\n\n\tShader compiled.\n\n";
        }
        return shaderobj;
    }
    catch(exception exc)
    {
        cout << "\n\n\tError making shader:  " << exc.what() << "\n\n";
        return 0;
    }
}

void Shader::Use() 
{ 
    glUseProgram(Program); 
}   

bool Shader::createBinary()
{
    //! Create the shader program binary for later use.
    if (progLength <= 0)
    {
        cout << "\n\n\tShader program length less than one.\n\n";
        progLength = 1000000;
    }
    binary = new unsigned char[progLength];
    glGetProgramBinary(Program, progLength, &progLenRet, &format, (GLvoid*) binary);
    if (progLength != progLenRet)
    {
        cout << "\n\n\tWarning program length of " << progLength 
        << " does not equal the size of the created binary "
        << progLenRet << " with format " << format
        << "\n\n";
    }
    FILE *shaderFile = fopen(outputFile.c_str(), "wb");
    if (!shaderFile)
    {
        cout << "\n\n\tError opening file " << "shaders/cubeshader" << ".\n\n";
        return false;
    }
    for (int x = 0; x < progLenRet; x++)
    { //! standard C I/O file reading loop
       fputc(binary[x], shaderFile);
    }
    return true;
}
    
void Shader::setBool(const std::string name, bool value) const
{         
    glUniform1i(glGetUniformLocation(Program, name.c_str()), (int)value); 
}
void Shader::setInt(const std::string name, int value) const
{ 
    glUniform1i(glGetUniformLocation(Program, name.c_str()), value); 
}
void Shader::setFloat(const std::string name, float value) const
{ 
    glUniform1f(glGetUniformLocation(Program, name.c_str()), value); 
} 
void Shader::setVec2(const std::string name, vec2 value) const
{ 
    glUniform2fv(glGetUniformLocation(Program, name.c_str()), 1, value_ptr(value)); 
} 
void Shader::setVec3(const std::string name, vec3 value) const
{ 
    glUniform3fv(glGetUniformLocation(Program, name.c_str()), 1, value_ptr(value)); 
} 
void Shader::setVec4(const std::string name, vec4 value) const
{ 
    glUniform4fv(glGetUniformLocation(Program, name.c_str()), 1, value_ptr(value)); 
} 
void Shader::setMat4(const std::string name, mat4 value) const
{ 
    glUniformMatrix4fv(glGetUniformLocation(Program, name.c_str()), 1, GL_FALSE, &value[0][0]); 
} 
