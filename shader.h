/*******************************************************************
 * Shader:  A class to encapsulate the creation and use of
 * a set of shaders. Note this class requires a seperate 
 * "shaders" directory to store the shaders in. Further 
 * this class creates a binary file that is used instead 
 * of recompiling the code.
 * If any changes are made to the shaders, the compiled binary
 * must be deleted for them to take effect.
 * Edward C. Eberle <eberdeed@eberdeed.net>
 * December 2019 San Diego, California USA
 * ****************************************************************/

#ifndef SHADER_H
#define SHADER_H

#include "commonheader.h"

/** \class Shader
 * A class to encapsulate the uploading, compiling, linking
 * and use of a shader.  Note this class requires a
 * seperate "shaders" directory to store the shaders in.  
 * Further, this class will create shader binary and reload it.
 * The binary will be recreated it if it ceases to validate as 
 * a shader program.
 */
class Shader
{
public:
    Shader();
    ~Shader();
   
    /** \brief initShader
     * Read and build the shader from two files.
     */
    void initShader(string vertexPath, string fragmentPath, 
    string outputFile);
    
    /** \brief createShader
     * Create the vertex or fragment shader from a file.
     */
    unsigned int createShader(unsigned int type, string fpath);
    
    /** \brief Use
     * Use the program.
     */
    void Use();
    
    /** \brief createBinary
     * Create the shader program binary and save it to a file.
     */
    bool createBinary();
    
    /** \brief  setBool
     * A Utility uniform function that sets a value in 
     * the shader(s).
     */
    void setBool(const string name, bool value) const;  

    /** \brief  setInt
     * A Utility uniform function that sets a value in 
     * the shader(s).
     */
    void setInt(const string name, int value) const;   

    /** \brief  setFloat
     * A Utility uniform function that sets a value in 
     * the shader(s).
     */
    void setFloat(const string name, float value) const;
    
    /** \brief  setVec2
     * A Utility uniform function that sets a value in 
     * the shader(s).
     */
    void setVec2(const std::string name, vec2 value) const;

    /** \brief  setVec3
     * A Utility uniform function that sets a value in 
     * the shader(s).
     */
    void setVec3(const std::string name, vec3 value) const;

    /** \brief  setVec4
     * A Utility uniform function that sets a value in 
     * the shader(s).
     */
    void setVec4(const string name, vec4 value) const; 

    /** \brief  setMat4
     * A Utility uniform function that sets a value in 
     * the shader(s).
     */
    void setMat4(const string name, mat4 value) const;    
    GLuint Program;
protected:
    //! Class global variables.
    int success = 0;
    int infoLength = 0;
    GLuint vertex, fragment;
    int progLength = 0;
    int progLenRet = 0;
    GLenum *valFormats;
    GLenum format;
    int response = 0;
    unsigned char *binary;
    string outputFile;

};
  
#endif //SHADER_H
