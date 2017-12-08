# Stealth Converter

![](https://github.com/tauril/stealth-converter/blob/master/resources/matrix_bypassed.gif)

![](https://github.com/tauril/stealth-converter/blob/master/resources/blocked_vs_allowed.png)

Video encoder to assemble standard videos into 360° videos.

The challenge was to break YouTube copyright infringement by embedding classic
videos into 360 ones and upload it on YouTube.

As it took a lot of time to process videos, we designed the executable with
three parts to be able to distribute the computations on multiple computers.

The original video is split then each sub-video is sent to a different
machine that will do the conversion process. Finally the sub-videos are
reassembled into a whole.

To further increase performance we parallelized the conversions of the frames
when a configurable option is activated.

## Application

### Disclaimer

This repository is the fruit of our research.
In no event shall the authors of this project be liable for any use
that would not respect the terms and conditions of YouTube's or any
other website.
We decline responsability for the use of our software for law infringement.

### Having fun with YouTube's Content ID

We made a talk about a possible application of our project for the
"nuit de l'info" hosted by GConfs on the 12/08/2017 at EPITA.
* [The talk is available here.](https://youtu.be/Iv0XtzvmJqs?t=4h41m40s)
* [The slides are available here.](slides/slides.md)

## Getting Started

The project is not installable, you however can get the executable
(giving that you have the adequates prerequisites) by simply
cloning the repository and running cmake.

### Prerequisites

- OpenCV      3.2.0
- Cmake       3.5.1
- ffmpeg      2.8.11
- libavfilter 5.40.101
- libavformat 56.40.101
- libavcodec  56.60.100

### Example

```
git clone https://github.com/Tauril/stealth-converter.git
cd stealth-converter
./configure -DPARALLEL=[0,1]
cmake .
make
```

## Using the program

The parallel option is to be set with the configure

```
./configure -DPARALLEL=[0,1]
```

### Usage

help:
```
./stealth-converter [-h|--help]
```

split:
```
./stealth-converter [-s|--split] <Video_Path>
```

convert:
```
./stealth-converter [-c|--convert] <Video_Path>
```

reassemble:
```
./stealth-converter [-r|--recombine] <Sub_videos>
```

## TODO

* [ ] Add option to choose the conversion face
* [ ] Add option to generate noise

## Authors

* **Guillaume Marques** - *Tauril*
* **Loic Banet** - *banetl*

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
