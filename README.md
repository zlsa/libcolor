# libcolor

This is a header-only C++ library for between different colorspaces. The type of a color value defines which colorspace the value lives in.

## Implemented colorspaces

The following colorspace conversions are implemented. In all cases the alpha value is untouched. Any hue component is returned in radians from 0 to 2&pi;. Other values goes from zero to one.


| From\To 	| HCYA 	| HCIA 	| HSLA 	| HSVA 	| RGBA 	| sRGBA 	| XYZA 	
|---------	|------	|------	|------	|------	|------	|-------	|------	
| HCYA   	|      	| No   	| No   	| No   	| Yes  	| No    	| No   	
| HCIA    	| No   	|      	| No   	| No   	| Yes  	| No    	| No   	
| HSLA    	| No   	| No   	|      	| No   	| No   	| Yes   	| No   	
| HSVA    	| No   	| No   	| No   	|      	| No   	| Yes   	| No   	
| RGBA    	| Yes  	| Yes  	| No   	| No   	|      	| Yes   	| Yes  	
| sRGBA   	| No  	| No   	| Yes  	| Yes  	| Yes  	|       	| No   	
| XYZA    	| No   	| No   	| No   	| No   	| Yes  	| No    	|      	

To convert from XYZA to HSVA you would do the following:

    using namespace Color;
    auto hsva=static_cast<HSVA>( static_cast<SRGBA>( static_cast<RGBA>(color_in)));
    
The reason for this three-step conversion is to avoid possible ambiguities and errors.

### HCY'A (Hue Chroma Luma Alpha)
This is a perceptual colorspace. The hue is defined identically to HSL and HSV, but the lightness uses the ITU-709 luma weights

### HCIA (Hue Chroma Intensity Alpha)
This colorspace uses the same formulae for hue and chroma as HCY'A, but uses linear input instead of sRGB values. The intensity is the average of all color channels.

### HSLA (Hue Saturation Lightness Alpha)
This is the colorspace used in Windows color picker dialog.

### HSVA (Hue Saturation Value Alpha)
This is the colorspace used in the GTK+ color picker dialog. 

### RGBA (Red Green Blue Alpha)
This is the linear RGB space.

### sRGBA (standard Red Green Blue Alpha)
This is the RGB space used for most images on the Internet.

### XYZA (CIE)
This colorspace is sometimes used as an intermediate step between reallity and RGB

### Y'CbCr (Luma Chroma_b Chroma_r)
This colorspace is used with photographic compression and video. This implementation uses the ITU-709 weights.
