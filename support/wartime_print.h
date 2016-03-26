/** WarTime-Project On GitHub
*   https://github.com/Kiritow/WarTime-Project.git
*/

/** Basic Drawing Interface
*   Provider: HC TECH
*   Platform Provider: LinuxWindowSDK Platform
*/

#ifndef WARTIME_HEAD_PRINT
#define WARTIME_HEAD_PRINT

/// Enum class of supported color.
enum Color {black=0,red,green,yellow,blue,purple,lightblue,white};

/// Changing color synchronously. Right now please don't use it too much because it may be changed soon.
int synchronizedChangeColor(Color ForegroundColor,Color BackgroundColor);

/** LinuxWindowSDK Platform
*   Screen is REAL hardware-based class. It reflects what is really being or will be shown on screen right now.
*   Surface is a software-based class. Printable objects can be added to a standalone surface class. Eventually submitted surfaces will be processed into a real screen object.
*   
*   Printable Objects --> Surface --> surface.submit() --> screen --> Basic print APIs --> Real graphic
*/

/// ATTENTION: At present both Screen and Surface class are unimplemented.
class Screen;
class Surface;

#endif /** End of WARTIME_HEAD_PRINT */
