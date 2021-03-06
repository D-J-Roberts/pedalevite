# Source code directory

Sub-directory content. Some libraries are external dependencies and come from other project or live on their own:

- **`asiosdk2`**: A library to use ASIO audio drivers. For the Windows emulator. *(external)*
- **`conc`**: A library for lock-free communications. *(external)*
- **`Eigen`**: The [Eigen](http://eigen.tuxfamily.org) linear algebra library, v3.3.8. Installed just by copying the `Eigen` folder. *(external)*
- **`ffft`**: Fast Fourier direct and inverse transforms for arrays of real numbers. *(external)*
- **`fontgen`**: A vectorial to bitmap font converter.
- **`fstb`**: A basic toolbox containing essential utility functions and classes. *(external)*
- **`hiir`**: A library to perform fast 2x-downsampling and 2x-upsampling, as well as π/2 phase shifts (Hilbert transforms). *(external)*
- **`jack`**: A library to use JACK audio connection system on Linux. Not used any more. *(external)*
- **`lal`**: A minimalist linear algebra library. *(external)*
- **`mfx`**: The main Pédale Vite source code.
- **`piapi2vst`**: A library to turn Pédale Vite internal plug-ins into VST plug-ins.
- **`pvvid2raw`**: Decodes `.pvvid` video capture files into raw 8-bit 4:4:4 Y’Cb’Cr’ data
- **`pvvst`**: Converted plug-ins using `piapi2vst` with their minimalist glue code.
- **`test`**: Some test code.
- **`vstsdk24`**: A library to build VST plug-ins. *(external)*

Generally, one sub-directory = one namespace, one header = one class.

And everything starts from `main.cpp`.
