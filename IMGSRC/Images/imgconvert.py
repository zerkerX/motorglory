#!/usr/bin/python3
# -*- coding: utf-8 -*-
import os
import struct
import pdb

from PIL import Image
from argparse import ArgumentParser

PCX_HEADER = b'4b6h48sxb2h58x'

# Common
BLACK = 0

#B&W
BW_WHITE = 0x1

# Yellow Palette
YELLOW = 0x3
RED = 0x2
GREEN = 0x1

# Cyan Palette
WHITE = 0x3
MAGENTA = 0x2
CYAN = 0x1

RLE_OFFSET = 192
MAX_NON_RLE = 127
RLE_MAX = 255-192

# EGA Palette to populate the header
EGA_PAL = b'\0\0\0UUU\xaa\xaa\xaa\xff\xff\xff\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0'


def isbw(im):
    # Just go by image dimensions. MGS4 doesn't support B&W mode for anything
    # but full screen images.
    return (im.width == 640 and im.height == 200)
    
    # Alternate algorithm below, but unsafe. Probably should just be
    # a command-line option
    #~ if len(im.getcolors()) == 2:
        #~ if im.getbands()[0] == '1':
            #~ return True
        #~ elif im.getbands()[0] == 'P':
            #~ return (im.getpalette()[:3] == [0, 0, 0] and 
                #~ im.getpalette()[3:6] == [255, 255, 255])
            
        #~ else:
            #~ return False
    #~ else:
        #~ return False


def getimgbyte(im, x, y, bpp):
    byte = 0
    if bpp == 1:
        # Assume indexed, where black = 0 and white = 1
        # Or grayscale. Use a bit mask to support both.
        byte = ((im.getpixel( (x, y) ) & 0x01) << 7) \
                | ((im.getpixel( (x+1, y) ) & 0x01) << 6) \
                | ((im.getpixel( (x+2, y) ) & 0x01) << 5) \
                | ((im.getpixel( (x+3, y) ) & 0x01) << 4) \
                | ((im.getpixel( (x+4, y) ) & 0x01) << 3) \
                | ((im.getpixel( (x+5, y) ) & 0x01) << 2) \
                | ((im.getpixel( (x+6, y) ) & 0x01) << 1) \
                |  (im.getpixel( (x+7, y) ) & 0x01)
    else:
        for delta in range(4):
            pixel = im.getpixel( (x + delta, y) )
            if type(pixel) != type(tuple()):
                # Paletted image. Look up actual value:
                pixel = ( im.getpalette()[3*pixel],
                        im.getpalette()[3*pixel+1],
                        im.getpalette()[3*pixel+2] )
            (R,G,B) = pixel[:3]
            
            if R > 128 and G > 128 and B > 128:
                cgapixel = WHITE
            elif R < 128 and G > 128 and B > 128:
                cgapixel = CYAN
            elif R > 128 and G < 128 and B > 128:
                cgapixel = MAGENTA
            elif R > 128 and G > 128 and B < 128:
                cgapixel = YELLOW
            elif R > 128 and G < 128 and B < 128:
                cgapixel = RED
            elif R < 128 and G > 128 and B < 128:
                cgapixel = GREEN
            else:
                cgapixel = BLACK
            
            byte |= cgapixel << (6 - delta * 2)
            
    return byte


def rle_encode(indata):
    outdata = []
    i = 0
    while i < len(indata):
        runlength = 1
        while i+runlength < len(indata) \
                and indata[i] == indata[i+runlength] \
                and runlength < RLE_MAX:
            runlength += 1
            
        if runlength > 1 or indata[i] > MAX_NON_RLE:
            outdata.append(runlength + RLE_OFFSET)
        
        outdata.append(indata[i])
        i += runlength
        
    return outdata



def convert_pcx(inname, outname):
    im = Image.open(inname)
    bpp = 1 if isbw(im) else 2
    ppb = (8//bpp)
    imgdata = []
    #pdb.set_trace()
    for y in range( im.height ):
        for x in range( im.width//ppb ):
            imgdata.append(getimgbyte(im, x*ppb, y, bpp))
            
    rledata = rle_encode(imgdata)
    with open(outname, 'wb') as outfile:
        outfile.write(struct.pack(PCX_HEADER,
            10, # Manufacturer
            5, # Version
            1, # Encoding
            bpp,
            1, 1, # Top left corner
            im.width, im.height,
            im.width, im.height, # Resolution
            EGA_PAL,
            1, # Color planes
            im.width//4, # Bytes per line
            0 # Palette type
            ) )
        outfile.write(bytes(rledata))


def convert_raw(inname, outname):
    im = Image.open(inname)
    bpp = 1 if isbw(im) else 2
    ppb = (8//bpp)
    imgdata = []
    #pdb.set_trace()
    for y in range( im.height ):
        for x in range( im.width//ppb ):
            imgdata.append(getimgbyte(im, x*ppb, y, bpp))
            
    with open(outname, 'wb') as outfile:
        outfile.write(bytes(imgdata))
    


if __name__ == '__main__':
    parser = ArgumentParser(description='Converts a specified image to 4-color PCX')
    parser.add_argument('filename', help='Image to convert', nargs='+')
    parser.add_argument('-p', '--pcx', help='Convert to PCX file', action='store_true')

    args = parser.parse_args()
    for pngfile in args.filename:
        (base, ext) = os.path.splitext(pngfile)
        if args.pcx:
            convert_pcx(pngfile, base + ".pcx")
        else:
            convert_raw(pngfile, base)

