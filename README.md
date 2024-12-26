## The famous Kvantum is on windows!

Remember you can **not** mix different environment together (by which I mean MSVC or mingw). The apps have to be compiled with the same compiler that compiles kvantum.

Configuration can be found under `~\.config\Kvantum`

# Installation

## Kvantum only
* Download the latest release and extract in a folder.
* Set env variable `QT_STYLE_OVERRIDE=kvantum`.
* Set env variable `KVANTUM_DATA=<path to kvantum>/share`
* Go to each app path and create a soft link from `<kvantum path>/styles` to `<your app path>\styles`
* **Notice: if selected style is not applied to certain apps, its most probably that the app hardcoded its own style. You will have to compile from source, download the source, set the content of `Kvantum\styles-hack\kvantum.json` to the style that the application is using, compile and replace the original style with `styles/kvantum-hack.dll` in the app directory styles. (My release version contains hack for breeze theme, which is default to kde apps)**

## If you have [qtct](https://github.com/RichardLuo0/qtct-for-windows) installed
* Download the latest release and extract in a folder.
* Copy the content of `<kvantum path>/style` to `<qtct path>/style`
* Create a soft link from `<qtct path>/styles` to `<kvantum path>/styles` and for `platformthemes` as well.
* Set env variable `KVANTUM_DATA=<path to kvantum>/share`
* Set env variable `QT_STYLE_OVERRIDE=qtct`.
* Open qtct and set kvantum as current style.
