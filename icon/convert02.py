from PIL import Image

bits    = 3
sizex   = 16
sizey   = 16
range   = 16
padx    = sizex * bits
pady    = sizey * bits


gamma = [0,0,1,2,3,5,9,15]
matrix =  ( 
            0.70 ,0    ,0.3  ,0,
            0    ,0.70 ,0.3 ,0,
            0    ,0    ,0    ,0
            )
 
def to_array(filename,name):
    im = Image.open("%s" % filename)
    parts = im.split()
    im = Image.merge("RGB",(parts[0],parts[1],parts[2]))
    out = im.convert("RGB",matrix)
    doc='''uint8_t  %s[ 16 * 16] = {\n''' % name
    data    = out.tostring()
    for i in xrange(0,sizex * sizey * bits,bits):
        red = gamma[ord(data[i]) / 32] 
        green = gamma[ord(data[i+1]) / 32]
        if i%(16*bits)==0 and i:
            doc +="\n"
        v = ( ( red  << 4 ) & 0xf0) | ( green &  0x0f )   
        doc += "0x%02x," % v
    doc = doc[:-1] + "\n};"
    print doc

to_array("man.png","fb1")
to_array("man2.png","fb2")

