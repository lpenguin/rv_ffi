### Data

* data/height.png - height texture in png8
* data/meta.png - metain png8
* data/table.png - height texture in png32, r = begin_color_offset, g = end_color_offset

### Depends:

* libSDL2
* libSDL2_image
* libGLESv2
* vange_rs/lib/ffi

### CMake

Set `-DRUST_VANGERS_ROOT=<path>`, where `path` is directory containing `libvangers_ffi.a`

### Compile/Run

* Edit `height_path`, `meta_path`, `table_path` 
* Compile
* Set exe working dir as Vangers game data dir
* Run
