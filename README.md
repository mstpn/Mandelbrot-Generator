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

## Sample Outputs
![blackred-yellowgreen](https://user-images.githubusercontent.com/57071649/155384566-98a4220c-4ce2-402e-b455-abfbc4234b91.jpg)
![blackgreen-yellowred](https://user-images.githubusercontent.com/57071649/155384628-0c1481c2-0eb7-47a0-afed-87e5663ac92a.jpg)
![dracula-bgpurp_30-greenpink](https://user-images.githubusercontent.com/57071649/155384692-2db85950-2809-4927-b3c6-5f740e4cca8a.jpg)
