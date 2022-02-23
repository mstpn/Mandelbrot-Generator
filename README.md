# Mandelbrot-Generator
Utilize the ImageMagick library to generate an image of the Mandelbrot set.

## Build
Requires the installation of ImageMagick/Magick++. Follow the instructions at https://imagemagick.org/Magick++/ to get set up. Once ImageMagick is installed, run `make all` to build the program. `make clean` removes the build files.

## Usage
Run with `mandel`. <br>
Input recommendations:

| Quality | Size | Iterations | File Size | Est. Time |
| --- | --- | --- | --- | --- |
|	Quick	  |   500px    |   100	  |    ~125 KiB	 | Instantaneous |
|	Standard  |   1000px	| 250	  |    ~500 KiB  |  5 s |
|	Better-    |  5000px   |   100	   |   ~6 Mib    |  1 min |
|	Better+     | 5000px   |   500     |     ~7 Mib   |   2.5 min |
|	Mandelbrot  | 20000px   |  250      |    ~20 Mib	|  30+ min |

- Quick: Instantaneous, small file size. Still viewable. Looks best
zoomed out.

- Standard: Recommended unless you need the small file size of quick.
Gives you some detail when zoomed in. Better expression of coloring,
clearer lines.

- Better-: Better then standard due to resolution. Much sharper image,
detail of set "better".

- Better+: More detail and better coloring when compared with Better-.
Similar file size. Cost is that it takes 2.5x longer to calculate.

- Mandelbrot: The way it is meant to be seen. Long calculation. Reducing
iterations can help. You are are waiting 15+ minutes regardless,
so take the time to get the full experience of a better set depth.
This may cause you to hit the caps of system storage/resource 
allocation. Do so at your own risk.
