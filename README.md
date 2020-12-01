# Photos

> Minimal photo application for viewing images and doing light edits

## Prerequisites

In order to build this project, you'll need to have Qt 5 and OpenCV 4
installed on your system. Once you've done that, you can use Qt's `qmake`
tool in order to build the project. `qmake` is a Makefile generation tool,
similar to a tool like CMake but more Qt specific. Furthermore, `qmake` relies
on `.pro` files to configure the Makefile that is to be generated.

## Building

Once you have the prerequisites installed, you should have `qmake` avaiable.
We can generated the `Makefile` using `qmake` and our project file `photos.pro`.
Then, we can build the project by simply running `make`.

```
$ qmake photos.pro
$ make
```

This will generate a `build` folder inside of `src`.

## Running

To run the application, we can run `./src/build/photos` after we've run the build commands.
This will open the GUI application. Alternatively, we could also give it a file path argument to an image which will then load that image immediately when the application is opened.

## License

This project is licensed under the [MIT](./LICENSE) license.

This project also uses Qt 5 and OpenCV 4. Qt 5 is used under the LGPL
license and OpenCV is licensed under the 3-clause BSD license for versions
<= 4.4.0 and the Apache 2 license for versions >= 4.5.0. Neither of these
projects are included in the source code of this project. It is expected
that the user has them installed on the given system.