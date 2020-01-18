/**********************************************************
 *   CreateImage:  A class to encapsulate loading an image
 *   into a GLvoid blob.  Designed for OpenGL. This class
 *   assumes that there is an images directory with the 
 *   graphics in it.
 *   Created by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   12/2019 San Diego, California USA
 * ********************************************************/

#define LIMIT 71
#ifndef CREATEIMAGE_H
#define CREATEIMAGE_H
#include "commonheader.h"

using namespace std;

/* \class CreateImage
 * Using Free Image Plus, this class loads an image into 
 * memory, converts it to a 32 bit format with alpha, and 
 * then passes it to an array of unsigned characters, which 
 * is then passed using a cast to a GLvoid blob. Optionally the 
 * image can be turned into an OpenGL buffer object. It can
 * also create a sky box or a texture array.  To create a 
 * texture array you have to have a set of pictures with the 
 * same size and either they must be all RGBA or RGB, you
 * cannot mix them.  This class only supports the making of
 * RGBA image data.
 */
class CreateImage
{
public:
    CreateImage();
    ~CreateImage();
    
    /** \brief setImage
     *  Load image and convert it.
     */
    void setImage(string imagefile);
    
    /** \brief getWidth
     * Accessor function.
     */
    GLsizei getWidth();

    /** \brief getHeight
     * Accessor function.
     */
    GLsizei getHeight();

    /** \brief getData
     * Accessor function.
     */
    GLvoid *getData();
    
    /** \brief textureObject
     * Return an OpenGL buffer object.
     */
    GLuint textureObject();
    
    /** \brief createSkyBoxTex 
     * Create a sky box using six pictures for the inside 
     * of the box.
     */
    void createSkyBoxTex(GLuint &textureID, string filenames[6]);
    
    /** \brief create2DTexArray
     * Create a texture array, you can adjust this to make up
     * to a maximum picture array size of 256.
     */
    void create2DTexArray(GLuint &textureID, string filenames[16]);
protected:
    //! Class global variables.
    fipImage txtImage;
    BYTE *picLine;
    GLsizei width;
    GLsizei height;
    int size = 0;
    unsigned char *pixels = NULL;
    int count, line;
};
#endif // CreateImage.h
