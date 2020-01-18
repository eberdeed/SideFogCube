
    Edward C. Eberle <eberdeed@eberdeed.net>
    San Diego, California USA
    
    sidefogcube is a program that displays a cloud of cubes in
    alternately a foggy environment or a clear one.  The 
    keys are as follows.
    a right
    s back
    d left
    w forward
    r up
    f down
    Space toggles the fog.
    Escape ends the program.
    Up arrow zooms in.
    Down arrow zooms out.
    Right arrow increases the fog distance from the viewer.
    Left arrow decreases the distance.
    z resets the camera.
    
    To compile the program, first adjust the screen size on lines 113 and 114 in sidefogcube.h, then you will need the 
    following libraries:
    FreeImage, FreeImagePlus, GLEW, ClanLib, boost. Also you must 
    have cmake and perhaps, if you wish, kdevelop.  The commands
    are:
    
    cmake .
    make
    make install
    
    To run the program:
    
    sidefogcube
    
    The documentation is located in:
    
    /usr/share/doc/sidefogcube-doc
    
    The sources I used to educate myself concerning OpenGL are
    as follows:
        "OpenGL ES 3.0 Programming Guide Second Edition" 
    by Dan Ginsburg and Budirijanto Purnomo published by Addison-Wesley 2014.
        www.learnopengl.com.
    
