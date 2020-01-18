/**********************************************************
 *   UniformPrinter:  A class to encapsulate printing out
 *   the unforms from a shader program.  The idea for
 *   this program comes from the book "OpenGL ES 3.0 
 *   Programming Guide Second Edition" by Dan Ginsburg 
 *   and Budirijanto Purnomo published by Addison Wesley, 2014, 
 *   pages:  80 - 87.
 *   Created by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   12/2019 San Diego, California USA
 * ********************************************************/

#ifndef UNIFORMPRINTER_H
#define UNIFORMPRINTER_H
#include "commonheader.h"

/** \class UniformPrinter
 * A class to encapsulate printing out the unforms from a 
 * shader program.  Gives the location of the uniform and 
 * it's status. To use this class simply instantiate it
 * with the shader program object.
 */
class UniformPrinter
{
public:
    /** \brief UniformPrinter 
     * A constructor that requires the shader program object.
     */
    UniformPrinter(int progObj);
    ~UniformPrinter();
protected:
    
    /** \brief printUniforms
     * Print the active uniforms.  This function is little
     * more than a giant case-switch statement to account
     * for the various types of uniforms available.
     */
    void printUniforms();
    
    //! Class global variables.
    int maxUniformLen, numUniforms, index, progObj;
    char *uniformName;
    
};



#endif // UNIFORMPRINTER_H
