## The famous Kvantum is on windows!

Remember you can **not** mix different environment together (by which I mean MSVC or mingw). The apps have to be compiled with the same compiler that compiles kvantum.

Configuration can be found under `~\.config\Kvantum`

# Installation

## Kvantum only
* Download the latest release and extract in a folder.
* Set env variable `QT_STYLE_OVERRIDE=kvantum`.
* Set env variable `KVANTUM_DATA=<path to kvantum>/share`
* go to each app path and create a soft link from `<kvantum path>/styles-qt*` to `<your app path>\styles`
* **Notice: if selected style is not applied to certain apps, its most probably that the app hardcoded its own style. You will have to compile from source, download the source, set the content of `Kvantum\style-hack\kvantum.json` to the style that the application is using, compile and replace the original style with `styles/kvantum-hack-qt*.dll` in the app directory styles. (My release version contains hack for breeze theme, which is default to kde apps)**

## If you have [qt6ct](https://github.com/RichardLuo0/qt6ct-for-windows) or [qt5ct](https://github.com/RichardLuo0/qt5ct-for-windows) installed
* Download the latest release and extract in a folder.
* Move the content of `style-qt5` to `<qt5ct path>/style`, do the same for `style-qt6`
* Set env variable `KVANTUM_DATA=<path to kvantum>/share`
* Set env variable `QT_STYLE_OVERRIDE=qt5ct`.
* Open qt*ct and set kvantum as current style.
